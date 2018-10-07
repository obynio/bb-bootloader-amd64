#include "root.h"
#include "idt.h"
#include "isr.h"

__attribute__((naked))
void isr_stub()
{
    __asm__ volatile("pusha");
    __asm__ volatile("call isr_handler");
    __asm__ volatile("popa");
    __asm__ volatile("add $8, %esp");
    __asm__ volatile("sti");
    __asm__ volatile("iret");
}

__attribute__((naked))
void irq_stub()
{
    __asm__ volatile("pusha");
    __asm__ volatile("call irq_handler");
    __asm__ volatile("popa");
    __asm__ volatile("add $8, %esp");
    __asm__ volatile("sti");
    __asm__ volatile("iret");
}

void isr_handler(struct registers regs)
{
    prints_32bits("-------------\n");
    prints_32bits("ISR number ");
    printc_32bits(regs.int_no + 'a');
}

void irq_handler(struct registers regs)
{
    prints_32bits("-------------\n");
    prints_32bits("IRQ number ");
    printc_32bits(regs.int_no + 'a');
}
