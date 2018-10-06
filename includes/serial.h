#define PORT 0x3f8   /* COM1 */

#define OUTB(PORT, VAL)                                       \
    __asm__ volatile ("out %%al,%%dx" : : "a"(VAL), "d"(PORT))

void prints_32bits(const char *chr);
void prints_64bits(const char *chr);
