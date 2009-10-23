/**
 * @file rtems/score/cpu.h
 */

/*
 *  This include file contains information pertaining to the Intel
 *  i386 processor.
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: cpu.h,v 1.29 2009/09/25 12:07:54 joel Exp $
 */

#ifndef _RTEMS_SCORE_CPU_H
#define _RTEMS_SCORE_CPU_H

#include <string.h> /* for memcpy */

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/score/i386.h>              /* pick up machine definitions */

#ifndef ASM
#include <rtems/score/types.h>
#include <rtems/score/interrupts.h>	/* formerly in libcpu/cpu.h> */
#include <rtems/score/registers.h>	/* formerly part of libcpu */
#endif

/* conditional compilation parameters */

#define CPU_INLINE_ENABLE_DISPATCH       TRUE
#define CPU_UNROLL_ENQUEUE_PRIORITY      FALSE

/*
 *  i386 has an RTEMS allocated and managed interrupt stack.
 */

#define CPU_HAS_SOFTWARE_INTERRUPT_STACK TRUE
#define CPU_HAS_HARDWARE_INTERRUPT_STACK FALSE
#define CPU_ALLOCATE_INTERRUPT_STACK     TRUE

/*
 *  Does the RTEMS invoke the user's ISR with the vector number and
 *  a pointer to the saved interrupt frame (1) or just the vector
 *  number (0)?
 */

#define CPU_ISR_PASSES_FRAME_POINTER 0

/*
 *  Some family members have no FP, some have an FPU such as the i387
 *  for the i386, others have it built in (i486DX, Pentium).
 */

#if ( I386_HAS_FPU == 1 )
#define CPU_HARDWARE_FP     TRUE    /* i387 for i386 */
#else
#define CPU_HARDWARE_FP     FALSE
#endif
#define CPU_SOFTWARE_FP     FALSE

#define CPU_ALL_TASKS_ARE_FP             FALSE
#define CPU_IDLE_TASK_IS_FP              FALSE
#define CPU_USE_DEFERRED_FP_SWITCH       TRUE

#define CPU_STACK_GROWS_UP               FALSE
#define CPU_STRUCTURE_ALIGNMENT

/*
 *  Does this port provide a CPU dependent IDLE task implementation?
 *
 *  If TRUE, then the routine _CPU_Thread_Idle_body
 *  must be provided and is the default IDLE thread body instead of
 *  _CPU_Thread_Idle_body.
 *
 *  If FALSE, then use the generic IDLE thread body if the BSP does
 *  not provide one.
 */

#define CPU_PROVIDES_IDLE_THREAD_BODY    TRUE

/*
 *  Define what is required to specify how the network to host conversion
 *  routines are handled.
 */

#define CPU_BIG_ENDIAN                           FALSE
#define CPU_LITTLE_ENDIAN                        TRUE

/* structures */

/*
 *  Basic integer context for the i386 family.
 */

typedef struct {
  uint32_t    eflags;   /* extended flags register                   */
  void       *esp;      /* extended stack pointer register           */
  void       *ebp;      /* extended base pointer register            */
  uint32_t    ebx;      /* extended bx register                      */
  uint32_t    esi;      /* extended source index register            */
  uint32_t    edi;      /* extended destination index flags register */
}   Context_Control;

#define _CPU_Context_Get_SP( _context ) \
  (_context)->esp

/*
 *  FP context save area for the i387 numeric coprocessors.
 */

typedef struct {
  uint8_t     fp_save_area[108];    /* context size area for I80387 */
                                    /*  28 bytes for environment    */
} Context_Control_fp;


/*
 *  The following structure defines the set of information saved
 *  on the current stack by RTEMS upon receipt of execptions.
 *
 * idtIndex is either the interrupt number or the trap/exception number.
 * faultCode is the code pushed by the processor on some exceptions.
 */

typedef struct {
  uint32_t    edi;
  uint32_t    esi;
  uint32_t    ebp;
  uint32_t    esp0;
  uint32_t    ebx;
  uint32_t    edx;
  uint32_t    ecx;
  uint32_t    eax;
  uint32_t    idtIndex;
  uint32_t    faultCode;
  uint32_t    eip;
  uint32_t    cs;
  uint32_t    eflags;
} CPU_Exception_frame;

typedef void (*cpuExcHandlerType) (CPU_Exception_frame*);
extern cpuExcHandlerType _currentExcHandler;
extern void rtems_exception_init_mngt(void);

/*
 *  The following structure defines the set of information saved
 *  on the current stack by RTEMS upon receipt of each interrupt
 *  that will lead to re-enter the kernel to signal the thread.
 */

typedef CPU_Exception_frame CPU_Interrupt_frame;

typedef enum {
  I386_EXCEPTION_DIVIDE_BY_ZERO      = 0,
  I386_EXCEPTION_DEBUG               = 1,
  I386_EXCEPTION_NMI                 = 2,
  I386_EXCEPTION_BREAKPOINT          = 3,
  I386_EXCEPTION_OVERFLOW            = 4,
  I386_EXCEPTION_BOUND               = 5,
  I386_EXCEPTION_ILLEGAL_INSTR       = 6,
  I386_EXCEPTION_MATH_COPROC_UNAVAIL = 7,
  I386_EXCEPTION_DOUBLE_FAULT        = 8,
  I386_EXCEPTION_I386_COPROC_SEG_ERR = 9,
  I386_EXCEPTION_INVALID_TSS         = 10,
  I386_EXCEPTION_SEGMENT_NOT_PRESENT = 11,
  I386_EXCEPTION_STACK_SEGMENT_FAULT = 12,
  I386_EXCEPTION_GENERAL_PROT_ERR    = 13,
  I386_EXCEPTION_PAGE_FAULT          = 14,
  I386_EXCEPTION_INTEL_RES15         = 15,
  I386_EXCEPTION_FLOAT_ERROR         = 16,
  I386_EXCEPTION_ALIGN_CHECK         = 17,
  I386_EXCEPTION_MACHINE_CHECK       = 18,
  I386_EXCEPTION_ENTER_RDBG          = 50     /* to enter manually RDBG */

} Intel_symbolic_exception_name;


/*
 *  context size area for floating point
 *
 *  NOTE:  This is out of place on the i386 to avoid a forward reference.
 */

#define CPU_CONTEXT_FP_SIZE sizeof( Context_Control_fp )

/* variables */

SCORE_EXTERN Context_Control_fp  _CPU_Null_fp_context;
SCORE_EXTERN void               *_CPU_Interrupt_stack_low;
SCORE_EXTERN void               *_CPU_Interrupt_stack_high;

/* constants */

/*
 *  This defines the number of levels and the mask used to pick those
 *  bits out of a thread mode.
 */

#define CPU_MODES_INTERRUPT_LEVEL  0x00000001 /* interrupt level in mode */
#define CPU_MODES_INTERRUPT_MASK   0x00000001 /* interrupt level in mode */

/*
 *  extra stack required by the MPCI receive server thread
 */

#define CPU_MPCI_RECEIVE_SERVER_EXTRA_STACK 1024

/*
 *  i386 family supports 256 distinct vectors.
 */

#define CPU_INTERRUPT_NUMBER_OF_VECTORS      256
#define CPU_INTERRUPT_MAXIMUM_VECTOR_NUMBER  (CPU_INTERRUPT_NUMBER_OF_VECTORS - 1)

/*
 *  This is defined if the port has a special way to report the ISR nesting
 *  level.  Most ports maintain the variable _ISR_Nest_level.
 */

#define CPU_PROVIDES_ISR_IS_IN_PROGRESS FALSE

/*
 *  Minimum size of a thread's stack.
 */

#define CPU_STACK_MINIMUM_SIZE          4096

/*
 *  i386 is pretty tolerant of alignment.  Just put things on 4 byte boundaries.
 */

#define CPU_ALIGNMENT                    4
#define CPU_HEAP_ALIGNMENT               CPU_ALIGNMENT
#define CPU_PARTITION_ALIGNMENT          CPU_ALIGNMENT

/*
 *  On i386 thread stacks require no further alignment after allocation
 *  from the Workspace.
 */

#define CPU_STACK_ALIGNMENT             0

/* macros */

/*
 *  ISR handler macros
 *
 *  These macros perform the following functions:
 *     + initialize the RTEMS vector table
 *     + disable all maskable CPU interrupts
 *     + restore previous interrupt level (enable)
 *     + temporarily restore interrupts (flash)
 *     + set a particular level
 */

#define _CPU_Initialize_vectors()

#define _CPU_ISR_Disable( _level ) i386_disable_interrupts( _level )

#define _CPU_ISR_Enable( _level )  i386_enable_interrupts( _level )

#define _CPU_ISR_Flash( _level )   i386_flash_interrupts( _level )

#define _CPU_ISR_Set_level( _new_level ) \
  { \
    if ( _new_level ) asm volatile ( "cli" ); \
    else              asm volatile ( "sti" ); \
  }

uint32_t   _CPU_ISR_Get_level( void );

/* end of ISR handler macros */

/*
 *  Context handler macros
 *
 *  These macros perform the following functions:
 *     + initialize a context area
 *     + restart the current thread
 *     + calculate the initial pointer into a FP context area
 *     + initialize an FP context area
 */

#define CPU_EFLAGS_INTERRUPTS_ON  0x00003202
#define CPU_EFLAGS_INTERRUPTS_OFF 0x00003002

#define _CPU_Context_Initialize( _the_context, _stack_base, _size, \
                                   _isr, _entry_point, _is_fp ) \
  do { \
    uint32_t   _stack; \
    \
    if ( (_isr) ) (_the_context)->eflags = CPU_EFLAGS_INTERRUPTS_OFF; \
    else          (_the_context)->eflags = CPU_EFLAGS_INTERRUPTS_ON; \
    \
    _stack = ((uint32_t)(_stack_base)) + (_size) - 4; \
    \
    *((proc_ptr *)(_stack)) = (_entry_point); \
    (_the_context)->ebp     = (void *) _stack; \
    (_the_context)->esp     = (void *) _stack; \
  } while (0)

#define _CPU_Context_Restart_self( _the_context ) \
   _CPU_Context_restore( (_the_context) );

#define _CPU_Context_Fp_start( _base, _offset ) \
   ( (void *) _Addresses_Add_offset( (_base), (_offset) ) )

#define _CPU_Context_Initialize_fp( _fp_area ) \
  { \
    memcpy( *_fp_area, &_CPU_Null_fp_context, CPU_CONTEXT_FP_SIZE ); \
  }

/* end of Context handler macros */

/*
 *  Fatal Error manager macros
 *
 *  These macros perform the following functions:
 *    + disable interrupts and halt the CPU
 */

#define _CPU_Fatal_halt( _error ) \
  { \
    asm volatile ( "cli ; \
                    movl %0,%%eax ; \
                    hlt" \
                    : "=r" ((_error)) : "0" ((_error)) \
    ); \
  }

/* end of Fatal Error manager macros */

/*
 *  Bitfield handler macros
 *
 *  These macros perform the following functions:
 *     + scan for the highest numbered (MSB) set in a 16 bit bitfield
 */

#define CPU_USE_GENERIC_BITFIELD_CODE FALSE
#define CPU_USE_GENERIC_BITFIELD_DATA FALSE

#define _CPU_Bitfield_Find_first_bit( _value, _output ) \
  { \
    register uint16_t   __value_in_register = (_value); \
    \
    _output = 0; \
    \
    asm volatile ( "bsfw    %0,%1 " \
                    : "=r" (__value_in_register), "=r" (_output) \
                    : "0"  (__value_in_register), "1"  (_output) \
    ); \
  }

/* end of Bitfield handler macros */

/*
 *  Priority handler macros
 *
 *  These macros perform the following functions:
 *    + return a mask with the bit for this major/minor portion of
 *      of thread priority set.
 *    + translate the bit number returned by "Bitfield_find_first_bit"
 *      into an index into the thread ready chain bit maps
 */

#define _CPU_Priority_Mask( _bit_number ) \
  ( 1 << (_bit_number) )

#define _CPU_Priority_bits_index( _priority ) \
  (_priority)

/* functions */

/*
 *  _CPU_Initialize
 *
 *  This routine performs CPU dependent initialization.
 */

void _CPU_Initialize(void);

/*
 *  _CPU_ISR_install_raw_handler
 *
 *  This routine installs a "raw" interrupt handler directly into the
 *  processor's vector table.
 */

void _CPU_ISR_install_raw_handler(
  uint32_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
);

/*
 *  _CPU_ISR_install_vector
 *
 *  This routine installs an interrupt vector.
 */

void _CPU_ISR_install_vector(
  uint32_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
);

/*
 *  _CPU_Thread_Idle_body
 *
 *  Use the halt instruction of low power mode of a particular i386 model.
 */

#if (CPU_PROVIDES_IDLE_THREAD_BODY == TRUE)

void *_CPU_Thread_Idle_body( uintptr_t ignored );

#endif /* CPU_PROVIDES_IDLE_THREAD_BODY */

/*
 *  _CPU_Context_switch
 *
 *  This routine switches from the run context to the heir context.
 */

void _CPU_Context_switch(
  Context_Control  *run,
  Context_Control  *heir
);

/*
 *  _CPU_Context_restore
 *
 *  This routine is generally used only to restart self in an
 *  efficient manner and avoid stack conflicts.
 */

void _CPU_Context_restore(
  Context_Control *new_context
) RTEMS_COMPILER_NO_RETURN_ATTRIBUTE;

/*
 *  _CPU_Context_save_fp
 *
 *  This routine saves the floating point context passed to it.
 */

void _CPU_Context_save_fp(
  Context_Control_fp **fp_context_ptr
);

/*
 *  _CPU_Context_restore_fp
 *
 *  This routine restores the floating point context passed to it.
 */

void _CPU_Context_restore_fp(
  Context_Control_fp **fp_context_ptr
);

#ifdef __cplusplus
}
#endif

#endif
