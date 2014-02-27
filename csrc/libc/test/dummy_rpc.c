/*
 * File: dummy_rpc.c
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: Dummy RPC answers for testing purpose
 *
 */

#include "../stdio/itoa_base.h"
#include "dummy_rpc.h"
#include "errno.h"
#include "linux_support.h"
#include "string.h"

#define FAKE_PID_PL 1
#define FAKE_PID_PG 2
#define FAKE_PID_IO 3

static int fd_pl;
static int fd_pg;
static int fd_io;
static int my_fd;

void *temp_mmap;
static char buffer[0x1000];

struct mmap_args
{
  u32 vaddr;
  int rights;
  u32 size;
} __attribute__((packed));

int dummy_rpc_init(int iam)
{
  static struct mq_attr attr =
  {
    0,
    10,
    0x1000,
    0
  };

  if (iam != IAM_SERV_PL)
  {
    if ((fd_pl = linux_mq_open("serv_pl", LINUX_O_RDWR, 0, 0)) < 0)
    {
      linux_write(1, "Cannot open pl mq\n", 18);
      return -1;
    }

    if (iam != IAM_SERV_PG)
      if ((fd_pg = linux_mq_open("serv_pg", LINUX_O_RDWR, 0, 0)) < 0)
      {
        linux_write(1, "Cannot open pg mq\n", 18);
        return -1;
      }

    if (iam != IAM_SERV_IO)
      if ((fd_io = linux_mq_open("serv_io", LINUX_O_RDWR, 0, 0)) < 0)
      {
        linux_write(1, "Cannot open io mq\n", 18);
        return -1;
      }
  }

  switch (iam)
  {
    case IAM_SERV_PL:
      if ((my_fd = linux_mq_open(
              "serv_pl",
              LINUX_O_RDWR | LINUX_O_CREAT,
              00644,
              &attr)) < 0)
      {
        linux_write(1, "Cannot create pl mq\n", 20);
        return -1;
      }

      if (linux_mq_open(
              "serv_pg",
              LINUX_O_RDWR | LINUX_O_CREAT,
              00644,
              &attr) < 0)
      {
        linux_write(1, "Cannot create pg mq\n", 20);
        return -1;
      }

      if (linux_mq_open(
              "serv_io",
              LINUX_O_RDWR | LINUX_O_CREAT,
              00644,
              &attr) < 0)
      {
        linux_write(1, "Cannot create io mq\n", 20);
        return -1;
      }
      break;

    case IAM_SERV_PG:
      if ((my_fd = linux_mq_open("serv_pg", LINUX_O_RDWR, 0, 0)) < 0)
      {
        linux_write(1, "Cannot open pg mq\n", 18);
        return -1;
      }
      break;

    case IAM_SERV_IO:
      if ((my_fd = linux_mq_open("serv_io", LINUX_O_RDWR, 0, 0)) < 0)
      {
        linux_write(1, "Cannot open pg io\n", 18);
        return -1;
      }
      break;

    default:
      break;
  }

  return 0;
}

void dummy_rpc_clean(void)
{
  linux_mq_unlink("/serv_pl");
  linux_mq_unlink("/serv_pg");
  linux_mq_unlink("/serv_io");
}

static void mmap_hook_out(void *msg, u32 size)
{
  struct mmap_args *mmap_arg = (struct mmap_args *) (msg + sizeof(u32));

  switch (*((u32 *) msg))
  {
    case 0: // MMAP
      if (size < sizeof(struct mmap_args) + sizeof(u32))
        break;

      temp_mmap = linux_mmap(NULL,
          mmap_arg->size << 12,
          mmap_arg->rights,
          LINUX_MMAP_FLAGS,
          0,
          0);
      break;

    case 2: // MUNMAP
      if (size < 2 * sizeof(u32))
        break;

      linux_munmap((void *) ((unsigned long) mmap_arg->vaddr), 0x1000);
      break;

    default:
      break;
  }
}

int dummy_rpc_on_send(u64 to, void *msg, u32 size)
{
  if (size >= 0x1000 - sizeof(unsigned))
    return -EMSGSIZE;

  int fd;

  switch (to)
  {
    case 42:
      fd = my_fd;
      break;

    case FAKE_PID_PL:
      fd = fd_pl;
      break;

    case FAKE_PID_PG:
      mmap_hook_out(msg, size);
      fd = fd_pg;
      break;

    case FAKE_PID_IO:
      fd = fd_io;
      break;

    default:
      linux_write(1, "RPC: bad pid\n", 13);
      return -EPERM;
  }

  int sys_ret = linux_mq_timedsend(fd, msg, size, 0, NULL);

  if (sys_ret < 0)
  {
    linux_write(1, "Error ", 6);
    int len = itoa_base(sys_ret, buffer, 10);
    linux_write(1, buffer, len);
    linux_write(1, " in send\n ", 9);

    return sys_ret;
  }

  return 0;
}

static void exit_hook_in(void *msg, u32 size)
{
  if (size >= sizeof(u32) && *((u32 *) msg) == 1) // EXIT case
    linux_exit(0);
}

static void mmap_hook_in(void *msg, u32 size)
{
  if (size >= 2 * sizeof(u32) && *((u32 *) msg) == 0) // MMAP case
  {
    u32 *mmap_res = (u32 *) msg;
    mmap_res[1] = (u32) ((unsigned long) temp_mmap);
  }
}

int dummy_rpc_on_recv(u64 from, void *msg, u32 size)
{
  if (size < sizeof(u32))
    return -EBADMSG;

  int fd;

  switch (from)
  {
    case 0:
      fd = my_fd;
      break;

    case FAKE_PID_PL:
      fd = fd_pl;
      break;

    case FAKE_PID_PG:
      fd = fd_pg;
      break;

    case FAKE_PID_IO:
      fd = fd_io;
      break;

    default:
      linux_write(1, "RPC: bad pid\n", 13);
      return -EPERM;
  }

  int sys_ret = linux_mq_timedreceive(fd, msg, size, NULL, NULL);

  if (sys_ret < 0)
  {
    linux_write(1, "Error ", 6);
    int len = itoa_base(sys_ret, buffer, 10);
    linux_write(1, buffer, len);
    linux_write(1, " in receive\n ", 12);

    goto no_process;
  }

  *((u64 *) (msg + sys_ret)) = 42;

  if (from == FAKE_PID_PL)
    exit_hook_in(msg, size);
  else if (from == FAKE_PID_PG)
    mmap_hook_in(msg, size);

no_process:
  return sys_ret;
}
