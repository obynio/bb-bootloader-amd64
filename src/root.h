#include <stdint.h>

// stage0.c
void printc(char chr);
void prints(char *chr);
uint8_t read_disk(uint8_t nb_sector);

// gdt.c
void gdt();

// idt.c
void idt();
