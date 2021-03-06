/* PANDABEGINCOMMENT
 * libgen.h
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
#include <stdio.h>
#include <string.h>
#include "kernelinfo.h"

bool init_plugin(void *);
void uninit_plugin(void *);

int before_block_exec(CPUState *env, TranslationBlock *tb);

int total = 0;
int old_cr3 = 0;


int before_block_exec(CPUState *env, TranslationBlock *tb) {
	#ifdef TARGET_I386
	// make sure process is in kernel
	if (panda_in_kernel(env)){
		uint64_t cr3 = (uint64_t) panda_current_asid(env);
    if (cr3!=old_cr3){
  		uint64_t esp = (uint64_t)((CPUArchState*)env->env_ptr)->regs[R_ESP];
  		uint64_t page_val =  esp &  0b1111111111111111111111111111111111111111111111111110000000000000;
			printf("CR3: %"PRIx64" PAGE_VAL: %"PRIx64" ESP: %"PRIx64"\n", cr3,page_val,esp);
  		if (total < 1000){
  			// valid case
  			int size = 8192;
  			unsigned char *buf = (unsigned char *) malloc(size);
  		  int err = panda_virtual_memory_rw(env, page_val, buf, size, 0);
        if (err==-1){
         	printf("couldn't read memory.\n");
         	return -1;
        }

				uint32_t *int_buf = (uint32_t*) buf;
				uint32_t buf0 = int_buf[0];
				int task_struct_size = 8192; //it's less than this.
  			unsigned char *t_struct_buf = (unsigned char *) malloc(size);
  		  int err_t_struct = panda_virtual_memory_rw(env, (long unsigned int)buf0, t_struct_buf, task_struct_size, 0);
				if (err_t_struct==-1){
         	printf("couldn't read memory.\n");
         	return -1;
        }
				struct task_struct* task = (struct task_struct*) (void*) t_struct_buf;

				printf("buf0 addr: %lx\n", (long unsigned int)buf0);
				// printf("%p\n", (void*)task);
				printf("state: %lx\n", (long unsigned int)task->state);
				printf("stack pointer: %p\n", task->stack);
				printf("flags: %lx\n", (long unsigned int)task->flags);
        // uint64_t count = rr_get_guest_instr_count();

        // write path for file
        char str[256];
        strcpy(str, "/home/luke/ece498/files/file");
        sprintf(str+28, "%"PRIx64, cr3);
				//asdf
        // open file for writing
        FILE *fp = fopen(str, "w+");

        //write buffer to file
        fwrite(t_struct_buf, 1, size, fp);
        fclose(fp);
        free(buf);
        total +=1;
  		}
      old_cr3 = cr3;
    }
  }
		// printf("0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64" 0x%"PRIx64"\n",cr3, page_val,esp,ebp);
		// offset 1111111111111111111111111111111111111111111111111110000000000000
	#endif
  return 0;
}

bool init_plugin(void *self) {
    panda_cb pcb = { .before_block_exec = before_block_exec };
    panda_register_callback(self, PANDA_CB_BEFORE_BLOCK_EXEC, pcb);
    panda_enable_memcb();
    return true;
}

void uninit_plugin(void *self) { }
