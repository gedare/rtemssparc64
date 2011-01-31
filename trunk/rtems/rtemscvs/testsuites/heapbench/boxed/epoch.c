/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/epoch.c,v 1.1 2001/05/01 21:23:09 emery Exp $
 *
 * $Log: epoch.c,v $
 * Revision 1.1  2001/05/01 21:23:09  emery
 * Initial check-in, works under Windows.
 *
 *
 */

#include <simulator.h>

typedef struct _Interaction {
    double  	    t;
    SimObjectPtr    obj;
    } Interaction, *InteractionPtr;


static ObjectList	free_objects = NULL;
static DynamicEpochList	free_dynamic = NULL;
static InteractionPtr	free_interactions = NULL;


#define Interaction_New(d)  { \
	    if ( free_interactions ) \
	    { \
		(d) = free_interactions; \
		free_interactions = (InteractionPtr)free_interactions->obj; \
	    } \
	    else \
		(d) = New(Interaction, 1); \
	}

#define Interaction_Free(v) { \
	    (v)->obj = (SimObjectPtr)free_interactions; \
	    free_interactions = (v); \
	}


void
Interactions_Free(ObjectList victim)
{
    ObjectList	ol;

    for ( ol = victim ; ol && ol->next ; ol = ol->next );
    if ( ol )
    {
	ol->next = free_objects;
	free_objects = victim;
    }
}


DynamicEpochList
Dynamic_Epoch_New()
{
    DynamicEpochList	res;

    if ( free_dynamic )
    {
	res = free_dynamic;
	free_dynamic = free_dynamic->next;
    }
    else
	res = New(DynamicEpoch, 1);

    res->interactions = NULL;
    res->checkpoints_head = res->checkpoints_tail = NULL;

    res->next = res->prev = NULL;

    return res;
}


void
Dynamic_Epoch_Free(DynamicEpochList e)
{
    Interactions_Free(e->interactions);

    e->next = free_dynamic;
    free_dynamic = e;
}


void
Dynamic_Epoch_Free_List(DynamicEpochList e)
{
    DynamicEpochList	el = e;
    for ( el = e ; el && el->next ; el = el->next )
    {
	Interactions_Free(el->interactions);
	Checkpoints_Free(el);
    }

    if ( el )
    {
	Interactions_Free(el->interactions);
	Checkpoints_Free(el);
	el->next = free_dynamic;
	free_dynamic = e;
    }
}


void
Dynamic_Epoch_Copy_List(DynObjectPtr in, DynObjectPtr out,
			SimulationPtr sim_out)
{
    ObjectList	    l_in, l_out;

    out->epoch_current = NULL;

    for ( in->epoch_current = in->epoch_head ;
	  in->epoch_current ;
	  in->epoch_current = in->epoch_current->next )
    {
	if ( out->epoch_current )
	{
	    out->epoch_current->next = Dynamic_Epoch_New();
	    out->epoch_current->next->prev = out->epoch_current;
	    out->epoch_current = out->epoch_current->next;
	}
	else
	{
	    out->epoch_current = out->epoch_head = Dynamic_Epoch_New();
	    out->epoch_head->next = out->epoch_head->prev = NULL;
	}

	out->epoch_current->t = in->epoch_current->t;
	out->epoch_current->state = in->epoch_current->state;
	out->epoch_current->bound = in->epoch_current->bound;

	l_out = NULL;
	for ( l_in = in->epoch_current->interactions ; l_in ; l_in=l_in->next )
	{
	    if ( l_out )
	    {
		if ( free_objects )
		{
		    l_out->next = free_objects;
		    free_objects = free_objects->next;
		}
		else
		    l_out->next = New(ObjectListElmt, 1);
		l_out->next->prev = l_out;
		l_out = l_out->next;
		l_out->next = NULL;
	    }
	    else
	    {
		if ( free_objects )
		{
		    l_out = out->epoch_current->interactions = free_objects;
		    free_objects = free_objects->next;
		}
		else
		    l_out =
		    out->epoch_current->interactions = New(ObjectListElmt, 1);
		l_out->next = l_out->prev = NULL;
	    }

	    l_out->obj = l_in->obj->fixed
	               ? sim_out->fixed_objects[l_in->obj->index]
	               : sim_out->dyn_objects[l_in->obj->index];
	}

	Checkpoints_Copy(in->epoch_current, out->epoch_current);
    }

    in->epoch_current = in->epoch_head;
}


void
Epoch_Add_Dyn_Interaction(DynObjectPtr dyn, SimObjectPtr obj)
{
    ObjectList	l;

    if ( obj->fixed )
	return;

    if ( ! dyn->epoch_current->interactions )
    {
	if ( free_objects )
	{
	    dyn->epoch_current->interactions = free_objects;
	    free_objects = free_objects->next;
	}
	else
	    dyn->epoch_current->interactions = New(ObjectListElmt, 1);

	dyn->epoch_current->interactions->obj = obj;
	dyn->epoch_current->interactions->next = NULL;
	dyn->epoch_current->interactions->prev = NULL;

	return;
    }

    for ( l = dyn->epoch_current->interactions ; l->next ; l = l->next )
    {
	if ( l->obj == obj )
	    return;
    }

    if ( l->obj == obj )
	return;

    if ( free_objects )
    {
	l->next = free_objects;
	free_objects = free_objects->next;
    }
    else
	l->next = New(ObjectListElmt, 1);

    l->next->next = NULL;
    l->next->prev = l;
    l = l->next;
    l->obj = obj;
}


void
Epoch_Add_Fixed_Interaction(FixedObjectPtr fixed, SimObjectPtr obj, double t)
{
    InteractionPtr  inter =
	(InteractionPtr)RBTree_Search(fixed->interactions, obj->index);

    if ( inter != (InteractionPtr)-1 )
    {
	if ( inter->t > t )
	    inter->t = t;
	return;
    }

    Interaction_New(inter);
    inter->t = t;
    inter->obj = obj;

    RBTree_Insert(fixed->interactions, (unsigned long)obj->index, (void*)inter);
}


static bool
Next_Dynamic_Epoch(SimulationPtr sim, SimObjectPtr obj, double t)
{
    DynObjectPtr    dyn = (DynObjectPtr)obj->detail;
    bool	    was_stable = dyn->epoch_current->state.stable;
    BoundingBox	    old_epoch_bound, old_obj_bound;

    if ( obj->changed || ! dyn->epoch_current->next )
    {
	if ( ( sim->debug_flags & SIM_PEDANTIC_DEBUG )
	  && ! obj->changed && ! dyn->state.stable )
	{
	    fprintf(stderr, "Object not changed and not stable.\n");
	    abort();
	}

	/* This gets the state right and does the final bound update. */
	if ( obj->changed
	  && ! Integrate_Object(sim, &(dyn->state), &(dyn->state), t, obj) )
	{
	    sim->error_flags |= SIM_INTEGRATE_ERR;
	    return false;
	}

	if ( sim->debug_flags & SIM_EPOCH_BOUND_DEBUG )
	{
	    old_epoch_bound = dyn->epoch_current->bound;
	    old_obj_bound = *(obj->bound);
	}

	dyn->epoch_current->next = Dynamic_Epoch_New();
	dyn->epoch_current->next->prev = dyn->epoch_current;
	dyn->epoch_current = dyn->epoch_current->next;

	dyn->epoch_current->t = t;
	dyn->epoch_current->state = dyn->state;
	Bound_Set_Stable(sim, obj, &(dyn->state.x),
			 &(dyn->state.o), &(dyn->epoch_current->bound));
    }
    else /* Still unchanged from before. */
    {
	if ( sim->debug_flags & SIM_EPOCH_BOUND_DEBUG )
	{
	    old_epoch_bound = dyn->epoch_current->bound;
	    old_obj_bound = *(obj->bound);
	}

	/* Everything will be the same. */
	dyn->epoch_current = dyn->epoch_current->next;

	dyn->state = dyn->epoch_current->state;
	*(obj->bound) = dyn->epoch_current->bound;
    }

    if ( sim->debug_flags & SIM_EPOCH_BOUND_DEBUG )
    {
    	if ( ! Bound_Contains(&old_epoch_bound, obj->r_max, &(dyn->state)) )
	{
	    fprintf(stderr, "Old epoch bound doesn't contain object\n");
	    abort();
	}
	if ( ! Bound_Contains(&old_obj_bound, obj->r_max, &(dyn->state)) )
	{
	    fprintf(stderr, "Old obj bound doesn't contain object\n");
	    abort();
	}
	if ( ! Bound_Contains(&(dyn->epoch_current->bound), obj->r_max,
			      &(dyn->state)) )
	{
	    fprintf(stderr, "New epoch bound doesn't contain object\n");
	    abort();
	}
	if ( ! Bound_Contains(obj->bound, obj->r_max, &(dyn->state)) )
	{
	    fprintf(stderr, "New obj bound doesn't contain object\n");
	    abort();
	}
    }

    if ( dyn->epoch_current->state.stable )
    {
	if ( ! was_stable )
	    sim->num_stable++;
    }
    else
    {
	if ( was_stable )
	    sim->num_stable--;
    }

    return true;
}


bool
Epoch_Process_Activate(SimulationPtr sim, SimObjectPtr obj, double t)
{
    DynObjectPtr    	dyn = (DynObjectPtr)obj->detail;
    bool	    	was_stable;
    DynamicEpochList	el;
    ObjectList	    	ol;

    /* Check if already activated. */
    if ( obj->changed )
	return true;

    if ( sim->debug_flags & SIM_ACTIVATE_DEBUG )
	fprintf(stderr, "Activate %s at %g\n", obj->name, t);

    if ( obj->start_index >= sim->num_started )
    {
	/* Not started, let the start routine do all the work. */
	obj->changed = true;
	return true;
    }

    was_stable = dyn->epoch_current->state.stable;

    if ( (sim->debug_flags & SIM_PEDANTIC_DEBUG) && dyn->epoch_current->t > t )
    {
	fprintf(stderr, "Activate earlier than current epoch time\n");
	abort();
    }

    /* Wind back the simulation. */
    sim->current_event_t = dyn->epoch_current->t;

    /* Deal with all the objects this one was expected to interact with,
    ** and delete the tail of its epoch list.
    */
    for ( ol = dyn->epoch_current->interactions ; ol ; ol = ol->next )
	if ( ! ol->obj->changed )
	    Event_Activate_Insert(sim, ol->obj, dyn->epoch_current->t);
    Interactions_Free(dyn->epoch_current->interactions);
    Checkpoints_Free(dyn->epoch_current);
    dyn->epoch_current->interactions = NULL;
    for ( el = dyn->epoch_current->next ; el ; el = el->next )
    {
	for ( ol = el->interactions ; ol ; ol = ol->next )
	    if ( ! ol->obj->changed )
		Event_Activate_Insert(sim, ol->obj, el->t);
    }
    Dynamic_Epoch_Free_List(dyn->epoch_current->next);
    dyn->epoch_current->next = NULL;

    /* Copy state over. */
    dyn->state = dyn->epoch_current->state;

    /* Reset the bound for this epoch. */
    Bound_Set_Stable(sim, obj, &(dyn->state.x), &(dyn->state.o),
		     &(dyn->epoch_current->bound));

    /* Change the bound and insert update events. */
    if ( ! dyn->state.stable )
    {
	dyn->state.last_event_t = dyn->state.t - 0.1;
	Update_Bound(sim, obj, dyn->state.t, true);
	dyn->state.last_event_t = dyn->state.t;
    }
    else
	Bound_Set_Stable(sim, obj, &(dyn->state.x), &(dyn->state.o),obj->bound);

    Checkpoint_Insert_Event(sim, obj, dyn->state.t);

    obj->changed = true;

    /* Do stability. */
    if ( dyn->epoch_current->state.stable )
    {
	if ( ! was_stable )
	    sim->num_stable++;
    }
    else
    {
	if ( was_stable )
	    sim->num_stable--;
    }

    /* Process overlaps, not making predictions. */
    /* Update all overlaps for this object. */
    return Overlaps_Process(sim, obj, NULL, false)
	&& Overlaps_Update(sim, obj, NULL, NULL);
}


bool
Epoch_Process_Epoch(SimulationPtr sim, double t)
{
    int	i;

    sim->epoch_time = t;

    for ( i = 0 ; i < sim->num_started ; i++ )
    {
	if ( ! Next_Dynamic_Epoch(sim, sim->dyn_objects[sim->started[i]], t) )
	    return false;
    }

    if ( Overlaps_Process_Full(sim, true) )
    {
	sim->epoch_event.t = t + sim->epoch_interval;
	Event_Epoch_Insert(sim);

	return true;
    }

    return false;
}


static void*
Copy_Func(unsigned long key, void *value, void *data)
{
    InteractionPtr  dest;
    InteractionPtr  src = (InteractionPtr)value;
    SimulationPtr   sim = (SimulationPtr)data;

    Interaction_New(dest);

    dest->t = src->t;
    dest->obj = sim->dyn_objects[src->obj->index];

    return (void*)dest;
}


void
Fixed_Interactions_Copy(FixedObjectPtr src, FixedObjectPtr dest,
			SimulationPtr sim)
{
    RBTree_Copy(src->interactions, dest->interactions, Copy_Func, (void*)sim);
}


static void
Activate_Func(unsigned long key, void *value, void *data)
{
    InteractionPtr  inter = (InteractionPtr)value;

    if ( ! inter->obj->fixed && ! inter->obj->changed )
	Event_Activate_Insert((SimulationPtr)data, inter->obj, inter->t);
}


static void
Clear_Func(void *value, void *data)
{
    Interaction_Free((InteractionPtr)value);
}


void
Fixed_Interactions_Clear(FixedObjectPtr fixed)
{
    RBTree_Clear(fixed->interactions, Clear_Func, NULL);
}


static void
Epoch_Reset_Fixed(SimulationPtr sim, SimObjectPtr obj)
{
    FixedObjectPtr	fixed = (FixedObjectPtr)obj->detail;

    if ( obj->changed )
	RBTree_Apply(fixed->interactions, Activate_Func, (void*)sim);

    Fixed_Interactions_Clear(fixed);
}


static void
Epoch_Reset_Dynamic(SimObjectPtr obj)
{
    DynObjectPtr    dyn = (DynObjectPtr)obj->detail;

    dyn->activate_event.type = activate_event;
    dyn->activate_event.queue_entry = NULL;
    dyn->activate_event.obj = obj;

    dyn->checkpoint_event.type = checkpoint_event;
    dyn->checkpoint_event.queue_entry = NULL;
    dyn->checkpoint_event.obj = obj;
}


void
Epoch_Reset(SimulationPtr sim)
{
    int	i;

    for ( i = 0 ; i < sim->num_fixed ; i++ )
	Epoch_Reset_Fixed(sim, sim->fixed_objects[i]);

    for ( i = 0 ; i < sim->num_dynamic ; i++ )
	Epoch_Reset_Dynamic(sim->dyn_objects[i]);

    if ( sim->epoch_interval > 0.0 )
    {
	sim->epoch_event.type = epoch_event;
	sim->epoch_event.t = sim->start_time;
	sim->epoch_event.queue_entry = NULL;
	sim->epoch_event.obj = NULL;

	Event_Epoch_Insert(sim);
    }
}


void
Epoch_Object_Start(SimulationPtr sim, SimObjectPtr obj)
{
    DynObjectPtr    dyn = (DynObjectPtr)obj->detail;

    if ( obj->changed )
    {
	/* Schedule activation events for all the objects this one previously
	** interacted with.
	*/
	for ( dyn->epoch_current = dyn->epoch_head ;
	      dyn->epoch_current ;
	      dyn->epoch_current = dyn->epoch_current->next )
	{
	    ObjectList	ol;

	    for ( ol = dyn->epoch_current->interactions ; ol ; ol = ol->next )
	      	if ( ! ol->obj->changed )
		    Event_Activate_Insert(sim, ol->obj, dyn->epoch_current->t);
	}
	Dynamic_Epoch_Free_List(dyn->epoch_head);

	dyn->epoch_head =
	dyn->epoch_current = Dynamic_Epoch_New();

	dyn->epoch_current->t = sim->epoch_time;
	dyn->epoch_current->state = dyn->state;
	Bound_Set_Stable(sim, obj, &(dyn->state.x), &(dyn->state.o),
			 &(dyn->epoch_current->bound));

	dyn->state.last_event_t = dyn->state.t - 0.1;
	Update_Bound(sim, obj, dyn->state.t, true);
	dyn->state.last_event_t = dyn->state.t;
    }
    else
    {
	/* All old epochs must be right. */
	dyn->epoch_current = dyn->epoch_head;
	dyn->state = dyn->epoch_head->state;
	*(obj->bound) = dyn->epoch_head->bound;
    }
}


