/*  cpu_asm.S
 *
 *  This file contains the basic algorithms for all assembly code used
 *  in the Blackfin port of RTEMS.  These algorithms must be implemented
 *  in assembly language
 *
 *  Copyright (c) 2008 Kallisti Labs, Los Gatos, CA, USA
 *             written by Allan Hessenflow <allanh@kallisti.com>
 *
 *  Based on earlier version:
 *
 *  Copyright (c) 2006 by Atos Automacao Industrial Ltda.
 *             written by Alain Schaefer <alain.schaefer@easc.ch>
 *                     and Antonio Giovanini <antonio@atos.com.br>
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: cpu_asm.S,v 1.9 2010/04/20 13:27:48 joel Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/asm.h>
#include <rtems/score/cpu_asm.h>
#include <rtems/score/bfin.h>
#include <rtems/bfin/bfin.h>

#define LO(con32) ((con32) & 0xFFFF)
#define HI(con32) (((con32) >> 16) & 0xFFFF)


#if 0
/* some debug routines */
	.globl __CPU_write_char;
__CPU_write_char:
	p0.h = 0xffc0;
	p0.l = 0x0400;
txWaitLoop:
	r1 = w[p0 + 0x14];
	cc = bittst(r1, 5);
	if !cc jump txWaitLoop;
	w[p0 + 0x00] = r0;
	rts;

	.globl __CPU_write_crlf;
__CPU_write_crlf:
	r0 = '\r';
	[--sp] = rets;
	call __CPU_write_char;
	rets = [sp++];
	r0 = '\n';
	jump __CPU_write_char;

__CPU_write_space:
	r0 = ' ';
	jump __CPU_write_char;

	.globl __CPU_write_nybble;
__CPU_write_nybble:
	r1 = 0x0f;
	r0 = r0 & r1;
	r0 += '0';
	r1 = '9';
	cc = r0 <= r1;
	if cc jump __CPU_write_char;
	r0 += 'a' - '0' - 10;
	jump __CPU_write_char;

	.globl __CPU_write_byte;
__CPU_write_byte:
	[--sp] = r0;
	[--sp] = rets;
	r0 >>= 4;
	call __CPU_write_nybble;
	rets = [sp++];
	r0 = [sp++];
	jump __CPU_write_nybble;

__CPU_write_chawmp:
	[--sp] = r0;
	[--sp] = rets;
	r0 >>= 8;
	call __CPU_write_byte;
	rets = [sp++];
	r0 = [sp++];
	jump __CPU_write_byte;

__CPU_write_gawble:
	[--sp] = r0;
	[--sp] = rets;
	r0 >>= 16;
	call __CPU_write_chawmp;
	rets = [sp++];
	r0 = [sp++];
	jump __CPU_write_chawmp;

__CPU_dump_registers:
	[--sp] = rets;
	[--sp] = r0;
	[--sp] = r1;
	[--sp] = p0;
	r0 = [sp + 8];
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = [sp + 4];
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = r2;
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = r3;
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = r4;
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = r5;
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = r6;
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = r7;
	call __CPU_write_gawble;
	call __CPU_write_crlf;
	r0 = [sp];
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = p1;
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = p2;
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = p3;
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = p4;
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = p5;
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = fp;
	call __CPU_write_gawble;
	call __CPU_write_space;
	r0 = sp;
	r0 += 16;
	call __CPU_write_gawble;
	call __CPU_write_crlf;

	p0 = [sp++];
	r1 = [sp++];
	r0 = [sp++];
	rets = [sp++];
	rts;

	.globl __CPU_Exception_handler;
__CPU_Exception_handler:
	usp = sp;
	sp.h = 0xffb0;
	sp.l = 0x1000;
	[--sp] = (r7:0,p5:0);

	r0 = 'x';
	call __CPU_write_char;
	jump hcf;


	.globl __CPU_Emulation_handler;
__CPU_Emulation_handler:
	usp = sp;
	sp.h = 0xffb0;
	sp.l = 0x1000;
	[--sp] = (r7:0,p5:0);

	r0 = 'e';
	call __CPU_write_char;
	jump hcf;

	.globl __CPU_Reset_handler;
__CPU_Reset_handler:
	usp = sp;
	sp.h = 0xffb0;
	sp.l = 0x1000;
	[--sp] = (r7:0,p5:0);

	r0 = 'r';
	call __CPU_write_char;
	jump hcf;

	.globl __CPU_NMI_handler;
__CPU_NMI_handler:
	usp = sp;
	sp.h = 0xffb0;
	sp.l = 0x1000;
	[--sp] = (r7:0,p5:0);

	r0 = 'n';
	call __CPU_write_char;
	jump hcf;

	.globl __CPU_Unhandled_Interrupt_handler;
__CPU_Unhandled_Interrupt_handler:
	usp = sp;
	sp.h = 0xffb0;
	sp.l = 0x1000;
	[--sp] = (r7:0,p5:0);

	call __CPU_write_crlf;
	r0 = 'i';
	call __CPU_write_char;
	p0.h = HI(IPEND);
	p0.l = LO(IPEND);
	r0 = [p0];
	call __CPU_write_chawmp;
	jump hcf;

hcf:
	idle;
	jump hcf;

#endif


/*  _CPU_Context_switch
 *
 *  This routine performs a normal non-FP context switch.
 *
 *  bfin Specific Information:
 *
 *  For now we simply save all registers.
 *
 */

/* make sure this sequence stays in sync with the definition for
   Context_Control in rtems/score/cpu.h */
	.globl __CPU_Context_switch
__CPU_Context_switch:
	/* Start saving context R0 = current, R1=heir */
	p0 = r0;
	[p0++] = r4;
	[p0++] = r5;
	[p0++] = r6;
	[p0++] = r7;

	/* save pointer registers */
	[p0++] = p3;
	[p0++] = p4;
	[p0++] = p5;
	[p0++] = fp;
	[p0++] = sp;

	/* save rets */
	r0 = rets;
	[p0++] = r0;

	/* save IMASK */
	p1.h = HI(IMASK);
	p1.l = LO(IMASK);
	r0 = [p1];
	[p0++] = r0;

	p0 = r1;
restore:
	/* restore data registers */
	r4 = [p0++];
	r5 = [p0++];
	r6 = [p0++];
	r7 = [p0++];

	/* restore pointer registers */
	p3 = [p0++];
	p4 = [p0++];
	p5 = [p0++];
	fp = [p0++];
	sp = [p0++];

	/* restore rets */
	r0 = [p0++];
	rets = r0;

	/* restore IMASK */
	r0 = [p0++];
	p1.h = HI(IMASK);
	p1.l = LO(IMASK);
	[p1] = r0;

	rts;


/*
 *  _CPU_Context_restore
 *
 *  This routine is generally used only to restart self in an
 *  efficient manner.  It may simply be a label in _CPU_Context_switch.
 *
 *  NOTE: May be unnecessary to reload some registers.
 *
 *  Blackfin Specific Information:
 *
 *  none
 *
 */
	.globl __CPU_Context_restore
__CPU_Context_restore:
	p0 = r0;
	jump restore;


	.globl __ISR_Handler
	.extern __CPU_Interrupt_stack_high;
	.extern __ISR_Nest_level
	.extern __Thread_Dispatch_disable_level
	.extern __Context_Switch_necessary
	.extern __ISR_Signals_to_thread_executing
__ISR_Handler:
	/* all interrupts are disabled at this point */
	/* the following few items are pushed onto the task stack for at
	   most one interrupt; nested interrupts will be using the interrupt
	   stack for everything. */
	[--sp] = astat;
	[--sp] = p1;
	[--sp] = p0;
	[--sp] = r1;
	[--sp] = r0;
	p0.h = __ISR_Nest_level;
	p0.l = __ISR_Nest_level;
	r0 = [p0];
	r0 += 1;
	[p0] = r0;
	cc = r0 <= 1 (iu);
	if !cc jump noStackSwitch;
	/* setup interrupt stack */
	r0 = sp;
	p0.h = __CPU_Interrupt_stack_high;
	p0.l = __CPU_Interrupt_stack_high;
	sp = [p0];
	[--sp] = r0;
noStackSwitch:
	/* disable thread dispatch */
	p0.h = __Thread_Dispatch_disable_level;
	p0.l = __Thread_Dispatch_disable_level;
	r0 = [p0];
	r0 += 1;
	[p0] = r0;

	[--sp] = reti;	/* interrupts are now enabled */

	/* figure out what vector we are */
	p0.h = HI(IPEND);
	p0.l = LO(IPEND);
	r1 = [p0];
	/* we should only get here for events that require RTI to return */
	r1 = r1 >> 5;
	r0 = 4;
	/* at least one bit must be set, so this loop will exit */
vectorIDLoop:
	r0 += 1;
	r1 = rot r1 by -1;
	if !cc jump vectorIDLoop;

	[--sp] = r2;
	p0.h = __ISR_Vector_table;
	p0.l = __ISR_Vector_table;
	r2 = [p0];
	r1 = r0 << 2;
	r1 = r1 + r2;
	p0 = r1;
	p0 = [p0];
	cc = p0 == 0;
	if cc jump noHandler;

	/* r2, r0, r1, p0, p1, astat are already saved */
	[--sp] = a1.x;
	[--sp] = a1.w;
	[--sp] = a0.x;
	[--sp] = a0.w;
	[--sp] = r3;
	[--sp] = p3;
	[--sp] = p2;
	[--sp] = lc1;
	[--sp] = lc0;
	[--sp] = lt1;
	[--sp] = lt0;
	[--sp] = lb1;
	[--sp] = lb0;
	[--sp] = i3;
	[--sp] = i2;
	[--sp] = i1;
	[--sp] = i0;
	[--sp] = m3;
	[--sp] = m2;
	[--sp] = m1;
	[--sp] = m0;
	[--sp] = l3;
	[--sp] = l2;
	[--sp] = l1;
	[--sp] = l0;
	[--sp] = b3;
	[--sp] = b2;
	[--sp] = b1;
	[--sp] = b0;
	[--sp] = rets;
	/* call user isr; r0 = vector number, r1 = frame pointer */
	r1 = fp; /* is this really what should be passed here? */
	r2 = 0;
	l0 = r2;
	l1 = r2;
	l2 = r2;
	l3 = r2;
	sp += -12; /* bizarre abi... */
	call (p0);
	sp += 12;
	rets = [sp++];
	b0 = [sp++];
	b1 = [sp++];
	b2 = [sp++];
	b3 = [sp++];
	l0 = [sp++];
	l1 = [sp++];
	l2 = [sp++];
	l3 = [sp++];
	m0 = [sp++];
	m1 = [sp++];
	m2 = [sp++];
	m3 = [sp++];
	i0 = [sp++];
	i1 = [sp++];
	i2 = [sp++];
	i3 = [sp++];
	lb0 = [sp++];
	lb1 = [sp++];
	lt0 = [sp++];
	lt1 = [sp++];
	lc0 = [sp++];
	lc1 = [sp++];
	p2 = [sp++];
	p3 = [sp++];
	r3 = [sp++];
	a0.w = [sp++];
	a0.x = [sp++];
	a1.w = [sp++];
	a1.x = [sp++];

noHandler:
	r2 = [sp++];
	/* this disables interrupts again */
	reti = [sp++];

	p0.h = __ISR_Nest_level;
	p0.l = __ISR_Nest_level;
	r0 = [p0];
	r0 += -1;
	[p0] = r0;
	cc = r0 == 0;
	if !cc jump noStackRestore;
	sp = [sp];
noStackRestore:

	/* check this stuff to ensure context_switch_necessary and
	   isr_signals_to_thread_executing are being handled appropriately. */
	p0.h = __Thread_Dispatch_disable_level;
	p0.l = __Thread_Dispatch_disable_level;
	r0 = [p0];
	r0 += -1;
	[p0] = r0;
	cc = r0 == 0;
	if !cc jump noDispatch

	/* do thread dispatch if necessary */
	p0.h = __Context_Switch_necessary;
	p0.l = __Context_Switch_necessary;
	r0 = B[p0] (Z);
	cc = r0 == 0;
	p0.h = __ISR_Signals_to_thread_executing;
	p0.l = __ISR_Signals_to_thread_executing;
	if !cc jump doDispatch
	r0 = B[p0] (Z);
	cc = r0 == 0;
	if cc jump noDispatch
doDispatch:
	r0 = 0;
	B[p0] = r0;
	raise 15;
noDispatch:
	r0 = [sp++];
	r1 = [sp++];
	p0 = [sp++];
	p1 = [sp++];
	astat = [sp++];
	rti


/* the approach here is for the main interrupt handler, when a dispatch is
   wanted, to do a "raise 15".  when the main interrupt handler does its
   "rti", the "raise 15" takes effect and we end up here.  we can now
   safely call _Thread_Dispatch, and do an "rti" to get back to the
   original interrupted function.  this does require self-nesting to be
   enabled; the maximum nest depth is the number of tasks. */
	.global __ISR15_Handler
	.extern __Thread_Dispatch
__ISR15_Handler:
	[--sp] = reti;
	[--sp] = rets;
	[--sp] = astat;
	[--sp] = a1.x;
	[--sp] = a1.w;
	[--sp] = a0.x;
	[--sp] = a0.w;
	[--sp] = r3;
	[--sp] = r2;
	[--sp] = r1;
	[--sp] = r0;
	[--sp] = p3;
	[--sp] = p2;
	[--sp] = p1;
	[--sp] = p0;
	[--sp] = lc1;
	[--sp] = lc0;
	[--sp] = lt1;
	[--sp] = lt0;
	[--sp] = lb1;
	[--sp] = lb0;
	[--sp] = i3;
	[--sp] = i2;
	[--sp] = i1;
	[--sp] = i0;
	[--sp] = m3;
	[--sp] = m2;
	[--sp] = m1;
	[--sp] = m0;
	[--sp] = l3;
	[--sp] = l2;
	[--sp] = l1;
	[--sp] = l0;
	[--sp] = b3;
	[--sp] = b2;
	[--sp] = b1;
	[--sp] = b0;
	r2 = 0;
	l0 = r2;
	l1 = r2;
	l2 = r2;
	l3 = r2;
	sp += -12; /* bizarre abi... */
	call __Thread_Dispatch;
	sp += 12;
	b0 = [sp++];
	b1 = [sp++];
	b2 = [sp++];
	b3 = [sp++];
	l0 = [sp++];
	l1 = [sp++];
	l2 = [sp++];
	l3 = [sp++];
	m0 = [sp++];
	m1 = [sp++];
	m2 = [sp++];
	m3 = [sp++];
	i0 = [sp++];
	i1 = [sp++];
	i2 = [sp++];
	i3 = [sp++];
	lb0 = [sp++];
	lb1 = [sp++];
	lt0 = [sp++];
	lt1 = [sp++];
	lc0 = [sp++];
	lc1 = [sp++];
	p0 = [sp++];
	p1 = [sp++];
	p2 = [sp++];
	p3 = [sp++];
	r0 = [sp++];
	r1 = [sp++];
	r2 = [sp++];
	r3 = [sp++];
	a0.w = [sp++];
	a0.x = [sp++];
	a1.w = [sp++];
	a1.x = [sp++];
	astat = [sp++];
	rets = [sp++];
	reti = [sp++];
	rti;

