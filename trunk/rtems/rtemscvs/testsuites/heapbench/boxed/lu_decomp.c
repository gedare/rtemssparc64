/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

#include <math.h>
#include <stdio.h>
#include <recipes.h>

#define New(t, n) (t*)malloc(sizeof(t) * (n))
#define More(x, t, n) (t*)realloc((char*)x, sizeof(t) * (n))
#define TINY 1.e-20

int
LU_Decomp(float **a, int n, int *indx, int *d, float *vv)
{
    int     i, imax, j, k;
    float   big, dum, sum, temp;

    *d = 1;
    for ( i = 0 ; i < n ; i++ )
    {
        big = 0.0;
        for ( j = 0 ; j < n ; j++ )
            if ( ( temp = fabs(a[i][j]) ) > big )
                big = temp;
        if ( big == 0.0 )
        {
            fprintf(stderr, "Singular matrix in LU_Decomp\n");
            return 0;
        }
        vv[i] = 1.0 / big;
    }
    for ( j = 0 ; j < n ; j++ )
    {
        for ( i = 0 ; i < j ; i++ )
        {
            sum = a[i][j];
            for ( k = 0 ; k < i ; k++ )
                sum -= a[i][k] * a[k][j];
            a[i][j] = sum;
        }
        big = 0.0;
        for ( i = j ; i < n ; i++ )
        {
            sum = a[i][j];
            for ( k = 0 ; k < j ; k++ )
                sum -= a[i][k] * a[k][j];
            a[i][j] = sum;
            if ( ( dum = vv[i] * fabs(sum) ) >= big )
            {
                big = dum;
                imax = i;
            }
        }
        if ( j != imax )
        {
            for ( k =0 ; k < n ; k++ )
            {
                dum = a[imax][k];
                a[imax][k] = a[j][k];
                a[j][k] = dum;
            }
            *d = - (*d);
            vv[imax] = vv[j];
        }
        indx[j] = imax;
        if ( a[j][j] == 0.0 )
            a[j][j] = TINY;
        if ( j != n - 1 )
        {
            dum = 1.0 / a[j][j];
            for ( i = j + 1 ; i < n ; i++ )
                a[i][j] *= dum;
        }
    }

    return 1;
}


void
LU_Back_Subst(float **a, int n, int *indx, float *b)
{
    int     i, ii = -1, ip, j;
    double  sum;

    for ( i = 0 ; i < n ; i++ )
    {
        ip = indx[i];
        sum = b[ip];
        b[ip] = b[i];
        if ( ii != -1 )
            for ( j = ii ; j <= i - 1 ; j++ )
                sum -= a[i][j] * b[j];
        else if ( sum )
            ii = i;
        b[i] = sum;
    }
    for ( i = n - 1 ; i >= 0 ; i-- )
    {
        sum = b[i];
        for ( j = i + 1 ; j < n ; j++ )
            sum -= a[i][j] * b[j];
        b[i] = sum / a[i][i];
    }
}


float
LU_Invert(float **in, int n, float *wf, int *wi, float **out)
{
    int	    i, j;
    int	    flips;
    float   res;

    /* This is overkill for small dimensions, but will work to large
    ** dimensions. The value returned is the determinant of the original
    ** matrix.
    */
    LU_Decomp(in, n, wi, &flips, wf);
    res = flips;
    for ( i = 0 ; i < n ; i++ )
	res *= in[i][i];

    for ( i = 0 ; i < n ; i++ )
    {
	for ( j = 0 ; j < n ; j++ )
	    wf[j] = 0;
	wf[i] = 1.0;
	LU_Back_Subst(in, n, wi, wf);
	for ( j = 0 ; j < n ; j++ )
	    out[j][i] = wf[j];
    }

    return res;
}


int
LU_Decompd(double **a, int n, int *indx, int *d, double *vv)
{
    int     i, imax, j, k;
    double  big, dum, sum, temp;

    *d = 1;
    for ( i = 0 ; i < n ; i++ )
    {
        big = 0.0;
        for ( j = 0 ; j < n ; j++ )
            if ( ( temp = fabs(a[i][j]) ) > big )
                big = temp;
        if ( big == 0.0 )
        {
            fprintf(stderr, "Singular matrix in LU_Decomp\n");
            return 0;
        }
        vv[i] = 1.0 / big;
    }
    for ( j = 0 ; j < n ; j++ )
    {
        for ( i = 0 ; i < j ; i++ )
        {
            sum = a[i][j];
            for ( k = 0 ; k < i ; k++ )
                sum -= a[i][k] * a[k][j];
            a[i][j] = sum;
        }
        big = 0.0;
        for ( i = j ; i < n ; i++ )
        {
            sum = a[i][j];
            for ( k = 0 ; k < j ; k++ )
                sum -= a[i][k] * a[k][j];
            a[i][j] = sum;
            if ( ( dum = vv[i] * fabs(sum) ) >= big )
            {
                big = dum;
                imax = i;
            }
        }
        if ( j != imax )
        {
            for ( k =0 ; k < n ; k++ )
            {
                dum = a[imax][k];
                a[imax][k] = a[j][k];
                a[j][k] = dum;
            }
            *d = - (*d);
            vv[imax] = vv[j];
        }
        indx[j] = imax;
        if ( a[j][j] == 0.0 )
            a[j][j] = TINY;
        if ( j != n - 1 )
        {
            dum = 1.0 / a[j][j];
            for ( i = j + 1 ; i < n ; i++ )
                a[i][j] *= dum;
        }
    }

    return 1;
}


void
LU_Back_Substd(double **a, int n, int *indx, double *b)
{
    int     i, ii = -1, ip, j;
    double  sum;

    for ( i = 0 ; i < n ; i++ )
    {
        ip = indx[i];
        sum = b[ip];
        b[ip] = b[i];
        if ( ii != -1 )
            for ( j = ii ; j <= i - 1 ; j++ )
                sum -= a[i][j] * b[j];
        else if ( sum )
            ii = i;
        b[i] = sum;
    }
    for ( i = n - 1 ; i >= 0 ; i-- )
    {
        sum = b[i];
        for ( j = i + 1 ; j < n ; j++ )
            sum -= a[i][j] * b[j];
        b[i] = sum / a[i][i];
    }
}



double
LU_Invertd(double **in, int n, double *wf, int *wi, double **out)
{
    int	    i, j;
    int	    flips;
    double   res;

    /* This is overkill for small dimensions, but will work to large
    ** dimensions. The value returned is the determinant of the original
    ** matrix.
    */
    LU_Decompd(in, n, wi, &flips, wf);
    res = flips;
    for ( i = 0 ; i < n ; i++ )
	res *= in[i][i];

    for ( i = 0 ; i < n ; i++ )
    {
	for ( j = 0 ; j < n ; j++ )
	    wf[j] = 0;
	wf[i] = 1.0;
	LU_Back_Substd(in, n, wi, wf);
	for ( j = 0 ; j < n ; j++ )
	    out[j][i] = wf[j];
    }

    return res;
}
