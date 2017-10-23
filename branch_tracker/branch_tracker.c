/* PANDABEGINCOMMENT
 * 
 * Authors:
 * Luke Craig 			craigla@rose-hulman.edu
 * 
PANDAENDCOMMENT */

#define __STDC_FORMAT_MACROS

#include "panda/plugin.h"
#include "panda/plugin_plugin.h"
#include "panda/common.h"
// #include "../wintrospection/wintrospection.h"
#include "osi/osi_types.h"
#include "osi/os_intro.h"
#include <inttypes.h>

// #ifdef TARGET_I386
// #define EAX ((CPUArchState*)cpu->env_ptr)->regs[R_EAX]
// #define EBX ((CPUArchState*)cpu->env_ptr)->regs[R_EBX]
// #define ECX ((CPUArchState*)cpu->env_ptr)->regs[R_ECX]
// #define EDI ((CPUArchState*)cpu->env_ptr)->regs[R_EDI]
// #define ESP ((CPUArchState*)cpu->env_ptr)->regs[R_ESP]
// #define EBP ((CPUArchState*)cpu->env_ptr)->regs[R_EBP]
// #endif

bool init_plugin(void *);
void uninit_plugin(void *);

int before_block_exec(CPUState *cpu, TranslationBlock *tb);

int before_block_exec(CPUState *cpu, TranslationBlock *tb) {
	// questions: how do I copy data. 
	//Which direction does the stack go in x86?

	OsiProc *current = get_current_process(cpu);
	if (!strcmp("wget", current->name)){
		// OsiPage *pages = current->pages;
		// int high_addr = pages->start;
		// int low_addr = high_addr+pages->len;
		// int i;
		// int size = (ESP-EBP)*sizeof(char);
		// unsigned char *buf = (unsigned char *) malloc(len*sizeof(char));
		// int err = panda_virtual_memory_rw(cpu, EBP, buf, size, 0);
		// if (err==-1){
			// printf("Couldn't read memory");
			// return 0;
		// }
		// for (i=EBP;i<=ESP; i++){

			// int value = val[i];
			// if (value > low_addr && value < high_addr){
				//probably a pointer
				// printf("%d %d %d", i, val[i], val[val[i]]);
			// }else{
			// printf("%d %d", i, buf[i]);
			// ASDFASDSADFSDF
			// }
		// }
		printf("process name: %s\n", current->name);
	}
    return 0;
}



bool init_plugin(void *self) {
    panda_cb pcb = { .before_block_exec = before_block_exec};//, .replay_handle_packet=replay_handle_packet};
    panda_register_callback(self, PANDA_CB_BEFORE_BLOCK_EXEC, pcb);
    // if (panda_os_type == OST_LINUX) {
        // panda_require("osi_linux");
        // assert(init_osi_linux_api());

        // PPP_REG_CB("syscalls2", on_sys_open_enter, linux_open_enter);
        // PPP_REG_CB("syscalls2", on_sys_read_enter, linux_read_enter);
        // PPP_REG_CB("syscalls2", on_sys_read_return, linux_read_return);
        // PPP_REG_CB("syscalls2", on_sys_pread64_enter, linux_pread_enter);
        // PPP_REG_CB("syscalls2", on_sys_pread64_return, linux_pread_return);
    // }
    panda_enable_precise_pc();
    return true;
}

void uninit_plugin(void *self) { }