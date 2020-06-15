#include "asm.h"

LEAF(invoke_syscall)
    // TODO syscall entry
	add     v0, a0, zero
	add     a0, a1, zero
	add     a1, a2, zero
	add     a2, a3, zero

	syscall    //自动跳转到例外处理入口
	jr      ra
END(invoke_syscall)

//调用syscall时，v0是系统调用号
//a0,a1,a2是参数