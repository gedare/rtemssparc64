/**
 * @file rtems/traps.h
 *
 * This include file defines macros for loading the sparc64 trap table.
 */


#ifndef _RTEMS_TRAPS_H
#define _RTEMS_TRAPS_H

#include <rtems/asm.h>


/*
 *  Entry for traps which jump to a programmer-specified trap handler.
 */

#define TRAP(_vector, _handler)  \
  rdpr   %pstate, %l0 ; \
  sethi %hi(_handler), %l4 ; \
  jmp   %l4+%lo(_handler); \
  mov   _vector, %l3 ; \
  nop ; \
  nop ; \
  nop ; \
  nop

/*
 *  Used for the reset trap to avoid a supervisor instruction
 */
 
#define RTRAP(_vector, _handler)  \
  mov   %g0, %l0 ; \
  sethi %hi(_handler), %l4 ; \
  jmp   %l4+%lo(_handler); \
  mov   _vector, %l3

#define L0_OFFSET       0
#define L1_OFFSET       8
#define L2_OFFSET       16
#define L3_OFFSET       24
#define L4_OFFSET       32
#define L5_OFFSET       40
#define L6_OFFSET       48
#define L7_OFFSET       56
#define I0_OFFSET       64
#define I1_OFFSET       72
#define I2_OFFSET       80
#define I3_OFFSET       88
#define I4_OFFSET       96
#define I5_OFFSET       104
#define I6_OFFSET       112
#define I7_OFFSET       120

/*
 * Used for register overflow to spill a register window
 */
.macro SPILL_WINDOW_HANDLER
	stx %l0, [%sp + STACK_BIAS + L0_OFFSET]	
	stx %l1, [%sp + STACK_BIAS + L1_OFFSET]
	stx %l2, [%sp + STACK_BIAS + L2_OFFSET]
	stx %l3, [%sp + STACK_BIAS + L3_OFFSET]
	stx %l4, [%sp + STACK_BIAS + L4_OFFSET]
	stx %l5, [%sp + STACK_BIAS + L5_OFFSET]
	stx %l6, [%sp + STACK_BIAS + L6_OFFSET]
	stx %l7, [%sp + STACK_BIAS + L7_OFFSET]
	stx %i0, [%sp + STACK_BIAS + I0_OFFSET]
	stx %i1, [%sp + STACK_BIAS + I1_OFFSET]
	stx %i2, [%sp + STACK_BIAS + I2_OFFSET]
	stx %i3, [%sp + STACK_BIAS + I3_OFFSET]
	stx %i4, [%sp + STACK_BIAS + I4_OFFSET]
	stx %i5, [%sp + STACK_BIAS + I5_OFFSET]
	stx %i6, [%sp + STACK_BIAS + I6_OFFSET]
	stx %i7, [%sp + STACK_BIAS + I7_OFFSET]
	saved
	retry
.endm

/*
 * Used to service a trap on restore
 */
.macro FILL_WINDOW_HANDLER
	ldx [%sp + STACK_BIAS + L0_OFFSET], %l0
	ldx [%sp + STACK_BIAS + L1_OFFSET], %l1
	ldx [%sp + STACK_BIAS + L2_OFFSET], %l2
	ldx [%sp + STACK_BIAS + L3_OFFSET], %l3
	ldx [%sp + STACK_BIAS + L4_OFFSET], %l4
	ldx [%sp + STACK_BIAS + L5_OFFSET], %l5
	ldx [%sp + STACK_BIAS + L6_OFFSET], %l6
	ldx [%sp + STACK_BIAS + L7_OFFSET], %l7
	ldx [%sp + STACK_BIAS + I0_OFFSET], %i0
	ldx [%sp + STACK_BIAS + I1_OFFSET], %i1
	ldx [%sp + STACK_BIAS + I2_OFFSET], %i2
	ldx [%sp + STACK_BIAS + I3_OFFSET], %i3
	ldx [%sp + STACK_BIAS + I4_OFFSET], %i4
	ldx [%sp + STACK_BIAS + I5_OFFSET], %i5
	ldx [%sp + STACK_BIAS + I6_OFFSET], %i6
	ldx [%sp + STACK_BIAS + I7_OFFSET], %i7
	restored
	retry
.endm

/*
 * Used to clean (wipe) a window
 */
.macro CLEAN_WINDOW_HANDLER
	rdpr %cleanwin, %l0
	add %l0, 1, %l0
	wrpr %l0, 0, %cleanwin
	mov %r0, %l0
	mov %r0, %l1
	mov %r0, %l2
	mov %r0, %l3
	mov %r0, %l4
	mov %r0, %l5
	mov %r0, %l6
	mov %r0, %l7
	mov %r0, %o0
	mov %r0, %o1
	mov %r0, %o2
	mov %r0, %o3
	mov %r0, %o4
	mov %r0, %o5
	mov %r0, %o6
	mov %r0, %o7
	retry
.endm


#endif
