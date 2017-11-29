/* PANDABEGINCOMMENT
 * 
 * Authors:
 *  Luke Craig      craigla@rose-hulman.edu
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

#include <inttypes.h>
#include <stdio.h>
// #include "../wintrospection/wintrospection.h"
#include "panda/plugin.h"
#include "osi/osi_types.h"
#include "osi/osi_ext.h"
#include <stdbool.h>
// #include "cpu.h"


// #define ESP ((CPUX86State *)((CPUState *)env->env_ptr))->regs[R_ESP]
// #define EAX ((CPUArchState*)env->env_ptr)->regs[R_EAX]
// #define EBP ((CPUArchState*)env->env_ptr)->regs[R_EBP]

bool init_plugin(void *);
void uninit_plugin(void *);

// int vmi_pgd_changed(CPUState *cpu, target_ulong old_pgd, target_ulong new_pgd);
int before_block_exec(CPUState *cpu, TranslationBlock *tb);
int virt_mem_w(CPUState *cpu, target_ulong pc, target_ulong addr, target_ulong size, void *buf);
// FILE *fp;

int virt_mem_w(CPUState *cpu, target_ulong pc, target_ulong addr, target_ulong size, void *buf){
    #ifdef TARGET_I386
    // if (get_current_process(cpu) != null && !panda_in_kernel(cpu)){
    // OsiProc *current = get_current_process(cpu);
    // // printf("%s\n",current->name);
    // if (!strcmp("vuln", current->name)){
    // if (addr){
    if (addr){
      printf("write: 0x%lx, %s\n", (uint64_t) addr, (char*) buf);
    }
    // }
    // }
    // }else{
        // printf("kernel %s\n", (char*) buf);
    // }
    #endif
    return 0;
}
// unsigned char* old_buffer;

int before_block_exec(CPUState *cpu, TranslationBlock *tb) {
    // int i;	
    
	// if (!strcmp("wget", current->name)){
		// printf("process: %s\n", current->name);
	// }
    #ifndef TARGET_I386
    printf("NOT I386");
    #endif

    #ifdef TARGET_I386  
    OsiProc *current = get_current_process(cpu);
    // printf("%s\n",current->name);
	if (!strcmp("vuln", current->name)){
		CPUArchState *env = (CPUArchState*)cpu->env_ptr;
		// int EAX = (int)env->regs[R_EAX];
		// printf("process: %s EAX: %d\n", current->name,EAX);
      //   if (current->pages){
		    // OsiPage *page = current->pages;
      //       int addr = page->start;
      //       int len = page->len;
      //       printf("memaddr: %u len: %u\n", addr, len);
      //       if (len > 0){
      //           unsigned char *buf = (unsigned char *) malloc(len*sizeof(char));
      //           int err = panda_physical_memory_rw(addr, buf, len, 0);
      //           if (err==-1){
      //               printf("couldn't read memory.\n");
      //           }else{
      //               printf("buff_mem %s\n", buf);
      //           }

      //       }else{
      //           printf("len %u\n",len);
      //       }
      //   }else{
      //       printf("no pages\n");
      //   }
		// printf("got pages");
		
		// int i;
		int EBP = (int)env->regs[R_EBP];
		int ESP = (int)env->regs[R_ESP];
		int size = (EBP-ESP)*sizeof(char);
		printf("proc: %s EBP: %d ESP: %d size: %d\n", current-> name, EBP, ESP, size);
		// printf("got to this stage");
        if (size > 0){
		  unsigned char *buf = (unsigned char *) malloc(size*sizeof(char));
		  int err = panda_virtual_memory_rw(cpu, EBP, buf, size, 0);
          if (err==-1){
            printf("couldn't read memory.\n");
            return -1;
          }

          // OsiPage *page = current->pages;
          // const uint64_t start = (uint64_t) page->start;
          // const uint64_t len = (uint64_t) page->len;
          // printf("Got pages: %lu %lu\n", start,len);
          uint64_t count = rr_get_guest_instr_count();
          printf("count %lu\n", count);
          int i;
          for (i=0; i<size; i++){
            printf("addr: %u val: %u\n", i+EBP, buf[i]);
          }
          // old_buffer = buf;
        }else{
            printf("size %u\n", size);
        }

		// if (err==-1){
		// 	printf("Couldn't read memory");
		// 	return 0;
		// }
		// for (i=EBP;i<=ESP; i++){
		// 	int value = buf[i];
		// 	if (value > low_addr && value < high_addr){
		// 		printf("prolly a pointer: %d val: %d val: %d", i, buf[i], buf[buf[i]-low_addr]);
		// 	}else{
		// 	printf("addr: %d val:%d", i+EBP, value);
		// 	ASDFASDSADFSDF
		// 	}
		// }
		// printf("process name: %s\n", current->name);
	}
	#endif

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


bool init_plugin(void *self) {
    printf("init called\n");
    panda_enable_memcb();
    panda_cb pcb = {    .before_block_exec = before_block_exec,
        .virt_mem_before_write = virt_mem_w};
    panda_register_callback(self, PANDA_CB_BEFORE_BLOCK_EXEC, pcb);
    panda_register_callback(self,PANDA_CB_VIRT_MEM_BEFORE_WRITE,pcb);
    return true;
}

void uninit_plugin(void *self) {}