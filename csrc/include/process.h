/************************************************
*
* Micro Kernel
*
* - Processes funcs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef                   __PROCESS_H__
# define                  __PROCESS_H__

# define                  PROG_LOADER_ID 1
# define                  PAGING_ID 2
# define                  IO_ID 3

typedef unsigned int      u32;
typedef unsigned long long u64;

int                       create_process(u64 *user_pid);
int                       run_process(void *task_struct, void *entry, void *stack, void *root_pt);
int                       kill(u64 pid);
int                       wait(u64 pid);
void                      sleep(u32 milli);

#endif                    /*__PROCESS_H__*/
