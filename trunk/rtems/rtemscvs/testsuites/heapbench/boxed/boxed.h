/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/boxed.h,v 1.1 2001/05/01 21:23:07 emery Exp $
 *
 * $Log: boxed.h,v $
 * Revision 1.1  2001/05/01 21:23:07  emery
 * Initial check-in, works under Windows.
 *
 *
 */

#ifndef _BOXED_H_
#define _BOXED_H_

#if _WIN32
#include <malloc.h>
#define drand48() ((double) rand()/ (double) RAND_MAX)
#define srand48(x) srand(x)
#endif

#define M_PI 3.1415926535846 /* FIX ME EDB */
#include <simulator.h>


typedef struct _WireFace {
    int num_verts;
    int	*verts;
    int	*norms;
    int	*colors;
    } WireFace, *WireFacePtr;

typedef struct _Wireframe {
    int	    	num_vertices;
    Vector  	*vertices;
    int	    	num_normals;
    Vector  	*normals;
    int	    	num_colors;
    Vectorf   	*colors;
    int	    	num_faces;
    WireFace	*faces;
    } Wireframe, *WireframePtr;


typedef struct _BoxedObject {
    WireframePtr    wire;
    SimObjectPtr    sim_obj;
    } BoxedObject, *BoxedObjectPtr;


typedef struct _Scenario {
    BoxedObjectPtr  *objects;
    SimulationPtr   sim;

    char	    *dump_basename;
    } Scenario, *ScenarioPtr;


extern void Mass_Inertia(int, Vector*, int, int*, int**, double, double*,
			 Vector*, Quaternion*, Vector*);

#endif

