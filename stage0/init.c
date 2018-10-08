#include "init.h"
#include "root.h"

extern char _next_addr;

void printc(char chr) {
    __asm__ volatile ("int $0x10"::"a"(0x0e00 | chr),"b"(0x00));
    return;
}

void prints(char *chr) {
    while (*chr)
        printc(*chr++);
}

// return true if a20 is initialized
static void set_a20(uint8_t status) {
    if (status)
        __asm__ volatile ("int $0x15"::"a"(0x2401));
    else
        __asm__ volatile ("int $0x15"::"a"(0x2400));
}

static uint8_t read_disk(uint16_t addr, uint8_t nb_sector, uint8_t sector) {
    uint8_t read_sector = 0;

    if (addr == 0x10)
    {
        __asm__ volatile ("mov $0xffff, \%ax");
        __asm__ volatile ("mov \%ax, \%es");
    }

    __asm__ volatile ("int $0x13":"=r"(read_sector):
            "a"(0x0200 | nb_sector),
            "b"(addr),
            "c"(0x0000 | sector),
            "d"(0x0));

    return read_sector;
}

void stage0() {
    set_a20(1);

    read_disk(&_next_addr, (STAGE1_SIZE) / 512,
            2);
    //TODO ?
    read_disk(0x10, (STAGE2_SIZE + STAGE3_SIZE) / 512,
            2);

    gdt();
}
