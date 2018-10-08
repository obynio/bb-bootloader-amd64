OUTPUT_FORMAT(binary);

SECTIONS {
    .stage01 0x7c00 : {
        *(.stage01*);
    }
    .stage02 : {
        *(.stage02*);
    }
    .magicnb 0x7dfe : {
        SHORT(0xaa55);
    }
    .stage1 ALIGN(0x200) : {
        _next_addr = .;
        *(.stage1*);
    }
    .stage2 0x100000 : {
        *(.stage2*);
    }
    .stage3 ALIGN(0x200) : {
        *(.stage3*);
    }
    /DISCARD/ : {
        *(.comment*)
        *(.note.*)
    }
}
