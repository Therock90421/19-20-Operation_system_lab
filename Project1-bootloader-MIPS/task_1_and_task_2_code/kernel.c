void (*printstr)(char *str);

void __attribute__((section(".entry_function"))) _start(void)
{
	// Call PMON BIOS printstr to print message "Hello OS!"
        
        char message[] = "Hello OS!";
        printstr = (void *)0x80011100;
        (*printstr)(message);
        
	return;
}
