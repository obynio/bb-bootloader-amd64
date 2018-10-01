#include "stage.h"

void idt() {
    __asm__ volatile ("mov $0x0,%eax");
}
