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

void start() {
    printc('b'); 

    set_a20(1);

    if (get_a20() == 1)
        printc('y'); 
    else
        printc('n'); 

    return;
}
