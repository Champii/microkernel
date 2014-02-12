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
  printk(COLOR_GREEN, "Good Magic ! Loading...\0123456789ABCDEFn");
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

    printk(COLOR_WHITE, "Size = ");
    printk(COLOR_WHITE, my_putnbr_base(module[i].mod_end - module[i].mod_start, "0123456789"));
    printk(COLOR_WHITE, "\n");

    // unsigned *reloc_process_mem = kmalloc_a(module[i].mod_end - module[i].mod_start);
    // memcpy(reloc_process_mem, module[i].mod_start, module[i].mod_end - module[i].mod_start);

    elf = (Elf32_Ehdr *)module[i].mod_start;
    // elf = (Elf32_Ehdr *)reloc_process_mem;

    if (check_elf_magic(elf->e_ident) < 0)
      continue ;

    //empty address space
    t_page_directory *new_pd = clone_directory(page_dir);

    ph = ((Elf32_Phdr *)(module[i].mod_start + elf->e_phoff));

    unsigned *new_stack;

    int j;
    for (j = 0; j < elf->e_phnum; j++)
    {
      printk(COLOR_GREEN, "Type : 0x");
      printk(COLOR_GREEN, my_putnbr_base(ph->p_type, "0123456789ABCDEF"));
      printk(COLOR_GREEN, "\n");
      if (ph->p_type != PT_LOAD)
      {
        ph = (Elf32_Phdr *)(((char *)ph) + elf->e_phentsize);
        continue ;
      }

      printk(COLOR_GREEN, "Segment offset/fsize/msize/v_address in mem = 0x");
      printk(COLOR_GREEN, my_putnbr_base(ph->p_offset, "0123456789ABCDEF"));
      printk(COLOR_GREEN, " 0x");
      printk(COLOR_GREEN, my_putnbr_base(ph->p_filesz, "0123456789ABCDEF"));
      printk(COLOR_GREEN, " 0x");
      printk(COLOR_GREEN, my_putnbr_base(ph->p_memsz, "0123456789ABCDEF"));
      printk(COLOR_GREEN, " 0x");
      printk(COLOR_GREEN, my_putnbr_base(ph->p_vaddr, "0123456789ABCDEF"));
      printk(COLOR_GREEN, "\n");


      int k = (ph->p_vaddr / 0x1000) * 0x1000;
      for (; k <= ph->p_vaddr + ph->p_memsz; k += 0x1000)
      {
        if (!alloc_page(get_page(k, 1, new_pd), 1, 1))
          printk(COLOR_GREEN, "Allocated page at : 0x");
        else
          printk(COLOR_RED, "ERROR Allocating page at : 0x");

        printk(COLOR_GREEN, my_putnbr_base(k, "0123456789ABCDEF"));
        printk(COLOR_GREEN, "\n");
        // alloc_page(get_page(k, 1, cur_dir), 1, 1);

      }

      switch_page_directory(new_pd);

      memcpy((void *)ph->p_vaddr, (void *)(module[i].mod_start + ph->p_offset), ph->p_filesz);
      if (ph->p_filesz != ph->p_memsz)
      {
        memset((void *)(ph->p_vaddr + ph->p_filesz), 0, ph->p_memsz - ph->p_filesz - 1);
        new_stack = ph->p_vaddr + ph->p_memsz;

      }
      switch_page_directory(cur_dir);

      ph = (Elf32_Phdr *)(((char *)ph) + elf->e_phentsize);
    }
      // for(;;);


    unsigned *test;
    u64 *task = kmalloc(sizeof(u64));

    // new stack

    //load

    //create
    test = (unsigned *)task;
    create_process(task);
    // __asm__ ("       \
    //   mov %0, %%ebx; \
    //   mov $0, %%eax;  \
    //   int $0x80;"
    //   : : "r"(task));

    printk(COLOR_WHITE, "Created process : ");
    printk(COLOR_WHITE, my_putnbr_base(test[0], "0123456789"));
    printk(COLOR_WHITE, "\n");

    // run
    // __asm__ ("       \
    //   mov %0, %%esi; \
    //   mov %1, %%edx; \
    //   mov %2, %%ecx; \
    //   mov %3, %%ebx; \
    //   mov $1, %%eax; \
    //   int $0x80;"
    //   : : "r"(new_pd), "r"(new_stack), "r"(elf->e_entry), "r"(test[1]));

    printk(COLOR_WHITE, "test 1 = 0x");
    printk(COLOR_WHITE, my_putnbr_base(test[0], "0123456789ABCDEF"));
    printk(COLOR_WHITE, "\n");
    printk(COLOR_WHITE, "test 2 = 0x");
    printk(COLOR_WHITE, my_putnbr_base(test[1], "0123456789ABCDEF"));
    printk(COLOR_WHITE, "\n");
    // run_process(test[1], (void *)elf->e_entry, new_stack, cur_dir);
    run_process(test[1], (void *)elf->e_entry, new_stack, new_pd);

    printk(COLOR_WHITE, "Entry point : 0x");
    printk(COLOR_WHITE, my_putnbr_base(elf->e_entry, "0123456789ABCDEF"));
    printk(COLOR_WHITE, "\n");

  }
}
