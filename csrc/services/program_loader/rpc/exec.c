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
#include                  <unistd.h>
#include                  <elf.h>

extern struct s_program   programs[32];

void                      exec_rpc(u64 sender, void *params, void **ret, unsigned *ret_size)
{
  u64                     new_pid;
  int                     sys_ret;
  // void                    *void_ret;
  char                    prog_name[1024];

  sender = sender;

  get_str_arg(&params, prog_name);
  kwrite(15, "EXEC : ", 0);
  kwrite(15, prog_name, 0);
  kwrite(15, "\n", 0);

  // Get prog elf by name

  if ((sys_ret = sys_create_process(&new_pid)) < 0)
  {
    kwrite(15, "Create process : ", 0);
    print_error(sys_ret);

    *ret = (void *)sys_ret;
    *ret_size = 4;
    return;
  }

  if ((sys_ret = rpc_create_as(new_pid)) < 0)
  {
    kwrite(15, "Create as : ", 0);
    print_error(sys_ret);
    *ret = (void *)sys_ret;
    *ret_size = 4;
    return;
  }

  unsigned entry;
  unsigned stack;

  //FIXME
  if ((sys_ret = load_elf(new_pid, (void *)prog_name, &entry, &stack)) < 0)
  {
    kwrite(15, "Load elf : ", 0);
    print_error(sys_ret);
    *ret = (void *)sys_ret;
    *ret_size = 4;
    return;
  }

  if ((sys_ret = sys_run_process(&new_pid, (void *)entry, (void *)stack, 0)) < 0)
  {
    kwrite(15, "Run process : ", 0);
    print_error(sys_ret);
    *ret = (void *)sys_ret;
    *ret_size = 4;
    return;
  }


}
