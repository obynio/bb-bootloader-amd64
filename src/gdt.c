#include "stage.h"
#include "gdt.h"

static void gdt_set_gate(int32_t,uint32_t,uint32_t,uint8_t,uint8_t);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;
//idt_entry_t idt_entries[256];
//idt_ptr_t   idt_ptr;

//void printn(uint8_t c)
//{
//    if (c > 0x9)
//        printc(c - 0xa + 'a');
//    else
//        printc(c + '0');
//}
//
//void printuint8(uint8_t i)
//{
//    printn(i >> 4);
//    printn(i & 0x0F);
//    printc(' ');
//}
//
//void hexdump()
//{
//    printc('\n');
//    for (uint8_t *c = gdt_entries; c < gdt_entries + 10; c++)
//        printuint8(*c);
//}

void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t)*3) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_set_gate(0,0,0,0,0);                    //Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); //Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); //Data segment
    //gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); //User mode code segment
    //gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); //User mode data segment

    // Load gdt
    __asm__ volatile ("lgdt %0"
            :
            : "m"(gdt_ptr)
            : "memory");

    //hexdump();

    __asm__ volatile ("mov \%cr0,\%eax;"
            "or $0x1, \%eax;"
            "mov \%eax,\%cr0");

    __asm__ volatile ("movw $0x10,\%ax;"
            "movw \%ax,\%ds;"
            "movw \%ax,\%es;"
            "movw \%ax,\%fs;"
            "movw \%ax,\%gs;"
            "movw \%ax,\%ss;"
            "ljmp $0x08, $idt");
}

static void gdt_set_gate(int32_t entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[entry].base_low = (base & 0xFFFF);
    gdt_entries[entry].base_middle = (base >> 16) & 0xFF;
    gdt_entries[entry].base_high = (base >> 24) & 0xFF;

    gdt_entries[entry].limit_low = (limit & 0xFFFF);
    gdt_entries[entry].granularity = (limit >> 16) & 0x0F;

    gdt_entries[entry].granularity |= gran & 0xF0;
    gdt_entries[entry].access = access;
}

void gdt() {
    char read[] = "/ok";
    prints(read);
    init_gdt();
}