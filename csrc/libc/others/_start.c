/*
 * File: _start.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: entry point
 *
 */

// #include <rpc/rpc.h>
#include <stdlib.h>

extern int main(void);

void _start()
{
  // prog_loader_pid = pl_pid;
  main();
  exit();
}
