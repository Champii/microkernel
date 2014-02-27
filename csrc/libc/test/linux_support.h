/*
 * File: linux_support.h
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: Linux support for testing purpose
 *
 */

#ifndef LINUX_SUPPORT_H_
#define LINUX_SUPPORT_H_

#include "sys/types.h"

#define LINUX_MMAP_FLAGS 0x20 | 0x2 | 0x40 // MAP_ANONYMOUS, MAP_PRIVATE, MAP_32BIT
#define LINUX_O_RDWR 02
#define LINUX_O_CREAT 0100

struct mq_attr {
  long mq_flags;       /* Flags: 0 or O_NONBLOCK */
  long mq_maxmsg;      /* Max. # of messages on queue */
  long mq_msgsize;     /* Max. message size (bytes) */
  long mq_curmsgs;     /* # of messages currently in queue */
};

extern int linux_write(int fd, const void *buf, unsigned count);
extern int linux_exit(int n);
extern void *linux_mmap(void *addr, u32 length, int prot, int flags, int fd,
    off_t offset);
extern int linux_munmap(void *addr, u32 length);

extern int linux_mq_open(const char *name, int oflag, u32 mode,
    struct mq_attr *attr);
extern int linux_mq_unlink(const char *name);
extern int linux_mq_timedsend(int mqdes, char *msg, u32 msg_len, u32 msg_prio,
    void *abs_timeout);
extern int linux_mq_timedreceive(int mqdes, char *msg_ptr, u32 msg_len,
    u32 *msg_prio, void *abs_timeout);

#endif /* !LINUX_SUPPORT_H_ */
