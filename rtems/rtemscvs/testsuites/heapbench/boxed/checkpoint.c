/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/checkpoint.c,v 1.1 2001/05/01 21:23:07 emery Exp $
 *
 * $Log: checkpoint.c,v $
 * Revision 1.1  2001/05/01 21:23:07  emery
 * Initial check-in, works under Windows.
 *
 */

#include <boxed.h>
#include <simulator.h>

#define Checkpoint_Error(s) { \
                    fprintf(stderr, "Error writing to checkfile.\n"); \
                    fprintf(stderr, s); \
		    return false; \
		}


static CheckpointList	free_checkpoints = NULL;


#define Checkpoint_New(d)   { \
    if ( free_checkpoints ) \
    { \
	if ( (d)->checkpoints_head ) \
	{ \
	    (d)->checkpoints_tail->next = free_checkpoints; \
	    free_checkpoints = free_checkpoints->next; \
	    (d)->checkpoints_tail->next->prev = (d)->checkpoints_tail; \
	    (d)->checkpoints_tail = (d)->checkpoints_tail->next; \
	    (d)->checkpoints_tail->next = NULL; \
	} \
	else \
	{ \
	    (d)->checkpoints_head = (d)->checkpoints_tail = free_checkpoints; \
	    free_checkpoints = free_checkpoints->next; \
	    (d)->checkpoints_tail->next = \
	    (d)->checkpoints_tail->prev = NULL; \
	} \
    } \
    else \
    { \
	if ( (d)->checkpoints_head ) \
	{ \
	    (d)->checkpoints_tail->next = New(CheckpointData, 1); \
	    (d)->checkpoints_tail->next->prev = (d)->checkpoints_tail; \
	    (d)->checkpoints_tail = (d)->checkpoints_tail->next; \
	    (d)->checkpoints_tail->next = NULL; \
	} \
	else \
	{ \
	    (d)->checkpoints_head = \
	    (d)->checkpoints_tail = New(CheckpointData, 1); \
	    (d)->checkpoints_tail->next = (d)->checkpoints_tail->prev = NULL; \
	} \
    } }


void
Checkpoint_Insert_Event(SimulationPtr sim, SimObjectPtr obj, double t_now)
{
    DynObjectPtr    dyn = (DynObjectPtr)obj->detail;

    if ( sim->dump_interval <= 0.0 )
	return;

    dyn->checkpoint_event.t =
	ceil(( t_now + 0.001 - sim->start_time ) / sim->dump_interval)
	* sim->dump_interval + sim->start_time;

    if ( dyn->checkpoint_event.queue_entry )
	Event_Change(sim, &(dyn->checkpoint_event));
    else
	Event_Insert(sim, &(dyn->checkpoint_event));
}


void
Checkpoint_Object_Start(SimulationPtr sim, SimObjectPtr obj)
{
    if ( sim->dump_interval > 0.0 && obj->changed )
	Checkpoint_Insert_Event(sim, obj,
				((DynObjectPtr)obj->detail)->init.state.t);
}


bool
Checkpoint_Process_Event(SimulationPtr sim, SimObjectPtr obj, double t)
{
    DynObjectPtr	dyn = (DynObjectPtr)obj->detail;
    DynamicEpochList	epoch = dyn->epoch_current;
    CheckpointList	last_check;	
    DynamicEpochList	el;
    DynamicState	s;

    if ( ( sim->debug_flags & SIM_PEDANTIC_DEBUG ) && ! obj->changed )
    {
	fprintf(stderr, "Checkpoint event for unchanged object.\n");
	abort();
    }

    last_check = NULL;
    for ( el = epoch ; el ; el = el->prev )
	if ( el->checkpoints_tail )
	{
	    last_check = el->checkpoints_tail;
	    break;
	}

    if ( ( sim->debug_flags & SIM_PEDANTIC_DEBUG )
      && last_check && last_check->t > t + sim->dump_interval * 0.25 )
	/* Can get here if the simulation has been rewound, but this
	** object was already active and hence valid. */
    {
	fprintf(stderr, "Checkpoint tail later than checkpoint time.\n");
	abort();
    }

    if ( dyn->state.stable && last_check && last_check->stable )
	return true;

    Checkpoint_New(epoch);

    if ( dyn->state.stable )
	s = dyn->state;
    else if ( ! Integrate_Object(sim, &(dyn->state), &s, t, NULL) )
    {
	sim->error_flags |= SIM_INTEGRATE_ERR;
	return false;
    }

    epoch->checkpoints_tail->t = t;
    epoch->checkpoints_tail->stable = dyn->state.stable;
    VtoVf(s.x, epoch->checkpoints_tail->x);
    QtoQf(s.o, epoch->checkpoints_tail->o);

    Checkpoint_Insert_Event(sim, obj, t);

    return true;
}


void
Checkpoints_Free(DynamicEpochList el)
{
    if ( ! el->checkpoints_head )
	return;

    el->checkpoints_tail->next = free_checkpoints;
    free_checkpoints = el->checkpoints_head;
    el->checkpoints_head = NULL;
    el->checkpoints_tail = NULL;
}


void
Checkpoints_Free_Tail(CheckpointList l, CheckpointList tail)
{
    if ( ! l )
	return;

    if ( l->next )
    {
	tail->next = free_checkpoints;
	free_checkpoints = l->next;
    }

    l->next = NULL;
}


void
Checkpoints_Copy(DynamicEpochList in, DynamicEpochList out)
{
    CheckpointList  l;

    out->checkpoints_head = out->checkpoints_tail = NULL;

    for ( l = in->checkpoints_head ; l ; l = l->next )
    {
	if ( out->checkpoints_tail )
	{
	    if ( free_checkpoints )
	    {
		out->checkpoints_tail->next = free_checkpoints;
		free_checkpoints = free_checkpoints->next;
	    }
	    else
		out->checkpoints_tail->next = New(CheckpointData,1);
	    out->checkpoints_tail->next->prev = out->checkpoints_tail;
	    out->checkpoints_tail = out->checkpoints_tail->next;
	    out->checkpoints_tail->next = NULL;
	}
	else
	{
	    if ( free_checkpoints )
	    {
		out->checkpoints_head =
		out->checkpoints_tail = free_checkpoints;
		free_checkpoints = free_checkpoints->next;
	    }
	    else
		out->checkpoints_head =
		out->checkpoints_tail = New(CheckpointData,1);
	    out->checkpoints_tail->prev = out->checkpoints_tail->next = NULL;
	}

	out->checkpoints_tail->t = l->t;
	out->checkpoints_tail->stable = l->stable;
	out->checkpoints_tail->x = l->x;
	out->checkpoints_tail->o = l->o;
    }
}


static int
Checkpoints_Count(DynObjectPtr dyn)
{
    DynamicEpochList	el;
    CheckpointList  	cl;
    int			count = 0;

    for ( el = dyn->epoch_head ; el ; el = el->next )
	for ( cl = el->checkpoints_head ; cl ; cl = cl->next )
	    count++;

    return count;
}


static bool
Dump_Dynamic(FILE *f, SimulationPtr sim)
{
    DynamicEpochList	el;
    CheckpointList  	cl;
    SimObjectPtr    	obj;
    DynObjectPtr    	dyn;
    int		    	i;

    if ( ! fprintf(f, "%d\n\n", sim->num_dynamic) )
	Checkpoint_Error("Writing num dynamic\n");

    for ( i = 0 ; i < sim->num_dynamic ; i++ )
    {
	obj = sim->dyn_objects[i];
	dyn = (DynObjectPtr)obj->detail;

	if ( ! fprintf(f, "%s ", obj->name) )
	    Checkpoint_Error("Writing dynamic name\n");

	if ( ! fprintf(f, "%d\n", Checkpoints_Count(dyn)) )
	    Checkpoint_Error("Writing num_checks\n");
	for ( el = dyn->epoch_head ; el ; el = el->next )
	  for ( cl = el->checkpoints_head ; cl ; cl = cl->next )
	  {
	    if ( ! fprintf(f, "%g %d %g %g %g %g %g %g %g\n",
			   cl->t, cl->stable ? 1 : 0, cl->x.x, cl->x.y, cl->x.z,
			   cl->o.real_part, cl->o.vect_part.x,
			   cl->o.vect_part.y, cl->o.vect_part.z) )
		Checkpoint_Error("Writing ballistic data\n");
	  }

	if ( ! fprintf(f, "\n") )
	    Checkpoint_Error("Writing newline\n");
    }

    return true;
}


bool
Simulation_Dump(FILE *sim_out, SimulationPtr sim)
{
    return Dump_Dynamic(sim_out, sim);
}


