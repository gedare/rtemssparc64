/*
 *  SPARC-v9 Dependent Source -- m5sim specific
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  This file is based on the SPARC cpu.c file. Modifications are made to 
 *  provide support for the SPARC-v9.
 *    COPYRIGHT (c) 2010. Gedare Bloom.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <rtems/system.h>
#include <rtems/asm.h>
#include <rtems/score/isr.h>
#include <rtems/rtems/cache.h>

/*
 *  This initializes the set of opcodes placed in each trap
 *  table entry.  The routine which installs a handler is responsible
 *  for filling in the fields for the _handler address and the _vector
 *  trap type.
 *
 *  The constants following this structure are masks for the fields which
 *  must be filled in when the handler is installed.
 */

/*  64-bit registers complicate this. Also, in sparc v9,
 *	each trap level gets its own set of global registers, but
 *	does not get its own dedicated register window. so we avoid
 *	using the local registers in the trap handler.
 */
const CPU_Trap_table_entry _CPU_Trap_slot_template = {
  0x89508000,	/* rdpr   %tstate, %g4       */
  0x05000000,	/* sethi %hh(_handler), %g2  */
  0x8410a000,	/* or     %g2, %hm(_handler), %g2 */
  0x8528b020,	/* sllx   %g2, 32, %g2 */
  0x07000000,	/* sethi  %hi(_handler), %g3 */
  0x8610c002,	/* or     %g3, %g2, %g3 */
  0x81c0e000, /* jmp   %g3 + %lo(_handler) */
  0x84102000  /* mov   _vector, %g2        */
};

/*PAGE
 *
 *  _CPU_ISR_Get_level
 *
 *  Input Parameters: NONE
 *
 *  Output Parameters:
 *    returns the current interrupt level (PIL field of the PSR)
 */

uint32_t   _CPU_ISR_Get_level( void )
{
  uint32_t   level;

  level = 0;  /* TODO: FIXME */

  return level;
}

/*PAGE
 *
 *  _CPU_ISR_install_raw_handler
 *
 *  This routine installs the specified handler as a "raw" non-executive
 *  supported trap handler (a.k.a. interrupt service routine).
 *
 *  Input Parameters:
 *    vector      - trap table entry number plus synchronous
 *                    vs. asynchronous information
 *    new_handler - address of the handler to be installed
 *    old_handler - pointer to an address of the handler previously installed
 *
 *  Output Parameters: NONE
 *    *new_handler - address of the handler previously installed
 *
 *  NOTE:
 *
 *  On the SPARC v8, there are really only 512 vectors.  However, the executive
 *  has no easy, fast, reliable way to determine which traps are synchronous
 *  and which are asynchronous.  By default, traps return to the
 *  instruction which caused the interrupt.  So if you install a software
 *  trap handler as an executive interrupt handler (which is desirable since
 *  RTEMS takes care of window and register issues), then the executive needs
 *  to know that the return address is to the trap rather than the instruction
 *  following the trap.
 *
 *  So vectors 0 through 511 are treated as regular asynchronous traps which
 *  provide the "correct" return address.  Vectors 512 through 1023 are assumed
 *  by the executive to be synchronous and to require that the return be to the
 *  trapping instruction.
 *
 *  If you use this mechanism to install a trap handler which must reexecute
 *  the instruction which caused the trap, then it should be installed as
 *  a synchronous trap.  This will avoid the executive changing the return
 *  address.
 */
/*  Verified this is working properly from sparc64_install_isr_entries */
void _CPU_ISR_install_raw_handler(
  uint32_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
)
{
}

/*PAGE
 *
 *  _CPU_ISR_install_vector
 *
 *  This kernel routine installs the RTEMS handler for the
 *  specified vector.
 *
 *  Input parameters:
 *    vector       - interrupt vector number
 *    new_handler  - replacement ISR for this vector number
 *    old_handler  - pointer to former ISR for this vector number
 *
 *  Output parameters:
 *    *old_handler - former ISR for this vector number
 *
 */

void _CPU_ISR_install_vector(
  uint64_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
)
{
   uint64_t   real_vector;
   proc_ptr   ignored;

  /*
   *  Get the "real" trap number for this vector ignoring the synchronous
   *  versus asynchronous indicator included with our vector numbers.
   */
   real_vector = SPARC_REAL_TRAP_NUMBER( vector );
   /*
    *  Return the previous ISR handler.
    */

   *old_handler = _ISR_Vector_table[ vector ];

   /*
    *  Install the wrapper so this ISR can be invoked properly.
    */

   _CPU_ISR_install_raw_handler( vector, _ISR_Handler, &ignored );

   /*
    *  We put the actual user ISR address in '_ISR_vector_table'.  This will
    *  be used by the _ISR_Handler so the user gets control.
    */

    _ISR_Vector_table[ real_vector ] = new_handler;
}

