/*
 *  systrap.S
 *
 *  This file contains emulated system calls using software trap 0.
 *  The following calls are supported:
 *
 *    + SYS_exit	(halt)
 *    + SYS_irqdis	(disable interrupts)
 *    + SYS_irqset	(set interrupt level)
 * 
 *  COPYRIGHT:
 *
 *  COPYRIGHT (c) 1995. European Space Agency.
 *
 *  This terms of the RTEMS license apply to this file.
 *
 */

#include <rtems/asm.h>
#include "syscall.h"

        .seg    "text"
        /*
         *  system call
         *
         *  On entry:
         *
         *    l0 = tstate (from trap table)
         *    XXX  l1 = pc  
         *    XXX  l2 = npc  
         *    g1 = system call id
         */

        PUBLIC(syscall)

SYM(syscall):

	subcc   %g1, 2, %g0		! syscall 2, disable interrupts
	bne 	3f
	rdpr    %pil, %l4
	and	%l0, SPARC_TSTATE_IE_MASK, %i0
	or	%i0, %l4, %i0		! return TSTATE_IE | PIL
	wrpr   	%g0, 0xf, %pil		! set PIL to 15
	andn    %l0, SPARC_TSTATE_IE_MASK, %l4
	wrpr    %g0, %l4, %tstate	! disable interrupts in trap state
	ba,a	9f
3:
	subcc   %g1, 3, %g0		! syscall 3, enable interrupts
	bne 	1f
	and     %i0, 0xf, %l3
	wrpr	%g0, %l3, %pil		! restore PIL
	and	%i0, SPARC_TSTATE_IE_MASK, %l4
	or	%l0, %l4, %l4		
	wrpr    %g0, %l4, %tstate	! restore IE to stored value
	ba,a	9f

1:					
	ta	0			! halt 
9:				! leave
	mov	0, %g1			! clear %g1 
	DONE

        PUBLIC(sparc_disable_interrupts)

SYM(sparc_disable_interrupts):

	mov	SYS_irqdis, %g1
	ta	0
	retl
	nop

        PUBLIC(sparc_enable_interrupts)

SYM(sparc_enable_interrupts):

	mov	SYS_irqen, %g1
	ta	0
	retl
	nop

/* end of file */
