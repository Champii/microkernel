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
#include                  "elf.h"
#include                  "mm.h"

extern t_page_directory   *page_dir;
extern t_page_directory   *cur_dir;

u64                       *pl_pid = 0;



int                       check_elf_magic(unsigned char *to_check)
{
  int                     i = 0;
  char                    magic[4] = ELFMAG;

  while (i < 4)
  {
    if (to_check[i] != magic[i])
    {
      printk(COLOR_RED, "MAGIC DON'T MATCH ELF, SKIPPING MODULE\n");
      return (-1);
    }
    i++;
  }
  // printk(COLOR_GREEN, "Good Magic ! Loading...\n");
  return (0);
}

void                      init_services(int count, struct s_multiboot_module *module)
{
  int                     i;

  printk(COLOR_WHITE, "Modules found = ");
  printk(COLOR_WHITE, my_putnbr_base(count, "01234564789"));
  printk(COLOR_WHITE, "\n");

  for (i = 0; i < count; i++)
  {
    Elf32_Ehdr *elf;
    Elf32_Phdr *ph;


    // unsigned *reloc_process_mem = kmalloc_a(module[i].mod_end - module[i].mod_start);
    // memcpy(reloc_process_mem, module[i].mod_start, module[i].mod_end - module[i].mod_start);

    elf = (Elf32_Ehdr *)module[i].mod_start;
    // elf = (Elf32_Ehdr *)reloc_process_mem;

    if (check_elf_magic(elf->e_ident) < 0)
      continue ;

    unsigned *task_split;
    u64 *task = kmalloc(sizeof(u64));

    task_split = (unsigned *)task;

    create_process(task);

    // if process loader
    if (i == 0)
    {
      printk(COLOR_WHITE, "PL !\n");
      pl_pid = task;
    }

    //empty address space
    t_page_directory *new_pd = clone_directory(page_dir);

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
        alloc_page(get_page(k, 1, new_pd), 1, 1);

      switch_page_directory(new_pd);

      memcpy((void *)ph->p_vaddr, (void *)(module[i].mod_start + ph->p_offset), ph->p_filesz);
      if (ph->p_filesz != ph->p_memsz)
      {
        memset((void *)(ph->p_vaddr + ph->p_filesz), 0, ph->p_memsz - ph->p_filesz);
        new_stack = ph->p_vaddr + ph->p_memsz;
        printk(COLOR_WHITE, "Created stack : 0x");
        printk(COLOR_WHITE, my_putnbr_base(new_stack, "0123456789ABCDEF"));
        printk(COLOR_WHITE, "\n");

      }
      switch_page_directory(cur_dir);

      ph = (Elf32_Phdr *)(((char *)ph) + elf->e_phentsize);
    }



    // new stack

    //load

    //create

    run_process(task_split[1], (void *)elf->e_entry, new_stack, new_pd);


  }
}
