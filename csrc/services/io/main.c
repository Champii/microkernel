/************************************************
*
* Micro Kernel
*
* - Paging Main
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/


#include                  <rpc/rpc.h>
#include                  <string.h>
#include                  <unistd.h>
#include                  <errno.h>
#include                  <screen.h>

#include                  <rpc.h>

void                      welcome_screen()
{
  clear_screen();

  printk(COLOR_BLUE, "********************************************************************************");
  printk(COLOR_BLUE, "*                                                                              *");
  printk(COLOR_BLUE, "*                                    PhenX                                     *");
  printk(COLOR_BLUE, "*                                                                              *");
  printk(COLOR_BLUE, "*                             Welcome to UserLand                              *");
  printk(COLOR_BLUE, "*                                                                              *");
  printk(COLOR_BLUE, "********************************************************************************");
  printk(COLOR_WHITE, "\n");
}

int                       main()
{

  // welcome_screen();

  printk(COLOR_WHITE, "Starting IO service\n");

  register_listen_rpcs();

  for (;;);
  return (0);
}
