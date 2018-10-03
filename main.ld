ENTRY(boot);
OUTPUT_FORMAT(binary);

SECTIONS{
    . = 0x7c00;
    .stage0 : AT(0x7c00) {
        boot.o(.text.origin);
        init.o(.text);
        init.o(.data);
        init.o(.bss);
        init.o(.rodata);
    }
    .magicnb : AT(0x7dfe) {
        SHORT(0xaa55);
    }
    .stage1 : AT(0x7e00) {
        _stage1_addr = .;
        stage1/*.o(.text);
        stage1/*.o(.data);
        stage1/*.o(.bss);
        stage1/*.o(.rodata);
    }
    .stage2 : {
        idt_asm.o(.text.origin);
        stage2/*.o(.text);
        stage2/*.o(.data);
        stage2/*.o(.bss);
        stage2/*.o(.rodata);
    }
}
