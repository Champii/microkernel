/*
 * File: _start.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: entry point
 *
 */

#include <rpc/rpc.h>
#include <stdlib.h>
#include                  <unistd.h>
#include                  <string.h>

extern int main(void);

int uitoa_base(unsigned n, char *str, unsigned size);

unsigned *stack_start = 0;

void _start(unsigned *stack)
{


  prog_loader_pid = *((u64 *)stack);
  stack_start = stack + 2;
  // char tmp[10];
  // memset(tmp, 0, 10);

  // unsigned *pid_split = (unsigned *)&prog_loader_pid;
  // uitoa_base(pid_split[1], tmp, 10);

  // kwrite(15, "PL pid = ", 0);
  // kwrite(15, tmp, 0);
  // kwrite(15, "\n", 0);

  // test = test;
  main();
  exit();
}

// void _start(u64 pl_pid, u64 test, u64 test2)
// {
//   stack_start = (unsigned *)&test2;
//   prog_loader_pid = pl_pid;
//   test = test;
//   main();
//   exit();
// }
