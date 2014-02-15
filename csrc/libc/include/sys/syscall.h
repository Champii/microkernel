/*
 * File: sys/syscall.h
 * Author: Victor Aperce <viaxxx@lse.epita.fr>
 *
 * Description: EPITECH K II system calls
 *
 */

#ifndef SYSCALL_H_
# define SYSCALL_H_

#include <sys/types.h>

/* SYSCALL ID */
#define SYS_CREATE_PROCESS  0
#define SYS_RUN_PROCESS     1
#define SYS_KILL            2
#define SYS_WAIT            3
#define SYS_SLEEP           4
#define SYS_SEND            5
#define SYS_RECV            6
#define SYS_INVLPG          7


/* SYSCALL MACROS */
#define SYSCALL_INT_NUM 0x80

#define ASM_INT_MACRO1(num) "int $"#num"\n"
#define ASM_INT_MACRO2(num) ASM_INT_MACRO1(num)
#define ASM_INT ASM_INT_MACRO2(SYSCALL_INT_NUM)

#define SYSCALL0(num, ret)  \
  __asm__ (                 \
      "mov %1, %%eax\n"     \
      ASM_INT               \
      : "=a" (ret)          \
      : "i"(num)            \
      )

#define SYSCALL1(num, ret, p1)  \
  __asm__ (                     \
      "mov %1, %%eax\n"         \
      ASM_INT                   \
      : "=a" (ret)              \
      : "i"(num), "b"(p1)       \
      )

#define SYSCALL2(num, ret, p1, p2)  \
  __asm__ (                         \
      "mov %1, %%eax \n"            \
      ASM_INT                       \
      : "=a" (ret)                  \
      : "i"(num), "b"(p1), "c"(p2)  \
      )



#define SYSCALL3(num, ret, p1, p2, p3)      \
  __asm__ (                                 \
      "mov %1, %%eax\n"                     \
      ASM_INT                               \
      : "=a" (ret)                          \
      : "i"(num), "b"(p1), "c"(p2), "d"(p3) \
      )

#define SYSCALL4(num, ret, p1, p2, p3, p4)            \
  __asm__ (                                           \
      "mov %1, %%eax\n"                               \
      ASM_INT                                         \
      : "=a" (ret)                                    \
      : "i"(num), "b"(p1), "c"(p2), "d"(p3), "S"(p4)  \
      )

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int sys_create_process(u64 *user_pid);
int sys_run_process(void *task_struct, void *entry, void *stack, void *root_pt);
int sys_kill(u64 pid);
int sys_wait(u64 pid);
int sys_sleep(u32 milli);
int sys_send(u64 to, void *msg, u32 size);
int sys_recv(u64 from, void *pool, u32 pool_size);
void sys_invlpg(void *vaddr);
void sys_write(int, const char *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !SYSCALL_H_ */
