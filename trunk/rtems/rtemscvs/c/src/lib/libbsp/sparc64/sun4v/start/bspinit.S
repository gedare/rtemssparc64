/*
 * bspinit.S
 *
 * BSP specific initialization for Sparc64 RTEMS -- sun4v BSP
 *
 *  COPYRIGHT (c) 2010 Gedare Bloom.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 * 
 *  This code defines start code specific to the sun4v BSP
 *
 *  $Id$
 *
 */

#include <rtems/asm.h>
#include <rtems/score/cpu.h>

#define STACK_WINDOW_SAVE_AREA_SIZE (16*8)

.section .text

PUBLIC(_BSP_init)
  .global _BSP_init
  SYM(_BSP_init):

  save %sp, -STACK_WINDOW_SAVE_AREA_SIZE, %sp  

  /* Set up ISR handler for interrupt enable/disable */
	setx SYM(syscall), %l0, %o1
	setx param_space, %l0, %o2
	call SYM(_CPU_ISR_install_raw_handler)
	mov 0x100, %o0 

  ret
  restore

  .align CPU_ALIGNMENT
  param_space:
  	.space 8
