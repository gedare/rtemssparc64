
# This code needs header before distribution~

#include <rtems/asm.h>
#include <rtems/score/cpu.h>
#include "stack.h"

#define PSTATE_IE   	2
#define PSTATE_PRIV	4
#define PSTATE_AM   	8


.register %g2, #scratch
.register %g3, #scratch
.register %g6, #scratch
.register %g7, #scratch

.section BOOTSTRAP, "ax"

PUBLIC(start)
  .global start
  SYM(start):
    start:
    b 1f
    nop

    /*
     * This header forces SILO to load the image at 0x4000.
     * More precisely, SILO will think this is an old version of Linux.
     */
    .ascii "HdrS"
    .word 0
    .half 0
    .half 0
    .half 0
    .half 0
    .global silo_ramdisk_image
    silo_ramdisk_image:
    .word 0
    .global silo_ramdisk_size
    silo_ramdisk_size:
    .word 0

    .align 8
    1:	
    /*
     * Disable interrupts and disable address masking.
     */
    wrpr %g0, PSTATE_PRIV, %pstate

    wrpr %g0, SPARC_NUMBER_OF_REGISTER_WINDOWS - 2, %cansave	! set maximum saveable windows
    wrpr %g0, 0, %canrestore		! get rid of windows we will never need again
    wrpr %g0, 0, %otherwin			! make sure the window state is consistent
    wrpr %g0, SPARC_NUMBER_OF_REGISTER_WINDOWS - 1, %cleanwin	! prevent needless clean_window traps for kernel

    mov %g0, %g4

!	set SYM(RAM_END), %sp
!	add %sp, -STACK_BIAS, %sp

	set SYM(syscall), %o1
	set param_space, %o2
	call SYM(_CPU_ISR_install_raw_handler)
	mov 0x100, %o0

	set ofw_cif, %l0
 
	call ofw_init		! initialize OpenFirmware
	stx %o4, [%l0]
	
    /* set trap table */
!    sethi %hi(trap_table), %g1
!    wrpr %g1, %lo(trap_table), %tba


!	call SYM(bootstrap)
!	nop

  /* Do we need to do anything with the MMU? */

  /* Don't need to copy initialized data to RAM--link puts all in RAM already */

  /* Clear the bss */
	set 	SYM(bss_start), %g2		! g2 = start of bss
	set 	SYM(_end), %g3		! g3 = end of bss
zerobss:
	stx	%g0, [%g2]
	add	%g2, 8, %g2
	cmp	%g2, %g3
	bleu,a	zerobss
	nop

	mov	%g0, %o0		! clear command line passed to main
	call SYM(boot_card)
	sub	%sp, 0x60, %sp		! make room on stack (necessary?)
	nop

  .align CPU_ALIGNMENT
  param_space:
  	.space 8

  .align CPU_STACK_ALIGNMENT
  initial_stack:
  .space INITIAL_STACK_SIZE
  initial_stack_top:
  .space STACK_WINDOW_SAVE_AREA_SIZE


    .global _fini
    _fini:
    nop
    nop

    .global _init
    _init:
    nop
    nop
