SECTIONS {
    .stage1 : {
        *.o(.text);
        *.o(.data);
        *.o(.bss);
        *.o(.rodata);
    }
}
