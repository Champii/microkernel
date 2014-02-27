/*
 * File: exit.c
 * Author: Victor <vaperce@gmail.com>
 *
 * Description: simple implementation for exit
 *
 */

#include <rpc/program_loader.h>
#include <stdlib.h>

__attribute__((noreturn)) void exit(void)
{
  rpc_exit();

  while (1)
    ;
}
