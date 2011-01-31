/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/integrate.c,v 1.1 2001/05/01 21:23:12 emery Exp $
 *
 * $Log: integrate.c,v $
 * Revision 1.1  2001/05/01 21:23:12  emery
 * Initial check-in, works under Windows.
 *
 */

/*
**  ODE integration routines from numerical recipes, specially tailored for
**  integrating ballistic motion.
*/

#include <simulator.h>

#define INTEG_TINY 1.0e-20

static double   dmaxarg1, dmaxarg2;
#define DMAX(a,b)   ( dmaxarg1 = (a) , dmaxarg2 = (b) , \
                      dmaxarg1 > dmaxarg2 ? dmaxarg1 : dmaxarg2 )
#define DMIN(a,b)   ( dmaxarg1 = (a) , dmaxarg2 = (b) , \
                      dmaxarg1 < dmaxarg2 ? dmaxarg1 : dmaxarg2 )
#define SIGN(a,b)   ( (b) >= 0.0 ? fabs(a) : -fabs(a) )


#define SAFETY 0.9
#define GROW 1.5            /* Largest factor by which stepsize can grow. */
#define PGROW -0.25
#define SHRNK 0.5
#define PSHRNK ( -1.0 / 3.0 )
#define ERRCON 0.1296       /* ( GROW / SAFETY ) ^ ( 1 / PGROW ) */

#define R_A2 0.2
#define R_A3 0.3
#define R_A4 0.6
#define R_A5 1.0
#define R_A6 0.875
#define R_B21 0.2
#define R_B31 ( 3.0 / 40.0 )
#define R_B32 ( 9.0 / 40.0 )
#define R_B41 0.3
#define R_B42 -0.9
#define R_B43 1.2
#define R_B51 ( -11.0 / 54.0 )
#define R_B52 2.5
#define R_B53 ( -70.0 / 27.0 )
#define R_B54 ( 35.0 / 27.0 )
#define R_B61 ( 1631.0 / 55296.0 )
#define R_B62 ( 175.0 / 512.0 )
#define R_B63 ( 575.0 / 13824.0 )
#define R_B64 ( 44275.0 / 110592.0 )
#define R_B65 ( 253.0 / 4096.0 )
#define R_C1 ( 37.0 / 378.0 )
#define R_C3 ( 250.0 / 621.0 )
#define R_C4 ( 125.0 / 594.0 )
#define R_C6 ( 512.0 / 1771.0 )
#define R_DC5 ( -277.0 / 14336.0 )
#define R_DC1 ( R_C1 - 2825.0 / 27648.0 )
#define R_DC3 ( R_C3 - 18575.0 / 48384.0 )
#define R_DC4 ( R_C4 - 13525.0 / 55296.0 )
#define R_DC6 ( R_C6 - 0.25 )

static double	y[4];
static double   dydx[4];
static double   yscal[4];
static double   err[4];
static double   ytemp1[4];
static double   ytemp2[4];
static double   ak2[4];
static double	ak3[4];
static double	ak4[4];
static double	ak5[4];
static double	ak6[4];


static void
Runge_Kutta_Step(Vector *w, double x, double h, double yout[], double yerr[])
{
    int i;

    for ( i = 0 ; i < 4 ; i++ )
        ytemp2[i] = y[i] + R_B21 * h * dydx[i];
    ak2[0] = -0.5 * ( ytemp2[1] * w->x + ytemp2[2] * w->y + ytemp2[3] * w->z );
    ak2[1] = 0.5 * ( ytemp2[0] * w->x - ytemp2[3] * w->y + ytemp2[2] * w->z );
    ak2[2] = 0.5 * ( ytemp2[3] * w->x + ytemp2[0] * w->y - ytemp2[1] * w->z );
    ak2[3] = 0.5 * ( -ytemp2[2] * w->x + ytemp2[1] * w->y + ytemp2[0] * w->z );
    for ( i = 0 ; i < 4 ; i++ )
        ytemp2[i] = y[i] + h * ( R_B31 * dydx[i] + R_B32 * ak2[i] );
    ak3[0] = -0.5 * ( ytemp2[1] * w->x + ytemp2[2] * w->y + ytemp2[3] * w->z );
    ak3[1] = 0.5 * ( ytemp2[0] * w->x - ytemp2[3] * w->y + ytemp2[2] * w->z );
    ak3[2] = 0.5 * ( ytemp2[3] * w->x + ytemp2[0] * w->y - ytemp2[1] * w->z );
    ak3[3] = 0.5 * ( -ytemp2[2] * w->x + ytemp2[1] * w->y + ytemp2[0] * w->z );
    for ( i = 0 ; i < 4 ; i++ )
        ytemp2[i] = y[i] + h * ( R_B41*dydx[i] + R_B42*ak2[i] + R_B43*ak3[i] );
    ak4[0] = -0.5 * ( ytemp2[1] * w->x + ytemp2[2] * w->y + ytemp2[3] * w->z );
    ak4[1] = 0.5 * ( ytemp2[0] * w->x - ytemp2[3] * w->y + ytemp2[2] * w->z );
    ak4[2] = 0.5 * ( ytemp2[3] * w->x + ytemp2[0] * w->y - ytemp2[1] * w->z );
    ak4[3] = 0.5 * ( -ytemp2[2] * w->x + ytemp2[1] * w->y + ytemp2[0] * w->z );
    for ( i = 0 ; i < 4 ; i++ )
        ytemp2[i] = y[i] + h *
                ( R_B51*dydx[i] + R_B52*ak2[i] + R_B53*ak3[i] + R_B54*ak4[i] );
    ak5[0] = -0.5 * ( ytemp2[1] * w->x + ytemp2[2] * w->y + ytemp2[3] * w->z );
    ak5[1] = 0.5 * ( ytemp2[0] * w->x - ytemp2[3] * w->y + ytemp2[2] * w->z );
    ak5[2] = 0.5 * ( ytemp2[3] * w->x + ytemp2[0] * w->y - ytemp2[1] * w->z );
    ak5[3] = 0.5 * ( -ytemp2[2] * w->x + ytemp2[1] * w->y + ytemp2[0] * w->z );
    for ( i = 0 ; i < 4 ; i++ )
        ytemp2[i] = y[i] + h * ( R_B61*dydx[i] + R_B62*ak2[i] + R_B63*ak3[i] +
                                 R_B64*ak4[i] + R_B65*ak5[i] );
    ak6[0] = -0.5 * ( ytemp2[1] * w->x + ytemp2[2] * w->y + ytemp2[3] * w->z );
    ak6[1] = 0.5 * ( ytemp2[0] * w->x - ytemp2[3] * w->y + ytemp2[2] * w->z );
    ak6[2] = 0.5 * ( ytemp2[3] * w->x + ytemp2[0] * w->y - ytemp2[1] * w->z );
    ak6[3] = 0.5 * ( -ytemp2[2] * w->x + ytemp2[1] * w->y + ytemp2[0] * w->z );
    for ( i = 0 ; i < 4 ; i++ )
        yout[i] = y[i] + h * ( R_C1*dydx[i] + R_C3*ak3[i] + R_C4*ak4[i] +
                               R_C6*ak6[i] );
    for ( i = 0 ; i < 4 ; i++ )
        yerr[i] = h * ( R_DC1 * dydx[i] + R_DC3 * ak3[i] + R_DC4 * ak4[i] +
                        R_DC5 * ak5[i] + R_DC6 * ak6[i] );
}


static bool
Runge_Kutta(Vector *w, double *x, double htry, double eps,
            double *hdid, double *hnext)
{
    int     i;
    double  errmax, h, htemp, xnew;

    h = htry;
    for ( ; ; )
    {
        Runge_Kutta_Step(w, *x, h, ytemp1, err);
        errmax = 0.0;
        for ( i = 0 ; i < 4 ; i++ )
            errmax = DMAX(errmax, fabs(err[i] / yscal[i]));
        errmax /= eps;
        if ( errmax < 1.0 )
            break;
        htemp = SAFETY * h * pow(errmax, PSHRNK);
        h = ( h > 0.0 ? DMAX(htemp, 0.1 * h) : DMIN(htemp, 0.1 * h));
        xnew = (*x) + h;
        if ( xnew == *x )
        {
            fprintf(stderr, "Stepsize underflow in Runge_Kutta\n");
	    return false;
        }
    }
    if ( errmax > ERRCON )
        *hnext = SAFETY * h * pow(errmax, PGROW);
    else
        *hnext = 5.0 * h;
    *x += ( *hdid = h );
    for ( i = 0 ; i < 4 ; i++ )
        y[i] = ytemp1[i];

    return true;
}


#define MAXSTP 10000

static bool
Ballistic_Integrate(Quaternion *in, Vector *w, Quaternion *out,
                    double x1, double x2, double eps, double h1, double hmin)
{
    int     	nstp, i;
    double  	x, hnext, hdid, h;

    x = x1;
    h = SIGN(h1, x2 - x1);
    y[0] = in->real_part;
    y[1] = in->vect_part.x;
    y[2] = in->vect_part.y;
    y[3] = in->vect_part.z;
    for ( nstp = 0 ; nstp < MAXSTP ; nstp++ )
    {
	dydx[0] = -0.5 * ( y[1] * w->x + y[2] * w->y + y[3] * w->z );
	dydx[1] = 0.5 * ( y[0] * w->x - y[3] * w->y + y[2] * w->z );
	dydx[2] = 0.5 * ( y[3] * w->x + y[0] * w->y - y[1] * w->z );
	dydx[3] = 0.5 * ( -y[2] * w->x + y[1] * w->y + y[0] * w->z );

        for ( i = 0 ; i < 4 ; i++ )
            yscal[i] = fabs(y[i]) + fabs(dydx[i] * h) + INTEG_TINY;

        if ( ( x + h - x2 ) * ( x + h - x1 ) > 0.0 )
            h = x2 - x;

	Runge_Kutta(w, &x, h, eps, &hdid, &hnext);

        if ( ( x - x2 ) * ( x2 - x1 ) >= 0.0 )
        {
	    out->real_part = y[0];
	    out->vect_part.x = y[1];
	    out->vect_part.y = y[2];
	    out->vect_part.z = y[3];
            return true;
        }
        if ( fabs(hnext) <= hmin )
        {
            fprintf(stderr, "Stepsize too small in ODE_Integrate\n");
            return false;
        }
        h = hnext;
    }

    fprintf(stderr, "Too many steps in ODE_Integrate\n");
    return false;
}


bool
Integrate_Object(SimulationPtr sim, DynamicStatePtr in, DynamicStatePtr out,
                 double t_next, SimObjectPtr obj)
{
    double  dt = t_next - in->t;
    double  h_min = 1.0e-6;

    if ( dt < 0.0 )
	return true;

    if ( in->stable || dt == 0.0 )
    {
	if ( in != out )
	    *out = *in;
	return true;
    }

    if ( sim->epoch_interval > 0.0 && obj && ( in == out ) )
	Bound_Increment(sim, obj, in, dt);

    if ( sim->do_statistics )
    {
	int log_dt = (int)floor(log10(dt));
	if ( log_dt < INTEG_MIN_STEP )
	    log_dt = INTEG_MIN_STEP;
	if ( log_dt > INTEG_MAX_STEP )
	    log_dt = INTEG_MAX_STEP;
	sim->integrator_steps[log_dt - INTEG_MIN_STEP]++;
    }

    out->x.x = in->x.x + in->v.x * dt;
    out->x.y = in->x.y + in->v.y * dt;
    out->x.z = in->x.z + ( in->v.z - 0.5 * sim->gravity * dt ) * dt;

    if ( h_min > dt * 0.1 )
	h_min = dt * 0.1;

    if ( ! Ballistic_Integrate(&(in->o), &(in->w), &(out->o), in->t, t_next,
		               INTEG_ERROR_BOUND, dt < 1.0 ? dt : 1.0, h_min) )
    {
	fprintf(stderr, "In: %15.15g %15.15g %15.15g %15.15g\n",
		in->o.real_part, in->o.vect_part.x, in->o.vect_part.y,
		in->o.vect_part.z);
	fprintf(stderr, "W: %15.15g %15.15g %15.15g\n",
		in->w.x, in->w.y, in->w.z);
	fprintf(stderr, "dt: %15.15g\n", dt);
	return false;
    }

    QNorm(out->o);

    out->v.x = in->v.x;
    out->v.y = in->v.y;
    out->v.z = in->v.z - sim->gravity * dt;
    out->w = in->w;

    out->t = t_next;

    out->stable = in->stable;

    return true;
}


void
Integrate_Init()
{
}

