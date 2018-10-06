#include "root.h"
#include "idt.h"
#include "isr.h"

void isr_handler(struct registers regs)
{
    if (regs.int_no == GENERAL_PROTECTION_FAULT)
    {

    }

    prints_32bits("-------------\n");
    prints_32bits("ISR number ");
    printc_32bits(regs.int_no + 'a');
}

void irq_handler(struct registers regs)
{
    //If int_no >= 40, we must reset the slave as well as the master
    if(regs.int_no >= 40)
    {
        //reset slave
    }
    prints_32bits("-------------\n");
    prints_32bits("IRQ number ");
    printc_32bits(regs.int_no + 'a');
}
