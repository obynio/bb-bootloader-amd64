#include <stdint.h>

// init.c
void printc(char chr);
void prints(char *chr);
uint8_t read_disk(uint8_t nb_sector);

// gdt.c
void gdt();

// idt.c
void idt();
void prints_32bits(const char *);

// paging.c
void paging();
