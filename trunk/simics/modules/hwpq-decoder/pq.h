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

//#define GAB_DEBUG

typedef struct pq_node {
  int             priority;
  uint64          payload;
  struct pq_node *next;
  struct pq_node *prev;
} pq_node;

typedef struct priority_queue {
  pq_node *head;
  int options;  // 0 for min, 1 for max, 2 for timer
} priority_queue;

void pq_print_node(pq_node *n) {
  printf("%d\t%llu\t%p\t%p\n",n->priority,n->payload,n->next,n->prev);
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

pq_node* pq_first(priority_queue *pq) { 
#ifdef GAB_DEBUG
  printf("pq_first:\n");
  pq_print_queue(pq);
#endif
  return pq->head;
}

pq_node* pq_extract(priority_queue *pq, uint64 data)
{
  pq_node *node = pq->head;
  while (node && node->payload != data) node = node->next;

  if (node) {
    if (node->prev) 
      node->prev->next = node->next;
    else 
      pq->head = node->next;
    if (node->next) {
      node->next->prev = node->prev;
    }
  }
#ifdef GAB_DEBUG
  printf("pq_extract:\n");
  pq_print_queue(pq);
#endif
  return node;
}

void pq_insert(priority_queue *pq, pq_node *node) 
{
  pq_node *iter = pq->head;
  pq_node *prev = NULL;
  node->next = NULL;
  node->prev = NULL;
  
  if (!iter) {
    pq->head = node;
    goto out;
  }

  if ( pq->options == 0 || pq->options == 2 ) { /* min sort, timer chain */
    while( iter ) {
      if ( node->priority < iter->priority ) {
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
    while( iter ) {
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
    printf("unrecognized options to pq: %d\n", pq->options);
  }

  /* end of list */
  prev->next = node;
  node->next = NULL;
  node->prev = prev;
out:
#ifdef GAB_DEBUG
  printf("pq_insert\n");
  pq_print_queue(pq);
#else
  ;
#endif
}
