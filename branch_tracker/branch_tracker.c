/* PANDABEGINCOMMENT
 * 
 * Authors:
 *  Tim Leek               tleek@ll.mit.edu
 *  Ryan Whelan            rwhelan@ll.mit.edu
 *  Joshua Hodosh          josh.hodosh@ll.mit.edu
 *  Michael Zhivich        mzhivich@ll.mit.edu
 *  Brendan Dolan-Gavitt   brendandg@gatech.edu
 * 
 * This work is licensed under the terms of the GNU GPL, version 2. 
 * See the COPYING file in the top-level directory. 
 * 
PANDAENDCOMMENT */
// This needs to be defined before anything is included in order to get
// the PRIx64 macro
#define __STDC_FORMAT_MACROS

// Choose a granularity for the OSI code to be invoked.
#define INVOKE_FREQ_PGD
//#define INVOKE_FREQ_BBL

#include "../wintrospection/wintrospection.h"
#include <inttypes.h>
#include "panda/plugin.h"
#include "osi/osi_types.h"
#include "osi/osi_ext.h"


// #define ESP ((CPUX86State *)((CPUState *)env->env_ptr))->regs[R_ESP]
// #define EAX ((CPUArchState*)env->env_ptr)->regs[R_EAX]
// #define EBP ((CPUArchState*)env->env_ptr)->regs[R_EBP]

bool init_plugin(void *);
void uninit_plugin(void *);

int vmi_pgd_changed(CPUState *cpu, target_ulong old_pgd, target_ulong new_pgd);
int before_block_exec(CPUState *cpu, TranslationBlock *tb);


int before_block_exec(CPUState *cpu, TranslationBlock *tb) {
    // int i;
    OsiProc *current = get_current_process(cpu);
	// if (!strcmp("wget", current->name)){
		// printf("process: %s\n", current->name);
	// }

	if (!strcmp("wget", current->name)){
		printf("process: %s\n", current->name);
		CPUArchState *env = (CPUArchState*)cpu->env_ptr;
		OsiPage *pages = current->pages;
		int high_addr = pages->start;
		int low_addr = high_addr+pages->len;
		int i;
		int EBP = (int)env->regs[R_EBP];
		int EAX = (int)env->regs[R_EAX];
		int ESP = (int)env->regs[R_ESP];
		int size = (ESP-EBP)*sizeof(char);

		unsigned char *buf = (unsigned char *) malloc(len*sizeof(char));
		int err = panda_virtual_memory_rw(cpu, EBP, buf, size, 0);
		if (err==-1){
			printf("Couldn't read memory");
			return 0;
		}
		for (i=EBP;i<=ESP; i++){
			int value = val[i];
			if (value > low_addr && value < high_addr){
				printf("%d %d %d", i, val[i], val[val[i]]);
			}else{
				printf("%d %d", i, buf[i]);
			// ASDFASDSADFSDF
			}
		}
		printf("process name: %s\n", current->name);
	}

    // OsiProc *current = get_current_process(cpu);
    // printf("Current process: %s PID:" TARGET_FMT_ld " PPID:" TARGET_FMT_ld "\n", current->name, current->pid, current->ppid);

    // OsiModules *ms = get_libraries(cpu, current);
    // if (ms == NULL) {
    //     printf("No mapped dynamic libraries.\n");
    // }
    // else {
    //     printf("Dynamic libraries list (%d libs):\n", ms->num);
    //     for (i = 0; i < ms->num; i++)
    //         printf("\t0x" TARGET_FMT_lx "\t" TARGET_FMT_ld "\t%-24s %s\n", ms->module[i].base, ms->module[i].size, ms->module[i].name, ms->module[i].file);
    // }

    // printf("\n");

    // OsiProcs *ps = get_processes(cpu);
    // if (ps == NULL) {
    //     printf("Process list not available.\n");
    // }
    // else {
    //     printf("Process list (%d procs):\n", ps->num);
    //     for (i = 0; i < ps->num; i++)
    //         printf("  %-16s\t" TARGET_FMT_ld "\t" TARGET_FMT_ld "\n", ps->proc[i].name, ps->proc[i].pid, ps->proc[i].ppid);
    // }

    // printf("\n");

    // OsiModules *kms = get_modules(cpu);
    // if (kms == NULL) {
    //     printf("No mapped kernel modules.\n");
    // }
    // else {
    //     printf("Kernel module list (%d modules):\n", kms->num);
    //     for (i = 0; i < kms->num; i++)
    //         printf("\t0x" TARGET_FMT_lx "\t" TARGET_FMT_ld "\t%-24s %s\n", kms->module[i].base, kms->module[i].size, kms->module[i].name, kms->module[i].file);
    // }

    // printf("\n-------------------------------------------------\n\n");

    // Cleanup
    // free_osiproc(current);
    // free_osiprocs(ps);
    // free_osimodules(ms);

    return 0;
}

int vmi_pgd_changed(CPUState *cpu, target_ulong old_pgd, target_ulong new_pgd) {
    // tb argument is not used by before_block_exec()
    return before_block_exec(cpu, NULL);
}

bool init_plugin(void *self) {
#if defined(INVOKE_FREQ_PGD)
    // relatively short execution
    panda_cb pcb = { .asid_changed = vmi_pgd_changed };
    panda_register_callback(self, PANDA_CB_ASID_CHANGED, pcb);
#else
    // expect this to take forever to run
    panda_cb pcb = { .before_block_exec = before_block_exec };
    panda_register_callback(self, PANDA_CB_BEFORE_BLOCK_EXEC, pcb);
#endif

    if(!init_osi_api()) return false;

    return true;
}

void uninit_plugin(void *self) { }


// /* PANDABEGINCOMMENT
//  * 
//  * Authors:
//  * Luke Craig 			craigla@rose-hulman.edu
//  * 
// PANDAENDCOMMENT */

// #ifndef __STDC_FORMAT_MACROS
// #define __STDC_FORMAT_MACROS
// #endif

// #ifdef NDEBUG
// #undef NDEBUG
// #endif

// #include "panda/plugin.h"
// #include "panda/tcg-llvm.h"
// #include "panda/plugin_plugin.h"
// #include "panda_hypercall_struct.h"

// #include <llvm/PassManager.h>
// #include <llvm/PassRegistry.h>
// #include <llvm/Analysis/Verifier.h>
// #include <llvm/ExecutionEngine/ExecutionEngine.h>
// #include <llvm/Transforms/IPO/PassManagerBuilder.h>


// extern "C" {
// #include <sys/time.h>
// #include "callstack_instr/callstack_instr.h"
// #include "callstack_instr/callstack_instr_ext.h"
// #include <inttypes.h>
// #include "panda/rr/rr_log.h"
// #include "panda/plog.h"
// #include "panda/addr.h"

// #include "osi/osi_types.h"
// #include "osi/osi_ext.h"

// // this provides the fd resolution magic

// #include "osi_linux/osi_linux_ext.h"

// #include "wintrospection/wintrospection.h"
// #include "wintrospection/wintrospection_ext.h"

// #include "syscalls2/gen_syscalls_ext_typedefs.h"

// bool init_plugin(void *);
// void uninit_plugin(void *);
// int before_block_exec(CPUState *cpu, TranslationBlock *tb);
// }

// // #include "../wintrospection/wintrospection.h"


// // #ifdef TARGET_I386
// // #define EAX ((CPUArchState*)cpu->env_ptr)->regs[R_EAX]
// // #define EBX ((CPUArchState*)cpu->env_ptr)->regs[R_EBX]
// // #define ECX ((CPUArchState*)cpu->env_ptr)->regs[R_ECX]
// // #define EDI ((CPUArchState*)cpu->env_ptr)->regs[R_EDI]
// // #define ESP ((CPUArchState*)cpu->env_ptr)->regs[R_ESP]
// // #define EBP ((CPUArchState*)cpu->env_ptr)->regs[R_EBP]
// // #endif
// int before_block_exec(CPUState *cpu, TranslationBlock *tb) {
// 	// questions: how do I copy data. 
// 	//Which direction does the stack go in x86?

// 	OsiProc *current = (OsiProc) get_current_process(cpu);
// 	if (!strcmp("wget", current->name)){
// 		// OsiPage *pages = current->pages;
// 		// int high_addr = pages->start;
// 		// int low_addr = high_addr+pages->len;
// 		// int i;
// 		// int size = (ESP-EBP)*sizeof(char);
// 		// unsigned char *buf = (unsigned char *) malloc(len*sizeof(char));
// 		// int err = panda_virtual_memory_rw(cpu, EBP, buf, size, 0);
// 		// if (err==-1){
// 			// printf("Couldn't read memory");
// 			// return 0;
// 		// }
// 		// for (i=EBP;i<=ESP; i++){

// 			// int value = val[i];
// 			// if (value > low_addr && value < high_addr){
// 				//probably a pointer
// 				// printf("%d %d %d", i, val[i], val[val[i]]);
// 			// }else{
// 			// printf("%d %d", i, buf[i]);
// 			// ASDFASDSADFSDF
// 			// }
// 		// }
// 		printf("process name: %s\n", current->name);
// 	}
//     return 0;
// }



// bool init_plugin(void *self) {
//     panda_cb pcb = { .before_block_exec = before_block_exec};//, .replay_handle_packet=replay_handle_packet};
//     panda_register_callback(self, PANDA_CB_BEFORE_BLOCK_EXEC, pcb);
//     panda_require("osi");
//     assert(init_osi_api());
//     // if (panda_os_type == OST_LINUX) {
//         // panda_require("osi_linux");
//         // assert(init_osi_linux_api());

//         // PPP_REG_CB("syscalls2", on_sys_open_enter, linux_open_enter);
//         // PPP_REG_CB("syscalls2", on_sys_read_enter, linux_read_enter);
//         // PPP_REG_CB("syscalls2", on_sys_read_return, linux_read_return);
//         // PPP_REG_CB("syscalls2", on_sys_pread64_enter, linux_pread_enter);
//         // PPP_REG_CB("syscalls2", on_sys_pread64_return, linux_pread_return);
//     // }
//     panda_enable_precise_pc();
//     return true;
// }

// void uninit_plugin(void *self) { }