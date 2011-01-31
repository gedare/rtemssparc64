/*
** Copyright (C) 2001 Stephen Chenney, University of Wisconsin at Madison
** schenney@acm.org
**
** May be used for non-commercial experimentation, with no warranty at all.
*/

/*
 * $Header: /v/hank/v90/code2/db/cvs/heaplayers/benchmarks/boxed-sim/red_black.c,v 1.1 2001/05/01 21:23:18 emery Exp $
 *
 * $Log: red_black.c,v $
 * Revision 1.1  2001/05/01 21:23:18  emery
 * Initial check-in, works under Windows.
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <red_black.h>


typedef enum _Color { red, black } Color;

typedef struct _RBTreeNode {
    unsigned long	key;
    void	    	*value;
    Color		color;
    struct _RBTreeNode	*parent;
    struct _RBTreeNode	*left;
    struct _RBTreeNode	*right;
    } RBTreeNode, *RBTreeNodePtr;

typedef struct _RBTreeRoot {
    RBTreeNodePtr   root;
    } RBTreeRoot;


static RBTreeNodePtr	free_list = NULL;


RBTree
RBTree_New()
{
    RBTree  res = (RBTree)malloc(sizeof(RBTreeRoot));
    res->root = NULL;

    return res;
}


static void*
Search_Main(RBTreeNodePtr node, unsigned long key)
{
    while ( node )
    {
	if ( node->key == key )
	    return node->value;

	if ( node->key < key )
	    node = node->right;
	else
	    node = node->left;
    }

    return (void*)-1;
}


void*
RBTree_Search(RBTree tree, unsigned long key)
{
    return Search_Main(tree->root, key);
}


static void*
Tree_Insert(RBTree tree, RBTreeNodePtr node)
{
    RBTreeNodePtr   p, c;

    if ( ! tree->root )
    {
	tree->root = node;
	return (void*)-1;
    }

    p = NULL;
    c = tree->root;
    while ( c )
    {
	p = c;
	if ( c->key == node->key )
	    return node->value;
	if ( c->key < node->key )
	    c = c->right;
	else
	    c = c->left;
    }
    node->parent = p;
    if ( p->key < node->key )
	p->right = node;
    else
	p->left = node;

    return (void*)-1;
}


static void
Left_Rotate(RBTree tree, RBTreeNodePtr x)
{
    RBTreeNodePtr   y = x->right;

    x->right = y->left;
    if ( x->right != NULL )
	x->right->parent = x;
    y->parent = x->parent;
    if ( y->parent )
    {
	if ( x == x->parent->left )
	    x->parent->left = y;
	else
	    x->parent->right = y;
    }
    else
	tree->root = y;
    y->left = x;
    x->parent = y;
}


static void
Right_Rotate(RBTree tree, RBTreeNodePtr x)
{
    RBTreeNodePtr   y = x->left;

    x->left = y->right;
    if ( x->left != NULL )
	x->left->parent = x;
    y->parent = x->parent;
    if ( y->parent )
    {
	if ( x == x->parent->right )
	    x->parent->right = y;
	else
	    x->parent->left = y;
    }
    else
	tree->root = y;
    y->right = x;
    x->parent = y;
}


/* Returns either (void*)-1 if nothing with that key was there, otherwise
** the value of the thing already there.
*/
void*
RBTree_Insert(RBTree tree, unsigned long key, void *value)
{
    RBTreeNodePtr   new_node, x;
    void    	    *res;

    if ( free_list )
    {
	new_node = free_list;
	free_list = free_list->left;
    }
    else
	new_node = (RBTreeNodePtr)malloc(sizeof(RBTreeNode));

    new_node->parent = NULL;
    new_node->left = new_node->right = NULL;
    new_node->key = key;
    new_node->value = value;
    new_node->color = red;

    if ( ( res = Tree_Insert(tree, new_node) ) != (void*)-1 )
	return res;

    x = new_node;
    while ( x->parent && x->parent->color == red )
    {
	if ( x->parent == x->parent->parent->left )
	{
	    RBTreeNodePtr   y = x->parent->parent->right;
	    if ( y && y->color == red )
	    {
		x->parent->color = black;
		y->color = black;
		x->parent->parent->color = red;
		x = x->parent->parent;
	    }
	    else
	    {
		if ( x == x->parent->right )
		{
		    x = x->parent;
		    Left_Rotate(tree, x);
		}
		x->parent->color = black;
		x->parent->parent->color = red;
		Right_Rotate(tree, x->parent->parent);
	    }
	}
	else
	{
	    RBTreeNodePtr   y = x->parent->parent->left;
	    if ( y && y->color == red )
	    {
		x->parent->color = black;
		y->color = black;
		x->parent->parent->color = red;
		x = x->parent->parent;
	    }
	    else
	    {
		if ( x == x->parent->left )
		{
		    x = x->parent;
		    Right_Rotate(tree, x);
		}
		x->parent->color = black;
		x->parent->parent->color = red;
		Left_Rotate(tree, x->parent->parent);
	    }
	}

    }

    tree->root->color = black;

    return (void*)-1;
}


static void
Apply_Main(RBTreeNodePtr node,
	   void (*func)(unsigned long, void*, void*), void *data)
{
    if ( node->left )
	Apply_Main(node->left, func, data);
    (*func)(node->key, node->value, data);
    if ( node->right )
	Apply_Main(node->right, func, data);
}


void
RBTree_Apply(RBTree tree, void (*func)(unsigned long, void*, void*), void *data)
{
    if ( tree->root )
	Apply_Main(tree->root, func, data);
}


static RBTreeNodePtr
Copy_Main(RBTreeNodePtr src,
	  void* (*func)(unsigned long, void*, void*), void *data)
{
    RBTreeNodePtr   dest;

    if ( free_list )
    {
	dest = free_list;
	free_list = free_list->left;
    }
    else
	dest = (RBTreeNodePtr)malloc(sizeof(RBTreeNode));

    dest->key = src->key;
    if ( src->left )
    {
	dest->left = Copy_Main(src->left, func, data);
	dest->left->parent = dest;
    }
    else
	dest->left = NULL;
    dest->value = (*func)(src->key, src->value, data);
    if ( src->right )
    {
	dest->right = Copy_Main(src->right, func, data);
	dest->right->parent = dest;
    }
    else
	dest->right = NULL;
    dest->color = src->color;

    return dest;
}


void
RBTree_Copy(RBTree src, RBTree dest,
	    void* (*func)(unsigned long, void*, void*), void *data)
{
    if ( src->root )
	dest->root = Copy_Main(src->root, func, data);
    else
	dest->root = NULL;
}


static void
Clear_Main(RBTreeNodePtr node, void (*func)(void*, void*), void *data)
{
    if ( node->left )
	Clear_Main(node->left, func, data);
    (*func)(node->value, data);
    if ( node->right )
	Clear_Main(node->right, func, data);
    node->left = free_list;
    free_list = node;
}


void
RBTree_Clear(RBTree tree, void (*func)(void*, void*), void *data)
{
    if ( tree->root )
    {
	Clear_Main(tree->root, func, data);
	tree->root = NULL;
    }
}


static void
Print_Main(RBTreeNodePtr node, FILE *f, int indent,
           void (*func)(FILE*, void*, int))
{
    int	i;

    if ( node->left )
	Print_Main(node->left, f, indent + 2, func);
    for ( i = 0 ; i < indent ; i++ )
	fprintf(f, " ");
    fprintf(f, "%lu: %s:\n", node->key, node->color == red ? "red" : "black");
    (*func)(f, node->value, indent);
    if ( node->right )
	Print_Main(node->right, f, indent + 2, func);
}


void
RBTree_Print(RBTree tree, FILE *f, void (*func)(FILE*, void*, int))
{
    if ( tree->root )
	Print_Main(tree->root, f, 0, func);
    else
	fprintf(f, "Empty");
}


