/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/heap.h,v 1.1 2001/05/01 21:23:12 emery Exp $
 *
 * $Log: heap.h,v $
 * Revision 1.1  2001/05/01 21:23:12  emery
 * Initial check-in, works under Windows.
 *
 * Revision 1.1  1998/08/21 20:02:16  schenney
 * Initial revision
 *
 * Revision 1.1  1998/08/21 19:55:40  schenney
 * Initial revision
 *
 */

#ifndef _HEAP_H_
#define _HEAP_H_

typedef struct _PriorityQueue	*PriorityQueuePtr;
typedef struct _QueueEntry	*QueueEntryPtr;

extern void		Priority_Key(QueueEntryPtr, double*, char*);
extern void*		Priority_Value(QueueEntryPtr);
extern PriorityQueuePtr	Priority_New();
extern int		Priority_Empty(PriorityQueuePtr);
extern QueueEntryPtr	Priority_Insert(PriorityQueuePtr, double, char, void*);
extern QueueEntryPtr	Priority_Min(PriorityQueuePtr);
extern QueueEntryPtr	Priority_Delete_Min(PriorityQueuePtr);
extern QueueEntryPtr	Priority_Delete(PriorityQueuePtr, QueueEntryPtr);
extern void             Priority_Free(PriorityQueuePtr);
extern void             Priority_Free_Entry(QueueEntryPtr);
extern void		Priority_Change(PriorityQueuePtr, QueueEntryPtr,
                                        double, char);

#endif
