/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/red_black.h,v 1.1 2001/05/01 21:23:19 emery Exp $
 *
 * $Log: red_black.h,v $
 * Revision 1.1  2001/05/01 21:23:19  emery
 * Initial check-in, works under Windows.
 *
 *
 */

#ifndef _RED_BLACK_H_
#define _RED_BLACK_H_

typedef struct _RBTreeRoot *RBTree;


extern RBTree	RBTree_New();
extern void*	RBTree_Search(RBTree, unsigned long);
extern void*	RBTree_Insert(RBTree, unsigned long, void*);
extern void	RBTree_Apply(RBTree,
			     void (*)(unsigned long, void*, void*), void*);
extern void	RBTree_Copy(RBTree, RBTree,
			    void* (*)(unsigned long, void*, void*), void*);
extern void	RBTree_Clear(RBTree, void (*)(void*, void*), void*);
extern void	RBTree_Print(RBTree, FILE*, void (*func)(FILE*, void*, int));


#endif
