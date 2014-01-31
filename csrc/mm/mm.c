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
#include                  "kmalloc.h"
#include                  "mm.h"

t_page_directory          *page_dir = 0;
t_page_table              *page_table = 0;

unsigned                  *frames;
unsigned                  nframes;

// t_mem_block               *memory = 0;

// void                      *phys_alloc(unsigned size)
// {
//   t_mem_block             *tmp;
//   struct list_head        *pos = &memory->list;

//   list_for_each(pos, &memory->list)
//   {
//     printk(COLOR_WHITE, "HEAY");
//     tmp = list_entry(pos, t_mem_block, list);

//     if (!tmp->used && tmp->size >= size)
//     {
//       tmp->used = 1;
//       tmp->size = size;

//       if (tmp->size & 0xFFFFF000)
//       {
//         tmp->size &= 0xFFFFF000;
//         tmp->size += 0x1000;
//       }

//       if (tmp->size > size)
//       {
//         t_mem_block       *new;

//         new = kmalloc(sizeof(t_mem_block));
//         memset(new, 0, sizeof(t_mem_block));

//         list_add_tail(&new->list, &memory->list);
//       }
//     }
//   }
//   return 0;
// }

// void                      init_dyn_mem()
// {
//   void                    *test = 0;

//   memory = kmalloc(sizeof(t_mem_block));
//   memset(memory, 0, sizeof(t_mem_block));

//   INIT_LIST_HEAD(&memory->list);
//   memory->used = 0;
//   memory->size = 0x00100000;

//   test = phys_alloc(100);

//   printk(COLOR_WHITE, "TEST : ");
//   printk(COLOR_WHITE, my_putnbr_base(test, "0123456789ABCDEF"));
//   printk(COLOR_WHITE, "\n");
// }

void                      alloc_reserved_hard()
{

}


unsigned                  virt_to_phys(unsigned virt)
{
  unsigned                phys;
  unsigned                offset;
  unsigned                table_idx;

  offset = virt % 0x1000;
  virt /= 0x1000;
  table_idx = virt / 1024;

  phys = (page_dir->tables[table_idx]->pages[virt%1024].frame * 0x1000) + offset;
  // printk(COLOR_WHITE, "Virt to phys: ");
  // printk(COLOR_WHITE, my_putnbr_base(virt, "0123456789ABCDEF"));
  // printk(COLOR_WHITE, " -> ");
  // printk(COLOR_WHITE, my_putnbr_base(phys, "0123456789ABCDEF"));
  // printk(COLOR_WHITE, "\n");

  return (phys);
}

unsigned                  phys_to_virt(unsigned phys)
{
  return (0);
}

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
    unsigned              phys;

    dir->tables[table_idx] = (t_page_table*)kmalloc_ap(sizeof(t_page_table), &phys);
    memset(dir->tables[table_idx], 0, sizeof(t_page_table));

    dir->tablesPhysical[table_idx] = phys | 0x7;
    return &dir->tables[table_idx]->pages[address%1024];
  }
  else
    return 0;
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
    alloc_page_at(i, get_page(virt + i, 1, page_dir), 1, 1);
    i += 0x1000;
  }
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

void                      init_page_dir()
{
  unsigned                page_dir_phys;

  unsigned                mem_end_page = 0xFFFFFFFF;

  nframes = mem_end_page / 0x1000;
  frames = kmalloc(INDEX_FROM_BIT(nframes));
  memset(frames, 0, INDEX_FROM_BIT(nframes));

  page_dir = kmalloc_ap(sizeof(*page_dir), &page_dir_phys);
  memset(page_dir, 0, sizeof(*page_dir));
  page_dir->physicalAddr = page_dir_phys + sizeof(page_dir->tables);

  // identity map : Bios and kernel
  initial_map(0x0, 1024 * 0x1000);

  // Bios and kernel in high
  initial_map(0xC0000000, 1024 * 0x1000);

  // TODO
  // idt_set_gate(14, &page_fault, 0x08, 0x8E);

  __asm__ volatile("mov %0, %%cr3":: "b"(page_dir->physicalAddr));
}

void                      init_mm()
{
  init_page_dir();
  // init_dyn_mem();

}
