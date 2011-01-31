/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/bounds.c,v 1.1 2001/05/01 21:23:05 emery Exp $
 *
 * $Log: bounds.c,v $
 * Revision 1.1  2001/05/01 21:23:05  emery
 * Initial check-in, works under Windows.
 *
 */

#include <simulator.h>


void
Bound_Set_Stable(SimulationPtr sim, SimObjectPtr obj, Vector *x, Quaternion *o,
		 BoundingBoxPtr bound)
{
    Vector  temp_v;
    int	    i, j;

    bound->min.x = bound->min.y = bound->min.z = HUGE_VAL;
    bound->max.x = bound->max.y = bound->max.z = -HUGE_VAL;

    for ( j = 0 ; j < obj->num_components ; j++ )
    {
	for ( i = 0 ; i < obj->components[j].poly->num_vertices ; i++ )
	{
	    QVMul(*o, obj->components[j].poly->vertices[i].posn, temp_v);
	    VAdd(*x, temp_v, temp_v);
	    if ( temp_v.x < bound->min.x )
		bound->min.x = temp_v.x;
	    if ( temp_v.x > bound->max.x )
		bound->max.x = temp_v.x;
	    if ( temp_v.y < bound->min.y )
		bound->min.y = temp_v.y;
	    if ( temp_v.y > bound->max.y )
		bound->max.y = temp_v.y;
	    if ( temp_v.z < bound->min.z )
		bound->min.z = temp_v.z;
	    if ( temp_v.z > bound->max.z )
		bound->max.z = temp_v.z;
	}
    }

    bound->min.x -= sim->min_separation;
    bound->min.y -= sim->min_separation;
    bound->min.z -= sim->min_separation;
    bound->max.x += sim->min_separation;
    bound->max.y += sim->min_separation;
    bound->max.z += sim->min_separation;
}


void
Bound_Set_Ballistic(SimulationPtr sim, double r, DynamicStatePtr s, double dt,
		    BoundingBoxPtr bound)
{
    double  	    t_special;

    if ( sim->gravity > 0.0 )
    {
	t_special = 2.0 * s->v.z / sim->gravity;
	VNew(s->x.x + ( s->v.x >= 0.0 ? 0.0 : s->v.x * dt ) - r,
	     s->x.y + ( s->v.y >= 0.0 ? 0.0 : s->v.y * dt ) - r,
	     s->x.z + ( dt > t_special ? s->v.z*dt - 0.5*sim->gravity*dt*dt : 0)
	     - r, bound->min);
	t_special *= 0.5;
	VNew(s->x.x + ( s->v.x <= 0.0 ? 0.0 : s->v.x * dt ) + r,
	     s->x.y + ( s->v.y <= 0.0 ? 0.0 : s->v.y * dt ) + r,
	     s->x.z + ( t_special < 0.0 ? 0.0 : ( dt > t_special ?
			s->v.z*t_special - 0.5*sim->gravity*t_special*t_special:
			s->v.z * dt - 0.5 * sim->gravity * dt * dt ) )
	     + r, bound->max);
    }
    else
    {
	VNew(s->x.x + ( s->v.x >= 0.0 ? 0.0 : s->v.x * dt ) - r,
	     s->x.y + ( s->v.y >= 0.0 ? 0.0 : s->v.y * dt ) - r,
	     s->x.z + ( s->v.z >= 0.0 ? 0.0 : s->v.z * dt ) - r,
	     bound->min);
	VNew(s->x.x + ( s->v.x <= 0.0 ? 0.0 : s->v.x * dt ) + r,
	     s->x.y + ( s->v.y <= 0.0 ? 0.0 : s->v.y * dt ) + r,
	     s->x.z + ( s->v.z <= 0.0 ? 0.0 : s->v.z * dt ) + r,
	     bound->max);
    }

    bound->min.x -= sim->min_separation;
    bound->min.y -= sim->min_separation;
    bound->min.z -= sim->min_separation;
    bound->max.x += sim->min_separation;
    bound->max.y += sim->min_separation;
    bound->max.z += sim->min_separation;
}


void
Bound_Increment(SimulationPtr sim, SimObjectPtr obj, DynamicStatePtr s,
		double dt)
{
    BoundingBoxPtr  old = &(((DynObjectPtr)obj->detail)->epoch_current->bound);
    BoundingBox	    new;

    Bound_Set_Ballistic(sim, obj->r_max, s, dt, &new);

    if ( new.min.x < old->min.x )
	old->min.x = new.min.x;
    if ( new.min.y < old->min.y )
	old->min.y = new.min.y;
    if ( new.min.z < old->min.z )
	old->min.z = new.min.z;
    if ( new.max.x > old->max.x )
	old->max.x = new.max.x;
    if ( new.max.y > old->max.y )
	old->max.y = new.max.y;
    if ( new.max.z > old->max.z )
	old->max.z = new.max.z;
}


bool
Bound_Contains(BoundingBoxPtr bound, double r_max, DynamicStatePtr d)
{
    if ( d->x.x - r_max < bound->min.x )
	return false;
    if ( d->x.y - r_max < bound->min.y )
	return false;
    if ( d->x.z - r_max < bound->min.z )
	return false;
    if ( d->x.x + r_max > bound->max.x )
	return false;
    if ( d->x.y + r_max > bound->max.y )
	return false;
    if ( d->x.z + r_max > bound->max.z )
	return false;

    return true;
}


