/*  bsp.h
 *
 *  This include file contains all C4X Simulator IO definitions.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bsp.h,v 1.17 2008/09/18 17:39:53 joel Exp $
 */

#ifndef _BSP_H
#define _BSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bspopts.h>

#ifdef ASM

#else
#include <rtems.h>
#include <rtems/console.h>
#include <rtems/clockdrv.h>
#include <rtems/console.h>
#include <rtems/iosupp.h>

/* functions */

rtems_isr_entry set_vector(                    /* returns old vector */
  rtems_isr_entry     handler,                  /* isr routine        */
  rtems_vector_number vector,                   /* vector number      */
  int                 type                      /* RTEMS or RAW intr  */
);

void BSP_fatal_return( void );

void bsp_spurious_initialize( void );

#endif /* ASM */

#ifdef __cplusplus
}
#endif

#endif
