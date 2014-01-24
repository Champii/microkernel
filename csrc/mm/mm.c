/************************************************
*
* Micro Kernel
*
* - Standard memory functs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/
#include                  "system.h"
#include                  "screen.h"
#include                  "idt.h"
#include                  "mm.h"


extern unsigned           end;
unsigned                  placement_address = (unsigned)&end;

t_page_directory          *page_dir = 0;
t_page_table              *page_table = 0;

unsigned                  *frames;
unsigned                  nframes;

static void               set_frame(unsigned frame_addr)
{
  unsigned               frame = frame_addr/0x1000;
  unsigned               idx = INDEX_FROM_BIT(frame);
  unsigned               off = OFFSET_FROM_BIT(frame);

  frames[idx] |= (0x1 << off);
}

// UNUSED YET

// static void               clear_frame(unsigned frame_addr)
// {
//   unsigned               frame = frame_addr/0x1000;
//   unsigned               idx = INDEX_FROM_BIT(frame);
//   unsigned               off = OFFSET_FROM_BIT(frame);

//   frames[idx] &= ~(0x1 << off);
// }

// static unsigned           test_frame(unsigned frame_addr)
// {
//   unsigned               frame = frame_addr/0x1000;
//   unsigned               idx = INDEX_FROM_BIT(frame);
//   unsigned               off = OFFSET_FROM_BIT(frame);

//   return (frames[idx] & (0x1 << off));
// }

static unsigned          first_frame()
{
  unsigned               i, j;

  for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
  {
    if (frames[i] != 0xFFFFFFFF)
    {
      for (j = 0; j < 32; j++)
      {
        unsigned         toTest = 0x1 << j;

        if (!(frames[i] & toTest))
          return i*4*8+j;
      }
    }
  }

  return (-1);
}

void                      *kmalloc(unsigned size)
{
  void                    *tmp;

  if (placement_address & 0xFFFFF000)
  {
    placement_address &= 0xFFFFF000;
    placement_address += 0x1000;
  }

  tmp = (void *)placement_address;
  placement_address += size;
  return (tmp);
}

void                      alloc_page(t_page *page, int is_kernel, int is_writeable)
{
  if (page->frame != 0)
    return ;

  else
  {
    unsigned           idx = first_frame();

    if (idx == (unsigned)-1)
    {
      printk(COLOR_RED, "No free frames!");
      for (;;);
    }

    set_frame(idx * 0x1000);
    page->present = 1;
    page->rw = (is_writeable)?1:0;
    page->user = (is_kernel)?0:1;
    page->frame = idx;
  }
}

t_page                    *get_page(unsigned address, int make, t_page_directory *dir)
{
  address /= 0x1000;
  unsigned                table_idx = address / 1024;

  if (dir->tables[table_idx])
    return &dir->tables[table_idx]->pages[address%1024];
  else if (make)
  {
    dir->tables[table_idx] = (t_page_table*)kmalloc(sizeof(t_page_table));
    memset(dir->tables[table_idx], 0, 0x1000);
    dir->tablesPhysical[table_idx] = ((unsigned)dir->tables[table_idx]) | 0x3;
    return &dir->tables[table_idx]->pages[address%1024];
  }
  else
    return 0;
}

void                      set_page_dir(t_page_directory *new)
{
  unsigned                cr0;

  __asm__ volatile("mov %0, %%cr3":: "b"(&new->tablesPhysical));

  __asm__ volatile("mov %%cr0, %0": "=b"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  __asm__ volatile("mov %0, %%cr0":: "b"(cr0));

}

void                      page_fault(t_registers regs)
{
  unsigned              faulting_address;

  int                   present = !(regs.err_code & 0x1);
  int                   rw = regs.err_code & 0x2;
  int                   us = regs.err_code & 0x4;
  int                   reserved = regs.err_code & 0x8;

  asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

  printk(COLOR_WHITE, "Page fault! ( ");

  if (present) {printk(COLOR_WHITE, "present ");}
  if (rw) {printk(COLOR_WHITE, "read-only ");}
  if (us) {printk(COLOR_WHITE, "user-mode ");}
  if (reserved) {printk(COLOR_WHITE, "reserved ");}

  printk(COLOR_WHITE, ") at 0x");
  // printk_COLOR_WHITE, hex(faulting_address);
  printk(COLOR_WHITE, "\n");
  printk(COLOR_WHITE, "Page fault");
}

void                      alloc_page_at(unsigned phys, t_page *page, int is_kernel, int is_writeable)
{
  if (page->frame != 0)
    return ;
  else
  {
    set_frame(phys);
    page->present = 1;
    page->rw = (is_writeable)?1:0;
    page->user = (is_kernel)?0:1;
    page->frame = phys / 0x1000;
  }
}

void                      initial_map(unsigned virt, unsigned size)
{
  int                     i;

  i = 0;
  while (i < size)
  {
    alloc_page_at(i, get_page(virt + i, 1, page_dir), 0, 0);
    i += 0x1000;
  }
}

void                      init_page_dir()
{
  int                     i;

  unsigned                mem_end_page = 0xFFFFFFFF;

  nframes = mem_end_page / 0x1000;
  frames = kmalloc(INDEX_FROM_BIT(nframes));
  memset(frames, 0, INDEX_FROM_BIT(nframes));

  page_dir = kmalloc(sizeof(*page_dir));
  memset(page_dir, 0, sizeof(*page_dir));

  // identity map : Bios and kernel
  initial_map(0x0, 1024 * 0x1000);

  printk(COLOR_WHITE, "Table 1: \n");
  my_put_nbr(page_dir->tables[0]);
  printk(COLOR_WHITE, "\n");

  // Bios and kernel in high
  initial_map(0xC0000000, 1024 * 0x1000);

  printk(COLOR_WHITE, "Table 2: \n");
  my_put_nbr(page_dir->tables[768]);
  printk(COLOR_WHITE, "\n");

 for(;;);

  // TODO
  // idt_set_gate(14, &page_fault, 0x08, 0x8E);

  set_page_dir(page_dir);
}

void                      init_mm()
{
  init_page_dir();

}
