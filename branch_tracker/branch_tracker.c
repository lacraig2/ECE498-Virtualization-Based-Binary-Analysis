/* PANDABEGINCOMMENT
 * 
 * Authors:
 * Luke Craig 			craigla@rose-hulman.edu
 * 
PANDAENDCOMMENT */

#define __STDC_FORMAT_MACROS

#include "panda/plugin.h"
#include "panda/plugin_plugin.h"
#include "../wintrospection/wintrospection.h"
#include <inttypes.h>

bool init_plugin(void *);
void uninit_plugin(void *);
void my_NtReadFile_enter(
        CPUState* env,
        target_ulong pc,
        uint32_t FileHandle,
        uint32_t Event,
        uint32_t UserApcRoutine,
        uint32_t UserApcContext,
        uint32_t IoStatusBlock,
        uint32_t Buffer,
        uint32_t BufferLength,
        uint32_t ByteOffset,
        uint32_t Key);
// int replay_handle_packet(CPUState *env, uint8_t *buf, int size,
                            // uint8_t direction, uint64_t old_buf_addr);

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
			printf("INSTR: 0x%" PRIx64 " EAX: 0x%" PRIx32 " CR3: 0x%" PRIx32 " in kerkel: %s\n", count, eax, cr3, panda_in_kernel(cpu) ? "true":"false");
		}
	#endif
    return 0;
}

void my_NtReadFile_enter(
        CPUState* env,
        target_ulong pc,
        uint32_t FileHandle,
        uint32_t Event,
        uint32_t UserApcRoutine,
        uint32_t UserApcContext,
        uint32_t IoStatusBlock,
        uint32_t Buffer,
        uint32_t BufferLength,
        uint32_t ByteOffset,
        uint32_t Key) {
   printf("NtReadFile(FileHandle=%x, Event=%x, UserApcRoutine=%x, "
                     "UserApcContext=%x, IoStatusBlock=%x, Buffer=%x, "
                     "BufferLength=%x, ByteOffset=%x, Key=%x)\n",
        FileHandle, Event, UserApcRoutine, UserApcContext,
        IoStatusBlock, Buffer, BufferLength, ByteOffset, Key);
}


// int replay_handle_packet(CPUState *env, uint8_t *buf, int size,
//                             uint8_t direction, uint64_t old_buf_addr){
// 	if (direction == PANDA_NET_RX){
// 		printf("RECEIVE NET TRANSFER\n");
// 	}
// 	return 0;
// }


bool init_plugin(void *self) {
    panda_cb pcb = { .before_block_exec = before_block_exec};//, .replay_handle_packet=replay_handle_packet};
    panda_register_callback(self, PANDA_CB_BEFORE_BLOCK_EXEC, pcb);
    PPP_REG_CB("syscalls2", on_NtReadFile_enter, my_NtReadFile_enter);
    panda_enable_precise_pc();
    return true;
}

void uninit_plugin(void *self) { }