/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

#include <boxed.h>
#include <quaternion.h>
#include <math.h>

#define IsZero(a)	( (a) < 0.000001 && (a) > -0.000001 )

void
Quaternion_Normalize(Quaternion *q)
{
    double  length = q->real_part * q->real_part
		   + VDot(q->vect_part, q->vect_part);
    length = 1.0 / sqrt(length);
    q->real_part *= length;
    VScalarMul(q->vect_part, length, q->vect_part);
}


/*  Matrix
**  Quaternion_To_Matrix(Quaternion q)
**  Converts a quaternion to a rotation matrix.
*/
Matrix
Quaternion_To_Matrix(Quaternion q)
{
    Matrix  result;
    double  xx, yy, zz;
    double  xy, xz, yz;
    double  wx, wy, wz;
    Vector  v1, v2, v3;

    xx = q.vect_part.x * q.vect_part.x;
    yy = q.vect_part.y * q.vect_part.y;
    zz = q.vect_part.z * q.vect_part.z;

    xy = q.vect_part.x * q.vect_part.y;
    xz = q.vect_part.x * q.vect_part.z;
    yz = q.vect_part.y * q.vect_part.z;

    wx = q.real_part * q.vect_part.x;
    wy = q.real_part * q.vect_part.y;
    wz = q.real_part * q.vect_part.z;

    VNew(1 - 2 * yy - 2 * zz, 2 * xy - 2 * wz, 2 * xz + 2 * wy, v1);
    VNew(2 * xy + 2 * wz, 1 - 2 * xx - 2 * zz, 2 * yz - 2 * wx, v2);
    VNew(2 * xz - 2 * wy, 2 * yz + 2 * wx, 1 - 2 * xx - 2 * yy, v3);
    MNew(v1, v2, v3, result);

    return result;
}


extern Quaternion
QQMul_Func(Quaternion *q1, Quaternion *q2)
{
    Quaternion  res;
    Vector      temp_v;

    res.real_part = q1->real_part * q2->real_part -
                    VDot(q1->vect_part, q2->vect_part);
    VCross(q1->vect_part, q2->vect_part, res.vect_part);
    VScalarMul(q1->vect_part, q2->real_part, temp_v);
    VAdd(temp_v, res.vect_part, res.vect_part);
    VScalarMul(q2->vect_part, q1->real_part, temp_v);
    VAdd(temp_v, res.vect_part, res.vect_part);

    return res;
}

extern void
Quaternion_To_Axis_Angle(Quaternion *q, Vector *axis, double *angle)
{
    double  half_angle;
    double  sin_half_angle;

    if ( q->real_part > 1.0 )
	q->real_part = 1.0;
    half_angle = acos(q->real_part);
    sin_half_angle = sin(half_angle);
    *angle = half_angle * 2;
    if ( IsZero(sin_half_angle) )
        VNew(0, 0, 1, *axis);
    else
    {
        sin_half_angle = 1 / sin_half_angle;
        VScalarMul(q->vect_part, sin_half_angle, *axis);
    }
}


Quaternion
Axis_Angle_To_Quaternion(Vector axis, double angle)
{
    Quaternion  quat;
    double  sin_half_angle;
    double  cos_half_angle;

    /* The quaternion requires half angles. */
    sin_half_angle = sin(angle / 2);
    cos_half_angle = cos(angle / 2);

    VScalarMul(axis, sin_half_angle, quat.vect_part);
    quat.real_part = cos_half_angle;

    return quat;
}

Quaternion
Quaternion_Invert(Quaternion *q)
{
    Quaternion	res = *q;

    VScalarMul(res.vect_part, -1.0, res.vect_part);

    return res;
}


Vector
QVMul_Func(Quaternion *q, Vector *v)
{
    Quaternion	qv;
    Quaternion	q1 = *q;
    Quaternion	q2;
    qv.real_part = 0.0;
    qv.vect_part = *v;
    VScalarMul(q1.vect_part, -1.0, q1.vect_part);
    q2 = QQMul_Func(&qv, &q1);
    q1 = QQMul_Func(q, &q2);

    return q1.vect_part;
}



void
Quaternionf_Normalize(Quaternionf *q)
{
    double  length = q->real_part * q->real_part
		   + VDot(q->vect_part, q->vect_part);
    length = 1.0 / sqrt(length);
    q->real_part *= length;
    VScalarMul(q->vect_part, length, q->vect_part);
}


Matrix
Quaternionf_To_Matrix(Quaternionf q)
{
    Matrix  result;
    double  xx, yy, zz;
    double  xy, xz, yz;
    double  wx, wy, wz;
    Vector  v1, v2, v3;

    xx = q.vect_part.x * q.vect_part.x;
    yy = q.vect_part.y * q.vect_part.y;
    zz = q.vect_part.z * q.vect_part.z;

    xy = q.vect_part.x * q.vect_part.y;
    xz = q.vect_part.x * q.vect_part.z;
    yz = q.vect_part.y * q.vect_part.z;

    wx = q.real_part * q.vect_part.x;
    wy = q.real_part * q.vect_part.y;
    wz = q.real_part * q.vect_part.z;

    VNew(1 - 2 * yy - 2 * zz, 2 * xy - 2 * wz, 2 * xz + 2 * wy, v1);
    VNew(2 * xy + 2 * wz, 1 - 2 * xx - 2 * zz, 2 * yz - 2 * wx, v2);
    VNew(2 * xz - 2 * wy, 2 * yz + 2 * wx, 1 - 2 * xx - 2 * yy, v3);
    MNew(v1, v2, v3, result);

    return result;
}


extern void
Quaternionf_To_Axis_Angle(Quaternionf *q, Vectorf *axis, float *angle)
{
    double  half_angle;
    double  sin_half_angle;

    if ( q->real_part > 1.0 )
	q->real_part = 1.0;
    half_angle = acos(q->real_part);
    sin_half_angle = sin(half_angle);
    *angle = half_angle * 2;
    if ( IsZero(sin_half_angle) )
        VNew(0, 0, 1, *axis);
    else
    {
        sin_half_angle = 1 / sin_half_angle;
        VScalarMul(q->vect_part, sin_half_angle, *axis);
    }
}


Quaternionf
Axis_Angle_To_Quaternionf(Vectorf axis, float angle)
{
    Quaternionf  quat;
    double  sin_half_angle;
    double  cos_half_angle;

    /* The quaternion requires half angles. */
    sin_half_angle = sin(angle / 2);
    cos_half_angle = cos(angle / 2);

    VScalarMul(axis, sin_half_angle, quat.vect_part);
    quat.real_part = cos_half_angle;

    return quat;
}

Quaternionf
Quaternionf_Invert(Quaternionf *q)
{
    Quaternionf	res = *q;

    VScalarMul(res.vect_part, -1.0, res.vect_part);

    return res;
}



void
Matrix_To_Axis_Angle(Matrix *r, Vector *axis, double *angle)
{
    /*
    ** Let (x,y,z) be the unit-length axis and let A be an angle of rotation.
    ** The rotation matrix is R = I + sin(A)*P + (1-cos(A))*P^2 where
    ** I is the identity and
    **
    **       +-        -+
    **   P = |  0 +z -y |
    **       | -z  0 +x |
    **       | +y -x  0 |
    **       +-        -+
    **
    ** Some algebra will show that
    **
    **   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
    **
    ** In the event that A = pi, R-R^t = 0 which prevents us from extracting
    ** the axis through P.  Instead note that R = I+2*P^2 when A = pi, so
    ** P^2 = (R-I)/2.  The diagonal entries of P^2 are x^2-1, y^2-1, and
    ** z^2-1. Solve for |x|, |y|, and |z| where the axis is (x,y,z).  There
    ** is a choice of sign on |y| and |z|.  I test |R*axis-axis|^2 for each
    ** of the four sign possibilities and return on the first squared length
    ** which is nearly zero.
    */

    double  trace = r->x.x + r->y.y + r->z.z;
    double  cs = 0.5*(trace-1.0);
    double  length;
    double  epsilon = 1.0e-15;

    if ( -1 < cs )
    {
        if ( cs < 1 )
            *angle = acos(cs);
        else
            *angle = 0.0;
    }
    else
    {
        *angle = M_PI;
    }

    axis->x = r->z.y - r->y.z;
    axis->y = r->x.z - r->z.x;
    axis->z = r->y.x - r->x.y;
    length = VMod(*axis);
    if ( length > epsilon )
    {
	    axis->x /= length;
	    axis->y /= length;
	    axis->z /= length;
    }
    else  /* angle is 0 or pi */
    {
        if ( *angle > 1.0 )  /* any number strictly between 0 and pi works */
        {
            Vector  test;

            // angle must be pi
            axis->x = sqrt(0.5*(1.0+r->x.x));
            axis->y = sqrt(0.5*(1.0+r->y.y));
            axis->z = sqrt(0.5*(1.0+r->z.z));

            // determine signs of axis components
	    MVMul(*r, *axis, test);
	    VSub(test, *axis, test);
            length = VDot(test, test);
            if ( length < epsilon )
                return;

            axis->y = -axis->y;
	    MVMul(*r, *axis, test);
	    VSub(test, *axis, test);
            length = VDot(test, test);
            if ( length < epsilon )
                return;

            axis->z = -axis->z;
	    MVMul(*r, *axis, test);
	    VSub(test, *axis, test);
            length = VDot(test, test);
            if ( length < epsilon )
                return;

            axis->y = -axis->y;
	    MVMul(*r, *axis, test);
	    VSub(test, *axis, test);
            length = VDot(test, test);
            if ( length < epsilon )
                return;
        }
        else
        {
            // Angle is zero, matrix is the identity, no unique axis, so
            // return (1,0,0) for as good a guess as any.
            axis->x = 1.0;
            axis->y = 0.0;
            axis->z = 0.0;
        }
    }
}


void
Matrix_To_Quaternion(Matrix *m, Quaternion *q)
{
    Vector  axis;
    double  angle;

    Matrix_To_Axis_Angle(m, &axis, &angle);
    *q = Axis_Angle_To_Quaternion(axis, angle);
    Quaternion_To_Axis_Angle(q, &axis, &angle);
}

