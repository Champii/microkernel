/************************************************
*
* Micro Kernel
*
* - Program Loader Main
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <rpc/rpc.h>
#include                  <unistd.h>
#include                  <string.h>
#include                  <errno.h>
#include                  <rpc/paging.h>

#include                  <rpc.h>

extern unsigned           *stack_start;

extern u64                paging_pid;
extern u64                io_pid;

int uitoa_base(unsigned n, char *str, unsigned size);

void                      get_services_pid()
{
  paging_pid = *(u64 *)stack_start;
  io_pid = *((u64 *)stack_start + 1);

  // char tmp[10];
  // unsigned *pid_split = (unsigned *)&paging_pid;

  // uitoa_base(pid_split[0], tmp, 16);
  // kwrite(15, "Paging pid = ", 0);
  // kwrite(15, tmp, 0);
  // pid_split = (unsigned *)&io_pid;
  // uitoa_base(pid_split[0], tmp, 16);
  // kwrite(15, "\nIo pid = ", 0);
  // kwrite(15, tmp, 0);
  // kwrite(15, "\n", 0);

  // io_pid = (u64 *)stack_start;
  // paging_pid = (u64 *)(stack_start + 2);
}

int                       main()
{
  // kwrite(15, "TEST\n", 0);
  get_services_pid();


  register_listen_rpcs();

  for (;;);

  return (0);
}
