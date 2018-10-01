#include "stage.h"

extern char begin_addr;

void printc(char chr) {
    __asm__ volatile ("int $0x10"::"a"(0x0e00 | chr),"b"(0x00));
    return;
}

void prints(char *chr) {
    while (*chr)
        printc(*chr++);
}

// return true if init is successful
uint8_t get_a20() {
    uint8_t a = 0;
    __asm__ volatile ("int $0x15":"=r"(a):"a"(0x2402));
    return a;
}

// return true if a20 is initialized
void set_a20(uint8_t status) {
    if (status)
        __asm__ volatile ("int $0x15"::"a"(0x2401));
    else
        __asm__ volatile ("int $0x15"::"a"(0x2400));
}

// return true if init is successful
uint8_t find_ram() {
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

uint8_t read_disk(uint8_t nb_sector) {

    // ah = 0x02 // bios read sector function
    // al = 0x04 // number of sector to read
    // ch = 0x00 // select cylinder 0
    // cl = 0x02 // read second sector
    // dh = 0x00 // select head 0
    // dl = 0x00 // drive number
    //
    // al // number of sector read
    uint8_t read_sector = 0;
    __asm__ volatile ("int $0x13":"=r"(read_sector):
            "a"(0x0200 | nb_sector),
            "b"(&begin_addr),
            "c"(0x2),
            "d"(0x0));

    return read_sector;
}

void stage0() {
    char boot[] = "boot";
    prints(boot);

    set_a20(1);

    if (get_a20())
    {
        char a20[] = "/a20";
        prints(a20);
    }

    if (find_ram())
    {
        char ram[] = "/ram";
        prints(ram);
    }
    
    char read[] = "/read";
    prints(read);

    uint8_t read_nb = read_disk(5);
    printc(read_nb + 48);

    stage1();

    return;
}
