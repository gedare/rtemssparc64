/*  Init
 *
 *  This routine is the initialization task for this test program.
 *  It is called from init_exec and has the responsibility for creating
 *  and starting the tasks that make up the test.  If the time of day
 *  clock is required for the test, it should also be set to a known
 *  value by this function.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  $Id$
 */

#define CONFIGURE_INIT
#include "system.h"
#include <stdio.h>

#define GAB_PRINT

#define NUM_NODES                          100
#define NONE                               9999

struct _NODE
{
  int iNode;
  int iDist;
  int iPrev;
  int hIndex;
  int hFlag;
};
typedef struct _NODE NODE;

/*
 * Heap implementation for dijkstra
 */
#define HEAP_PARENT(i) (i>>1)
#define HEAP_FIRST (1)
#define HEAP_LEFT(i) (i<<1)
#define HEAP_RIGHT(i) (HEAP_LEFT(i)+1)

NODE* the_heap[NUM_NODES+1];
int heap_current_size;

static inline void swap_entries(int a, int b) {
  NODE *tmp = the_heap[a];
  int tmpIndex = the_heap[a]->hIndex;
  the_heap[a]->hIndex = the_heap[b]->hIndex;
  the_heap[b]->hIndex = tmpIndex;
  the_heap[a] = the_heap[b];
  the_heap[b] = tmp;
}

static void bubble_up( int i )
{
  while ( i > 1 && the_heap[i]->iDist < the_heap[HEAP_PARENT(i)]->iDist ) {
    swap_entries (i, HEAP_PARENT(i));
    i = HEAP_PARENT(i);
  }
}

static void bubble_down( int i ) {
  int j = 0;

  do {
    j = i;
    if ( HEAP_LEFT(j) <= heap_current_size ) {
      if (the_heap[HEAP_LEFT(j)]->iDist < the_heap[i]->iDist)
        i = HEAP_LEFT(j);
    }
    if ( HEAP_RIGHT(j) <= heap_current_size ) {
      if (the_heap[HEAP_RIGHT(j)]->iDist < the_heap[i]->iDist) 
        i = HEAP_RIGHT(j);
    }
    swap_entries(i,j);
  } while (i != j);
}

void heap_initialize( void ) {
  heap_current_size = 0;
}

void heap_insert( NODE *n ) {
  ++heap_current_size;
  the_heap[heap_current_size] = n;
  n->hIndex = heap_current_size;
  bubble_up(heap_current_size);
}

void heap_remove( int i ) {
  if (i < 1 || i > heap_current_size) return;
  swap_entries(i, heap_current_size);
  --heap_current_size;
  bubble_down(1);
}

NODE *heap_min( ) {
  if (heap_current_size) {
    return the_heap[1];
  }
  return NULL;
}

NODE *heap_pop_min( ) {
  NODE *n = heap_min();
  heap_remove(1);
  return n;
}

int AdjMatrix[NUM_NODES][NUM_NODES];

int g_qCount = 0;
NODE rgnNodes[NUM_NODES];
int ch;
int iPrev, iNode;
int iCost, iDist;

void print_path (NODE *rgnNodes, int chNode)
{
#if defined(GAB_PRINT)
  if (rgnNodes[chNode].iPrev != NONE)
    {
      print_path(rgnNodes, rgnNodes[chNode].iPrev);
    }
  printf (" %d", chNode);
  fflush(stdout);
#endif
}

void enqueue (int iNode, int iDist, int iPrev)
{
  int i = 0;
  NODE *hNew = &rgnNodes[iNode];

  if (hNew->hFlag) {
    heap_remove( hNew->hIndex );
    g_qCount--;
  }

  hNew->iDist = iDist;
  hNew->iPrev = iPrev;
  hNew->hFlag = 1;
  heap_insert( hNew );
  g_qCount++;
  //               ASSERT(g_qCount);
}

void dequeue (int *piNode, int *piDist, int *piPrev)
{
  NODE *hKill = NULL;

  hKill = heap_min();

  if (hKill)
    {
      //                 ASSERT(g_qCount);
      *piNode = hKill->iNode;
      *piDist = hKill->iDist;
      *piPrev = hKill->iPrev;
      heap_remove(hKill->hIndex);
      hKill->hFlag = 0;
      hKill->hIndex = 0;
      g_qCount--;
    } 
}

int qcount (void)
{
  return(g_qCount);
}

int dijkstra(int chStart, int chEnd)
{
  int i;
  heap_initialize();
  for (ch = 0; ch < NUM_NODES; ch++)
    {
      rgnNodes[ch].iNode = ch;
      rgnNodes[ch].iDist = NONE;
      rgnNodes[ch].iPrev = NONE;
      rgnNodes[ch].hIndex = 0;
      rgnNodes[ch].hFlag = 0;
    }

  if (chStart == chEnd)
    {
#if defined(GAB_PRINT)
      printf("Shortest path is 0 in cost. Just stay where you are.\n");
#endif
    }
  else
    {
      rgnNodes[chStart].iDist = 0;
      rgnNodes[chStart].iPrev = NONE;

      enqueue (chStart, 0, NONE);

     while (qcount() > 0)
	{
	  dequeue (&iNode, &iDist, &iPrev);
	  for (i = 0; i < NUM_NODES; i++)
	    {
	      if ((iCost = AdjMatrix[iNode][i]) != NONE)
		{
		  if ((NONE == rgnNodes[i].iDist) ||
		      (rgnNodes[i].iDist > (iCost + iDist)))
		    {
		      rgnNodes[i].iDist = iDist + iCost;
		      rgnNodes[i].iPrev = iNode;
		      enqueue (i, iDist + iCost, iNode);
		    }
		}
	    }
	}
#if defined(GAB_PRINT)
      printf("Shortest path is %d in cost. ", rgnNodes[chEnd].iDist);
      printf("Path is: ");
      print_path(rgnNodes, chEnd);
      printf("\n");
#endif
    }
}


rtems_task Init(
  rtems_task_argument ignored
)
{
  char in_file[20] = "/input.dat";
  int i,j,k;
  FILE *fp;

  printf("Unpacking tar filesystem\nThis may take awhile...\n");
  if(Untar_FromMemory(FileSystemImage, FileSystemImage_size) != 0) {
    printf("Can't unpack tar filesystem\n");
    exit(1);
  }

  printf( "\n\n*** dijkstra benchmark ***\n" );

  /* open the adjacency matrix file */
  fp = fopen (in_file,"r");

  /* make a fully connected matrix */
  for (i=0;i<NUM_NODES;i++) {
    for (j=0;j<NUM_NODES;j++) {
      /* make it more sparce */
      fscanf(fp,"%d",&k);
			AdjMatrix[i][j]= k;
    }
  }

  asm volatile("break_start_opal:");
  /* finds 10 shortest paths between nodes */
  for (i=0,j=NUM_NODES/2;i<20;i++,j++) {
			j=j%NUM_NODES;
      dijkstra(i,j);
  }
  MAGIC_BREAKPOINT;

  printf( "*** end of dijkstra benchmark ***\n" );
  exit( 0 );
}

