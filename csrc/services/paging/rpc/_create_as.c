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


void                      _create_as_rpc(u64 sender, void *params, void **ret, unsigned *ret_size)
{
  sender = sender;
  params = params;
  ret = ret;
  ret_size = ret_size;

  u64 new_pid = get_u64_arg(&params);
  unsigned *pid_split = (unsigned *)&new_pid;

  t_page_directory *new_pd = get_as_from_pid(pid_split[0]);
  memset(new_pd, 0, sizeof(*new_pd));
  // new_pd = new_pd;

  *ret = 0;
  *ret_size = 4;

}
