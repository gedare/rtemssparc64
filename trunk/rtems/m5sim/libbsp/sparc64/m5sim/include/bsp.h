/*
 *  This include file contains all board IO definitions.
 *
 *  SPARC_SE M5 Simulator
 *
 *  Author: Gedare Bloom 
 *
 *  COPYRIGHT (c) 2010, Gedare Bloom
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 * $Id$
 */

#ifndef _BSP_H
#define _BSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems.h>
#include <rtems/clockdrv.h>
#include <rtems/console.h>

#include <bspopts.h>

/*
 * FIXME: One of these would be enough.
 */
#include <rtems/devnull.h>

/* Constants */

Thread clock_driver_sim_idle_body(uintptr_t);
#define BSP_IDLE_TASK_BODY clock_driver_sim_idle_body

/* this should be defined somewhere */
rtems_isr_entry set_vector(                     /* returns old vector */
    rtems_isr_entry     handler,                /* isr routine        */
    rtems_vector_number vector,                 /* vector number      */
    int                 type                    /* RTEMS or RAW intr  */
);

/*
 * FIXME: Not applicable with m5 simulator
 */
#define rtems_bsp_delay( microseconds ) ;


#ifdef __cplusplus
}
#endif

#endif
