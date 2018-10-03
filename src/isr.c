#include "idt.h"
#include "isr.h"

void isr_handler(struct registers regs)
{
    if (regs.int_no == GENERAL_PROTECTION_FAULT)
    {

    }

    OUTB(PORT, regs.int_no + '0');
}

void irq_handler(struct registers regs)
{
    //If int_no >= 40, we must reset the slave as well as the master
    if(regs.int_no >= 40)
    {
        //reset slave
    }
    OUTB(PORT, '4');
}
