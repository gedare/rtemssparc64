/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/pairwise.c,v 1.1 2001/05/01 21:23:15 emery Exp $
 *
 * $Log: pairwise.c,v $
 * Revision 1.1  2001/05/01 21:23:15  emery
 * Initial check-in, works under Windows.
 *
 */

#include <simulator.h>

/*
#include <exhaust_dist.h>
*/

static unsigned char
Pair_Colliding_Work(SimulationPtr sim, OverlapPtr overlap,
		    bool do_rad, Vector *r1, Vector *r2, Vector *norm)
{
    Quaternion	*o1, *o2;
    Vector	*t1, *t2;
    int	    	i, j;
    double  	test_dist;
    Vector  	test_dir;
    Vector  	test_cp1, test_cp2;
    Vector  	cp1, cp2;

    if ( overlap->o1->fixed )
    {
	o1 = &(((FixedObjectPtr)overlap->o1->detail)->state.o);
	t1 = &(((FixedObjectPtr)overlap->o1->detail)->state.x);
    }
    else
    {
	o1 = &(((DynObjectPtr)overlap->o1->detail)->state.o);
	t1 = &(((DynObjectPtr)overlap->o1->detail)->state.x);
    }
    if ( overlap->o2->fixed )
    {
	o2 = &(((FixedObjectPtr)overlap->o2->detail)->state.o);
	t2 = &(((FixedObjectPtr)overlap->o2->detail)->state.x);
    }
    else
    {
	o2 = &(((DynObjectPtr)overlap->o2->detail)->state.o);
	t2 = &(((DynObjectPtr)overlap->o2->detail)->state.x);
    }

    overlap->dist = 1.0e20;
    for ( i = 0 ; i < overlap->o1->num_components ; i++ )
	for ( j = 0 ; j < overlap->o2->num_components ; j++ )
	{
/*
	    double  	exhaust_dist;
	    FeaturePair	exhaust_feat;
	    Vector	ep1, ep2, edir;
*/

	    test_dist =
		VClip_Distance(overlap->o1->components[i].poly,
			       o1, t1, &(overlap->features[i][j].f1), &test_cp1,
			       overlap->o2->components[j].poly,
			       o2, t2, &(overlap->features[i][j].f2), &test_cp2,
			       &test_dir);

/*
	    exhaust_dist =
		Exhaustive_Dist(overlap->o1->components[i].poly,
				overlap->o1->components[i].poly_copy,
				o1, t1, &(exhaust_feat.f1), &ep1,
				overlap->o2->components[j].poly,
				overlap->o2->components[j].poly_copy,
				o2, t2, &(exhaust_feat.f2), &ep2, &edir);

	    if ( exhaust_feat.f1.feature != overlap->features[i][j].f1.feature
	      || exhaust_feat.f2.feature != overlap->features[i][j].f2.feature )
	    {
		fprintf(stderr, "Features don't match\n");
		abort();
	    }
*/

	    if ( sim->debug_flags & SIM_DISTANCE_DEBUG )
		fprintf(stderr,
			"Comps %d %d: P1 %g %g %g; P2 %g %g %g; DIR %g %g %g\n",
			i, j, test_cp1.x, test_cp1.y, test_cp1.z,
			test_cp2.x, test_cp2.y, test_cp2.z,
			test_dir.x, test_dir.y, test_dir.z);

	    if ( test_dist < 0.0 )
	    {
		if ( sim->debug_flags & SIM_ABORT_DEBUG )
		{
		    fprintf(stderr, "Dist < 0.0 : %g\n", test_dist);
		    abort();
		}
		return PENETRATING;
	    }

	    if ( test_dist < overlap->dist )
	    {
		overlap->o1_close_comp = i;
		overlap->o2_close_comp = j;
		overlap->dist = test_dist;
		overlap->dir = test_dir;

		if ( do_rad )
		    *norm = test_dir;
		cp1 = test_cp1;
		cp2 = test_cp2;
	    }
	}

    if ( do_rad && overlap->dist < sim->min_separation )
    {
	if ( overlap->o1->fixed )
	    VSub(cp1, ((FixedObjectPtr)overlap->o1->detail)->state.x, *r1);
	else
	    VSub(cp1, ((DynObjectPtr)overlap->o1->detail)->state.x, *r1);
	if ( overlap->o2->fixed )
	    VSub(cp2, ((FixedObjectPtr)overlap->o2->detail)->state.x, *r2);
	else
	    VSub(cp2, ((DynObjectPtr)overlap->o2->detail)->state.x, *r2);

	return COLLIDING;
    }

    return ( overlap->dist < sim->min_separation ? COLLIDING : NOT_COLLIDING );
}


static unsigned char
Pair_Distance(SimulationPtr sim, OverlapPtr overlap,
	      DynamicStatePtr s1, DynamicStatePtr s2)
{
    if ( sim->debug_flags & SIM_DISTANCE_DEBUG )
	fprintf(stderr, "Pair Dist: %g %g\n", s1->t, s2->t);
    return Pair_Colliding_Work(sim, overlap, false, NULL, NULL, NULL);
}


unsigned char
Pair_Colliding(SimulationPtr sim, OverlapPtr overlap,
               Vector *r1, Vector *ww1, Vector *r2, Vector *ww2,
	       Vector *norm, double *un)
{
    int     close;
    Vector  u1, u2;

    DynamicStatePtr s1 = overlap->o1->fixed
			    ? &(((FixedObjectPtr)overlap->o1->detail)->state)
			    : &(((DynObjectPtr)overlap->o1->detail)->state);
    DynamicStatePtr s2 = overlap->o2->fixed
			    ? &(((FixedObjectPtr)overlap->o2->detail)->state)
			    : &(((DynObjectPtr)overlap->o2->detail)->state);

    close = Pair_Colliding_Work(sim, overlap, true, r1, r2, norm);

    if ( close == PENETRATING )
    {
	DynObjectPtr	d1 = (DynObjectPtr)overlap->o1->detail;

	if ( s1->t == d1->init.state.t )
	{
	    printf("Interference: %s %s\n",
		    overlap->o1->name, overlap->o2->name);
	    return INTERFERENCE;
	}

	if ( ! overlap->o2->fixed )
	{
	    DynObjectPtr    d2 = (DynObjectPtr)overlap->o2->detail;

	    if ( s2->t == d2->init.state.t )
	    {
		printf("Interference: %s %s\n",
			overlap->o1->name, overlap->o2->name);
		return INTERFERENCE;
	    }

	}

	if ( sim->debug_flags & SIM_ABORT_DEBUG )
	{
	    fprintf(stderr, "Pair Penetrating in Pair_Colliding.\n");
	    abort();
	}
	return PENETRATING;
    }

    if ( close )
    {
	if ( sim->debug_flags & SIM_DISTANCE_DEBUG )
	    fprintf(stderr, "R1 %g %g %g; R2 %g %g %g\n",
		    r1->x, r1->y, r1->z, r2->x, r2->y, r2->z);

	if ( ((DynObjectPtr)overlap->o1->detail)->state.stable )
	{
	    DynObjectPtr    d = (DynObjectPtr)overlap->o2->detail;
	    VNew(0.0, 0.0, 0.0, *ww1);
	    QVMul(d->state.o, d->state.w, *ww2);
	    VCross(*ww2, *r2, u2);
	    VAdd(d->state.v, u2, u2);
	    *un = -VDot(u2, *norm);
	    if ( sim->debug_flags & SIM_DISTANCE_DEBUG )
		fprintf(stderr, "u2 %g %g %g : %g\n", u2.x, u2.y, u2.z, *un);
	}
	else if ( overlap->o2->fixed 
	       || ((DynObjectPtr)overlap->o2->detail)->state.stable )
	{
	    DynObjectPtr    d = (DynObjectPtr)overlap->o1->detail;
	    VNew(0.0, 0.0, 0.0, *ww2);
	    QVMul(d->state.o, d->state.w, *ww1);
	    VCross(*ww1, *r1, u1);
	    VAdd(d->state.v, u1, u1);
	    *un = VDot(u1, *norm);
	    if ( sim->debug_flags & SIM_DISTANCE_DEBUG )
		fprintf(stderr, "u1 %g %g %g : %g\n", u1.x, u1.y, u1.z, *un);
	}
	else
	{
	    DynObjectPtr    d1 = (DynObjectPtr)overlap->o1->detail;
	    DynObjectPtr    d2 = (DynObjectPtr)overlap->o2->detail;

	    QVMul(d1->state.o, d1->state.w, *ww1);
	    VCross(*ww1, *r1, u1);
	    VAdd(d1->state.v, u1, u1);
	    QVMul(d2->state.o, d2->state.w, *ww2);
	    VCross(*ww2, *r2, u2);
	    VAdd(d2->state.v, u2, u2);
	    *un = VDot(u1, *norm) - VDot(u2, *norm);
	    if ( sim->debug_flags & SIM_DISTANCE_DEBUG )
	    {
		fprintf(stderr, "u1 %g %g %g : u2 %g %g %g : %g\n",
			u1.x, u1.y, u1.z, u2.x, u2.y, u2.z, *un);
		fprintf(stderr, "%g %g : %g %g\n",
			VDot(u1, *norm), VDot(d1->state.v, *norm),
			VDot(u2, *norm), VDot(d2->state.v, *norm));
	    }
	}

	if ( *un <= 0.0 )
	    return NOT_COLLIDING;

	return COLLIDING;
    }

    return NOT_COLLIDING;
}


static bool
Solve_Quad_For_Min(double a, double b, double c, double *dt)
{
    double  	    discrim;

    if ( a > -1.0e-10 && a < 1.0e-10 )
	*dt = -c / b;
    else
    {
	discrim = b * b / ( a * a ) - ( 4.0 * c / a );
	if ( discrim < 0.0 )
	    return false;
	discrim = sqrt(discrim);
	*dt = 0.5 * ( -b / a  - discrim );
    }

    return true;
}


static bool
Solve_Quad_For_Max(double a, double b, double c, double *dt)
{
    double  	    discrim;

    if ( a > -1.0e-10 && a < 1.0e-10 )
	*dt = -c / b;
    else
    {
	discrim = b * b / ( a * a ) - ( 4.0 * c / a );
	if ( discrim < 0.0 )
	    return false;
	discrim = sqrt(discrim);
	*dt = 0.5 * ( -b / a  + discrim );
    }

    return true;
}


static unsigned char
Predict_Stable_Time(Vector *norm, double sep_dist, double *min_t,
		    Vector *v, double r_max, double w_max, double target,
		    double gravity, bool debug)
{
    double  a, b;
    bool    soln;

    a = 0.5 * norm->z * -gravity;
    b = VDot(*v, *norm) + r_max * w_max;

    if ( a > -1.0e-10 && a < 1.0e-10 )
    {
	/* Take care of linear case, where direction is perp to gravity. */
	if ( b <= 0.0 )
	    return WONT_HIT;

	*min_t = ( sep_dist - target ) / b;

	if ( *min_t < ABS_MIN_STEP
	  && sep_dist * 0.05 > sep_dist - target )
	    *min_t = ( sep_dist * 0.05 ) / b;

	if ( *min_t > 0.0 )
	    return WILL_HIT;

	return WONT_HIT;
    }

    if ( a > 0.0 )
    {
	/* Body moving away then back in. We only care about max solutions. */
	soln = Solve_Quad_For_Max(a, b, target - sep_dist, min_t);
	if ( ! soln || ( *min_t < ABS_MIN_STEP
		      && sep_dist * 0.05 > sep_dist - target ) )
	    soln = Solve_Quad_For_Max(a, b, -sep_dist * 0.05, min_t);
    }
    else
    {
	/* Body moving in then away. Only care about min solutions. */
	soln = Solve_Quad_For_Min(a, b, target - sep_dist, min_t);

	if ( ! soln )
	    return WONT_HIT;

	if ( *min_t < ABS_MIN_STEP
	  && sep_dist * 0.05 > sep_dist - target )
	    soln = Solve_Quad_For_Min(a, b, -sep_dist * 0.05, min_t);
    }

    if ( soln && *min_t > 0.0 )
	return WILL_HIT;

    return WONT_HIT;
}


static unsigned char
Predict_Stable(SimulationPtr sim, OverlapPtr overlap, DynamicStatePtr s)
{
    SimObjectPtr    o1, o2;
    DynObjectPtr    d;
    bool	    reverse;
    Vector	    dir;
    double	    w_max;
    bool	    status = WONT_HIT;
    double	    min_t;

    reverse = ((DynObjectPtr)overlap->o1->detail)->state.stable;
    if ( reverse )
    {
	o1 = overlap->o2;
	o2 = overlap->o1;
    }
    else
    {
	o1 = overlap->o1;
	o2 = overlap->o2;
    }

    d = (DynObjectPtr)o1->detail;
    s = &(d->state);

    w_max = sqrt( ( d->inertia.x * s->w.x * s->w.x
		  + d->inertia.y * s->w.y * s->w.y
		  + d->inertia.z * s->w.z * s->w.z )
		/ ( d->inertia.x > d->inertia.y
		    ? ( d->inertia.y > d->inertia.z
		      ? d->inertia.z : d->inertia.y )
		    : ( d->inertia.x > d->inertia.z
		      ? d->inertia.z : d->inertia.x ) ) );

    if ( reverse )
    {
	dir.x = -overlap->dir.x;
	dir.y = -overlap->dir.y;
	dir.z = -overlap->dir.z;
    }
    else
	dir = overlap->dir;

    if ( sim->debug_flags & SIM_PREDICT_DEBUG )
	fprintf(stderr,
		"Predict: %s %s : dist %g : dir %g %g %g\n",
		overlap->o1->name, overlap->o2->name,
		overlap->dist, dir.x, dir.y, dir.z);

    status = Predict_Stable_Time(&dir, overlap->dist, &min_t,
				 &(s->v), o1->r_max, w_max,
				 sim->target_separation, sim->gravity,
				 sim->debug_flags & SIM_PREDICT_DEBUG);

    if ( status == WILL_HIT )
	overlap->min_t = min_t + s->t;

    return status;
}


static unsigned char
Predict_Ballistic_Time(Vector *norm, double sep_dist, double *min_t,
		       Vector *v1, double r1_max, double w1_max,
		       Vector *v2, double r2_max, double w2_max,
		       double target, bool debug)
{
    double  denom;

    denom = VDot(*v1, *norm) + r1_max * w1_max
	  - VDot(*v2, *norm) + r2_max * w2_max;

    if ( denom <= 0.0 )
    {
	if ( debug )
	    fprintf(stderr, "Pre Bal Time: denom <= 0\n");
	return WONT_HIT;
    }

    *min_t = ( sep_dist - target ) / denom;

    if ( debug )
	fprintf(stderr, "First attempt: dt: %g\n", *min_t);

    if ( *min_t < ABS_MIN_STEP
      && sep_dist * 0.05 > sep_dist - target )
    {
	*min_t = ( sep_dist * 0.05 ) / denom;
	if ( debug )
	    fprintf(stderr, "Second attempt: dt: %g\n", *min_t);
    }

    if ( *min_t > 0.0 )
	return WILL_HIT;

    return WONT_HIT;
}


static unsigned char
Predict_Ballistic(SimulationPtr sim, OverlapPtr overlap,
		  DynamicStatePtr s1, DynamicStatePtr s2)
{
    SimObjectPtr    o1, o2;
    DynObjectPtr    d1, d2;
    double	    w1_max, w2_max;
    bool	    status = WONT_HIT;
    double	    min_t;

    o1 = overlap->o1;
    d1 = (DynObjectPtr)o1->detail;
    o2 = overlap->o2;
    d2 = (DynObjectPtr)o2->detail;

    w1_max = sqrt( ( d1->inertia.x * s1->w.x * s1->w.x
		   + d1->inertia.y * s1->w.y * s1->w.y
		   + d1->inertia.z * s1->w.z * s1->w.z )
		 / ( d1->inertia.x > d1->inertia.y
		     ? ( d1->inertia.y > d1->inertia.z
		       ? d1->inertia.z : d1->inertia.y )
		     : ( d1->inertia.x > d1->inertia.z
		       ? d1->inertia.z : d1->inertia.x)));
    w2_max = sqrt( ( d2->inertia.x * s2->w.x * s2->w.x
		   + d2->inertia.y * s2->w.y * s2->w.y
		   + d2->inertia.z * s2->w.z * s2->w.z )
		 / ( d2->inertia.x > d2->inertia.y
		     ? ( d2->inertia.y > d2->inertia.z
		       ? d2->inertia.z : d2->inertia.y )
		     : ( d2->inertia.x > d2->inertia.z
		       ? d2->inertia.z : d2->inertia.x)));

    status = Predict_Ballistic_Time(&(overlap->dir), overlap->dist, &min_t,
				    &(s1->v), o1->r_max, w1_max,
				    &(s2->v), o2->r_max, w2_max,
				    sim->target_separation,
				    sim->debug_flags & SIM_PREDICT_DEBUG);

    if ( status == WILL_HIT )
	overlap->min_t = min_t + s1->t;

    return status;
}


unsigned char
Pair_Predict(SimulationPtr sim, OverlapPtr overlap, bool do_dist)
{
    DynamicStatePtr s1, s2;

    if ( overlap->o2->fixed )
    {
	s1 = &(((DynObjectPtr)overlap->o1->detail)->state);
	s2 = &(((FixedObjectPtr)overlap->o2->detail)->state);

	if ( s1->stable )
	    return WONT_HIT;
	else
	{
	    if ( do_dist && Pair_Distance(sim, overlap, s1, s2) == PENETRATING )
	    {
		DynObjectPtr	d1 = (DynObjectPtr)overlap->o1->detail;

		if ( d1->state.t == d1->init.state.t )
		{
		    printf("Interference: %s %s\n",
			    overlap->o1->name, overlap->o2->name);
		    return INTERFERENCE;
		}

		if ( sim->debug_flags & SIM_ABORT_DEBUG )
		{
		    fprintf(stderr, "Penetration not at start: %s %s\n",
			    overlap->o1->name, overlap->o2->name);
		    abort();
		}
		else
		{
		    printf("Penetration: %s %s\n",
			    overlap->o1->name, overlap->o2->name);
		}
		return PENETRATING;
	    }
	    return Predict_Stable(sim, overlap, s1);
	}
    }

    if ( ((DynObjectPtr)overlap->o1->detail)->state.stable )
    {
	if ( ((DynObjectPtr)overlap->o2->detail)->state.stable )
	    return WONT_HIT;
	else
	{
	    s2 = &(((DynObjectPtr)overlap->o2->detail)->state);

	    if ( do_dist )
	    {
		s1 = &(((DynObjectPtr)overlap->o1->detail)->state);

		if ( s1->t > s2->t )
		{
		    if ( ! Integrate_Object(sim, s2, s2, s1->t, overlap->o2) )
			return INTEG_ERROR;
		}
		if ( Pair_Distance(sim, overlap, s1, s2) == PENETRATING )
		{
		    DynObjectPtr	d1 = (DynObjectPtr)overlap->o1->detail;
		    DynObjectPtr	d2 = (DynObjectPtr)overlap->o2->detail;

		    if ( d1->state.t == d1->init.state.t
		      || d2->state.t == d2->init.state.t )
		    {
			printf("Interference: %s %s\n",
				overlap->o1->name, overlap->o2->name);
			return INTERFERENCE;
		    }

		    if ( sim->debug_flags & SIM_ABORT_DEBUG )
		    {
			fprintf(stderr, "Penetration not at start: %s %s\n",
				overlap->o1->name, overlap->o2->name);
			abort();
		    }
		    else
		    {
			printf("Penetration: %s %s\n",
				overlap->o1->name, overlap->o2->name);
		    }
		    return PENETRATING;
		}
	    }
	    return Predict_Stable(sim, overlap, s2);
	}
    }
    else if ( ((DynObjectPtr)overlap->o2->detail)->state.stable )
    {
	s1 = &(((DynObjectPtr)overlap->o1->detail)->state);

	if ( do_dist )
	{
	    s2 = &(((DynObjectPtr)overlap->o2->detail)->state);

	    if ( s2->t > s1->t )
	    {
		if ( ! Integrate_Object(sim, s1, s1, s2->t, overlap->o1) )
		    return INTEG_ERROR;
	    }
	    if ( Pair_Distance(sim, overlap, s1, s2) == PENETRATING )
	    {
		DynObjectPtr	d1 = (DynObjectPtr)overlap->o1->detail;
		DynObjectPtr	d2 = (DynObjectPtr)overlap->o2->detail;

		if ( d1->state.t == d1->init.state.t
		  || d2->state.t == d2->init.state.t )
		{
		    printf("Interference: %s %s\n",
			    overlap->o1->name, overlap->o2->name);
		    return INTERFERENCE;
		}

		if ( sim->debug_flags & SIM_ABORT_DEBUG )
		{
		    fprintf(stderr, "Penetration not at start: %s %s\n",
			    overlap->o1->name, overlap->o2->name);
		    abort();
		}
		else
		{
		    printf("Penetration: %s %s\n",
			    overlap->o1->name, overlap->o2->name);
		}
		return PENETRATING;
	    }
	}
	return Predict_Stable(sim, overlap, s1);
    }
    else
    {
	s1 = &(((DynObjectPtr)overlap->o1->detail)->state);
	s2 = &(((DynObjectPtr)overlap->o2->detail)->state);

	if ( do_dist )
	{
	    if ( s1->t > s2->t )
	    {
		if ( ! Integrate_Object(sim, s2, s2, s1->t, overlap->o2) )
		    return INTEG_ERROR;
	    }
	    else if ( s2->t > s1->t )
	    {
		if ( ! Integrate_Object(sim, s1, s1, s2->t, overlap->o1) )
		    return INTEG_ERROR;
	    }
	    if ( Pair_Distance(sim, overlap, s1, s2) == PENETRATING )
	    {
		DynObjectPtr	d1 = (DynObjectPtr)overlap->o1->detail;
		DynObjectPtr	d2 = (DynObjectPtr)overlap->o2->detail;

		if ( d1->state.t == d1->init.state.t
		  || d2->state.t == d2->init.state.t )
		{
		    printf("Interference: %s %s\n",
			    overlap->o1->name, overlap->o2->name);
		    return INTERFERENCE;
		}

		if ( sim->debug_flags & SIM_ABORT_DEBUG )
		{
		    fprintf(stderr, "Penetration not at start: %s %s\n",
			    overlap->o1->name, overlap->o2->name);
		    abort();
		}
		else
		{
		    printf("Penetration: %s %s\n",
			    overlap->o1->name, overlap->o2->name);
		}
		return PENETRATING;
	    }
	}
	return Predict_Ballistic(sim, overlap, s1, s2);
    }
}


