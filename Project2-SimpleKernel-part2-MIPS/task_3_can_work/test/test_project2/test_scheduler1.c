#include "sched.h"
#include "stdio.h"
#include "test2.h"
#include "syscall.h"

static char blank[] = {"                   "};
static char plane1[] = {"    ___         _  "};
static char plane2[] = {"| __\\_\\______/_| "};
static char plane3[] = {"<[___\\_\\_______| "};
static char plane4[] = {"|  o'o             "};

static void dis_int()
{
    uint32_t cp0_status = GET_CP0_STATUS();
    cp0_status &= 0xfffffffe;
    SET_CP0_STATUS(cp0_status);
}

static void en_int()
{
    uint32_t cp0_status = GET_CP0_STATUS();
    cp0_status |= 0x01;
    SET_CP0_STATUS(cp0_status);
}


void printk_task1(void)
{
    int i;
    int print_location = 1;
	
    for (i = 0;; i++)
    {   
		dis_int();
        vt100_move_cursor(1, print_location);
        printk("> [TASK] This task is to test scheduler. (%d)", i);
        //do_scheduler();
		en_int();
    }
}

void printk_task2(void)
{
    int i;
    int print_location = 2;

    for (i = 0;; i++)
    {
		dis_int();
        vt100_move_cursor(1, print_location);
        printk("> [TASK] This task is to test scheduler. (%d)", i);
        //do_scheduler();
		en_int();
    }
}

void drawing_task1(void)
{
    int i, j = 22;

    while (1)
    {
		dis_int();
        for (i = 60; i > 0; i--)
        {
            /* move */
            vt100_move_cursor(i, j + 0);
            printk("%s", plane1);

            vt100_move_cursor(i, j + 1);
            printk("%s", plane2);

            vt100_move_cursor(i, j + 2);
            printk("%s", plane3);

            vt100_move_cursor(i, j + 3);
            printk("%s", plane4);
        }
        //do_scheduler();
		en_int();

		dis_int();
        vt100_move_cursor(1, j + 0);
        printk("%s", blank);

        vt100_move_cursor(1, j + 1);
        printk("%s", blank);

        vt100_move_cursor(1, j + 2);
        printk("%s", blank);

        vt100_move_cursor(1, j + 3);
        printk("%s", blank);
		en_int();
    }
}