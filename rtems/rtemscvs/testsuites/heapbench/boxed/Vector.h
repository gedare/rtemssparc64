/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/* $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/Vector.h,v 1.1 2001/05/01 21:23:04 emery Exp $
 * 
 * $Log: Vector.h,v $
 * Revision 1.1  2001/05/01 21:23:04  emery
 * Initial check-in, works under Windows.
 *
 * Revision 1.0  1997/02/27 01:36:40  schenney
 * Initial revision
 *
 */
/****************************************************************/
/*                                                              */
/*  Header file for the vector macros.                          */
/*  Written by Stephen Chenney.  Dec 1993.                      */
/*								*/
/*  (c) Stephen Chenney 1993-1999                               */
/*                                                              */
/****************************************************************/

#ifndef __VECTOR__
#define __VECTOR__

/* Define a vector.  */
typedef struct _Vector
    {
        double  x;
        double  y;
        double  z;
    } Vector, *VectorPtr;
typedef struct _Vectorf
    {
        float  x;
        float  y;
        float  z;
    } Vectorf, *VectorfPtr;

/* A Matrix is a set of vectors. */
typedef struct _Matrix {
    Vector x;
    Vector y;
    Vector z;
    } Matrix, *MatrixPtr;
typedef struct _Matrixf {
    Vectorf x;
    Vectorf y;
    Vectorf z;
    } Matrixf, *MatrixfPtr;

#define VtoVf(v, f) ( (f).x = (v).x, (f).y = (v).y, (f).z = (v).z )

#define VECT_EPSILON 1.e-13
#define VECT_EPSILON_SQ 1.e-26

/* Zero vector check. */
#define VNearZero(v)    ((v).x < VECT_EPSILON && \
                         (v).x > -VECT_EPSILON && \
                         (v).y < VECT_EPSILON && \
                         (v).y > -VECT_EPSILON && \
                         (v).z < VECT_EPSILON && \
                         (v).z > -VECT_EPSILON )

#define VZero(v)        ( VNearZero(v) && VDot((v), (v)) < VECT_EPSILON_SQ )
#define VEqual(a, b, d) ( VSub((a), (b), (d)), VNearZero(d) && VZero(d))

#define MZero(m)    (VZero((m).x) && VZero((m).y) && VZero((m).z))

/* Takes the modulus of v */
#define VMod(v)     (sqrt((v).x*(v).x + (v).y*(v).y + (v).z*(v).z))

/* Returns the dot product of v1 & v2 */
#define VDot(v1, v2)    ((v1).x*(v2).x + (v1).y*(v2).y + (v1).z*(v2).z)

/* Fills the fields of a vector.    */
#define VNew(a, b, c, r)    ((r).x = (a), (r).y = (b), (r).z = (c))

#define VAdd(v1, v2, r)     ((r).x = (v1).x + (v2).x , \
                             (r).y = (v1).y + (v2).y , \
                             (r).z = (v1).z + (v2).z )

#define VSub(v1, v2, r)     ((r).x = (v1).x - (v2).x , \
                             (r).y = (v1).y - (v2).y , \
                             (r).z = (v1).z - (v2).z )

#define VScalarMul(v, d, r) ((r).x = (v).x * (d) , \
                             (r).y = (v).y * (d) , \
                             (r).z = (v).z * (d) )

#define VAddMul(v, a, b, r) ((r).x = (v).x + (a) * (b).x, \
                             (r).y = (v).y + (a) * (b).y, \
                             (r).z = (v).z + (a) * (b).z )


#define VElmtMul(v1, v2, r) ((r).x = (v1).x * (v2).x, \
                             (r).y = (v1).y * (v2).y, \
                             (r).z = (v1).z * (v2).z )

#define VCross(v1, v2, r)   ((r).x = (v1).y * (v2).z - (v1).z * (v2).y , \
                             (r).y = (v1).z * (v2).x - (v1).x * (v2).z , \
                             (r).z = (v1).x * (v2).y - (v1).y * (v2).x )

#define VUnit(v, t, r)      ((t) = 1 / VMod(v) , \
                             VScalarMul(v, t, r) )

#define VNegate(v)          ((v).x = -(v).x, (v).y = -(v).y, (v).z = -(v).z )

#define MNew(a, b, c, r)    ((r).x = (a), (r).y = (b) , (r).z = (c) )
#define MIdentity(r)        ((r).x.x = 1.0, (r).x.y = 0.0, (r).x.z = 0.0, \
                             (r).y.x = 0.0, (r).y.y = 1.0, (r).y.z = 0.0, \
                             (r).z.x = 0.0, (r).z.y = 0.0, (r).z.z = 1.0 )

#define MScalarMul(m, d, r) (VScalarMul((m).x, (d), (r).x) , \
                             VScalarMul((m).y, (d), (r).y) , \
                             VScalarMul((m).z, (d), (r).z) )

#define MVMul(m, v, r)      ((r).x = VDot((m).x, (v)) , \
                             (r).y = VDot((m).y, (v)) , \
                             (r).z = VDot((m).z, (v)) )

#define MTrans(m, n)        ((n).x.x = (m).x.x , (n).y.x = (m).x.y , \
                             (n).z.x = (m).x.z , (n).x.y = (m).y.x , \
                             (n).y.y = (m).y.y , (n).z.y = (m).y.z , \
                             (n).x.z = (m).z.x , (n).y.z = (m).z.y , \
                             (n).z.z = (m).z.z )

#define MMMul(a, b, r)      ((r).x.x = (a).x.x * (b).x.x  \
                                     + (a).x.y * (b).y.x  \
                                     + (a).x.z * (b).z.x, \
                             (r).x.y = (a).x.x * (b).x.y  \
                                     + (a).x.y * (b).y.y  \
                                     + (a).x.z * (b).z.y, \
                             (r).x.z = (a).x.x * (b).x.z  \
                                     + (a).x.y * (b).y.z  \
                                     + (a).x.z * (b).z.z, \
                             (r).y.x = (a).y.x * (b).x.x  \
                                     + (a).y.y * (b).y.x  \
                                     + (a).y.z * (b).z.x, \
                             (r).y.y = (a).y.x * (b).x.y  \
                                     + (a).y.y * (b).y.y  \
                                     + (a).y.z * (b).z.y, \
                             (r).y.z = (a).y.x * (b).x.z  \
                                     + (a).y.y * (b).y.z  \
                                     + (a).y.z * (b).z.z, \
                             (r).z.x = (a).z.x * (b).x.x  \
                                     + (a).z.y * (b).y.x  \
                                     + (a).z.z * (b).z.x, \
                             (r).z.y = (a).z.x * (b).x.y  \
                                     + (a).z.y * (b).y.y  \
                                     + (a).z.z * (b).z.y, \
                             (r).z.z = (a).z.x * (b).x.z  \
                                     + (a).z.y * (b).y.z  \
                                     + (a).z.z * (b).z.z )



/* Printing functions. */
#define VPrint(of, v)   (fprintf(of,"%1.6g %1.6g %1.6g\n",(v).x,(v).y,(v).z))
#define MPrint(of, m)   (VPrint(of,(m).x), VPrint(of,(m).y), VPrint(of,(m).z))
#define VRead(s, v) ( sscanf(s, "%lg %lg %lg", &(v).x, &(v).y, &(v).z) )
#define VFRead(f, v)    ( fscanf(f, "%lg %lg %lg", &(v).x, &(v).y, &(v).z) )

/* External functions. */
extern Matrix MInvert(Matrix*);

#endif
