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
  sethi  %hh(_handler), %l3 ; \
  or     %l3, %hm(_handler), %l3 ; \
  sllx   %l3, 32, %l3 ; \
  sethi  %hi(_handler), %l4 ; \
  or     %l4, %l3, %l4 ; \
  jmp   %l4+%lo(_handler); \
  mov   _vector, %l3 ; 

/*
 *  Used for the reset trap to avoid a supervisor instruction
 */
 
#define RTRAP(_vector, _handler)  \
  mov   %g0, %l0 ; \
  sethi %hi(_handler), %l4 ; \
  jmp   %l4+%lo(_handler); \
  mov   _vector, %l3

/* 
 * These are defined in cpu.h, but for offset in context_control
 */
#define L0_OFFSET_FROM_SP       0
#define L1_OFFSET_FROM_SP       8
#define L2_OFFSET_FROM_SP       16
#define L3_OFFSET_FROM_SP       24
#define L4_OFFSET_FROM_SP       32
#define L5_OFFSET_FROM_SP       40
#define L6_OFFSET_FROM_SP       48
#define L7_OFFSET_FROM_SP       56
#define I0_OFFSET_FROM_SP       64
#define I1_OFFSET_FROM_SP       72
#define I2_OFFSET_FROM_SP       80
#define I3_OFFSET_FROM_SP       88
#define I4_OFFSET_FROM_SP       96
#define I5_OFFSET_FROM_SP       104
#define I6_OFFSET_FROM_SP       112
#define I7_OFFSET_FROM_SP       120

/*
 * Used for register overflow to spill a register window
 */
.macro SPILL_WINDOW_HANDLER
	stx %l0, [%sp + STACK_BIAS + L0_OFFSET_FROM_SP]	
	stx %l1, [%sp + STACK_BIAS + L1_OFFSET_FROM_SP]
	stx %l2, [%sp + STACK_BIAS + L2_OFFSET_FROM_SP]
	stx %l3, [%sp + STACK_BIAS + L3_OFFSET_FROM_SP]
	stx %l4, [%sp + STACK_BIAS + L4_OFFSET_FROM_SP]
	stx %l5, [%sp + STACK_BIAS + L5_OFFSET_FROM_SP]
	stx %l6, [%sp + STACK_BIAS + L6_OFFSET_FROM_SP]
	stx %l7, [%sp + STACK_BIAS + L7_OFFSET_FROM_SP]
	stx %i0, [%sp + STACK_BIAS + I0_OFFSET_FROM_SP]
	stx %i1, [%sp + STACK_BIAS + I1_OFFSET_FROM_SP]
	stx %i2, [%sp + STACK_BIAS + I2_OFFSET_FROM_SP]
	stx %i3, [%sp + STACK_BIAS + I3_OFFSET_FROM_SP]
	stx %i4, [%sp + STACK_BIAS + I4_OFFSET_FROM_SP]
	stx %i5, [%sp + STACK_BIAS + I5_OFFSET_FROM_SP]
	stx %i6, [%sp + STACK_BIAS + I6_OFFSET_FROM_SP]
	stx %i7, [%sp + STACK_BIAS + I7_OFFSET_FROM_SP]
	saved
	retry
.endm

/*
 * Used to service a trap on restore
 */
.macro FILL_WINDOW_HANDLER
	ldx [%sp + STACK_BIAS + L0_OFFSET_FROM_SP], %l0
	ldx [%sp + STACK_BIAS + L1_OFFSET_FROM_SP], %l1
	ldx [%sp + STACK_BIAS + L2_OFFSET_FROM_SP], %l2
	ldx [%sp + STACK_BIAS + L3_OFFSET_FROM_SP], %l3
	ldx [%sp + STACK_BIAS + L4_OFFSET_FROM_SP], %l4
	ldx [%sp + STACK_BIAS + L5_OFFSET_FROM_SP], %l5
	ldx [%sp + STACK_BIAS + L6_OFFSET_FROM_SP], %l6
	ldx [%sp + STACK_BIAS + L7_OFFSET_FROM_SP], %l7
	ldx [%sp + STACK_BIAS + I0_OFFSET_FROM_SP], %i0
	ldx [%sp + STACK_BIAS + I1_OFFSET_FROM_SP], %i1
	ldx [%sp + STACK_BIAS + I2_OFFSET_FROM_SP], %i2
	ldx [%sp + STACK_BIAS + I3_OFFSET_FROM_SP], %i3
	ldx [%sp + STACK_BIAS + I4_OFFSET_FROM_SP], %i4
	ldx [%sp + STACK_BIAS + I5_OFFSET_FROM_SP], %i5
	ldx [%sp + STACK_BIAS + I6_OFFSET_FROM_SP], %i6
	ldx [%sp + STACK_BIAS + I7_OFFSET_FROM_SP], %i7
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
