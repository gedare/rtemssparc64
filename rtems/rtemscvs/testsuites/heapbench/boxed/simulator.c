/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/simulator.c,v 1.1 2001/05/01 21:23:19 emery Exp $
 *
 * $Log: simulator.c,v $
 * Revision 1.1  2001/05/01 21:23:19  emery
 * Initial check-in, works under Windows.
 *
 *
 */

#include <boxed.h>
#include <simulator.h>
#include <string.h>

static unsigned int  global_count = 0;


void
Update_Bound(SimulationPtr sim, SimObjectPtr obj, double t, bool do_interval)
{
    DynObjectPtr    d = (DynObjectPtr)obj->detail;

    if ( do_interval )
    {
	double	interval = ( t - d->state.last_event_t ) * 2.0;
	if ( interval < ABS_MIN_STEP )
	    interval = ABS_MIN_STEP;
	d->state.next_update_t = d->state.t + interval;
	Bound_Set_Ballistic(sim, obj->r_max, &(d->state), interval, obj->bound);
    }

    d->update_event.t = d->state.next_update_t;
    if ( d->update_event.queue_entry )
	Event_Change(sim, (SimEventPtr)&(d->update_event));
    else
	Event_Insert(sim, (SimEventPtr)&(d->update_event));
}


int
Update_Prediction(SimulationPtr sim, OverlapPtr overlap, bool do_dist)
{
    DynamicStatePtr s1, s2;
    int		    prediction;

    if ( ! overlap->o1->changed )
    {
	if ( ! overlap->o2->changed )
	{
	    /* Won't change */
	    if ( overlap->queue_entry )
		Event_Delete(sim, (SimEventPtr)overlap);
	    overlap->type = no_event;
	    overlap->min_t = 1.0e20;
	    return WONT_HIT;
	}
	else if ( ! overlap->o1->fixed )
	{
	    /* Activate o1 */
	    Event_Activate_Insert(sim, overlap->o1, sim->current_event_t);

	    if ( overlap->queue_entry )
		Event_Delete(sim, (SimEventPtr)overlap);
	    overlap->type = no_event;
	    overlap->min_t = 1.0e20;
	    return WONT_HIT;
	}
    }
    else if ( ! overlap->o2->changed && ! overlap->o2->fixed )
    {
	/* Activate o2 */
	Event_Activate_Insert(sim, overlap->o2, sim->current_event_t);

	if ( overlap->queue_entry )
	    Event_Delete(sim, (SimEventPtr)overlap);
	overlap->type = no_event;
	overlap->min_t = 1.0e20;
	return WONT_HIT;
    }

    s1 = overlap->o1->fixed ? &(((FixedObjectPtr)overlap->o1->detail)->state)
			    : &(((DynObjectPtr)overlap->o1->detail)->state);
    s2 = overlap->o2->fixed ? &(((FixedObjectPtr)overlap->o2->detail)->state)
			    : &(((DynObjectPtr)overlap->o2->detail)->state);

    if ( s1->t > s2->t && s1->t > sim->current_event_t )
    {
	/* Put it off until time catches up. */
	overlap->type = pairwise_event;
	overlap->min_t = s1->t;
	if ( overlap->queue_entry )
	    Event_Change(sim, (SimEventPtr)overlap);
	else
	    Event_Insert(sim, (SimEventPtr)overlap);
	return WONT_HIT;
    }
    else if ( s2->t > s1->t && s2->t > sim->current_event_t )
    {
	/* Put it off until time catches up. */
	overlap->type = pairwise_event;
	overlap->min_t = s2->t;
	if ( overlap->queue_entry )
	    Event_Change(sim, (SimEventPtr)overlap);
	else
	    Event_Insert(sim, (SimEventPtr)overlap);
	return WONT_HIT;
    }

    switch ( ( prediction = Pair_Predict(sim, overlap, do_dist) ) )
    {
      case WILL_HIT:
	if ( overlap->queue_entry )
	    Event_Change(sim, (SimEventPtr)overlap);
	else
	{
	    overlap->type = pairwise_event;
	    Event_Insert(sim, (SimEventPtr)overlap);
	}
	break;

      case WONT_HIT:
	if ( overlap->queue_entry )
	    Event_Delete(sim, (SimEventPtr)overlap);
	overlap->type = no_event;
	overlap->min_t = 1.0e20;
	break;

      case INTEG_ERROR:
	sim->error_flags |= SIM_INTEGRATE_ERR;
	break;

      case PENETRATING:
	sim->error_flags |= SIM_PENETRATE_ERR;
	break;

      case INTERFERENCE:
	sim->error_flags |= SIM_INTERFERE_ERR;
	break;
    }

    return prediction;
}


static bool
Start_Object(SimulationPtr sim, SimObjectPtr obj)
{
    DynObjectPtr    dyn = (DynObjectPtr)obj->detail;

    sim->started[obj->start_index] = sim->started[sim->num_started];
    sim->dyn_objects[sim->started[obj->start_index]]->start_index =
							    obj->start_index;
    sim->started[sim->num_started] = obj->index;
    obj->start_index = sim->num_started;
    sim->num_started++;

    if ( sim->debug_flags & SIM_EVENTS_DEBUG )
	fprintf(stderr, "Object %s is index %d\n", obj->name, obj->index);

    dyn->state = dyn->init.state;

    Overlaps_Object_Start(sim, obj);

    Epoch_Object_Start(sim, obj);

    Checkpoint_Object_Start(sim, obj);

    return true;
}


static void
Set_Stability(SimulationPtr sim, SimObjectPtr obj, Vector n, Vector r, double t)
{
    DynObjectPtr    dyn = (DynObjectPtr)obj->detail;
    double	    v_sq;
    bool    	    this_stable;
    Vector  	    cross;
    double  	    d;

    VCross(n, r, cross);
    d = VDot(cross, cross);

    v_sq = sim->min_velocity * sim->min_velocity;
    this_stable = ( VDot(dyn->state.v, dyn->state.v)<v_sq * sim->stable_range
                 && VDot(dyn->state.w, dyn->state.w)<v_sq/d*sim->stable_range );

    if ( this_stable )
    {
	dyn->state.stable_steps++;
	if ( dyn->state.stable_steps > MIN_STABLE_STEPS )
	{
	    if ( ! dyn->state.stable )
	    {
		dyn->state.stable = true;
		if ( dyn->update_event.queue_entry )
		    Event_Delete(sim, (SimEventPtr)&(dyn->update_event));
		VNew(0.0, 0.0, 0.0, dyn->state.v);
		VNew(0.0, 0.0, 0.0, dyn->state.w);

		if ( sim->epoch_interval <= 0.0 )
		    sim->num_stable++;
	    }
	}
    }
    else
    {
	dyn->state.stable_steps = 0;
	if ( dyn->state.stable )
	{
	    dyn->state.stable = false;
	    dyn->state.t = t;
	    dyn->state.last_event_t = t - 0.1;
	    Checkpoint_Insert_Event(sim, obj, t);

	    if ( sim->epoch_interval <= 0.0 )
		sim->num_stable--;
	}
    }

    if ( sim->debug_flags & SIM_STABLE_DEBUG )
	fprintf(stderr, "%s %s: %d: %g %g : %g %g : %d\n",
		this_stable ? "  stable" : "unstable",
		dyn->state.stable ? "  stable" : "unstable",
		dyn->state.stable_steps,
		VDot(dyn->state.v, dyn->state.v), v_sq * sim->stable_range,
		VDot(dyn->state.w, dyn->state.w), v_sq/d * sim->stable_range,
		sim->num_stable);
}




static bool
Process_Fixed(SimulationPtr sim, double t, OverlapPtr overlap)
{
    SimObjectPtr    dynamic = overlap->o1;
    DynObjectPtr    dyn = (DynObjectPtr)dynamic->detail;
    SimObjectPtr    fixed = overlap->o2;
    ComponentPtr    c_dyn = dynamic->components + overlap->o1_close_comp;
    ComponentPtr    c_fix = fixed->components + overlap->o2_close_comp;
    Vector  	    r1, r2;
    Vector  	    w1, w2;
    Vector	    norm;
    double	    un;

    if ( ! Integrate_Object(sim, &(dyn->state), &(dyn->state), t, dynamic) )
    {
	sim->error_flags |= SIM_INTEGRATE_ERR;
	return false;
    }

    switch ( Pair_Colliding(sim, overlap, &r1, &w1, &r2, &w2, &norm, &un) )
    {
      case COLLIDING:
	if ( sim->debug_flags & SIM_EVENTS_DEBUG )
	    fprintf(stderr, "Colliding\n");
	if ( sim->epoch_interval > 0.0 )
	{
	    Epoch_Add_Fixed_Interaction((FixedObjectPtr)fixed->detail,
					dynamic, t);
	    Epoch_Add_Dyn_Interaction(dyn, fixed);
	}

        if ( ! Collide_Move_Fixed(sim, norm, un,
			          c_dyn->restitution * c_fix->restitution,
			          c_dyn->friction * c_fix->friction,
			          &(dyn->state), &r1, &w1, dyn->mass,
				  &(dyn->inertia)) )
	{
	    sim->error_flags |= SIM_RESOLVE_ERR;
	    return false;
	}

	Set_Stability(sim, dynamic, norm, r1, t);

	if ( dyn->state.stable )
	    Bound_Set_Stable(sim, dynamic, &(dyn->state.x),
			     &(dyn->state.o), dynamic->bound);
	else
	    Update_Bound(sim, dynamic, t, true);

	dyn->state.last_event_t = t;

	return Overlaps_Process(sim, dynamic, NULL, false)
	    && Overlaps_Update(sim, dynamic, fixed, NULL);

      case NOT_COLLIDING:
    	if ( Update_Prediction(sim, overlap, false) == INTEG_ERROR )
	    return false;
	break;

      case PENETRATING:
	sim->error_flags |= SIM_PENETRATE_ERR;
	return false;

      case INTERFERENCE:
	sim->error_flags |= SIM_INTERFERE_ERR;
	return false;
    }

    return true;
}


static bool
Process_Ballistic(SimulationPtr sim, double t, OverlapPtr overlap)
{
    SimObjectPtr    o1 = overlap->o1;
    SimObjectPtr    o2 = overlap->o2;
    ComponentPtr    c1 = o1->components + overlap->o1_close_comp;
    ComponentPtr    c2 = o2->components + overlap->o2_close_comp;
    DynObjectPtr    d1 = (DynObjectPtr)o1->detail;
    DynObjectPtr    d2 = (DynObjectPtr)o2->detail;
    DynObject	    d1_backup;
    DynObject	    d2_backup;
    Vector          r1, r2;
    Vector	    norm;
    Vector          w1, w2;
    double	    un;

    if ( ! Integrate_Object(sim, &(d1->state), &(d1->state), t, o1) )
    {
	sim->error_flags |= SIM_INTEGRATE_ERR;
	return false;
    }
    if ( ! Integrate_Object(sim, &(d2->state), &(d2->state), t, o2) )
    {
	sim->error_flags |= SIM_INTEGRATE_ERR;
	return false;
    }

    switch ( Pair_Colliding(sim, overlap, &r1, &w1, &r2, &w2, &norm, &un) )
    {
      case COLLIDING:
	if ( sim->debug_flags & SIM_EVENTS_DEBUG )
	    fprintf(stderr, "Colliding\n");
	if ( sim->epoch_interval > 0.0 )
	{
	    Epoch_Add_Dyn_Interaction(d1, o2);
	    Epoch_Add_Dyn_Interaction(d2, o1);
	}

	d1_backup = *d1;
	d2_backup = *d2;
        if ( ! Collide_Move_Move(sim, norm, un,
		          c1->restitution * c2->restitution,
		          c1->friction * c2->friction,
		          &(d1->state), d1->mass, &(d1->inertia), &r1, &w1,
		          &(d2->state), d2->mass, &(d2->inertia), &r2, &w2) )
	{
	    sim->error_flags |= SIM_RESOLVE_ERR;
	    return false;
	}

	Set_Stability(sim, o1, norm, r1, t);
	Set_Stability(sim, o2, norm, r2, t);

	if ( ! ((DynObjectPtr)o1->detail)->state.stable
	  && ((DynObjectPtr)o2->detail)->state.stable )
	{
	    Vector  u;

	    *d1 = d1_backup;

	    /* Must recompute un with other obj fixed. */
	    QVMul(d1->state.o, d1->state.w, w1);
	    VCross(w1, r1, u);
	    VAdd(d1->state.v, u, u);
	    un = VDot(u, norm);

	    if ( ! Collide_Move_Fixed(sim, norm, un,
			   c1->restitution * c2->restitution,
			   c1->friction * c2->friction,
			   &(d1->state), &r1, &w1, d1->mass, &(d1->inertia)) )
	    {
		sim->error_flags |= SIM_RESOLVE_ERR;
		return false;
	    }

	    Set_Stability(sim, o1, norm, r1, t);
	}
	else if ( ((DynObjectPtr)o1->detail)->state.stable
	       && ! ((DynObjectPtr)o2->detail)->state.stable )
	{
	    Vector  u;

	    *d2 = d2_backup;

	    /* Must recompute un with other obj fixed. */
	    QVMul(d2->state.o, d2->state.w, w2);
	    VCross(w2, r2, u);
	    VAdd(d2->state.v, u, u);
	    un = -VDot(u, norm);

	    norm.x = -norm.x;
	    norm.y = -norm.y;
	    norm.z = -norm.z;
	    if ( ! Collide_Move_Fixed(sim, norm, un,
			   c1->restitution * c2->restitution,
			   c1->friction * c2->friction,
			   &(d2->state), &r2, &w2, d2->mass, &(d2->inertia)) )
	    {
		sim->error_flags |= SIM_RESOLVE_ERR;
		return false;
	    }

	    Set_Stability(sim, o2, norm, r2, t);
	}

	if ( ((DynObjectPtr)o1->detail)->state.stable )
	    Bound_Set_Stable(sim, o1, &(d1->state.x), &(d1->state.o),o1->bound);
	else
	    Update_Bound(sim, o1, t, true);
	d1->state.last_event_t = t;
	if ( ((DynObjectPtr)o2->detail)->state.stable )
	    Bound_Set_Stable(sim, o2, &(d2->state.x), &(d2->state.o),o2->bound);
	else
	    Update_Bound(sim, o2, t, true);
	d2->state.last_event_t = t;

	return Overlaps_Process(sim, o1, o2, false)
	    && Overlaps_Process(sim, o2, NULL, false)
	    && Overlaps_Update(sim, o1, o2, NULL)
	    && Overlaps_Update(sim, o2, o1, o1);

      case NOT_COLLIDING:
    	if ( Update_Prediction(sim, overlap, false) == INTEG_ERROR )
	    return false;
	break;

      case PENETRATING:
	sim->error_flags |= SIM_PENETRATE_ERR;
	return false;

      case INTERFERENCE:
	sim->error_flags |= SIM_INTERFERE_ERR;
	return false;
    }

    return true;
}


static bool
Process_Update(SimulationPtr sim, SimEventPtr event)
{
    DynObjectPtr    d = (DynObjectPtr)event->obj->detail;
    double	    t = event->t;

    if ( ! Integrate_Object(sim, &(d->state), &(d->state), t, event->obj) )
    {
	sim->error_flags |= SIM_INTEGRATE_ERR;
	return false;
    }
    if ( VDot(d->state.x, d->state.x) > sim->range * sim->range )
    {
	d->state.stable_steps = MIN_STABLE_STEPS + 1;
	d->state.stable = true;
	if ( d->update_event.queue_entry )
	    Event_Delete(sim, &(d->update_event));
	VNew(0.0, 0.0, 0.0, d->state.v);
	VNew(0.0, 0.0, 0.0, d->state.w);

	if ( sim->epoch_interval <= 0.0 )
	    sim->num_stable++;

	sim->error_flags |= SIM_RANGE_ERR;
    }

    if ( d->state.stable )
	Bound_Set_Stable(sim, event->obj, &(d->state.x), &(d->state.o),
			 event->obj->bound);
    else
	Update_Bound(sim, event->obj, t, true);

    d->state.last_event_t = t;

    return Overlaps_Process(sim, event->obj, NULL, true);
}


static bool
Process_Event(SimulationPtr sim, SimEventPtr event, double t_limit,
	      bool *finished)
{
    *finished = false;

    if ( sim->do_statistics )
	sim->event_counts[event->type - start_event]++;

    switch ( event->type )
    {
	case start_event: {
	    SimObjectPtr    obj = event->obj;
	    if ( ! Start_Object(sim, obj) )
	    {
		free(event);
		return false;
	    }
	    free(event);
	    return Overlaps_Process(sim, obj, NULL, true);
	    }

	case update_event: {
	    return Process_Update(sim, event);
	    } break;

	case pairwise_event: {
	    OverlapPtr	col = (OverlapPtr)event;
	    if ( col->o2->fixed )
		return Process_Fixed(sim, event->t, col);
	    else
		return Process_Ballistic(sim, event->t, col);
	    } break;

	case epoch_event: {
	    double  t_event = event->t;
	    bool    res = Epoch_Process_Epoch(sim, event->t);
	    *finished = ( sim->num_stable == sim->num_dynamic
		       || t_event >= t_limit );
	    return res;
	    } break;

	case activate_event: {
	    return Epoch_Process_Activate(sim, event->obj, event->t);
	    }

	case checkpoint_event: {
    	    return Checkpoint_Process_Event(sim, event->obj, event->t);
	    } break;

	case no_event:
	    break;
    }

    return true;
}


static int
Fixed_Sort_X(const void *a, const void *b)
{
    SimObjectPtr  o1, o2;
    int	    	    ind_a, ind_b;
    double  	    val_a, val_b;

    o1 = (*(ObjectList*)a)->obj;
    o2 = (*(ObjectList*)b)->obj;

    ind_a = o1->index;
    ind_b = o2->index;

    val_a = (*(ObjectList*)a == &(o1->x_min)
	  ? o1->bound->min.x : o1->bound->max.x);
    val_b = (*(ObjectList*)b == &(o2->x_min)
	  ? o2->bound->min.x : o2->bound->max.x);

    if ( val_a < val_b )
	return -1;
    else if ( val_a > val_b )
	return 1;
    else if ( ind_a < ind_b )
	return -1;

    return 1;
}


static int
Fixed_Sort_Y(const void *a, const void *b)
{
    SimObjectPtr  o1, o2;
    int	    	    ind_a, ind_b;
    double  	    val_a, val_b;

    o1 = (*(ObjectList*)a)->obj;
    o2 = (*(ObjectList*)b)->obj;

    ind_a = o1->index;
    ind_b = o2->index;

    val_a = (*(ObjectList*)a == &(o1->y_min)
	  ? o1->bound->min.y : o1->bound->max.y);
    val_b = (*(ObjectList*)b == &(o2->y_min)
	  ? o2->bound->min.y : o2->bound->max.y);

    if ( val_a < val_b )
	return -1;
    else if ( val_a > val_b )
	return 1;
    else if ( ind_a < ind_b )
	return -1;

    return 1;
}


static int
Fixed_Sort_Z(const void *a, const void *b)
{
    SimObjectPtr  o1, o2;
    int	    	    ind_a, ind_b;
    double  	    val_a, val_b;

    o1 = (*(ObjectList*)a)->obj;
    o2 = (*(ObjectList*)b)->obj;

    ind_a = o1->index;
    ind_b = o2->index;

    val_a = (*(ObjectList*)a == &(o1->z_min)
	  ? o1->bound->min.z : o1->bound->max.z);
    val_b = (*(ObjectList*)b == &(o2->z_min)
	  ? o2->bound->min.z : o2->bound->max.z);

    if ( val_a < val_b )
	return -1;
    else if ( val_a > val_b )
	return 1;
    else if ( ind_a < ind_b )
	return -1;

    return 1;
}


static void
Fixed_Insert_Bounds(SimulationPtr sim)
{
    ObjectList      *bounds = New(ObjectList, sim->num_fixed * 2);
    int		    i;

    for ( i = 0 ; i < sim->num_fixed ; i++ )
    {
	bounds[i*2] = &(sim->fixed_objects[i]->x_min);
	bounds[i*2+1] = &(sim->fixed_objects[i]->x_max);
    }
    qsort((void*)bounds, sim->num_fixed * 2, sizeof(ObjectList), Fixed_Sort_X);
    sim->sorted_x = bounds[0];
    bounds[0]->prev = NULL;
    for ( i = 1 ; i < sim->num_fixed * 2 ; i++ )
    {
	bounds[i]->prev = bounds[i-1];
	bounds[i-1]->next = bounds[i];
    }
    bounds[sim->num_fixed * 2 - 1]->next = NULL;

    for ( i = 0 ; i < sim->num_fixed ; i++ )
    {
	bounds[i*2] = &(sim->fixed_objects[i]->y_min);
	bounds[i*2+1] = &(sim->fixed_objects[i]->y_max);
    }
    qsort((void*)bounds, sim->num_fixed * 2, sizeof(ObjectList), Fixed_Sort_Y);
    sim->sorted_y = bounds[0];
    bounds[0]->prev = NULL;
    for ( i = 1 ; i < sim->num_fixed * 2 ; i++ )
    {
	bounds[i]->prev = bounds[i-1];
	bounds[i-1]->next = bounds[i];
    }
    bounds[sim->num_fixed * 2 - 1]->next = NULL;

    for ( i = 0 ; i < sim->num_fixed ; i++ )
    {
	bounds[i*2] = &(sim->fixed_objects[i]->z_min);
	bounds[i*2+1] = &(sim->fixed_objects[i]->z_max);
    }
    qsort((void*)bounds, sim->num_fixed * 2, sizeof(ObjectList), Fixed_Sort_Z);
    sim->sorted_z = bounds[0];
    bounds[0]->prev = NULL;
    for ( i = 1 ; i < sim->num_fixed * 2 ; i++ )
    {
	bounds[i]->prev = bounds[i-1];
	bounds[i-1]->next = bounds[i];
    }
    bounds[sim->num_fixed * 2 - 1]->next = NULL;

    free(bounds);
}


static void
Fixed_Start(SimulationPtr sim)
{
    SimObjectPtr  obj;
    FixedObjectPtr  fixed;
    int		    i;

    for ( i = 0 ; i < sim->num_fixed ; i++ )
    {
	obj = sim->fixed_objects[i];
	fixed = (FixedObjectPtr)obj->detail;

	Bound_Set_Stable(sim, obj, &(fixed->state.x), &(fixed->state.o),
			 obj->bound);
    }
}


static void
Reset_Dynamic(SimulationPtr sim, SimObjectPtr obj)
{
    DynObjectPtr    dyn = (DynObjectPtr)obj->detail;

    dyn->state.stable = false;
    dyn->state.stable_steps = 0;

    dyn->update_event.type = update_event;
    dyn->update_event.queue_entry = NULL;
    dyn->update_event.obj = obj;

    Overlaps_Free(dyn->xyz_overlaps);
    dyn->xyz_overlaps = NULL;

    dyn->epoch_current = NULL;
}


void
Reset_Simulation(SimulationPtr sim)
{
    int	i;

    global_count = 0;

    sim->min_velocity = MIN_TIME * Max(sim->gravity, 1.0) / 2.0;
    sim->min_height = MIN_TIME * sim->min_velocity / 4.0;
    sim->target_separation = sim->min_height * 0.5;
    sim->min_separation = sim->target_separation * 10.0;

    Event_Init(sim);

    sim->num_started = 0;
    sim->num_stable = 0;

    sim->error_flags = SIM_SUCCESS;

    if ( sim->do_statistics )
    {
	for ( i = start_event ; i <= no_event ; i++ )
	    sim->event_counts[i - start_event] = 0;
	for ( i = INTEG_MIN_STEP ; i <= INTEG_MAX_STEP ; i++ )
	    sim->integrator_steps[i - INTEG_MIN_STEP] = 0;
    }

    Fixed_Start(sim);
    Fixed_Insert_Bounds(sim);

    for ( i = 0 ; i < sim->num_dynamic ; i++ )
    {
	Reset_Dynamic(sim, sim->dyn_objects[i]);
	sim->started[i] = i;
	sim->dyn_objects[i]->start_index = i;
	Event_Start_Insert(sim, sim->dyn_objects[i]);
    }

    Hash_Clear(sim->xyz_overlaps);
    Hash_Clear(sim->x_overlaps);
    Hash_Clear(sim->y_overlaps);
    Hash_Clear(sim->z_overlaps);
    Hash_Clear(sim->add_del_ops);

    Epoch_Reset(sim);
}


bool
Simulate_Step(SimulationPtr sim, unsigned int num_steps, double t_limit,
	      bool verbose)
{
    SimEventPtr	    event;
    unsigned int    event_count = 0;
    bool	    finished = false;

    sim->error_flags = SIM_SUCCESS;

    while ( event_count < num_steps && ! finished )
    {
	event = Event_Next(sim);

	if ( ! event )
	{
	    finished = true;
	    break;
	}

	sim->current_event_t = event->t;
	if ( ! Process_Event(sim, event, t_limit, &finished) )
	    return true;    /* Error flags will have been set. */

	event_count++;
	global_count++;

	if ( sim->epoch_interval <= 0.0 )
	    finished = ( sim->num_stable == sim->num_dynamic
		      || sim->current_event_t >= t_limit );

	if ( sim->debug_flags & SIM_TERMINATE_DEBUG )
	{
	    int	i, count;

	    count = 0;
	    for ( i = 0 ; i < sim->num_started ; i++ )
	    {
		if ( ((DynObjectPtr)sim->dyn_objects[sim->started[i]]->detail)
			->epoch_current->state.stable )
		    count++;
	    }
	    if ( count != sim->num_stable )
	    {
		fprintf(stderr, "Stable count doesn't match: %d %d\n",
			count, sim->num_stable);
		abort();
	    }
	}

	if ( verbose && ! ( global_count % 100000 ) )
	{
	    printf("%u events, Sim time %g, %d / %d stable\n",
		   global_count, sim->current_event_t,
		   sim->num_stable, sim->num_dynamic);
	}
    }

    if ( finished )
    {
	if ( verbose )
	    printf("%u events, Sim time %g, %d / %d stable\n",
		   global_count, sim->current_event_t,
		   sim->num_stable, sim->num_dynamic);
	return true;
    }

    return false;
}


void
Simulator_Init()
{
    Collide_Init();
    Integrate_Init();
}


void
Simulation_Copy_Vals(SimulationPtr src, SimulationPtr dest)
{
    int	i, j;

    /* Assumes that the src and dest have the same number of objects
    ** and that each object has the same topology.
    */

    dest->num_fixed = src->num_fixed;
    dest->num_dynamic = src->num_dynamic;

    for ( i = 0 ; i < src->num_fixed ; i++ )
    {
	SimObjectPtr	o_dest = dest->fixed_objects[i];
	SimObjectPtr	o_src = src->fixed_objects[i];
	FixedObjectPtr	f_dest = (FixedObjectPtr)o_dest->detail;
	FixedObjectPtr	f_src = (FixedObjectPtr)o_src->detail;

	for ( j = 0 ; j < o_dest->num_components ; j++ )
	{
	    Polyhedron_Copy_Values(o_src->components[j].poly,
				   o_dest->components[j].poly);
	    o_dest->components[j].restitution =o_src->components[j].restitution;
	    o_dest->components[j].friction = o_src->components[j].friction;
	}

	f_dest->state = f_src->state;

	o_dest->changed = ( dest->epoch_interval <= 0.0 );

	Fixed_Interactions_Clear(f_dest);
	Fixed_Interactions_Copy(f_src, f_dest, dest);
    }
    for ( i = 0 ; i < dest->num_dynamic ; i++ )
    {
	SimObjectPtr	o_dest = dest->dyn_objects[i];
	SimObjectPtr	o_src = src->dyn_objects[i];
	DynObjectPtr	d_dest = (DynObjectPtr)o_dest->detail;
	DynObjectPtr	d_src = (DynObjectPtr)o_src->detail;

	for ( j = 0 ; j < o_dest->num_components ; j++ )
	{
	    Polyhedron_Copy_Values(o_src->components[j].poly,
				   o_dest->components[j].poly);
	    o_dest->components[j].restitution =o_src->components[j].restitution;
	    o_dest->components[j].friction = o_src->components[j].friction;
	}

	d_dest->mass = d_src->mass;
	d_dest->inertia = d_src->inertia;
	d_dest->init = d_src->init;

	o_dest->changed = ( dest->epoch_interval <= 0.0 );

	Dynamic_Epoch_Free_List(d_dest->epoch_head);
	Dynamic_Epoch_Copy_List(d_src, d_dest, dest);
    }
}



void
Simulation_Object_Clear(SimObjectPtr obj)
{
    DynObjectPtr    dyn = (DynObjectPtr)obj->detail;

    Dynamic_Epoch_Free_List(dyn->epoch_head);
    dyn->epoch_head = dyn->epoch_current = NULL;
}


SimulationPtr
Simulation_New(double start, double epoch, double frame, double range,
	       double stable_range, double gravity, bool do_stats,
	       int fixed, int dynamic)
{
    SimulationPtr   sim = New(Simulation, 1);

    sim->start_time = start;
    sim->epoch_interval = epoch;
    sim->dump_interval = frame;
    sim->range = range;
    sim->stable_range = stable_range;
    sim->gravity = gravity;
    sim->do_statistics = do_stats;

    sim->num_fixed = fixed;
    sim->fixed_objects = New(SimObjectPtr, fixed);
    sim->num_dynamic = dynamic;
    sim->dyn_objects = New(SimObjectPtr, dynamic);
    sim->started = New(int, dynamic);

    sim->debug_flags = 0;

    sim->xyz_overlaps = Hash_New_Table(dynamic * 5);
    sim->x_overlaps = Hash_New_Table(dynamic * dynamic);
    sim->y_overlaps = Hash_New_Table(dynamic * dynamic);
    sim->z_overlaps = Hash_New_Table(dynamic * dynamic);
    sim->add_del_ops = Hash_New_Table(dynamic + fixed);

    return sim;
}


static double
R_Max(int num_comps, ComponentPtr comps)
{
    double  r_max = 0.0;
    double  r;
    int	    i, j;

    for ( i = 0 ; i < num_comps ; i++ )
	for ( j = 0 ; j < comps[i].poly->num_vertices ; j++ )
	    if ( ( r = VDot(comps[i].poly->vertices[j].posn,
	                    comps[i].poly->vertices[j].posn) ) > r_max )
		r_max = r;

    return sqrt(r_max);
}


SimObjectPtr
Simulation_New_Fixed(SimulationPtr sim, int index, char *name,
		     int num_comps, ComponentPtr comps,
		     Vector x, Vector axis, double angle)
{
    SimObjectPtr    obj;
    FixedObjectPtr  fixed;

    obj = sim->fixed_objects[index] = New(SimObject, 1);
    obj->index = index;
    obj->start_index = index;
    obj->detail = fixed = New(FixedObject, 1);

    obj->name = strdup(name);
    obj->fixed = true;

    obj->num_components = num_comps;
    obj->components = comps;

    obj->r_max = R_Max(num_comps, comps);

    fixed->state.x = x;
    fixed->axis = axis;
    fixed->angle = angle;
    fixed->state.o = Axis_Angle_To_Quaternion(fixed->axis,
					      fixed->angle * M_PI / 180.0);

    obj->x_min.obj = obj;
    obj->x_max.obj = obj;
    obj->y_min.obj = obj;
    obj->y_max.obj = obj;
    obj->z_min.obj = obj;
    obj->z_max.obj = obj;

    obj->bound = &(fixed->bound);
    obj->changed = true;

    fixed->interactions = RBTree_New();

    return obj;
}


SimObjectPtr
Simulation_New_Dynamic(SimulationPtr sim, int index, char *name,
		       int num_comps, ComponentPtr comps,
		       double mass, Vector inertia,
		       double t, Vector x, Vector axis,
		       double angle, Vector v, Vector w)
{
    SimObjectPtr    obj;
    DynObjectPtr    dyn;

    obj = sim->dyn_objects[index] = New(SimObject, 1);
    obj->index = index;
    obj->start_index = index;
    obj->detail = dyn = New(DynObject, 1);

    obj->name = strdup(name);
    obj->fixed = false;

    obj->num_components = num_comps;
    obj->components = comps;

    obj->r_max = R_Max(num_comps, comps);

    dyn->mass = mass;
    dyn->inertia = inertia;

    dyn->init.state.t = t;
    dyn->init.state.x = x;
    dyn->init.axis = axis;
    dyn->init.angle = angle;
    dyn->init.state.o =
	Axis_Angle_To_Quaternion(dyn->init.axis, dyn->init.angle * M_PI/180.0);
    dyn->init.state.v = v;
    dyn->init.state.w = w;

    dyn->init.state.stable = false;
    dyn->init.state.stable_steps = 0;

    obj->x_min.obj = obj;
    obj->x_max.obj = obj;
    obj->y_min.obj = obj;
    obj->y_max.obj = obj;
    obj->z_min.obj = obj;
    obj->z_max.obj = obj;

    dyn->xyz_overlaps = NULL;

    dyn->epoch_head = dyn->epoch_current = NULL;

    obj->bound = &(dyn->state.bound);

    obj->changed = true;

    return obj;
}




