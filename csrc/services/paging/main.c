#include <rpc/rpc.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define COLOR_WHITE 15

void print_error(int ret)
{
  write(COLOR_WHITE, "PAGING = Error : ", 0);
  switch (ret)
  {
    case -1:
      write(COLOR_WHITE, "PAGING = No more memory\n", 0);
      break;
    case -2:
      write(COLOR_WHITE, "PAGING = Operation not permitted\n", 0);
      break;
    case -3:
      write(COLOR_WHITE, "PAGING = Invalid argument\n", 0);
      break;
    case -4:
      write(COLOR_WHITE, "PAGING = No such process\n", 0);
      break;
    case -5:
      write(COLOR_WHITE, "PAGING = Bad address\n", 0);
      break;
    case -6:
      write(COLOR_WHITE, "PAGING = Bad message\n", 0);
      break;
    case -7:
      write(COLOR_WHITE, "PAGING = Message too long\n", 0);
      break;

  }
}

int main()
{
  int ret;
  u64 pid;



  if ((ret = paging_service_pid(&pid)) < 0)
    print_error(ret);


  write(COLOR_WHITE, "PAGING = Paging Service LOADED !\n", 0);

  for (;;);

  return (0);
}
