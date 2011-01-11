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

//#define GAB_PRINT

#define NUM_NODES                          100
#define NONE                               9999

struct _NODE
{
  int iNode;
  int iDist;
  int iPrev;
  struct _NODE *qNext;
  struct _NODE *qPrev;
  int qFlag;
};
typedef struct _NODE NODE;

NODE *qHead = NULL;

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
  NODE *qLast = NULL;
  NODE *qNew = &rgnNodes[iNode];

  if (qNew->qFlag) {
    if (qNew->qNext) {
      qNew->qNext->qPrev = qNew->qPrev;
    }
    if (qNew->qPrev) {
      qNew->qPrev->qNext = qNew->qNext;
    } else {
      qHead = qNew->qNext;
    }
    g_qCount--;
  }
  
  qNew->iDist = iDist;
  qNew->iPrev = iPrev;
  qNew->qNext = NULL;
  qNew->qPrev = NULL;
  qNew->qFlag = 1;

  qLast = qHead;
  // GAB: instead of tail-queueing, sort entries as a 
  // priority queue by the iDist.
  if (!qLast)
  {
    qHead = qNew;
  }
  else if (qLast->iDist > qNew->iDist) // new case to handle head's priority
  {
    qHead = qNew;
    qNew->qNext = qLast;
    qLast->qPrev = qNew;
  }
  else
  {
    // already handled head case, find the Q element to insert qNew after.
    while (qLast->qNext && qLast->qNext->iDist <= qNew->iDist) {
      qLast = qLast->qNext;
    }
    qNew->qNext = qLast->qNext;
    qNew->qPrev = qLast;
    if (qNew->qNext)
      qNew->qNext->qPrev = qNew;
    qLast->qNext = qNew;
  }
  g_qCount++;
  //               ASSERT(g_qCount);
}

void dequeue (int *piNode, int *piDist, int *piPrev)
{
  NODE *qKill = qHead;

  if (qHead)
    {
      //                 ASSERT(g_qCount);
      *piNode = qHead->iNode;
      *piDist = qHead->iDist;
      *piPrev = qHead->iPrev;
      qHead = qHead->qNext;
      if (qHead)
        qHead->qPrev = NULL;
      qKill->qNext = NULL;
      qKill->qPrev = NULL;
      qKill->qFlag = 0;
//      free(qKill);
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
  for (ch = 0; ch < NUM_NODES; ch++)
    {
      rgnNodes[ch].iNode = ch;
      rgnNodes[ch].iDist = NONE;
      rgnNodes[ch].iPrev = NONE;
      rgnNodes[ch].qPrev = NULL;
      rgnNodes[ch].qNext = NULL;
      rgnNodes[ch].qFlag = 0;
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

