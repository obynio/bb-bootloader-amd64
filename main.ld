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
    .gdt : AT(0x7e00) {
        gdt_addr = .;
        gdt.o(.text);
        gdt.o(.data);
        gdt.o(.bss);
        gdt.o(.rodata);
    }
    .idt : {
        idt_addr = .;
        idt.o(.text);
        idt.o(.data);
        idt.o(.bss);
        idt.o(.rodata);
    }
    .isr : {
        isr_addr = .;
        isr.o(.text);
        isr.o(.data);
        isr.o(.bss);
        isr.o(.rodata);
    }
    .paging : {
        paging_addr = .;
        paging.o(.text);
        paging.o(.data);
        paging.o(.bss);
        paging.o(.rodata);
    }
}
