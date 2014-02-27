/*
 * File: test.c
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: dummy program for testing purpose
 *
 */

#include "dummy_rpc.h"
#include "linux_support.h"
#include "rpc/io.h"
#include "rpc/paging.h"
#include "rpc/program_loader.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

static void goToHell(const char *str)
{
  linux_write(1, str, strlen(str));
  linux_exit(1);
}

int main(void)
{
  if (dummy_rpc_init(IAM_NOTHING))
    goToHell("RPC init failed\n");

  printf("lol");

  void *lol = malloc(42);
  free(lol);

  if (rpc_exec("lol"))
    goToHell("rpc_exec failed!\n");

  if (rpc_kill(42))
    goToHell("rpc_kill failed!\n");

  if (rpc_mmap_phys((void *) 0x42, (void *) 0x42, 0xf, 1))
    goToHell("rpc_mmap_phys failed!\n");

  if (rpc_create_as(42))
    goToHell("rpc_create_as failed!\n");

  if (rpc_mmap_sys(42, (void *) 0x42, 0xf, 1))
    goToHell("rpc_del_as failed!\n");

  if (rpc_del_as(42))
    goToHell("rpc_del_as failed!\n");

  if (rpc_write_at(27, 27, 0xf, 'v'))
    goToHell("rpc_write_at failed!\n");

  if (rpc_read_one() != 'v')
    goToHell("rpc_read failed!\n");

  char buff[128];

  if (rpc_read(buff, 128) < 0)
    goToHell("rpc_read failed p1!\n");
  if (strcmp("mikro!", buff))
    goToHell("rpc_read failed p2!\n");

  linux_write(1, "Success!\n", 9);
  return 0;
}
