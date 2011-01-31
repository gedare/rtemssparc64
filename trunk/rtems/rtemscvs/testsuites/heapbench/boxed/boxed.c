/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/boxed.c,v 1.1 2001/05/01 21:23:06 emery Exp $
 *
 * $Log: boxed.c,v $
 * Revision 1.1  2001/05/01 21:23:06  emery
 * Initial check-in, works under Windows.
 *
 *
 */

#include <boxed.h>
#include <time.h>
#include <string.h>

#define New_Stack(t, n)	(t*)alloca(sizeof(t) * (n))


#define	T_LIMIT 1000.0
#define	FRAME_STEP 0.0
#define	EPOCH_STEP 0.0
#define	START_TIME 0.0
#define	RANGE 100.0
#define STABLE_RANGE 10.0
#define GRAVITY 0.0


#define NUM_FIXED 6


#define BALL_RADIUS 0.24
#define BALL_LEVEL 3
#define BALL_RENDER_LEVEL 2
#define INIT_V_MEAN 5.0
#define INIT_V_STD_DEV 2.0

#define SLOT_SIZE 0.25
#define BOX_SIZE ( SLOT_SIZE * 10.0 )
#define WALL_THICK 0.1


static Scenario	scenario;


static void
Sphere_Base_Polyhedron(int *v_num, Vector *verts, int *f_num, int **faces)
{
    *v_num = 6;
    VNew( 0.0,  0.0,  1.0, verts[0]);
    VNew( 0.0,  0.0, -1.0, verts[1]);
    VNew( 1.0,  0.0,  0.0, verts[2]);
    VNew( 0.0,  1.0,  0.0, verts[3]);
    VNew(-1.0,  0.0,  0.0, verts[4]);
    VNew( 0.0, -1.0,  0.0, verts[5]);

    *f_num = 8;
    faces[0][0] = 0;
    faces[0][1] = 5;
    faces[0][2] = 2;
    faces[1][0] = 0;
    faces[1][1] = 2;
    faces[1][2] = 3;
    faces[2][0] = 0;
    faces[2][1] = 3;
    faces[2][2] = 4;
    faces[3][0] = 0;
    faces[3][1] = 4;
    faces[3][2] = 5;
    faces[4][0] = 1;
    faces[4][1] = 2;
    faces[4][2] = 5;
    faces[5][0] = 1;
    faces[5][1] = 3;
    faces[5][2] = 2;
    faces[6][0] = 1;
    faces[6][1] = 4;
    faces[6][2] = 3;
    faces[7][0] = 1;
    faces[7][1] = 5;
    faces[7][2] = 4;
}


static void
Sphere_Next_Polyhedron(int *v_num, Vector *verts, int *f_num, int **face_verts)
{
    int	    start_verts = *v_num;
    int	    start_faces = *f_num;
    int	    i, j;
    double  temp_d;
    Vector  d;

    for ( i = 0 ; i < start_faces ; i++ )
    {
	int 	a, b, c;
	Vector	v_new;

	VAdd(verts[face_verts[i][0]],
	     verts[face_verts[i][1]], v_new);
	VScalarMul(v_new, 0.5, v_new);
	for ( j = start_verts ; j < (*v_num) ; j++ )
	    if ( VEqual(verts[j], v_new, d) )
		break;
	a = j;
	if ( j == (*v_num) )
	{
	    verts[j] = v_new;
	    (*v_num)++;
	}

	VAdd(verts[face_verts[i][1]],
	     verts[face_verts[i][2]], v_new);
	VScalarMul(v_new, 0.5, v_new);
	for ( j = start_verts ; j < (*v_num) ; j++ )
	    if ( VEqual(verts[j], v_new, d) )
		break;
	b = j;
	if ( j == (*v_num) )
	{
	    verts[j] = v_new;
	    (*v_num)++;
	}

	VAdd(verts[face_verts[i][2]],
	     verts[face_verts[i][0]], v_new);
	VScalarMul(v_new, 0.5, v_new);
	for ( j = start_verts ; j < (*v_num) ; j++ )
	    if ( VEqual(verts[j], v_new, d) )
		break;
	c = j;
	if ( j == (*v_num) )
	{
	    verts[j] = v_new;
	    (*v_num)++;
	}

	face_verts[(*f_num)][0] = a;
	face_verts[(*f_num)][1] = face_verts[i][1];
	face_verts[(*f_num)][2] = b;
	(*f_num)++;

	face_verts[(*f_num)][0] = a;
	face_verts[(*f_num)][1] = b;
	face_verts[(*f_num)][2] = c;
	(*f_num)++;

	face_verts[(*f_num)][0] = b;
	face_verts[(*f_num)][1] = face_verts[i][2];
	face_verts[(*f_num)][2] = c;
	(*f_num)++;

	face_verts[i][1] = a;
	face_verts[i][2] = c;
    }

    for ( i = start_verts ; i < (*v_num) ; i++ )
	VUnit(verts[i], temp_d, verts[i]);
}


static void
Sphere_Wire(WireframePtr wire,
	    int num_v, Vector *verts, int num_f, int *f_counts, int **faces)
{
    double  temp_d;
    int	    i, j;

    wire->num_vertices = num_v;
    wire->num_normals = num_v;
    wire->num_colors = 1;
    wire->num_faces = num_f;

    wire->vertices = New(Vector, wire->num_vertices);
    wire->normals = New(Vector, wire->num_vertices);
    wire->colors = New(Vectorf, wire->num_colors);
    wire->faces = New(WireFace, wire->num_faces);

    for ( i = 0 ; i < num_v ; i++ )
    {
	wire->vertices[i] = verts[i];
	VUnit(verts[i], temp_d, wire->normals[i]);
    }

    VNew(drand48() * 0.5 + 0.5, drand48() * 0.5 + 0.5, drand48() * 0.5 + 0.5,
	 wire->colors[0]);

    for ( i = 0 ; i < wire->num_faces ; i++ )
    {
	wire->faces[i].num_verts = f_counts[i];
	wire->faces[i].verts = New(int, f_counts[i]);
	wire->faces[i].norms = New(int, f_counts[i]);
	wire->faces[i].colors = New(int, f_counts[i]);
	for ( j = 0 ; j < f_counts[i] ; j++ )
	{
	    wire->faces[i].verts[j] = faces[i][j];
	    wire->faces[i].norms[j] = faces[i][j];
	    wire->faces[i].colors[j] = 0;
	}
    }
}


static void
Sphere_Polyhedron(WireframePtr wire, int *v_num, Vector **verts, int *f_num,
		  int **face_nums, int ***face_verts, int level,
		  int render_level)
{
    int	    num_edges;
    int	    i;

    *v_num = 6;
    num_edges = 12;
    *f_num = 8;

    for ( i = 0 ; i < level ; i++ )
    {
	(*v_num) += num_edges;
	num_edges = 2 * num_edges + 3 * (*f_num);
	(*f_num) *= 4;
    }

    *verts = New(Vector, *v_num);
    *face_nums = New(int, *f_num);
    *face_verts = New(int*, *f_num);
    for ( i = 0 ; i < *f_num ; i++ )
    {
	(*face_nums)[i] = 3;
	(*face_verts)[i] = New(int, 3);
    }

    Sphere_Base_Polyhedron(v_num, *verts, f_num, *face_verts);
    if ( ! render_level )
	Sphere_Wire(wire, *v_num, *verts, *f_num, *face_nums, *face_verts);

    for ( i = 1 ; i <= level ; i++ )
    {
	Sphere_Next_Polyhedron(v_num, *verts, f_num, *face_verts);
	if ( render_level == i )
	    Sphere_Wire(wire, *v_num, *verts, *f_num, *face_nums, *face_verts);
    }
}


static void
Wall_Geometry(ComponentPtr *comp, WireframePtr *wire)
{
    int		    i, j;
    int		    *f_counts;
    int		    **f_verts;

    (*wire) = New(Wireframe, 1);

    (*wire)->num_vertices = 8;
    (*wire)->vertices = New(Vector, 8);
    VNew( BOX_SIZE + WALL_THICK,
	  BOX_SIZE + WALL_THICK,
	  BOX_SIZE + WALL_THICK,
	 (*wire)->vertices[0]);
    VNew(-BOX_SIZE - WALL_THICK,
	  BOX_SIZE + WALL_THICK,
	  BOX_SIZE + WALL_THICK,
	 (*wire)->vertices[1]);
    VNew(-BOX_SIZE,
	  BOX_SIZE,
	  BOX_SIZE,
	 (*wire)->vertices[2]);
    VNew( BOX_SIZE,
	  BOX_SIZE,
	  BOX_SIZE,
	 (*wire)->vertices[3]);
    VNew( BOX_SIZE + WALL_THICK,
	  BOX_SIZE + WALL_THICK,
	 -BOX_SIZE - WALL_THICK,
	 (*wire)->vertices[4]);
    VNew(-BOX_SIZE - WALL_THICK,
	  BOX_SIZE + WALL_THICK,
	 -BOX_SIZE - WALL_THICK,
	 (*wire)->vertices[5]);
    VNew(-BOX_SIZE,
	  BOX_SIZE,
	 -BOX_SIZE,
	 (*wire)->vertices[6]);
    VNew( BOX_SIZE,
	  BOX_SIZE,
	 -BOX_SIZE,
	 (*wire)->vertices[7]);

    (*wire)->num_colors = 1;
    (*wire)->colors = New(Vectorf, 1);
    VNew(0.9, 0.9, 0.9, (*wire)->colors[0]);

    (*wire)->num_faces = 6;
    (*wire)->faces = New(WireFace, 6);
    f_counts = New_Stack(int, 6);
    f_verts = New_Stack(int*, 6);
    for ( i = 0 ; i < 6 ; i++ )
    {
	(*wire)->faces[i].num_verts = 4;
	(*wire)->faces[i].verts = New(int, 4);
	(*wire)->faces[i].norms = New(int, 4);
	(*wire)->faces[i].colors = New(int, 4);
	f_counts[i] = 4;
	f_verts[i] = New_Stack(int, 4);
	for ( j = 0 ; j < 4 ; j++ )
	{
	    (*wire)->faces[i].norms[j] = i;
	    (*wire)->faces[i].colors[j] = 0;
	}
    }
    f_verts[0][0] = (*wire)->faces[0].verts[0] = 0;
    f_verts[0][1] = (*wire)->faces[0].verts[1] = 1;
    f_verts[0][2] = (*wire)->faces[0].verts[2] = 2;
    f_verts[0][3] = (*wire)->faces[0].verts[3] = 3;
    f_verts[1][0] = (*wire)->faces[1].verts[0] = 7;
    f_verts[1][1] = (*wire)->faces[1].verts[1] = 6;
    f_verts[1][2] = (*wire)->faces[1].verts[2] = 5;
    f_verts[1][3] = (*wire)->faces[1].verts[3] = 4;
    f_verts[2][0] = (*wire)->faces[2].verts[0] = 0;
    f_verts[2][1] = (*wire)->faces[2].verts[1] = 4;
    f_verts[2][2] = (*wire)->faces[2].verts[2] = 5;
    f_verts[2][3] = (*wire)->faces[2].verts[3] = 1;
    f_verts[3][0] = (*wire)->faces[3].verts[0] = 0;
    f_verts[3][1] = (*wire)->faces[3].verts[1] = 3;
    f_verts[3][2] = (*wire)->faces[3].verts[2] = 7;
    f_verts[3][3] = (*wire)->faces[3].verts[3] = 4;
    f_verts[4][0] = (*wire)->faces[4].verts[0] = 3;
    f_verts[4][1] = (*wire)->faces[4].verts[1] = 2;
    f_verts[4][2] = (*wire)->faces[4].verts[2] = 6;
    f_verts[4][3] = (*wire)->faces[4].verts[3] = 7;
    f_verts[5][0] = (*wire)->faces[5].verts[0] = 2;
    f_verts[5][1] = (*wire)->faces[5].verts[1] = 1;
    f_verts[5][2] = (*wire)->faces[5].verts[2] = 5;
    f_verts[5][3] = (*wire)->faces[5].verts[3] = 6;

    (*wire)->num_normals = 6;
    (*wire)->normals = New(Vector, 6);
    for ( i = 0 ; i < 6 ; i++ )
    {
	Vector	v1, v2;
	double	temp_d;

	VSub((*wire)->vertices[(*wire)->faces[i].verts[1]],
	     (*wire)->vertices[(*wire)->faces[i].verts[0]], v1);
	VSub((*wire)->vertices[(*wire)->faces[i].verts[2]],
	     (*wire)->vertices[(*wire)->faces[i].verts[0]], v2);
	VCross(v1, v2, (*wire)->normals[i]);
	VUnit((*wire)->normals[i], temp_d, (*wire)->normals[i]);
    }

    (*comp) = New(Component, 1);
    (*comp)->poly = Polyhedron_New((*wire)->num_vertices, (*wire)->vertices,
				   (*wire)->num_faces, f_counts, f_verts);
    (*comp)->restitution = 1.0;
    (*comp)->friction = 0.0;
}


static void
Wall_Piece_Create(ScenarioPtr scenario, ComponentPtr comp, WireframePtr wire,
		  int index, Vector axis, double angle)
{
    BoxedObjectPtr obj;
    Vector  	    pos;
    char	    name[64];

    VNew(0.0, 0.0, 0.0, pos);

    obj = scenario->objects[index] = New(BoxedObject, 1);

    obj->wire = wire;

    sprintf(name, "wall-%d", index);
    scenario->objects[index]->sim_obj =
	Simulation_New_Fixed(scenario->sim, index, name, 1, comp, pos,
			     axis, angle);
}


static double
Std_Normal_Deviate()
{
    /* Assume the rand48 sequence has been initialised. */
    static int       iset = 0;
    static double    gset;
    double           fac, rsq, v1, v2;

    if ( ! iset )
    {
        do {
            v1 = 2.0 * drand48() - 1.0;
            v2 = 2.0 * drand48() - 1.0;
            rsq = v1 * v1 + v2 * v2;
        } while ( rsq >= 1.0 || rsq == 0.0 );
        fac = sqrt(-2.0 * log(rsq) / rsq);
        gset = v1 * fac;
        iset = 1;
        return ( v2 * fac );
    }
    iset = 0;
    return gset;
}


static double
Normal_Deviate(double mean, double std_dev)
{
    return Std_Normal_Deviate() * std_dev + mean;
}


static Vector
Random_Sphere()
{
    Vector  res;
    double  l;

    do {
	res.x = drand48() * 2.0 - 1.0;
	res.y = drand48() * 2.0 - 1.0;
	res.z = drand48() * 2.0 - 1.0;
    } while ( ( l = VMod(res) ) > 1.0 );

    res.x /= l;
    res.y /= l;
    res.z /= l;

    return res;
}


static void
Sphere_Geometry(ComponentPtr *comp, WireframePtr *wire, double *mass,
		Vector *inertia, Quaternion *rot, Vector *trans)
{
    int		    pv_num;
    Vector	    *p_verts;
    int		    pf_num;
    int		    *pf_counts;
    int		    **pf_verts;
    int		    i;
    double	    temp_d;

    (*wire) = New(Wireframe, 1);

    Sphere_Polyhedron(*wire, &pv_num, &p_verts, &pf_num, &pf_counts,
		      &pf_verts, BALL_LEVEL, BALL_RENDER_LEVEL);
    for ( i = 0 ; i < pv_num ; i++ )
	VScalarMul(p_verts[i], BALL_RADIUS, p_verts[i]);
    for ( i = 0 ; i < (*wire)->num_vertices ; i++ )
    {
	VUnit((*wire)->vertices[i], temp_d, (*wire)->normals[i]);
	VScalarMul((*wire)->vertices[i], BALL_RADIUS, (*wire)->vertices[i]);
    }

    Mass_Inertia(pv_num, p_verts, pf_num, pf_counts, pf_verts, 1000.0,
		 mass, inertia, rot, trans);

    (*comp) = New(Component, 1);
    (*comp)->poly = Polyhedron_New(pv_num, p_verts, pf_num, pf_counts,pf_verts);
    (*comp)->restitution = 1.0;
    (*comp)->friction = 0.0;
}


static void
Ball_Create(ScenarioPtr scenario, ComponentPtr comp, WireframePtr wire,
	    double mass, Vector inertia, Quaternion rot, Vector trans,
	    int index, Vector posn)
{
    BoxedObjectPtr  obj;
    Vector  	    t_apply, axis, dir, v, w;
    Quaternion	    r_final, r_apply;
    double	    angle, v_mod;
    char	    name[64];

    obj = scenario->objects[index + NUM_FIXED] = New(BoxedObject, 1);

    obj->wire = wire;

    VAdd(posn, trans, t_apply);

    axis = Random_Sphere();
    angle = drand48() * 2.0 * M_PI;
    r_apply = Axis_Angle_To_Quaternion(axis, angle);
    QQMul(r_apply, rot, r_final);
    Quaternion_To_Axis_Angle(&r_final, &axis, &angle);

    dir = Random_Sphere();
    v_mod = Normal_Deviate(INIT_V_MEAN, INIT_V_STD_DEV);
    VScalarMul(dir, v_mod, v);

    VNew(0.0, 0.0, 0.0, w);

    sprintf(name, "ball-%d", index);
    obj->sim_obj = Simulation_New_Dynamic(scenario->sim, index, name, 1, comp,
					  mass, inertia, 0.0, t_apply,
					  axis, angle * 180.0 / M_PI, v, w);
}


void
Scenario_Create(ScenarioPtr scenario, int num_balls)
{
    ComponentPtr    wall_comp;
    WireframePtr    wall_wire;
    ComponentPtr    sphere_comp;
    WireframePtr    sphere_wire;
    double	    mass;
    Vector	    inertia;
    Quaternion	    rot;
    Vector	    trans;
    int		    num_per_dir;
    int		    num_slots;
    int		    *slots;
    Vector	    posn;
    Vector  	    axis;
    int	    	    i;

    scenario->sim = Simulation_New(START_TIME, EPOCH_STEP, FRAME_STEP,
				   RANGE, STABLE_RANGE, GRAVITY, true,
				   NUM_FIXED, num_balls);

    scenario->objects = New(BoxedObjectPtr, NUM_FIXED + num_balls);

    Wall_Geometry(&wall_comp, &wall_wire);
    Sphere_Geometry(&sphere_comp, &sphere_wire, &mass, &inertia, &rot, &trans);

    VNew(0.0, 0.0, 1.0, axis);
    Wall_Piece_Create(scenario, wall_comp, wall_wire, 0, axis, 0.0);
    Wall_Piece_Create(scenario, wall_comp, wall_wire, 1, axis, 90.0);
    Wall_Piece_Create(scenario, wall_comp, wall_wire, 2, axis, 180.0);
    Wall_Piece_Create(scenario, wall_comp, wall_wire, 3, axis, 270.0);
    VNew(1.0, 0.0, 0.0, axis);
    Wall_Piece_Create(scenario, wall_comp, wall_wire, 4, axis, -90.0);
    Wall_Piece_Create(scenario, wall_comp, wall_wire, 5, axis, 90.0);

    num_per_dir = (int)floor(BOX_SIZE / SLOT_SIZE + 0.01);
    num_slots = num_per_dir * num_per_dir * num_per_dir;
    slots = New(int, num_slots);
    for ( i = 0 ; i < num_slots ; i++ )
	slots[i] = i;
    for ( i = 0 ; i < num_balls ; i++ )
    {
	int choice = (int)floor(num_slots * drand48());
	int rem;
	int x_pos, y_pos, z_pos;

	x_pos = slots[choice] / ( num_per_dir * num_per_dir );
	rem = slots[choice] % ( num_per_dir * num_per_dir );
	y_pos = rem / num_per_dir;
	rem = rem % num_per_dir;
	z_pos = rem;

	posn.x = ( SLOT_SIZE * 2.0 * x_pos + SLOT_SIZE ) - BOX_SIZE;
	posn.y = ( SLOT_SIZE * 2.0 * y_pos + SLOT_SIZE ) - BOX_SIZE;
	posn.z = ( SLOT_SIZE * 2.0 * z_pos + SLOT_SIZE ) - BOX_SIZE;

	Ball_Create(scenario, sphere_comp, sphere_wire, mass, inertia, rot,
		    trans, i, posn);

	slots[choice] = slots[num_slots - 1];
	num_slots--;
    }

    scenario->sim->debug_flags = SIM_NO_DEBUG;
}


static bool
Base_Dump(FILE *check, FILE *data, BoxedObjectPtr obj)
{
    int i, j;

    if ( ! fprintf(check, "%s %d\n", obj->sim_obj->name,
				     obj->sim_obj->fixed ? 1 : 0) )
	return false;
    if ( ! fprintf(data, "%s %d\n", obj->sim_obj->name,
				    obj->sim_obj->fixed ? 1 : 0) )
	return false;

    fprintf(check, "%d\n", obj->wire->num_vertices);
    for ( j = 0 ; j < obj->wire->num_vertices ; j++ )
    {
	if ( ! fprintf(check, "%g %g %g\n",
		       obj->wire->vertices[j].x,
		       obj->wire->vertices[j].y,
		       obj->wire->vertices[j].z) )
	    return false;
	if ( ! fprintf(data, "%15g %15g %15g\n",
		       obj->wire->vertices[j].x,
		       obj->wire->vertices[j].y,
		       obj->wire->vertices[j].z) )
	    return false;
    }
    fprintf(check, "%d\n", obj->wire->num_normals);
    for ( j = 0 ; j < obj->wire->num_normals ; j++ )
    {
	if ( ! fprintf(check, "%g %g %g\n",
		       obj->wire->normals[j].x,
		       obj->wire->normals[j].y,
		       obj->wire->normals[j].z) )
	    return false;
    }
    fprintf(check, "%d\n", obj->wire->num_colors);
    for ( j = 0 ; j < obj->wire->num_colors ; j++ )
    {
	if ( ! fprintf(check, "%g %g %g\n",
		       obj->wire->colors[j].x,
		       obj->wire->colors[j].y,
		       obj->wire->colors[j].z) )
	    return false;
    }
    fprintf(check, "%d\n", obj->wire->num_faces);
    for ( j = 0 ; j < obj->wire->num_faces ; j++ )
    {
	fprintf(check, "%d", obj->wire->faces[j].num_verts);
	for ( i = 0 ; i < obj->wire->faces[j].num_verts ; i++ )
	    fprintf(check, " %d %d %d",
		    obj->wire->faces[j].verts[i],
		    obj->wire->faces[j].norms[i],
		    obj->wire->faces[j].colors[i]);
	if ( ! fprintf(check, "\n") )
	    return false;
    }

    return true;
}


bool
Scenario_Dump(FILE *check, FILE *data, ScenarioPtr scenario)
{
    SimulationPtr   sim = scenario->sim;
    int 	    i;

    if ( ! fprintf(check, "%d\n\n", sim->num_fixed + sim->num_dynamic ) )
	return false;
    if ( ! fprintf(data, "%d\n\n", sim->num_fixed + sim->num_dynamic ) )
	return false;
    for ( i = 0 ; i < sim->num_fixed ; i++ )
    {
	SimObjectPtr	obj = sim->fixed_objects[i];
	FixedObjectPtr  fixed = (FixedObjectPtr)obj->detail;

	if ( ! Base_Dump(check, data, scenario->objects[i]) )
	    return false;

	if ( ! fprintf(check, "%g %g %g %g %g %g %g\n\n",
		       fixed->state.x.x, fixed->state.x.y, fixed->state.x.z,
		       fixed->axis.x, fixed->axis.y,
		       fixed->axis.z, fixed->angle ) )
	    return false;
	if ( ! fprintf(data, "%15g %15g %15g %15g %15g %15g %15g\n\n",
		       fixed->state.x.x, fixed->state.x.y, fixed->state.x.z,
		       fixed->axis.x, fixed->axis.y,
		       fixed->axis.z, fixed->angle ) )
	    return false;
    }

    for ( i = 0 ; i < sim->num_dynamic ; i++ )
    {
	SimObjectPtr	obj = sim->dyn_objects[i];
	DynObjectPtr    dyn = (DynObjectPtr)obj->detail;

	if ( ! Base_Dump(check, data, scenario->objects[i+sim->num_fixed]) )
	    return false;

	if ( ! fprintf(data, "%15g\n"
			     "%15g %15g %15g\n"
			     "%15g %15g %15g %15g\n"
			     "%15g %15g %15g\n"
			     "%15g %15g %15g\n",
		       dyn->init.state.t, dyn->init.state.x.x,
		       dyn->init.state.x.y, dyn->init.state.x.z,
		       dyn->init.axis.x, dyn->init.axis.y,
		       dyn->init.axis.z, dyn->init.angle,
		       dyn->init.state.v.x, dyn->init.state.v.y,
		       dyn->init.state.v.z, dyn->init.state.w.x,
		       dyn->init.state.w.y, dyn->init.state.w.z ) )
	    return false;
    }
    fprintf(check, "\n\n");
    fprintf(data, "\n\n");

    return true;
}


static void
Run_Simulation(SimulationPtr sim, double t_max, bool verbose)
{
    Reset_Simulation(sim);

    Simulate_Step(sim, 0xffffffff, t_max, verbose);
}


static bool
Dump(ScenarioPtr scenario, int index)
{
    char    *checkname;
    FILE    *checkfile;
    char    *dataname;
    FILE    *datafile;

    checkname = New(char, strlen(scenario->dump_basename) + 16);
    sprintf(checkname, "%s-%4.4d.sim", scenario->dump_basename, index);
    dataname = New(char, strlen(scenario->dump_basename) + 16);
    sprintf(dataname, "%s-%4.4d.vals", scenario->dump_basename, index);

    if ( ! ( checkfile = fopen(checkname, "w") )
      || ! ( datafile = fopen(dataname, "w") )
      || ! Scenario_Dump(checkfile, datafile, scenario)
      || ! Simulation_Dump(checkfile, scenario->sim) )
    {
	fprintf(stderr, "Couldn't open or write to file: %s\n", checkname);
	return false;
    }

    fclose(checkfile);
    free(checkname);
    fclose(datafile);
    free(dataname);

    return true;
}


int
main(int argc, char *argv[])
{
    int	    num_balls = 0;
    int	    total_events;
    int	    seed;
    int	    i;
    bool    verbose = true;

    double	    t_all_start;
    double	    t_sim_start;
    double	    t_end;

    seed = time(NULL);

    for ( i = 1 ; i < argc ; i++ )
    {
	if ( ! strncmp(argv[i], "-n", 2) )
	{
	    i++;
	    if ( sscanf(argv[i], "%d", &num_balls) != 1 )
	    {
		fprintf(stderr, "Couldn't read number of balls\n");
		num_balls = 0;
	    }
	}
	else if ( ! strncmp(argv[i], "-s", 2) )
	{
	    i++;
	    if ( sscanf(argv[i], "%d", &seed) != 1 )
	    {
		fprintf(stderr, "Couldn't read seed\n");
		seed = time(NULL);
	    }
	}
	else if ( ! strncmp(argv[i], "-q", 2) )
	    verbose = false;
	else
	    fprintf(stderr, "Don't know what %s means\n", argv[i]);
    }

    printf("Seed is %d\n", seed);
    srand48(seed);

    Simulator_Init();

    scenario.dump_basename = "/output";

    Scenario_Create(&scenario, num_balls);

    Run_Simulation(scenario.sim, T_LIMIT, verbose);

#if 0
    t_end = resources.ru_utime.tv_sec + 0.000001 * resources.ru_utime.tv_usec
	  + resources.ru_stime.tv_sec + 0.000001 * resources.ru_stime.tv_usec;

    printf("Total time: %g\n", t_end - t_all_start);
    printf("Sim time: %g\n", t_end - t_sim_start);
#endif

    printf("Start events: %d\n",
	    scenario.sim->event_counts[start_event - start_event]);
    printf("Pairwise events: %d\n",
	    scenario.sim->event_counts[pairwise_event - start_event]);
#if ASYNC
    printf("Update events: %d\n",
	    scenario.sim->event_counts[update_event - start_event]);
    printf("Epoch events: %d\n",
	    scenario.sim->event_counts[epoch_event - start_event]);
    printf("Activate events: %d\n",
	    scenario.sim->event_counts[activate_event - start_event]);
#endif
    printf("Checkpoint events: %d\n",
	    scenario.sim->event_counts[checkpoint_event - start_event]);
    printf("No events: %d\n",
	    scenario.sim->event_counts[no_event - start_event]);
    total_events = 0;
    for ( i = start_event ; i <= no_event ; i++ )
	total_events += scenario.sim->event_counts[i - start_event];
    printf("Total events: %d\n", total_events);

    for ( i = INTEG_MIN_STEP ; i <= INTEG_MAX_STEP ; i++ )
	printf("Integrator step %d: %d\n",
		i, scenario.sim->integrator_steps[i - INTEG_MIN_STEP]);

    if ( 0 && ! Dump(&scenario, 0) )
    {
	fprintf(stderr, "Apparently can't write file.\n");
	return 3;
    }

    if ( scenario.sim->error_flags != SIM_SUCCESS )
    {
	if ( scenario.sim->error_flags & SIM_RANGE_ERR )
	    printf("Range error\n");
	if ( scenario.sim->error_flags & SIM_PENETRATE_ERR )
	    printf("Penetrate error\n");
	if ( scenario.sim->error_flags & SIM_INTEGRATE_ERR )
	    printf("Integrate error\n");
	if ( scenario.sim->error_flags & SIM_RESOLVE_ERR )
	    printf("Resolve error\n");
	if ( scenario.sim->error_flags & SIM_INTERFERE_ERR )
	    printf("Interfere error\n");
    }

    return 0;
}



