/*
 * File: io_wrapper.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: basic i/o functions
 *
 */

#include <rpc/io.h>
#include <unistd.h>
#include <sys/syscall.h>

int kwrite(int color, const char *str, unsigned size)
{
  // int ret;

  sys_write(color, str);
  size = size;

  return 0;
  // return rpc_write(color, str, size);
}

int write(int color, const char *str, unsigned size)
{
  // int ret;

  // sys_write(color, str);
  // size = size;

  // return 0;
  kwrite(15, "TEST\n", 4);
  return rpc_write(color, str, size);
}

int write_at(int x, int y, int color, char c)
{
  return rpc_write_at(x, y, color, c);
}

int read(char *buff, unsigned size)
{
  return rpc_read(buff, size);
}

char read_one()
{
  return rpc_read_one();
}
