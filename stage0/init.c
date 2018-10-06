#include "init.h"
#include "root.h"

extern char _stage1_begin;
extern char _stage2_begin;
extern char _stage3_begin;

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

/*
static uint8_t find_ram(struct disk_info *buffer) {
    uint32_t cont_id = 0;
    uint8_t flags = 0;

    do {
        __asm__ volatile ("int $0x15":"=r"(flags), "=b"(cont_id):
            "a"(0xe820),
            "b"(0x0),
            "c"(0x20),
            "d"(0x534d4150),
            "D"(buffer));
    } while (!buffer->type);

    // get cf flag
    // cf clear if successful and set on error
    return !(flags & 0x1);
}
*/

static uint8_t read_disk(uint16_t begin, uint8_t nb_sector, uint8_t sector) {
    uint8_t read_sector = 0;

    __asm__ volatile ("int $0x13":"=r"(read_sector):
            "a"(0x0200 | nb_sector),
            "b"(begin),
            "c"(0x0000 | sector),
            "d"(0x0));

    return read_sector;
}

void stage0() {
    set_a20(1);

    uint8_t read_nb = 2;
    read_nb += read_disk(&_stage1_begin,
            (STAGE1_SIZE + STAGE2_SIZE + STAGE3_SIZE) / 512, read_nb);

    gdt();
}
