#include "irq.h"
#include "time.h"
#include "sched.h"
#include "string.h"
#include "screen.h"

static void irq_timer()
{
    // TODO clock interrupt handler.
    // scheduler, time counter in here to do, emmmmmm maybe.
	current_running->cursor_x = screen_cursor_x; //保存屏幕指针现场
	current_running->cursor_y = screen_cursor_y;
	do_scheduler(); //调度
	//printk("%x\n",current_running->kernel_context.regs[31]);
	//printk("%x\n",current_running->kernel_context.cp0_status);
	//printk("%x\n",current_running->user_context.regs[31]);
	screen_cursor_x = current_running->cursor_x;
	screen_cursor_y = current_running->cursor_y;
	SET_CP0_COUNT(0);
	SET_CP0_COMPARE(20000);
	return;
}

void interrupt_helper(uint32_t status, uint32_t cause)
{
    // TODO interrupt handler.
    // Leve3 exception Handler.
    // read CP0 register to analyze the type of interrupt.
	uint32_t interrupt_kind = cause & 0x0000ff00;
	if(interrupt_kind & 0x00008000)//时钟中断
		irq_timer();
	else
		other_exception_handler();
	return;
}

void other_exception_handler()
{
    // TODO other exception handler
	return;
}