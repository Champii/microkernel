/************************************************
*
* Micro Kernel
*
* - Standard memory functs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef               __MM_H__
# define              __MM_H__

typedef struct        s_page
{
  unsigned            present    : 1;
  unsigned            rw         : 1;
  unsigned            user       : 1;
  unsigned            accessed   : 1;
  unsigned            dirty      : 1;
  unsigned            unused     : 7;
  unsigned            frame      : 20;
}                     t_page;

typedef struct        s_page_table
{
  t_page              pages[1024];
}                     t_page_table;

typedef struct        s_page_directory
{
  t_page_table        *tables[1024];
  unsigned            tablesPhysical[1024];
  unsigned            physicalAddr;
}                     t_page_directory;

typedef struct        s_registers
{
  unsigned            ds;                  // Data segment selector
  unsigned            edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
  unsigned            int_no, err_code;    // Interrupt number and error code (if applicable)
  unsigned            eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
}                     t_registers;

void                  init_mm();
t_page                *get_page(unsigned, int, t_page_directory *);
void                  set_page_dir(t_page_directory *);
void                  *kmalloc(unsigned);

/*LOOK stos (asm) => page table*/

#endif                /*__MM_H__*/
