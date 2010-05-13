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

.section .text

PUBLIC(_BSP_init)
.global _BSP_init
  SYM(_BSP_init):
  

  retl
  nop
