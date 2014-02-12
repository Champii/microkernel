/************************************************
*
* Micro Kernel
*
* - Multiboot functs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef                   __MULTIBOOT_H__
# define                  __MULTIBOOT_H__


# define                  MULTIBOOT_TAG_TYPE_END 0
# define                  MULTIBOOT_TAG_TYPE_MODULE 3

struct                    s_multiboot_aout_symbol_table
{
  unsigned                tabsize;
  unsigned                strsize;
  unsigned                addr;
  unsigned                reserved;
};

struct                    s_multiboot_elf_section_header_table
{
  unsigned                num;
  unsigned                size;
  unsigned                addr;
  unsigned                shndx;
};

struct                    s_multiboot_info
{
  unsigned                flags;

  unsigned                mem_lower;
  unsigned                mem_upper;

  unsigned                boot_device;

  unsigned                cmdline;

  unsigned                mods_count;
  unsigned                mods_addr;

  union
  {
    struct s_multiboot_aout_symbol_table        aout_sym;
    struct s_multiboot_elf_section_header_table elf_sec;
  } u;

  unsigned                mmap_length;
  unsigned                mmap_addr;

  unsigned                drives_length;
  unsigned                drives_addr;

  unsigned                config_table;

  unsigned                boot_loader_name;

  unsigned                apm_table;

  unsigned                vbe_control_info;
  unsigned                vbe_mode_info;
  unsigned short          vbe_mode;
  unsigned short          vbe_interface_seg;
  unsigned short          vbe_interface_off;
  unsigned short          vbe_interface_len;
}                         __attribute__((packed));

struct                    s_multiboot_tag
{
  unsigned                type;
  unsigned                size;
};

struct                    s_multiboot_module
{
  unsigned                mod_start;
  unsigned                mod_end;
  char                    *cmdline;
  unsigned                reserved;
};

#endif                    /*__MULTIBOOT_H__*/
