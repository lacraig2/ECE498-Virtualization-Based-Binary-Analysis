/* PANDABEGINCOMMENT
 * 
 * Authors:
 * Luke Craig 			craigla@rose-hulman.edu
 * 
PANDAENDCOMMENT */

#define __STDC_FORMAT_MACROS

#include "panda/plugin.h"
#include "../wintrospection/wintrospection.h"
#include <inttypes.h>

bool init_plugin(void *);
void uninit_plugin(void *);
int replay_net_transfer(CPUState *env, uint32_t type, uint64_t src_addr,
                            uint64_t dest_addr, uint32_t num_bytes);

int before_block_exec(CPUState *cpu, TranslationBlock *tb);

int before_block_exec(CPUState *cpu, TranslationBlock *tb) {
	//char *base = panda_parse_string(args, "base", NULL);
	#if defined (TARGET_I386)
		panda_arg_list *args = panda_get_args("branch_tracker");
		uint32_t pid = panda_parse_uint32(args, "PID", 0);
		int cr3 = (int) panda_current_asid(cpu);
		if (cr3 == pid || pid == -1){
			CPUArchState *env = (CPUArchState*)cpu->env_ptr;
			uint64_t count = rr_get_guest_instr_count();
			int eax = (int)env->regs[R_EAX];
			printf("INSTR: 0x%" PRIx64 " EAX: 0x%" PRIx32 " CR3: 0x%" PRIx32 "\n", count, eax, cr3);
		}
	#endif
    return 0;
}

int replay_net_transfer(CPUState *env, uint32_t type, uint64_t src_addr,
                            uint64_t dest_addr, uint32_t num_bytes){
	printf("NET TRANSFER\n")
	return 0;
}


bool init_plugin(void *self) {
    panda_cb pcb = { .before_block_exec = before_block_exec, .replay_net_transfer=replay_net_transfer};
    panda_register_callback(self, PANDA_CB_BEFORE_BLOCK_EXEC, pcb);
    panda_enable_precise_pc();
    return true;
}

void uninit_plugin(void *self) { }
