#include "printf.h"
#include "idt.h"
#include "isr.h"

void isr_handler(struct registers regs)
{
    if (regs.int_no == GENERAL_PROTECTION_FAULT)
    {

    }

    printf("-------------\n");
    printf("ISR number %u\n", regs.int_no);
}

void irq_handler(struct registers regs)
{
    //If int_no >= 40, we must reset the slave as well as the master
    if(regs.int_no >= 40)
    {
        //reset slave
    }
    printf("-------------\n");
    printf("IRQ number %u\n", regs.int_no);
}
