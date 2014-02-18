/************************************************
*
* Micro Kernel
*
* - Paging Service  Memory Management
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef               __MM_H__
# define              __MM_H__

#define               INDEX_FROM_BIT(a) (a/(8*4))
#define               OFFSET_FROM_BIT(a) (a%(8*4))

typedef struct        s_page
{
  unsigned            present    : 1;
  unsigned            rw         : 1;
  unsigned            user       : 1;
  unsigned            accessed   : 1;
  unsigned            dirty      : 1;
  unsigned            unused     : 7;
  unsigned            frame      : 20;
}                     t_page;

typedef struct        s_page_table
{
  t_page              pages[1024];
}                     t_page_table;

typedef struct        s_page_directory
{
  t_page_table        *tables[1024];
  unsigned            tablesPhysical[1024];
  unsigned            physicalAddr;
}                     t_page_directory;

unsigned              virt_to_phys(unsigned virt);
unsigned              phys_to_virt(unsigned phys);

void                  init_mm();
unsigned              test_frame(unsigned frame_addr);
t_page                *get_page(unsigned, int, t_page_directory *);
int                   alloc_page(t_page *page, int is_kernel, int is_writeable);
void                  alloc_page_at(unsigned phys, t_page *page, int is_kernel, int is_writeable);
t_page_directory      *get_as_from_pid(unsigned pid);
/*LOOK stos (asm) => page table*/

#endif                /*__MM_H__*/
