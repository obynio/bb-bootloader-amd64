SECTIONS{
    .stage2 : {
        idt_asm.o(.text.origin);
        *.o(.text);
        *.o(.data);
        *.o(.bss);
        *.o(.rodata);
    }
}
