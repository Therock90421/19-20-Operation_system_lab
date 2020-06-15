/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *        Process scheduling related content, such as: scheduler, process blocking, 
 *                 process wakeup, process creation, process kill, etc.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE. 
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#ifndef INCLUDE_SCHEDULER_H_
#define INCLUDE_SCHEDULER_H_

#include "type.h"
#include "queue.h"

#define NUM_MAX_TASK 16

/* used to save register infomation */
typedef struct regs_context
{
    /* Saved main processor registers.*/
    /* 32 * 4B = 128B */
    uint32_t regs[32];

    /* Saved special registers. */
    /* 7 * 4B = 28B */
    uint32_t cp0_status;
    uint32_t hi;
    uint32_t lo;
    uint32_t cp0_badvaddr;
    uint32_t cp0_cause;
    uint32_t cp0_epc;
    uint32_t pc;

} regs_context_t; /* 128 + 28 = 156B */

typedef enum {
    TASK_BLOCKED,
    TASK_RUNNING,
    TASK_READY,
    TASK_EXITED,
} task_status_t;

typedef enum {
    KERNEL_PROCESS,
    KERNEL_THREAD,
    USER_PROCESS,
    USER_THREAD,
} task_type_t;

/* Process Control Block */
typedef struct pcb
{
    /* register context */
    regs_context_t kernel_context;//内核上下文
    regs_context_t user_context; //用户上下文
                                 //上下文包括32个寄存器和status，hi，lo，badvaddr，cause，epc和pc
    uint32_t kernel_stack_top;   //内核栈基地址
    uint32_t user_stack_top;     //用户栈基地址

    /* previous, next pointer */
    void *prev;                  //在队列里的链表链接信息
    void *next;

    /* process id */
    pid_t pid;                   //进程号

    /* kernel/user thread/process */
    task_type_t type;            //任务类型 内核进程，内核线程，用户进程，用户线程

    /* BLOCK | READY | RUNNING */
    task_status_t status;        //任务状态 阻塞，就绪，进行，退出

    /* cursor position */
    int cursor_x;            //光标的位置
    int cursor_y;

	/* Priority */
    int priority;
    int task_priority;

	/* sleep time*/
    uint32_t begin_time;
    uint32_t sleep_time;

} pcb_t;

/* task information, used to init PCB */
typedef struct task_info
{
    uint32_t entry_point;
    task_type_t type;
} task_info_t;

/* ready queue to run */
extern queue_t ready_queue;

/* block queue to wait */
extern queue_t block_queue[NUM_MAX_TASK];
extern int lock_queue_id;

/* sleep queue to wait */
extern queue_t sleep_queue;

/* current running task PCB */
extern pcb_t *current_running;
extern pid_t process_id;

extern pcb_t pcb[NUM_MAX_TASK];
extern uint32_t initial_cp0_status;
extern uint32_t exception_handler[32];

void do_scheduler(void);
void do_sleep(uint32_t);

void do_block(queue_t *);
void do_unblock_one(queue_t *);
void do_unblock_all(queue_t *);

#endif