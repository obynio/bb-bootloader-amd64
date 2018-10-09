#include "init.h"
#include "root.h"

extern char _beg;
extern char _end;

void printc(char chr) {
    __asm__ volatile ("int $0x10"::"a"(0x0e00 | chr),"b"(0x00));
    return;
}

void printn(uint8_t n)
{
    printc('=');
    printc(n >> 4 + '0');
    printc(n & 0xf + '0');
    printc('\n');
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

static void read_disk(uint8_t nb_sector,
        uint16_t cylinder, uint16_t head, uint8_t sector) {
    uint8_t read_sector = 0;

    __asm__ volatile ("mov $0xffff, \%ax;"
            "mov \%ax, \%es");

    __asm__ volatile ("int $0x13":"=r"(read_sector):
            "a"(0x0200 | nb_sector), // nb_sector = ~45
            "b"(0x10),
            "c"(sector | cylinder << 8),
            "d"(head << 8)
            :"memory");
    //while (1)
    //    printc('A' + read_sector);
}

void stage0() {
    set_a20(1);

    uint16_t cx, dx;
    __asm__ volatile ("int $0x13":
            "=c"(cx),
            "=d"(dx):
            "a"(0x0800),
            "d"(0));

    uint8_t max_sector = cx & 0b00111111;
    uint8_t max_cylinder = cx >> 8;
    uint8_t max_head = dx >> 8;
    uint8_t cylinder = 2048 / (max_head * max_sector);
    uint8_t head = (2048 / max_sector) % max_head;
    uint8_t sector = (2048 % max_sector) + 1;

    read_disk((&_end - &_beg) / 512 + 1, cylinder, head, sector);
    for(;;);

    gdt();
}
