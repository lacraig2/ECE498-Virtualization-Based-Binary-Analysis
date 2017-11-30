/* PANDABEGINCOMMENT
 * 
 * Authors:
 *  Luke Craig 		craigla@rose-hulman.edu
 * 
 * This work is licensed under the terms of the GNU GPL, version 2. 
 * See the COPYING file in the top-level directory. 
 * 
PANDAENDCOMMENT */
#define __STDC_FORMAT_MACROS

#include "panda/plugin.h"
#include <inttypes.h>

bool init_plugin(void *);
void uninit_plugin(void *);

int before_block_exec(CPUState *env, TranslationBlock *tb);

int before_block_exec(CPUState *env, TranslationBlock *tb) {
	#ifdef TARGET_I386
	// make sure process is in kernel
	if (panda_in_kernel(env)){
		// uint64_t cr3 = (uint64_t) panda_current_asid(env);
		uint64_t esp = (uint64_t)((CPUArchState*)env->env_ptr)->regs[R_ESP];
		uint64_t ebp = (uint64_t)((CPUArchState*)env->env_ptr)->regs[R_EBP];
		uint64_t page_val =  esp &  0b1111111111111111111111111111111111111111111111111110000000000000;
		uint64_t page_val2 = ebp &  0b1111111111111111111111111111111111111111111111111110000000000000;
		printf("page_val %s equal page_val2\n", (page_val==page_val2)? "does": "does not");
		// printf("0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64"\n",cr3, page_val,esp,ebp);
		// offset 1111111111111111111111111111111111111111111111111110000000000000
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