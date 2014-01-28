/*
 * File: syscall.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: all system calls
 *
 */

#include <sys/syscall.h>

#define SYS_LSB64(val) ((u32) (val))
#define SYS_MSB64(val) ((u32) ((val) >> 32))

int sys_create_process(u64 *user_pid)
{
  int res;

  SYSCALL1(SYS_CREATE_PROCESS, res, user_pid);

  return res;
}

int sys_run_process(void *task_struct, void *entry, void *stack, void *root_pt)
{
  int res;

  SYSCALL4(SYS_RUN_PROCESS, res, task_struct, entry, stack, root_pt);

  return res;
}

int sys_kill(u64 pid)
{
  int res;

  SYSCALL2(SYS_KILL, res, SYS_LSB64(pid), SYS_MSB64(pid));

  return res;
}

int sys_wait(u64 pid)
{
  int res;

  SYSCALL2(SYS_WAIT, res, SYS_LSB64(pid), SYS_MSB64(pid));

  return res;
}

int sys_sleep(u32 milli)
{
  int res;

  SYSCALL1(SYS_SLEEP, res, milli);

  return res;
}

int sys_send(u64 to, void *msg, u32 size)
{
  int res;

  SYSCALL4(SYS_SEND, res, SYS_LSB64(to), SYS_MSB64(to), msg, size);

  return res;
}

int sys_recv(u64 from, void *pool, u32 pool_size)
{
  int res;

  SYSCALL4(SYS_RECV, res, SYS_LSB64(from), SYS_MSB64(from), pool, pool_size);

  return res;
}

void sys_invlpg(void *vaddr)
{
  int res;

  SYSCALL1(SYS_INVLPG, res, vaddr);
}
