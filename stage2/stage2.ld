SECTIONS{
    .stage2 : {
        *.o(.text);
        *.o(.data);
        *.o(.bss);
        *.o(.rodata);
    }
}
