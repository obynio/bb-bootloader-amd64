.section .text.origin
.code16
.globl boot

boot:
    cli # disable ring0 interupts
    xor %ax, %ax
    xor %ax, %ax
    mov %ax, %ss
    mov %ax, %es
    mov %ax, %ds
    mov $0x7c00, %sp

    jmpl $0, $stage0

#    mov $0x61, %al
#    mov $0x0e, %ah
#    mov $0x00, %bh
#    mov $0x07, %bl

#    int $0x10
