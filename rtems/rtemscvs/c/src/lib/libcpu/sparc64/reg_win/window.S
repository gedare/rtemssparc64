/*
 *  window.s
 *
 *  This file contains the register window management routines for the 
 *  SPARC architecture.  Trap handlers for the following capabilities
 *  are included in this file:
 *
 *    + Window Overflow (spill)
 *    + Window Underflow (fill)
 *    + Window Clean (clean)
 */

#include <rtems/asm.h>
#include <rtems/score/cpu.h>

/* is this BSP independent? */
#define STACK_BIAS 2047

        .seg    "text"

/*
 * Used for register overflow to spill a register window
 */
PUBLIC(win_spill_code)
SYM(win_spill_code):
	stx %l0, [%sp + STACK_BIAS + CPU_STACK_FRAME_L0_OFFSET]	
	stx %l1, [%sp + STACK_BIAS + CPU_STACK_FRAME_L1_OFFSET]
	stx %l2, [%sp + STACK_BIAS + CPU_STACK_FRAME_L2_OFFSET]
	stx %l3, [%sp + STACK_BIAS + CPU_STACK_FRAME_L3_OFFSET]
/* 5 */	stx %l4, [%sp + STACK_BIAS + CPU_STACK_FRAME_L4_OFFSET]
	stx %l5, [%sp + STACK_BIAS + CPU_STACK_FRAME_L5_OFFSET]
	stx %l6, [%sp + STACK_BIAS + CPU_STACK_FRAME_L6_OFFSET]
	stx %l7, [%sp + STACK_BIAS + CPU_STACK_FRAME_L7_OFFSET]
	stx %i0, [%sp + STACK_BIAS + CPU_STACK_FRAME_I0_OFFSET]
/* 10 */stx %i1, [%sp + STACK_BIAS + CPU_STACK_FRAME_I1_OFFSET]
	stx %i2, [%sp + STACK_BIAS + CPU_STACK_FRAME_I2_OFFSET]
	stx %i3, [%sp + STACK_BIAS + CPU_STACK_FRAME_I3_OFFSET]
	stx %i4, [%sp + STACK_BIAS + CPU_STACK_FRAME_I4_OFFSET]
	stx %i5, [%sp + STACK_BIAS + CPU_STACK_FRAME_I5_OFFSET]
/* 15 */stx %i6, [%sp + STACK_BIAS + CPU_STACK_FRAME_I6_FP_OFFSET]
	stx %i7, [%sp + STACK_BIAS + CPU_STACK_FRAME_I7_OFFSET]
	saved
	retry


/*
 * Used to service a trap on restore
 */
PUBLIC(win_fill_code)
SYM(win_fill_code):
	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_L0_OFFSET], %l0
	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_L1_OFFSET], %l1
	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_L2_OFFSET], %l2
	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_L3_OFFSET], %l3
/* 5 */	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_L4_OFFSET], %l4
	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_L5_OFFSET], %l5
	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_L6_OFFSET], %l6
	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_L7_OFFSET], %l7
	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_I0_OFFSET], %i0
/* 10 */ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_I1_OFFSET], %i1
	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_I2_OFFSET], %i2
	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_I3_OFFSET], %i3
	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_I4_OFFSET], %i4
	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_I5_OFFSET], %i5
/* 15 */ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_I6_FP_OFFSET], %i6
	ldx [%sp + STACK_BIAS + CPU_STACK_FRAME_I7_OFFSET], %i7
	restored
	retry

/*
 * Used to clean (wipe) a window
 */
PUBLIC(win_clean_code)
SYM(win_clean_code):
	rdpr %cleanwin, %l0
	add %l0, 1, %l0
	wrpr %l0, 0, %cleanwin
	mov %r0, %l0
/* 5 */	mov %r0, %l1
	mov %r0, %l2
	mov %r0, %l3
	mov %r0, %l4
	mov %r0, %l5
/* 10 */mov %r0, %l6
	mov %r0, %l7
	mov %r0, %o0
	mov %r0, %o1
	mov %r0, %o2
/* 15 */mov %r0, %o3
	mov %r0, %o4
	mov %r0, %o5
	mov %r0, %o6
	mov %r0, %o7
/* 20 */retry

/* end of file */
