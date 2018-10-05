ENTRY(boot);

SECTIONS{
    .stage0 : {
        boot.o(.text.origin);
        init.o(.text);
        init.o(.data);
        init.o(.bss);
        init.o(.rodata);
    }
}
