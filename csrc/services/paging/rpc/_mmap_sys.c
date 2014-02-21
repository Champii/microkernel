/************************************************
*
* Micro Kernel
*
* - Program Loader Service_pid RPC
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <rpc/rpc.h>
#include                  <rpc.h>
#include                  <mm.h>
#include                  <get_argument.h>

unsigned pl_map_start = 0x30000000;

void                      _mmap_sys_rpc(u64 sender, void *params, void **ret, unsigned *ret_size)
{
  sender = sender;
  params = params;
  ret = ret;
  ret_size = ret_size;

  unsigned vaddr = get_unsigned_arg(&params);
  int rights = get_int_arg(&params);
  unsigned size = get_unsigned_arg(&params);
  unsigned *pid_split = (unsigned *)&sender;

  rights = rights;

  // get page dir
  t_page_directory *pd = get_as_from_pid(pid_split[0]);
  pid_split = (unsigned *)&prog_loader_pid;
  t_page_directory *pl_pd = get_as_from_pid(pid_split[0]);

  // alloc page

  unsigned pl_virt_start = pl_map_start;
  unsigned i;
  for (i = 0; i < size; i += 0x1000)
  {
    t_page *page = get_page(vaddr + i, 0, pd);

    if (!page)
    {
      *ret = 0;
      *ret_size = sizeof(unsigned);
      return ;
    }

    page = get_page(vaddr + i, 1, pd);

    alloc_page(page, 0, 1);

    alloc_page_at(page->frame * 0x1000, get_page(pl_map_start, 1, pl_pd), 0, 1);
    pl_map_start += 0x1000;

  }

  // return pointer
  *ret = (void *)pl_virt_start;
  *ret_size = sizeof(unsigned);

}
