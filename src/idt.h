#include <stdint.h>

#define PORT 0x3f8   /* COM1 */

#define DIVISION_BY_ZERO            0
#define DEBUG_EXCEPTION             1
#define NON_MASKABLE_INTERRUPT      2
#define BREAKPOINT_EXCEPTION        3
#define INTO_DETECTED_OVERFLOW      4
#define OUT_OF_BOUNDS_EXCEPTION     5
#define INVALID_OPCODE_EXCEPTION    6
#define NO_COPROCESSOR_EXCEPTION    7
#define DOUBLE_FAULT                8
#define COPROCESSOR_SEGMENT_OVERRUN 9
#define BAD_TSS                     10
#define SEGMENT_NOT_PRESENT         11
#define STACK_FAULT                 12
#define GENERAL_PROTECTION_FAULT    13
#define PAGE_FAULT                  14
#define UNKNOWN_INTERRUPT_EXCEPTION 15
#define COPROCESSOR_FAULT           16
#define ALIGNMENT_CHECK_EXCEPTION   17
#define MACHINE_CHECK_EXCEPTION     18

#define OUTB(PORT, VAL)                                       \
    __asm__ volatile ("out %%al,%%dx" : : "a"(VAL), "d"(PORT))

// A struct describing an interrupt gate.
struct idt_entry_struct
{
    uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16_t sel;                 // Kernel segment selector.
    uint8_t  always0;             // This must always be zero.
    uint8_t  flags;               // More flags. See documentation.
    uint16_t base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct
{
    uint16_t limit;
    uint32_t base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

void prints_32bits(const char *chr);
