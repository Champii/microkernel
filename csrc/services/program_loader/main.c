/************************************************
*
* Micro Kernel
*
* - Program Loader Main
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/


#include <rpc/rpc.h>
#include <unistd.h>
#include <sys/syscall.h>

int main()
{

  // sys_send(prog_loader_pid, "lol", 4);
  write(1, "lol", 4);
  for (;;)
  {
    int a;
    int b;

    a = 1;
    b = 2;
    a = b;
    b = a;
    // sys_send(prog_loader_pid, "lol", 4);
  }
  return (0);
}