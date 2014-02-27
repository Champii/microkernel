/*
 * File: dummy_serv_io.c
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: dummy io service for testing purpose
 *
 */

#include "../stdio/itoa_base.h"
#include "errno.h"
#include "dummy_rpc.h"
#include "linux_support.h"
#include "rpc/io.h"
#include "rpc/rpc.h"
#include "string.h"

static char buffer[0x1000];

struct write_args
{
  int color;
  u32 size;
} __attribute__((packed));

struct write_at_args
{
  int x;
  int y;
  int color;
  char c;
} __attribute__((packed));

static void my_write(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;
  struct write_args *write_arg = (struct write_args *) params;
  int len;

  linux_write(1, "RPC-IO: write(0x", 16);
  len = uitoa_base((u32) write_arg->color, buffer, 16);
  linux_write(1, buffer, len);
  linux_write(1, ", ", 2);
  linux_write(1, (void *) (write_arg + 1),
      param_size - sizeof(struct write_args) - 1);
  len = uitoa_base(write_arg->size, buffer, 10);
  linux_write(1, ", ", 2);
  linux_write(1, buffer, len);
  linux_write(1, ")\n", 2);

  *((u32 *) ret) = 0;
  *ret_size = sizeof(u32);
}

static void my_write_at(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;
  (void) param_size;
  struct write_at_args *write_at_arg = (struct write_at_args *) params;
  int len;

  linux_write(1, "RPC-IO: write_at(", 17);
  len = uitoa_base((u32) write_at_arg->x, buffer, 10);
  linux_write(1, buffer, len);
  linux_write(1, ", ", 2);
  len = uitoa_base((u32) write_at_arg->y, buffer, 10);
  linux_write(1, buffer, len);
  linux_write(1, ", 0x", 4);
  len = uitoa_base((u32) write_at_arg->color, buffer, 16);
  linux_write(1, buffer, len);
  linux_write(1, ", ", 2);
  linux_write(1, &write_at_arg->c, 1);
  linux_write(1, ")\n", 2);

  *((u32 *) ret) = 0;
  *ret_size = sizeof(u32);
}

static void my_read(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;
  (void) params;
  (void) param_size;

  linux_write(1, "RPC-IO: read()\n", 15);

  memcpy(ret, "mikro!", 7);
  *ret_size = 7;
}

static void my_read_one(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;
  (void) params;
  (void) param_size;

  linux_write(1, "RPC-IO: read_one()\n", 19);

  *((char *) ret) = 'v';
  *ret_size = sizeof(char);
}

static handler_rpc io_rpcs[] =
{
  &my_write,
  &my_write_at,
  &my_read,
  &my_read_one
};

int main(void)
{
  if (dummy_rpc_init(IAM_SERV_IO))
  {
    linux_write(1, "RPC init failed\n", 16);
    linux_exit(0);
  }

  if (register_rpc(io_rpcs, rpc_io_desc, 4))
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
