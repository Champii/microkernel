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


void                      init_services(int count, struct s_multiboot_module *module)
{
  // int                     i;

  // for (i = 0; i < count; i++)
  // {

  // }


  printk(COLOR_WHITE, "Modules found = ");
  printk(COLOR_WHITE, my_putnbr_base(count, "01234564789"));
  printk(COLOR_WHITE, "\n");



  printk(COLOR_WHITE, "Test = ");
  printk(COLOR_WHITE, my_putnbr_base(module->mod_start, "01234564789ABCDEF"));
  printk(COLOR_WHITE, "\n");
  printk(COLOR_WHITE, my_putnbr_base(module->mod_end, "01234564789ABCDEF"));
  printk(COLOR_WHITE, "\n");
  printk(COLOR_WHITE, (char *)module->mod_start);

}
