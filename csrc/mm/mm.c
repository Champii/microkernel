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
#include                  "task.h"
#include                  "idt.h"
#include                  "kmalloc.h"
#include                  "mm.h"
#include                  "isrs.h"

t_page_directory          *page_dir = 0;
t_page_directory          *cur_dir=0;
t_page_table              *page_table = 0;
t_free_block		  *free_block = 0;

unsigned                  *frames;
unsigned                  nframes;

extern void               copy_page_physical(unsigned src, unsigned dest);

unsigned                  virt_to_phys(unsigned virt)
{
  unsigned                phys;
  unsigned                offset;
  unsigned                table_idx;

  virt /= 0x1000;
  table_idx = virt / 1024;
  offset = virt % 0x1000;

  phys = (page_dir->tables[table_idx]->pages[virt%1024].frame * 0x1000) + offset;

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

unsigned                  first_frame()
{
  unsigned                i, j;

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

/*

void init_struct_mm_block() {
  int i = 2;
  t_free_block* tmp = free_block;

  free_block = kmalloc(sizeof(t_free_block));

  tmp->p_prev = NULL;
  while (i <= 4096) {
     tmp->block = kmalloc(i);
     tmp->p_next = free_block
     tmp->p_next->p_prev = tmp;
     tmp = tmp->p_next;
     i ^= 2;
  }
}

int list_size(void* list) {
   int i = 0;
   if (list->p_next != NULL)
     i++;
   return i;
}

static void* first_fit(int q_block_size) {
  int i = 0;
  int list_len = list_size(block_size);
  t_free_block *tmp = free_block;
  void* block_ret;

  while (tmp->p_next != NULL) {
    if (sizeof(tmp->block) >= q_block_size) {
      block_ret = free_block->block;
      t_free_block* new_free_block = kmalloc(sizeof(t_free_block));

      tmp->p_prev->p_next = new_free_block;
      new_free_block->p_prev = tmp->p_prev;
      break;
     }
    tmp = tmp->p_next;
  }

  // return sizeof(block_ret);
  return block_ret;
}
*/
int                       alloc_page(t_page *page, int is_kernel, int is_writeable)
{
  if (page->frame != 0)
    return -1;

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

  return 0;

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
  set_frame(phys);
  page->present = 1;
  page->rw = (is_writeable)?1:0;
  page->user = (is_kernel)?0:1;
  page->frame = phys / 0x1000;
}

void                      initial_map(unsigned virt, unsigned size)
{
  int                     i;

  i = 0;
  while (i < size + 0x1000)
  {
    alloc_page_at(i, get_page(virt + i, 1, page_dir), 0, 1);
    i += 0x1000;
  }
}


static t_page_table       *clone_table(t_page_table *src, unsigned int *physAddr)
{
  int                     i;
  t_page_table            *table = kmalloc_ap(sizeof(t_page_table), physAddr);

  memset(table, 0, sizeof(t_page_directory));

  for (i = 0; i < 1024; i++)
  {
    if (src->pages[i].frame)
    {
      alloc_page(&table->pages[i], 0, 0);

      if (src->pages[i].present) table->pages[i].present = 1;
      if (src->pages[i].rw) table->pages[i].rw = 1;
      if (src->pages[i].user) table->pages[i].user = 1;
      if (src->pages[i].accessed) table->pages[i].accessed = 1;
      if (src->pages[i].dirty) table->pages[i].dirty = 1;
      copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
    }
  }
  return table;
}

t_page_directory          *clone_directory(t_page_directory *src)
{
  unsigned int          phys;

  t_page_directory      *dir = (t_page_directory*)kmalloc_ap(sizeof(t_page_directory), &phys);

  memset(dir, 0, sizeof(t_page_directory));

  unsigned int offset = (unsigned int)dir->tablesPhysical - (unsigned int)dir;

  dir->physicalAddr = phys + offset;

  int i;
  for (i = 0; i < 1024; i++)
  {
    if (!src->tables[i])
      continue;

    if (page_dir->tables[i] == src->tables[i])
    {
      dir->tables[i] = src->tables[i];
      dir->tablesPhysical[i] = src->tablesPhysical[i];
    }
    else
    {
      unsigned int phys;
      dir->tables[i] = clone_table(src->tables[i], &phys);
      dir->tablesPhysical[i] = phys | 0x07;
    }
  }
  return dir;
}

t_page_directory          *create_empty_directory()
{
  unsigned int            phys;
  t_page_directory        *src = page_dir;
  t_page_directory        *dir = (t_page_directory*)kmalloc_ap(sizeof(t_page_directory), &phys);

  memset(dir, 0, sizeof(t_page_directory));

  unsigned int offset = (unsigned int)dir->tablesPhysical - (unsigned int)dir;
  dir->physicalAddr = phys + offset;

  int i;
  for (i = 0; i < 1024; i++)
  {
    if (i >= 768 || i <= 2)
    {
      dir->tables[i] = src->tables[i];
      dir->tablesPhysical[i] = src->tablesPhysical[i];
    }

  }
  return dir;
}

void                      invalid_opcode(struct s_regs *regs)
{
  printk(COLOR_RED, "INVALID OPCODE AT : 0x");
  printk(COLOR_RED, my_putnbr_base(regs->eip, "0123456789ABCDEF"));
  printk(COLOR_RED, "\n");
  for(;;);
}

void                      general_protection_exception(struct s_regs *regs)
{
  printk(COLOR_RED, "General protection exception AT EIP : 0x");
  printk(COLOR_RED, my_putnbr_base(regs->eip, "0123456789ABCDEF"));
  printk(COLOR_RED, "\n");
  for(;;);
}

void                      page_fault(struct s_regs *regs)
{
  unsigned                faulting_address;

  int                     present = regs->err_code & 0x1;
  int                     rw = regs->err_code & 0x2;
  int                     us = regs->err_code & 0x4;
  int                     reserved = regs->err_code & 0x8;

  asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

  printk(COLOR_WHITE, "\nPage fault! ( ");


  if (present) {printk(COLOR_WHITE, "protection violation ");} else { printk(COLOR_WHITE, "not present ");}
  if (rw) {printk(COLOR_WHITE, "write-error ");} else { printk(COLOR_WHITE, "read-error "); }
  if (us) {printk(COLOR_WHITE, "user-mode ");} else { printk(COLOR_WHITE, "supervisor-mode ");}
  if (reserved) {printk(COLOR_WHITE, "reserved ");}

  printk(COLOR_WHITE, ") at 0x");
  printk(COLOR_WHITE, my_putnbr_base(faulting_address, "0123456789ABCDEF"));
  printk(COLOR_WHITE, "\n");
  printk(COLOR_WHITE, " PID = ");
  printk(COLOR_WHITE, my_putnbr_base(getpid(), "0123456789"));
  printk(COLOR_WHITE, "\n");
  printk(COLOR_WHITE, "Page fault\n");

  for(;;);
}

void                      switch_page_directory(t_page_directory *new_dir)
{

  __asm__ volatile("mov %0, %%cr3":: "b"(new_dir->physicalAddr));
}

void                      invlpg(void *vaddr)
{
  unsigned addr = 0;
  __asm__ volatile("mov %%cr3, %0": "+b"(addr));
  __asm__ volatile("mov %0, %%cr3":: "b"(addr));

  // __asm__ volatile("invlpg (%0)":: "r"(vaddr) : "memory");
}

void                      init_page_dir()
{
  unsigned                page_dir_phys;

  unsigned                mem_end_page = 0xFFFFFF;

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

  idt_set_gate(6, (unsigned)&invalid_opcode, 0x08, 0x8E);
  // idt_set_gate(13, (unsigned)&general_protection_exception, 0x08, 0x8E);
  idt_set_gate(14, (unsigned)&page_fault, 0x08, 0x8E);

  cur_dir = page_dir;
  switch_page_directory(page_dir);

}



void                      init_mm()
{
  init_page_dir();
}
