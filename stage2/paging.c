#include "gdt.h"
#include "root.h"
#include "paging.h"

static void init_paging()
{
    struct pt pt[32] __attribute__((aligned(0x1000)));
    struct pdt pdt[32] __attribute__((aligned(0x1000)));
    struct pdpt pdpt[32] __attribute__((aligned(0x1000)));
    struct pml4e pml4e[32] __attribute__((aligned(0x1000)));

    for (int i = 0; i < 32; ++i)
    {
        pt[i].present = 1;
        pt[i].rw = 1;
        pt[i].huge_page = 1;
        pt[i].frame = 0x20000 * i;
    }

    for (int i = 0; i < 1; ++i)
    {
        *(uint64_t*)(pdt + i) = (uint64_t)(pt + i);
        pdt[i].present = 1;
        pdt[i].rw = 1;
    }

    for (int i = 0; i < 1; ++i)
    {
        *(uint64_t*)(pdpt + i) = (uint64_t)(pdt + i);
        pdpt[i].present = 1;
        pdpt[i].rw = 1;
    }

    for (int i = 0; i < 1; ++i)
    {
        *(uint64_t*)(pml4e + i) = (uint64_t)(pdpt + i);
        pml4e[i].present = 1;
        pml4e[i].rw = 1;
    }

    //prints_32bits("bPAE\n");
    // Set PAE
    SET_BIT(cr4, 5);

    //prints_32bits("bLME\n");
    // Set MSR for LME
    __asm__ volatile ("rdmsr" :: "c"(0xc0000080));
    __asm__ volatile ("wrmsr" :: "a"(0x00000100));

    //prints_32bits("bCR3\n");
    // Set cr3 to pml4e address
    __asm__ volatile ("mov %0, %%eax;"
            "mov %%eax, %%cr3"
            :
            : "m"(pml4e)
            : "memory");

    //prints_32bits("bPG\n");
    // Enable PG bit (paging)
    SET_BIT(cr0, 31);
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

static void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t)*3) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xAF); //Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xAF); //Data segment

    __asm__ volatile ("lgdt %0"
            :
            : "m"(gdt_ptr)
            : "memory");

    __asm__ volatile ("cli");
    __asm__ volatile ("movw $0x10,\%ax;"
            "movw \%ax,\%ds;"
            "movw \%ax,\%es;"
            "movw \%ax,\%fs;"
            "movw \%ax,\%gs;"
            "movw \%ax,\%ss;"
            "ljmp $0x08, $test");
}

void paging()
{
    init_paging();
    init_gdt();
}
