#include <rpc/rpc.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <screen.h>

#include <rpc.h>

void welcome_screen()
{
  clear_screen();

  printk(COLOR_BLUE, "*******************************************************************************\n");
  printk(COLOR_BLUE, "*                                                                             *\n");
  printk(COLOR_BLUE, "*                                    PhenX                                    *\n");
  printk(COLOR_BLUE, "*                                                                             *\n");
  printk(COLOR_BLUE, "*                             Welcome to UserLand                             *\n");
  printk(COLOR_BLUE, "*                                                                             *\n");
  printk(COLOR_BLUE, "*******************************************************************************\n");
  printk(COLOR_WHITE, "\n");
}

int main()
{

  welcome_screen();

  printk(COLOR_WHITE, "Starting IO service\n");

  register_listen_rpcs();

  for (;;);
  return (0);
}
