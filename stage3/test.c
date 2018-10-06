#include "printf.h"
#include "root.h"

extern void win(int (*printf_ptr)(const char *fmt, ...));

void test()
{
    win(&printf);
    for (;;);
}
