/*
 * File: syscall.c
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: fake system calls for testing purpose
 *
 */

#include "../stdio/itoa_base.h"
#include "dummy_rpc.h"
#include "linux_support.h"

static char buffer[4096];

int sys_create_process(u64 *user_pid)
{
  (void) user_pid;

  linux_write(1, "SYSCALL: create_process\n", 24);
  return 0;
}

int sys_run_process(void *task_struct, void *entry, void *stack, void *root_pt)
{
  (void) task_struct;
  (void) entry;
  (void) stack;
  (void) root_pt;

  linux_write(1, "SYSCALL: run_process\n", 21);
  return 0;
}

int sys_kill(u64 pid)
{
  (void) pid;

  linux_write(1, "SYSCALL: kill\n", 14);
  return 0;
}

int sys_wait(u64 pid)
{
  (void) pid;

  linux_write(1, "SYSCALL: wait\n", 14);
  return 0;
}

int sys_sleep(u32 milli)
{
  (void) milli;

  linux_write(1, "SYSCALL: sleep\n", 15);
  return 0;
}

int sys_send(u64 to, void *msg, u32 size)
{
  int len;

  linux_write(1, "SYSCALL: send\n TO: ", 19);
  len = uitoa_base((u32) to, buffer, 10);
  linux_write(1, buffer, len);
  linux_write(1, " FUNC ID: ", 10);
  if (size > 0)
  {
    len = uitoa_base(*((u32 *) msg), buffer, 10);
    linux_write(1, buffer, len);
  }
  linux_write(1, "\n", 1);

  return dummy_rpc_on_send(to, msg, size);
}

int sys_recv(u64 from, void *pool, u32 pool_size)
{
  int len;

  linux_write(1, "SYSCALL: recv\n FROM: ", 21);
  len = uitoa_base((u32) from, buffer, 10);
  linux_write(1, buffer, len);
  linux_write(1, "\n", 1);

  return dummy_rpc_on_recv(from, pool, pool_size);
}

void sys_invlpg(void *vaddr)
{
  (void) vaddr;

  linux_write(1, "SYSCALL: invlpg\n", 16);
}
