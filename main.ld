ENTRY(boot);
OUTPUT_FORMAT(binary);

SECTIONS{
    . = 0x7c00;
    .stage0 : AT(0x7c00) {
        boot.o(.text.origin);
        stage0.o(.text);
        stage0.o(.data);
        stage0.o(.bss);
        stage0.o(.rodata);
    }
    .magicnb : AT(0x7dfe) {
        SHORT(0xaa55);
    }
    .stage1 : {
        begin_addr = .;
        stage1.o(.text);
        stage1.o(.data);
        stage1.o(.bss);
        stage1.o(.rodata);
    }
 
}
