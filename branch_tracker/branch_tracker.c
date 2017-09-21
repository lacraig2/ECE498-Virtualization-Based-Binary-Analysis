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

int before_block_exec(CPUState *cpu, TranslationBlock *tb);

int before_block_exec(CPUState *cpu, TranslationBlock *tb) {
	//char *base = panda_parse_string(args, "base", NULL);
	#if defined (TARGET_I386)
		panda_arg_list *args = panda_get_args("llvm_trace");
		uint32_t pid = panda_parse_uint32(args, "PID", 0);
		int cr3 = (int) panda_current_asid(cpu);
		if (cr3 == pid){
			CPUArchState *env = (CPUArchState*)cpu->env_ptr;
			uint64_t count = rr_get_guest_instr_count();
			int eax = (int)env->regs[R_EAX];
			printf("INSTR: 0x%" PRIx64 " EAX: 0x%" PRIx32 " CR3: 0x%" PRIx32 "\n", count, eax, cr3);
		}
	#endif
    return 0;
}

bool init_plugin(void *self) {
    panda_cb pcb = { .before_block_exec = before_block_exec };
    panda_register_callback(self, PANDA_CB_BEFORE_BLOCK_EXEC, pcb);
    return true;
}

void uninit_plugin(void *self) { }
