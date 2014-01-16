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
    jmp 0x08:flush2
flush2:
    ret

	global idt_load
	extern idtp
idt_load:
    lidt [idtp]
    ret
		
SECTION .bss
  resb 8192

stack:
