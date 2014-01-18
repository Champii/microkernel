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

	global idt_load
	extern idtp
idt_load:
    lidt [idtp]
    ret

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10	
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

;; Division By Zero
isr0:
	cli
	push byte 0
	push byte 0
	jmp isr_common_stub

;; Debug
isr1:
	cli
	push byte 0
	push byte 1
	jmp isr_common_stub

;; Non Maskable Interrupt
isr2:
	cli
	push byte 0
	push byte 2
	jmp isr_common_stub

;; Breakpoint
isr3:
	cli
	push byte 0
	push byte 3
	jmp isr_common_stub

;; Into Detected Overflow
isr4:
	cli
	push byte 0
	push byte 4
	jmp isr_common_stub

;; Out of bounds
isr5:
	cli
	push byte 0
	push byte 5
	jmp isr_common_stub

;; Invalid Opcode 
isr6:
	cli
	push byte 0
	push byte 6
	jmp isr_common_stub

;; No Coprocessor
isr7:
	cli
	push byte 0
	push byte 7
	jmp isr_common_stub

;; Double Fault
isr8:
	cli
	push byte 0
	push byte 8
	jmp isr_common_stub

;; Coprocessor Segment Overrun
isr9:
	cli
	push byte 0
	push byte 9
	jmp isr_common_stub

;; Bad Tss 
isr10:
	cli
	push byte 0
	push byte 10
	jmp isr_common_stub

;; Segment Not Present 
isr11:
	cli
	push byte 0
	push byte 11
	jmp isr_common_stub

;; Stack Fault 
isr12:
	cli
	push byte 0
	push byte 12
	jmp isr_common_stub

;; General Protection Fault
isr13:
	cli
	push byte 0
	push byte 13
	jmp isr_common_stub

;; Page Fault
isr14:
	cli
	push byte 0
	push byte 14
	jmp isr_common_stub

;; Unknown Interrupt 
isr15:
	cli
	push byte 0
	push byte 15
	jmp isr_common_stub

;; Coprocesseur Fault
isr16:
	cli
	push byte 0
	push byte 16
	jmp isr_common_stub

;; Alignment Check
isr17:
	cli
	push byte 0
	push byte 17
	jmp isr_common_stub

;; Machine Check 
isr18:
	cli
	push byte 0
	push byte 18
	jmp isr_common_stub

;; Reserved
isr19:
	cli
	push byte 0
	push byte 19
	jmp isr_common_stub

;; Reserved
isr20:
	cli
	push byte 0
	push byte 20
	jmp isr_common_stub

;; Reserved
isr21:
	cli
	push byte 0
	push byte 21
	jmp isr_common_stub

;; Reserved
isr22:
	cli
	push byte 0
	push byte 22
	jmp isr_common_stub

;; Reserved
isr23:
	cli
	push byte 0
	push byte 23
	jmp isr_common_stub

;; Reserved
isr24:
	cli
	push byte 0
	push byte 24
	jmp isr_common_stub

;; Reserved
isr25:
	cli
	push byte 0
	push byte 25
	jmp isr_common_stub

;; Reserved
isr26:
	cli
	push byte 0
	push byte 26
	jmp isr_common_stub

;; Reserved
isr27:
	cli
	push byte 0
	push byte 27
	jmp isr_common_stub

;; Reserved
isr28:
	cli
	push byte 0
	push byte 28
	jmp isr_common_stub

;; Reserved
isr29:
	cli
	push byte 0
	push byte 29
	jmp isr_common_stub

;; Reserved
isr30:
	cli
	push byte 0
	push byte 30
	jmp isr_common_stub

;; Reserved
isr31:
	cli
	push byte 0
	push byte 31
	jmp isr_common_stub


extern fault_handler

;;  This is our common ISR stub. It saves the processor state, sets
;;  up for kernel mode segments, calls the C-level fault handler,
;;  and finally restores the stack frame.
isr_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10   	; Load the Kernel Data Segment descriptor!
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp   	; Push us the stack
	push eax
	mov eax, fault_handler
	call eax       	; A special call, preserves the 'eip' register
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8     	; Cleans up the pushed error code and pushed ISR number
	iret   
	

	
SECTION .bss
  resb 8192

stack:
