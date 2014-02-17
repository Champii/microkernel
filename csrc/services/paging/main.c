/************************************************
*
* Micro Kernel
*
* - Paging Main
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <rpc/rpc.h>
#include                  <stdio.h>
#include                  <unistd.h>
#include                  <sys/syscall.h>
#include                  <errno.h>

#include                  <rpc.h>

#define                   COLOR_WHITE 15

int main()
{

  write(COLOR_WHITE, "Starting Paging service\n", 0);

  register_listen_rpcs();

  for (;;);

  return (0);
}
