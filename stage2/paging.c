#include "gdt.h"
#include "root.h"
#include "paging.h"
#include "printf.h"

static void init_paging()
{
    struct pt pt[512] __attribute__((aligned(0x1000)));
    struct pdt pdt[512] __attribute__((aligned(0x1000)));
    struct pdpt pdpt[512] __attribute__((aligned(0x1000)));
    struct pml4e pml4e[512] __attribute__((aligned(0x1000)));

    for (int i = 0; i < 512; ++i)
    {
        pt[i].present = 1;
        pt[i].rw = 1;
        pt[i].huge_page = 1;
        pt[i].frame = 0x20000 * i;
    }

    for (int i = 0; i < 512; ++i)
    {
        *(uint64_t*)(pdt + i) = (uint64_t)(pt + i);
        pdt[i].present = 1;
        pdt[i].rw = 1;
    }

    for (int i = 0; i < 512; ++i)
    {
        *(uint64_t*)(pdpt + i) = (uint64_t)(pdt + i);
        pdpt[i].present = 1;
        pdpt[i].rw = 1;
    }

    for (int i = 0; i < 512; ++i)
    {
        *(uint64_t*)(pml4e + i) = (uint64_t)(pdpt + i);
        pml4e[i].present = 1;
        pml4e[i].rw = 1;
    }

    printf("bPAE\n");
    // Set PAE
    SET_BIT(cr4, 5);

    printf("bLME\n");
    // Set MSR for LME
    __asm__ volatile ("rdmsr" :: "c"(0xc0000080));
    __asm__ volatile ("wrmsr" :: "a"(0x00000100));

    printf("bCR3\n");
    // Set cr3 to pml4e address
    __asm__ volatile ("mov %0, %%eax;"
            "mov %%eax, %%cr3"
            :
            : "m"(pml4e)
            : "memory");

    printf("bPG\n");
    // Enable PG bit (paging)
    SET_BIT(cr0, 31);
}

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

    gdt_set(1, 0, 0xFFFFFFFF, 0x9A, 0xAF); //Code segment
    gdt_set(2, 0, 0xFFFFFFFF, 0, 0xCF);    //Data segment

    __asm__ volatile ("lgdt %0"
            :
            : "m"(gdt_ptr)
            : "memory");
    __asm__ volatile ("ljmp $0x08, $test");
}

void paging()
{
    init_paging();
    init_gdt();
}
