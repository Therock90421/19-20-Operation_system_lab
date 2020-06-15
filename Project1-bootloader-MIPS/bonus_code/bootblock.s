.text
.global main

main:
	# 1) task1 call BIOS print string "It's bootblock!"
	la   $a0, msg
        jal  0x80011100
	# 2) task2 call BIOS read kernel in SD card and jump to kernel start
        lw   $a0, kernel_main      #kernel覆盖bootblock
        li   $a1, 0x200
        li   $a2, 0x200
        lw   $ra, kernel_main      #手动存储返回地址    如此一来在函数执行结束之后返回的就是kernel的开头    
        lw   $t2, read_sd_card
        jr   $t2            #执行read_sd_Card
       
# while(1) --> stop here
stop:
	j stop

.data

msg: .ascii "It's a bootloader...\n"

# 1. PMON read SD card function address
# read_sd_card();
read_sd_card: .word 0x80011000

# 2. PMON print string function address
# printstr(char *string)
printstr: .word 0x80011100

# 3. PMON print char function address
# printch(char ch)
printch: .word 0x80011140

# 4. kernel address (move kernel to here ~)
kernel : .word 0xa0800000

# 5. kernel main address (jmp here to start kernel main!)
kernel_main : .word 0xa0800000
