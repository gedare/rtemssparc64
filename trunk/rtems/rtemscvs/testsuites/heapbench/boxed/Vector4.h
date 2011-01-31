/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/* $Header
 * 
 * $Log
 */
#define PATCHLEVEL 0
/*
**    ScEd: A Constraint Based Scene Editor.
**    Copyright (C) 1994-1995  Stephen Chenney (stephen@cs.su.oz.au)
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program; if not, write to the Free Software
**    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/****************************************************************/
/*                                                              */
/*  Header file for the 4-vector macros.                        */
/*                                                              */
/****************************************************************/

#ifndef __VECTOR4__
#define __VECTOR4__

/* Define a vector.  */
typedef struct _Vector4
    {
        double  x;
        double  y;
        double  z;
        double  w;
    } Vector4, *Vector4Ptr;

/* A Matrix4 is a set of vectors. */
typedef struct _Matrix4 {
    Vector4 x;
    Vector4 y;
    Vector4 z;
    Vector4 w;
    } Matrix4, *Matrix4Ptr;

/* Zero vector check. */
#define V4NearZero(v)   ((v).x < VECT_EPSILON && \
                         (v).x > -VECT_EPSILON && \
                         (v).y < VECT_EPSILON && \
                         (v).y > -VECT_EPSILON && \
                         (v).z < VECT_EPSILON && \
                         (v).z > -VECT_EPSILON && \
                         (v).w < VECT_EPSILON && \
                         (v).w > -VECT_EPSILON )

#define V4Zero(v)       ( V4NearZero(v) && V4Dot((v), (v)) < VECT_EPSILON_SQ )
#define V4Equal(a, b, d)    ( V4Sub((a), (b), (d)), V4NearZero(d) && V4Zero(d))

#define M4Zero(m)   ( V4Zero((m).x) && V4Zero((m).y) && \
                      V4Zero((m).z) && V4Zero((m).w) )

/* Takes the modulus of v */
#define V4Mod(v)    ( sqrt( (v).x * (v).x + (v).y * (v).y + (v).z * (v).z + \
                            (v).w * (v).w ) )

/* Returns the dot product of v1 & v2 */
#define V4Dot(v1, v2)   ( (v1).x * (v2).x + (v1).y * (v2).y + \
                          (v1).z * (v2).z + (v1).w * (v2).w )

/* Fills the fields of a vector.    */
#define V4New(a, b, c, d, r)    ( (r).x = (a), (r).y = (b), (r).z = (c), \
                                  (r).w = (d) )

#define V4Add(v1, v2, r)    ((r).x = (v1).x + (v2).x , \
                             (r).y = (v1).y + (v2).y , \
                             (r).z = (v1).z + (v2).z , \
                             (r).w = (v1).w + (v2).w )

#define V4Sub(v1, v2, r)    ((r).x = (v1).x - (v2).x , \
                             (r).y = (v1).y - (v2).y , \
                             (r).z = (v1).z - (v2).z , \
                             (r).w = (v1).w - (v2).w )

#define V4ScalarMul(v, d, r)    ((r).x = (v).x * (d) , \
                                 (r).y = (v).y * (d) , \
                                 (r).z = (v).z * (d) , \
                                 (r).w = (v).w * (d) )

#define V4Unit(v, t, r)     ((t) = 1 / V4Mod(v) , V4ScalarMul(v, t, r) )

#define UnHomog(v4, v3)     ((v3).x = (v4).x / (v4).w, \
                 (v3).y = (v4).y / (v4).w, \
                 (v3).z = (v4).z / (v4).w);

#define M4New(a, b, c, d, r)    ((r).x = (a), (r).y = (b), (r).z = (c), \
                                 (r).w = (d) )

#define M4ScalarMul(m, d, r)    (VScalarMul((m).x, (d), (r).x) , \
                                 VScalarMul((m).y, (d), (r).y) , \
                                 VScalarMul((m).z, (d), (r).z) , \
                                 VScalarMul((m).w, (d), (r).w) )

#define M4V4Mul(m, v, r)    ((r).x = V4Dot((m).x, (v)) , \
                             (r).y = V4Dot((m).y, (v)) , \
                             (r).z = V4Dot((m).z, (v)) , \
                             (r).w = V4Dot((m).w, (v)) )

#define M4Trans(m, n)       ((n).x.x = (m).x.x , (n).y.x = (m).x.y , \
                             (n).z.x = (m).x.z , (n).w.x = (m).x.w , \
                             (n).x.y = (m).y.x , (n).y.y = (m).y.y , \
                             (n).z.y = (m).y.z , (n).w.y = (m).y.w , \
                             (n).x.z = (m).z.x , (n).y.z = (m).z.y , \
                             (n).z.z = (m).z.z , (n).w.z = (m).z.w , \
                             (n).x.w = (m).w.x , (n).y.w = (m).w.y , \
                             (n).z.w = (m).w.z , (n).w.w = (m).w.w )

#define NewIdentityMatrix4(n)   ((n).x.x = 1.0, (n).x.y = 0.0, (n).x.z = 0.0, \
                                 (n).x.w = 0.0, (n).y.x = 0.0, (n).y.y = 1.0, \
                                 (n).y.z = 0.0, (n).y.w = 0.0, (n).z.x = 0.0, \
                                 (n).z.y = 0.0, (n).z.z = 1.0, (n).z.w = 0.0, \
                                 (n).w.x = 0.0, (n).w.y = 0.0, (n).w.z = 0.0, \
                                 (n).w.w = 1.0 )

#define M4ToM(m4, m3)   ((m3).x.x = (m4).x.x, (m3).x.y = (m4).x.y, \
                         (m3).x.z = (m4).x.z, (m3).y.x = (m4).y.x, \
                         (m3).y.y = (m4).y.y, (m3).y.z = (m4).y.z, \
                         (m3).z.x = (m4).z.x, (m3).z.y = (m4).z.y, \
                         (m3).z.z = (m4).z.z )

#define MToM4(m3, m4)   ((m4).x.x = (m3).x.x, (m4).x.y = (m3).x.y, \
                         (m4).x.z = (m3).x.z, (m4).y.x = (m3).y.x, \
                         (m4).y.y = (m3).y.y, (m4).y.z = (m3).y.z, \
                         (m4).z.x = (m3).z.x, (m4).z.y = (m3).z.y, \
                         (m4).z.z = (m3).z.z )

/* Printing functions. */
#define V4Print(of, v) \
    ( fprintf(of, "%1.15g %1.15g %1.15g %1.15g\n", (v).x, (v).y, (v).z, (v).w) )
#define M4Print(of, m) \
    (V4Print(of, (m).x), V4Print(of, (m).y), V4Print(of, (m).z), V4Print(of, (m).w))


/* Extern declarations for the rest. */
extern int    V4Read(char *s, Vector4 *v);

/* Matrix4 functions. */
extern Matrix4 M4M4Mul(Matrix4 *m1, Matrix4 *m2);
extern Matrix4 M4Invert(Matrix4 *m);


#endif /* __VECTOR4__ */
