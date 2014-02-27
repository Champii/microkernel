/************************************************
*
* Micro Kernel
*
* - Services funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  "services.h"
#include                  "multiboot.h"
#include                  "system.h"
#include                  "screen.h"
#include                  "kmalloc.h"
#include                  "process.h"
#include                  "task.h"
#include                  "elf.h"
#include                  "mm.h"

extern t_page_directory   *page_dir;
extern t_page_directory   *cur_dir;

u64                       *pl_pid = 0;
u64                       *paging_pid = 0;
u64                       *io_pid = 0;

struct s_multiboot_module *program_names;
struct s_multiboot_module *programs[32];

struct s_service_task
{
  void *task;
  void *entry;
  void *stack;
  void *pd;
};

struct s_service_task     services[3];


int                       check_elf_magic(unsigned char *to_check)
{
  int                     i = 0;
  char                    magic[4] = ELFMAG;

  while (i < 4)
  {
    if (to_check[i] != magic[i])
    {
      if (io_pid == 0)
      {
        printk(COLOR_RED, "SERVICE ELF MAGIC DON'T MATCH, GENERAL ERROR !\n");
        for (;;);
      }

      return (-1);
    }
    i++;
  }

  return (0);
}

#define PAGING_HEAP_ADDR 0x30000000
#define PAGING_AS_ADDR 0x20000000

#define PAGING_AS_START_OFFSET 128 // 0x20000000

  // paging_dir->tables[PAGING_AS_START_OFFSET] = (t_page_table *)pl_dir;
  // paging_dir->tables[PAGING_AS_START_OFFSET + 1] = (t_page_table *)paging_dir;
  // paging_dir->tables[PAGING_AS_START_OFFSET + 2] = (t_page_table *)io_dir;

void                      map_services_as_in_paging()
{
  t_page_directory        *pl_dir = services[0].pd;
  t_page_directory        *paging_dir = services[1].pd;
  t_page_directory        *io_dir = services[2].pd;

  // t_page *pl_dir_virt = get_page((unsigned)pl_dir, 0, page_dir);
  // t_page *paging_dir_virt = get_page((unsigned)paging_dir, 0, page_dir);
  // t_page *io_dir_virt = get_page((unsigned)io_dir, 0, page_dir);

  unsigned i;
  unsigned j = 0;
  unsigned start_map = PAGING_AS_ADDR;

  /***************/

  alloc_page(get_page(start_map, 1, paging_dir), 0, 1);

  alloc_page_at(pl_dir->physicalAddr, get_page(start_map + 0x1000, 1, paging_dir), 0, 1);
  alloc_page_at(pl_dir->physicalAddr + 0x1000, get_page(start_map + 0x2000, 1, paging_dir), 0, 1);

  start_map += 0x3000;

  for (i = start_map; i < start_map + (1024 * 0x1000); i += 0x1000)
    alloc_page_at(pl_dir->tablesPhysical[j++], get_page(i, 1, paging_dir), 0, 1);

  start_map += 1024 * 0x1000;

  /***************/
  alloc_page_at(0x4242, get_page(0x42424242, 1, paging_dir), 0, 1);

  alloc_page(get_page(start_map, 1, paging_dir), 0, 1);

  alloc_page_at(paging_dir->physicalAddr, get_page(start_map + 0x1000, 1, paging_dir), 0, 1);
  alloc_page_at(paging_dir->physicalAddr + 0x1000, get_page(start_map + 0x2000, 1, paging_dir), 0, 1);

  start_map += 0x3000;

  j = 0;
  for (i = start_map; i < start_map + (1024 * 0x1000); i += 0x1000)
    alloc_page_at(paging_dir->tablesPhysical[j++], get_page(i, 1, paging_dir), 0, 1);


  start_map += 1024 * 0x1000;

  /***************/

  alloc_page(get_page(start_map, 1, paging_dir), 0, 1);

  alloc_page_at(io_dir->physicalAddr, get_page(start_map + 0x1000, 1, paging_dir), 0, 1);
  alloc_page_at(io_dir->physicalAddr + 0x1000, get_page(start_map + 0x2000, 1, paging_dir), 0, 1);

  start_map += 0x3000;

  j = 0;
  for (i = start_map; i < start_map + (1024 * 0x1000); i += 0x1000)
    alloc_page_at(io_dir->tablesPhysical[j++], get_page(i, 1, paging_dir), 0, 1);

  /***************/


  // alloc_page_at(pl_dir_virt->frame * 0x1000, get_page(PAGING_AS_ADDR, 1, paging_dir), 0, 1);
  // alloc_page_at((pl_dir_virt->frame + 1) * 0x1000, get_page(PAGING_AS_ADDR + 0x1000, 1, paging_dir), 0, 1);

  // alloc_page_at(paging_dir_virt->frame * 0x1000, get_page(PAGING_AS_ADDR + pd_size, 1, paging_dir), 0, 1);
  // alloc_page_at((paging_dir_virt->frame + 1) * 0x1000, get_page(PAGING_AS_ADDR + pd_size + 0x1000, 1, paging_dir), 0, 1);

  // alloc_page_at(io_dir_virt->frame * 0x1000, get_page(PAGING_AS_ADDR + (2 * pd_size), 1, paging_dir), 0, 1);
  // alloc_page_at((io_dir_virt->frame + 1) * 0x1000, get_page(PAGING_AS_ADDR + (2 * pd_size) + 0x1000, 1, paging_dir), 0, 1);


  // alloc_page_at(pl_dir->physicalAddr, get_page(PAGING_AS_ADDR, 1, paging_dir), 0, 1);

  // alloc_page_at(paging_dir->physicalAddr, get_page(PAGING_AS_ADDR + 0x1000, 1, paging_dir), 0, 1);

  // alloc_page_at(io_dir->physicalAddr, get_page(PAGING_AS_ADDR + (2 * 0x1000), 1, paging_dir), 0, 1);


  // int i;
  // for (i = PAGING_AS_ADDR + (3 * 0x1000); i < PAGING_AS_ADDR + (32 * 0x1000); i += 0x1000)
  //   alloc_page(get_page(i, 1, paging_dir), 0, 1);

  // alloc page for temp heap
  for (i = PAGING_HEAP_ADDR; i < PAGING_HEAP_ADDR + (1024 * 0x1000); i++)
    alloc_page(get_page(i, 1, paging_dir), 0, 1);

}

// #define PL_MAP_ADDR 0x30000000

void                      map_regular_in_pl(unsigned count)
{
  t_page_directory        *pl_dir = services[0].pd;
  unsigned i;
  unsigned j;

  for (i = program_names->mod_start; i < program_names->mod_end; i += 0x1000)
  {
    t_page *pl_dir_pnames = get_page(i, 0, page_dir);
    alloc_page_at(pl_dir_pnames->frame * 0x1000, get_page(i, 1, pl_dir), 0, 1);
  }

  for (j = 0; j < count - 4; j++)
  {
    printk(COLOR_WHITE, "Mapped regular prog = ");
    printk(COLOR_WHITE, my_putnbr_base(programs[j]->mod_start, "0123456789ABCDEF"));
    printk(COLOR_WHITE, "\n");

    if (check_elf_magic((unsigned char *)programs[j]->mod_start) < 0)
      printk(COLOR_RED, "ERROR REGULAR PROGRAM ELF\n");

    for (i = programs[j]->mod_start; i < programs[j]->mod_end; i += 0x1000)
    {
      t_page *pl_dir_p = get_page(i, 0, page_dir);
      alloc_page_at(pl_dir_p->frame * 0x1000, get_page(i, 1, pl_dir), 0, 1);
    }
  }

  //TEST
  alloc_page_at(0x4242, get_page(0x42424242, 1, pl_dir), 0, 1);

}

static void               push_pid_on_stack(unsigned **stack, u64 *pid)
{
  unsigned *pid_split = (unsigned *)pid;
  *stack -= 1;
  **stack = pid_split[1];
  *stack -= 1;
  **stack = pid_split[0];
}

static void               push_addr_on_stack(unsigned **stack, unsigned addr)
{
  *stack -= 1;
  **stack = addr;
}
// static void               push_pid_on_stack(unsigned **stack, u64 *pid)
// {
//   unsigned *pid_split = (unsigned *)pid;
//   **stack = pid_split[1];
//   *stack -= 1;
//   **stack = pid_split[0];
//   *stack -= 1;
// }

// static void               push_addr_on_stack(unsigned **stack, unsigned addr)
// {
//   **stack = addr;
//   *stack -= 1;
// }

static void               prepare_stack(t_task *task, t_page_directory *root_pd, void **stack, unsigned count)
{
  unsigned                *ustack = *stack;

  // Push pl_pid on stack
  switch_page_directory(root_pd);

  // if Program Loader, push all services pid on stack (reverse order)
  if (task->id == 1)
  {
    unsigned i;

    for (i = 0; i < count - 4; i++)
      push_addr_on_stack(&ustack, programs[i]->mod_start);

    push_addr_on_stack(&ustack, program_names->mod_end);
    push_addr_on_stack(&ustack, program_names->mod_start);
    push_pid_on_stack(&ustack, io_pid);
    push_pid_on_stack(&ustack, paging_pid);
  }
  else if (task->id == 2)
  {
    printk(COLOR_WHITE, "First frame = ");
    printk(COLOR_WHITE, my_putnbr_base(first_frame(), "0123456789"));
    printk(COLOR_WHITE, "\n");
    push_addr_on_stack(&ustack, first_frame());
    push_addr_on_stack(&ustack, PAGING_AS_ADDR);

  }

  push_pid_on_stack(&ustack, pl_pid);
  push_addr_on_stack(&ustack, (unsigned)ustack);
  // printk(COLOR_WHITE, "Address pushed on stack : 0x");
  // printk(COLOR_WHITE, my_putnbr_base((unsigned)*ustack, "0123456789ABCDEF"));
  // printk(COLOR_WHITE, "\n");

  ustack -= 1;
  switch_page_directory(cur_dir);

  *stack = ustack;
}

void                      prepare_processes(unsigned count)
{
  int                     i;

  map_services_as_in_paging();
  map_regular_in_pl(count);

  for (i = 0; i < 3; i++)
  {
    prepare_stack(services[i].task, services[i].pd, &services[i].stack, count);
    run_process(services[i].task, services[i].entry, services[i].stack, services[i].pd);
    // printk(COLOR_WHITE, "Process stack : 0x");
    // printk(COLOR_WHITE, my_putnbr_base(services[i].stack, "0123456789ABCDEF"));
    // printk(COLOR_WHITE, "\n");
  }
}

void                      init_services(unsigned count, struct s_multiboot_module *module)
{
  int                     i;

  printk(COLOR_WHITE, "Modules found = ");
  printk(COLOR_WHITE, my_putnbr_base(count, "0123456789"));
  printk(COLOR_WHITE, "\n");

  for (i = 0; i < count; i++)
  {
    Elf32_Ehdr *elf;
    Elf32_Phdr *ph;

    elf = (Elf32_Ehdr *)module[i].mod_start;

    int ret;
    if ((ret = check_elf_magic(elf->e_ident)) < 0)
    {
      int j;

      for (j = i; j < count; j++)
      {
        if (j == 3)
          program_names = &module[j];
        else if (j >= 4)
          programs[j - 4] = &module[j];
      }

      prepare_processes(count);

      return ;
    }

    unsigned *task_split;
    u64 *task = kmalloc(sizeof(u64));

    task_split = (unsigned *)task;

    create_process(task);

    // save pid to push on stack of services
    if (i == 0)
      pl_pid = task;
    else if (i == 1)
      paging_pid = task;
    else if (i == 2)
      io_pid = task;

    //empty address space
    t_page_directory *new_pd = create_empty_directory();

    ph = ((Elf32_Phdr *)(module[i].mod_start + elf->e_phoff));

    unsigned *new_stack = 0;

    int j;
    for (j = 0; j < elf->e_phnum; j++)
    {
      if (ph->p_type != PT_LOAD)
      {
        ph = (Elf32_Phdr *)(((char *)ph) + elf->e_phentsize);
        continue ;
      }

      int k = (ph->p_vaddr / 0x1000) * 0x1000;
      for (; k <= ph->p_vaddr + ph->p_memsz; k += 0x1000)
        alloc_page(get_page(k, 1, new_pd), 0, 1);

      switch_page_directory(new_pd);

      memcpy((void *)ph->p_vaddr, (void *)(module[i].mod_start + ph->p_offset), ph->p_filesz);
      if (ph->p_filesz != ph->p_memsz)
      {
        memset((void *)(ph->p_vaddr + ph->p_filesz), 0, ph->p_memsz - ph->p_filesz);
        alloc_page(get_page(((ph->p_vaddr + ph->p_memsz) / 0x1000 * 0x1000) + 0x1000, 1, new_pd), 0, 1);
        new_stack = (unsigned *)(((ph->p_vaddr + ph->p_memsz) / 0x1000 * 0x1000) + 0x1000);
        printk(COLOR_WHITE, "Stack start = 0x");
        printk(COLOR_WHITE, my_putnbr_base((unsigned)new_stack, "0123456789ABCDEF"));
        printk(COLOR_WHITE, "\n");
      }
      switch_page_directory(cur_dir);

      ph = (Elf32_Phdr *)(((char *)ph) + elf->e_phentsize);
    }

    services[i].task = (void *)task_split[1];
    services[i].entry = (void *)elf->e_entry;
    services[i].stack = new_stack;
    services[i].pd = new_pd;

  }

}
