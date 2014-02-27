/*
 * File: io.c
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: RPC for io service
 *
 */

#include <rpc/io.h>
#include <rpc/rpc.h>

const char *rpc_io_desc[4] =
{
  "iiis",   // write
  "iiiic",  // write_at
  "s",      // read
  "c",      // read_one
};

int rpc_write(int color, const char *str, unsigned size)
{
  u64 io_pid;
  int res;
  int sys_ret;

  sys_ret = io_service_pid(&io_pid);
  if (sys_ret < 0)
    return sys_ret;

  sys_ret = call_rpc(
      io_pid,
      RPC_IO_WRITE,
      rpc_io_desc[RPC_IO_WRITE],
      (void *) &res,
      0,
      color,
      size,
      str);

  if (sys_ret < 0)
    return sys_ret;

  return res;
}

int rpc_write_at(int x, int y, int color, char c)
{
  u64 io_pid;
  int res;
  int sys_ret;

  sys_ret = io_service_pid(&io_pid);
  if (sys_ret < 0)
    return sys_ret;

  sys_ret = call_rpc(
      io_pid,
      RPC_IO_WRITE_AT,
      rpc_io_desc[RPC_IO_WRITE_AT],
      (void *) &res,
      0,
      x,
      y,
      color,
      c);

  if (sys_ret < 0)
    return sys_ret;

  return res;
}

int rpc_read(char *buff, u32 size)
{
  u64 io_pid;
  int sys_ret;

  sys_ret = io_service_pid(&io_pid);
  if (sys_ret < 0)
    return sys_ret;

  sys_ret = call_rpc(
      io_pid,
      RPC_IO_READ,
      rpc_io_desc[RPC_IO_READ],
      (void *) buff,
      size,
      size);

  return sys_ret;
}

char rpc_read_one(void)
{
  u64 io_pid;
  char res;
  int sys_ret;

  sys_ret = io_service_pid(&io_pid);
  if (sys_ret < 0)
    return sys_ret;

  sys_ret = call_rpc(
      io_pid,
      RPC_IO_READ_ONE,
      rpc_io_desc[RPC_IO_READ_ONE],
      (void *) &res,
      0);

  if (sys_ret < 0)
    return sys_ret;

  return res;
}
