/* PANDABEGINCOMMENT
 * 
 * Authors:
 * Luke Craig 			craigla@rose-hulman.edu
 * 
PANDAENDCOMMENT */

#define __STDC_FORMAT_MACROS

#include "panda/plugin.h"
#include <inttypes.h>

bool init_plugin(void *);
void uninit_plugin(void *);

int before_block_exec(CPUState *env, TranslationBlock *tb);

int before_block_exec(CPUState *env, TranslationBlock *tb) {
	uint64_t count = rr_get_guest_instr_count();
	printf("BEFORE BLOCK EXEC%" PRId64 "\n", count);
    return 0;
}

bool init_plugin(void *self) {
    panda_cb pcb = { .before_block_exec = before_block_exec };
    panda_register_callback(self, PANDA_CB_BEFORE_BLOCK_EXEC, pcb);
    return true;
}

void uninit_plugin(void *self) { }
