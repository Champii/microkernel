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

; Global definitions
global start
extern main

; Multiboot define
MULTIBOOT_PAGE_ALIGN    equ 1<<0
MULTIBOOT_HEADER_MAGIC  equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS  equ MULTIBOOT_PAGE_ALIGN
MULTIBOOT_CHECKSUM      equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

; Higher Half define
KERNEL_VIRTUAL_BASE equ 0xC0000000                  ; 3GB
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)  ; Page directory index of kernel's 4MB PTE.

STACKSIZE equ 0x4000


;
; SECTION .multiboot
;
; Multiboot stuct and flags
;
section .multiboot

multibootheader:

  dd MULTIBOOT_HEADER_MAGIC
  dd MULTIBOOT_HEADER_FLAGS
  dd MULTIBOOT_CHECKSUM


;
; SECTION .data
;
; Temporary Page Directory definition
;
section .data

align 0x1000

;pagedirectory:

;  dd 0x00000083
;  times (KERNEL_PAGE_NUMBER - 1) dd 0

;  dd 0x00000083
;  times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0


;
; SECTION .test
;
; Contains Higher Half jump code
;
section .text

align 0x1000

higherhalf:

  ; Remove old pagedirectory
;  mov dword [pagedirectory], 0
 ; invlpg [0]

 hlt

  ; Setting stack
  mov esp, stack

  ; Passing Multiboot info
  push eax                 ; magic number
  push ebx                 ; info structure

  call  main
  hlt

;
; SECTION .setup
;
; Init temporary GDT
; Init temporary PageDirectory
; Jump to Higher Half
;
section .setup
align 0x1000

pagedirectory:

  dd 0x00000083
  times (KERNEL_PAGE_NUMBER - 1) dd 0

  dd 0x00000083
  times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0

start:

  ; Init GDT
  lgdt [trickgdt]
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  ; Init PageDirectory
  mov ecx, pagedirectory
;  mov ecx, (pagedirectory - KERNEL_VIRTUAL_BASE)
  mov cr3, ecx

  mov ecx, cr4
  or ecx, 0x00000010
  mov cr4, ecx

  mov ecx, cr0
  or ecx, 0x80000000
  mov cr0, ecx
;  hlt

  ; Actualy jump
;  jmp [higherhalf]
  lea ecx, [higherhalf]
  jmp ecx

trickgdt:
  dw gdt_end - gdt - 1  ; size of the GDT
  dd gdt                ; address of GDT

gdt:
  dd 0, 0                                             ; null gate
  db 0xFF, 0xFF, 0, 0, 0, 10011010b, 11001111b, 0x00  ; code selector 0x08: base 0x40000000, limit 0xFFFFFFFF, type 0x9A, granularity 0xCF
  db 0xFF, 0xFF, 0, 0, 0, 10010010b, 11001111b, 0x00  ; data selector 0x10: base 0x40000000, limit 0xFFFFFFFF, type 0x92, granularity 0xCF

gdt_end:

;
; SECTION .bss
;
; Reserve space for stack
;
section .bss
  resb STACKSIZE

stack:
