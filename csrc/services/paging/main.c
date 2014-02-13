#include <rpc/rpc.h>
#include <unistd.h>

int main()
{
  write(1, "lol", 4);
  for (;;);
  return (0);
}
