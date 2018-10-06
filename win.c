void win(int (*printf_ptr)(const char *fmt, ...))
{
        printf_ptr("%s\n", "Hello World");
}
