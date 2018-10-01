#include <stdint.h>

void printc(char chr) {
    __asm__ volatile ("int $0x10"::"a"(0x0e00 | chr),"b"(0x00));
    return;
}

uint8_t get_a20() {
    uint8_t a = 0;
    __asm__ volatile ("int $0x15":"=r"(a):"a"(0x2402));
    return a;
}

void set_a20(uint8_t status) {
    if (status)
        __asm__ volatile ("int $0x15"::"a"(0x2401));
    else
        __asm__ volatile ("int $0x15"::"a"(0x2400));
}

uint8_t secte() {

    // ah = 0x02 // bios read sector function
    // al = 0x04 // number of sector to read
    // ch = 0x00 // select cylinder 0
    // cl = 0x02 // read second sector
    // dh = 0x00 // select head 0
    // dl = 0x00 // drive number
    //
    // al // number of sector read
    uint8_t read_nb = 0;
    __asm__ volatile ("int $0x13":"=a"(read_nb):"a"(0x2 << 8 & 0x4),"c"(0x2),"d"(0x0));
    return read_nb;
}

void start() {
    printc('b'); 

    set_a20(1);

    if (get_a20() == 1)
        printc('y'); 
    else
        printc('n'); 

    uint8_t read_nb = secte();
    printc(read_nb + 48);

    return;
}
