/************************************************
*
* Micro Kernel
*
* - Syscalls funcs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef               __SYSCALL_H__
# define              __SYSCALL_H__

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


void                  init_syscalls();

#endif                /*__SYSCALL_H__*/
