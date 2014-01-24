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
