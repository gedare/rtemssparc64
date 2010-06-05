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

#include <traptable.h>

#define LSU_CR_IM_MASK  (0x0004)  /* bit 2 */
#define LSU_CR_DM_MASK  (0x0008)  /* bit 3 */

#define STACK_WINDOW_SAVE_AREA_SIZE (16*8)

.register %g2, #scratch
.register %g3, #scratch

.section .text

PUBLIC(_BSP_init)
.global _BSP_init
  SYM(_BSP_init):
  save %sp, -STACK_WINDOW_SAVE_AREA_SIZE, %sp





#if 1
  /* copy firmware trap table so that RTEMS can install ISR handlers */
  setx  SYM(trap_table), %l0, %o0
  rdpr  %tba, %o1
  set TABLE_SIZE, %o2
  call memcpy
  nop
#endif

  /* map firmware trap table so that RTEMS can install ISR handlers */




  mov %g0, %o0
  call _take_mmu
  nop

#if 0 /* this is in shared/start.S */
  /* Set up ISR handler for interrupt enable/disable */
	setx SYM(syscall), %l0, %o1
	setx param_space, %l0, %o2
	call SYM(_CPU_ISR_install_raw_handler)
	mov 0x100, %o0 
#endif


  /* turn off the I-MMU and D-MMU */
  /* this approach fails -- apparently the RTEMS application is not entirely 
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

#if 0
  .align CPU_ALIGNMENT
  param_space:
  	.space 8
#endif
