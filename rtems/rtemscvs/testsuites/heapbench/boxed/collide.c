/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/collide.c,v 1.1 2001/05/01 21:23:08 emery Exp $
 *
 * $Log: collide.c,v $
 * Revision 1.1  2001/05/01 21:23:08  emery
 * Initial check-in, works under Windows.
 *
 */

#include <simulator.h>
#include <recipes.h>


static double	**col_mat;
static double	*col_res;
static int	*col_index;
static double	*col_temp;

#define COL_DECOMP_ERR 0
#define COL_STICK 1
#define COL_SLIDE 2


static int
Immobile_Stick(Vector *r, double mass, Matrix *inertia, Vector *vi, Vector *wi,
	       Vector *vf, Vector *wf, Vector *p, double rest, double friction)
{
    int	    i, j, k;
    int	    permut[6];

    // Solve it all using gaussian elimination
    col_mat[0][0] = inertia->x.x;
    col_mat[0][1] = inertia->x.y;
    col_mat[0][2] = inertia->x.z;
    col_mat[0][3] = 0.0;
    col_mat[0][4] = -r->z;
    col_mat[0][5] = r->y;
    col_res[0] = VDot(inertia->x, *wi);
    col_mat[1][0] = inertia->y.x;
    col_mat[1][1] = inertia->y.y;
    col_mat[1][2] = inertia->y.z;
    col_mat[1][3] = r->z;
    col_mat[1][4] = 0.0;
    col_mat[1][5] = -r->x;
    col_res[1] = VDot(inertia->y, *wi);
    col_mat[2][0] = inertia->z.x;
    col_mat[2][1] = inertia->z.y;
    col_mat[2][2] = inertia->z.z;
    col_mat[2][3] = -r->y;
    col_mat[2][4] = r->x;
    col_mat[2][5] = 0.0;
    col_res[2] = VDot(inertia->z, *wi);
    col_mat[3][0] = 0.0;
    col_mat[3][1] = r->z;
    col_mat[3][2] = -r->y;
    col_mat[3][3] = -1.0 / mass;
    col_mat[3][4] = 0.0;
    col_mat[3][5] = 0.0;
    col_res[3] = -vi->x - rest*(vi->x + wi->y * r->z - wi->z * r->y);
    col_mat[4][0] = -r->z;
    col_mat[4][1] = 0.0;
    col_mat[4][2] = r->x;
    col_mat[4][3] = 0.0;
    col_mat[4][4] = -1.0 / mass;
    col_mat[4][5] = 0.0;
    col_res[4] = -vi->y;
    col_mat[5][0] = r->y;
    col_mat[5][1] = -r->x;
    col_mat[5][2] = 0.0;
    col_mat[5][3] = 0.0;
    col_mat[5][4] = 0.0;
    col_mat[5][5] = -1.0 / mass;
    col_res[5] = -vi->z;

    for ( i = 0 ; i < 6 ; i++ )
	permut[i] = i;

    for ( i = 0 ; i < 5 ; i++ )
    {
	int	    max = i;
	double  max_val = fabs(col_mat[permut[i]][i]);
	double  temp_d;

	// Find the pivot
	for ( j = i + 1 ; j < 6 ; j++ )
	{
	    if ( ( temp_d = fabs(col_mat[permut[j]][i]) ) > max_val )
	    {
		max = j;
		max_val = temp_d;
	    }
	}

	// Pivot
	if ( i != max )
	{
	    j = permut[i];
	    permut[i] = permut[max];
	    permut[max] = j;
	}

	// Eliminate
	for ( k = i + 1 ; k < 6 ; k++ )
	    col_mat[permut[i]][k] /= col_mat[permut[i]][i];
	col_res[permut[i]] /= col_mat[permut[i]][i];
	col_mat[permut[i]][i] = 1.0;

	for ( j = i + 1 ; j < 6 ; j++ )
	{
	    for ( k = i + 1 ; k < 6 ; k++ )
		col_mat[permut[j]][k] -= col_mat[permut[j]][i]
					 * col_mat[permut[i]][k];
	    col_res[permut[j]] -= col_mat[permut[j]][i]
				  * col_res[permut[i]];
	}
    }

    p->z = col_res[permut[5]] / col_mat[permut[5]][5];
    p->y = col_res[permut[4]] - p->z * col_mat[permut[4]][5];
    p->x = col_res[permut[3]] - p->z * col_mat[permut[3]][5]
			      - p->y * col_mat[permut[3]][4];

    if ( p->y * p->y + p->z * p->z > friction * friction * p->x * p->x )
	/* Short circuit, because result will not be used. */
	return COL_SLIDE;

    wf->z = col_res[permut[2]] - p->z * col_mat[permut[2]][5]
			       - p->y * col_mat[permut[2]][4]
			       - p->x * col_mat[permut[2]][3];
    wf->y = col_res[permut[1]] - p->z * col_mat[permut[1]][5]
			       - p->y * col_mat[permut[1]][4]
			       - p->x * col_mat[permut[1]][3]
			       - wf->z * col_mat[permut[1]][2];
    wf->x = col_res[permut[0]] - p->z * col_mat[permut[0]][5]
			       - p->y * col_mat[permut[0]][4]
			       - p->x * col_mat[permut[0]][3]
			       - wf->z * col_mat[permut[0]][2]
			       - wf->y * col_mat[permut[0]][1];

    vf->x = vi->x - p->x / mass;
    vf->y = vi->y - p->y / mass;
    vf->z = vi->z - p->z / mass;

    return COL_STICK;
}


static int
Immobile_Slide(Vector *r, double mass, Matrix *inertia, Vector *vi, Vector *wi,
	       Vector *vf, Vector *wf, Vector *p, double rest, double friction)
{
    int	    i, j, k;
    int	    permut[6];

    p->y = 0.0;

    col_mat[0][0] = inertia->x.x;
    col_mat[0][1] = inertia->x.y;
    col_mat[0][2] = inertia->x.z;
    col_mat[0][3] = 0.0;
    col_mat[0][4] = r->y;
    col_res[0] = VDot(inertia->x, *wi);
    col_mat[1][0] = inertia->y.x;
    col_mat[1][1] = inertia->y.y;
    col_mat[1][2] = inertia->y.z;
    col_mat[1][3] = r->z;
    col_mat[1][4] = -r->x;
    col_res[1] = VDot(inertia->y, *wi);
    col_mat[2][0] = inertia->z.x;
    col_mat[2][1] = inertia->z.y;
    col_mat[2][2] = inertia->z.z;
    col_mat[2][3] = -r->y;
    col_mat[2][4] = 0.0;
    col_res[2] = VDot(inertia->z, *wi);
    col_mat[3][0] = 0.0;
    col_mat[3][1] = r->z;
    col_mat[3][2] = -r->y;
    col_mat[3][3] = -1.0 / mass;
    col_mat[3][4] = 0.0;
    col_res[3] = -vi->x - rest * ( vi->x + wi->y * r->z - wi->z * r->y );
    col_mat[4][0] = 0.0;
    col_mat[4][1] = 0.0;
    col_mat[4][2] = 0.0;
    col_mat[4][3] = -friction;
    col_mat[4][4] = 1.0;
    col_res[4] = 0.0;

    for ( i = 0 ; i < 5 ; i++ )
	permut[i] = i;

    for ( i = 0 ; i < 4 ; i++ )
    {
	int	    max = i;
	double  max_val = fabs(col_mat[permut[i]][i]);
	double  temp_d;

	// Find the pivot
	for ( j = i + 1 ; j < 5 ; j++ )
	{
	    if ( ( temp_d = fabs(col_mat[permut[j]][i]) ) > max_val )
	    {
		max = j;
		max_val = temp_d;
	    }
	}

	// Pivot
	if ( i != max )
	{
	    j = permut[i];
	    permut[i] = permut[max];
	    permut[max] = j;
	}

	// Eliminate
	for ( k = i + 1 ; k < 5 ; k++ )
	    col_mat[permut[i]][k] /= col_mat[permut[i]][i];
	col_res[permut[i]] /= col_mat[permut[i]][i];
	col_mat[permut[i]][i] = 1.0;

	for ( j = i + 1 ; j < 5 ; j++ )
	{
	    for ( k = i + 1 ; k < 5 ; k++ )
		col_mat[permut[j]][k] -= col_mat[permut[j]][i]
					 * col_mat[permut[i]][k];
	    col_res[permut[j]] -= col_mat[permut[j]][i]
				  * col_res[permut[i]];
	}
    }

    p->z = col_res[permut[4]] / col_mat[permut[4]][4];
    p->x = col_res[permut[3]] - p->z * col_mat[permut[3]][4];
    wf->z = col_res[permut[2]] - p->z * col_mat[permut[2]][4]
			       - p->x * col_mat[permut[2]][3];
    wf->y = col_res[permut[1]] - p->z * col_mat[permut[1]][4]
			       - p->x * col_mat[permut[1]][3]
			       - wf->z * col_mat[permut[1]][2];
    wf->x = col_res[permut[0]] - p->z * col_mat[permut[0]][4]
			       - p->x * col_mat[permut[0]][3]
			       - wf->z * col_mat[permut[0]][2]
			       - wf->y * col_mat[permut[0]][1];

    vf->x = vi->x - p->x / mass;
    vf->y = vi->y;
    vf->z = vi->z - p->z / mass;

    return COL_SLIDE;
}


bool
Collide_Move_Fixed(SimulationPtr sim, Vector norm, double ux, double def_rest,
		   double def_fric, DynamicStatePtr dyn, Vector *radius,
		   Vector *ww, double m, Vector *i)
{
    Vector  t1, t2;
    Vector  r;
    Vector  vi, vf, wi, wf;
    Matrix  i_w, i_c;
    Matrix  w_to_c, c_to_w;
    Vector  p, p_w;
    double  temp_d;
    Matrix  b_to_w;
    Matrix  w_to_b;
    Vector  temp_v;
    Matrix  temp_m;
    double  rest, fric;

    /* Get a reference frame. */
    VNew(0.0, norm.z, -norm.y, t2);
    if ( VZero(t2) )
    {
	VNew(-norm.z, 0.0, norm.x, t2);
	VUnit(t2, temp_d, t2);
    }
    else
	VUnit(t2, temp_d, t2);
    VCross(t2, norm, t1);
    MNew(norm, t1, t2, w_to_c);
    MTrans(w_to_c, c_to_w);

    QToM(dyn->o, b_to_w);
    MTrans(b_to_w, w_to_b);

    temp_m.x.x = w_to_b.x.x * i->x;
    temp_m.x.y = w_to_b.x.y * i->y;
    temp_m.x.z = w_to_b.x.z * i->z;
    temp_m.y.x = w_to_b.y.x * i->x;
    temp_m.y.y = w_to_b.y.y * i->y;
    temp_m.y.z = w_to_b.y.z * i->z;
    temp_m.z.x = w_to_b.z.x * i->x;
    temp_m.z.y = w_to_b.z.y * i->y;
    temp_m.z.z = w_to_b.z.z * i->z;
    MMMul(b_to_w, temp_m, i_w);

    /* Convert the various parameters into it. */
    /* The radius vector from the center of mass to the collision pt. */
    MVMul(w_to_c, *radius, r);
    /* The velocities. */
    MVMul(w_to_c, dyn->v, vi);
    MVMul(w_to_c, *ww, wi);
    MMMul(w_to_c, i_w, temp_m);
    MMMul(temp_m, c_to_w, i_c);

    rest = sim->min_velocity / ux;
    if ( sim->debug_flags & SIM_RESOLVE_DEBUG )
	fprintf(stderr, "ux: %15.15g, rest: %15.15g\n", ux, rest);
    if ( rest < def_rest )
	rest = def_rest;
    if ( rest > 1.0e2 )
	rest = 1.0e2;
    fric = def_fric * def_rest / rest;

    /* Do the collision. */
    switch ( Immobile_Stick(&r, m, &i_c, &vi, &wi, &vf, &wf, &p, rest, fric) )
    {
      case COL_SLIDE:
	MVMul(c_to_w, p, p_w);
	VCross(p_w, norm, t1);
	VUnit(t1, temp_d, t1);
	VCross(norm, t1, t2);
	MNew(norm, t1, t2, w_to_c);
	MTrans(w_to_c, c_to_w);
	MVMul(w_to_c, *radius, r);
	MVMul(w_to_c, dyn->v, vi);
	MVMul(w_to_c, *ww, wi);
	MMMul(w_to_c, i_w, temp_m);
	MMMul(temp_m, c_to_w, i_c);
	Immobile_Slide(&r, m, &i_c, &vi, &wi, &vf, &wf, &p, rest, fric);
	break;

      case COL_STICK:
      	break;

      case COL_DECOMP_ERR:
	return false;
    }

    /* Convert back. */
    MVMul(c_to_w, vf, dyn->v);
    MVMul(c_to_w, wf, temp_v);
    MVMul(w_to_b, temp_v, dyn->w);

    if ( sim->debug_flags & SIM_RESOLVE_DEBUG )
    {
	ux = vf.x + wf.y * r.z - wf.z * r.y;
	fprintf(stderr, "ux = %g\n", ux);
	fprintf(stderr, "%g %g %g ; %g %g %g\n",
		vi.x, vi.y, vi.z, wi.x, wi.y, wi.z);
	fprintf(stderr, "%g %g %g ; %g %g %g ; %g %g %g\n",
		vf.x, vf.y, vf.z, wf.x, wf.y, wf.z,
		p.x, p.y, p.z);
	{
	    Vector	ui, uf;

	    VCross(wi, r, ui);
	    VAdd(vi, ui, ui);
	    VCross(wf, r, uf);
	    VAdd(vf, uf, uf);

	    fprintf(stderr, "%g %g %g -> %g %g %g\n", ui.x, ui.y, ui.z,
		    uf.x, uf.y, uf.z);
	}
    }

    return true;
}


static int
Resolve_Stick(Vector *v1i, Vector *w1i, Vector *r1, double m1, Matrix *i1,
	      Vector *v2i, Vector *w2i, Vector *r2, double m2, Matrix *i2,
	      Vector *v1f, Vector *w1f, Vector *v2f, Vector *w2f, Vector *p,
	      double rest, double friction)
{
    int	    d;

    col_mat[0][0] = i1->x.x;
    col_mat[0][1] = i1->x.y;
    col_mat[0][2] = i1->x.z;
    col_mat[1][0] = i1->y.x;
    col_mat[1][1] = i1->y.y;
    col_mat[1][2] = i1->y.z;
    col_mat[2][0] = i1->z.x;
    col_mat[2][1] = i1->z.y;
    col_mat[2][2] = i1->z.z;

    col_mat[3][3] = i2->x.x;
    col_mat[3][4] = i2->x.y;
    col_mat[3][5] = i2->x.z;
    col_mat[4][3] = i2->y.x;
    col_mat[4][4] = i2->y.y;
    col_mat[4][5] = i2->y.z;
    col_mat[5][3] = i2->z.x;
    col_mat[5][4] = i2->z.y;
    col_mat[5][5] = i2->z.z;

    col_mat[0][6] = 0.0;
    col_mat[0][7] = -r1->z;
    col_mat[0][8] = r1->y;
    col_mat[1][6] = r1->z;
    col_mat[1][7] = 0.0;
    col_mat[1][8] = -r1->x;
    col_mat[2][6] = -r1->y;
    col_mat[2][7] = r1->x;
    col_mat[2][8] = 0.0;

    col_mat[3][6] = 0.0;
    col_mat[3][7] = r2->z;
    col_mat[3][8] = -r2->y;
    col_mat[4][6] = -r2->z;
    col_mat[4][7] = 0.0;
    col_mat[4][8] = r2->x;
    col_mat[5][6] = r2->y;
    col_mat[5][7] = -r2->x;
    col_mat[5][8] = 0.0;

    col_mat[6][0] = 0.0;
    col_mat[6][1] = r1->z;
    col_mat[6][2] = -r1->y;
    col_mat[6][3] = 0.0;
    col_mat[6][4] = -r2->z;
    col_mat[6][5] = r2->y;
    col_mat[7][0] = -r1->z;
    col_mat[7][1] = 0.0;
    col_mat[7][2] = r1->x;
    col_mat[7][3] = r2->z;
    col_mat[7][4] = 0.0;
    col_mat[7][5] = -r2->x;
    col_mat[8][0] = r1->y;
    col_mat[8][1] = -r1->x;
    col_mat[8][2] = 0.0;
    col_mat[8][3] = -r2->y;
    col_mat[8][4] = r2->x;
    col_mat[8][5] = 0.0;

    col_mat[0][3] = col_mat[0][4] = col_mat[0][5] = 0.0;
    col_mat[1][3] = col_mat[1][4] = col_mat[1][5] = 0.0;
    col_mat[2][3] = col_mat[2][4] = col_mat[2][5] = 0.0;

    col_mat[3][0] = col_mat[3][1] = col_mat[3][2] = 0.0;
    col_mat[4][0] = col_mat[4][1] = col_mat[4][2] = 0.0;
    col_mat[5][0] = col_mat[5][1] = col_mat[5][2] = 0.0;

    col_mat[6][6] = col_mat[7][7] = col_mat[8][8] = -1.0 / m1 - 1.0 / m2;
    col_mat[6][7] = col_mat[6][8] = 0.0;
    col_mat[7][6] = col_mat[7][8] = 0.0;
    col_mat[8][6] = col_mat[8][7] = 0.0;

    col_res[0] = VDot(i1->x, *w1i);
    col_res[1] = VDot(i1->y, *w1i);
    col_res[2] = VDot(i1->z, *w1i);
    col_res[3] = VDot(i2->x, *w2i);
    col_res[4] = VDot(i2->y, *w2i);
    col_res[5] = VDot(i2->z, *w2i);
    col_res[6] = -rest * ( v1i->x + w1i->y * r1->z - w1i->z * r1->y
			 - v2i->x - w2i->y * r2->z + w2i->z * r2->y )
	       - v1i->x + v2i->x;
    col_res[7] = v2i->y - v1i->y;
    col_res[8] = v2i->z - v1i->z;

    if ( ! LU_Decompd(col_mat, 9, col_index, &d, col_temp) )
	return COL_DECOMP_ERR;
    LU_Back_Substd(col_mat, 9, col_index, col_res);

    p->x = col_res[6];
    p->y = col_res[7];
    p->z = col_res[8];
    if ( p->y * p->y + p->z * p->z > friction * friction * p->x * p->x )
	/* Short circuit, because result will not be used. */
	return COL_SLIDE;


    w1f->x = col_res[0];
    w1f->y = col_res[1];
    w1f->z = col_res[2];
    w2f->x = col_res[3];
    w2f->y = col_res[4];
    w2f->z = col_res[5];

    v1f->x = -p->x / m1 + v1i->x;
    v1f->y = -p->y / m1 + v1i->y;
    v1f->z = -p->z / m1 + v1i->z;
    v2f->x = p->x / m2 + v2i->x;
    v2f->y = p->y / m2 + v2i->y;
    v2f->z = p->z / m2 + v2i->z;

    return COL_STICK;
}


static int
Resolve_Slide(Vector *v1i, Vector *w1i, Vector *r1, double m1, Matrix *i1,
	      Vector *v2i, Vector *w2i, Vector *r2, double m2, Matrix *i2,
	      Vector *v1f, Vector *w1f, Vector *v2f, Vector *w2f, Vector *p,
	      double rest, double friction)
{
    int	    d;

    col_mat[0][0] = i1->x.x;
    col_mat[0][1] = i1->x.y;
    col_mat[0][2] = i1->x.z;
    col_mat[1][0] = i1->y.x;
    col_mat[1][1] = i1->y.y;
    col_mat[1][2] = i1->y.z;
    col_mat[2][0] = i1->z.x;
    col_mat[2][1] = i1->z.y;
    col_mat[2][2] = i1->z.z;

    col_mat[3][3] = i2->x.x;
    col_mat[3][4] = i2->x.y;
    col_mat[3][5] = i2->x.z;
    col_mat[4][3] = i2->y.x;
    col_mat[4][4] = i2->y.y;
    col_mat[4][5] = i2->y.z;
    col_mat[5][3] = i2->z.x;
    col_mat[5][4] = i2->z.y;
    col_mat[5][5] = i2->z.z;

    col_mat[0][6] = 0.0;
    col_mat[0][7] = -r1->z;
    col_mat[0][8] = r1->y;
    col_mat[1][6] = r1->z;
    col_mat[1][7] = 0.0;
    col_mat[1][8] = -r1->x;
    col_mat[2][6] = -r1->y;
    col_mat[2][7] = r1->x;
    col_mat[2][8] = 0.0;

    col_mat[3][6] = 0.0;
    col_mat[3][7] = r2->z;
    col_mat[3][8] = -r2->y;
    col_mat[4][6] = -r2->z;
    col_mat[4][7] = 0.0;
    col_mat[4][8] = r2->x;
    col_mat[5][6] = r2->y;
    col_mat[5][7] = -r2->x;
    col_mat[5][8] = 0.0;

    col_mat[6][0] = 0.0;
    col_mat[6][1] = r1->z;
    col_mat[6][2] = -r1->y;
    col_mat[6][3] = 0.0;
    col_mat[6][4] = -r2->z;
    col_mat[6][5] = r2->y;
    col_mat[6][6] = -1.0 / m1 - 1.0 / m2;
    col_mat[6][7] = col_mat[6][8] = 0.0;

    col_mat[0][3] = col_mat[0][4] = col_mat[0][5] = 0.0;
    col_mat[1][3] = col_mat[1][4] = col_mat[1][5] = 0.0;
    col_mat[2][3] = col_mat[2][4] = col_mat[2][5] = 0.0;

    col_mat[3][0] = col_mat[3][1] = col_mat[3][2] = 0.0;
    col_mat[4][0] = col_mat[4][1] = col_mat[4][2] = 0.0;
    col_mat[5][0] = col_mat[5][1] = col_mat[5][2] = 0.0;

    col_mat[7][0] = col_mat[7][1] = col_mat[7][2] =
    col_mat[7][3] = col_mat[7][4] = col_mat[7][5] =
    col_mat[7][6] = col_mat[7][8] = 0.0;
    col_mat[7][7] = 1.0;
    col_mat[8][0] = col_mat[8][1] = col_mat[8][2] =
    col_mat[8][3] = col_mat[8][4] = col_mat[8][5] = col_mat[8][7] = 0.0;
    col_mat[8][6] = -friction;
    col_mat[8][8] = 1.0;

    col_res[0] = VDot(i1->x, *w1i);
    col_res[1] = VDot(i1->y, *w1i);
    col_res[2] = VDot(i1->z, *w1i);
    col_res[3] = VDot(i2->x, *w2i);
    col_res[4] = VDot(i2->y, *w2i);
    col_res[5] = VDot(i2->z, *w2i);
    col_res[6] = -rest * ( v1i->x + w1i->y * r1->z - w1i->z * r1->y
			 - v2i->x - w2i->y * r2->z + w2i->z * r2->y )
	       - v1i->x + v2i->x;
    col_res[7] = 0.0;
    col_res[8] = 0.0;

    if ( ! LU_Decompd(col_mat, 9, col_index, &d, col_temp) )
	return COL_DECOMP_ERR;
    LU_Back_Substd(col_mat, 9, col_index, col_res);

    w1f->x = col_res[0];
    w1f->y = col_res[1];
    w1f->z = col_res[2];
    w2f->x = col_res[3];
    w2f->y = col_res[4];
    w2f->z = col_res[5];
    p->x = col_res[6];
    p->y = col_res[7];
    p->z = col_res[8];

    v1f->x = -p->x / m1 + v1i->x;
    v1f->y = v1i->y;
    v1f->z = -p->z / m1 + v1i->z;
    v2f->x = p->x / m2 + v2i->x;
    v2f->y = v2i->y;
    v2f->z = p->z / m2 + v2i->z;

    return COL_SLIDE;
}


bool
Collide_Move_Move(SimulationPtr sim, Vector norm, double ux,
	    double def_rest, double def_fric,
	    DynamicStatePtr b1, double m1, Vector *i1, Vector *r1, Vector *ww1,
	    DynamicStatePtr b2, double m2, Vector *i2, Vector *r2, Vector *ww2)
{
    Vector  t1, t2;
    Vector  r1c, r2c;
    Vector  v1i, v1f, w1i, w1f;
    Vector  v2i, v2f, w2i, w2f;
    Matrix  i1_w, i1_c;
    Matrix  i2_w, i2_c;
    Matrix  w_to_c, c_to_w;
    Vector  p, p_w;
    double  temp_d;
    Matrix  b1_to_w;
    Matrix  w_to_b1;
    Matrix  b2_to_w;
    Matrix  w_to_b2;
    double  rest, fric;
    Matrix  temp_m;

    /* Get a reference frame. */
    VNew(0.0, norm.z, -norm.y, t2);
    if ( VZero(t2) )
    {
	VNew(-norm.z, 0.0, norm.x, t2);
	VUnit(t2, temp_d, t2);
    }
    else
	VUnit(t2, temp_d, t2);
    VCross(t2, norm, t1);
    MNew(norm, t1, t2, w_to_c);
    MTrans(w_to_c, c_to_w);

    QToM(b1->o, b1_to_w);
    MTrans(b1_to_w, w_to_b1);
    QToM(b2->o, b2_to_w);
    MTrans(b2_to_w, w_to_b2);

    /* Set inertia matrices in world space. */
    temp_m.x.x = w_to_b1.x.x * i1->x;
    temp_m.x.y = w_to_b1.x.y * i1->y;
    temp_m.x.z = w_to_b1.x.z * i1->z;
    temp_m.y.x = w_to_b1.y.x * i1->x;
    temp_m.y.y = w_to_b1.y.y * i1->y;
    temp_m.y.z = w_to_b1.y.z * i1->z;
    temp_m.z.x = w_to_b1.z.x * i1->x;
    temp_m.z.y = w_to_b1.z.y * i1->y;
    temp_m.z.z = w_to_b1.z.z * i1->z;
    MMMul(b1_to_w, temp_m, i1_w);

    temp_m.x.x = w_to_b2.x.x * i2->x;
    temp_m.x.y = w_to_b2.x.y * i2->y;
    temp_m.x.z = w_to_b2.x.z * i2->z;
    temp_m.y.x = w_to_b2.y.x * i2->x;
    temp_m.y.y = w_to_b2.y.y * i2->y;
    temp_m.y.z = w_to_b2.y.z * i2->z;
    temp_m.z.x = w_to_b2.z.x * i2->x;
    temp_m.z.y = w_to_b2.z.y * i2->y;
    temp_m.z.z = w_to_b2.z.z * i2->z;
    MMMul(b2_to_w, temp_m, i2_w);

    /* Convert the various parameters into collision space. */
    MVMul(w_to_c, *r1, r1c);
    MVMul(w_to_c, b1->v, v1i);
    MVMul(w_to_c, *ww1, w1i);
    MMMul(w_to_c, i1_w, temp_m);
    MMMul(temp_m, c_to_w, i1_c);

    MVMul(w_to_c, *r2, r2c);
    MVMul(w_to_c, b2->v, v2i);
    MVMul(w_to_c, *ww2, w2i);
    MMMul(w_to_c, i2_w, temp_m);
    MMMul(temp_m, c_to_w, i2_c);

    rest = sim->min_velocity / ux;
    if ( sim->debug_flags & SIM_RESOLVE_DEBUG )
	fprintf(stderr, "ux: %15.15g, rest: %15.15g\n", ux, rest);
    if ( rest < def_rest )
	rest = def_rest;
    if ( rest > 1.0e2 )
	rest = 1.0e2;
    fric = def_fric * def_rest / rest;

    /* Do the collision. */
    switch ( Resolve_Stick(&v1i, &w1i, &r1c, m1, &i1_c,
			   &v2i, &w2i, &r2c, m2, &i2_c,
		           &v1f, &w1f, &v2f, &w2f, &p, rest, fric) )
    {
      case COL_SLIDE:
	MVMul(c_to_w, p, p_w);
	VCross(p_w, norm, t1);
	VUnit(t1, temp_d, t1);
	VCross(norm, t1, t2);
	MNew(norm, t1, t2, w_to_c);
	MTrans(w_to_c, c_to_w);

	MVMul(w_to_c, *r1, r1c);
	MVMul(w_to_c, b1->v, v1i);
	MVMul(w_to_c, *ww1, w1i);
	MMMul(w_to_c, i1_w, temp_m);
	MMMul(temp_m, c_to_w, i1_c);

	MVMul(w_to_c, *r2, r2c);
	MVMul(w_to_c, b2->v, v2i);
	MVMul(w_to_c, *ww2, w2i);
	MMMul(w_to_c, i2_w, temp_m);
	MMMul(temp_m, c_to_w, i2_c);

	Resolve_Slide(&v1i, &w1i, &r1c, m1, &i1_c, &v2i, &w2i, &r2c, m2, &i2_c,
		      &v1f, &w1f, &v2f, &w2f, &p, rest, fric);
	break;

      case COL_STICK:
	break;

      case COL_DECOMP_ERR:
	return false;
    }

    if ( sim->debug_flags & SIM_RESOLVE_DEBUG )
    {
	Vector	u1i, u1f;
	Vector	u2i, u2f;

	ux = v1f.x + w1f.y * r1c.z - w1f.z * r1c.y
	   - v2f.x - w2f.y * r2c.z + w2f.z * r2c.y;
	fprintf(stderr, "ux = %g\n", ux);
	fprintf(stderr, "%g %g %g ; %g %g %g ; %g %g %g ; %g %g %g\n",
		v1i.x, v1i.y, v1i.z, w1i.x, w1i.y, w1i.z,
		v2i.x, v2i.y, v2i.z, w2i.x, w2i.y, w2i.z);
	fprintf(stderr,"%g %g %g ; %g %g %g ; %g %g %g ; %g %g %g ; %g %g %g\n",
		v1f.x, v1f.y, v1f.z, w1f.x, w1f.y, w1f.z,
		v2f.x, v2f.y, v2f.z, w2f.x, w2f.y, w2f.z,
		p.x, p.y, p.z);

	VCross(w1i, r1c, u1i);
	VAdd(v1i, u1i, u1i);
	VCross(w2i, r2c, u2i);
	VAdd(v2i, u2i, u2i);
	VCross(w1f, r1c, u1f);
	VAdd(v1f, u1f, u1f);
	VCross(w2f, r2c, u2f);
	VAdd(v2f, u2f, u2f);

	fprintf(stderr, "%g %g %g -> %g %g %g\n", u1i.x, u1i.y, u1i.z,
		u1f.x, u1f.y, u1f.z);
	fprintf(stderr, "%g %g %g -> %g %g %g\n", u2i.x, u2i.y, u2i.z,
		u2f.x, u2f.y, u2f.z);
    }

    /* Convert back. */
    MVMul(c_to_w, v1f, b1->v);
    MVMul(c_to_w, w1f, *ww1);
    MVMul(w_to_b1, *ww1, b1->w);
    MVMul(c_to_w, v2f, b2->v);
    MVMul(c_to_w, w2f, *ww2);
    MVMul(w_to_b2, *ww2, b2->w);

    return true;
}


void
Collide_Init()
{
    int	i;

    col_mat = New(double*, 9);
    for ( i = 0 ; i < 9 ; i++ )
	col_mat[i] = New(double, 9);
    col_res = New(double, 9);
    col_index = New(int, 9);
    col_temp = New(double, 9);
}


