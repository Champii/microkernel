;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Micro Kernel
;
; - Asm bootstrap
;
; Florian Greiner <florian.greiner@epitech.eu>
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 32]
global start
start:
  mov esp, stack
  jmp kernel

ALIGN 4
mboot:
  MULTIBOOT_PAGE_ALIGN    equ 1<<0
  MULTIBOOT_HEADER_MAGIC  equ 0x1BADB002
  MULTIBOOT_HEADER_FLAGS  equ MULTIBOOT_PAGE_ALIGN
  MULTIBOOT_CHECKSUM      equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

  dd MULTIBOOT_HEADER_MAGIC
  dd MULTIBOOT_HEADER_FLAGS
  dd MULTIBOOT_CHECKSUM

kernel:
  extern main
  call main
  jmp $

global flush_gdt
extern gdt_desc
flush_gdt:
    lgdt [gdt_desc]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush_ret
flush_ret:
    ret

SECTION .bss
  resb 8192

stack:
