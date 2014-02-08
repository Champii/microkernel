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
#include                  "screen.h"
#include                  "kmalloc.h"
#include                  "process.h"
#include                  "elf.h"
#include                  "mm.h"

extern t_page_directory   *page_dir;

void                      init_services(int count, struct s_multiboot_module *module)
{
  // int                     i;

  // for (i = 0; i < count; i++)
  // {

  // }


  printk(COLOR_WHITE, "Modules found = ");
  printk(COLOR_WHITE, my_putnbr_base(count, "01234564789"));
  printk(COLOR_WHITE, "\n");

  if (count)
  {
    Elf32_Ehdr *elf;
    Elf32_Phdr *ph;

    printk(COLOR_WHITE, "Size = ");
    printk(COLOR_WHITE, my_putnbr_base(module->mod_end - module->mod_start, "0123456789"));
    printk(COLOR_WHITE, "\n");
    elf = (Elf32_Ehdr *)module->mod_start;
    ph = elf->e_phoff;

    int i = 0;
    char magic[4] = ELFMAG;

    while (i < 4)
    {
      if (elf->e_ident[i] != magic[i])
      {
        printk(COLOR_RED, "MAGIC DON'T MATCH ELF\n");
        return ;
      }
      i++;
    }
    printk(COLOR_GREEN, "Good Magic ! Loading...\n");

    unsigned *test;
    u64 *task = kmalloc(sizeof(u64));

    //empty address space
    t_page_directory *new_pd = clone_directory(page_dir);

    printk(COLOR_GREEN, "Size = 0x");
    printk(COLOR_GREEN, my_putnbr_base(ph->p_memsz, "0123456789ABCDEF"));
    printk(COLOR_GREEN, "\n");

    for (i = ph->p_vaddr; i < ph->p_memsz; i = i + 0x1000)
    {
      printk(COLOR_GREEN, "i = 0x");
      printk(COLOR_GREEN, my_putnbr_base(i, "0123456789ABCDEF"));
      printk(COLOR_GREEN, "\n");

      alloc_page_at(module->mod_start, get_page(i, 1, new_pd), 0, 1);
    }

    // memcpy();

    // new stack
    unsigned *new_stack = kmalloc(sizeof(0x4000));

    //load

    //create
    test = (unsigned *)task;
    __asm__ ("       \
      mov %0, %%ebx; \
      mov $0, %%eax;  \
      int $0x80;"
      : : "r"(task));

    printk(COLOR_WHITE, "Created process : ");
    printk(COLOR_WHITE, my_putnbr_base(test[0], "0123456789"));
    printk(COLOR_WHITE, "\n");

    //run
    __asm__ ("       \
      mov %0, %%esi; \
      mov %1, %%edx; \
      mov %2, %%ecx; \
      mov %3, %%ebx; \
      mov $1, %%eax; \
      int $0x80;"
      : : "r"(new_pd), "r"(new_stack), "r"(elf->e_entry), "r"(task));
    printk(COLOR_WHITE, "Loading virtual address : 0x");
    printk(COLOR_WHITE, my_putnbr_base(ph->p_vaddr, "0123456789ABCDEF"));
    printk(COLOR_WHITE, "\n");

    // printk(COLOR_WHITE, (char *)module->mod_start);
  }




  // TEST
  // unsigned *test = kmalloc(sizeof(int) * 2);

  // test[0] = 1;
  // test[1] = 2;


}
