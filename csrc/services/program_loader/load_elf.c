/************************************************
*
* Micro Kernel
*
* - Program Loader Load Elf
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  <rpc/rpc.h>
#include                  <rpc/paging.h>
#include                  <elf.h>
#include                  <errno.h>
#include                  <string.h>
#include                  <unistd.h>
#include                  <sys/syscall.h>

int itoa_base(int n, char *str, unsigned base);
int uitoa_base(unsigned n, char *str, unsigned base);

static int                check_elf_magic(unsigned char *to_check)
{
  int                     i = 0;
  char                    magic[4] = ELFMAG;

  while (i < 4)
  {
    if (to_check[i] != magic[i])
    {
      kwrite(4, "MAGIC DON'T MATCH ELF, SKIPPING MODULE\n", 0);
      return (-1);
    }
    i++;
  }
  return (0);
}


int                       load_elf(u64 pid, void *elf_start, unsigned *entry, unsigned *stack)
{
  Elf32_Ehdr *elf;
  Elf32_Phdr *ph;
  // char tmp[10];

  elf = (Elf32_Ehdr *)elf_start;

  if (check_elf_magic(elf->e_ident) < 0)
    return -EINVAL;

  *entry = elf->e_entry;

  // itoa_base(*entry, tmp, 16);
  // kwrite(4, "Entry point = 0x", 0);
  // kwrite(4, tmp, 0);
  // kwrite(4, "\n", 0);
  ph = ((Elf32_Phdr *)(elf_start + elf->e_phoff));

  int j;
  void *void_ret;
  for (j = 0; j < elf->e_phnum; j++)
  {
    if (ph->p_type != PT_LOAD)
    {
      ph = (Elf32_Phdr *)(((char *)ph) + elf->e_phentsize);
      continue ;
    }

    int k = (ph->p_vaddr / 0x1000) * 0x1000;
    // kwrite(4, "GOOD MAGIC\n", 0);
    if ((int)(void_ret = rpc_mmap_sys(pid, (void *)k, 0, ph->p_memsz)) < 0)
    {
      // itoa_base((int)void_ret, tmp, 10);
      // kwrite(4, "MMAP RET = ", 0);
      // kwrite(4, tmp, 0);
      // kwrite(4, "\n", 0);
      // kwrite(4, "ERROR mmap ", 0);
      print_error((int)void_ret);
      return (int)void_ret;
    }

    kwrite(15, "LOAD END\n", 0);
    sys_invlpg((void *)((unsigned)void_ret / 0x1000));

    memcpy(void_ret + (ph->p_vaddr % 0x1000), (void *)(elf_start + ph->p_offset), ph->p_filesz);
    if (ph->p_filesz != ph->p_memsz)
    {
      memset((void *)(void_ret + ((ph->p_vaddr + ph->p_filesz) %0x1000)), 0, ph->p_memsz - ph->p_filesz);
      *stack = (unsigned)(void_ret + ((ph->p_vaddr + ph->p_memsz) %0x1000));
    }

    ph = (Elf32_Phdr *)(((char *)ph) + elf->e_phentsize);
  }

  return 0;
}
