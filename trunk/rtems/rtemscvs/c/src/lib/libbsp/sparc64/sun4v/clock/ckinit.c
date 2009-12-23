/*  ckinit.c
 *
 *  This file provides a template for the clock device driver initialization.
 *
 *  Modified for sun4v - niagara
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: ckinit.c,v 1.17 2008/09/16 19:09:53 joel Exp $
 */

#include <stdlib.h>

#include <rtems.h>
#include <bsp.h>
#include <bspopts.h>
#include <asm.h>

/* this is probably wrong, and not the right way to do things */
#define CPU_FREQ (5000000)

uint64_t sun4v_cycles_per_tick;

/* CLOCK_DRIVER_USE_FAST_IDLE ? */
#if SIMSPARC_FAST_IDLE==1
#define CLOCK_DRIVER_USE_FAST_IDLE
#endif

/* sun4v: TICK_CMPR triggers soft interrupt 14 */
#define CLOCK_VECTOR SPARC_ASYNCHRONOUS_TRAP(0x4E)

void Clock_driver_support_at_tick(void)
{
  uint64_t tick_reg;
  int bit_mask;


  bit_mask = SPARC_SOFTINT_TM_BIT;
//  sparc64_clear_interrupt_bits(SPARC_SOFTINT_TM_BIT);
//  sparc64_clear_interrupt_tm;

//  asm volatile("wr %%g0, %c0, %%softint_clr": : "i" (0x01) );

  /* TODO: this could be more efficiently implemented as a single assembly 
   * inline */
  sparc64_read_tick(tick_reg);
  tick_reg += sun4v_cycles_per_tick;
  sparc64_write_tick_cmpr(tick_reg);
}

/* this should be #define, but set_vector is not declared right in shell */
#define Clock_driver_support_install_isr(_new, _old) \
  do { \
    _old = set_vector( _new, CLOCK_VECTOR, 1 ); \
  } while ( 0 )

void Clock_driver_support_initialize_hardware(void)
{
  uint64_t tick_reg;

  sun4v_cycles_per_tick = rtems_configuration_get_microseconds_per_tick()*(CPU_FREQ/1000000);

  sparc64_read_tick(tick_reg);
  tick_reg += sun4v_cycles_per_tick;
  sparc64_write_tick_cmpr(tick_reg);
}

#define Clock_driver_support_shutdown_hardware( ) \
  do { \
    \
  } while ( 0 )


#include "../../../shared/clockdrv_shell.h"

