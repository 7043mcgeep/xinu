/**
 * @file setupStack.c
 */
/* Embedded Xinu, Copyright (C) 2013.  All rights reserved. */

#include <platform.h>
#include <arm.h>

/* Length of ARM context record in words (includes r0-r11, cpsr, lr, pc).  */
#define CONTEXT_WORDS 15

/* The standard ARMv8 calling convention passes first eight arguments in x0-x7; the
 * rest spill onto the stack.  */
#define MAX_REG_ARGS 4

/**
 * @ingroup bcm2837
 *
 * Set up the context record and arguments on the stack for a new thread
 * (ARM version)  
 * @param stackaddr	Stack address
 * @param procaddr	Process address
 * @return Top of the stack
 */
void *setupStack(void *stackaddr, void *procaddr,
                 void *retaddr, uint nargs, va_list ap)
{
    uint spilled_nargs;
    uint reg_nargs;
    uint i;
    uint *saddr = stackaddr;

    /* Determine if any arguments will spill onto the stack (outside the context
     * record).  If so, reserve space for them.  */
    if (nargs > MAX_REG_ARGS) {
        spilled_nargs = nargs - MAX_REG_ARGS;
        reg_nargs = MAX_REG_ARGS;
        saddr -= spilled_nargs;
    } else {
        spilled_nargs = 0;
        reg_nargs = nargs;
    }

    /* Possibly skip a word to ensure the stack is aligned on a **8-byte** boundary
     * after the new thread pops off the context record.  */
    if ((uint)saddr & 0x4)
    {
        --saddr;
    }


    /* Construct the context record for the new thread.  */
    saddr -= CONTEXT_WORDS;

    saddr[0] = 0;

    /* Arguments passed in registers (part of context record)  */
    for (i = 0; i < reg_nargs; i++)
    {
        saddr[i] = va_arg(ap, ulong);
    }

    for (; i < CONTEXT_WORDS - 3; i++)
    {
        saddr[i] = 0;
    }
    

    /* Control bits of program status register
     * (SYS mode, IRQs initially enabled) */
    saddr[CONTEXT_WORDS - 3] = ARM_MODE_SYS | ARM_F_BIT;

    /* return address  */
    saddr[CONTEXT_WORDS - 2] = (ulong)retaddr;

    /* program counter  */
    saddr[CONTEXT_WORDS - 1] = (ulong)procaddr;

    /* Arguments spilled onto stack (not part of context record)  */
    for (i = 0; i < spilled_nargs; i++)
    {
        saddr[CONTEXT_WORDS + i] = va_arg(ap, ulong);
    }

    /* Return "top" of stack (lowest address).  */

    return saddr;
}
