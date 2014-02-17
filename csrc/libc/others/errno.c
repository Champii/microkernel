#include "sys/sys_types.h"
#include "unistd.h"
#include "errno.h"

#define COLOR_WHITE 15

void print_error(int ret)
{
  kwrite(COLOR_WHITE, "Error : ", 0);
  switch (ret)
  {
    case -1:
      kwrite(COLOR_WHITE, "No more memory\n", 0);
      break;
    case -2:
      kwrite(COLOR_WHITE, "Operation not permitted\n", 0);
      break;
    case -3:
      kwrite(COLOR_WHITE, "Invalid argument\n", 0);
      break;
    case -4:
      kwrite(COLOR_WHITE, "No such process\n", 0);
      break;
    case -5:
      kwrite(COLOR_WHITE, "Bad address\n", 0);
      break;
    case -6:
      kwrite(COLOR_WHITE, "Bad message\n", 0);
      break;
    case -7:
      kwrite(COLOR_WHITE, "Message too long\n", 0);
      break;

  }
}
