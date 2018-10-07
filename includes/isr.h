#pragma once
#include <stdint.h>

#define ISR(n)                          \
__attribute__((naked))                  \
void isr##n()                           \
{                                       \
    __asm__ volatile("cli");            \
    __asm__ volatile("push $0");        \
    __asm__ volatile("push $" #n);      \
    isr_stub();                         \
}

#define ISRERR(n)                       \
__attribute__((naked))                  \
void isr##n()                           \
{                                       \
    __asm__ volatile("cli");            \
    __asm__ volatile("push $" #n);      \
    isr_stub();                         \
}

#define IRQ(n)                          \
__attribute__((naked))                  \
void irq##n()                           \
{                                       \
    __asm__ volatile("cli");            \
    __asm__ volatile("push $0");        \
    __asm__ volatile("push $" #n);      \
    irq_stub();                         \
}


struct registers
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
};

// Enables registration of callbacks for interrupts or IRQs.
// For IRQs, to ease confusion, use the #defines above as the
// first parameter.
typedef void (*isr_t)(struct registers);
void register_interrupt_handler(uint8_t n, isr_t handler);

void isr_stub();
void irq_stub();
