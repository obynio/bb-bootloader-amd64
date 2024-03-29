#include <stdint.h>

#define STRINGIFY(in) # in

#define SET_BIT(reg, bit)                               \
    __asm__ volatile ("mov \%" STRINGIFY(reg) ",\%eax;" \
            "bts $" STRINGIFY(bit) ",\%eax;"      \
            "mov \%eax,\%" STRINGIFY(reg))

// init.c
void printc(char chr);
void prints(char *chr);

// gdt.c
void gdt();

// idt.c
void idt();
void prints_32bits(const char *);
void printc_32bits(const char);

// paging.c
void paging();

// test.c
void test();
