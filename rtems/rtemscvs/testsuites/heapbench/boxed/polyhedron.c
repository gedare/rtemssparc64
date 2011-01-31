/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 *
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/polyhedron.c,v 1.1 2001/05/01 21:23:15 emery Exp $
 *
 * $Log: polyhedron.c,v $
 * Revision 1.1  2001/05/01 21:23:15  emery
 * Initial check-in, works under Windows.
 *
 *
 */

#include <vclip.h>


#define New(t, n)   	(t*)malloc(sizeof(t) * (n))
#define New_Stack(t, n)	(t*)alloca(sizeof(t) * (n))


#define Add_Vert_Cone_Plane(h, e, p) { \
	ConeList    _new_c = New(ConeElmt, 1); \
	_new_c->edge = e; \
	_new_c->plane = p; \
	_new_c->prev = NULL; \
	_new_c->next = (h); \
	if ( h ) \
	    (h)->prev = _new_c; \
	(h) = _new_c; \
    }

#define Add_Face_Cone_Plane(h, e, p) { \
	ConeList    _new_c = New(ConeElmt, 1); \
	_new_c->edge = e; \
	_new_c->plane = p; \
	if ( h ) \
	{ \
	    (h)->prev->next = _new_c; \
	    _new_c->prev = (h)->prev; \
	    _new_c->next = (h); \
	    (h)->prev = _new_c; \
	} \
	else \
	{ \
	    (h) = _new_c; \
	    _new_c->next = _new_c->prev = _new_c; \
	} \
    }


static void
Add_Edge(PolyhedronPtr poly, FacePtr face, int p1, int p2)
{
    EdgePtr edge = NULL;
    double  d;
    int	    i;

    /* Look for it. */
    for ( i = 0 ; i < poly->num_edges ; i++ )
	if ( poly->edges[i].head == face->vertices[p1]
	  && poly->edges[i].tail == face->vertices[p2] )
	{
	    edge = poly->edges + i;
	    break;
	}

    if ( edge )
    {
	edge->right = face;
	VCross(edge->dir, face->face_plane.normal, edge->right_plane.normal);
	VUnit(edge->right_plane.normal, d, edge->right_plane.normal);
	edge->right_plane.offset =
	    -VDot(edge->right_plane.normal, edge->tail->posn);

	/* Add the cone plane to the face. */
	Add_Face_Cone_Plane(face->planes, edge, &(edge->right_plane));
    }
    else
    {
	/* New edge. */
	edge = poly->edges + poly->num_edges;
	poly->num_edges++;

	edge->tail = face->vertices[p1];
	edge->head = face->vertices[p2];

	VSub(edge->head->posn, edge->tail->posn, edge->dir);
	edge->length = VMod(edge->dir);
	edge->dir.x /= edge->length;
	edge->dir.y /= edge->length;
	edge->dir.z /= edge->length;

	edge->head_plane.normal = edge->dir;
	edge->head_plane.offset =
	    -VDot(edge->head_plane.normal, edge->head->posn);

	edge->tail_plane.normal.x = -edge->dir.x;
	edge->tail_plane.normal.y = -edge->dir.y;
	edge->tail_plane.normal.z = -edge->dir.z;
	edge->tail_plane.offset =
	    -VDot(edge->tail_plane.normal, edge->tail->posn);

	edge->left = face;
	VCross(face->face_plane.normal, edge->dir, edge->left_plane.normal);
	VUnit(edge->left_plane.normal, d, edge->left_plane.normal);
	edge->left_plane.offset =
	    -VDot(edge->left_plane.normal, edge->tail->posn);

	/* Add the cone planes to the vertices. */
	Add_Vert_Cone_Plane(edge->head->planes, edge, &(edge->head_plane));
	Add_Vert_Cone_Plane(edge->tail->planes, edge, &(edge->tail_plane));

	/* Add the cone plane to the face. */
	Add_Face_Cone_Plane(face->planes, edge, &(edge->left_plane));
    }
}


static void
Build_Voronoi_Structure(PolyhedronPtr poly)
{
    int	i, j;

    poly->num_edges = 0;
    poly->edges = New(Edge, poly->num_vertices + poly->num_faces - 2);

    for ( i = 0 ; i < poly->num_faces ; i++ )
    {
	FacePtr	face = poly->faces + i;
	Vector	e1, e2;
	double	temp_d;

	/* Get the face plane. */
	VSub(face->vertices[2]->posn, face->vertices[1]->posn, e1);
	VSub(face->vertices[0]->posn, face->vertices[1]->posn, e2);
	VCross(e1, e2, face->face_plane.normal);
	VUnit(face->face_plane.normal, temp_d, face->face_plane.normal);
	face->face_plane.offset = -VDot(face->face_plane.normal,
				        face->vertices[1]->posn);

	/* Add all the edges. */
	Add_Edge(poly, face, face->num_vertices - 1, 0);
	for ( j = 0 ; j < face->num_vertices - 1 ; j++ )
	    Add_Edge(poly, face, j, j + 1);
    }
}


PolyhedronPtr
Polyhedron_New(int num_verts, Vector *vertices,
	       int num_faces, int *face_num_verts, int **faces)
{
    PolyhedronPtr   poly = New(Polyhedron, 1);
    int		    i, j;

    poly->num_vertices = num_verts;
    poly->vertices = New(Vertex, num_verts);
    for ( i = 0 ; i < num_verts ; i++ )
    {
	poly->vertices[i].posn = vertices[i];
	poly->vertices[i].planes = NULL;
    }

    poly->num_faces = num_faces;
    poly->faces = New(Face, num_faces);
    for ( i = 0 ; i < num_faces ; i++ )
    {
	poly->faces[i].num_vertices = face_num_verts[i];
	poly->faces[i].vertices = New(VertexPtr, poly->faces[i].num_vertices);
	for ( j = 0 ; j < poly->faces[i].num_vertices ; j++ )
	    poly->faces[i].vertices[j] = poly->vertices + faces[i][j];
	poly->faces[i].planes = NULL;
    }

    Build_Voronoi_Structure(poly);

    return poly;
}


void
Polyhedron_Copy_Values(PolyhedronPtr src, PolyhedronPtr dest)
{
    /* The assumption here is that every vertex, edge and face exactly
    ** corresponds in the src and dest. so it is only necessary to copy
    ** geometric, as oppsoed to topological, information.
    */

    int	i;

    for ( i = 0 ; i < src->num_vertices ; i++ )
    {
	dest->vertices[i].posn = src->vertices[i].posn;
	/* Planes updated by edge. */
    }

    for ( i = 0 ; i < src->num_edges ; i++ )
    {
	dest->edges[i].length = src->edges[i].length;
	dest->edges[i].dir = src->edges[i].dir;

	dest->edges[i].head_plane.normal = src->edges[i].head_plane.normal;
	dest->edges[i].head_plane.offset = src->edges[i].head_plane.offset;
	dest->edges[i].tail_plane.normal = src->edges[i].tail_plane.normal;
	dest->edges[i].tail_plane.offset = src->edges[i].tail_plane.offset;
	dest->edges[i].left_plane.normal = src->edges[i].left_plane.normal;
	dest->edges[i].left_plane.offset = src->edges[i].left_plane.offset;
	dest->edges[i].right_plane.normal = src->edges[i].right_plane.normal;
	dest->edges[i].right_plane.offset = src->edges[i].right_plane.offset;
    }

    for ( i = 0 ; i < src->num_faces ; i++ )
    {
	dest->faces[i].face_plane.normal = src->faces[i].face_plane.normal;
	dest->faces[i].face_plane.offset = src->faces[i].face_plane.offset;
	/* Planes updated by edge. */
    }
}



PolyhedronPtr
Polyhedron_Copy(PolyhedronPtr src)
{
    PolyhedronPtr   poly = New(Polyhedron, 1);
    int		    i, j;

    poly->num_vertices = src->num_vertices;
    poly->vertices = New(Vertex, src->num_vertices);
    for ( i = 0 ; i < src->num_vertices ; i++ )
    {
	poly->vertices[i].posn = src->vertices[i].posn;
	poly->vertices[i].planes = NULL;
    }

    poly->num_faces = src->num_faces;
    poly->faces = New(Face,src-> num_faces);
    for ( i = 0 ; i < src->num_faces ; i++ )
    {
	poly->faces[i].num_vertices = src->faces[i].num_vertices;
	poly->faces[i].vertices = New(VertexPtr, poly->faces[i].num_vertices);
	for ( j = 0 ; j < poly->faces[i].num_vertices ; j++ )
	    poly->faces[i].vertices[j] =
		poly->vertices + ( src->faces[i].vertices[j] - src->vertices );
	poly->faces[i].planes = NULL;
    }

    Build_Voronoi_Structure(poly);

    return poly;
}


