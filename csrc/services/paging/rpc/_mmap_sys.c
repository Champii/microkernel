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
#include                  <unistd.h>
#include                  <get_argument.h>

unsigned pl_map_start = 0x30000000;

int itoa_base(int n, char *str, unsigned base);
int uitoa_base(unsigned n, char *str, unsigned base);

void                      _mmap_sys_rpc(u64 sender, void *params, void **ret, unsigned *ret_size)
{
  sender = sender;
  params = params;
  ret = ret;
  ret_size = ret_size;

  u64 pid = get_u64_arg(&params);
  unsigned vaddr = get_unsigned_arg(&params);
  int rights = get_int_arg(&params);
  unsigned size = get_unsigned_arg(&params);
  unsigned *pid_split = (unsigned *)&pid;

  rights = rights;
  pid = pid;
  char tmp[10];

  // get page dir
  t_page_directory *pd = get_as_from_pid(pid_split[0]);
  uitoa_base((unsigned)pd, tmp, 16);
  kwrite(15, "MMAP SYS pl_pid addr = 0x", 0);
  kwrite(15, tmp, 0);
  kwrite(15, "\n", 0);
  pid_split = (unsigned *)&prog_loader_pid;
  t_page_directory *pl_pd = get_as_from_pid(*(unsigned *)(pid_split[1]));

  // alloc page

  unsigned pl_virt_start = pl_map_start;
  unsigned i;


  for (i = 0; i < size; i += 0x1000)
  {
    // t_page *page = get_page(vaddr + i, 0, pd);


    t_page *page = get_page(vaddr + i, 1, pd);

    if (page->frame)
    {
      kwrite(4, "MMAP SYS : Existing page ! Abort.\n", 0);
      *ret = 0;
      *ret_size = sizeof(unsigned);
      return ;
    }

    alloc_page(page, 0, 1);
    itoa_base((int)page->frame, tmp, 16);
    kwrite(15, "MMAP SYS PAGE FRAME 1 = ", 0);
    kwrite(15, tmp, 0);
    kwrite(15, "\n", 0);

    alloc_page_at(page->frame * 0x1000, get_page(pl_map_start, 1, pl_pd), 0, 1);

    page = get_page(pl_map_start, 0, pl_pd);
    itoa_base((int)page->frame, tmp, 16);
    kwrite(15, "MMAP SYS PAGE FRAME 2 = ", 0);
    kwrite(15, tmp, 0);
    kwrite(15, "\n", 0);
    pl_map_start += 0x1000;

  }

  // return pointer
  *ret = (void *)pl_virt_start;
  *ret_size = sizeof(unsigned);

}
