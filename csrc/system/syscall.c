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
#include                  "idt.h"
#include                  "ipc.h"
#include                  "screen.h"

//test
#include                  "kmalloc.h"


void                      syscall_handler(struct s_regs *regs);
extern int                syscall;

static void               *syscalls[8] =
{
  &create_process,        // create_process
  &run_process,           // run_process
  &kill,                  // kill
  &wait,                  // wait
  &sleep,                 // sleep
  &send,                  // send
  &recv,                  // recv
  0                       //invlpg
};

unsigned                  num_syscalls = 8;

void                      init_syscalls()
{
  idt_set_gate(0x80, (unsigned)&syscall, 0x08, 0x8E);
}

void                      syscall_handler(struct s_regs *regs)
{
  printk(COLOR_WHITE, "Syscall ! ");
  printk(COLOR_WHITE, my_putnbr_base(regs->eax, "0123456789"));
  printk(COLOR_WHITE, "\n");

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