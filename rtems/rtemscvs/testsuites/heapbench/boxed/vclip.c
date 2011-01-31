/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 *
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/vclip.c,v 1.1 2001/05/01 21:23:22 emery Exp $
 *
 * $Log: vclip.c,v $
 * Revision 1.1  2001/05/01 21:23:22  emery
 * Initial check-in, works under Windows.
 *
 *
 */

#include <boxed.h>
#include <vclip.h>


#define New_Stack(t, n) (t*)alloca(sizeof(t) * (n))


#define PENETRATING -1
#define CONTINUE 0
#define DISJOINT 1


typedef struct _TransformedGeom {
    void	*feature;
    Vector	coords;
    Vector	tail;
    Vector	head;
    Vector	seg;
    } TransformedGeom, *TransformedGeomPtr;


typedef enum _Code { inside_code, outside_code, min_code, max_code } Code;


#define Trans_Vertex(q, t, v, xg) \
    if ( (xg)->feature != (void*)(v) ) \
    { \
	QVMul(*(q), (v)->posn, (xg)->coords); \
	VAdd(*(t), (xg)->coords, (xg)->coords); \
	(xg)->feature = (void*)(v); \
    }


#define Trans_Edge(q, t, e, xg) \
    if ( (xg)->feature != (void*)(e) ) \
    { \
	QVMul(*(q), (e)->tail->posn, (xg)->tail); \
	VAdd(*(t), (xg)->tail, (xg)->tail); \
	QVMul(*(q), (e)->head->posn, (xg)->head); \
	VAdd(*(t), (xg)->head, (xg)->head); \
	VSub((xg)->head, (xg)->tail, (xg)->seg); \
	(xg)->feature = (void*)(e); \
    }


#define Plane_Dist(p, x)    ( VDot((p)->normal, x) + (p)->offset )


static int
Vert_Vert_Test(FeaturePtr f1, FeaturePtr f2, TransformedGeomPtr xg1,
	       TransformedGeomPtr xg2, Quaternion *q_1_to_2, Vector *t_1_to_2,
	       Quaternion *q_2_to_1, Vector *t_2_to_1)
{
    VertexPtr	v1 = (VertexPtr)f1->feature;
    VertexPtr	v2 = (VertexPtr)f2->feature;
    ConeList	l;
    Vector	temp_v;

    Trans_Vertex(q_2_to_1, t_2_to_1, v2, xg2);
    for ( l = v1->planes ; l ; l = l->next )
    {
	if ( Plane_Dist(l->plane, xg2->coords) < 0.0 )
	{
	    f1->type = edge_feat;
	    f1->feature = (void*)l->edge;
	    return CONTINUE;
	}
    }

    Trans_Vertex(q_1_to_2, t_1_to_2, v1, xg1);
    for ( l = v2->planes ; l ; l = l->next )
    {
	if ( Plane_Dist(l->plane, xg1->coords) < 0.0 )
	{
	    f2->type = edge_feat;
	    f2->feature = (void*)l->edge;
	    return CONTINUE;
	}
    }

    return ( VEqual(xg1->coords, v2->posn, temp_v) ? PENETRATING : DISJOINT );
}


int
Vert_Face_Test(FeaturePtr f1, FeaturePtr f2, TransformedGeomPtr xg1,
	       Quaternion *q_1_to_2, Vector *t_1_to_2, int num_faces,
	       FacePtr faces)
{
    VertexPtr	vert = (VertexPtr)f1->feature;
    FacePtr	face = (FacePtr)f2->feature;
    int    	update;
    double	d, d2, dmin;
    ConeList	l;
    int		i;

    Trans_Vertex(q_1_to_2, t_1_to_2, vert, xg1);
    update = 0;
    dmin = 0.0;
    for ( i = 0, l = face->planes ; i < face->num_vertices ; i++, l = l->next )
    {
	if ( ( d = Plane_Dist(l->plane, xg1->coords) ) < dmin )
	{
	    f2->type = edge_feat;
	    f2->feature = (void*)l->edge;
	    dmin = d;
	    update = 1;
	}
    }
    if ( update )
	return CONTINUE;

    if ( ( d = Plane_Dist(&(face->face_plane), xg1->coords) ) == 0.0 )
	return PENETRATING;
    for ( l = vert->planes ; l ; l = l->next )
    {
	EdgePtr	e = l->edge;
	Vector	x_other;

	if ( e->tail == vert )
	    QVMul(*q_1_to_2, e->head->posn, x_other)
	else
	    QVMul(*q_1_to_2, e->tail->posn, x_other)
	VAdd(x_other, *t_1_to_2, x_other);

	d2 = Plane_Dist(&(face->face_plane), x_other);
	if ( ( d < 0.0 && d2 > d ) || ( d > 0.0 && d2 < d ) )
	{
	    if ( e->tail == vert )
	    {
		xg1->tail = xg1->coords;
		xg1->head = x_other;
	    }
	    else
	    {
		xg1->head = xg1->coords;
		xg1->tail = x_other;
	    }
	    VSub(xg1->head, xg1->tail, xg1->seg);
	    xg1->feature = (void*)e;
	    f1->type = edge_feat;
	    f1->feature = e;

	    return CONTINUE;
	}
    }

    if ( d > 0.0 )
	return DISJOINT;

    /* V is in local min on back side of f's cone. */
    for ( i = 0 ; i < num_faces ; i++ )
    {
	if ( ( d2 = Plane_Dist(&(faces[i].face_plane), xg1->coords) ) > d )
	{
	    d = d2;
	    f2->feature = faces + i;
	}
    }
    if ( d > 0.0 )
	return CONTINUE;

    return PENETRATING;
}


static int
Vert_Edge_Test(FeaturePtr f1, FeaturePtr f2, TransformedGeomPtr xg1,
	       TransformedGeomPtr xg2, Quaternion *q_1_to_2, Vector *t_1_to_2,
	       Quaternion *q_2_to_1, Vector *t_2_to_1)
{
    VertexPtr	vert = (VertexPtr)f1->feature;
    EdgePtr	edge = (EdgePtr)f2->feature;
    EdgePtr 	minNbr, maxNbr;
    ConeList	l;
    double	min, max, lambda, dt, dh;
    Vector	offset;

    /* Test vertex against edge's cone. */
    Trans_Vertex(q_1_to_2, t_1_to_2, vert, xg1);
    if ( Plane_Dist(&(edge->head_plane), xg1->coords) > 0.0 )
    {
	f2->type = vertex_feat;
	f2->feature = (void*)edge->head;
	return CONTINUE;
    }
    if ( Plane_Dist(&(edge->tail_plane), xg1->coords) > 0.0 )
    {
	f2->type = vertex_feat;
	f2->feature = (void*)edge->tail;
	return CONTINUE;
    }
    if ( Plane_Dist(&(edge->left_plane), xg1->coords) > 0.0 )
    {
	f2->type = face_feat;
	f2->feature = (void*)edge->left;
	return CONTINUE;
    }
    if ( Plane_Dist(&(edge->right_plane), xg1->coords) > 0.0 )
    {
	f2->type = face_feat;
	f2->feature = (void*)edge->right;
	return CONTINUE;
    }

    /* Clip edge against vertex cone. */
    Trans_Edge(q_2_to_1, t_2_to_1, edge, xg2);
    min = 0.0;
    max = 1.0;
    minNbr = maxNbr = NULL;
    for ( l = vert->planes ; l ; l = l->next )
    {
	dt = Plane_Dist(l->plane, xg2->tail);
	dh = Plane_Dist(l->plane, xg2->head);

	if ( dt >= 0.0 )
	{
	    if ( dh >= 0.0 )
		continue;
	    if ( ( lambda = dt / ( dt - dh ) ) < max )
	    {
		max = lambda;
		maxNbr = l->edge;
		if ( max < min )
		    break;
	    }
	}
	else
	{
	    if ( dh < 0.0 )
	    {
		minNbr = maxNbr = l->edge;
		break;
	    }
	    if ( ( lambda = dt / ( dt - dh ) ) > min )
	    {
		min = lambda;
		minNbr = l->edge;
		if ( min > max )
		    break;
	    }
	}
    }
    if ( l && ( minNbr == maxNbr ) )
    {
	f1->type = edge_feat;
	f1->feature = minNbr;
	return CONTINUE;
    }

    if ( minNbr || maxNbr )
    {
	if ( minNbr )
	{
	    VScalarMul(xg2->seg, min, offset);
	    VAdd(offset, xg2->tail, offset);
	    VSub(offset, vert->posn, offset);
	    if ( VZero(offset) )
		return PENETRATING;
	    if ( VDot(offset, xg2->seg) > 0.0 )
	    {
		f1->type = edge_feat;
		f1->feature = (void*)minNbr;
		return CONTINUE;
	    }
	}
	if ( maxNbr )
	{
	    VScalarMul(xg2->seg, max, offset);
	    VAdd(offset, xg2->tail, offset);
	    VSub(offset, vert->posn, offset);
	    if ( VZero(offset) )
		return PENETRATING;
	    if ( VDot(offset, xg2->seg) < 0.0 )
	    {
		f1->type = edge_feat;
		f1->feature = (void*)maxNbr;
		return CONTINUE;
	    }
	}
    }

    return DISJOINT;
}


static int
Edge_Edge_Subtest(FeaturePtr f, TransformedGeomPtr xg)
{
    EdgePtr 	e = (EdgePtr)f->feature;
    Feature  	minNbr, maxNbr, vminNbr, vmaxNbr;
    FacePtr 	nbr;
    PlanePtr	plane;
    double  	dt, dh, lambda, min, max, vmin, vmax, dmin, dmax;
    Vector	point;
    int		i;

    min = 0.0;
    max = 1.0;
    minNbr.feature = maxNbr.feature = NULL;

    /* Clip against tail vertex plane. */
    dt = -Plane_Dist(&(e->tail_plane), xg->tail);
    dh = -Plane_Dist(&(e->tail_plane), xg->head);
    if ( dt < 0.0 )
    {
	if ( dh < 0.0 )
	{
	    f->type = vertex_feat;
	    f->feature = (void*)e->tail;
	    return CONTINUE;
	}
	min = dt / ( dt - dh );
	minNbr.type = vertex_feat;
	minNbr.feature = (void*)e->tail;
    }
    else if ( dh < 0.0 )
    {
	max = dt / ( dt - dh );
	maxNbr.type = vertex_feat;
	maxNbr.feature = (void*)e->tail;
    }

    /* Clip against head vertex. */
    dt = -Plane_Dist(&(e->head_plane), xg->tail);
    dh = -Plane_Dist(&(e->head_plane), xg->head);
    if ( dt < 0.0 )
    {
	if ( dh < 0.0 )
	{
	    f->type = vertex_feat;
	    f->feature = (void*)e->head;
	    return CONTINUE;
	}
	min = dt / ( dt - dh );
	minNbr.type = vertex_feat;
	minNbr.feature = (void*)e->head;
    }
    else if ( dh < 0.0 )
    {
	max = dt / ( dt - dh );
	maxNbr.type = vertex_feat;
	maxNbr.feature = (void*)e->head;
    }

    vminNbr = minNbr;
    if ( vminNbr.feature )
	vmin = min;
    vmaxNbr = maxNbr;
    if ( vmaxNbr.feature )
	vmax = max;

    for ( i = 0 ; i < 2 ; i++ )
    {
	if ( i )
	{
	    nbr = e->right;
	    plane = &(e->right_plane);
	}
	else
	{
	    nbr = e->left;
	    plane = &(e->left_plane);
	}
	dt = -Plane_Dist(plane, xg->tail);
	dh = -Plane_Dist(plane, xg->head);
	if ( dt < 0.0 )
	{
	    if ( dh < 0.0 )
	    {
		/* Completely clipped by a face plane. */
		if ( vminNbr.feature )
		{
		    VScalarMul(xg->seg, vmin, point);
		    VAdd(point, xg->tail, point);
		    VSub(point, ((VertexPtr)vminNbr.feature)->posn, point);
		    if ( VZero(point) )
			return PENETRATING;
		    if ( VDot(point, xg->seg) > 0.0 )
		    {
			*f = vminNbr;
			return CONTINUE;
		    }
		}
		if ( vmaxNbr.feature )
		{
		    VScalarMul(xg->seg, vmax, point);
		    VAdd(point, xg->tail, point);
		    VSub(point, ((VertexPtr)vmaxNbr.feature)->posn, point);
		    if ( VZero(point) )
			return PENETRATING;
		    if ( VDot(point, xg->seg) < 0.0 )
		    {
			*f = vmaxNbr;
			return CONTINUE;
		    }
		}
		f->type = face_feat;
		f->feature = (void*)nbr;
		return CONTINUE;
	    }
	    else if ( ( lambda = dt / ( dt - dh ) ) > min )
	    {
		min = lambda;
		minNbr.type = face_feat;
		minNbr.feature = (void*)nbr;
		if ( min > max )
		    break;
	    }
	}
	else if ( dh < 0.0 )
	{
	    if ( ( lambda = dt / ( dt - dh ) ) < max )
	    {
		max = lambda;
		maxNbr.type = face_feat;
		maxNbr.feature = (void*)nbr;
		if ( max < min )
		    break;
	    }
	}
    }

    if ( i < 2 ) /* Broke out above. */
    {
	/* Edge lies outside voronoi region. */
	if ( minNbr.type == vertex_feat )
	{
	    VScalarMul(xg->seg, min, point);
	    VAdd(point, xg->tail, point);
	    VSub(point, ((VertexPtr)minNbr.feature)->posn, point);
	    if ( VZero(point) )
		return PENETRATING;
	    *f = ( VDot(point, xg->seg) >= 0 ? minNbr : maxNbr );
	    return CONTINUE;
	}
	if ( maxNbr.type == vertex_feat )
	{
	    VScalarMul(xg->seg, max, point);
	    VAdd(point, xg->tail, point);
	    VSub(point, ((VertexPtr)maxNbr.feature)->posn, point);
	    if ( VZero(point) )
		return PENETRATING;
	    *f = ( VDot(point, xg->seg) <= 0 ? maxNbr : minNbr );
	    return CONTINUE;
	}
	/* Complete clipping by combination of both faces. */
	dt = Plane_Dist(&(((FacePtr)minNbr.feature)->face_plane), xg->tail);
	dh = Plane_Dist(&(((FacePtr)minNbr.feature)->face_plane), xg->head);
	dmin = dt + min * ( dh - dt );
	if ( dmin == 0.0 )
	    return PENETRATING;
	*f = ( dmin > 0.0 ? ( dt < dh ? minNbr : maxNbr )
			  : ( dt > dh ? minNbr : maxNbr ) );
	return CONTINUE;
    }

    /* Edge intersects V-region. Analyze derivs. */
    if ( minNbr.feature )
    {
	if ( minNbr.type == face_feat )
	{
	    dt = Plane_Dist(&(((FacePtr)minNbr.feature)->face_plane),xg->tail);
	    dh = Plane_Dist(&(((FacePtr)minNbr.feature)->face_plane),xg->head);
	    dmin = dt + min * ( dh - dt );
	    dmax = ( maxNbr.feature ? dt + max * ( dh - dt ) : dh );
	    if ( dmin == 0.0 )
		return PENETRATING;
	    if ( ( dmin > 0.0 && dmin < dmax )
	      || ( dmin < 0.0 && dmin > dmax ) )
	    {
		*f = minNbr;
		return CONTINUE;
	    }
	}
	else
	{
	    VScalarMul(xg->seg, min, point);
	    VAdd(point, xg->tail, point);
	    VSub(point, ((VertexPtr)minNbr.feature)->posn, point);
	    if ( VZero(point) )
		return PENETRATING;
	    if ( VDot(point, xg->seg) > 0.0 )
	    {
		*f = minNbr;
		return CONTINUE;
	    }
	}
    }

    if ( maxNbr.feature )
    {
	if ( maxNbr.type == face_feat )
	{
	    dt = Plane_Dist(&(((FacePtr)maxNbr.feature)->face_plane),xg->tail);
	    dh = Plane_Dist(&(((FacePtr)maxNbr.feature)->face_plane),xg->head);
	    dmin = ( minNbr.feature ? dt + min * ( dh - dt ) : dt );
	    dmax = dt + max * ( dh - dt );
	    if ( dmax == 0.0 )
		return PENETRATING;
	    if ( ( dmax > 0.0 && dmin > dmax )
	      || ( dmax < 0.0 && dmin < dmax ) )
	    {
		*f = maxNbr;
		return CONTINUE;
	    }
	}
	else
	{
	    VScalarMul(xg->seg, max, point);
	    VAdd(point, xg->tail, point);
	    VSub(point, ((VertexPtr)maxNbr.feature)->posn, point);
	    if ( VZero(point) )
		return PENETRATING;
	    if ( VDot(point, xg->seg) < 0.0 )
	    {
		*f = maxNbr;
		return CONTINUE;
	    }
	}
    }

    return DISJOINT;
}


static int
Edge_Edge_Test(FeaturePtr f1, FeaturePtr f2, TransformedGeomPtr xg1,
	       TransformedGeomPtr xg2, Quaternion *q_1_to_2, Vector *t_1_to_2,
	       Quaternion *q_2_to_1, Vector *t_2_to_1)
{
    EdgePtr e1 = (EdgePtr)f1->feature;
    EdgePtr e2 = (EdgePtr)f2->feature;
    int	    res;

    /* Clip e1 against e2. */
    Trans_Edge(q_1_to_2, t_1_to_2, e1, xg1);
    if ( ( res = Edge_Edge_Subtest(f2, xg1) ) != DISJOINT )
	return res;

    /* Clip e2 against e1. */
    Trans_Edge(q_2_to_1, t_2_to_1, e2, xg2);
    if ( ( res = Edge_Edge_Subtest(f1, xg2) ) != DISJOINT )
	return res;

    /* Disjoint. */
    return DISJOINT;
}


static int
Edge_Face_Test(FeaturePtr f1, FeaturePtr f2, TransformedGeomPtr xg,
	       Quaternion *q_1_to_2, Vector *t_1_to_2)
{
    EdgePtr	edge = (EdgePtr)f1->feature;
    FacePtr	face = (FacePtr)f2->feature;
    ConeList	l;
    ConeList	cn, prev, next, maxCn, minCn, chopCn;
    EdgePtr	s;
    VertexPtr	minv, maxv;
    double	lambda, min, max, dt, dh, dmin, dmax;
    Vector	point;
    Code	*code = New_Stack(Code, face->num_vertices);
    double	*lam = New_Stack(double, face->num_vertices);
    int		i, intersect;

    Trans_Edge(q_1_to_2, t_1_to_2, edge, xg);

    min = 0.0;
    max = 1.0;
    minCn = maxCn = chopCn = NULL;
    for ( i = 0, l = face->planes ; i < face->num_vertices ; i++, l = l->next )
    {
	l->index = i;
	dt = Plane_Dist(l->plane, xg->tail);
	dh = Plane_Dist(l->plane, xg->head);
	if ( dt >= 0.0 )
	{
	    if ( dh >= 0.0 )
		code[i] = inside_code;
	    else
	    {
		code[i] = max_code;
		if ( ( lam[i] = dt / ( dt - dh ) ) < max )
		{
		    max = lam[i];
		    maxCn = l;
		}
	    }
	}
	else
	{
	    if ( dh >= 0.0 )
	    {
		code[i] = min_code;
		if ( ( lam[i] = dt / ( dt - dh ) ) > min )
		{
		    min = lam[i];
		    minCn = l;
		}
	    }
	    else
	    {
		code[i] = outside_code;
		chopCn = l;
	    }
	}
    }

    if ( chopCn || min > max )
    {
	if ( chopCn )
	    cn = chopCn;
	else
	    cn = ( min + max > 1.0 ? minCn : maxCn );

	prev = NULL;
	next = cn;
	intersect = 0;
	while ( next != prev )
	{
	    prev = cn;
	    cn = next;
	    s = cn->edge;
	    minv = maxv = NULL;

	    /* test edge plane. */
	    i = cn->index;
	    if ( code[i] == inside_code )
		break;
	    else if ( code[i] == outside_code )
	    {
		min = 0.0;
		max = 1.0;
	    }
	    else if ( code[i] == min_code )
	    {
		min = 0.0;
		max = lam[i];
	    }
	    else if ( code[i] == max_code )
	    {
		min = lam[i];
		max = 1.0;
	    }

	    /* Test tail plane. */
	    dt = -Plane_Dist(&(s->tail_plane), xg->tail);
	    dh = -Plane_Dist(&(s->tail_plane), xg->head);
	    if ( dt >= 0.0 )
	    {
		if ( dh < 0.0 )
		{
		    if ( ( lambda = dt / ( dt - dh ) ) < max )
		    {
			max = lambda;
			maxv = s->tail;
			if ( min > max )
			{
			    if ( intersect )
				break;
			    next = ( s->left == face ? cn->prev : cn->next );
			    continue;
			}
		    }
		}
	    }
	    else
	    {
		if ( dh < 0.0 )
		{
		    next = ( s->left == face ? cn->prev : cn->next );
		    continue;
		}
		if ( ( lambda = dt / ( dt - dh ) ) > min )
		{
		    min = lambda;
		    minv = s->tail;
		    if ( min > max )
		    {
			if ( intersect )
			    break;
			next = ( s->left == face ? cn->prev : cn->next );
			continue;
		    }
		}
	    }

	    /* Test head plane. */
	    dt = - Plane_Dist(&(s->head_plane), xg->tail);
	    dh = - Plane_Dist(&(s->head_plane), xg->head);
	    if ( dt >= 0.0 )
	    {
		if ( dh < 0.0 )
		{
		    if ( ( lambda = dt / ( dt - dh ) ) < max )
		    {
			max = lambda;
			maxv = s->head;
			if ( min > max )
			{
			    if ( intersect )
				break;
			    next = ( s->left == face ? cn->next : cn->prev );
			    continue;
			}
		    }
		}
	    }
	    else
	    {
		if ( dh < 0.0 )
		{
		    next = ( s->left == face ? cn->next : cn->prev );
		    continue;
		}
		if ( ( lambda = dt / ( dt - dh ) ) > min )
		{
		    min = lambda;
		    minv = s->head;
		    if ( min > max )
		    {
			if ( intersect )
			    break;
			next = ( s->left == face ? cn->next : cn->prev );
			continue;
		    }
		}
	    }

	    intersect = 1;

	    if ( minv )
	    {
		VScalarMul(xg->seg, min, point);
		VAdd(point, xg->tail, point);
		VSub(point, minv->posn, point);
		if ( VDot(point, xg->seg) > 0.0 )
		{
		    next = ( s->left == face
			   ? ( s->tail == minv ? cn->prev : cn->next )
			   : ( s->tail == minv ? cn->next : cn->prev ) );
		    continue;
		}
	    }
	    if ( maxv )
	    {
		VScalarMul(xg->seg, max, point);
		VAdd(point, xg->tail, point);
		VSub(point, maxv->posn, point);
		if ( VDot(point, xg->seg) < 0.0 )
		{
		    next = ( s->left == face
			   ? ( s->head == maxv ? cn->next : cn->prev )
			   : ( s->head == maxv ? cn->prev : cn->next ) );
		    continue;
		}
	    }

	    f2->type = edge_feat;
	    f2->feature = s;
	    return CONTINUE;
	}

	f2->type = vertex_feat;
	f2->feature = cn->next == prev ? ( s->left == face ? s->head : s->tail )
				      : ( s->left == face ? s->tail : s->head );
	return CONTINUE;
    }

    /* Edge intersects faces cone. Check deriv. */
    dt = Plane_Dist(&(face->face_plane), xg->tail);
    dh = Plane_Dist(&(face->face_plane), xg->head);
    dmin = ( minCn ? dt + min * ( dh - dt ) : dt );
    dmax = ( maxCn ? dt + max * ( dh - dt ) : dh );
    if ( dmin <= 0.0 )
    {
	if ( dmax >= 0.0 )
	    return PENETRATING;
    }
    else if ( dmax <= 0.0 )
	return PENETRATING;

    /* At this point, dmin and dmax have the same sign. */
    if ( ( dmin > 0.0 && dt <= dh ) || ( dmin < 0.0 && dt >= dh ) )
    {
	if ( minCn )
	{
	    f2->type = edge_feat;
	    f2->feature = (void*)minCn->edge;
	}
	else
	{
	    xg->coords = xg->tail;
	    xg->feature = edge->tail;

	    f1->type = vertex_feat;
	    f1->feature = edge->tail;
	}
    }
    else
    {
	if ( maxCn )
	{
	    f2->type = edge_feat;
	    f2->feature = (void*)maxCn->edge;
	}
	else
	{
	    xg->coords = xg->head;
	    xg->feature = edge->head;

	    f1->type = vertex_feat;
	    f1->feature = edge->head;
	}
    }

    return CONTINUE;
}


static double
Vert_Vert_Dist(VertexPtr v1, Quaternion *r1, Vector *t1, Vector *cp1,
	       VertexPtr v2, Quaternion *r2, Vector *t2, Vector *cp2,
	       Vector *norm)
{
    double  l;

    QVMul(*r1, v1->posn, *cp1);
    VAdd(*cp1, *t1, *cp1);

    QVMul(*r2, v2->posn, *cp2);
    VAdd(*cp2, *t2, *cp2);

    VSub(*cp2, *cp1, *norm);
    l = VMod(*norm);

    norm->x /= l;
    norm->y /= l;
    norm->z /= l;

    return l;
}


static double
Vert_Edge_Dist(VertexPtr v, Quaternion *r1, Vector *t1, Vector *cp1,
	       EdgePtr e, Quaternion *r2, Vector *t2, Vector *cp2, Vector *norm)
{
    Vector  wp1, wp2, dir, v1;
    double  l;

    QVMul(*r1, v->posn, *cp1);
    VAdd(*cp1, *t1, *cp1);

    QVMul(*r2, e->tail->posn, wp1);
    VAdd(wp1, *t2, wp1);
    QVMul(*r2, e->head->posn, wp2);
    VAdd(wp2, *t2, wp2);
    VSub(wp2, wp1, dir);
    VUnit(dir, l, dir);

    VSub(*cp1, wp1, v1);

    l = VDot(dir, v1);
    VScalarMul(dir, l, *cp2);
    VAdd(*cp2, wp1, *cp2);

    VSub(*cp2, *cp1, *norm);
    l = VMod(*norm);

    norm->x /= l;
    norm->y /= l;
    norm->z /= l;

    return l;
}


static double
Vert_Face_Dist(VertexPtr v, Quaternion *r1, Vector *t1, Vector *cp1,
	       FacePtr f, Quaternion *r2, Vector *t2, Vector *cp2, Vector *norm)
{
    Vector  pt, v1;
    double  temp_d;

    QVMul(*r1, v->posn, *cp1);
    VAdd(*cp1, *t1, *cp1);

    QVMul(*r2, f->face_plane.normal, *norm);
    VUnit(*norm, temp_d, *norm);
    QVMul(*r2, f->vertices[0]->posn, pt);
    VAdd(pt, *t2, pt);

    VSub(*cp1, pt, v1);
    temp_d = VDot(v1, *norm);
    VScalarMul(*norm, -temp_d, pt);
    VAdd(pt, *cp1, *cp2);

    return temp_d;
}


static double
Edge_Edge_Dist(EdgePtr e1, Quaternion *r1, Vector *t1, Vector *cp1,
	       EdgePtr e2, Quaternion *r2, Vector *t2, Vector *cp2,
	       Vector *norm)
{
    Vector  d1, d2;
    Vector  p1, p2;
    double  d1d2;
    double  d1d1;
    double  d2d2;
    Vector  q1_q2;
    double  s, t;
    double  denom;
    Vector  v1, v2, v3;
    double  l;

    QVMul(*r1, e1->dir, d1);
    QVMul(*r1, e1->tail->posn, p1);
    VAdd(p1, *t1, p1);

    QVMul(*r2, e2->dir, d2);
    QVMul(*r2, e2->tail->posn, p2);
    VAdd(p2, *t2, p2);

    d1d2 = VDot(d1, d2);
    d1d1 = VDot(d1, d1);
    d2d2 = VDot(d2, d2);

    VSub(p1, p2, q1_q2);
    denom = d1d1 * d2d2 - d1d2 * d1d2;

    VScalarMul(d2, d1d2, v1);
    VScalarMul(d1, d2d2, v2);
    VSub(v1, v2, v3);
    s = VDot(q1_q2, v3) / denom;

    VScalarMul(d2, d1d1, v1);
    VScalarMul(d1, d1d2, v2);
    VSub(v1, v2, v3);
    t = VDot(q1_q2, v3) / denom;

    VScalarMul(d1, s, v1);
    VAdd(v1, p1, *cp1);

    VScalarMul(d2, t, v1);
    VAdd(v1, p2, *cp2);

    VSub(*cp2, *cp1, *norm);
    l = VMod(*norm);
    norm->x /= l;
    norm->y /= l;
    norm->z /= l;

    return l;
}


static double
Edge_Face_Dist(EdgePtr e, Quaternion *r1, Vector *t1, Vector *cp1,
	       FacePtr f, Quaternion *r2, Vector *t2, Vector *cp2, Vector *norm)
{
    Vector  pt, v1;
    Vector  hp, tp;
    double  hd, td;
    double  temp_d;

    QVMul(*r1, e->head->posn, hp);
    VAdd(hp, *t1, hp);
    QVMul(*r1, e->tail->posn, tp);
    VAdd(tp, *t1, tp);

    QVMul(*r2, f->face_plane.normal, *norm);
    VUnit(*norm, temp_d, *norm);
    QVMul(*r2, f->vertices[0]->posn, pt);
    VAdd(pt, *t2, pt);

    VSub(hp, pt, v1);
    hd = VDot(v1, *norm);

    VSub(tp, pt, v1);
    td = VDot(v1, *norm);

    if ( hd < td )
    {
	*cp1 = hp;
	VScalarMul(*norm, -hd, pt);
	VAdd(pt, *cp1, *cp2);

	return hd;
    }
    else
    {
	*cp1 = tp;
	VScalarMul(*norm, -td, pt);
	VAdd(pt, *cp1, *cp2);

	return hd;
    }

    return 0.0;
}


double
VClip_Distance(
    PolyhedronPtr poly1, Quaternion *r1, Vector *x1, FeaturePtr f1, Vector *cp1,
    PolyhedronPtr poly2, Quaternion *r2, Vector *x2, FeaturePtr f2, Vector *cp2,
    Vector *norm)
{
    Vector	    t_1_to_2, t_2_to_1;
    Vector	    v_temp;
    Quaternion	    q_1_to_2, q_2_to_1;
    Quaternion	    q_temp;
    TransformedGeom xg1, xg2;
    int		    loop = 0.0;
    int		    max_iters;
    int	    	    result;
    double	    dist;

    max_iters = ( poly1->num_vertices + poly1->num_faces + poly1->num_edges )
	      * ( poly2->num_vertices + poly2->num_faces + poly2->num_edges )
	      * 100;

    QInvert(*r2, q_temp);
    QQMul(q_temp, *r1, q_1_to_2);
    VSub(*x1, *x2, v_temp);
    QVMul(q_temp, v_temp, t_1_to_2);
    QInvert(*r1, q_temp);
    QQMul(q_temp, *r2, q_2_to_1);
    VSub(*x2, *x1, v_temp);
    QVMul(q_temp, v_temp, t_2_to_1);

    xg1.feature = xg2.feature = NULL;

    /* Initialize features if not given by caller. */
    if ( ! f1->feature )
    {
	f1->type = vertex_feat;
	f1->feature = (void*)poly1->vertices;
	f2->type = vertex_feat;
	f2->feature = (void*)poly2->vertices;
    }

    do {
	switch ( f1->type )
	{
	  case vertex_feat:
	    switch ( f2->type )
	    {
	      case vertex_feat:
		result = Vert_Vert_Test(f1, f2, &xg1, &xg2, &q_1_to_2,
					&t_1_to_2, &q_2_to_1, &t_2_to_1);
		break;

	      case edge_feat:
		result = Vert_Edge_Test(f1, f2, &xg1, &xg2, &q_1_to_2,
					&t_1_to_2, &q_2_to_1, &t_2_to_1);
		break;

	      case face_feat:
		result = Vert_Face_Test(f1, f2, &xg1, &q_1_to_2, &t_1_to_2,
					poly2->num_faces, poly2->faces);
		break;
	    }
	    break;

	  case edge_feat:
	    switch ( f2->type )
	    {
	      case vertex_feat:
		result = Vert_Edge_Test(f2, f1, &xg2, &xg1, &q_2_to_1,
					&t_2_to_1, &q_1_to_2, &t_1_to_2);
		break;
	      
	      case edge_feat:
		result = Edge_Edge_Test(f1, f2, &xg1, &xg2, &q_1_to_2,
					&t_1_to_2, &q_2_to_1, &t_2_to_1);
		break;

	      case face_feat:
		result = Edge_Face_Test(f1, f2, &xg1, &q_1_to_2, &t_1_to_2);
		break;
	    }
	    break;

	  case face_feat:
	    switch ( f2->type )
	    {
	      case vertex_feat:
		result = Vert_Face_Test(f2, f1, &xg2, &q_2_to_1, &t_2_to_1,
					poly1->num_faces, poly1->faces);
		break;

	      case edge_feat:
		result = Edge_Face_Test(f2, f1, &xg2, &q_2_to_1, &t_2_to_1);
		break;

	      case face_feat:
		fprintf(stderr, "Face Face test should never happen\n");
		abort();
	    }
	    break;
	}
	loop++;
    } while ( ( result == CONTINUE ) && ( loop < max_iters ) );

    if ( loop == max_iters )
    {
	fprintf(stderr, "Cycling in vclip\n");
	abort();
    }

    if ( result == PENETRATING )
	return -1.0;

    /* f1 and f2 hold the closest features. Now find the closest pts and
    ** distance.
    */
    switch ( f1->type )
    {
      case vertex_feat:
	switch ( f2->type )
	{
	  case vertex_feat:
	    return Vert_Vert_Dist((VertexPtr)f1->feature, r1, x1, cp1,
				  (VertexPtr)f2->feature, r2, x2, cp2, norm);

	  case edge_feat:
	    return Vert_Edge_Dist((VertexPtr)f1->feature, r1, x1, cp1,
				  (EdgePtr)f2->feature, r2, x2, cp2, norm);

	  case face_feat:
	    dist = Vert_Face_Dist((VertexPtr)f1->feature, r1, x1, cp1,
				  (FacePtr)f2->feature, r2, x2, cp2, norm);
	    norm->x = -norm->x;
	    norm->y = -norm->y;
	    norm->z = -norm->z;
	    return dist;
	}

      case edge_feat:
	switch ( f2->type )
	{
	  case vertex_feat:
	    dist = Vert_Edge_Dist((VertexPtr)f2->feature, r2, x2, cp2,
				  (EdgePtr)f1->feature, r1, x1, cp1, norm);
	    norm->x = -norm->x;
	    norm->y = -norm->y;
	    norm->z = -norm->z;
	    return dist;
	  
	  case edge_feat:
	    return Edge_Edge_Dist((EdgePtr)f1->feature, r1, x1, cp1,
				  (EdgePtr)f2->feature, r2, x2, cp2, norm);

	  case face_feat:
	    dist = Edge_Face_Dist((EdgePtr)f1->feature, r1, x1, cp1,
				  (FacePtr)f2->feature, r2, x2, cp2, norm);
	    norm->x = -norm->x;
	    norm->y = -norm->y;
	    norm->z = -norm->z;
	    return dist;
	}

      case face_feat:
	switch ( f2->type )
	{
	  case vertex_feat:
	    return Vert_Face_Dist((VertexPtr)f2->feature, r2, x2, cp2,
				  (FacePtr)f1->feature, r1, x1, cp1, norm);

	  case edge_feat:
	    return Edge_Face_Dist((EdgePtr)f2->feature, r2, x2, cp2,
				  (FacePtr)f1->feature, r1, x1, cp1, norm);

	  case face_feat:
	    fprintf(stderr, "Face Face dist should never happen\n");
	    abort();
	}
    }

    return 0.0;
}


