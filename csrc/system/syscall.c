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

extern void               syscall();

void                      syscall_handler(struct s_regs *regs);

static void               *syscalls[9] =
{
  &create_process,        // create_process
  &run_process,           // run_process
  &kill,                  // kill
  &wait,                  // wait
  &sleep,                 // sleep
  &send,                  // send
  &recv,                  // recv
  0,                      //invlpg
  &printk                 //TEMP write
};

unsigned                  num_syscalls = 9;


void                      syscall_handler(struct s_regs *regs)
{
    // printk(COLOR_WHITE, "Syscall ! ");
    // printk(COLOR_WHITE, my_putnbr_base(regs->eax, "0123456789"));
    // printk(COLOR_WHITE, "\n");

  if (regs->eax >= num_syscalls)
    return;

  void *location = syscalls[regs->eax];

  if (!location)
    return;

  volatile int ret = 0;
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

  // printk(COLOR_WHITE, "RETURN FROM SYSCALL = ");
  // printk(COLOR_WHITE, my_putnbr_base(regs->eax, "0123456789"));
  // printk(COLOR_WHITE, "\n");

  regs->eax = ret;

}

void                      init_syscalls()
{
  // register_interrupt_handler(0x80, syscall_handler);
  idt_set_gate(0x80, (unsigned)syscall, 0x08, 0x8E);
}