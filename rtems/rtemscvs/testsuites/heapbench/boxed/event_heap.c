/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/event_heap.c,v 1.1 2001/05/01 21:23:10 emery Exp $
 *
 * $Log: event_heap.c,v $
 * Revision 1.1  2001/05/01 21:23:10  emery
 * Initial check-in, works under Windows.
 *
 */

#include <simulator.h>


static void
Event_Print(SimEventPtr event, char *message)
{
    if ( event->type == pairwise_event )
	fprintf(stderr, "Events: %s pairwise, %g, %s %s\n", message, event->t,
		((OverlapPtr)event)->o1->name, ((OverlapPtr)event)->o2->name);
    else
    {
	fprintf(stderr, "Events: %s ", message);
	switch ( event->type )
	{
	  case update_event:
	    fprintf(stderr, "update");
	    break;
	  case checkpoint_event:
	    fprintf(stderr, "checkpoint");
	    break;
	  case epoch_event:
	    fprintf(stderr, "epoch");
	    break;
	  case activate_event:
	    fprintf(stderr, "activate");
	    break;
	  case start_event:
	    fprintf(stderr, "start");
	    break;
	  case no_event:
	    fprintf(stderr, "no_event");
	    break;
	  case pairwise_event:
	    break;
	}
	fprintf(stderr, ", %g, %s\n", event->t,
		event->obj ? event->obj->name : "NULL");
    }
}


void
Event_Init(SimulationPtr sim)
{
    if ( sim->event_queue )
    {
	QueueEntryPtr	entry;
	while ( ( entry = Priority_Delete_Min(sim->event_queue) ) )
	    Priority_Free_Entry(entry);
    }
    else
	sim->event_queue = Priority_New();

    if ( sim->debug_flags & SIM_EVENTS_DEBUG )
	fprintf(stderr, "Events Init\n");
}


void
Event_Insert(SimulationPtr sim, SimEventPtr event)
{
    if ( ( sim->debug_flags & SIM_PEDANTIC_DEBUG ) && event->type == no_event )
    {
	fprintf(stderr, "Inserting no_event\n");
	abort();
    }

    if ( ( sim->debug_flags & SIM_PEDANTIC_DEBUG ) && event->queue_entry )
    {
	fprintf(stderr, "Event already in queue\n");
	if ( event->type == pairwise_event )
	    fprintf(stderr, "pairwise, %g, %s %s\n", event->t,
		    ((OverlapPtr)event)->o1->name,
		    ((OverlapPtr)event)->o2->name);
	abort();
    }

    if ( sim->debug_flags & SIM_EVENTS_DEBUG )
	Event_Print(event, "Insert");

    event->queue_entry =
	Priority_Insert(sim->event_queue, event->t, 3, (void*)event);
}


void
Event_Start_Insert(SimulationPtr sim, SimObjectPtr obj)
{
    SimEventPtr	start;

    start = New(SimEvent, 1);
    start->type = start_event;
    start->t = ((DynObjectPtr)obj->detail)->init.state.t;
    start->obj = obj;
    Priority_Insert(sim->event_queue, start->t, 2, (void*)start);

    if ( sim->debug_flags & SIM_EVENTS_DEBUG )
	fprintf(stderr, "Events: Insert Start, %g %s\n", start->t, obj->name);
}


void
Event_Activate_Insert(SimulationPtr sim, SimObjectPtr obj, double t)
{
    SimEventPtr	event;

    if ( ( sim->debug_flags & SIM_PEDANTIC_DEBUG )
      && ( obj->fixed || obj->changed ) )
    {
	fprintf(stderr,
		"Trying to insert activate for fixed or activated object\n");
	abort();
    }

    event = &(((DynObjectPtr)obj->detail)->activate_event);

    if ( event->queue_entry )
    {
	if ( event->t <= t )
	{
	    if ( sim->debug_flags & SIM_EVENTS_DEBUG )
		fprintf(stderr,
			"Events: No change Activate, %g %s\n", t, obj->name);
	    return;
	}

	event->t = t;
	Priority_Change(sim->event_queue, event->queue_entry, t, 1);

	if ( sim->debug_flags & SIM_EVENTS_DEBUG )
	    fprintf(stderr, "Events: Change Activate, %g %s\n", t, obj->name);
    }
    else
    {
	event->t = t;
	event->queue_entry =
	    Priority_Insert(sim->event_queue, t, 1, (void*)event);

	if ( sim->debug_flags & SIM_EVENTS_DEBUG )
	    fprintf(stderr, "Events: Insert Activate, %g %s\n", t, obj->name);
    }
}


void    
Event_Epoch_Insert(SimulationPtr sim)
{
    SimEventPtr	event = &(sim->epoch_event);

    if ( sim->debug_flags & SIM_EVENTS_DEBUG )
	fprintf(stderr, "Events: Insert Epoch, %g\n", event->t);

    event->queue_entry =
	Priority_Insert(sim->event_queue, event->t, 0, (void*)event);
}


void
Event_Delete(SimulationPtr sim, SimEventPtr event)
{
    Priority_Delete(sim->event_queue, event->queue_entry);
    Priority_Free_Entry(event->queue_entry);

    if ( sim->debug_flags & SIM_EVENTS_DEBUG )
	Event_Print(event, "Delete");

    event->queue_entry = NULL;
}


void
Event_Change(SimulationPtr sim, SimEventPtr event)
{
    Priority_Change(sim->event_queue, event->queue_entry, event->t, 3);

    if ( sim->debug_flags & SIM_EVENTS_DEBUG )
	Event_Print(event, "Change");
}


SimEventPtr
Event_Next(SimulationPtr sim)
{
    QueueEntryPtr   entry;
    SimEventPtr	    res = NULL;

    if ( ( entry = Priority_Delete_Min(sim->event_queue) ) )
    {
	res = (SimEventPtr)Priority_Value(entry);
	Priority_Free_Entry(entry);

	if ( sim->debug_flags & SIM_EVENTS_DEBUG )
	{
	    fprintf(stderr, "\n");
	    Event_Print(res, "Next");
	}

	res->queue_entry = NULL;
    }

    return res;
}


