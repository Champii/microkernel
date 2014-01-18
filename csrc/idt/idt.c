#include "idt.h"
#include "system.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
  /* We'll leave you to try and code this function: take the
   *  argument 'base' and split it up into a high and low 16-bits,
   *  storing them in idt[num].base_hi and base_lo. The rest of the
   *  fields that you must set in idt[num] are fairly self-
   *  explanatory when it comes to setup */

  /* The interrupt routine's base address */
  idt[num].base_lo = (base & 0xFFFF);
  idt[num].base_hi = (base >> 16) & 0xFFFF;

  /* The segment or 'selector' that this IDT entry will use
   *  is set here, along with any access flags */
  idt[num].sel = sel;
  idt[num].always0 = 0;
  idt[num].flags = flags;
}

void init_idt()
{
  /* Sets the special IDT pointer up, just like in 'gdt.c' */
  idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
  idtp.base = &idt;

  /* Clear out the entire IDT, initializing it to zeros */
  memset(&idt, 0, sizeof(struct idt_entry) * 256);

  /* Add any new ISRs to the IDT here using idt_set_gate */
  
  //idt_set_gate();

  /* Points the processor's internal register to the new IDT */
  idt_load();
}
