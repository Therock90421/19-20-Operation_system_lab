void (*printstr)(char *str);
char  value[] = "2019";
void __attribute__((section(".entry_function"))) _start(void)
{
	// Call PMON BIOS printstr to print message "Hello OS!"
        
        char message[] = "Hello OS!\n";
        char v[] = "Version:";
        printstr = (void *)0x80011100;
        (*printstr)(message);
        (*printstr)(v);
        (*printstr)(value);
        
	return;
}
