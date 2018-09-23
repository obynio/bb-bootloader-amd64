void start() {
    __asm__ volatile ("int $0x10"::"a"(0x0e00 | 'a'),"b"(0x00));
    return;
}
