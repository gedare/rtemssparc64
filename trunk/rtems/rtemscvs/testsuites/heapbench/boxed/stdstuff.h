/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

#ifndef _STDSTUFF_H_
#define _STDSTUFF_H_

/*
 * (c) Stephen Chenney 1999
 */

#include <stdio.h>
#include <stdlib.h>
/* #include <alloca.h> */

typedef unsigned char bool;
#define true 1
#define false 0


#define Max(a, b) ( (a) >= (b) ? (a) : (b) )
#define Min(a, b) ( (a) < (b) ? (a) : (b) )


#define New(t, n) 	( (t*)malloc(sizeof(t) * (n)) )
#define More(p, t, n) 	( (t*)realloc((void*)p, sizeof(t) * (n)) )
#define NewZero(t, n)	( (t*)calloc((n), sizeof(t)) )
#define NewStack(t, n)  ( (t*)alloca(sizeof(t) * (n)) )


#endif
