#include "stage.h"

#define PORT 0x3f8   /* COM1 */

void outb(uint16_t port, uint8_t v)
{
    __asm__ volatile ("out %%al,%%dx" : : "a"(v), "d"(port));
}

void init_serial() {
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

void victory()
{
    outb(PORT, '\n');
    outb(PORT, '3');
    outb(PORT, '2');
    outb(PORT, 'b');
    outb(PORT, 'i');
    outb(PORT, 't');
    outb(PORT, 's');
    outb(PORT, '!');
    outb(PORT, ' ');
    outb(PORT, '\\');
    outb(PORT, 'o');
    outb(PORT, '/');
    outb(PORT, '\n');
}

void idt() {
    __asm__ volatile ("mov $0x0,%eax");
    init_serial();
    victory();
    while (1)
        ;
}
