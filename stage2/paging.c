#include "gdt.h"
#include "root.h"
#include "paging.h"

static void setup_pml4e(struct pml4e *pml4e_ptr, struct pdpe *base) {
    pml4e_ptr->p = 1;
    pml4e_ptr->rw = 1;
    pml4e_ptr->us = 0;
    pml4e_ptr->pwt = 0;
    pml4e_ptr->pcd = 1;
    pml4e_ptr->a = 0;
    pml4e_ptr->ign = 0;
    pml4e_ptr->mbz = 0;
    pml4e_ptr->avl = 0;
    pml4e_ptr->pdpba = (uint64_t)base >> 12;
    pml4e_ptr->available = 0;
    pml4e_ptr->nx = 0;
}
static void setup_pdpe(struct pdpe *pdpe_ptr, struct pde *base) {
    pdpe_ptr->p = 1;
    pdpe_ptr->rw = 1;
    pdpe_ptr->us = 0;
    pdpe_ptr->pwt = 0;
    pdpe_ptr->pcd = 1;
    pdpe_ptr->a = 0;
    pdpe_ptr->ign = 0;
    pdpe_ptr->zero = 0;
    pdpe_ptr->mbz = 0;
    pdpe_ptr->avl = 0;
    pdpe_ptr->pdpba = (uint64_t)base >> 12;
    pdpe_ptr->available = 0;
    pdpe_ptr->nx = 0;
}
static void setup_pde(struct pde *pde_ptr) {
    for (int i = 0; i < 512; i++, pde_ptr++)
    {
        pde_ptr->p = 1;
        pde_ptr->rw = 1;
        pde_ptr->us = 0; 
        pde_ptr->pwt = 0;
        pde_ptr->pcd = 1;
        pde_ptr->a = 0;
        pde_ptr->d = 0;
        pde_ptr->one = 1;
        pde_ptr->g = 0;
        pde_ptr->avl = 0;
        pde_ptr->pat = 0;
        pde_ptr->mbz = 0;
        pde_ptr->pdpba = i;
        pde_ptr->available = 0;
        pde_ptr->nx = 0;
    }
}

struct pde pde[512] __attribute__((aligned(0x1000)));
struct pdpe pdpe[512] __attribute__((aligned(0x1000)));
struct pml4e pml4e[512] __attribute__((aligned(0x1000)));

static void init_paging()
{
    setup_pml4e(pml4e, pdpe);
    setup_pdpe(pdpe, pde);
    setup_pde(pde);

    // set cr3
    __asm__ volatile ("mov $pml4e, %eax;"
            "mov %eax, %cr3");

    // enable pae
    SET_BIT(cr4, 5); 

    // enable longmode
    __asm__ volatile ("rdmsr" :: "c"(0xc0000080));
    __asm__ volatile ("wrmsr" :: "a"(0x00000100));
   
    // enable paging
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
    gdt_set(2, 0, 0xFFFFFFFF, 0x92, 0xAF); //Data segment

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
