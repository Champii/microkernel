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
  mov esp, _sys_stack
  jmp stublet

ALIGN 4
mboot:
  MULTIBOOT_PAGE_ALIGN    equ 1<<0
  MULTIBOOT_MEMORY_INFO   equ 1<<1
  MULTIBOOT_HEADER_MAGIC  equ 0x1BADB002
  MULTIBOOT_HEADER_FLAGS  equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
  MULTIBOOT_CHECKSUM      equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

  dd MULTIBOOT_HEADER_MAGIC
  dd MULTIBOOT_HEADER_FLAGS
  dd MULTIBOOT_CHECKSUM

stublet:
  EXTERN main
  CALL main
  jmp $

SECTION .bss
  resb 8192

_sys_stack:
