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
 *  $Id: pheapallocatealigned.c,v 1.4 2009/09/06 15:24:07 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/protectedheap.h>

void *_Protected_heap_Allocate_aligned(
  Heap_Control *the_heap,
  uintptr_t     size,
  uintptr_t     alignment
)
{
  void *p;

  _RTEMS_Lock_allocator();
    p = _Heap_Allocate_aligned( the_heap, size, alignment );
  _RTEMS_Unlock_allocator();
  return p;
}

