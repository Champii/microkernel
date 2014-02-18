/************************************************
*
* Micro Kernel
*
* - Program Loader Exec RPC
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <sys/syscall.h>
#include                  <errno.h>
#include                  <rpc/paging.h>
#include                  <rpc.h>
#include                  <get_argument.h>
#include                  <elf.h>

void                      exec_rpc(u64 sender, void *params, void **ret, unsigned *ret_size)
{
  u64                     new_pid;
  int                     sys_ret;
  // void                    *void_ret;
  char                    prog_name[1024];

  sender = sender;
  ret = ret;
  ret_size = ret_size;

  get_str_arg(&params, prog_name);

  // Get prog elf by name

  if ((sys_ret = sys_create_process(&new_pid)) < 0)
    print_error(sys_ret);

  if ((sys_ret = rpc_create_as(new_pid)) < 0)
    print_error(sys_ret);

  unsigned stack;

  //FIXME
  if ((sys_ret = load_elf(new_pid, (void *)prog_name, &stack)) < 0)
    print_error(sys_ret);

  if ((sys_ret = sys_run_process(&new_pid, 0, &stack, 0)) < 0)
    print_error(sys_ret);


}
