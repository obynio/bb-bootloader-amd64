#include "root.h"
#include "paging.h"

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

void paging()
{
    init_paging();
    gdt_64();
}
