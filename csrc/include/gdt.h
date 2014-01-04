/************************************************
*
* Micro Kernel
*
* - GDT main init funcs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef               __GDT_H__
# define              __GDT_H__

# define              BASE        0x0;
# define              LIMIT       0xFFFFFFFF;

struct                s_gdt_entry
{
  unsigned            limit_low   : 16;
  unsigned            base_low    : 16;
  unsigned            base_middle : 8;
  unsigned            access      : 8;
  unsigned            granularity : 8;
  unsigned            base_high   : 8;
}                     __attribute__((packed));

struct                s_gdt_pseudo_desc
{
  unsigned            limit       : 16;
  unsigned            base        : 32;
}                     __attribute__((packed));

void                  init_gdt();
extern void           flush_gdt();

#endif                /*__GDT_H__*/
