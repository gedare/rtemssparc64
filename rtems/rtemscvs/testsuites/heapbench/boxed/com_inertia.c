/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/com_inertia.c,v 1.1 2001/05/01 21:23:08 emery Exp $
 *
 * $Log: com_inertia.c,v $
 * Revision 1.1  2001/05/01 21:23:08  emery
 * Initial check-in, works under Windows.
 *
 */

#define MASS_INERTIA_CHECK 0

	/*******************************************************
        *                                                      *
	*  volInt.c                                            *
	*                                                      *
	*  This code computes volume integrals needed for      *
	*  determining mass properties of polyhedral bodies.   *
	*                                                      *
	*  For more information, see the accompanying README   *
	*  file, and the paper                                 *
	*                                                      *
	*  Brian Mirtich, "Fast and Accurate Computation of    *
	*  Polyhedral Mass Properties," journal of graphics    *
	*  tools, volume 1, number 1, 1996.                    *
	*                                                      *
	*  This source code is public domain, and may be used  *
	*  in any way, shape or form, free of charge.          *
	*                                                      *
	*  Copyright 1995 by Brian Mirtich                     *
	*                                                      *
	*  mirtich@cs.berkeley.edu                             *
	*  http://www.cs.berkeley.edu/~mirtich                 *
        *                                                      *
	*******************************************************/

/*
	Revision history

	26 Jan 1996	Program creation.

	 3 Aug 1996	Corrected bug arising when polyhedron density
			is not 1.0.  Changes confined to function main().
			Thanks to Zoran Popovic for catching this one.

	27 May 1997     Corrected sign error in translation of inertia
	                product terms to center of mass frame.  Changes 
			confined to function main().  Thanks to 
			Chris Hecker.
*/


#include <stdio.h>
#include <math.h>
#include <Vector.h>
#include <quaternion.h>

#define X 0
#define Y 1
#define Z 2

/*
   ============================================================================
   macros
   ============================================================================
*/

#define SQR(x) ((x)*(x))
#define CUBE(x) ((x)*(x)*(x))


/*
   ============================================================================
   globals
   ============================================================================
*/

/* projection integrals */
static double P1, Pa, Pb, Paa, Pab, Pbb, Paaa, Paab, Pabb, Pbbb;

/* face integrals */
static double Fa, Fb, Fc, Faa, Fbb, Fcc, Faaa, Fbbb, Fccc, Faab, Fbbc, Fcca;

/* volume integrals */
static double T0, T1[3], T2[3], TP[3];


#define SIGN(a,b)   ( (b) >= 0.0 ? fabs(a) : -fabs(a) )

/* Routines for computing eigenvectors and eigenvalues. */


static void
Tri_Diag(double **a, int n, double *d, double *e)
{
    /* Taken from Numerical Recipes function tred2. */

    int	    i, j, k, l;
    double  scale, hh, h, g, f;

    for ( i = n - 1 ; i >= 1 ; i-- )
    {
	l = i - 1;
	h = scale = 0.0;
	if ( l > 0 )
	{
	    for ( k = 0 ; k <= l ; k++ )
		scale += fabs((double)a[i][k]);
	    if ( scale == 0.0 )
		e[i] = a[i][l];
	    else
	    {
		for ( k = 0 ; k <= l ; k++ )
		{
		    a[i][k] /= scale;
		    h += a[i][k] * a[i][k];
		}
		f = a[i][l];
		g = ( f >= 0 ? -sqrt(h) : sqrt(h) );
		e[i] = scale * g;
		h -= f * g;
		a[i][l] = f - g;
		f = 0.0;
		for ( j = 0 ; j <= l ; j++ )
		{
		    a[j][i] = a[i][j] / h;
		    g = 0.0;
		    for ( k = 0 ; k <= j ; k++ )
			g += a[j][k] * a[i][k];
		    for ( k = j + 1 ; k <= l ; k++ )
			g += a[k][j] * a[i][k];
		    e[j] = g / h;
		    f += e[j] * a[i][j];
		}
		hh = f / ( h + h );
		for ( j = 0 ; j <= l ; j++ )
		{
		    f = a[i][j];
		    e[j] = g = e[j] - hh * f;
		    for ( k = 0 ; k <= j ; k++ )
			a[j][k] -= ( f * e[k] + g * a[i][k] );
		}
	    }
	}
	else
	    e[i] = a[i][l];
	d[i] = h;
    }
    d[0] = 0.0;
    e[0] = 0.0;
    for ( i = 0 ; i < n ; i++ )
    {
	l = i - 1;
	if ( d[i] != 0.0 )
	{
	    for ( j = 0 ; j <= l ; j++ )
	    {
		g = 0.0;
		for ( k = 0 ; k <= l ; k++ )
		    g += a[i][k] * a[k][j];
		for ( k = 0 ; k <= l ; k++ )
		    a[k][j] -= g * a[k][i];
	    }
	}
	d[i] = a[i][i];
	a[i][i] = 1.0;
	for ( j = 0 ; j <= l ; j++ )
	    a[j][i] = a[i][j] = 0.0;
    }
}


static double
Pythag(double a, double b)
{
    double  absa = fabs(a);
    double  absb = fabs(b);
    double  temp;

    if ( absa > absb )
    {
	temp = absb / absa;
	return absa * sqrt(1.0 + temp * temp);
    }
    else
    {
	temp = absa / absb;
	return ( absb == 0.0 ? 0.0 : absb * sqrt(1.0 + temp * temp) );
    }
}

static void
Tri_QL_Implicit(double *d, double *e, int n, double **z)
{
    int	    m, l, iter, i, k;
    double  s, r, p, g, f, dd, c, b;

    for ( i = 1 ; i < n ; i++ )
	e[i - 1] = e[i];
    e[n - 1] = 0.0;
    for ( l = 0 ; l < n ; l++ )
    {
	iter = 0;
	do
	{
	    for ( m = l ; m <= n - 2 ; m++ )
	    {
		dd = fabs(d[m]) + fabs(d[m+1]);
		if ( fabs(e[m]) + dd == dd )
		    break;
	    }
	    if ( m != l )
	    {
		if ( iter++ == 30 )
		{
		    fprintf(stderr, "Too many iterations in Tri_QL_Implicit\n");
		    exit(2);
		}
		g = ( d[l + 1] - d[l] ) / ( 2.0 * e[l] );
		r = Pythag(g, 1.0);
		g = d[m] - d[l] + e[l] / ( g + SIGN(r, g));
		s = c = 1.0;
		p = 0.0;
		for ( i = m - 1 ; i >= l ; i-- )
		{
		    f = s * e[i];
		    b = c * e[i];
		    e[i+1] = ( r = Pythag(f, g) );
		    if ( r == 0.0 )
		    {
			d[i+1] -= p;
			e[m] = 0.0;
			break;
		    }
		    s = f / r;
		    c = g / r;
		    g = d[i+1] - p;
		    r = ( d[i] - g ) * s + 2.0 * c * b;
		    d[i+1] = g + ( p = s * r );
		    g = c * r - b;
		    for ( k = 0 ; k < n ; k++ )
		    {
			f = z[k][i+1];
			z[k][i+1] = s * z[k][i] + c * f;
			z[k][i] = c * z[k][i] - s * f;
		    }
		}
		if ( r == 0.0 && i >= l )
		    continue;
		d[l] -= p;
		e[l] = g;
		e[m] = 0.0;
	    }
	} while ( m != l );
    }
}


/*
   ============================================================================
   compute mass properties
   ============================================================================
*/


/* compute various integrations over projection of face */
static void
compProjectionIntegrals(Vector *verts, int fc, int *f, int a)
{
  double a0, a1, da;
  double b0, b1, db;
  double a0_2, a0_3, a0_4, b0_2, b0_3, b0_4;
  double a1_2, a1_3, b1_2, b1_3;
  double C1, Ca, Caa, Caaa, Cb, Cbb, Cbbb;
  double Cab, Kab, Caab, Kaab, Cabb, Kabb;
  int i;

  P1 = Pa = Pb = Paa = Pab = Pbb = Paaa = Paab = Pabb = Pbbb = 0.0;

  for ( i = 0 ; i < fc ; i++ )
  {
    if ( a == X )
    {
      a0 = verts[f[i]].x;
      b0 = verts[f[i]].y;
      a1 = verts[f[(i+1) % fc]].x;
      b1 = verts[f[(i+1) % fc]].y;
    }
    else if ( a == Y )
    {
      a0 = verts[f[i]].y;
      b0 = verts[f[i]].z;
      a1 = verts[f[(i+1) % fc]].y;
      b1 = verts[f[(i+1) % fc]].z;
    }
    else
    {
      a0 = verts[f[i]].z;
      b0 = verts[f[i]].x;
      a1 = verts[f[(i+1) % fc]].z;
      b1 = verts[f[(i+1) % fc]].x;
    }

    da = a1 - a0;
    db = b1 - b0;
    a0_2 = a0 * a0; a0_3 = a0_2 * a0; a0_4 = a0_3 * a0;
    b0_2 = b0 * b0; b0_3 = b0_2 * b0; b0_4 = b0_3 * b0;
    a1_2 = a1 * a1; a1_3 = a1_2 * a1; 
    b1_2 = b1 * b1; b1_3 = b1_2 * b1;

    C1 = a1 + a0;
    Ca = a1*C1 + a0_2; Caa = a1*Ca + a0_3; Caaa = a1*Caa + a0_4;
    Cb = b1*(b1 + b0) + b0_2; Cbb = b1*Cb + b0_3; Cbbb = b1*Cbb + b0_4;
    Cab = 3*a1_2 + 2*a1*a0 + a0_2; Kab = a1_2 + 2*a1*a0 + 3*a0_2;
    Caab = a0*Cab + 4*a1_3; Kaab = a1*Kab + 4*a0_3;
    Cabb = 4*b1_3 + 3*b1_2*b0 + 2*b1*b0_2 + b0_3;
    Kabb = b1_3 + 2*b1_2*b0 + 3*b1*b0_2 + 4*b0_3;

    P1 += db*C1;
    Pa += db*Ca;
    Paa += db*Caa;
    Paaa += db*Caaa;
    Pb += da*Cb;
    Pbb += da*Cbb;
    Pbbb += da*Cbbb;
    Pab += db*(b1*Cab + b0*Kab);
    Paab += db*(b1*Caab + b0*Kaab);
    Pabb += da*(a1*Cabb + a0*Kabb);
  }

  P1 /= 2.0;
  Pa /= 6.0;
  Paa /= 12.0;
  Paaa /= 20.0;
  Pb /= -6.0;
  Pbb /= -12.0;
  Pbbb /= -20.0;
  Pab /= 24.0;
  Paab /= 60.0;
  Pabb /= -60.0;
}

static void
compFaceIntegrals(Vector *verts, int fc, int *f, Vector norm, int a)
{
  double  n[3], w;
  double  k1, k2, k3, k4;
  int	  b = ( a + 1 ) % 3;
  int	  c = ( b + 1 ) % 3;

  compProjectionIntegrals(verts, fc, f, a);

  w = -VDot(norm, verts[f[0]]);
  n[X] = norm.x;
  n[Y] = norm.y;
  n[Z] = norm.z;
  k1 = 1 / n[c];
  k2 = k1 * k1; k3 = k2 * k1; k4 = k3 * k1;

  Fa = k1 * Pa;
  Fb = k1 * Pb;
  Fc = -k2 * (n[a]*Pa + n[b]*Pb + w*P1);

  Faa = k1 * Paa;
  Fbb = k1 * Pbb;
  Fcc = k3 * (SQR(n[a])*Paa + 2*n[a]*n[b]*Pab + SQR(n[b])*Pbb
	 + w*(2*(n[a]*Pa + n[b]*Pb) + w*P1));

  Faaa = k1 * Paaa;
  Fbbb = k1 * Pbbb;
  Fccc = -k4 * (CUBE(n[a])*Paaa + 3*SQR(n[a])*n[b]*Paab 
	   + 3*n[a]*SQR(n[b])*Pabb + CUBE(n[b])*Pbbb
	   + 3*w*(SQR(n[a])*Paa + 2*n[a]*n[b]*Pab + SQR(n[b])*Pbb)
	   + w*w*(3*(n[a]*Pa + n[b]*Pb) + w*P1));

  Faab = k1 * Paab;
  Fbbc = -k2 * (n[a]*Pabb + n[b]*Pbbb + w*Pbb);
  Fcca = k3 * (SQR(n[a])*Paaa + 2*n[a]*n[b]*Paab + SQR(n[b])*Pabb
	 + w*(2*(n[a]*Paa + n[b]*Pab) + w*Pa));
}

static void
compVolumeIntegrals(Vector *verts, int num_f, int *fc, int **f)
{
  Vector  n;
  double  nx, ny, nz;
  int	  a, b, c;
  Vector  v1, v2;
  int 	  i;
  double  temp_d;

  T0 = T1[X] = T1[Y] = T1[Z] 
     = T2[X] = T2[Y] = T2[Z] 
     = TP[X] = TP[Y] = TP[Z] = 0;

  for ( i = 0 ; i < num_f ; i++ )
  {
    VSub(verts[f[i][1]], verts[f[i][0]], v1);
    VSub(verts[f[i][2]], verts[f[i][0]], v2);
    VCross(v1, v2, n);
    VUnit(n, temp_d, n);

    nx = fabs(n.x);
    ny = fabs(n.y);
    nz = fabs(n.z);
    if (nx > ny && nx > nz) c = X;
    else c = (ny > nz) ? Y : Z;
    a = (c + 1) % 3;
    b = (a + 1) % 3;

    compFaceIntegrals(verts, fc[i], f[i], n, a);

    T0 += n.x * ((a == X) ? Fa : ((b == X) ? Fb : Fc));

    if ( a == X )
    {
      T1[a] += n.x * Faa;
      T1[b] += n.y * Fbb;
      T1[c] += n.z * Fcc;
      T2[a] += n.x * Faaa;
      T2[b] += n.y * Fbbb;
      T2[c] += n.z * Fccc;
      TP[a] += n.x * Faab;
      TP[b] += n.y * Fbbc;
      TP[c] += n.z * Fcca;
    }
    else if ( a == Y )
    {
      T1[a] += n.y * Faa;
      T1[b] += n.z * Fbb;
      T1[c] += n.x * Fcc;
      T2[a] += n.y * Faaa;
      T2[b] += n.z * Fbbb;
      T2[c] += n.x * Fccc;
      TP[a] += n.y * Faab;
      TP[b] += n.z * Fbbc;
      TP[c] += n.x * Fcca;
    }
    else if ( a == Z )
    {
      T1[a] += n.z * Faa;
      T1[b] += n.x * Fbb;
      T1[c] += n.y * Fcc;
      T2[a] += n.z * Faaa;
      T2[b] += n.x * Fbbb;
      T2[c] += n.y * Fccc;
      TP[a] += n.z * Faab;
      TP[b] += n.x * Fbbc;
      TP[c] += n.y * Fcca;
    }

  }

  T1[X] /= 2; T1[Y] /= 2; T1[Z] /= 2;
  T2[X] /= 3; T2[Y] /= 3; T2[Z] /= 3;
  TP[X] /= 2; TP[Y] /= 2; TP[Z] /= 2;
}


void
Mass_Inertia(int num_vertices, Vector *vertices, int num_faces,
	     int *face_counts, int **faces,
	     double density, double *mass, Vector *inertia, Quaternion *rot,
	     Vector *trans)
{
  Vector  	com;
  double  	J[3][3];
  double  	*a[3];
  double  	a0[3], a1[3], a2[3];
  double  	d[3], e[3];
  Matrix    	rotate;
  Quaternion	q_rot;
  double  	det;
  Vector  	temp_v;
  int	  	i;

  compVolumeIntegrals(vertices, num_faces, face_counts, faces);

#if MASS_INERTIA_CHECK
  fprintf(stderr, "\nT1 =   %+20.6g\n\n", T0);

  fprintf(stderr, "Tx =   %+20.6g\n", T1[X]);
  fprintf(stderr, "Ty =   %+20.6g\n", T1[Y]);
  fprintf(stderr, "Tz =   %+20.6g\n\n", T1[Z]);
  
  fprintf(stderr, "Txx =  %+20.6g\n", T2[X]);
  fprintf(stderr, "Tyy =  %+20.6g\n", T2[Y]);
  fprintf(stderr, "Tzz =  %+20.6g\n\n", T2[Z]);

  fprintf(stderr, "Txy =  %+20.6g\n", TP[X]);
  fprintf(stderr, "Tyz =  %+20.6g\n", TP[Y]);
  fprintf(stderr, "Tzx =  %+20.6g\n\n", TP[Z]);
#endif

  *mass = density * T0;

  /* compute center of mass */
  com.x = T1[X] / T0;
  com.y = T1[Y] / T0;
  com.z = T1[Z] / T0;

  /* compute inertia tensor */
  J[X][X] = density * (T2[Y] + T2[Z]);
  J[Y][Y] = density * (T2[Z] + T2[X]);
  J[Z][Z] = density * (T2[X] + T2[Y]);
  J[X][Y] = J[Y][X] = - density * TP[X];
  J[Y][Z] = J[Z][Y] = - density * TP[Y];
  J[Z][X] = J[X][Z] = - density * TP[Z];

  /* translate inertia tensor to center of mass */
  J[X][X] -= *mass * (com.y*com.y + com.z*com.z);
  J[Y][Y] -= *mass * (com.z*com.z + com.x*com.x);
  J[Z][Z] -= *mass * (com.x*com.x + com.y*com.y);
  J[X][Y] = J[Y][X] += *mass * com.x * com.y; 
  J[Y][Z] = J[Z][Y] += *mass * com.y * com.z; 
  J[Z][X] = J[X][Z] += *mass * com.z * com.x; 

  /* Find eigenvalues. */
  a[0] = a0;
  a[0][0] = J[0][0]; a[0][1] = J[0][1]; a[0][2] = J[0][2];
  a[1] = a1;
  a[1][0] = J[1][0]; a[1][1] = J[1][1]; a[1][2] = J[1][2];
  a[2] = a2;
  a[2][0] = J[2][0]; a[2][1] = J[2][1]; a[2][2] = J[2][2];
  Tri_Diag(a, 3, d, e);
  Tri_QL_Implicit(d, e, 3, a);
  rotate.x.x = a[0][0];
  rotate.x.y = a[1][0];
  rotate.x.z = a[2][0];
  rotate.y.x = a[0][1];
  rotate.y.y = a[1][1];
  rotate.y.z = a[2][1];
  rotate.z.x = a[0][2];
  rotate.z.y = a[1][2];
  rotate.z.z = a[2][2];
  det = rotate.x.x * ( rotate.y.y * rotate.z.z - rotate.z.y * rotate.y.z )
      - rotate.x.y * ( rotate.y.x * rotate.z.z - rotate.z.x * rotate.y.z )
      + rotate.x.z * ( rotate.y.x * rotate.z.y - rotate.z.x * rotate.y.y );
  if ( det < 0.0 )
  {
    double  temp_d;
    Vector  temp_v;

    temp_d = d[1];
    d[1] = d[2];
    d[2] = temp_d;
    temp_v = rotate.y;
    rotate.y = rotate.z;
    rotate.z = temp_v;
  }
  inertia->x = d[0];
  inertia->y = d[1];
  inertia->z = d[2];

  Matrix_To_Quaternion(&rotate, &q_rot);
  for ( i = 0 ; i < num_vertices ; i++ )
  {
    VSub(vertices[i], com, temp_v);
    QVMul(q_rot, temp_v, vertices[i]);
  }

  QInvert(q_rot, *rot);
  *trans = com;

#if MASS_INERTIA_CHECK
  fprintf(stderr, "mass:  %+12.6g\n\n", *mass);

  fprintf(stderr, "center of mass:  (%+12.6g,%+12.6g,%+12.6g)\n\n",
	  com.x, com.y, com.z);

  fprintf(stderr, "inertia tensor with origin at c.o.m. :\n");
  fprintf(stderr, "%+15.6g  %+15.6g  %+15.6g\n", J[X][X], J[X][Y], J[X][Z]);
  fprintf(stderr, "%+15.6g  %+15.6g  %+15.6g\n", J[Y][X], J[Y][Y], J[Y][Z]);
  fprintf(stderr, "%+15.6g  %+15.6g  %+15.6g\n\n", J[Z][X], J[Z][Y], J[Z][Z]);

  fprintf(stderr, "\nPrinciple components:\n");
  fprintf(stderr, "%+15.6g  %+15.6g  %+15.6g\n",
	  inertia->x, inertia->y, inertia->z);

#endif
  
  return;
}


