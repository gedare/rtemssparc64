/*
 * Support for UltraSparc T1 M5 Simulator
 *
 * Copyright (c) 2010 Gedare Bloom.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 *
 * $Id$
 */

#include <rtems/system.h>
#include <rtems/score/cpu.h>
#include <rtems/score/isr.h>
#include <rtems/score/thread.h>
#include <rtems/score/sparc64.h>

/*
 *  _CPU_Context_save_fp_context
 *
 *  This routine is responsible for saving the FP context
 *  at *fp_context_ptr.  If the point to load the FP context
 *  from is changed then the pointer is modified by this routine.
 *
 *  Sometimes a macro implementation of this is in cpu.h which dereferences
 *  the ** and a similarly named routine in this file is passed something
 *  like a (Context_Control_fp *).  The general rule on making this decision
 *  is to avoid writing assembly language.
 *
 *  m5sim Specific Information:
 *
 */

void _CPU_Context_save_fp(
  Context_Control_fp **fp_context_ptr
)
{
}

/*
 *  _CPU_Context_restore_fp_context
 *
 *  This routine is responsible for restoring the FP context
 *  at *fp_context_ptr.  If the point to load the FP context
 *  from is changed then the pointer is modified by this routine.
 *
 *  Sometimes a macro implementation of this is in cpu.h which dereferences
 *  the ** and a similarly named routine in this file is passed something
 *  like a (Context_Control_fp *).  The general rule on making this decision
 *  is to avoid writing assembly language.
 *
 *  m5sim Specific Information:
 *
 */

void _CPU_Context_restore_fp(
  Context_Control_fp **fp_context_ptr
)
{
}

/*  _CPU_Context_switch
 *
 *  This routine performs a normal non-FP context switch.
 *
 *  m5sim Specific Information:
 *
 */

void _CPU_Context_switch(
  Context_Control  *run,
  Context_Control  *heir
)
{
}

/*
 *  _CPU_Context_restore
 *
 *  This routine is generally used only to restart self in an
 *  efficient manner.  It may simply be a label in _CPU_Context_switch.
 *
 *  NOTE: May be unnecessary to reload some registers.
 *
 *  m5sim Specific Information:
 *
 */

void _CPU_Context_restore(
  Context_Control *new_context
)
{
}

/*  void __ISR_Handler()
 *
 *  This routine provides the RTEMS interrupt management.
 *
 *  m5sim Specific Information:
 *
 */

void _ISR_Handler(void)
{
   /*
    *  This discussion ignores a lot of the ugly details in a real
    *  implementation such as saving enough registers/state to be
    *  able to do something real.  Keep in mind that the goal is
    *  to invoke a user's ISR handler which is written in C and
    *  uses a certain set of registers.
    *
    *  Also note that the exact order is to a large extent flexible.
    *  Hardware will dictate a sequence for a certain subset of
    *  _ISR_Handler while requirements for setting
    */

  /*
   *  At entry to "common" _ISR_Handler, the vector number must be
   *  available.  On some CPUs the hardware puts either the vector
   *  number or the offset into the vector table for this ISR in a
   *  known place.  If the hardware does not give us this information,
   *  then the assembly portion of RTEMS for this port will contain
   *  a set of distinct interrupt entry points which somehow place
   *  the vector number in a known place (which is safe if another
   *  interrupt nests this one) and branches to _ISR_Handler.
   *
   *  save some or all context on stack
   *  may need to save some special interrupt information for exit
   *
   *  #if ( CPU_HAS_SOFTWARE_INTERRUPT_STACK == TRUE )
   *    if ( _ISR_Nest_level == 0 )
   *      switch to software interrupt stack
   *  #endif
   *
   *  _ISR_Nest_level++;
   *
   *  _Thread_Dispatch_disable_level++;
   *
   *  (*_ISR_Vector_table[ vector ])( vector );
   *
   *  _Thread_Dispatch_disable_level--;
   *
   *  --_ISR_Nest_level;
   *
   *  if ( _ISR_Nest_level )
   *    goto the label "exit interrupt (simple case)"
   *
   *  if ( _Thread_Dispatch_disable_level )
   *    _ISR_Signals_to_thread_executing = FALSE;
   *    goto the label "exit interrupt (simple case)"
   *
   *  if ( _Context_Switch_necessary || _ISR_Signals_to_thread_executing ) {
   *    _ISR_Signals_to_thread_executing = FALSE;
   *    call _Thread_Dispatch() or prepare to return to _ISR_Dispatch
   *    prepare to get out of interrupt
   *    return from interrupt  (maybe to _ISR_Dispatch)
   *
   *  LABEL "exit interrupt (simple case):
   *  #if ( CPU_HAS_SOFTWARE_INTERRUPT_STACK == TRUE )
   *    if outermost interrupt
   *      restore stack
   *  #endif
   *  prepare to get out of interrupt
   *  return from interrupt
   */
}