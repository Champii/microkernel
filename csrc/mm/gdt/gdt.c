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

struct s_gdt_entry        gdt[3];
struct s_gdt_pseudo_desc  gdt_desc;

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

void                      init_gdt()
{
  gdt_desc.limit = (sizeof(struct s_gdt_entry) * 3) - 1;
  gdt_desc.base = (unsigned int)&gdt;

  /* Null segment */
  init_segment(0, 0, 0, 0, 0);

  /* Code segment */
  init_segment(1, BASE, LIMIT, 0xC, 0x9A);

  /* Data segment */
  init_segment(2, BASE, LIMIT, 0xC, 0x92);

  flush_gdt();
}
