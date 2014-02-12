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
extern u64 prog_loader_pid;

void _start(u64 pl_pid)
{
  prog_loader_pid = pl_pid;
  main();
  exit();
}
