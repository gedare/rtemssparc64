/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include <Vector.h>

typedef struct _Quaternion {
    Vector  vect_part;
    double  real_part;
    } Quaternion;

typedef struct _Quaternionf {
    Vectorf vect_part;
    float   real_part;
    } Quaternionf;

#define QtoQf(a, b) ( VtoVf((a).vect_part, (b).vect_part), \
                      (b).real_part = (a).real_part )

#define QToM(q, m)  { double _xx = (q).vect_part.x * (q).vect_part.x; \
		      double _yy = (q).vect_part.y * (q).vect_part.y; \
		      double _zz = (q).vect_part.z * (q).vect_part.z; \
    		      double _xy = (q).vect_part.x * (q).vect_part.y; \
    		      double _xz = (q).vect_part.x * (q).vect_part.z; \
    		      double _yz = (q).vect_part.y * (q).vect_part.z; \
    		      double _wx = (q).real_part * (q).vect_part.x; \
    		      double _wy = (q).real_part * (q).vect_part.y; \
    		      double _wz = (q).real_part * (q).vect_part.z; \
		      (m).x.x = 1 - 2 * _yy - 2 * _zz; \
    		      (m).x.y = 2 * _xy - 2 * _wz; \
		      (m).x.z = 2 * _xz + 2 * _wy; \
    		      (m).y.x = 2 * _xy + 2 * _wz; \
		      (m).y.y = 1 - 2 * _xx - 2 * _zz; \
		      (m).y.z = 2 * _yz - 2 * _wx; \
    		      (m).z.x = 2 * _xz - 2 * _wy; \
		      (m).z.y = 2 * _yz + 2 * _wx; \
		      (m).z.z = 1 - 2 * _xx - 2 * _yy; }

#define QVMul(q, v, r) { double _q_dot = VDot((q).vect_part, (v)); \
                         Vector _q_temp; \
			 _q_temp.x = -(v).y * (q).vect_part.z \
				   + (v).z * (q).vect_part.y \
				   + (q).real_part * (v).x; \
			 _q_temp.y = -(v).z * (q).vect_part.x \
				   + (v).x * (q).vect_part.z \
				   + (q).real_part * (v).y; \
			 _q_temp.z = -(v).x * (q).vect_part.y \
				   + (v).y * (q).vect_part.x \
				   + (q).real_part * (v).z; \
			 VCross((q).vect_part, _q_temp, (r)); \
			 (r).x += (q).real_part * _q_temp.x \
			       + _q_dot * (q).vect_part.x; \
			 (r).y += (q).real_part * _q_temp.y \
			       + _q_dot * (q).vect_part.y; \
			 (r).z += (q).real_part * _q_temp.z \
			       + _q_dot * (q).vect_part.z; }

#define QInvert(q, r)	( (r).real_part = (q).real_part, \
                          (r).vect_part.x = -1.0 * (q).vect_part.x, \
                          (r).vect_part.y = -1.0 * (q).vect_part.y, \
                          (r).vect_part.z = -1.0 * (q).vect_part.z )

#define QNorm(q)    { double _length = (q).real_part * (q).real_part \
                                     + (q).vect_part.x * (q).vect_part.x \
                                     + (q).vect_part.y * (q).vect_part.y \
                                     + (q).vect_part.z * (q).vect_part.z; \
		      if ( _length-1.0 < -1.0e-16 || _length-1.0 > 1.0e-16 ) { \
			  _length = 1.0 / sqrt(_length); \
			  (q).real_part *= _length; \
			  (q).vect_part.x *= _length; \
			  (q).vect_part.y *= _length; \
			  (q).vect_part.z *= _length; \
		      } \
		    }

#define QQMul(a, b, r)	( (r).real_part = (a).real_part * (b).real_part \
                                        - (a).vect_part.x * (b).vect_part.x \
                                        - (a).vect_part.y * (b).vect_part.y \
                                        - (a).vect_part.z * (b).vect_part.z, \
			  (r).vect_part.x = (a).vect_part.y * (b).vect_part.z \
					  - (a).vect_part.z * (b).vect_part.y \
					  + (b).real_part * (a).vect_part.x \
					  + (a).real_part * (b).vect_part.x, \
			  (r).vect_part.y = (a).vect_part.z * (b).vect_part.x \
					  - (a).vect_part.x * (b).vect_part.z \
					  + (b).real_part * (a).vect_part.y \
					  + (a).real_part * (b).vect_part.y, \
			  (r).vect_part.z = (a).vect_part.x * (b).vect_part.y \
					  - (a).vect_part.y * (b).vect_part.x \
					  + (b).real_part * (a).vect_part.z \
					  + (a).real_part * (b).vect_part.z )


#define QPrint(f, q) ( fprintf(f, "%g %g %g %g\n", (q).real_part, \
                           (q).vect_part.x, (q).vect_part.y, (q).vect_part.z) )

extern void	    Quaternion_Normalize(Quaternion*);
extern Matrix       Quaternion_To_Matrix(Quaternion);
extern Quaternion   Axis_Angle_To_Quaternion(Vector, double);
extern Quaternion   QQMul_Func(Quaternion*, Quaternion*);
extern void         Quaternion_To_Axis_Angle(Quaternion*, Vector*, double*);
extern Quaternion   Quaternion_Invert(Quaternion*);
extern Vector	    QVMul_Func(Quaternion *, Vector *);
extern void	    Matrix_To_Axis_Angle(Matrix*, Vector*, double*);
extern void	    Matrix_To_Quaternion(Matrix*, Quaternion*);

extern void	    Quaternionf_Normalize(Quaternionf*);
extern Matrix       Quaternionf_To_Matrix(Quaternionf);
extern Quaternionf  Axis_Angle_To_Quaternionf(Vectorf, float);
extern void         Quaternionf_To_Axis_Angle(Quaternionf*, Vectorf*, float*);
extern Quaternionf  Quaternionf_Invert(Quaternionf*);

#endif
