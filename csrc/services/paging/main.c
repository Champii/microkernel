#include <rpc/rpc.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define COLOR_WHITE 15

unsigned itoa_base(int n, char *s, int base);

void print_error(int ret)
{
  kwrite(COLOR_WHITE, "PAGING = Error : ", 0);
  switch (ret)
  {
    case -1:
      kwrite(COLOR_WHITE, "PAGING = No more memory\n", 0);
      break;
    case -2:
      kwrite(COLOR_WHITE, "PAGING = Operation not permitted\n", 0);
      break;
    case -3:
      kwrite(COLOR_WHITE, "PAGING = Invalid argument\n", 0);
      break;
    case -4:
      kwrite(COLOR_WHITE, "PAGING = No such process\n", 0);
      break;
    case -5:
      kwrite(COLOR_WHITE, "PAGING = Bad address\n", 0);
      break;
    case -6:
      kwrite(COLOR_WHITE, "PAGING = Bad message\n", 0);
      break;
    case -7:
      kwrite(COLOR_WHITE, "PAGING = Message too long\n", 0);
      break;

  }
}

int main()
{
  int ret;
  u64 pid;
  // unsigned *pid_split = (unsigned *)&pid;
  // char tmp[10];



  // if ((ret = write(1, "Lol", 4)) < 0)
  //   print_error(ret);


  if ((ret = paging_service_pid(&pid)) < 0)
    print_error(ret);

  // itoa_base(pid_split[0], tmp, 10);
  // kwrite(COLOR_WHITE, "PAGING = Paging Service LOADED !\n", 0);
  // kwrite(COLOR_WHITE, "PAGING = Paging service PID = ", 0);
  // kwrite(COLOR_WHITE, tmp, 0);
  // kwrite(COLOR_WHITE, "\n", 0);

  for (;;);

  return (0);
}
