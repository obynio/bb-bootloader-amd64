OUTPUT_FORMAT(elf64-x86-64)

SECTIONS{
    .stage3 : {
        *.o(.text);
        *.o(.data);
        *.o(.bss);
        *.o(.rodata);
    }
}
