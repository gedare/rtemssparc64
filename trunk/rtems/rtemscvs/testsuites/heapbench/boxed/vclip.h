/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 *
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/vclip.h,v 1.1 2001/05/01 21:23:23 emery Exp $
 *
 * $Log: vclip.h,v $
 * Revision 1.1  2001/05/01 21:23:23  emery
 * Initial check-in, works under Windows.
 *
 *
 */

#ifndef _VCLIP_H_
#define _VCLIP_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Vector.h>
#include <quaternion.h>


typedef struct _Vertex	*VertexPtr;
typedef struct _Edge	*EdgePtr;
typedef struct _Face	*FacePtr;

typedef enum _FeatureType { vertex_feat, edge_feat, face_feat } FeatureType;

typedef struct _Feature {
    FeatureType	type;
    void	*feature;
    } Feature, *FeaturePtr;


typedef struct _Plane {
    Vector  normal;
    double  offset;
    } Plane, *PlanePtr;


typedef struct _ConeElmt {
    PlanePtr	plane;
    EdgePtr 	edge;
    int		index;

    struct _ConeElmt    *next;
    struct _ConeElmt    *prev;
    } ConeElmt, *ConeList;


typedef struct _Vertex {
    Vector  	posn;
    ConeList	planes;
    } Vertex;


typedef struct _Edge {
    double	length;
    Vector	dir;

    VertexPtr	head;
    Plane	head_plane;

    VertexPtr	tail;
    Plane	tail_plane;

    FacePtr	left;
    Plane	left_plane;

    FacePtr	right;
    Plane	right_plane;
    } Edge;


typedef struct _Face {
    int	    	num_vertices;
    VertexPtr	*vertices;
    Plane   	face_plane;
    ConeList	planes;
    } Face;


typedef struct _Polyhedron {
    int	    num_vertices;
    Vertex  *vertices;
    int	    num_edges;
    Edge    *edges;
    int	    num_faces;
    Face    *faces;
    } Polyhedron, *PolyhedronPtr;


extern PolyhedronPtr	Polyhedron_New(int, Vector*, int, int*, int**);
extern void		Polyhedron_Copy_Values(PolyhedronPtr, PolyhedronPtr);
extern PolyhedronPtr	Polyhedron_Copy(PolyhedronPtr);

extern double	VClip_Distance(PolyhedronPtr, Quaternion*, Vector*, FeaturePtr,
			       Vector*, PolyhedronPtr, Quaternion*, Vector*,
			       FeaturePtr, Vector*, Vector*);


#endif


