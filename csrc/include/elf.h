/************************************************
*
* Micro Kernel
*
* - Elf header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef               __ELF_H__
# define              __ELF_H__

typedef unsigned short Elf32_Half;

typedef unsigned      Elf32_Word;
typedef int           Elf32_Sword;

typedef unsigned long long Elf32_Xword;
typedef long long     Elf32_Sxword;

typedef unsigned      Elf32_Addr;

typedef unsigned      Elf32_Off;

typedef unsigned short Elf32_Section;

typedef Elf32_Half    Elf32_Versym;

#define EI_MAG0   0   /* File identification byte 0 index */
#define ELFMAG0   0x7f    /* Magic number byte 0 */

#define EI_MAG1   1   /* File identification byte 1 index */
#define ELFMAG1   'E'   /* Magic number byte 1 */

#define EI_MAG2   2   /* File identification byte 2 index */
#define ELFMAG2   'L'   /* Magic number byte 2 */

#define EI_MAG3   3   /* File identification byte 3 index */
#define ELFMAG3   'F'   /* Magic number byte 3 */

/* Conglomeration of the identification bytes, for easy testing as a word.  */
#define ELFMAG    "\177ELF"

# define EI_NIDENT 16

typedef struct
{
  unsigned char e_ident[EI_NIDENT];     /* Magic number and other info */
  Elf32_Half    e_type;                 /* Object file type */
  Elf32_Half    e_machine;              /* Architecture */
  Elf32_Word    e_version;              /* Object file version */
  Elf32_Addr    e_entry;                /* Entry point virtual address */
  Elf32_Off     e_phoff;                /* Program header table file offset */
  Elf32_Off     e_shoff;                /* Section header table file offset */
  Elf32_Word    e_flags;                /* Processor-specific flags */
  Elf32_Half    e_ehsize;               /* ELF header size in bytes */
  Elf32_Half    e_phentsize;            /* Program header table entry size */
  Elf32_Half    e_phnum;                /* Program header table entry count */
  Elf32_Half    e_shentsize;            /* Section header table entry size */
  Elf32_Half    e_shnum;                /* Section header table entry count */
  Elf32_Half    e_shstrndx;             /* Section header string table index */
} Elf32_Ehdr;

/* Program segment header.  */

typedef struct
{
  Elf32_Word    p_type;                 /* Segment type */
  Elf32_Off     p_offset;               /* Segment file offset */
  Elf32_Addr    p_vaddr;                /* Segment virtual address */
  Elf32_Addr    p_paddr;                /* Segment physical address */
  Elf32_Word    p_filesz;               /* Segment size in file */
  Elf32_Word    p_memsz;                /* Segment size in memory */
  Elf32_Word    p_flags;                /* Segment flags */
  Elf32_Word    p_align;                /* Segment alignment */
} Elf32_Phdr;

#endif                    /*__ELF_H__*/
