/*
 *
 *  COPYRIGHT (c) 2010, Gedare Bloom
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <rtems/asm.h>
#include <rtems/score/cpu.h>

.register %g2, #scratch
.register %g3, #scratch

  PUBLIC(_start)
SYM (_start):
	PUBLIC(start)
SYM (start):
  ! install the stack pointer. see linkcmds
  set SYM(StackStart), %sp
  add %sp, -STACK_BIAS, %sp

	! zero out bss
	setx	SYM(bss_start), %l0, %g2	! g2 = start of bss
	setx	SYM(_end), %l0, %g3		    ! g3 = end of bss
zerobss:
	stx	%g0, [%g2]
	add	%g2, 8, %g2
	cmp	%g2, %g3
	bleu,a	zerobss
	nop

  ! Set interrupt handlers
	setx SYM(syscall), %l0, %o1
	setx param_space, %l0, %o2
	call SYM(_CPU_ISR_install_raw_handler)
	mov 0x100, %o0

	! call bootcard
	mov	%g0, %o0		! clear command line passed to main
	call SYM(boot_card)
	sub	%sp, 0x60, %sp		! make room on stack (necessary?)

  /* execution returns here when RTEMS terminates */
_exit:
	    nop
	    b SYM(_exit)
! TODO: use a syscall to exit gracefully.
!  .long	SYM(_sys_exit)


  .align CPU_ALIGNMENT
  param_space:
    .space 8

    .global _fini
    _fini:
    nop
    nop

    .global _init
    _init:
    nop
    nop


