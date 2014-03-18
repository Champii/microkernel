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
#include                  <string.h>
#include                  <sys/syscall.h>
#include                  <errno.h>

#include                  <rpc.h>
#include                  <mm.h>

#define                   COLOR_WHITE 15

extern unsigned           *stack_start;

int itoa_base(int n, char *str, unsigned size);

unsigned                  pd_addr;
unsigned                  start_frame;

void get_stack_args()
{
  pd_addr = *stack_start;
  start_frame = *(stack_start + 1);
}

int main()
{
  get_stack_args();

  // write(COLOR_WHITE, "Starting Paging service\n", 0);

  init_mm();

  register_listen_rpcs();

  for (;;);

  return (0);
}
