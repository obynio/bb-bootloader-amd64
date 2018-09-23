ENTRY(boot);
OUTPUT_FORMAT(binary);

SECTIONS{
    . = 0x7c00;
    .stage0 : AT(0x7c00) {
        boot.o(.text.origin);
        main.o(.text);
        main.o(.data);
        main.o(.bss);
        main.o(.rodata);
    }
    .magicnb : AT(0x7dfe) {
        SHORT(0xaa55);
    }
}
