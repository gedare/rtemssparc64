/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/heap.c,v 1.1 2001/05/01 21:23:11 emery Exp $
 *
 * $Log: heap.c,v $
 * Revision 1.1  2001/05/01 21:23:11  emery
 * Initial check-in, works under Windows.
 *
 * Revision 1.1  1998/08/21 20:02:16  schenney
 * Initial revision
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <heap.h>

#define New(t, n)	( (t*)malloc(sizeof(t) * (n)) )
#define More(p, t, n)	( (t*)realloc((char*)(p), sizeof(t) * (n)) )

/* Code derived from:
     Jeffrey H. Kingston,
     "Algorithms and Data Structures: Design, Correctness, Analysis",
     Addison Wesley, 1990
*/

typedef struct _QueueEntry {
    double  key;
    char    sub_key;
    void    *value;
    int	    back;
} QueueEntry;


typedef struct _PriorityQueue {
    int		    num_entries;
    int		    max_num_entries;
    QueueEntryPtr   *entries;
} PriorityQueue;


static int		max_free = 0;
static int		num_free = 0;
static QueueEntryPtr	*free_list = NULL;


#define Swap(a, b, c) ( (c) = (a), (a) = (b), (b) = (c) )

#define Key_Greater(a, b) ( a->key > b->key || \
                            ( a->key == b->key && a->sub_key > b->sub_key ) )


void
Priority_Free(PriorityQueuePtr q)
{
    free(q->entries);
}

void
Priority_Key(QueueEntryPtr e, double *key, char *sub_key)
{
    *key = e->key;
    *sub_key = e->sub_key;
}

void*
Priority_Value(QueueEntryPtr e)
{
    return e->value;
}

PriorityQueuePtr
Priority_New()
{
    PriorityQueuePtr	res = New(PriorityQueue, 1);

    res->max_num_entries = 10;
    res->entries = New(QueueEntryPtr, res->max_num_entries);
    res->num_entries = 0;

    return res;
}


static void
Add_Leaf(PriorityQueuePtr q, int i)
{
    QueueEntryPtr   e;
    int	    	    j;

    e = q->entries[i];
    j = ( i + 1 ) / 2 - 1;
    while ( j >= 0 && Key_Greater(q->entries[j], e) )
    {
	q->entries[i] = q->entries[j];
	q->entries[i]->back = i;
	i = j;
	j = ( i + 1 ) / 2 - 1;
    }
    q->entries[i] = e;
    q->entries[i]->back = i;
}


static void
Add_Root(PriorityQueuePtr q, int i)
{
    QueueEntryPtr   temp;
    int		    j;

    j = 2 * ( i + 1 ) - 1;

    if ( j < q->num_entries )
    {
	if ( j < q->num_entries - 1
	  && Key_Greater(q->entries[j], q->entries[j+1]) )
	    j++;
	if ( Key_Greater(q->entries[i], q->entries[j]) )
	{
	    Swap(q->entries[i], q->entries[j], temp);
	    q->entries[i]->back = i;
	    q->entries[j]->back = j;
	    Add_Root(q, j);
	}
    }
}


int
Priority_Empty(PriorityQueuePtr q)
{
    return ! q->num_entries;
}


QueueEntryPtr
Priority_Insert(PriorityQueuePtr q, double key, char sub_key, void *value)
{
    QueueEntryPtr   res;

    if ( q->num_entries == q->max_num_entries )
    {
	q->max_num_entries += 10;
	q->entries = More(q->entries, QueueEntryPtr, q->max_num_entries);
    }
    res = ( num_free ? free_list[--num_free] : New(QueueEntry, 1) );
    res->key = key;
    res->sub_key = sub_key;
    res->value = value;
    res->back = q->num_entries;
    q->entries[q->num_entries] = res;
    q->num_entries++;
    Add_Leaf(q, q->num_entries - 1);

    return res;
}


QueueEntryPtr
Priority_Min(PriorityQueuePtr q)
{
    if ( ! q->num_entries )
	return NULL;
    return q->entries[0];
}


QueueEntryPtr
Priority_Delete_Min(PriorityQueuePtr q)
{
    QueueEntryPtr   e;

    if ( ! q->num_entries )
	return NULL;

    e = q->entries[0];

    q->num_entries--;
    if ( q->num_entries )
    {
	q->entries[0] = q->entries[q->num_entries];
	q->entries[0]->back = 0;
	Add_Root(q, 0);
    }

    return e;
}


QueueEntryPtr
Priority_Delete(PriorityQueuePtr q, QueueEntryPtr e)
{
    int	i;

    q->num_entries--;

    if ( e->back < q->num_entries )
    {
	i = e->back;
	q->entries[i] = q->entries[q->num_entries];
	q->entries[i]->back = i;
	Add_Root(q, i);
	Add_Leaf(q, i);
    }

    return e;
}


void
Priority_Change(PriorityQueuePtr q, QueueEntryPtr e, double key, char sub_key)
{
    int	i = e->back;

    e->key = key;
    e->sub_key = sub_key;
    Add_Root(q, i);
    Add_Leaf(q, i);
}


void
Priority_Free_Entry(QueueEntryPtr e)
{
    if ( num_free == max_free )
    {
	if ( max_free )
	{
	    max_free *= 2;
	    free_list = More(free_list, QueueEntryPtr, max_free);
	}
	else
	{
	    max_free = 16;
	    free_list = New(QueueEntryPtr, max_free);
	}
    }

    free_list[num_free++] = e;
}


