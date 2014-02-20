/************************************************
*
* Micro Kernel
*
* - Main C entry point for SHELL
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <rpc/rpc.h>
#include                  <unistd.h>

int                       main()
{
  kwrite(15, "SHELL !!!!\n", 0);

  return 0;
}
