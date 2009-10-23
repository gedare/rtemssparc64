/**
 * @file
 *
 * @ingroup ScoreProtHeap
 *
 * @brief Protected Heap Handler implementation.
 */

/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: pheapallocate.c,v 1.3 2009/09/06 15:24:07 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/protectedheap.h>

void *_Protected_heap_Allocate(
  Heap_Control *the_heap,
  uintptr_t     size
)
{
  void *p;

  _RTEMS_Lock_allocator();
    p = _Heap_Allocate( the_heap, size );
  _RTEMS_Unlock_allocator();
  return p;
}

