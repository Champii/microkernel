/*
 * File: rpc.c
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: RPC system
 *
 */

#include <errno.h>
#include <rpc/program_loader.h>
#include <rpc/rpc.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define COLOR_WHITE 15

u64 prog_loader_pid;
static u64 paging_pid = 0;
static u64 io_pid = 0;

static struct rpc *reg_rpcs = 0;
static unsigned reg_rpcs_num = 0;

unsigned uitoa_base(unsigned n, char *s, int base);

static int check_param_ret(char param_ret, void *ret)
{
  switch (param_ret)
  {
    case 'v':
      break;

    case 'i':
    case 'I':
    case 'c':
    case 's':
      if (!ret)
        return -EINVAL;
      break;

    default:
      return -EINVAL;
  }

  return 0;
}

static int update_size(u32 *msg_size, u32 to_add)
{
  if (*msg_size + to_add >= MSG_MAXSIZE)
    return -1;

  *msg_size += to_add;
  return 0;
}

static int call_rpc_get_ret(u64 pid, u32 function_id, char param_ret,
    void *ret, u32 ret_size, char *msg_buff)
{
  int sys_ret;

  if (param_ret != 'v')
  {
    sys_ret = recv(pid, msg_buff, MSG_MAXSIZE, NULL);
    if (sys_ret < 0)
      return sys_ret;

    if (sys_ret < (int) sizeof(u32))
      return -EBADMSG;


    if (*((u32 *) msg_buff) != function_id)
      return -EBADMSG;

    if (param_ret == 's')
    {
      sys_ret -= sizeof(u32);

      int copy_size;

      if (sys_ret < (int) ret_size)
        copy_size = sys_ret;
      else
        copy_size = ret_size;

      memcpy(ret, &msg_buff[sizeof(u32)], copy_size);
      return copy_size;
    }

    switch (param_ret)
    {
      case 'i':
        if (sys_ret != 2 * sizeof(u32))
          return -EBADMSG;

        *((u32 *) ret) = *((u32 *) &msg_buff[sizeof(u32)]);
        break;

      case 'I':
        if (sys_ret != sizeof(u32) + sizeof(u64))
          return -EBADMSG;

        memcpy(ret, &msg_buff[sizeof(u32)], sizeof(u64));
        // *((u64 *) ret) = *((u64 *) &msg_buff[sizeof(u32)]);
        break;

      case 'c':
        if (sys_ret != sizeof(u32) + sizeof(char))
          return -EBADMSG;

        *((char *) ret) = *((char *) &msg_buff[sizeof(u32)]);
        break;
    }
  }

  return 0;
}

int call_rpc(u64 pid, u32 function_id, const char *func_desc, void *ret,
    u32 ret_size, ...)
{
  static char msg_buff[MSG_MAXSIZE];
  u32 msg_size = sizeof(u32);
  char *cur = (char *) &msg_buff;
  va_list args;
  char *str;
  size_t params_len = strlen(func_desc);
  int sys_ret;

  if (params_len < 1)
    return -EINVAL;

  sys_ret = check_param_ret(func_desc[0], ret);
  if (sys_ret < 0)
    return sys_ret;

  *((u32 *) cur) = function_id;
  cur += sizeof(u32);

  va_start(args, ret_size);
  for (u32 i = 1; i < params_len; i++)
  {
    switch (func_desc[i])
    {
      case 'i':
        if (!update_size(&msg_size, sizeof(u32)))
        {
          *((u32 *) cur) = va_arg(args, u32);
          cur += sizeof(u32);
          break;
        }
        else
          return -EINVAL;

      case 'I':
        if (!update_size(&msg_size, sizeof(u64)))
        {
          *((u64 *) cur) = va_arg(args, u64);
          cur += sizeof(u64);
          break;
        }
        else
          return -EINVAL;

      case 'c':
        if (i != params_len - 1 || !update_size(&msg_size, sizeof(char)))
        {
          *cur = va_arg(args, int);
          cur++;
          break;
        }
        else
          return -EINVAL;

      case 's':
        str = va_arg(args, char *);
        u32 len = strlen(str);
        if (!update_size(&msg_size, len + 4))
        {
          *cur = len;
          cur++;
          memcpy(cur, str, len + 1);
          cur += len;

          break;
        }
        else
          return -EINVAL;

      default:
        return -EINVAL;
    }
  }

  va_end(args);

  sys_ret = send(pid, &msg_buff, msg_size);

  if (sys_ret < 0)
    return sys_ret;

  return call_rpc_get_ret(pid, function_id, func_desc[0], ret, ret_size,
      msg_buff);
}

int register_rpc(struct rpc *rpcs, unsigned size)
{
  if (!rpcs || !size)
  {
    reg_rpcs = 0;
    reg_rpcs_num = 0;
    return -EINVAL;
  }

  for (u32 i = 0; i < size; i++)
    if (!rpcs[i].handler || strlen(rpcs[i].func_desc) < 1)
      return -EINVAL;

  reg_rpcs = rpcs;
  reg_rpcs_num = size;
  return 0;
}

static int check_msg_size(u32 func_id, int msg_size)
{
  const char *func_desc = reg_rpcs[func_id].func_desc;
  unsigned fdesc_size = strlen(func_desc);
  int target_size = sizeof(u32);

  for (u32 i = 1; i < fdesc_size; i++)
  {
    switch (func_desc[i])
    {
      case 'i':
        target_size += sizeof(u32);
        break;

      case 'I':
        target_size += sizeof(u64);
        break;

      case 'c':
        target_size++;
        break;

      case 's':

        // write(COLOR_WHITE, "Hello STRING size = ", 0);
        // itoa_base(msg_size, tmp, 10);
        // write(COLOR_WHITE, tmp, 0);
        // write(COLOR_WHITE, " target = ", 0);
        // itoa_base(target_size, tmp, 10);
        // write(COLOR_WHITE, tmp, 0);
        // if (msg_size < target_size)
          // return -EBADMSG;

        // FIXME
        target_size = msg_size;
        break;

      default:
        break;
    }
  }

  if (msg_size != target_size)
    return -EBADMSG;

  return 0;
}

int listen_rpc(void)
{
  static char msg_buff[MSG_MAXSIZE];
  u64 sender;
  u32 *func_id;
  int sys_ret;
  int msg_size;
  void *ret;
  u32 ret_size;

  if (!reg_rpcs)
    return 0;

  while (1)
  {
    // kwrite(COLOR_WHITE, "Test\n", 0);
    sys_ret = recv(RCV_ANYONE, msg_buff, MSG_MAXSIZE, &sender);
    // kwrite(COLOR_WHITE, "Hello\n", 0);

    if (sys_ret < 0)
      return sys_ret;

    msg_size = sys_ret;

    if (msg_size < (int) sizeof(u32))
      return -EBADMSG;

    func_id = (u32 *) msg_buff;

    if (*func_id >= reg_rpcs_num)
      return -EBADMSG;

    sys_ret = check_msg_size(*func_id, msg_size);
    if (sys_ret < 0)
      return sys_ret;


    reg_rpcs[*func_id].handler(
        sender,
        ((void *) &msg_buff[sizeof(u32)]),
        &ret,
        &ret_size);

    if (reg_rpcs[*func_id].func_desc[0] != 'v')
    {
      if (ret_size + sizeof(u32) > MSG_MAXSIZE)
        return -EMSGSIZE;

      memcpy(&msg_buff[sizeof(u32)], ret, ret_size);
      // free(ret);

      sys_ret = send(sender, msg_buff, ret_size + sizeof(u32));

      if (sys_ret < 0)
        return sys_ret;
    }
  }
}

int paging_service_pid(u64 *pid)
{
  if (!paging_pid)
  {
    int sys_ret = rpc_service_pid("paging", &paging_pid);

    if (sys_ret < 0)
      return sys_ret;
  }

  *pid = paging_pid;
  return 0;
}

int io_service_pid(u64 *pid)
{
  if (!io_pid)
  {
    int sys_ret = rpc_service_pid("io", &io_pid);

    if (sys_ret < 0)
      return sys_ret;
  }

  *pid = io_pid;
  return 0;
}
