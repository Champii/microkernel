/************************************************
*
* Micro Kernel
*
* - Restrict RPC
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <sys/sys_types.h>
#include                  <unistd.h>

u64                       restricted[6];

#define COLOR_WHITE 15

int itoa_base(int n, char *str, unsigned base);

void                      init_restrict_rpc()
{
  int                     i;

  for (i = 0; i < 6; i++)
  {

    // kwrite(COLOR_WHITE, "TEST\n", 0);
    restricted[i] = (u64)0;
  }
    // kwrite(COLOR_WHITE, "\n", 0);
}

int                       check_restrict_rpc(unsigned func_id, u64 sender)
{
  // unsigned *pid_split = (unsigned *)&sender;
  // u64 restricted_pid = restricted[func_id];
  // char tmp[10];

  // itoa_base(pid_split[0], tmp, 10);
  // kwrite(COLOR_WHITE, "Check : ", 0);
  // kwrite(COLOR_WHITE, tmp, 0);
  // pid_split = ((unsigned *)&restricted_pid);
  // itoa_base(pid_split[0], tmp, 10);
  // kwrite(COLOR_WHITE, " restricted : ", 0);
  // kwrite(COLOR_WHITE, tmp, 0);
  // kwrite(COLOR_WHITE, "\n", 0);

  if (!restricted[func_id])
    return 0;

  if (restricted[func_id] != sender)
    return -1;

  return 0;
}

void                      restrict_rpc(unsigned func_id, u64 sender)
{
  restricted[func_id] = sender;
}
