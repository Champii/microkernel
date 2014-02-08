#include "sys/types.h"
#include "unistd.h"

int main()
{
  write(1, "lol", 4);
  return (0);
}