/*
 *  SPARC-v9 Dependent Source
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

/*PAGE
 *
 *  _CPU_Initialize
 *
 *  This routine performs processor dependent initialization.
 *
 *  INPUT PARAMETERS: NONE
 *
 *  Output Parameters: NONE
 *
 *  NOTE: There is no need to save the pointer to the thread dispatch routine.
 *        The SPARC's assembly code can reference it directly with no problems.
 */

void _CPU_Initialize(void)
{
#if (SPARC_HAS_FPU == 1)
  Context_Control_fp *pointer;

  /*
   *  This seems to be the most appropriate way to obtain an initial
   *  FP context on the SPARC.  The NULL fp context is copied in to
   *  the task's FP context during Context_Initialize_fp.
   */

  pointer = &_CPU_Null_fp_context;
  _CPU_Context_save_fp( &pointer );

#endif

  /*
   *  Since no tasks have been created yet and no interrupts have occurred,
   *  there is no way that the currently executing thread can have an
   *  _ISR_Dispatch stack frame on its stack.
   */
  _CPU_ISR_Dispatch_disable = 0;
}

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

  sparc64_get_interrupt_level( level );

  return level;
}

/*PAGE
 *
 *  _CPU_Context_Initialize
 *
 *  This kernel routine initializes the basic non-FP context area associated
 *  with each thread.
 *
 *  Input parameters:
 *    the_context  - pointer to the context area
 *    stack_base   - address of memory for the SPARC
 *    size         - size in bytes of the stack area
 *    new_level    - interrupt level for this context area
 *    entry_point  - the starting execution point for this this context
 *    is_fp        - TRUE if this context is associated with an FP thread
 *
 *  Output parameters: NONE
 */

void _CPU_Context_Initialize(
  Context_Control  *the_context,
  void         *stack_base,
  uint32_t          size,
  uint32_t          new_level,
  void             *entry_point,
  bool              is_fp
)
{
    uint64_t     stack_high;  /* highest "stack aligned" address */
    uint32_t     the_size;
    uint64_t     tmp_pstate;
    uint64_t    tmp_pil;

    /*
     *  On CPUs with stacks which grow down (i.e. SPARC), we build the stack
     *  based on the stack_high address.
     */

    stack_high = ((uint64_t)(stack_base) + size);
    stack_high &= ~(CPU_STACK_ALIGNMENT - 1);

    the_size = size & ~(CPU_STACK_ALIGNMENT - 1);

    /*
     *  See the README in this directory for a diagram of the stack.
     */

    the_context->o7    = ((uint64_t) entry_point) - 8;
    the_context->o6_sp = stack_high - CPU_MINIMUM_STACK_FRAME_SIZE - STACK_BIAS;
    the_context->i6_fp = 0;

#ifdef GICADEBUG
	printk("\n\r"
			"_CPU_Context_Initialize() "
			"stack_base = %x "
			"size = %x "
			"CPU_STACK_ALIGNMENT = %d "
			"stack_high = %x "
			"the_context->o6_sp = %x"
			"CPU_MINIMUM_STACK_FRAME_SIZE = %d"
		"\n\r",
		stack_base,
		size,
		CPU_STACK_ALIGNMENT,
		stack_high,
		the_context->o6_sp,
		CPU_MINIMUM_STACK_FRAME_SIZE
		);
#endif
    /*
     *  Build the pstate for the task.  Most everything can be 0 and the
     *  CWP is corrected during the context switch if necessary. 
     *
     *  The PEF bit determines if the floating point unit is available.
     *  The FPU is ONLY enabled if the context is associated with an FP task
     *  and this SPARC model has an FPU.
     */

    sparc64_get_pstate( tmp_pstate );
    tmp_pstate &= ~SPARC_PSTATE_PEF_MASK;  /* disable FPU by default */
    tmp_pstate |= SPARC_PSTATE_IE_MASK;    /* enable interrupts */

    /*
     * Set the processor interrupt level (PIL) to the new_level
     * The PIL is a 4-bit register specifying the level above which
     * interrupts are taken.
     */
    tmp_pil = new_level & 0xF;

#if (SPARC_HAS_FPU == 1)
    /*
     *  If this bit is not set, then a task gets a fault when it accesses
     *  a floating point register.  This is a nice way to detect floating
     *  point tasks which are not currently declared as such.
     */

    if ( is_fp )
      tmp_pstate |= SPARC_PSTATE_PEF_MASK;
#endif
    the_context->pstate = tmp_pstate;
    the_context->pil = tmp_pil;

  /*
   *  Since THIS thread is being created, there is no way that THIS
   *  thread can have an _ISR_Dispatch stack frame on its stack.
   */
    the_context->isr_dispatch_disable = 0;
}
