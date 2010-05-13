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

#define LSU_CR_IM_MASK  (0x0004)  /* bit 2 */
#define LSU_CR_DM_MASK  (0x0008)  /* bit 3 */

#define STACK_WINDOW_SAVE_AREA_SIZE (16*8)
#define TABLE_SIZE (1024*32)

.register %g2, #scratch
.register %g3, #scratch

.section .ttable
.align (TABLE_SIZE)
PUBLIC(trap_table)
  SYM(trap_table):
  .space TABLE_SIZE

.section .text

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

  /* turn off the I-MMU and D-MMU */
  /* this approach fails -- because the RTEMS application is not entirely 
     resident in physical memory... */
#if 0
  ! LSU Control Register ASI is 0x45, with VA 0x00
  ldxa [%g0] (0x45), %g1 
  andn %g1, LSU_CR_IM_MASK, %g1
  andn %g1, LSU_CR_DM_MASK, %g1
  stxa %g1, [%g0] (0x45)

  /* set the trap base address */
  setx  SYM(trap_table), %l0, %l1
  wrpr  %g0, %l1, %tba
#endif
  ret
  restore

