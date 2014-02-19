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

#include                  <rpc.h>

extern unsigned           *stack_start;

u64                       *paging_pid = 0;
u64                       *io_pid = 0;

void                      get_services_pid()
{
  io_pid = (u64 *)stack_start;
  paging_pid = (u64 *)(stack_start + 2);
}

int                       main()
{
  get_services_pid();

  register_listen_rpcs();

  for (;;);

  return (0);
}
