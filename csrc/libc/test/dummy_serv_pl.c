/*
 * File: dummy_serv_pl.c
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: dummy program loader service for testing purpose
 *
 */

#include "../stdio/itoa_base.h"
#include "dummy_rpc.h"
#include "linux_support.h"
#include "rpc/program_loader.h"
#include "rpc/rpc.h"
#include "string.h"

static char buffer[0x1000];

static void my_exec(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;

  linux_write(1, "RPC-PL: exec(", 13);
  linux_write(1, params, param_size - 1);
  linux_write(1, ")\n", 2);

  *((u32 *) ret) = 0;
  *ret_size = sizeof(u32);
}

static void my_exit(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;
  (void) params;
  (void) param_size;

  linux_write(1, "RPC-PL: exit()\n", 15);

  *((u32 *) ret) = 0;
  *ret_size = sizeof(u32);
}

static void my_kill(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;
  (void) param_size;
  int len;

  linux_write(1, "RPC-PL: kill(", 13);
  len = uitoa_base((u32) *((u64 *) params), buffer, 10);
  linux_write(1, buffer, len);
  linux_write(1, ")\n", 2);

  *((u32 *) ret) = 0;
  *ret_size = sizeof(u32);
}

static void my_service_pid(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;

  *ret_size = sizeof(u64);
  linux_write(1, "RPC-PL: service_pid() - ", 24);
  if (!strncmp(params, "paging", param_size))
  {
    linux_write(1, "ok\n", 3);
    *((u64 *) ret) = 2;

    return;
  }

  if (!strncmp(params, "io", param_size))
  {
    linux_write(1, "ok\n", 3);
    *((u64 *) ret) = 3;

    return;
  }

  linux_write(1, "ko\n", 3);
  *((u64 *) ret) = 0;
}

static handler_rpc pl_rpcs[] =
{
  &my_exec,
  &my_exit,
  &my_kill,
  &my_service_pid
};

int main(void)
{
  if (dummy_rpc_init(IAM_SERV_PL))
  {
    linux_write(1, "RPC init failed\n", 16);
    linux_exit(0);
  }


  if (register_rpc(pl_rpcs, rpc_pl_desc, 4))
  {
    linux_write(1, "Error while registering rpcs\n", 29);
    linux_exit(0);
  }

  if (listen_rpc())
  {
    linux_write(1, "Error while listening for rpcs\n", 31);
    linux_exit(0);
  }

  return 0;
}
