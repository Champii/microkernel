ENTRY(start)
OUTPUT_FORMAT(elf32-i386)

start_point = 0xC0000000;
phys = 0xC0200000;
SECTIONS
{
  . = 0x100000;

  .setup :
  {
    *(.multiboot)
    *(.setup)
    . = ALIGN(4096);
  }
  realend = .;

  . = phys;

  .text : AT(ADDR(.text) - start_point)
  {
    *(.text)
    *(.rodata)
    . = ALIGN(4096);
  }
  .data : AT(ADDR(.data) - start_point)
  {
    *(.data)
    . = ALIGN(4096);
  }
  .bss : AT(ADDR(.bss) - start_point)
  {
    *(.bss)
    . = ALIGN(4096);
    end = .;
  }
}
