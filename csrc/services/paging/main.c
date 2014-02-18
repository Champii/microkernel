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
#include                  <mm.h>

#define                   COLOR_WHITE 15

int itoa_base(int n, char *str, unsigned size);

extern unsigned           *stack_start;

unsigned                  *pd_addr;
unsigned                  start_frame;

int main()
{

  write(COLOR_WHITE, "Starting Paging service\n", 0);

  pd_addr = (unsigned *)(*(stack_start));
  // start_frame = *(stack_start + 3);

  // init_mm();
  // start_frame = start_frame;
  char tmp[15];

  itoa_base((unsigned)pd_addr, tmp, 16);
  kwrite(COLOR_WHITE, " pd_addr = 0x", 0);
  kwrite(COLOR_WHITE, tmp, 0);
  // itoa_base(start_frame, tmp, 16);
  // kwrite(COLOR_WHITE, "Start frame = ", 0);
  // kwrite(COLOR_WHITE, tmp, 0);
  // kwrite(COLOR_WHITE, "\n", 0);

  register_listen_rpcs();

  for (;;);

  return (0);
}
