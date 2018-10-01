void printn(uint8_t c)
{
    if (c > 0x9)
        printc(c - 0xa + 'a');
    else
        printc(c + '0');
}

void printuint8(uint8_t i)
{
    printn(i >> 4);
    printn(i & 0x0F);
    printc(' ');
}

void hexdump()
{
    printc('\n');
    for (uint8_t *c = gdt_entries; c < gdt_entries + 10; c++)
        printuint8(*c);
}
