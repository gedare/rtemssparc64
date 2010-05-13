/*
* bspinit.S
*
* BSP specific initialization for Sparc64 RTEMS -- sun4u BSP
*
*  COPYRIGHT (c) 2010 Gedare Bloom.
*
*  The license and distribution terms for this file may be
*  found in the file LICENSE in this distribution or at
*  http://www.rtems.com/license/LICENSE.
* 
*  This code defines start code specific to the sun4u BSP
*
*  $Id$
*
*/

#include <rtems/asm.h>
#include <rtems/score/cpu.h>


#define STACK_WINDOW_SAVE_AREA_SIZE (16*8)
#define TABLE_SIZE (1024*32)

.section .text

.align (4*32)
PUBLIC(trap_table)
  SYM(trap_table):
  .space TABLE_SIZE

PUBLIC(_BSP_init)
.global _BSP_init
  SYM(_BSP_init):

  save %sp, -STACK_WINDOW_SAVE_AREA_SIZE, %sp  

  /* copy firmware trap table so that RTEMS can install ISR handlers */
  setx  SYM(trap_table), %l0, %o0
  rdpr  %tba, %o1
  set TABLE_SIZE, %o2
  call memcpy
  nop

  ret
  restore

