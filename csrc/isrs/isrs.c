#include                  "idt.h"
#include                  "isrs.h"
#include                  "irq.h"
#include                  "system.h"
#include                  "screen.h"

t_isr                     interrupt_handlers[256];

char                      *exception_messages[] =
{
  "Division By Zero",
  "Debug",
  "Non Maskable Interrupt",
  "Breakpoint",
  "Into Detected Overflow",
  "Out of Bounds",
  "Invalid Opcode",
  "No Coprocessor",
  "Double Fault",
  "Coprocessor Segment Overrun",
  "Bad TSS",
  "Segment Not Present",
  "Stack Fault",
  "General Protection Fault",
  "Page Fault",
  "Unknown Interrupt",
  "Coprocessor Fault",
  "Alignment Check",
  "Machine Check",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved"
};

void                      register_interrupt_handler(unsigned char n, t_isr handler)
{
  interrupt_handlers[n] = handler;
}

void                      isr_handler(struct s_regs *r)
{
  if (r->int_no < 32)
  {
    printk(COLOR_RED, exception_messages[r->int_no]);
    printk(COLOR_RED, " Exception. System Halted!\n");
    printk(COLOR_WHITE, "EIP = 0x");
    printk(COLOR_WHITE, my_putnbr_base(r->eip, "0123456789ABCDEF"));
    printk(COLOR_WHITE, "\n ESP = 0x");
    printk(COLOR_WHITE, my_putnbr_base(r->esp, "0123456789ABCDEF"));
    printk(COLOR_WHITE, "\n USER ESP = 0x");
    printk(COLOR_WHITE, my_putnbr_base(r->useresp, "0123456789ABCDEF"));
    printk(COLOR_WHITE, "\n");
    for (;;);
  }
  else if (interrupt_handlers[r->int_no] != 0)
  {
    t_isr handler = interrupt_handlers[r->int_no];
    handler(r);
  }
}

void                      irq_handler(struct s_regs *r)
{
  if (r->int_no >= 40)
    outportb(0xA0, 0x20);

  outportb(0x20, 0x20);

  if (interrupt_handlers[r->int_no] != 0)
  {
    t_isr handler = interrupt_handlers[r->int_no];
    handler(r);
  }
}

void                      init_isrs()
{
  idt_set_gate(0, (unsigned)isr0, 0x08, 0x8E);
  idt_set_gate(1, (unsigned)isr1, 0x08, 0x8E);
  idt_set_gate(2, (unsigned)isr2, 0x08, 0x8E);
  idt_set_gate(3, (unsigned)isr3, 0x08, 0x8E);
  idt_set_gate(4, (unsigned)isr4, 0x08, 0x8E);
  idt_set_gate(5, (unsigned)isr5, 0x08, 0x8E);
  idt_set_gate(6, (unsigned)isr6, 0x08, 0x8E);
  idt_set_gate(7, (unsigned)isr7, 0x08, 0x8E);
  idt_set_gate(8, (unsigned)isr8, 0x08, 0x8E);
  idt_set_gate(9, (unsigned)isr9, 0x08, 0x8E);
  idt_set_gate(10, (unsigned)isr10, 0x08, 0x8E);
  idt_set_gate(11, (unsigned)isr11, 0x08, 0x8E);
  idt_set_gate(12, (unsigned)isr12, 0x08, 0x8E);
  idt_set_gate(13, (unsigned)isr13, 0x08, 0x8E);
  idt_set_gate(14, (unsigned)isr14, 0x08, 0x8E);
  idt_set_gate(15, (unsigned)isr15, 0x08, 0x8E);
  idt_set_gate(16, (unsigned)isr16, 0x08, 0x8E);
  idt_set_gate(17, (unsigned)isr17, 0x08, 0x8E);
  idt_set_gate(18, (unsigned)isr18, 0x08, 0x8E);
  idt_set_gate(19, (unsigned)isr19, 0x08, 0x8E);
  idt_set_gate(20, (unsigned)isr20, 0x08, 0x8E);
  idt_set_gate(21, (unsigned)isr21, 0x08, 0x8E);
  idt_set_gate(22, (unsigned)isr22, 0x08, 0x8E);
  idt_set_gate(23, (unsigned)isr23, 0x08, 0x8E);
  idt_set_gate(24, (unsigned)isr24, 0x08, 0x8E);
  idt_set_gate(25, (unsigned)isr25, 0x08, 0x8E);
  idt_set_gate(26, (unsigned)isr26, 0x08, 0x8E);
  idt_set_gate(27, (unsigned)isr27, 0x08, 0x8E);
  idt_set_gate(28, (unsigned)isr28, 0x08, 0x8E);
  idt_set_gate(29, (unsigned)isr29, 0x08, 0x8E);
  idt_set_gate(30, (unsigned)isr30, 0x08, 0x8E);
  idt_set_gate(31, (unsigned)isr31, 0x08, 0x8E);
  idt_set_gate(128, (unsigned)isr128, 0x08, 0x8E);

  /*IRQ*/
  idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
  idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
  idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
  idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
  idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
  idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
  idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);
  idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
  idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
  idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
  idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
  idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
  idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
  idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
  idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
}

