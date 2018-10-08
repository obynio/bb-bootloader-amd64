#include "root.h"

void jump()
{
    idt();
    __asm__ volatile("mov $0x1000, \%ax;"
            "mov \%ax, \%es;"
            "mov $0x00, \%bx;"
            "push \%es;"
            "push \%bx;"
            "retf");
}
