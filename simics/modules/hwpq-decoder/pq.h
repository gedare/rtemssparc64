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

pq_node* pq_first(priority_queue *pq) { return pq->head; }

pq_node* pq_extract(priority_queue *pq, uint64 data)
{
  pq_node *node = pq->head;
  while (node && node->payload != data) node = node->next;
  if (node) {
    if (node->prev) node->prev->next = node->next;
    else pq->head = node->next;
    if (node->next) node->next->prev = node->prev;
  }
  return node;
}

void pq_insert(priority_queue *pq, pq_node *node) 
{
  pq_node *iter = pq->head;
  pq_node *prev = NULL;
  if ( pq->options == 0 ) { /* min sort */
    while( iter ) {
      if ( node->priority < iter->priority ) {
        if (iter->prev)
          iter->prev->next = node;
        else
          pq->head = node;
        node->prev = iter->prev;
        node->next = iter;
        iter->prev = node;
        return;
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
        return;
      }
      prev = iter;
      iter = iter->next;
    }
  } else if (pq->options == 2) { /* timer chain insert */
     while( iter ) {
      if ( node->priority < iter->priority ) {
        if (iter->prev)
          iter->prev->next = node;
        else
          pq->head = node;
        node->prev = iter->prev;
        node->next = iter;
        iter->prev = node;
        return;
      }
      node->priority -= iter->priority;
      prev = iter;
      iter = iter->next;
    }
  } else {
    printf("unrecognized options to pq: %d\n", pq->options);
  }

  // !iter
  if (!prev) { /* empty list */
    pq->head = node;
    node->next = NULL;
    node->prev = NULL;
  } else { /* end of list */
    prev->next = node;
    node->next = NULL;
    node->prev = prev;
  }
}
