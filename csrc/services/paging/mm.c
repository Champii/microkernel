/************************************************
*
* Micro Kernel
*
* - Paging Service Memory Management
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <string.h>
#include                  <sys/sys_types.h>
#include                  <unistd.h>
#include                  <mm.h>

/*
*   Reserved spaces in paging service addess space :
*   0x10000000 -> temporary heap (memory init)
*   0x20000000 -> services/programs page directory
*/

extern unsigned           pd_addr;

extern unsigned           start_frame;

unsigned                  *frames;
unsigned                  nframes;


t_page_directory          *processes_pd = (t_page_directory *)0x20000000;

t_page_directory          *paging_pd;

unsigned                  temp_heap = 0x30000000;

static void               *_kmalloc(unsigned size, int align, unsigned *phys)
{
  void                    *tmp;

  if (align && temp_heap & 0xFFFFF000)
  {
    temp_heap &= 0xFFFFF000;
    temp_heap += 0x1000;
  }
  if (phys)
    *phys = virt_to_phys(temp_heap);

  tmp = (void *)temp_heap;
  temp_heap += size;

  // printk(COLOR_WHITE, "KMALLOC = ");
  // printk(COLOR_WHITE, my_putnbr_base(size, "0123456789"));
  // printk(COLOR_WHITE, "\n");
  return (tmp);
}

void                      *kmalloc(unsigned size)
{
  return _kmalloc(size, 0, 0);
}

void                      *kmalloc_a(unsigned size)
{
  return _kmalloc(size, 1, 0);
}

void                      *kmalloc_p(unsigned size, unsigned *phys)
{
  return _kmalloc(size, 0, phys);
}

void                      *kmalloc_ap(unsigned size, unsigned *phys)
{
  return _kmalloc(size, 1, phys);
}


unsigned                  virt_to_phys(unsigned virt)
{
  unsigned                phys;
  unsigned                offset;
  unsigned                table_idx;

  virt /= 0x1000;
  table_idx = virt / 1024;
  offset = virt % 0x1000;

  phys = (paging_pd->tables[table_idx]->pages[virt%1024].frame * 0x1000) + offset;

  return (phys);
}

unsigned                  phys_to_virt(unsigned phys)
{
  phys = phys;
  return (0);
}

void               set_frame(unsigned frame_addr)
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

unsigned                  test_frame(unsigned frame_addr)
{
  unsigned               frame = frame_addr/0x1000;
  unsigned               idx = INDEX_FROM_BIT(frame);
  unsigned               off = OFFSET_FROM_BIT(frame);

  return (frames[idx] & (0x1 << off));
}

static unsigned          first_frame()
{
  unsigned               i, j;

  for (i = 0; i < INDEX_FROM_BIT(0xFFFFFF / 0x1000); i++)
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

int                       alloc_page(t_page *page, int is_kernel, int is_writeable)
{
  if (page->frame != 0)
    return -1;

  unsigned           idx = first_frame();

  if (idx == (unsigned)-1)
  {
    kwrite(4, "No free frames!", 0);
    for (;;);
  }

  set_frame(idx * 0x1000);
  page->present = 1;
  page->rw = (is_writeable)?1:0;
  page->user = (is_kernel)?0:1;
  page->frame = idx;

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

t_page_directory          *get_as_from_pid(unsigned pid)
{
  return processes_pd + (pid - 1);
}

int uitoa_base(unsigned n, char *str, unsigned base);

void                      init_page_dir()
{
  // unsigned                mem_end_page = 0xFFFFFF;

  // processes_pd = (t_page_directory *)pd_addr;

  paging_pd = processes_pd + 1;

  // mem_end_page = mem_end_page;
  // nframes = 0xFFFFFF / 0x1000;
  frames = kmalloc(INDEX_FROM_BIT(0xFFFFFF / 0x1000));
  memset(frames, 0, INDEX_FROM_BIT(0xFFFFFF / 0x1000));

  // char tmp[10];
  // memset(tmp, 0, 10);

  // uitoa_base((unsigned)start_frame, tmp, 10);
  // kwrite(15, "lol ", 0);
  // kwrite(15, tmp, 0);
  unsigned i;
  for (i = 0; i < start_frame; i++)
  {
    set_frame(i * 0x1000);
  }

  // kwrite(15, "lol2", 0);
}

void                      init_mm()
{
  init_page_dir();
  // init_dyn_mem();

}


