/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/hash.h,v 1.1 2001/05/01 21:23:11 emery Exp $
 *
 * $Log: hash.h,v $
 * Revision 1.1  2001/05/01 21:23:11  emery
 * Initial check-in, works under Windows.
 *
 * Revision 1.0  1997/05/06 20:42:12  schenney
 * Initial revision
 *
 */

#ifndef _HASH_H_
#define _HASH_H_

typedef struct _HashTable *HashTable;

extern HashTable    Hash_New_Table(unsigned long);
extern void* 	    Hash_Insert(HashTable, unsigned long, void*);
extern void*	    Hash_Get_Value(HashTable, unsigned long);
extern void 	    Hash_Free(HashTable);
extern void 	    Hash_Clear(HashTable);
extern void*	    Hash_Delete(HashTable, unsigned long);
extern void*	    Hash_Traverse(HashTable, int);

#if HASH_STATS
extern void	    Hash_Print_Stats(HashTable, FILE*);
#endif

#endif
