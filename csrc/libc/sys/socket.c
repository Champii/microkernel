/*
 * File: socket.c
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: send/recv functions
 *
 */

#include <sys/socket.h>
#include <sys/syscall.h>

int send(u64 to, void *msg, u32 size)
{
  return sys_send(to, msg, size);
}

int recv(u64 from, void *pool, u32 pool_size, u64 *pid)
{
  int sys_ret;
  while ((sys_ret = sys_recv(from, pool, pool_size)) == 0);

  if (pid && sys_ret >= 0)
    *pid = *((u64 *) (&((char *) pool)[sys_ret]));

  return sys_ret;
}
