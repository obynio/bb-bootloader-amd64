ENTRY(boot);

SECTIONS{
    .stage01 : {
        boot.o(.text.origin);
        init.o(.text);
        init.o(.data);
        init.o(.bss);
        init.o(.rodata);
    }
    .stage02 : {
        gdt.o(.text);
        gdt.o(.data);
        gdt.o(.bss);
        gdt.o(.rodata);
    }
}
