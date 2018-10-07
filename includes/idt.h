#include <stdint.h>

// Describes an interrupt gate
struct idt_entry
{
    uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16_t sel;                 // Kernel segment selector.
    uint8_t  always0;             // This must always be zero.
    uint8_t  flags;               // More flags. See documentation.
    uint16_t base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));

// Array of handlers, suitable for giving to 'lidt'.
struct idt_ptr
{
    uint16_t limit;
    uint32_t base; // The address of the first idt_entry element
} __attribute__((packed));
