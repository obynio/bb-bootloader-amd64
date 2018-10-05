OUTPUT_FORMAT(binary);

SECTIONS {
    .stage0 0x7c00 : {
        *(.stage0*);
    }
    .magicnb 0x7dfe : {
        SHORT(0xaa55);
    }
    .stage1 0x7e00 : {
        _stage1_begin = .;
        *(.stage1*);
        _stage1_end = .;
    }
    .stage2 ALIGN(0x200) : {
        _stage2_begin = .;
        *(.stage2*);
        _stage2_end = .;
    }
    .stage3 ALIGN(0x200) : {
        _stage3_begin = .;
        *(.stage3*);
        _stage3_end = .;
    }
    /DISCARD/ : {
        *(.comment*)
        *(.note.*)
    }
}
