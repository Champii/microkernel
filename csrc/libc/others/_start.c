/*
 * File: _start.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: entry point
 *
 */

#include <rpc/rpc.h>
#include <stdlib.h>

extern int main(void);
// extern u64 prog_loader_pid;

unsigned *stack_start = 0;

void _start(u64 pl_pid, u64 test, u64 test2)
{
  stack_start = (unsigned *)&test2;
  prog_loader_pid = pl_pid;
  test = test;
  main();
  exit();
}
