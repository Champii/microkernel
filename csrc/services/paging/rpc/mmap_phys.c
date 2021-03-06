/************************************************
*
* Micro Kernel
*
* - Program Loader Exit RPC
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <rpc.h>
#include                  <mm.h>
#include                  <get_argument.h>

void                      mmap_phys_rpc(u64 sender, void *params, u32 param_size, void *ret, unsigned *ret_size)
{
  sender = sender;
  params = params;
  ret = ret;
  ret_size = ret_size;
  param_size = param_size;

  unsigned vaddr = get_unsigned_arg(&params);
  unsigned paddr = get_unsigned_arg(&params);
  int rights = get_int_arg(&params);
  unsigned size = get_unsigned_arg(&params);
  unsigned *pid_split = (unsigned *)&sender;

  rights = rights;

  // get page dir
  t_page_directory *pd = get_as_from_pid(pid_split[0]);

  // alloc page
  unsigned i;
  for (i = 0; i < size; i += 0x1000)
  {
    t_page *page = get_page(vaddr + i, 0, pd);

    if (!page || test_frame(paddr + i))
    {
      *((unsigned *)ret) = 0;
      *ret_size = sizeof(unsigned);
      return ;
    }

    alloc_page_at(paddr + i, get_page(vaddr + i, 1, pd), 0, 1);

  }

  // return pointer
  *(unsigned *)ret = vaddr;
  *ret_size = sizeof(unsigned);
}