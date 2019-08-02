
/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2013.  All rights reserved. */

#include <arm.h>

.text
        .align 4
        .globl  ctxsw

/**
 * @fn void ctxsw(&oldregs, &newregs)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought; as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldregs address of outgoing register save area
 * @param  &newregs address of incoming register save area
 * @return special case -- see above
 */

ctxsw:

    .func ctxsw

	/* Save callee-save ("non-volatile") registers */
/*
	mov	ip, r1
	str	r0, [r0, #PREG_R0*4]
	str	r1, [r0, #PREG_R1*4]
	str	r2, [r0, #PREG_R2*4]
	str	r3, [r0, #PREG_R3*4]
	str	r4, [r0, #PREG_R4*4]
	str	r5, [r0, #PREG_R5*4]
	str	r6, [r0, #PREG_R6*4]
	str	r7, [r0, #PREG_R7*4]
	str	r8, [r0, #PREG_R8*4]
	str	r9, [r0, #PREG_R9*4]
	str	r10, [r0, #PREG_R10*4]
	str	r11, [r0, #PREG_R11*4]
	str	sp, [r0, #PREG_SP*4]

	str	lr, [r0, #PREG_LR*4]
	str	lr, [r0, #PREG_PC*4]
*/

	/* Restore callee-save ("non-volatile") registers */
/*
	ldr	r0, [ip, #PREG_R0*4]
	ldr	r1, [ip, #PREG_R1*4] 
	ldr	r2, [ip, #PREG_R2*4]
	ldr	r3, [ip, #PREG_R3*4]
	ldr	r4, [ip, #PREG_R4*4]
	ldr	r5, [ip, #PREG_R5*4]
	ldr	r6, [ip, #PREG_R6*4]
	ldr	r7, [ip, #PREG_R7*4]
	ldr	r8, [ip, #PREG_R8*4]
	ldr	r9, [ip, #PREG_R9*4]
	ldr	r10, [ip, #PREG_R10*4]
	ldr	r11, [ip, #PREG_R11*4]
	ldr	sp, [ip, #PREG_SP*4]
	
	ldr	lr, [ip, #PREG_LR*4]
	ldr	pc, [ip, #PREG_PC*4]
*/


   /* Store registers in respective places in stack.  Link register gets saved into location for stack pointer*/
    /* so when process switches back, program will continue execution in correct location*/


    mrs	   r12, cpsr
    push   {r0-r14}

    str    sp, [r0]
    ldr    sp, [r1]

    /* Load registers from respective places in stack. */
    /* Program counter must be loaded last, since program will jump.*/
    /* In this case, returning is same as loading the link register into the program counter*/

   
    pop	   {r0-r12}
    msr    cpsr_c, r12


    /* Load link register into program counter*/
    /* Not returning back to function we came from, but going to new function in different process, */
    /* since we changed value of link register*/
 
    pop    {lr, pc}

    .endfunc
