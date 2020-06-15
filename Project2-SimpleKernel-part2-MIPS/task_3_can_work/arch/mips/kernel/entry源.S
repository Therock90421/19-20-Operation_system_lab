#include "asm.h"

.equ    KERNEL, 0
.equ    USER,   156

.macro STI     //开中断
    mfc0    k0, CP0_STATUS
	nop
	ori     k0, 0x1        //最低位  置1
	mtc0    k0, CP0_STATUS
	nop
.endm

.macro CLI     //关中断
    mfc0    k0, CP0_STATUS
	nop
	li      k1, 0xfffffffe  
	and     k0, k0, k1        //最低位  置0
	mtc0    k0, CP0_STATUS
	nop
.endm

LEAF(asm_start)
    mtc0    zero, CP0_STATUS
    mtc0    zero, CP0_WATCHLO
    mtc0    zero, CP0_WATCHHI

    mfc0    t0, CP0_CONFIG
    and     t0, ~0x7
    ori     t0, 0x2
    mtc0    t0, CP0_CONFIG
    
    jr  ra
END(asm_start)

.macro SAVE_CONTEXT offset  //offset为存储偏移量
    // TODO save context
	lw      k0, current_running
	addi	k0, k0,  \offset             //宏调用,不然会被当作标号
    sw      AT, OFFSET_REG1(k0)
    sw      v0, OFFSET_REG2(k0)
    sw      v1, OFFSET_REG3(k0)
	sw      a0, OFFSET_REG4(k0)
	sw      a1, OFFSET_REG5(k0)
	sw      a2, OFFSET_REG6(k0)
	sw      a3, OFFSET_REG7(k0)
	sw      t0, OFFSET_REG8(k0)
	sw      t1, OFFSET_REG9(k0)
	sw      t2, OFFSET_REG10(k0)
	sw      t3, OFFSET_REG11(k0)
	sw      t4, OFFSET_REG12(k0)
	sw      t5, OFFSET_REG13(k0)
	sw      t6, OFFSET_REG14(k0)
	sw      t7, OFFSET_REG15(k0)
	sw      s0, OFFSET_REG16(k0)
	sw      s1, OFFSET_REG17(k0)
	sw      s2, OFFSET_REG18(k0)
	sw      s3, OFFSET_REG19(k0)
	sw      s4, OFFSET_REG20(k0)
	sw      s5, OFFSET_REG21(k0)
	sw      s6, OFFSET_REG22(k0)
	sw      s7, OFFSET_REG23(k0)
	sw      t8, OFFSET_REG24(k0)
	sw      t9, OFFSET_REG25(k0)

	/*$26(k0) $27(k1) will not be saved*/
	sw      gp, OFFSET_REG28(k0)
	sw      sp, OFFSET_REG29(k0)
    sw      fp, OFFSET_REG30(k0)
	sw      ra, OFFSET_REG31(k0)

	mfc0    k1, CP0_STATUS
	nop           /*mf act slow*/
	sw      k1,OFFSET_STATUS(k0)

	mfhi	k1
	nop
	sw		k1, OFFSET_HI(k0)

	mflo	k1
	nop
	sw		k1, OFFSET_LO(k0)

	mfc0	k1, CP0_BADVADDR
	nop
	sw		k1, OFFSET_BADVADDR(k0)

	mfc0	k1, CP0_CAUSE
	nop
	sw		k1, OFFSET_CAUSE(k0)

	mfc0	k1, CP0_EPC
	nop
	sw		k1, OFFSET_EPC(k0)
.endm

.macro RESTORE_CONTEXT offset
    // TODO restore context
	lw      k0, current_running
	addi	k0, k0,  \offset
    lw      AT, OFFSET_REG1(k0)
    lw      v0, OFFSET_REG2(k0)
    lw      v1, OFFSET_REG3(k0)
	lw      a0, OFFSET_REG4(k0)
	lw      a1, OFFSET_REG5(k0)
	lw      a2, OFFSET_REG6(k0)
	lw      a3, OFFSET_REG7(k0)
	lw      t0, OFFSET_REG8(k0)
	lw      t1, OFFSET_REG9(k0)
	lw      t2, OFFSET_REG10(k0)
	lw      t3, OFFSET_REG11(k0)
	lw      t4, OFFSET_REG12(k0)
	lw      t5, OFFSET_REG13(k0)
	lw      t6, OFFSET_REG14(k0)
	lw      t7, OFFSET_REG15(k0)
	lw      s0, OFFSET_REG16(k0)
	lw      s1, OFFSET_REG17(k0)
	lw      s2, OFFSET_REG18(k0)
	lw      s3, OFFSET_REG19(k0)
	lw      s4, OFFSET_REG20(k0)
	lw      s5, OFFSET_REG21(k0)
	lw      s6, OFFSET_REG22(k0)
	lw      s7, OFFSET_REG23(k0)
	lw      t8, OFFSET_REG24(k0)
	lw      t9, OFFSET_REG25(k0)
	lw      gp, OFFSET_REG28(k0)
	lw      sp, OFFSET_REG29(k0)
    lw      fp, OFFSET_REG30(k0)
	lw      ra, OFFSET_REG31(k0)

	lw		k1, OFFSET_STATUS(k0)
	mtc0    k1, CP0_STATUS
	nop

	lw		k1, OFFSET_HI(k0)
	mthi	k1
	nop

	lw		k1, OFFSET_LO(k0)
	mtlo	k1
	nop

	lw		k1, OFFSET_BADVADDR(k0)
	mtc0	k1, CP0_BADVADDR
	nop

	lw		k1, OFFSET_CAUSE(k0)
	mtc0	k1, CP0_CAUSE
	nop

	lw		k1, OFFSET_EPC(k0)
	mtc0	k1, CP0_EPC
	nop
.endm

NESTED(do_scheduler, 0, ra)
    SAVE_CONTEXT(KERNEL)
    jal     scheduler
    RESTORE_CONTEXT(KERNEL)
    jr      ra
END(do_scheduler)

.global exception_handler_begin
.global exception_handler_end

NESTED(exception_handler_entry, 0, sp)   
exception_handler_begin:
    // TODO close interrupt
    // jmp exception_handler[i] which decided by CP0_CAUSE
    // Leve2 exception Handler.
	CLI    //关中断
	SAVE_CONTEXT(USER)    //保存用户现场
	mfc0    k0, CP0_CAUSE
	nop
	andi    k0, k0, CAUSE_EXCCODE  //从第2到6位取出exccode
	la      k1, exception_handler  //把exception_handler标号的地址取过来
	add     k0, k0, k1             //选取对应程序的标号(0+exccode)
	lw      k0, 0(k0)              //把标号对应的处理程序入口地址拿过来
	jr      k0                     //跳转到例外处理程序
exception_handler_end:
END(exception_handler_entry)

NESTED(handle_int, 0, sp)
    // interrupt handler
    // Leve3 exception Handler.
	mfc0     a0, CP0_STATUS
	nop
	mfc0     a1, CP0_CAUSE
	nop
	jal      interrupt_helper    //传参给第三级处理程序

	j        exception_handler_work_for_exit
END(handle_int)

NESTED(handle_syscall, 0, sp)
    // system call handler
	//先不写
END(handle_syscall)

NESTED(handle_other, 0, sp)
    // other exception handler
	//啥也不用做
	j         exception_handler_work_for_exit
END(handle_other)


LEAF(exception_handler_work_for_exit)
	
	RESTORE_CONTEXT(USER)   //恢复用户现场
	STI                     //开中断
	eret
END(exception_handler_work_for_exit)


LEAF(GET_CP0_STATUS)      //取cp0_status的值
      mfc0     v0, CP0_STATUS
	  nop
	  jr       ra
END(GET_CP0_STATUS)

LEAF(GET_CP0_COUNT)       //取cp0_count的值
      mfc0     v0, CP0_COUNT
	  nop
	  jr       ra
END(GET_CP0_COUNT)

LEAF(GET_CP0_COMPARE)     //取cp0_compare的值
      mfc0     v0, CP0_COMPARE
	  nop
	  jr       ra
END(GET_CP0_COMPARE)


LEAF(SET_CP0_STATUS)      //修改cp0_status的值
      mtc0     a0, CP0_STATUS
	  nop
	  jr       ra
END(SET_CP0_STATUS)

LEAF(SET_CP0_COUNT)       //修改cp0_count的值
      mtc0     a0, CP0_COUNT
	  nop
	  jr       ra
END(SET_CP0_COUNT)

LEAF(SET_CP0_COMPARE)     //修改cp0_compare的值
      mtc0     a0, CP0_COMPARE
	  nop
	  jr       ra
END(SET_CP0_COMPARE)