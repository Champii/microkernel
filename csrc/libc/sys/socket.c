/*
 * File: socket.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: send/recv functions
 *
 */

#include <sys/socket.h>
#include <sys/syscall.h>
#include <unistd.h>

unsigned itoa_base(int n, char *s, int base);

#define COLOR_WHITE 15

void sys_printk(int color, char *str)
{
  int ret;

  SYSCALL2(8, ret, color, str);
}


int send(u64 to, void *msg, u32 size)
{
  return sys_send(to, msg, size);
}

int recv(u64 from, void *pool, u32 pool_size, u64 *pid)
{
  int sys_ret;

  while (!(sys_ret = sys_recv(from, pool, pool_size)))
  {
    sleep(100);
  }

  char tmp[10];

  itoa_base(sys_ret, tmp, 10);
  sys_printk(COLOR_WHITE, "out of sleep ! Ret = ");
  sys_printk(COLOR_WHITE, tmp);

  if (pid && sys_ret >= 0)
    *pid = *((u64 *) (&((char *) pool)[sys_ret]));

  return sys_ret;
}
