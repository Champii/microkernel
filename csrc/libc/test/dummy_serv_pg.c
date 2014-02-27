/*
 * File: dummy_serv_pg.c
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: dummy paging service for testing purpose
 *
 */

#include "../stdio/itoa_base.h"
#include "dummy_rpc.h"
#include "linux_support.h"
#include "rpc/paging.h"
#include "rpc/rpc.h"
#include "string.h"

static char buffer[0x1000];

struct mmap_args
{
  u32 vaddr;
  int rights;
  u32 size;
} __attribute__((packed));

struct mmap_phys_args
{
  u32 vaddr;
  u32 paddr;
  int rights;
  u32 size;
} __attribute__((packed));

struct mmap_sys_args
{
  u64 pid;
  u32 vaddr;
  int rights;
  u32 size;
} __attribute__((packed));

static void my_mmap(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;
  (void) param_size;
  struct mmap_args *mmap_arg = (struct mmap_args *) params;
  int len;

  linux_write(1, "RPC-PG: mmap(0x", 15);
  len = uitoa_base(mmap_arg->vaddr, buffer, 16);
  linux_write(1, buffer, len);
  linux_write(1, ", 0x", 4);
  len = uitoa_base((u32) mmap_arg->rights, buffer, 16);
  linux_write(1, buffer, len);
  linux_write(1, ", ", 2);
  len = uitoa_base(mmap_arg->size, buffer, 10);
  linux_write(1, buffer, len);
  linux_write(1, ")\n", 2);

  *((u32 *) ret) = 0;
  *ret_size = sizeof(u32);
}

static void my_mmap_phys(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;
  (void) param_size;
  struct mmap_phys_args *mmap_phys_arg = (struct mmap_phys_args *) params;
  int len;

  linux_write(1, "RPC-PG: mmap_phys(0x", 20);
  len = uitoa_base(mmap_phys_arg->vaddr, buffer, 16);
  linux_write(1, buffer, len);
  linux_write(1, ", 0x", 4);
  len = uitoa_base(mmap_phys_arg->paddr, buffer, 16);
  linux_write(1, buffer, len);
  linux_write(1, ", 0x", 4);
  len = uitoa_base((u32) mmap_phys_arg->rights, buffer, 16);
  linux_write(1, buffer, len);
  linux_write(1, ", ", 2);
  len = uitoa_base(mmap_phys_arg->size, buffer, 10);
  linux_write(1, buffer, len);
  linux_write(1, ")\n", 2);

  *((u32 *) ret) = 0;
  *ret_size = sizeof(u32);
}

static void my_munmap(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;
  (void) params;
  (void) param_size;

  linux_write(1, "RPC-PG: munmap(...)\n", 20);

  *((u32 *) ret) = 0;
  *ret_size = sizeof(u32);
}

static void my_create_as(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;
  (void) param_size;
  int len;

  linux_write(1, "RPC-PG: create_as(", 18);
  len = uitoa_base((u32) *((u64 *) params), buffer, 10);
  linux_write(1, buffer, len);
  linux_write(1, ")\n", 2);

  *((u32 *) ret) = 0;
  *ret_size = sizeof(u32);
}

static void my_del_as(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;
  (void) param_size;
  int len;

  linux_write(1, "RPC-PG: del_as(", 15);
  len = uitoa_base((u32) *((u64 *) params), buffer, 10);
  linux_write(1, buffer, len);
  linux_write(1, ")\n", 2);

  *((u32 *) ret) = 0;
  *ret_size = sizeof(u32);
}

static void my_mmap_sys(u64 sender, void *params, u32 param_size, void *ret,
    u32 *ret_size)
{
  (void) sender;
  (void) param_size;
  struct mmap_sys_args *mmap_sys_arg = (struct mmap_sys_args *) params;
  int len;

  linux_write(1, "RPC-PG: mmap_sys(", 17);
  len = uitoa_base((u32) mmap_sys_arg->pid, buffer, 10);
  linux_write(1, buffer, len);
  linux_write(1, ", 0x", 4);
  len = uitoa_base(mmap_sys_arg->vaddr, buffer, 16);
  linux_write(1, buffer, len);
  linux_write(1, ", 0x", 4);
  len = uitoa_base((u32) mmap_sys_arg->rights, buffer, 16);
  linux_write(1, buffer, len);
  linux_write(1, ", ", 2);
  len = uitoa_base(mmap_sys_arg->size, buffer, 10);
  linux_write(1, buffer, len);
  linux_write(1, ")\n", 2);

  *((u32 *) ret) = 0;
  *ret_size = sizeof(u32);
}

static handler_rpc pg_rpcs[] =
{
  &my_mmap,
  &my_mmap_phys,
  &my_munmap,
  &my_create_as,
  &my_del_as,
  &my_mmap_sys
};

int main(void)
{
  if (dummy_rpc_init(IAM_SERV_PG))
  {
    linux_write(1, "RPC init failed\n", 16);
    linux_exit(0);
  }


  if (register_rpc(pg_rpcs, rpc_paging_desc, 6))
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
