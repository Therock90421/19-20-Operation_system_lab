#include "test2.h"
#include "lock.h"
#include "stdio.h"
#include "syscall.h"

int is_init = FALSE;
static char blank[] = {"                                             "};

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

/* if you want to use spin lock, you need define SPIN_LOCK */
//  #define SPIN_LOCK
spin_lock_t spin_lock;

/* if you want to use mutex lock, you need define MUTEX_LOCK */
#define MUTEX_LOCK
mutex_lock_t mutex_lock;

void lock_task1(void)
{
      //  int print_location = 1;
	    int print_location = 3;
        while (1)
        {
			dis_int();
                int i;
                if (!is_init)
                {

#ifdef SPIN_LOCK
                        spin_lock_init(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                        do_mutex_lock_init(&mutex_lock);
#endif
                        is_init = TRUE;
                }

                vt100_move_cursor(1, print_location);
                printk("%s", blank);

                vt100_move_cursor(1, print_location);
                printk("> [TASK] Applying for a lock.\n");

				en_int();
                //do_scheduler();
				dis_int();
#ifdef SPIN_LOCK
                spin_lock_acquire(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                do_mutex_lock_acquire(&mutex_lock);
#endif
				en_int();
                for (i = 0; i < 20; i++)
                {
					dis_int();
                        vt100_move_cursor(1, print_location);
                        printk("> [TASK] Has acquired lock and running.(%d)\n", i);
                      en_int();  
						//do_scheduler();
                }
				dis_int();
                vt100_move_cursor(1, print_location);
                printk("%s", blank);

                vt100_move_cursor(1, print_location);
                printk("> [TASK] Has acquired lock and exited.\n");

#ifdef SPIN_LOCK
                spin_lock_release(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                do_mutex_lock_release(&mutex_lock);
#endif
				en_int();
               // do_scheduler();
        }
}

void lock_task2(void)
{
        //int print_location = 2;
	    int print_location = 4;
        while (1)
        {
			dis_int();
                int i;
                if (!is_init)
                {

#ifdef SPIN_LOCK
                        spin_lock_init(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                        do_mutex_lock_init(&mutex_lock);
#endif
                        is_init = TRUE;
                }

                vt100_move_cursor(1, print_location);
                printk("%s", blank);

                vt100_move_cursor(1, print_location);
                printk("> [TASK] Applying for a lock.\n");
               en_int();
              //  do_scheduler();
			   dis_int();
#ifdef SPIN_LOCK
                spin_lock_acquire(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                do_mutex_lock_acquire(&mutex_lock);
#endif
				en_int();

                for (i = 0; i < 20; i++)
                {
					dis_int();
                        vt100_move_cursor(1, print_location);
                        printk("> [TASK] Has acquired lock and running.(%d)\n", i);
						en_int();
                        //do_scheduler();
                }
				dis_int();
                vt100_move_cursor(1, print_location);
                printk("%s", blank);

                vt100_move_cursor(1, print_location);
                printk("> [TASK] Has acquired lock and exited.\n");

#ifdef SPIN_LOCK
                spin_lock_release(&spin_lock);
#endif

#ifdef MUTEX_LOCK
                do_mutex_lock_release(&mutex_lock);
#endif
				en_int();
              //  do_scheduler();
        }
}