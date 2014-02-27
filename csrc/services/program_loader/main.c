/************************************************
*
* Micro Kernel
*
* - Program Loader Main
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <rpc/rpc.h>
#include                  <unistd.h>
#include                  <string.h>
#include                  <errno.h>
#include                  <stdlib.h>
#include                  <rpc/paging.h>

#include                  <rpc.h>

extern unsigned           *stack_start;

extern u64                paging_pid;
extern u64                io_pid;

struct s_program          programs[32];

unsigned                  nb_progs = 0;

unsigned                  program_names_addr_start;
unsigned                  program_names_addr_end;

int                       uitoa_base(unsigned n, char *str, unsigned size);
int                       itoa_base(int n, char *str, unsigned size);


void                      get_stack_args()
{
  paging_pid = *(u64 *)stack_start;
  io_pid = *((u64 *)stack_start + 1);


  program_names_addr_start = *(stack_start + 4);
  program_names_addr_end = *(stack_start + 5);

  // char tmp[10];
  // unsigned *pid_split = (unsigned *)&paging_pid;

  // uitoa_base(pid_split[0], tmp, 16);
  // kwrite(15, "Paging pid = ", 0);
  // kwrite(15, tmp, 0);
  // pid_split = (unsigned *)&io_pid;
  // uitoa_base(pid_split[0], tmp, 16);
  // kwrite(15, "\nIo pid = ", 0);
  // kwrite(15, tmp, 0);
  // kwrite(15, "\n", 0);

  // io_pid = (u64 *)stack_start;
  // paging_pid = (u64 *)(stack_start + 2);
}

int                       prepare_programs()
{
  unsigned                i;
  int                     j = 0;

  for (i = program_names_addr_start; i < program_names_addr_end; i++)
  {
    if (*(char *)i != '\n')
      programs[nb_progs].name[j++] = *(char *)i;
    else
    {
      kwrite(15, "FOUND NAME = ", 0);
      kwrite(15, programs[nb_progs].name, 0);
      kwrite(15, "\n", 0);
      programs[nb_progs].name[j++] = 0;
      nb_progs++;
      j = 0;
    }
  }

  for (i = 0; i < nb_progs; i++)
  {
    char tmp[10];

    programs[i].elf_addr = (unsigned *)*(stack_start + i + 6);
    uitoa_base((unsigned)programs[i].elf_addr, tmp, 16);
    kwrite(15, "Program addr = 0x", 0);
    kwrite(15, tmp, 0);
    kwrite(15, "\n", 0);
  }

  return 0;
}

int                       launch_first()
{
  char                    tmp[64];
  unsigned                size;
  void                    *ret;
  unsigned                ret_size;

  size = strlen(programs[0].name);
  memcpy(tmp, (void *)&size, sizeof(unsigned));
  memcpy(tmp + sizeof(unsigned), programs[0].name, size);

  *(tmp + sizeof(unsigned) + size) = 0;

  exec_rpc(prog_loader_pid, tmp, 0, &ret, &ret_size);

  return 0;
}

int                       main()
{
  int                     ret;

  get_stack_args();

  if ((ret = prepare_programs()) < 0)
    print_error(ret);

  if ((ret = launch_first()) < 0)
    print_error(ret);

  register_listen_rpcs();

  for (;;);

  return (0);
}
