/************************************************
*
* Micro Kernel
*
* - GDT main init funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  "gdt.h"
#include                  "system.h"
#include                  "kmalloc.h"

struct s_gdt_entry        gdt[5];
struct s_gdt_pseudo_desc  gdt_desc;

struct s_tss              tss_entry;

extern void               flush_tss();

extern unsigned           initial_esp;

void                      init_segment(int seg_num, unsigned long base, unsigned long limit, unsigned char granularity, unsigned char access)
{
  gdt[seg_num].base_low = base & 0xFFFF;
  gdt[seg_num].base_middle = (base >> 16) & 0xFF;
  gdt[seg_num].base_high = (base >> 24) & 0xFF;

  gdt[seg_num].limit_low = limit & 0xFFFF;

  gdt[seg_num].granularity = (limit >> 16) & 0x0F;
  gdt[seg_num].granularity |= (granularity << 4) & 0xF0;

  gdt[seg_num].access = access;
}

static void               write_tss(int num, unsigned short ss0, unsigned esp0)
{
  unsigned                base;
  unsigned                limit;

  // tss_entry = kmalloc(sizeof(*tss_entry));

  base = (unsigned) &tss_entry;
  limit = base + sizeof(tss_entry);

  init_segment(num, base, limit, 0x00, 0xE9);
  memset(&tss_entry, 0, sizeof(tss_entry));

  tss_entry.ss0  = ss0;
  tss_entry.esp0 = esp0;

  tss_entry.cs   = 0x0b;
  tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}

void                      set_kernel_stack(unsigned stack)
{
   tss_entry.esp0 = stack;
}

void                      init_gdt()
{
  gdt_desc.limit = (sizeof(struct s_gdt_entry) * 6) - 1;
  gdt_desc.base = (unsigned int)&gdt;

  /* Null segment */
  init_segment(0, 0, 0, 0, 0);

  /*Ring 0*/
  /* Code segment */
  init_segment(1, BASE, LIMIT, 0xC, 0x9A);
  /* Data segment */
  init_segment(2, BASE, LIMIT, 0xC, 0x92);

  /*Ring 3*/
  /* Code segment */
  init_segment(3, BASE, LIMIT, 0xC, 0xFA);
  /* Data segment */
  init_segment(4, BASE, LIMIT, 0xC, 0xF2);

  /*TSS*/
  write_tss(5, 0x10, 0);

  flush_gdt();
  flush_tss();
}
