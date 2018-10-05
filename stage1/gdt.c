#include "root.h"
#include "gdt.h"

struct gdt_entry gdt_entries[3];
struct gdt_ptr gdt_ptr;

static void gdt_set(int32_t entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
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
    gdt_ptr.limit = (sizeof(struct gdt_entry)*3) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_set(0,0,0,0,0);                    //Null segment
    gdt_set(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); //Code segment
    gdt_set(2, 0, 0xFFFFFFFF, 0x92, 0xCF); //Data segment

    // Load gdt
    __asm__ volatile ("lgdt %0"
            :
            : "m"(gdt_ptr)
            : "memory");

    __asm__ volatile ("mov %cr0,%eax;"
            "or $0x1,%eax;"
            "mov %eax,%cr0");

    __asm__ volatile ("movw $0x10,%ax;"
            "movw %ax,%ds;"
            "movw %ax,%es;"
            "movw %ax,%fs;"
            "movw %ax,%gs;"
            "movw %ax,%ss;"
            "jmpl $0x08, $idt");
}

void gdt() {
    prints("FUCKMYLIFE\n");
    init_gdt();
}
