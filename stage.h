
#include <stdint.h>

// stage0.c
void printc(char chr);
void prints(char *chr);
uint8_t read_disk(uint8_t nb_sector);

// stage1.c
void stage1();
