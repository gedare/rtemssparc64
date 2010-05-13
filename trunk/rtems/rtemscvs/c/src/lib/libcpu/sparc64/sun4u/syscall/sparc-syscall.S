/*
 *  systrap.S
 *
 *  This file contains emulated system calls using software trap 0.
 *  The following calls are supported:
 *
 *    + SYS_exit  (halt)
 *    + SYS_irqdis  (disable interrupts)
 *    + SYS_irqset  (set interrupt level)
 * 
 *  COPYRIGHT:
 *
 *  COPYRIGHT (c) 1995. European Space Agency.
 *  COPYRIGHT (c) 2010. Gedare Bloom.
 *
 *  This terms of the RTEMS license apply to this file.
 *
 */

#include <rtems/asm.h>
#include "sparc-syscall.h"


.seg    "text"
/*
 *  system call
 *
 *  On entry:
 *    if arch = sun4v:
 *      g4 = tstate (from trap table)
 *      g2 = trap vector # (256)
 *      g3 = address of SYM(syscall)
 *      g4[GL-1] = system call id
 *
 *      We need to back to GL-1, so we won't use g2-g4. So the 
 *      syscall code needs to re-read tstate
 *   on sun4u:
 *    TODO: has an alternative set based on the type of trap
 *    
 *    First thing is to return to the previous set of globals, so 
 *    that the system call id can be read.
 */

PUBLIC(syscall)

  SYM(syscall):
/*
    mov   %g0, %g4  ! clear %g4 at this GL
    rdpr  %gl, %g1
    dec   %g1
    wrpr  %g0, %g1, %gl                     ! go back to GL = GL - 1
    subcc %g4, 2, %g0                     
    bne   3f
    rdpr  %tstate, %g5                      ! re-read tstate, use delay slot

    ! syscall 2, disable interrupts
    rdpr  %pil, %g1
    and   %g5, SPARC_TSTATE_IE_MASK, %o0
    or    %o0, %g1, %o0                     ! return TSTATE_IE | PIL
    wrpr  %g0, 0xf, %pil                    ! set PIL to 15
    andn  %g5, SPARC_TSTATE_IE_MASK, %g1
    wrpr  %g0, %g1, %tstate                 ! disable interrupts in trap state
    ba,a  9f

    3:  ! syscall 3, enable interrupts
    subcc %g4, 3, %g0                     
    bne   1f
    and   %o0, 0xf, %g1
    wrpr  %g0, %g1, %pil                    ! restore PIL
!    and   %o0, SPARC_TSTATE_IE_MASK, %g1
!    or    %g5, %g1, %g1                     ! restore saved IE
    or    %g5, SPARC_TSTATE_IE_MASK, %g1    ! restore IE (safe?)
    wrpr  %g0, %g1, %tstate     
    ba,a  9f

    1:          
    ba,a  1b                                ! spin. taking a trap here -> htrap

    9:                                      ! leave
    mov  0, %g4                             ! clear %g4
    DONE
*/

PUBLIC(sparc_disable_interrupts)

  SYM(sparc_disable_interrupts):
/*
    mov  SYS_irqdis, %g4
    ta  0
*/
    retl  
    nop

PUBLIC(sparc_enable_interrupts)

  SYM(sparc_enable_interrupts):
/*
    mov  SYS_irqen, %g4
    ta  0
*/
    retl  
    nop

    /* end of file */
