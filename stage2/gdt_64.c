#include "root.h"
#include "gdt.h"

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

static void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t)*3) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_set_gate(0,0,0,0,0);                    //Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xAF); //Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0, 0xCF);    //Data segment

    __asm__ volatile ("lgdt %0"
            :
            : "m"(gdt_ptr)
            : "memory");
    __asm__ volatile ("movw $0x10,\%ax;"
            "movw \%ax,\%ds;"
            "movw \%ax,\%es;"
            "movw \%ax,\%fs;"
            "movw \%ax,\%gs;"
            "movw \%ax,\%ss;"
            "ljmp $0x08, $idt");
}

void gdt_64() {
    init_gdt();
    while (1)
        ;
}
