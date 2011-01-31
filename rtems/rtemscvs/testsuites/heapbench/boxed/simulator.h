/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/simulator.h,v 1.1 2001/05/01 21:23:20 emery Exp $
 *
 * $Log: simulator.h,v $
 * Revision 1.1  2001/05/01 21:23:20  emery
 * Initial check-in, works under Windows.
 *
 */

#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_


#define SIM_NO_DEBUG		( 0 )
#define SIM_EVENTS_DEBUG 	( 1 )
#define SIM_PREDICT_DEBUG	( 1 << 1 )
#define SIM_RESOLVE_DEBUG	( 1 << 2 )
#define SIM_SORT_DEBUG		( 1 << 3 )
#define SIM_STABLE_DEBUG	( 1 << 4 )
#define SIM_BOUND_DEBUG		( 1 << 5 )
#define SIM_EPOCH_DEBUG		( 1 << 6 )
#define SIM_PEDANTIC_DEBUG	( 1 << 7 )
#define SIM_ABORT_DEBUG		( 1 << 8 )
#define SIM_SORT_PRINT		( 1 << 9 )
#define SIM_OVERLAP_DEBUG	( 1 << 10 )
#define SIM_DISTANCE_DEBUG	( 1 << 11 )
#define SIM_ACTIVATE_DEBUG	( 1 << 12 )
#define SIM_TERMINATE_DEBUG	( 1 << 13 )
#define SIM_EPOCH_BOUND_DEBUG	( 1 << 14 )

#define SIM_SUCCESS 	    ( 0 )
#define SIM_RANGE_ERR 	    ( 1 )
#define SIM_PENETRATE_ERR   ( 1 << 1 )
#define SIM_INTEGRATE_ERR   ( 1 << 2 )
#define SIM_RESOLVE_ERR     ( 1 << 3 )
#define SIM_INTERFERE_ERR   ( 1 << 4 )

#define INTEG_MIN_STEP -15
#define INTEG_MAX_STEP 5


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Vector.h>
#include <quaternion.h>
#include <heap.h>
#include <hash.h>
#include <red_black.h>
#include <vclip.h>


#define New(t, n)	( (t*)malloc(sizeof(t) * (n)) )
#define More(p, t, n)	( (t*)realloc((char*)(p), sizeof(t) * (n)) )
#define Min(a, b)	( (a) < (b) ? (a) : (b) )
#define Max(a, b)	( (a) >= (b) ? (a) : (b) )
#define IsZero(a)	( (a) < 0.000001 && (a) > -0.000001 )
#define DEqual(x, y)    ( (x) < (y) + 0.00001 && (x) > (y) - 0.00001 )

typedef unsigned char bool;
#define false 0
#define true 1

#define NOT_COLLIDING 0
#define COLLIDING 1
#define PENETRATING 2
#define INTEG_ERROR 3
#define INTERFERENCE 4
#define WONT_HIT 0
#define WILL_HIT 1


#define MIN_TIME 1.0e-3
#define ABS_MIN_STEP 1.0e-5


#define MIN_STABLE_STEPS 250

#define INTEG_ERROR_BOUND 1.0e-5


typedef struct _SimObject 	*SimObjectPtr;
typedef struct _DynObject 	*DynObjectPtr;
typedef struct _ObjectListElmt	*ObjectList;


typedef struct _BoundingBox {
    Vector  	    min;
    Vector  	    max;
    } BoundingBox, *BoundingBoxPtr;


typedef struct _DynamicState {
    double     	t;
    Vector     	x;
    Quaternion 	o;
    Vector    	v;
    Vector	w;
    bool	stable;
    int		stable_steps;
    double	last_event_t;
    double	next_update_t;
    BoundingBox	bound;
    } DynamicState, *DynamicStatePtr;


typedef struct _CheckpointData {
    float   	t;
    bool	stable;
    Vectorf 	x;
    Quaternionf	o;

    struct _CheckpointData  *next;
    struct _CheckpointData  *prev;
    } CheckpointData, *CheckpointList;


typedef struct _DynInitState {
    DynamicState    state;
    Vector	    axis;
    double	    angle;
    } DynInitState, *DynInitStatePtr;


typedef enum _SimEventType {
    start_event,
    update_event,
    pairwise_event,
    epoch_event,
    checkpoint_event,
    activate_event,
    no_event
    } SimEventType;


typedef struct _SimEvent {
    SimEventType    type;
    QueueEntryPtr   queue_entry;
    double	    t;
    SimObjectPtr    obj;
    } SimEvent, *SimEventPtr;


typedef struct _ObjectListElmt {
    SimObjectPtr    obj;

    ObjectList	    next;
    ObjectList	    prev;
    } ObjectListElmt;


typedef struct _DynamicEpoch {
    double  	    t;

    DynamicState    state;

    BoundingBox	    bound;

    ObjectList	    interactions;

    CheckpointList  checkpoints_head;
    CheckpointList  checkpoints_tail;

    struct _DynamicEpoch    *next;
    struct _DynamicEpoch    *prev;
    } DynamicEpoch, *DynamicEpochList;


typedef struct _OverlapListElmt	*OverlapList;

typedef struct _Component {
    PolyhedronPtr   poly;

    double  	    friction;
    double  	    restitution;
    } Component, *ComponentPtr;


typedef struct _SimObject {
    bool	    fixed;
    bool	    changed;

    char	    *name;
    int		    index;
    int		    start_index;

    int		    num_components;
    Component	    *components;
    double	    r_max;

    BoundingBoxPtr  bound;
    ObjectListElmt  x_min, x_max;
    ObjectListElmt  y_min, y_max;
    ObjectListElmt  z_min, z_max;

    void	    *detail;
    } SimObject;


typedef struct _DynObject {
    double  	    	mass;
    Vector  	    	inertia;

    DynInitState    	init;
    DynamicState    	state;

    SimEvent	    	update_event;
    SimEvent	    	activate_event;
    SimEvent	    	checkpoint_event;

    DynamicEpochList	epoch_head;
    DynamicEpochList	epoch_current;

    OverlapList     	xyz_overlaps;

    } DynObject;


typedef struct _FixedObject {
    BoundingBox	    bound;
    DynamicState    state;
    Vector	    axis;
    double	    angle;

    RBTree	    interactions;

    } FixedObject, *FixedObjectPtr;


typedef struct _FeaturePair {
    Feature f1;
    Feature f2;
    } FeaturePair, *FeaturePairPtr;

typedef struct _Overlap {
    SimEventType    type;
    QueueEntryPtr   queue_entry;
    double	    min_t;

    SimObjectPtr    o1;
    SimObjectPtr    o2; /* This will always be the fixed one, if any. */

    FeaturePair	    **features;

    double	    dist;
    Vector	    dir;

    int		    o1_close_comp;
    int		    o2_close_comp;

    OverlapList     o1_xyz_elmt;
    OverlapList     o2_xyz_elmt;

    } Overlap, *OverlapPtr;


typedef struct _OverlapListElmt {
    OverlapPtr	overlap;

    OverlapList next;
    OverlapList	prev;
    } OverlapListElmt;


typedef struct _Simulation {
    double		start_time;
    double  		range;
    double		epoch_interval;
    double		dump_interval;
    double		stable_range;
    double		gravity;

    double		min_velocity;
    double		min_height;
    double		target_separation;
    double		min_separation;

    int		    	num_fixed;
    int			num_dynamic;
    int			*started;
    SimObjectPtr  	*dyn_objects;
    SimObjectPtr  	*fixed_objects;

    HashTable		xyz_overlaps;
    HashTable		x_overlaps;
    HashTable		y_overlaps;
    HashTable		z_overlaps;
    HashTable		add_del_ops;

    int			debug_flags;
    int			error_flags;

    bool		do_statistics;
    int			event_counts[no_event - start_event + 1];
    int			integrator_steps[INTEG_MAX_STEP - INTEG_MIN_STEP + 1];

    int			num_started;
    int			num_stable;

    ObjectList		sorted_x;
    ObjectList		sorted_y;
    ObjectList		sorted_z;

    SimEvent	    	epoch_event;
    double		epoch_time;

    PriorityQueuePtr	event_queue;

    double		current_event_t;

    } Simulation, *SimulationPtr;



extern void Bound_Set_Stable(SimulationPtr, SimObjectPtr, Vector*, Quaternion*,
			     BoundingBoxPtr);
extern void Bound_Set_Ballistic(SimulationPtr, double, DynamicStatePtr, double,
				BoundingBoxPtr);
extern void Bound_Increment(SimulationPtr, SimObjectPtr, DynamicStatePtr,
			    double);
extern bool Bound_Contains(BoundingBoxPtr, double, DynamicStatePtr);


extern void Checkpoint_Insert_Event(SimulationPtr, SimObjectPtr, double);
extern void Checkpoint_Object_Start(SimulationPtr, SimObjectPtr);
extern bool Checkpoint_Process_Event(SimulationPtr, SimObjectPtr, double);
extern void Checkpoints_Free(DynamicEpochList);
extern void Checkpoints_Free_Tail(CheckpointList, CheckpointList);
extern void Checkpoints_Copy(DynamicEpochList, DynamicEpochList);
extern bool Simulation_Dump(FILE*, SimulationPtr);


extern void Collide_Init();
extern bool Collide_Move_Fixed(SimulationPtr,
			       Vector, double, double, double, DynamicStatePtr,
			       Vector*, Vector*, double, Vector*);
extern bool Collide_Move_Move(SimulationPtr, Vector, double, double, double,
                            DynamicStatePtr, double, Vector*, Vector*, Vector*,
                            DynamicStatePtr, double, Vector*, Vector*, Vector*);


extern void	Epoch_Reset(SimulationPtr);
extern void  	Epoch_Object_Start(SimulationPtr, SimObjectPtr);
extern bool  	Epoch_Process_Epoch(SimulationPtr, double);
extern bool  	Epoch_Process_Activate(SimulationPtr, SimObjectPtr, double);
extern void	Dynamic_Epoch_Free_List(DynamicEpochList);
extern void 	Dynamic_Epoch_Copy_List(DynObjectPtr, DynObjectPtr,
					SimulationPtr);
extern void	Fixed_Interactions_Clear(FixedObjectPtr);
extern void 	Fixed_Interactions_Copy(FixedObjectPtr, FixedObjectPtr,
				        SimulationPtr);
extern void	Epoch_Add_Fixed_Interaction(FixedObjectPtr,SimObjectPtr,double);
extern void	Epoch_Add_Dyn_Interaction(DynObjectPtr, SimObjectPtr);


extern void	    Event_Init(SimulationPtr);
extern SimEventPtr  Event_Next(SimulationPtr);
extern void    	    Event_Insert(SimulationPtr, SimEventPtr);
extern void	    Event_Start_Insert(SimulationPtr, SimObjectPtr);
extern void    	    Event_Activate_Insert(SimulationPtr, SimObjectPtr, double);
extern void    	    Event_Epoch_Insert(SimulationPtr);
extern void	    Event_Delete(SimulationPtr, SimEventPtr);
extern void	    Event_Change(SimulationPtr, SimEventPtr);


extern void Integrate_Init();
extern bool Integrate_Object(SimulationPtr, DynamicStatePtr, DynamicStatePtr,
			     double, SimObjectPtr);


extern void Overlaps_Object_Start(SimulationPtr, SimObjectPtr);
extern bool Overlaps_Process(SimulationPtr, SimObjectPtr, SimObjectPtr, bool);
extern bool Overlaps_Process_Full(SimulationPtr, bool);
extern bool Overlaps_Update(SimulationPtr, SimObjectPtr, SimObjectPtr,
			    SimObjectPtr);
extern void Overlaps_Free(OverlapList);

extern unsigned char	Pair_Predict(SimulationPtr, OverlapPtr, bool);
extern unsigned char	Pair_Colliding(SimulationPtr, OverlapPtr, Vector*,
				       Vector*, Vector*, Vector*, Vector*,
				       double*);


extern void Update_Bound(SimulationPtr, SimObjectPtr, double, bool);
extern int  Update_Prediction(SimulationPtr, OverlapPtr, bool);
extern void Reset_Simulation(SimulationPtr);
extern bool Simulate_Step(SimulationPtr, unsigned int, double, bool);

extern void Simulator_Init();
extern void Simulation_Copy_Vals(SimulationPtr, SimulationPtr);
extern void Simulation_Object_Clear(SimObjectPtr);

extern SimulationPtr	Simulation_New(double, double, double, double, double,
				       double, bool, int, int);

extern SimObjectPtr Simulation_New_Fixed(SimulationPtr, int, char*, int,
					 ComponentPtr,  Vector, Vector, double);

extern SimObjectPtr Simulation_New_Dynamic(SimulationPtr, int, char*,
					   int, ComponentPtr, double,
					   Vector, double, Vector, Vector,
					   double, Vector, Vector);


#endif

