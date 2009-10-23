/*
 *  malloc_walk Implementation
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: malloc_walk.c,v 1.4 2009/09/14 14:48:38 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef RTEMS_NEWLIB
#include "malloc_p.h"

#include <stdlib.h>

void malloc_walk(size_t source, size_t printf_enabled)
{
  _Protected_heap_Walk( RTEMS_Malloc_Heap, (int) source, printf_enabled );
}

#endif
