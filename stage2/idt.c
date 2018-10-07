#include "idt.h"
#include "isr.h"
#include "root.h"
#include "serial.h"

static void init_serial() {
    OUTB(PORT + 1, 0x00); // Disable all interrupts
    OUTB(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    OUTB(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    OUTB(PORT + 1, 0x00); //                  (hi byte)
    OUTB(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    OUTB(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    OUTB(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

void printc_32bits(char c)
{
    OUTB(PORT, c);
}

void prints_32bits(const char *chr)
{
    while (*chr)
        printc_32bits(*chr++);
}

void stage2_victory()
{
    char s[] = "\n32bits! \\o/\n";
    prints_32bits(s);
}

struct idt_entry idt_entries[256];
struct idt_ptr idt_ptr;

// These macros define the handlers
//
// ISR Handlers
ISR(0)
ISR(1)
ISR(2)
ISR(3)
ISR(4)
ISR(5)
ISR(6)
ISR(7)
ISRERR(8)
ISR(9)
ISRERR(10)
ISRERR(11)
ISRERR(12)
ISRERR(13)
ISRERR(14)
ISR(15)
ISR(16)
ISR(17)
ISR(18)
ISR(19)
ISR(20)
ISR(21)
ISR(22)
ISR(23)
ISR(24)
ISR(25)
ISR(26)
ISR(27)
ISR(28)
ISR(29)
ISR(30)
ISR(31)
//
// IRQ Handlers
IRQ(0)
IRQ(1)
IRQ(2)
IRQ(3)
IRQ(4)
IRQ(5)
IRQ(6)
IRQ(7)
IRQ(8)
IRQ(9)
IRQ(10)
IRQ(11)
IRQ(12)
IRQ(13)
IRQ(14)
IRQ(15)

void (*idt_funcs[32])() =
{
    &isr0, &isr1, &isr2, &isr3,
    &isr4, &isr5, &isr6, &isr7,
    &isr8, &isr9, &isr10, &isr11,
    &isr12, &isr13, &isr14, &isr15,
    &isr16, &isr17, &isr18, &isr19,
    &isr20, &isr21, &isr22, &isr23,
    &isr24, &isr25, &isr26, &isr27,
    &isr28, &isr29, &isr30, &isr31
};

void (*irq_funcs[16])() =
{
    &irq0, &irq1, &irq2, &irq3,
    &irq4, &irq5, &irq6, &irq7,
    &irq8, &irq9, &irq10, &irq11,
    &irq12, &irq13, &irq14, &irq15
};

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags   = flags /* | 0x60 */;
}

void init_idt()
{
    idt_ptr.limit = sizeof(struct idt_entry) * 256 -1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    // ISR entries
    for (uint8_t i = 0; i < 32; ++i)
        idt_set_gate( i, (uint32_t)idt_funcs[i], 0x08, 0x8E);

    // IRQ entries
    for (uint8_t i = 0; i < 16; ++i)
        idt_set_gate(i + 32, (uint32_t)irq_funcs[i], 0x08, 0x8E);

    // Reload our idt
    __asm__ volatile ("lidt %0"
            :
            : "m"(idt_ptr)
            : "memory");
}

void idt() {
    init_serial();
    init_idt();
    // Jump to paging stage
    paging();
}
