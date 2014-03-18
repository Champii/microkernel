/************************************************
*
* Micro Kernel
*
* - Program Loader Service_pid RPC
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <rpc.h>
#include                  <string.h>
#include                  <mm.h>
#include                  <unistd.h>
#include                  <get_argument.h>
#include                  <sys/syscall.h>

extern t_page_directory   *paging_dir;

// typedef struct        task
// {
//   int                 id;               // Process ID.
//   struct s_regs       regs;
//   unsigned            sleep_count;
//   unsigned            kernel_stack;
//   t_page_directory    *page_directory;  // Page directory.
//   t_mess              *mess_queue;
//   struct task         *next;            // The next task in a linked list.
// }                     t_task;

void                      prepare_pd(t_page_directory *pd)
{
  unsigned i;

  alloc_page(get_page((unsigned)((char *)pd), 1, paging_dir), 0, 1);
  alloc_page(get_page((unsigned)((char *)pd + 0x1000), 1, paging_dir), 0, 1);
  alloc_page(get_page((unsigned)((char *)pd + 0x2000), 1, paging_dir), 0, 1);

  for (i = (unsigned)((char *)pd + 0x3000); i < (unsigned)((char *)pd + (1024 * 0x1000) + 0x3000); i += 0x1000)
    alloc_page(get_page(i, 1, paging_dir), 0, 1);

}

void                      _create_as_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size)
{
  sender = sender;
  params = params;
  ret = ret;
  ret_size = ret_size;
  param_size = param_size;

  u64 new_pid = get_u64_arg(&params);
  unsigned *pid_split = (unsigned *)&new_pid;

  t_page_directory *new_pd = get_as_from_pid(pid_split[0]);

  prepare_pd(new_pd);

  sys_invlpg((void *)0x42424242);

  memset(new_pd, 0, 0x3000 + (1024 * 0x1000));
  // new_pd = new_pd;

  *(unsigned *)ret = 0;
  *ret_size = 4;

}
