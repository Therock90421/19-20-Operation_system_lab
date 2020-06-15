//void (*printstr)(char *str);

void __attribute__((section(".entry_function"))) _start(void)
{
	// Call PMON BIOS printstr to print message "Hello OS!"

        void (*printstr)(char *str);       
        char *message = "Hello OS!\n";
        char *v = "Version: 2019\n\n";
        char *signature = "made by moto!";
        printstr = (void *)0x80011100;
        (*printstr)(message-0x200);
        (*printstr)(v-0x200);
        
        (*printstr)(signature-0x200);
        while(1);

        
	return;
}
