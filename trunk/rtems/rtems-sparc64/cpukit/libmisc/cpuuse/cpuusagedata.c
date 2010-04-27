/*
 *  CPU Usage Reporter - Shared Data
 *
 *  COPYRIGHT (c) 1989-2009
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: cpuusagedata.c,v 1.3 2009/12/02 18:22:18 humph Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <rtems/cpuuse.h>
#ifndef __RTEMS_USE_TICKS_FOR_STATISTICS__
  #include <rtems/score/timespec.h>
#endif

#ifndef __RTEMS_USE_TICKS_FOR_STATISTICS__
  struct timespec    CPU_usage_Uptime_at_last_reset;
#else
  uint32_t           CPU_usage_Ticks_at_last_reset;
#endif
