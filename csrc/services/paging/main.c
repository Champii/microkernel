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

  // kwrite(15, "TEST\n", 0);
  kwrite(COLOR_WHITE, "Starting Paging service\n", 0);

  init_mm();
  // start_frame = start_frame;
  // char tmp[10];
  // unsigned *pid_split = (unsigned *)&prog_loader_pid;

  // itoa_base(pid_split[0], tmp, 10);
  // kwrite(COLOR_WHITE, "PL Pid = ", 0);
  // kwrite(COLOR_WHITE, tmp, 0);
  // memset(tmp, 0, 10);
  // itoa_base(pd_addr, tmp, 16);
  // kwrite(COLOR_WHITE, " pd_addr = 0x", 0);
  // kwrite(COLOR_WHITE, tmp, 0);
  // itoa_base(start_frame, tmp, 10);
  // kwrite(COLOR_WHITE, " start frame = ", 0);
  // kwrite(COLOR_WHITE, tmp, 0);
  // kwrite(COLOR_WHITE, "\n", 0);

  register_listen_rpcs();

  for (;;);

  return (0);
}
