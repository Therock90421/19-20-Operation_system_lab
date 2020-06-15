#include "irq.h"
#include "time.h"
#include "sched.h"
#include "string.h"
#include "screen.h"

static void irq_timer()
{
    // TODO clock interrupt handler.
    // scheduler, time counter in here to do, emmmmmm maybe.
	
	/*current_running->cursor_x = screen_cursor_x; //保存屏幕指针现场
	current_running->cursor_y = screen_cursor_y;
	do_scheduler(); //调度
	screen_cursor_x = current_running->cursor_x;
	screen_cursor_y = current_running->cursor_y;
	SET_CP0_COUNT(0);
	SET_CP0_COMPARE(10000);*/    //时钟中断轮询方式

	screen_reflush();
    //time_elapsed += 3000000;
	time_elapsed += 100000;
    current_running->cursor_x = screen_cursor_x;
    current_running->cursor_y = screen_cursor_y;
    
    do_scheduler();
    screen_cursor_x = current_running->cursor_x;
    screen_cursor_y = current_running->cursor_y;    //基于sleep的系统中断调度

	return;
}

void interrupt_helper(uint32_t status, uint32_t cause)
{
    // TODO interrupt handler.
    // Leve3 exception Handler.
    // read CP0 register to analyze the type of interrupt.
	uint32_t interrupt_kind = status & cause & 0x0000ff00;
	if(interrupt_kind & 0x00008000)//时钟中断
		irq_timer();
	else
		other_exception_handler();
	return;
}

void other_exception_handler()
{
    // TODO other exception handler
	time_elapsed += 100000; //基于sleep的系统调用调度方式
	//time_elapsed += 3000000;
	return;
}