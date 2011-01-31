/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/overlaps.c,v 1.1 2001/05/01 21:23:13 emery Exp $
 *
 * $Log: overlaps.c,v $
 * Revision 1.1  2001/05/01 21:23:13  emery
 * Initial check-in, works under Windows.
 *
 */

#include <simulator.h>

#define X 0
#define Y 1
#define Z 2 


typedef struct _DimOverlapListElmt {
    unsigned long   index;
    unsigned char   dim;
    bool            add;

    struct _DimOverlapListElmt  *next;
    struct _DimOverlapListElmt  *prev;
    } DimOverlapListElmt, *DimOverlapList;


static OverlapList	free_overlaps = NULL;
static OverlapList  	free_overlap_lists = NULL;
static DimOverlapList  	free_dim_overlap_head = NULL;
static DimOverlapList  	free_dim_overlap_tail = NULL;


#define Dim_Overlap_Remove(head, tail, elmt) { \
	if ( (elmt)->next ) \
	    (elmt)->next->prev = (elmt)->prev; \
	else \
	    (tail) = (elmt)->prev; \
	if ( (elmt)->prev ) \
	    (elmt)->prev->next = (elmt)->next; \
	else \
	    (head) = (elmt)->next; \
    }


#define Dim_Overlap_Insert_Elmt(head, tail, elmt) { \
	if ( tail ) \
	    (tail)->next = (elmt); \
	else \
	    (head) = elmt; \
	(elmt)->next = NULL; \
	(elmt)->prev = (tail); \
	(tail) = (elmt); \
    }


#define Dim_Overlap_Insert(head, tail, ind, d, a) { \
	DimOverlapList  _new_list; \
	if ( free_dim_overlap_tail ) \
	{ \
	    _new_list = free_dim_overlap_tail; \
	    free_dim_overlap_tail = free_dim_overlap_tail->prev; \
	    if ( free_dim_overlap_tail ) \
		free_dim_overlap_tail->next = NULL; \
	    else \
		free_dim_overlap_head = NULL; \
	} \
	else \
	    _new_list = New(DimOverlapListElmt, 1); \
	Dim_Overlap_Insert_Elmt(head, tail, _new_list); \
	_new_list->index = (ind); \
	_new_list->dim = (d); \
	_new_list->add = (a); \
    }


#define Overlap_List_Insert(list, o, elmt) {\
	if ( free_overlap_lists ) \
	{ \
	    (elmt) = free_overlap_lists; \
	    free_overlap_lists = (elmt)->next; \
	} \
	else \
	    (elmt) = New(OverlapListElmt, 1); \
	(elmt)->overlap = o; \
	(elmt)->next = (list); \
	(elmt)->prev = NULL; \
	if ( (elmt)->next ) \
	    (elmt)->next->prev = (elmt); \
	(list) = (elmt); \
    }


#define Overlap_List_Delete(list, elmt) { \
	OverlapList _temp_l = (elmt); \
	if ( (elmt)->next ) \
	    (elmt)->next->prev = (elmt)->prev; \
	if ( (elmt)->prev ) \
	    (elmt)->prev->next = (elmt)->next; \
	else \
	    (list) = (elmt)->next; \
	_temp_l->next = free_overlap_lists; \
	free_overlap_lists = _temp_l; \
    }


static void
Overlap_Free_Features(OverlapPtr o)
{
    int	i;

    for ( i = 0 ; i < o->o1->num_components ; i++ )
    {
	free(o->features[i]);
    }
    free(o->features);
    o->features = NULL;
}


static void
Overlap_Allocate_Features(OverlapPtr o)
{
    int	i, j;

    o->features = New(FeaturePair*, o->o1->num_components);
    for ( i = 0 ; i < o->o1->num_components ; i++ )
    {
	o->features[i] = New(FeaturePair, o->o2->num_components);
	for ( j = 0 ; j < o->o2->num_components ; j++ )
	{
	    o->features[i][j].f1.feature = NULL;
	    o->features[i][j].f2.feature = NULL;
	}
    }
}


void
Overlaps_Free(OverlapList list)
{
    OverlapList	junk;

    while ( list )
    {
	OverlapList victim = list;
	list = list->next;

	if ( victim->overlap->o1_xyz_elmt
	  && victim != victim->overlap->o1_xyz_elmt )
	{
	    Overlap_List_Delete(
		((DynObjectPtr)victim->overlap->o1->detail)->xyz_overlaps,
		victim->overlap->o1_xyz_elmt);
	    victim->overlap->o1_xyz_elmt = NULL;
	}
	if ( victim->overlap->o2_xyz_elmt
	  && victim != victim->overlap->o2_xyz_elmt )
	{
	    Overlap_List_Delete(
		((DynObjectPtr)victim->overlap->o2->detail)->xyz_overlaps,
		victim->overlap->o2_xyz_elmt);
	    victim->overlap->o2_xyz_elmt = NULL;
	}
	Overlap_Free_Features(victim->overlap);
	Overlap_List_Insert(free_overlaps, victim->overlap, junk);
	victim->next = free_overlap_lists;
	free_overlap_lists = victim;
    }
}


static bool
Process_Mod_Overlaps(SimulationPtr sim, bool predict, DimOverlapList head,
		     DimOverlapList tail)
{
    DimOverlapList  l, temp_l;
    DimOverlapList  change_head = NULL;
    DimOverlapList  change_tail = NULL;
    OverlapPtr	    o;
    int		    ind;
    bool	    adding, deleting;
    OverlapList	    junk;

    l = head;
    while ( l )
    {
	adding = deleting = false;
	switch ( l->dim )
	{
	  case X:
	    if ( l->add )
	    {
		if ( Hash_Insert(sim->x_overlaps, l->index, NULL) == (void*)-1
		  && Hash_Get_Value(sim->y_overlaps, l->index) != (void*)-1
		  && Hash_Get_Value(sim->z_overlaps, l->index) != (void*)-1 )
		    adding = true;
	    }
	    else
	    {
		Hash_Delete(sim->x_overlaps, l->index);
		deleting = true;
	    }
	    break;
	  case Y:
	    if ( l->add )
	    {
		if ( Hash_Insert(sim->y_overlaps, l->index, NULL) == (void*)-1
		  && Hash_Get_Value(sim->x_overlaps, l->index) != (void*)-1
		  && Hash_Get_Value(sim->z_overlaps, l->index) != (void*)-1 )
		    adding = true;
	    }
	    else
	    {
		Hash_Delete(sim->y_overlaps, l->index);
		deleting = true;
	    }
	    break;
	  case Z:
	    if ( l->add )
	    {
		if ( Hash_Insert(sim->z_overlaps, l->index, NULL) == (void*)-1
		  && Hash_Get_Value(sim->x_overlaps, l->index) != (void*)-1
		  && Hash_Get_Value(sim->y_overlaps, l->index) != (void*)-1 )
		    adding = true;
	    }
	    else
	    {
		Hash_Delete(sim->z_overlaps, l->index);
		deleting = true;
	    }
	    break;
	}

	temp_l = l;
	l = l->next;

	if ( adding || deleting )
	{
	    DimOverlapList  existing =
	        (DimOverlapList)Hash_Get_Value(sim->add_del_ops, temp_l->index);

	    if ( existing == (DimOverlapList)-1 )
	    {
		Dim_Overlap_Remove(head, tail, temp_l);
		temp_l->add = adding;
		Hash_Insert(sim->add_del_ops, temp_l->index, (void*)temp_l);
		Dim_Overlap_Insert_Elmt(change_head, change_tail, temp_l);
	    }
	    else
		existing->add = adding;
	}
    }

    for ( l = change_head ; l ; l = l->next )
    {
	if ( (DimOverlapList)Hash_Delete(sim->add_del_ops, l->index) != l )
	{
	    fprintf(stderr, "Thing in hash table is not same as list elmt.\n");
	    abort();
	}

	if ( l->add
	  && Hash_Get_Value(sim->xyz_overlaps, l->index) == (void*)-1 )
	{
	    if ( free_overlaps )
	    {
		o = free_overlaps->overlap;
		Overlap_List_Delete(free_overlaps, free_overlaps);
	    }
	    else
		o = New(Overlap, 1);

	    o->type = pairwise_event;
	    o->queue_entry = NULL;

	    o->o1 = sim->dyn_objects[l->index & 0xFFFF];
	    Overlap_List_Insert(((DynObjectPtr)o->o1->detail)->xyz_overlaps, o,
				o->o1_xyz_elmt);

	    ind = ( l->index >> 16 ) & 0xFFFF;
	    if ( ind >= sim->num_dynamic )
	    {
		o->o2 = sim->fixed_objects[ind - sim->num_dynamic];
		o->o2_xyz_elmt = NULL;
	    }
	    else
	    {
		o->o2 = sim->dyn_objects[ind];
		Overlap_List_Insert(((DynObjectPtr)o->o2->detail)->xyz_overlaps,
				    o, o->o2_xyz_elmt);
	    }

	    Overlap_Allocate_Features(o);

	    Hash_Insert(sim->xyz_overlaps, l->index, (void*)o);

	    if ( predict )
	    {
		unsigned char prediction = Update_Prediction(sim, o, true);
		if ( prediction == PENETRATING
		  || prediction == INTEG_ERROR
		  || prediction == INTERFERENCE )
		    return false;
	    }
	    else
		o->min_t = -1.0e20;
	}

	if ( ! l->add
	  && ( o = Hash_Delete(sim->xyz_overlaps, l->index) ) != (void*)-1 )
	{
	    Overlap_List_Delete(((DynObjectPtr)o->o1->detail)->xyz_overlaps,
				o->o1_xyz_elmt);
	    o->o1_xyz_elmt = NULL;
	    if ( o->o2_xyz_elmt )
	    {
		Overlap_List_Delete(
		    ((DynObjectPtr)o->o2->detail)->xyz_overlaps,
		    o->o2_xyz_elmt);
		o->o2_xyz_elmt = NULL;
	    }
	    if ( o->queue_entry )
		Event_Delete(sim, (SimEventPtr)o);

	    Overlap_Free_Features(o);
	    Overlap_List_Insert(free_overlaps, o, junk);
	}
    }

    if ( tail )
    {
	tail->next = free_dim_overlap_head;
	if ( free_dim_overlap_head )
	    free_dim_overlap_head->prev = tail;
	else
	    free_dim_overlap_tail = tail;
	free_dim_overlap_head = head;
    }
    if ( change_tail )
    {
	change_tail->next = free_dim_overlap_head;
	if ( free_dim_overlap_head )
	    free_dim_overlap_head->prev = change_tail;
	else
	    free_dim_overlap_tail = change_tail;
	free_dim_overlap_head = change_head;
    }

    return true;
}


static void
Sort_Check(SimulationPtr sim)
{
    ObjectList	elmt;
    double  	val, last_val;

    last_val = -1.0e20;
    for ( elmt = sim->sorted_x ; elmt ; elmt = elmt->next )
    {
	val = ( elmt == &(elmt->obj->x_min) ? elmt->obj->bound->min.x
					    : elmt->obj->bound->max.x );
	if ( val < last_val )
	{
	    fprintf(stderr, "Sort failed\n");
	    abort();
	}
	last_val = val;
    }
    last_val = -1.0e20;
    for ( elmt = sim->sorted_y ; elmt ; elmt = elmt->next )
    {
	val = ( elmt == &(elmt->obj->y_min) ? elmt->obj->bound->min.y
					    : elmt->obj->bound->max.y );
	if ( val < last_val )
	{
	    fprintf(stderr, "Sort failed\n");
	    abort();
	}
	last_val = val;
    }
    last_val = -1.0e20;
    for ( elmt = sim->sorted_z ; elmt ; elmt = elmt->next )
    {
	val = ( elmt == &(elmt->obj->z_min) ? elmt->obj->bound->min.z
					    : elmt->obj->bound->max.z );
	if ( val < last_val )
	{
	    fprintf(stderr, "Sort failed\n");
	    abort();
	}
	last_val = val;
    }
}


static void
Sort_Print(SimulationPtr sim)
{
    ObjectList	elmt;
    double  	val;

    fprintf(stderr, "X:\n");
    for ( elmt = sim->sorted_x ; elmt ; elmt = elmt->next )
    {
	val = ( elmt == &(elmt->obj->x_min) ? elmt->obj->bound->min.x
					    : elmt->obj->bound->max.x );
	fprintf(stderr, "%d: %15.15g\n", elmt->obj->index, val);
    }
    fprintf(stderr, "Y:\n");
    for ( elmt = sim->sorted_y ; elmt ; elmt = elmt->next )
    {
	val = ( elmt == &(elmt->obj->y_min) ? elmt->obj->bound->min.y
					    : elmt->obj->bound->max.y );
	fprintf(stderr, "%d: %15.15g\n", elmt->obj->index, val);
    }
    fprintf(stderr, "Z:\n");
    for ( elmt = sim->sorted_z ; elmt ; elmt = elmt->next )
    {
	val = ( elmt == &(elmt->obj->z_min) ? elmt->obj->bound->min.z
					    : elmt->obj->bound->max.z );
	fprintf(stderr, "%d: %15.15g\n", elmt->obj->index, val);
    }
}


static void
Sort_Obj_X(SimulationPtr sim, SimObjectPtr obj, SimObjectPtr ignore,
	   DimOverlapList *head, DimOverlapList *tail)
{
    ObjectList	elmt, prev_elmt;
    bool    	elmt_min;
    double  	elmt_val;
    int		elmt_i;
    bool	max_out = false;

    /* Look for the right place for the left edge. */
    elmt = obj->x_min.prev;
    while ( elmt && elmt->obj == ignore )
	elmt = elmt->prev;
    if ( elmt )
    {
	elmt_min = ( &(elmt->obj->x_min) == elmt );
	elmt_val = ( elmt_min ? elmt->obj->bound->min.x
			      : elmt->obj->bound->max.x );
	elmt_i = elmt->obj->index;
	if ( elmt->obj->fixed )
	    elmt_i += sim->num_dynamic;
    }
    else
	elmt_val = obj->bound->min.x - 1.0;
    if ( obj->bound->min.x < elmt_val
      || ( obj->bound->min.x == elmt_val && obj->index < elmt_i ) )
    {
	/* Need to shuffle obj down. */

	/* Take the obj out. */
	obj->x_min.prev->next = obj->x_min.next;
	obj->x_min.next->prev = obj->x_min.prev;

	do
	{
	    if ( ! elmt_min )
	    {
		unsigned long key = ( obj->index > elmt_i
				    ? ( obj->index << 16 ) | elmt_i
				    : ( elmt_i << 16 ) | obj->index );
		Dim_Overlap_Insert(*head, *tail, key, X, true);
	    }

	    elmt = elmt->prev;
	    while ( elmt && elmt->obj == ignore )
		elmt = elmt->prev;
	    if ( elmt )
	    {
		elmt_min = ( &(elmt->obj->x_min) == elmt );
		elmt_val = ( elmt_min ? elmt->obj->bound->min.x
				      : elmt->obj->bound->max.x );
		elmt_i = elmt->obj->index;
		if ( elmt->obj->fixed )
		    elmt_i += sim->num_dynamic;
	    }
	    else
		elmt_val = obj->bound->min.x - 1.0;
	} while ( obj->bound->min.x < elmt_val
	       || ( obj->bound->min.x == elmt_val && obj->index < elmt_i ) );

	/* Put obj in after elmt. */
	if ( elmt )
	{
	    obj->x_min.next = elmt->next;
	    elmt->next = &(obj->x_min);
	    obj->x_min.prev = elmt;
	    obj->x_min.next->prev = &(obj->x_min);
	}
	else
	{
	    obj->x_min.next = sim->sorted_x;
	    sim->sorted_x = &(obj->x_min);
	    obj->x_min.prev = NULL;
	    obj->x_min.next->prev = &(obj->x_min);
	}
    }
    else
    {
	elmt = obj->x_min.next;
	while ( elmt && elmt->obj == ignore )
	    elmt = elmt->next;
	if ( elmt == &(obj->x_max) )
	{
	    if ( obj->x_max.next )
		obj->x_max.next->prev = obj->x_max.prev;
	    obj->x_max.prev->next = obj->x_max.next;
	    elmt = obj->x_max.next;
	    while ( elmt && elmt->obj == ignore )
		elmt = elmt->next;
	    max_out = true;
	}
	if ( elmt )
	{
	    elmt_min = ( &(elmt->obj->x_min) == elmt );
	    elmt_val = ( elmt_min ? elmt->obj->bound->min.x
				  : elmt->obj->bound->max.x );
	    elmt_i = elmt->obj->index;
	    if ( elmt->obj->fixed )
		elmt_i += sim->num_dynamic;
	}
	else
	    elmt_val = obj->bound->min.x + 1.0;
	if ( obj->bound->min.x > elmt_val
	  || ( obj->bound->min.x == elmt_val && obj->index > elmt_i ) )
	{
	    /* Need to shuffle obj up. */

	    /* Take the obj out. */
	    if ( obj->x_min.prev )
		obj->x_min.prev->next = obj->x_min.next;
	    else
		sim->sorted_x = obj->x_min.next;
	    obj->x_min.next->prev = obj->x_min.prev;

	    do
	    {
		if ( ! elmt_min )
		{
		    unsigned long key = ( obj->index > elmt_i
					? ( obj->index << 16 ) | elmt_i
					: ( elmt_i << 16 ) | obj->index );
		    Dim_Overlap_Insert(*head, *tail, key, X, false);
		}
		else if ( max_out )
		{
		    unsigned long key = ( obj->index > elmt_i
					? ( obj->index << 16 ) | elmt_i
					: ( elmt_i << 16 ) | obj->index );
		    Dim_Overlap_Insert(*head, *tail, key, X, true);

		}

		prev_elmt = elmt;
		elmt = elmt->next;
		while ( elmt && elmt->obj == ignore )
		{
		    prev_elmt = elmt;
		    elmt = elmt->next;
		}
		if ( elmt == &(obj->x_max) )
		{
		    prev_elmt = obj->x_max.prev;
		    if ( obj->x_max.next )
			obj->x_max.next->prev = obj->x_max.prev;
		    obj->x_max.prev->next = obj->x_max.next;
		    elmt = obj->x_max.next;
		    max_out = true;
		    while ( elmt && elmt->obj == ignore )
		    {
			prev_elmt = elmt;
			elmt = elmt->next;
		    }
		}
		if ( elmt )
		{
		    elmt_min = ( &(elmt->obj->x_min) == elmt );
		    elmt_val = ( elmt_min ? elmt->obj->bound->min.x
					  : elmt->obj->bound->max.x );
		    elmt_i = elmt->obj->index;
		    if ( elmt->obj->fixed )
			elmt_i += sim->num_dynamic;
		}
		else
		    elmt_val = obj->bound->min.x + 1.0;
	    } while ( obj->bound->min.x > elmt_val
		   || ( obj->bound->min.x == elmt_val && obj->index > elmt_i ));

	    /* Put obj in before elmt. */
	    obj->x_min.next = elmt;
	    obj->x_min.prev = prev_elmt;
	    obj->x_min.prev->next = &(obj->x_min);
	    if ( elmt )
		elmt->prev = &(obj->x_min);
	}
    }

    if ( max_out )
    {
	/* Put it back in after the min. */
	obj->x_max.next = obj->x_min.next;
	if ( obj->x_max.next )
	    obj->x_max.next->prev = &(obj->x_max);
	obj->x_max.prev = &(obj->x_min);
	obj->x_min.next = &(obj->x_max);
    }

    /* Look for the right place for the right edge. */
    elmt = obj->x_max.prev;
    while ( elmt && elmt->obj == ignore )
	elmt = elmt->prev;
    if ( elmt )
    {
	elmt_min = ( &(elmt->obj->x_min) == elmt );
	elmt_val = ( elmt_min ? elmt->obj->bound->min.x
			      : elmt->obj->bound->max.x );
	elmt_i = elmt->obj->index;
	if ( elmt->obj->fixed )
	    elmt_i += sim->num_dynamic;
    }
    else
	elmt_val = obj->bound->max.x - 1.0;
    if ( obj->bound->max.x < elmt_val
      || ( obj->bound->max.x == elmt_val && obj->index < elmt_i ) )
    {
	/* Need to shuffle obj down. */

	/* Take the obj out. */
	obj->x_max.prev->next = obj->x_max.next;
	if ( obj->x_max.next )
	    obj->x_max.next->prev = obj->x_max.prev;

	do
	{
	    if ( elmt_min )
	    {
		unsigned long key = ( obj->index > elmt_i
				    ? ( obj->index << 16 ) | elmt_i
				    : ( elmt_i << 16 ) | obj->index );
		Dim_Overlap_Insert(*head, *tail, key, X, false);
	    }

	    elmt = elmt->prev;
	    while ( elmt && elmt->obj == ignore )
		elmt = elmt->prev;
	    if ( elmt )
	    {
		elmt_min = ( &(elmt->obj->x_min) == elmt );
		elmt_val = ( elmt_min ? elmt->obj->bound->min.x
				      : elmt->obj->bound->max.x );
		elmt_i = elmt->obj->index;
		if ( elmt->obj->fixed )
		    elmt_i += sim->num_dynamic;
	    }
	    else
		elmt_val = obj->bound->max.x - 1.0;
	} while ( obj->bound->max.x < elmt_val
	       || ( obj->bound->max.x == elmt_val && obj->index < elmt_i ) );

	/* Put obj in after elmt. */
	obj->x_max.next = elmt->next;
	elmt->next = &(obj->x_max);
	obj->x_max.prev = elmt;
	obj->x_max.next->prev = &(obj->x_max);
    }
    else
    {
	elmt = obj->x_max.next;
	while ( elmt && elmt->obj == ignore )
	    elmt = elmt->next;
	if ( elmt )
	{
	    elmt_min = ( &(elmt->obj->x_min) == elmt );
	    elmt_val = ( elmt_min ? elmt->obj->bound->min.x
				  : elmt->obj->bound->max.x );
	    elmt_i = elmt->obj->index;
	    if ( elmt->obj->fixed )
		elmt_i += sim->num_dynamic;
	}
	else
	    elmt_val = obj->bound->max.x + 1.0;
	if ( obj->bound->max.x > elmt_val
	  || ( obj->bound->max.x == elmt_val && obj->index > elmt_i ) )
	{
	    /* Need to shuffle obj up. */

	    /* Take the obj out. */
	    if ( obj->x_max.prev )
		obj->x_max.prev->next = obj->x_max.next;
	    else
		sim->sorted_x = obj->x_max.next;
	    obj->x_max.next->prev = obj->x_max.prev;

	    do
	    {
		if ( elmt_min )
		{
		    unsigned long key = ( obj->index > elmt_i
					? ( obj->index << 16 ) | elmt_i
					: ( elmt_i << 16 ) | obj->index );
		    Dim_Overlap_Insert(*head, *tail, key, X, true);
		}

		prev_elmt = elmt;
		elmt = elmt->next;
		while ( elmt && elmt->obj == ignore )
		{
		    prev_elmt = elmt;
		    elmt = elmt->next;
		}
		if ( elmt )
		{
		    elmt_min = ( &(elmt->obj->x_min) == elmt );
		    elmt_val = ( elmt_min ? elmt->obj->bound->min.x
					  : elmt->obj->bound->max.x );
		    elmt_i = elmt->obj->index;
		    if ( elmt->obj->fixed )
			elmt_i += sim->num_dynamic;
		}
		else
		    elmt_val = obj->bound->max.x + 1.0;
	    } while ( obj->bound->max.x > elmt_val
		   || ( obj->bound->max.x == elmt_val && obj->index > elmt_i ));

	    /* Put obj in before elmt. */
	    obj->x_max.next = elmt;
	    obj->x_max.prev = prev_elmt;
	    obj->x_max.prev->next = &(obj->x_max);
	    if ( elmt )
		elmt->prev = &(obj->x_max);
	}
    }
}


static void
Sort_Obj_Y(SimulationPtr sim, SimObjectPtr obj, SimObjectPtr ignore,
	   DimOverlapList *head, DimOverlapList *tail)
{
    ObjectList	elmt, prev_elmt;
    bool    	elmt_min;
    double  	elmt_val;
    int		elmt_i;
    bool	max_out = false;

    /* Look for the right place for the left edge. */
    elmt = obj->y_min.prev;
    while ( elmt && elmt->obj == ignore )
	elmt = elmt->prev;
    if ( elmt )
    {
	elmt_min = ( &(elmt->obj->y_min) == elmt );
	elmt_val = ( elmt_min ? elmt->obj->bound->min.y
			      : elmt->obj->bound->max.y );
	elmt_i = elmt->obj->index;
	if ( elmt->obj->fixed )
	    elmt_i += sim->num_dynamic;
    }
    else
	elmt_val = obj->bound->min.y - 1.0;
    if ( obj->bound->min.y < elmt_val
      || ( obj->bound->min.y == elmt_val && obj->index < elmt_i ) )
    {
	/* Need to shuffle obj down. */

	/* Take the obj out. */
	obj->y_min.prev->next = obj->y_min.next;
	obj->y_min.next->prev = obj->y_min.prev;

	do
	{
	    if ( ! elmt_min )
	    {
		unsigned long key = ( obj->index > elmt_i
				    ? ( obj->index << 16 ) | elmt_i
				    : ( elmt_i << 16 ) | obj->index );
		Dim_Overlap_Insert(*head, *tail, key, Y, true);
	    }

	    elmt = elmt->prev;
	    while ( elmt && elmt->obj == ignore )
		elmt = elmt->prev;
	    if ( elmt )
	    {
		elmt_min = ( &(elmt->obj->y_min) == elmt );
		elmt_val = ( elmt_min ? elmt->obj->bound->min.y
				      : elmt->obj->bound->max.y );
		elmt_i = elmt->obj->index;
		if ( elmt->obj->fixed )
		    elmt_i += sim->num_dynamic;
	    }
	    else
		elmt_val = obj->bound->min.y - 1.0;
	} while ( obj->bound->min.y < elmt_val
	       || ( obj->bound->min.y == elmt_val && obj->index < elmt_i ) );

	/* Put obj in after elmt. */
	if ( elmt )
	{
	    obj->y_min.next = elmt->next;
	    elmt->next = &(obj->y_min);
	    obj->y_min.prev = elmt;
	    obj->y_min.next->prev = &(obj->y_min);
	}
	else
	{
	    obj->y_min.next = sim->sorted_y;
	    sim->sorted_y = &(obj->y_min);
	    obj->y_min.prev = NULL;
	    obj->y_min.next->prev = &(obj->y_min);
	}
    }
    else
    {
	elmt = obj->y_min.next;
	while ( elmt && elmt->obj == ignore )
	    elmt = elmt->next;
	if ( elmt == &(obj->y_max) )
	{
	    if ( obj->y_max.next )
		obj->y_max.next->prev = obj->y_max.prev;
	    obj->y_max.prev->next = obj->y_max.next;
	    elmt = obj->y_max.next;
	    max_out = true;
	    while ( elmt && elmt->obj == ignore )
		elmt = elmt->next;
	}
	if ( elmt )
	{
	    elmt_min = ( &(elmt->obj->y_min) == elmt );
	    elmt_val = ( elmt_min ? elmt->obj->bound->min.y
				  : elmt->obj->bound->max.y );
	    elmt_i = elmt->obj->index;
	    if ( elmt->obj->fixed )
		elmt_i += sim->num_dynamic;
	}
	else
	    elmt_val = obj->bound->min.y + 1.0;
	if ( obj->bound->min.y > elmt_val
	  || ( obj->bound->min.y == elmt_val && obj->index > elmt_i ) )
	{
	    /* Need to shuffle obj up. */

	    /* Take the obj out. */
	    if ( obj->y_min.prev )
		obj->y_min.prev->next = obj->y_min.next;
	    else
		sim->sorted_y = obj->y_min.next;
	    obj->y_min.next->prev = obj->y_min.prev;

	    do
	    {
		if ( ! elmt_min )
		{
		    unsigned long key = ( obj->index > elmt_i
					? ( obj->index << 16 ) | elmt_i
					: ( elmt_i << 16 ) | obj->index );
		    Dim_Overlap_Insert(*head, *tail, key, Y, false);
		}
		else if ( max_out )
		{
		    unsigned long key = ( obj->index > elmt_i
					? ( obj->index << 16 ) | elmt_i
					: ( elmt_i << 16 ) | obj->index );
		    Dim_Overlap_Insert(*head, *tail, key, Y, true);
		}

		prev_elmt = elmt;
		elmt = elmt->next;
		while ( elmt && elmt->obj == ignore )
		{
		    prev_elmt = elmt;
		    elmt = elmt->next;
		}
		if ( elmt == &(obj->y_max) )
		{
		    prev_elmt = obj->y_max.prev;
		    if ( obj->y_max.next )
			obj->y_max.next->prev = obj->y_max.prev;
		    obj->y_max.prev->next = obj->y_max.next;
		    elmt = obj->y_max.next;
		    max_out = true;
		    while ( elmt && elmt->obj == ignore )
		    {
			prev_elmt = elmt;
			elmt = elmt->next;
		    }
		}
		if ( elmt )
		{
		    elmt_min = ( &(elmt->obj->y_min) == elmt );
		    elmt_val = ( elmt_min ? elmt->obj->bound->min.y
					  : elmt->obj->bound->max.y );
		    elmt_i = elmt->obj->index;
		    if ( elmt->obj->fixed )
			elmt_i += sim->num_dynamic;
		}
		else
		    elmt_val = obj->bound->min.y + 1.0;
	    } while ( obj->bound->min.y > elmt_val
		   || ( obj->bound->min.y == elmt_val && obj->index > elmt_i ));

	    /* Put obj in before elmt. */
	    obj->y_min.next = elmt;
	    obj->y_min.prev = prev_elmt;
	    obj->y_min.prev->next = &(obj->y_min);
	    if ( elmt )
		elmt->prev = &(obj->y_min);
	}
    }

    if ( max_out )
    {
	/* Put it back in after the min. */
	obj->y_max.next = obj->y_min.next;
	if ( obj->y_max.next )
	    obj->y_max.next->prev = &(obj->y_max);
	obj->y_max.prev = &(obj->y_min);
	obj->y_min.next = &(obj->y_max);
    }

    /* Look for the right place for the right edge. */
    elmt = obj->y_max.prev;
    while ( elmt && elmt->obj == ignore )
	elmt = elmt->prev;
    if ( elmt )
    {
	elmt_min = ( &(elmt->obj->y_min) == elmt );
	elmt_val = ( elmt_min ? elmt->obj->bound->min.y
			      : elmt->obj->bound->max.y );
	elmt_i = elmt->obj->index;
	if ( elmt->obj->fixed )
	    elmt_i += sim->num_dynamic;
    }
    else
	elmt_val = obj->bound->max.y - 1.0;
    if ( obj->bound->max.y < elmt_val
      || ( obj->bound->max.y == elmt_val && obj->index < elmt_i ) )
    {
	/* Need to shuffle obj down. */

	/* Take the obj out. */
	obj->y_max.prev->next = obj->y_max.next;
	if ( obj->y_max.next )
	    obj->y_max.next->prev = obj->y_max.prev;

	do
	{
	    if ( elmt_min )
	    {
		unsigned long key = ( obj->index > elmt_i
				    ? ( obj->index << 16 ) | elmt_i
				    : ( elmt_i << 16 ) | obj->index );
		Dim_Overlap_Insert(*head, *tail, key, Y, false);
	    }

	    elmt = elmt->prev;
	    while ( elmt && elmt->obj == ignore )
		elmt = elmt->prev;
	    if ( elmt )
	    {
		elmt_min = ( &(elmt->obj->y_min) == elmt );
		elmt_val = ( elmt_min ? elmt->obj->bound->min.y
				      : elmt->obj->bound->max.y );
		elmt_i = elmt->obj->index;
		if ( elmt->obj->fixed )
		    elmt_i += sim->num_dynamic;
	    }
	    else
		elmt_val = obj->bound->max.y - 1.0;
	} while ( obj->bound->max.y < elmt_val
	       || ( obj->bound->max.y == elmt_val && obj->index < elmt_i ) );

	/* Put obj in after elmt. */
	obj->y_max.next = elmt->next;
	elmt->next = &(obj->y_max);
	obj->y_max.prev = elmt;
	obj->y_max.next->prev = &(obj->y_max);
    }
    else
    {
	elmt = obj->y_max.next;
	while ( elmt && elmt->obj == ignore )
	    elmt = elmt->next;
	if ( elmt )
	{
	    elmt_min = ( &(elmt->obj->y_min) == elmt );
	    elmt_val = ( elmt_min ? elmt->obj->bound->min.y
				  : elmt->obj->bound->max.y );
	    elmt_i = elmt->obj->index;
	    if ( elmt->obj->fixed )
		elmt_i += sim->num_dynamic;
	}
	else
	    elmt_val = obj->bound->max.y + 1.0;
	if ( obj->bound->max.y > elmt_val
	  || ( obj->bound->max.y == elmt_val && obj->index > elmt_i ) )
	{
	    /* Need to shuffle obj up. */

	    /* Take the obj out. */
	    if ( obj->y_max.prev )
		obj->y_max.prev->next = obj->y_max.next;
	    else
		sim->sorted_y = obj->y_max.next;
	    obj->y_max.next->prev = obj->y_max.prev;

	    do
	    {
		if ( elmt_min )
		{
		    unsigned long key = ( obj->index > elmt_i
					? ( obj->index << 16 ) | elmt_i
					: ( elmt_i << 16 ) | obj->index );
		    Dim_Overlap_Insert(*head, *tail, key, Y, true);
		}

		prev_elmt = elmt;
		elmt = elmt->next;
		while ( elmt && elmt->obj == ignore )
		{
		    prev_elmt = elmt;
		    elmt = elmt->next;
		}
		if ( elmt )
		{
		    elmt_min = ( &(elmt->obj->y_min) == elmt );
		    elmt_val = ( elmt_min ? elmt->obj->bound->min.y
					  : elmt->obj->bound->max.y );
		    elmt_i = elmt->obj->index;
		    if ( elmt->obj->fixed )
			elmt_i += sim->num_dynamic;
		}
		else
		    elmt_val = obj->bound->max.y + 1.0;
	    } while ( obj->bound->max.y > elmt_val
		   || ( obj->bound->max.y == elmt_val && obj->index > elmt_i ));

	    /* Put obj in before elmt. */
	    obj->y_max.next = elmt;
	    obj->y_max.prev = prev_elmt;
	    obj->y_max.prev->next = &(obj->y_max);
	    if ( elmt )
		elmt->prev = &(obj->y_max);
	}
    }
}


static void
Sort_Obj_Z(SimulationPtr sim, SimObjectPtr obj, SimObjectPtr ignore,
	   DimOverlapList *head, DimOverlapList *tail)
{
    ObjectList	elmt, prev_elmt;
    bool    	elmt_min;
    double  	elmt_val;
    int		elmt_i;
    bool	max_out = false;

    /* Look for the right place for the left edge. */
    elmt = obj->z_min.prev;
    while ( elmt && elmt->obj == ignore )
	elmt = elmt->prev;
    if ( elmt )
    {
	elmt_min = ( &(elmt->obj->z_min) == elmt );
	elmt_val = ( elmt_min ? elmt->obj->bound->min.z
			      : elmt->obj->bound->max.z );
	elmt_i = elmt->obj->index;
	if ( elmt->obj->fixed )
	    elmt_i += sim->num_dynamic;
    }
    else
	elmt_val = obj->bound->min.z - 1.0;
    if ( obj->bound->min.z < elmt_val
      || ( obj->bound->min.z == elmt_val && obj->index < elmt_i ) )
    {
	/* Need to shuffle obj down. */

	/* Take the obj out. */
	obj->z_min.prev->next = obj->z_min.next;
	obj->z_min.next->prev = obj->z_min.prev;

	do
	{
	    if ( ! elmt_min )
	    {
		unsigned long key = ( obj->index > elmt_i
				    ? ( obj->index << 16 ) | elmt_i
				    : ( elmt_i << 16 ) | obj->index );
		Dim_Overlap_Insert(*head, *tail, key, Z, true);
	    }

	    elmt = elmt->prev;
	    while ( elmt && elmt->obj == ignore )
		elmt = elmt->prev;
	    if ( elmt )
	    {
		elmt_min = ( &(elmt->obj->z_min) == elmt );
		elmt_val = ( elmt_min ? elmt->obj->bound->min.z
				      : elmt->obj->bound->max.z );
		elmt_i = elmt->obj->index;
		if ( elmt->obj->fixed )
		    elmt_i += sim->num_dynamic;
	    }
	    else
		elmt_val = obj->bound->min.z - 1.0;
	} while ( obj->bound->min.z < elmt_val
	       || ( obj->bound->min.z == elmt_val && obj->index < elmt_i ) );

	/* Put obj in after elmt. */
	if ( elmt )
	{
	    obj->z_min.next = elmt->next;
	    elmt->next = &(obj->z_min);
	    obj->z_min.prev = elmt;
	    obj->z_min.next->prev = &(obj->z_min);
	}
	else
	{
	    obj->z_min.next = sim->sorted_z;
	    sim->sorted_z = &(obj->z_min);
	    obj->z_min.prev = NULL;
	    obj->z_min.next->prev = &(obj->z_min);
	}
    }
    else
    {
	elmt = obj->z_min.next;
	while ( elmt && elmt->obj == ignore )
	    elmt = elmt->next;
	if ( elmt == &(obj->z_max) )
	{
	    if ( obj->z_max.next )
		obj->z_max.next->prev = obj->z_max.prev;
	    obj->z_max.prev->next = obj->z_max.next;
	    elmt = obj->z_max.next;
	    max_out = true;
	    while ( elmt && elmt->obj == ignore )
		elmt = elmt->next;
	}
	if ( elmt )
	{
	    elmt_min = ( &(elmt->obj->z_min) == elmt );
	    elmt_val = ( elmt_min ? elmt->obj->bound->min.z
				  : elmt->obj->bound->max.z );
	    elmt_i = elmt->obj->index;
	    if ( elmt->obj->fixed )
		elmt_i += sim->num_dynamic;
	}
	else
	    elmt_val = obj->bound->min.z + 1.0;
	if ( obj->bound->min.z > elmt_val
	  || ( obj->bound->min.z == elmt_val && obj->index > elmt_i ) )
	{
	    /* Need to shuffle obj up. */

	    /* Take the obj out. */
	    /*
	    if ( obj->z_min.next == &(obj->z_max) )
	    {
		obj->z_max.prev = obj->z_min.prev;
		if ( obj->z_min.prev )
		    obj->z_min.prev->next = &(obj->z_max);
		else
		    sim->sorted_z = &(obj->z_max);
	    }
	    else
	    {
		elmt->prev = obj->z_min.prev;
		if ( obj->z_min.prev )
		    obj->z_min.prev->next = elmt;
		else
		    sim->sorted_z = elmt;
	    }
	    */
	    if ( obj->z_min.prev )
		obj->z_min.prev->next = obj->z_min.next;
	    else
		sim->sorted_z = obj->z_min.next;
	    obj->z_min.next->prev = obj->z_min.prev;

	    do
	    {
		if ( ! elmt_min )
		{
		    unsigned long key = ( obj->index > elmt_i
					? ( obj->index << 16 ) | elmt_i
					: ( elmt_i << 16 ) | obj->index );
		    Dim_Overlap_Insert(*head, *tail, key, Z, false);
		}
		else if ( max_out )
		{
		    unsigned long key = ( obj->index > elmt_i
					? ( obj->index << 16 ) | elmt_i
					: ( elmt_i << 16 ) | obj->index );
		    Dim_Overlap_Insert(*head, *tail, key, Z, true);
		}

		prev_elmt = elmt;
		elmt = elmt->next;
		while ( elmt && elmt->obj == ignore )
		{
		    prev_elmt = elmt;
		    elmt = elmt->next;
		}
		if ( elmt == &(obj->z_max) )
		{
		    prev_elmt = obj->z_max.prev;
		    if ( obj->z_max.next )
			obj->z_max.next->prev = obj->z_max.prev;
		    obj->z_max.prev->next = obj->z_max.next;
		    elmt = obj->z_max.next;
		    max_out = true;
		    while ( elmt && elmt->obj == ignore )
		    {
			prev_elmt = elmt;
			elmt = elmt->next;
		    }
		}
		if ( elmt )
		{
		    elmt_min = ( &(elmt->obj->z_min) == elmt );
		    elmt_val = ( elmt_min ? elmt->obj->bound->min.z
					  : elmt->obj->bound->max.z );
		    elmt_i = elmt->obj->index;
		    if ( elmt->obj->fixed )
			elmt_i += sim->num_dynamic;
		}
		else
		    elmt_val = obj->bound->min.z + 1.0;
	    } while ( obj->bound->min.z > elmt_val
		   || ( obj->bound->min.z == elmt_val && obj->index > elmt_i ));

	    /* Put obj in before elmt. */
	    obj->z_min.next = elmt;
	    obj->z_min.prev = prev_elmt;
	    obj->z_min.prev->next = &(obj->z_min);
	    if ( elmt )
		elmt->prev = &(obj->z_min);
	}
    }

    if ( max_out )
    {
	/* Put it back in after the min. */
	obj->z_max.next = obj->z_min.next;
	if ( obj->z_max.next )
	    obj->z_max.next->prev = &(obj->z_max);
	obj->z_max.prev = &(obj->z_min);
	obj->z_min.next = &(obj->z_max);
    }

    /* Look for the right place for the right edge. */
    elmt = obj->z_max.prev;
    while ( elmt && elmt->obj == ignore )
	elmt = elmt->prev;
    if ( elmt )
    {
	elmt_min = ( &(elmt->obj->z_min) == elmt );
	elmt_val = ( elmt_min ? elmt->obj->bound->min.z
			      : elmt->obj->bound->max.z );
	elmt_i = elmt->obj->index;
	if ( elmt->obj->fixed )
	    elmt_i += sim->num_dynamic;
    }
    else
	elmt_val = obj->bound->max.z - 1.0;
    if ( obj->bound->max.z < elmt_val
      || ( obj->bound->max.z == elmt_val && obj->index < elmt_i ) )
    {
	/* Need to shuffle obj down. */

	/* Take the obj out. */
	obj->z_max.prev->next = obj->z_max.next;
	if ( obj->z_max.next )
	    obj->z_max.next->prev = obj->z_max.prev;

	do
	{
	    if ( elmt_min )
	    {
		unsigned long key = ( obj->index > elmt_i
				    ? ( obj->index << 16 ) | elmt_i
				    : ( elmt_i << 16 ) | obj->index );
		Dim_Overlap_Insert(*head, *tail, key, Z, false);
	    }

	    elmt = elmt->prev;
	    while ( elmt && elmt->obj == ignore )
		elmt = elmt->prev;
	    if ( elmt )
	    {
		elmt_min = ( &(elmt->obj->z_min) == elmt );
		elmt_val = ( elmt_min ? elmt->obj->bound->min.z
				      : elmt->obj->bound->max.z );
		elmt_i = elmt->obj->index;
		if ( elmt->obj->fixed )
		    elmt_i += sim->num_dynamic;
	    }
	    else
		elmt_val = obj->bound->max.z - 1.0;
	} while ( obj->bound->max.z < elmt_val
	       || ( obj->bound->max.z == elmt_val && obj->index < elmt_i ) );

	/* Put obj in after elmt. */
	obj->z_max.next = elmt->next;
	elmt->next = &(obj->z_max);
	obj->z_max.prev = elmt;
	obj->z_max.next->prev = &(obj->z_max);
    }
    else
    {
	elmt = obj->z_max.next;
	while ( elmt && elmt->obj == ignore )
	    elmt = elmt->next;
	if ( elmt )
	{
	    elmt_min = ( &(elmt->obj->z_min) == elmt );
	    elmt_val = ( elmt_min ? elmt->obj->bound->min.z
				  : elmt->obj->bound->max.z );
	    elmt_i = elmt->obj->index;
	    if ( elmt->obj->fixed )
		elmt_i += sim->num_dynamic;
	}
	else
	    elmt_val = obj->bound->max.z + 1.0;
	if ( obj->bound->max.z > elmt_val
	  || ( obj->bound->max.z == elmt_val && obj->index > elmt_i ) )
	{
	    /* Need to shuffle obj up. */

	    /* Take the obj out. */
	    if ( obj->z_max.prev )
		obj->z_max.prev->next = obj->z_max.next;
	    else
		sim->sorted_z = obj->z_max.next;
	    obj->z_max.next->prev = obj->z_max.prev;

	    do
	    {
		if ( elmt_min )
		{
		    unsigned long key = ( obj->index > elmt_i
					? ( obj->index << 16 ) | elmt_i
					: ( elmt_i << 16 ) | obj->index );
		    Dim_Overlap_Insert(*head, *tail, key, Z, true);
		}

		prev_elmt = elmt;
		elmt = elmt->next;
		while ( elmt && elmt->obj == ignore )
		{
		    prev_elmt = elmt;
		    elmt = elmt->next;
		}
		if ( elmt )
		{
		    elmt_min = ( &(elmt->obj->z_min) == elmt );
		    elmt_val = ( elmt_min ? elmt->obj->bound->min.z
					  : elmt->obj->bound->max.z );
		    elmt_i = elmt->obj->index;
		    if ( elmt->obj->fixed )
			elmt_i += sim->num_dynamic;
		}
		else
		    elmt_val = obj->bound->max.z + 1.0;
	    } while ( obj->bound->max.z > elmt_val
		   || ( obj->bound->max.z == elmt_val && obj->index > elmt_i ));

	    /* Put obj in before elmt. */
	    obj->z_max.next = elmt;
	    obj->z_max.prev = prev_elmt;
	    obj->z_max.prev->next = &(obj->z_max);
	    if ( elmt )
		elmt->prev = &(obj->z_max);
	}
    }
}


bool
Overlaps_Process(SimulationPtr sim, SimObjectPtr obj, SimObjectPtr ignore,
                 bool predict)
{
    DimOverlapList  overlap_head = NULL;
    DimOverlapList  overlap_tail = NULL;

    Sort_Obj_X(sim, obj, ignore, &overlap_head, &overlap_tail);
    Sort_Obj_Y(sim, obj, ignore, &overlap_head, &overlap_tail);
    Sort_Obj_Z(sim, obj, ignore, &overlap_head, &overlap_tail);

    if ( ( sim->debug_flags & SIM_SORT_PRINT ) && ! ignore )
    {
	fprintf(stderr, "Process\n");
	Sort_Print(sim);
    }
    if ( ( sim->debug_flags & SIM_SORT_DEBUG ) && ! ignore )
	Sort_Check(sim);

    /* Process all the modified overlaps */
    return Process_Mod_Overlaps(sim, predict, overlap_head, overlap_tail);
}


static void
Sort_X_Full(SimulationPtr sim, DimOverlapList *head, DimOverlapList *tail)
{
    ObjectList	test_elmt;
    bool	test_min;
    double	test_val;
    int		test_i;
    ObjectList	elmt;
    bool	elmt_min;
    double	elmt_val;
    int		elmt_i;
    ObjectList	next_elmt;

    if ( ! sim->sorted_x )
	return;

    test_elmt = sim->sorted_x->next;
    while ( test_elmt )
    {
	test_min = ( &(test_elmt->obj->x_min) == test_elmt );
	test_val = ( test_min ? test_elmt->obj->bound->min.x
			      : test_elmt->obj->bound->max.x );
	test_i = test_elmt->obj->index;
	if ( test_elmt->obj->fixed )
	    test_i += sim->num_dynamic;

	elmt = test_elmt->prev;
	elmt_min = ( &(elmt->obj->x_min) == elmt );
	elmt_val = ( elmt_min ? elmt->obj->bound->min.x
			      : elmt->obj->bound->max.x );
	elmt_i = elmt->obj->index;
	if ( elmt->obj->fixed )
	    elmt_i += sim->num_dynamic;

	next_elmt = test_elmt->next;

	while ( test_val < elmt_val
	     || ( test_val == elmt_val && test_i < elmt_i ) )
	{
	    /* Must do a swap. */

	    if ( test_min && ! elmt_min )
	    {
		unsigned long key = ( test_i > elmt_i
				    ? ( test_i << 16 ) | elmt_i
				    : ( elmt_i << 16 ) | test_i );
		Dim_Overlap_Insert(*head, *tail, key, X, true);
	    }
	    else if ( ! test_min && elmt_min )
	    {
		unsigned long key = ( test_i > elmt_i
				    ? ( test_i << 16 ) | elmt_i
				    : ( elmt_i << 16 ) | test_i );
		Dim_Overlap_Insert(*head, *tail, key, X, false);
	    }

	    elmt->next = test_elmt->next;
	    if ( elmt->next )
		elmt->next->prev = elmt;
	    test_elmt->prev = elmt->prev;
	    if ( test_elmt->prev )
		test_elmt->prev->next = test_elmt;
	    else
		sim->sorted_x = test_elmt;
	    test_elmt->next = elmt;
	    elmt->prev = test_elmt;

	    elmt = test_elmt->prev;
	    if ( ! elmt )
		break;
	    elmt_min = ( &(elmt->obj->x_min) == elmt );
	    elmt_val = ( elmt_min ? elmt->obj->bound->min.x
				  : elmt->obj->bound->max.x );
	    elmt_i = elmt->obj->index;
	    if ( elmt->obj->fixed )
		elmt_i += sim->num_dynamic;
	}

	test_elmt = next_elmt;
    }
}


static void
Sort_Y_Full(SimulationPtr sim, DimOverlapList *head, DimOverlapList *tail)
{
    ObjectList	test_elmt;
    bool	test_min;
    double	test_val;
    int		test_i;
    ObjectList	elmt;
    bool	elmt_min;
    double	elmt_val;
    int		elmt_i;
    ObjectList	next_elmt;

    if ( ! sim->sorted_y )
	return;

    test_elmt = sim->sorted_y->next;
    while ( test_elmt )
    {
	test_min = ( &(test_elmt->obj->y_min) == test_elmt );
	test_val = ( test_min ? test_elmt->obj->bound->min.y
			      : test_elmt->obj->bound->max.y );
	test_i = test_elmt->obj->index;
	if ( test_elmt->obj->fixed )
	    test_i += sim->num_dynamic;

	elmt = test_elmt->prev;
	elmt_min = ( &(elmt->obj->y_min) == elmt );
	elmt_val = ( elmt_min ? elmt->obj->bound->min.y
			      : elmt->obj->bound->max.y );
	elmt_i = elmt->obj->index;
	if ( elmt->obj->fixed )
	    elmt_i += sim->num_dynamic;

	next_elmt = test_elmt->next;

	while ( test_val < elmt_val
	     || ( test_val == elmt_val && test_i < elmt_i ) )
	{
	    /* Must do a swap. */

	    if ( test_min && ! elmt_min )
	    {
		unsigned long key = ( test_i > elmt_i
				    ? ( test_i << 16 ) | elmt_i
				    : ( elmt_i << 16 ) | test_i );
		Dim_Overlap_Insert(*head, *tail, key, Y, true);
	    }
	    else if ( ! test_min && elmt_min )
	    {
		unsigned long key = ( test_i > elmt_i
				    ? ( test_i << 16 ) | elmt_i
				    : ( elmt_i << 16 ) | test_i );
		Dim_Overlap_Insert(*head, *tail, key, Y, false);
	    }

	    elmt->next = test_elmt->next;
	    if ( elmt->next )
		elmt->next->prev = elmt;
	    test_elmt->prev = elmt->prev;
	    if ( test_elmt->prev )
		test_elmt->prev->next = test_elmt;
	    else
		sim->sorted_y = test_elmt;
	    test_elmt->next = elmt;
	    elmt->prev = test_elmt;

	    elmt = test_elmt->prev;
	    if ( ! elmt )
		break;
	    elmt_min = ( &(elmt->obj->y_min) == elmt );
	    elmt_val = ( elmt_min ? elmt->obj->bound->min.y
				  : elmt->obj->bound->max.y );
	    elmt_i = elmt->obj->index;
	    if ( elmt->obj->fixed )
		elmt_i += sim->num_dynamic;
	}

	test_elmt = next_elmt;
    }
}


static void
Sort_Z_Full(SimulationPtr sim, DimOverlapList *head, DimOverlapList *tail)
{
    ObjectList	test_elmt;
    bool	test_min;
    double	test_val;
    int		test_i;
    ObjectList	elmt;
    bool	elmt_min;
    double	elmt_val;
    int		elmt_i;
    ObjectList	next_elmt;

    if ( ! sim->sorted_z )
	return;

    test_elmt = sim->sorted_z->next;
    while ( test_elmt )
    {
	test_min = ( &(test_elmt->obj->z_min) == test_elmt );
	test_val = ( test_min ? test_elmt->obj->bound->min.z
			      : test_elmt->obj->bound->max.z );
	test_i = test_elmt->obj->index;
	if ( test_elmt->obj->fixed )
	    test_i += sim->num_dynamic;

	elmt = test_elmt->prev;
	elmt_min = ( &(elmt->obj->z_min) == elmt );
	elmt_val = ( elmt_min ? elmt->obj->bound->min.z
			      : elmt->obj->bound->max.z );
	elmt_i = elmt->obj->index;
	if ( elmt->obj->fixed )
	    elmt_i += sim->num_dynamic;

	next_elmt = test_elmt->next;

	while ( test_val < elmt_val
	     || ( test_val == elmt_val && test_i < elmt_i ) )
	{
	    /* Must do a swap. */

	    if ( test_min && ! elmt_min )
	    {
		unsigned long key = ( test_i > elmt_i
				    ? ( test_i << 16 ) | elmt_i
				    : ( elmt_i << 16 ) | test_i );
		Dim_Overlap_Insert(*head, *tail, key, Z, true);
	    }
	    else if ( ! test_min && elmt_min )
	    {
		unsigned long key = ( test_i > elmt_i
				    ? ( test_i << 16 ) | elmt_i
				    : ( elmt_i << 16 ) | test_i );
		Dim_Overlap_Insert(*head, *tail, key, Z, false);
	    }

	    elmt->next = test_elmt->next;
	    if ( elmt->next )
		elmt->next->prev = elmt;
	    test_elmt->prev = elmt->prev;
	    if ( test_elmt->prev )
		test_elmt->prev->next = test_elmt;
	    else
		sim->sorted_z = test_elmt;
	    test_elmt->next = elmt;
	    elmt->prev = test_elmt;

	    elmt = test_elmt->prev;
	    if ( ! elmt )
		break;
	    elmt_min = ( &(elmt->obj->z_min) == elmt );
	    elmt_val = ( elmt_min ? elmt->obj->bound->min.z
				  : elmt->obj->bound->max.z );
	    elmt_i = elmt->obj->index;
	    if ( elmt->obj->fixed )
		elmt_i += sim->num_dynamic;
	}

	test_elmt = next_elmt;
    }
}


bool
Overlaps_Process_Full(SimulationPtr sim, bool predict)
{
    DimOverlapList  overlap_head = NULL;
    DimOverlapList  overlap_tail = NULL;

    Sort_X_Full(sim, &overlap_head, &overlap_tail);
    Sort_Y_Full(sim, &overlap_head, &overlap_tail);
    Sort_Z_Full(sim, &overlap_head, &overlap_tail);

    if ( sim->debug_flags & SIM_SORT_PRINT )
    {
	fprintf(stderr, "Process\n");
	Sort_Print(sim);
    }
    if ( sim->debug_flags & SIM_SORT_DEBUG )
	Sort_Check(sim);

    /* Process all the modified overlaps */
    return Process_Mod_Overlaps(sim, predict, overlap_head, overlap_tail);
}



bool
Overlaps_Update(SimulationPtr sim, SimObjectPtr obj, SimObjectPtr excl_dist,
		SimObjectPtr excl_event)
{
    SimObjectPtr  other;
    OverlapList	    l;

    for ( l = ((DynObjectPtr)obj->detail)->xyz_overlaps ; l ; l = l->next )
    {
	other = l->overlap->o1 == obj ? l->overlap->o2 : l->overlap->o1;
	if ( other != excl_event )
	{
	    unsigned char   prediction =
			 Update_Prediction(sim, l->overlap, other != excl_dist);
	    if ( prediction == PENETRATING
	      || prediction == INTEG_ERROR
	      || prediction == INTERFERENCE )
		return false;
	}
    }

    return true;
}


void
Overlaps_Object_Start(SimulationPtr sim, SimObjectPtr obj)
{
    obj->x_max.next = sim->sorted_x;
    if ( obj->x_max.next )
	obj->x_max.next->prev = &(obj->x_max);
    obj->x_max.prev = &(obj->x_min);
    obj->x_min.next = &(obj->x_max);
    obj->x_min.prev = NULL;
    sim->sorted_x = &(obj->x_min);

    obj->y_max.next = sim->sorted_y;
    if ( obj->y_max.next )
	obj->y_max.next->prev = &(obj->y_max);
    obj->y_max.prev = &(obj->y_min);
    obj->y_min.next = &(obj->y_max);
    obj->y_min.prev = NULL;
    sim->sorted_y = &(obj->y_min);

    obj->z_max.next = sim->sorted_z;
    if ( obj->z_max.next )
	obj->z_max.next->prev = &(obj->z_max);
    obj->z_max.prev = &(obj->z_min);
    obj->z_min.next = &(obj->z_max);
    obj->z_min.prev = NULL;
    sim->sorted_z = &(obj->z_min);
}


