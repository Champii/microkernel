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
#include                  <string.h>
#include                  <elf.h>

extern struct s_program   programs[32];
extern unsigned           nb_progs;


static unsigned           *get_prog_addr_by_name(char *name)
{
  unsigned                i;

  for (i = 0; i < nb_progs; i++)
    if (!strncmp(programs[i].name, name, strlen(name)))
      return programs[i].elf_addr;

  return 0;
}

void                      exec_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size)
{
  u64                     new_pid;
  int                     sys_ret;
  // void                    *void_ret;
  char                    prog_name[1024];
  unsigned                *prog_addr;

  sender = sender;
  param_size = param_size;

  get_str_arg(&params, prog_name);
  // kwrite(15, "EXEC : ", 0);
  // kwrite(15, prog_name, 0);
  // kwrite(15, "\n", 0);

  if (!(prog_addr = get_prog_addr_by_name(prog_name)))
    kwrite(15, "NO PROG FOUND FOR\n", 0);


  // Get prog elf by name

  if ((sys_ret = sys_create_process(&new_pid)) < 0)
  {
    kwrite(15, "Create process : ", 0);
    print_error(sys_ret);

    *(unsigned *)ret = sys_ret;
    *ret_size = 4;
    return;
  }

  kwrite(15, "Create as : ", 0);
  if ((sys_ret = rpc_create_as(new_pid)) < 0)
  {
    kwrite(15, "Create as : ", 0);
    print_error(sys_ret);
    *(unsigned *)ret = sys_ret;
    *ret_size = 4;
    return;
  }

  unsigned entry;
  unsigned stack;

  //FIXME
  kwrite(15, "Load elf !", 0);
  if ((sys_ret = load_elf(new_pid, prog_addr, &entry, &stack)) < 0)
  {
    kwrite(15, "Load elf : ", 0);
    print_error(sys_ret);
    *(unsigned *)ret = sys_ret;
    *ret_size = 4;
    return;
  }

  kwrite(15, "Run process !", 0);
  if ((sys_ret = sys_run_process(&new_pid, (void *)entry, (void *)stack, 0)) < 0)
  {
    kwrite(15, "Run process : ", 0);
    print_error(sys_ret);
    *(unsigned *)ret = sys_ret;
    *ret_size = 4;
    return;
  }


}
