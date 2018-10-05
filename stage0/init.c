#include "root.h"

extern uint8_t _stage1_begin;
extern uint8_t _stage2_begin;
extern uint8_t _stage3_begin;

extern uint8_t _stage1_end;
extern uint8_t _stage2_end;
extern uint8_t _stage3_end;

void printc(char chr) {
    __asm__ volatile ("int $0x10"::"a"(0x0e00 | chr),"b"(0x00));
    return;
}

void prints(char *chr) {
    while (*chr)
        printc(*chr++);
}

// return true if init is successful
/*
static uint8_t get_a20() {
    uint8_t a = 0;
    __asm__ volatile ("int $0x15":"=r"(a):"a"(0x2402));
    return a;
}
*/

// return true if a20 is initialized
static void set_a20(uint8_t status) {
    if (status)
        __asm__ volatile ("int $0x15"::"a"(0x2401));
    else
        __asm__ volatile ("int $0x15"::"a"(0x2400));
}

// return true if init is successful
static uint8_t find_ram() {
    uint8_t flags = 0;
    __asm__ volatile ("int $0x15":"=r"(flags):
            "a"(0xe820),
            "b"(0x0),
            "c"(0x20),
            "d"(0x534d4150));

    // get cf flag
    // cf clear if successful and set on error
    return !(flags & 0x1);
}

static uint8_t read_disk(uint8_t *begin, uint8_t size, uint8_t sector) {
    uint8_t read_sector = 0;
    printc('A');
    printc(size + 'a');
    __asm__ volatile ("int $0x13":"=r"(read_sector):
            "a"(0x0200 | size),
            "b"(begin),
            "c"(sector),
            "d"(0x0));

    return read_sector;
}

void stage0() {
    set_a20(1);
    find_ram();

    uint8_t read_nb = 2;
    read_nb += read_disk((uint8_t*)&_stage1_begin,
            ((uint8_t)&_stage1_end - (uint8_t)&_stage1_begin) / 512 + 1,
            read_nb);

    read_nb += read_disk((uint8_t*)&_stage2_begin,
            ((uint8_t)&_stage2_end - (uint8_t)&_stage2_begin) / 512 + 1,
            read_nb);

    read_nb += read_disk((uint8_t*)&_stage3_begin,
            ((uint8_t)&_stage3_end - (uint8_t)&_stage3_begin) / 512 + 1,
            read_nb);

    gdt();
}
