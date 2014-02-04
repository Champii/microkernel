/************************************************
*
* Micro Kernel
*
* - Syscall funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  "syscall.h"
#include                  "process.h"
#include                  "isrs.h"

static void               syscall_handler(struct s_regs *regs);

static void               *syscalls[8] =
{
  &create_process,        // create_process
  &run_process,           // run_process
  &kill,                  // kill
  &wait,                  // wait
  &sleep,                 // sleep
  0,                      // send
  0,                      // recv
  0                       //invlpg
};

unsigned                  num_syscalls = 8;

void                      init_syscalls()
{
  register_interrupt_handler(0x80, &syscall_handler);
}

void                      syscall_handler(struct s_regs *regs)
{
  if (regs->eax >= num_syscalls)
    return;

  void *location = syscalls[regs->eax];

  if (!location)
    return;

  int ret;
  asm volatile (" \
    push %1; \
    push %2; \
    push %3; \
    push %4; \
    push %5; \
    call *%6; \
    pop %%ebx; \
    pop %%ebx; \
    pop %%ebx; \
    pop %%ebx; \
    pop %%ebx; \
  " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (location));
  regs->eax = ret;
}