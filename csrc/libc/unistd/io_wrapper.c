/*
 * File: io_wrapper.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: basic i/o functions
 *
 */

#include <rpc/io.h>
#include <unistd.h>

int write(int color, const char *str, unsigned size)
{
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
