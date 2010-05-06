/*
 *  syscall.S
 *
 *  COPYRIGHT (c) 2010. Gedare Bloom.
 *
 *  The terms of the RTEMS license apply to this file.
 *
 */

#include <rtems/asm.h>
#include "sparc-syscall.h"

.seg    "text"
/*
 *  system call
 *
 *  TODO: make these actually work
 *
 */

PUBLIC(syscall)

  SYM(syscall):


PUBLIC(sparc_disable_interrupts)

  SYM(sparc_disable_interrupts):

    retl  
    nop

PUBLIC(sparc_enable_interrupts)

  SYM(sparc_enable_interrupts):

    retl  
    nop

    /* end of file */
