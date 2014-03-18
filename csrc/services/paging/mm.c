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
int itoa_base(int n, char *str, unsigned size);

t_page_directory          *processes_pd = (t_page_directory *)0x20000000;

t_page_directory          *paging_dir;

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

  phys = (paging_dir->tables[table_idx]->pages[virt%1024].frame * 0x1000) + offset;

  return (phys);
}

unsigned                  phys_to_virt(unsigned phys)
{
  phys = phys;
  return (0);
}

void                      set_frame(unsigned frame_addr)
{
  unsigned                frame = frame_addr/0x1000;
  unsigned                idx = INDEX_FROM_BIT(frame);
  unsigned                off = OFFSET_FROM_BIT(frame);

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
  unsigned                frame = frame_addr/0x1000;
  unsigned                idx = INDEX_FROM_BIT(frame);
  unsigned                off = OFFSET_FROM_BIT(frame);

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
  return (t_page_directory *)((char *)processes_pd + ((pid - 1) * 0x1000 * 1024) + ((pid - 1) * 0x3000));
}

int uitoa_base(unsigned n, char *str, unsigned base);

void                      prepare_paging()
{
  t_page_directory *pl_dir = get_as_from_pid(1);
  paging_dir = get_as_from_pid(2);
  t_page_directory *io_dir = get_as_from_pid(3);


  unsigned i;
  unsigned j = 0;

  memset(pl_dir->tables, 0, 0x1000);
  for (i = (unsigned)((char *)pl_dir + 0x3000); i < (unsigned)((char *)pl_dir + (1024 * 0x1000) + 0x3000); i += 0x1000)
    pl_dir->tables[j++] = (t_page_table *)i;

  memset(paging_dir->tables, 0, 0x1000);
  j = 0;
  for (i = (unsigned)((char *)paging_dir + 0x3000); i < (unsigned)((char *)paging_dir + (1024 * 0x1000) + 0x3000); i += 0x1000)
    paging_dir->tables[j++] = (t_page_table *)i;

  memset(io_dir->tables, 0, 0x1000);
  j = 0;
  for (i = (unsigned)((char *)io_dir + 0x3000); i < (unsigned)((char *)io_dir + (1024 * 0x1000) + 0x3000); i += 0x1000)
    io_dir->tables[j++] = (t_page_table *)i;
}

void                      init_page_dir()
{
  prepare_paging();
  frames = kmalloc(INDEX_FROM_BIT(0xFFFFFF / 0x1000));
  memset(frames, 0, INDEX_FROM_BIT(0xFFFFFF / 0x1000));

  unsigned i;
  for (i = 0; i < start_frame; i++)
    set_frame(i * 0x1000);
}

void                      init_mm()
{
  init_page_dir();
}


