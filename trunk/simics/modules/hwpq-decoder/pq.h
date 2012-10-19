/*
 * pq.h
 *
 * A priority queue supporting:
 *  enqueue
 *  extract
 *  first (peek)
 * For modelling a HW PQ.  Additional options are included to support special
 * queue structures.
 */

#ifndef _GAB_PQ_H_
#define _GAB_PQ_H_

//#define GAB_DEBUG

//#define GAB_HWPQ
#define GAB_USE_LRU

#include <simics/api.h>
#include <simics/alloc.h>
#include <simics/utils.h>

#include "gab_helper.h"

typedef struct pq_node {
  int             priority;
  uint64          payload;
  struct pq_node *next;
  struct pq_node *prev;
  int valid;
  struct pq_node *next_lru;
} pq_node;

typedef struct priority_queue {
  pq_node *head;
  pq_node *tail;
  pq_node *lru;
  int options;  // 0 for min, 1 for max, 2 for timer
  int max_size;
  int current_size;
  int queue_id;
  int spill_bounds; // num bytes in spill area
  int spill_count;  // number of spilled nodes in spill area
} priority_queue;

void pq_set_max_size(priority_queue *pq, int size)
{
  if ( pq->current_size < size )
    pq->max_size = size;
}

void pq_init(priority_queue *pq, int opts, int size)
{
  pq->head = NULL;
  pq->tail = NULL;
  pq->lru = NULL;
  pq->current_size = 0;
  pq->queue_id = 0;
  pq->spill_bounds = 0;
  pq->spill_count = 0;
  pq->options = opts;
  pq_set_max_size(pq, size);
}

void pq_print_node(pq_node *n) {
  fprintf(stderr,"%d\t%llu\t%p\t%p\t%d\n",n->priority,n->payload,n->next,n->prev,n->valid);
}

void pq_print_queue(priority_queue *pq) {
  pq_node* n = NULL;
  if (!pq) return;
  n = pq->head;
  while(n) {
    pq_print_node(n);
    n = n->next;
  }
}

void pq_extract_lru(priority_queue *pq, pq_node *x_lru) {
  pq_node *tmp = pq->lru;
  if ( tmp == x_lru ) {
    pq->lru = x_lru->next_lru;
    x_lru->next_lru = NULL;
    return;
  }

   while ( tmp ) {
    if (tmp->next_lru == x_lru) {
      tmp->next_lru = x_lru->next_lru;
      x_lru->next_lru = NULL;
      return;
    }
    tmp = tmp->next_lru;
  }
}

void pq_insert_lru(priority_queue *pq, pq_node *new_lru) {
  pq_node *temp = pq->lru;
  new_lru->next_lru = NULL;
  if ( !temp ) {
    pq->lru = new_lru;
    return;
  }
  while ( temp->next_lru ) {
    temp = temp->next_lru;
  }
  temp->next_lru = new_lru;
}

void pq_update_lru(priority_queue *pq, pq_node *new_lru) {
  if ( !new_lru )
    return;

  pq_extract_lru(pq, new_lru);
  pq_insert_lru(pq, new_lru);
}

pq_node* pq_get_lru(priority_queue *pq) {
  return pq->lru;
}

pq_node* pq_extract_last(priority_queue *pq) { 
#ifdef GAB_DEBUG
  fprintf(stderr,"pq_extract_last:\n");
  pq_print_queue(pq);
  fprintf(stderr,"pq tail:\n");
  if (pq->tail)
    pq_print_node(pq->tail);
#endif
  pq_node *last = pq->tail;
  if (last) {
    pq_extract_lru(pq, last);
    pq->tail = last->prev;
    --pq->current_size;
    //pq_update_lru(pq, last);
    //pq->lru = last->next_lru;
  }

  if (pq->tail) {
    pq->tail->next = NULL;
  }
  else
    pq->head = NULL;
  
  return last;
}

pq_node* pq_last(priority_queue *pq) {
  pq_update_lru(pq, pq->tail);
  return pq->tail;
}

pq_node* pq_first(priority_queue *pq) { 
#ifdef GAB_DEBUG
  fprintf(stderr,"pq_first:\n");
  pq_print_queue(pq);
#endif
  if (!pq->head) {
#ifdef GAB_DEBUG
    fprintf(stderr,"Nothing at head of queue\n");
#endif
  } else if (!pq->head->valid) {
    fprintf(stderr, "invalid first node (%d spilled, %d current)\n",pq->spill_count, pq->current_size);
    pq_print_queue(pq);
  }
  pq_update_lru(pq, pq->head);
  return pq->head;
}

pq_node* pq_search(priority_queue *pq, int key)
{
  pq_node *node = pq->head;
  while (node && node->priority != key)
    node = node->next;
  if ( node ) {
    pq_update_lru(pq, node);
  }
  return node;
}

void pq_remove(priority_queue *pq, pq_node *node)
{
  pq_extract_lru(pq, node);
  if (node->prev) 
    node->prev->next = node->next;
  else 
    pq->head = node->next;
  if (node->next) {
    node->next->prev = node->prev;
  } else {
    pq->tail = node->prev;
  }
  --pq->current_size;
}

pq_node* pq_extract(priority_queue *pq, int priority)
{
  pq_node *node = pq_search(pq, priority);

  if (node) {
    pq_remove(pq, node);
  } else {
    return 0;
  }
#ifdef GAB_DEBUG
  fprintf(stderr,"pq_extract:\n");
  pq_print_queue(pq);
  
  fprintf(stderr,"pq tail:\n");
  if (pq->tail)
    pq_print_node(pq->tail);
#endif
  return node;
}

void pq_insert(priority_queue *pq, pq_node *node) 
{
  pq_node *iter = pq->head;
  pq_node *prev = NULL;
  node->next = NULL;
  node->prev = NULL;
  pq_update_lru(pq, node);
  
  if (!iter) {
    pq->head = node;
    pq->tail = node;
#if defined(GAB_HWPQ)
    if (pq->spill_count)
      node->valid = 0;
    else
      node->valid = 1;
#endif
    goto out;
  }
#if defined(GAB_HWPQ)
  node->valid = iter->valid;
#endif

  if ( pq->options == 0 || pq->options == 2 ) { /* min sort, timer chain */
    while( iter ) {
      /* The ordering here matters. First fix the valid bits then see whether
       * to insert the node (before iter). */
#if defined(GAB_HWPQ)
      if (!iter->valid) {
        if (pq->spill_count) {
          node->valid = 0;
        }
        else {
          /* need to update valid bit, nothing is spilled.
           * This can happen due to software extract. */
          iter->valid = 1; 
        }
      }
#endif
      /* see if found node's place */
      if ( node->priority <= iter->priority ) {
        if (iter->prev)
          iter->prev->next = node;
        else
          pq->head = node;
        node->prev = iter->prev;
        node->next = iter;
        iter->prev = node;
        goto out;
      }

      prev = iter;
      iter = iter->next;
    }
  } else if (pq->options == 1) { /* max sort */
    fprintf(stderr,"untested option: max sort\n");
    while( iter ) {
#if defined(GAB_HWPQ)
      if (!iter->valid)  {
        if (pq->spill_count) {
          node->valid = 0;
        }
        else {
          /* need to update valid bit, nothing is spilled.
           * This can happen due to software extract. */
          iter->valid = 1; 
        }
      }
#endif

      if ( node->priority > iter->priority ) {
        if (iter->prev)
          iter->prev->next = node;
        else
          pq->head = node;
        node->prev = iter->prev;
        node->next = iter;
        iter->prev = node;
        goto out;
      }
      prev = iter;
      iter = iter->next;
    }
  } else {
    fprintf(stderr,"unrecognized options to pq: %d\n", pq->options);
    return;
  }

  /* end of list */
  prev->next = node;
  node->next = NULL;
  node->prev = prev;
  pq->tail = node;
#if defined(GAB_HWPQ)
  if (pq->spill_count)
    node->valid = 0; /* special case */ 
#endif
out:
  ++pq->current_size;
  if ( pq->current_size > pq->max_size ) {
    fprintf(stderr,"%d\tHWPQ size exceeded! current size = %d\n", pq->queue_id, pq->current_size);
  }
#ifdef GAB_DEBUG
  fprintf(stderr,"pq_insert\n");
  pq_print_queue(pq);

  fprintf(stderr,"pq tail:\n");
  if (pq->tail)
    pq_print_node(pq->tail);
#endif
}

/**
 * latest and greatest spill and fill routines
 */
uint64 pq_spill(priority_queue *pq) {
  pq_node *last;
  pq_node *tmp;
  uint64 payload;

#ifdef GAB_DEBUG
  fprintf(stderr,"PQ before spill:\n");
  pq_print_queue(pq);
#endif

  // remove last element from hwpq
#if defined(GAB_HWPQ)
  last = pq_extract_last(pq);
#elif defined(GAB_USE_LRU)
  last = pq_get_lru(pq);
  if ( last ) {
    pq_remove(pq, last);
  } else {
    fprintf(stderr, "no lru found!\n");
    last = pq_extract_last(pq);
  }
#else
  last = pq_extract_last(pq);
#endif
  if (!last) {
#ifdef GAB_DEBUG
    fprintf(stderr,"empty queue\n");
#endif
    return 0; // exception?
  }

  // store return value
  payload = last->payload;

  // update count
  ++pq->spill_count;

#if defined(GAB_HWPQ)
  // update valid
  if (pq->tail)
    pq->tail->valid = 0;
#endif
 
#ifdef GAB_DEBUG
  fprintf(stderr,"PQ after spill:\n");
  pq_print_queue(pq);
#endif

  // release resources and return node's payload
  free(last);
  return payload;
}

void pq_fill(priority_queue *pq, pq_node *node)
{
  pq_node *tmp;

#ifdef GAB_DEBUG
  fprintf(stderr,"PQ before fill:\n");
  pq_print_queue(pq);
#endif

  // insert node to hwpq
  pq_insert(pq,node);

  // update count
  --pq->spill_count;

#if defined(GAB_HWPQ)
  // fix valid bits (should be done while inserting, but this is easy)
  tmp = pq->head;
  while (tmp && tmp != node) {
    tmp->valid = 1;
    tmp = tmp->next;
  }
  node->valid = 1;

  // handle special case for when fill last element from SW spill area.
  if (!pq->spill_count) {
    while (tmp) {
      tmp->valid = 1;
      tmp = tmp->next;
    }
  }
#endif

#ifdef GAB_DEBUG
  fprintf(stderr,"PQ after fill:\n");
  pq_print_queue(pq);
#endif
}

/** Overflow/Underflow routines */
int pq_need_spill(priority_queue *pq) {
  return pq->current_size == pq->max_size;
}

int pq_need_fill(priority_queue *pq) {
  if (!pq->head) {
    return pq->spill_count;
  }
#if defined(GAB_HWPQ)
  if (!pq->head->valid) {
    return pq->spill_count;
  }
#endif
  return ((pq->spill_count && pq->current_size < pq->max_size/4));
}

void pq_destroy(priority_queue *pq) {
  pq_node* n = NULL;
  pq_node* next = NULL;
  if (!pq) return;
#ifdef GAB_DEBUG
  fprintf(stderr,"PQ destroy:\n");
  pq_print_queue(pq);
#endif

  next = pq->head;
  while((n = next)) {
    next = n->next;
    free(n);
  }
}

void pq_invalidate(priority_queue *pq) {
  pq_node* n = NULL;

  if (!pq)
    return;

  if ( !pq->spill_count )
    return;

  n = pq->head;
  while(n && n->valid) {
    n->valid = 0;
    n = n->next;
  }
}

#endif
