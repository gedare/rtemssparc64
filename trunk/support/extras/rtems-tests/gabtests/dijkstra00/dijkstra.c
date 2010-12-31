/*
 * dijkstra.c
 *
 * the dijkstra benchmark code from MIBench with some small modifications
 * to facilitate running dijkstra's algorithm inside of RTEMS tasks
 *
 * The MIBench licensing applies to this file (GPL)
 */
#include "system.h"
#include <stdio.h>

#define NUM_NODES                          100
#define NONE                               9999

struct _NODE
{
  int iDist;
  int iPrev;
};
typedef struct _NODE NODE;

struct _QITEM
{
  int iNode;
  int iDist;
  int iPrev;
  struct _QITEM *qNext;
};
typedef struct _QITEM QITEM;

static QITEM *qHead = NULL;




static int AdjMatrix[NUM_NODES][NUM_NODES];

static int g_qCount = 0;
static NODE rgnNodes[NUM_NODES];
static int ch;
static int iPrev, iNode;
static int iCost, iDist;


static void print_path (NODE *rgnNodes, int chNode)
{
   /* Recursion is bad. for now print the path backwards*/
  while(chNode != NONE) {
    printf(" %d", chNode);
    chNode = rgnNodes[chNode].iPrev;
  }
#if 0
  if (rgnNodes[chNode].iPrev != NONE)
    {
      print_path(rgnNodes, rgnNodes[chNode].iPrev);
    }
  printf (" %d", chNode);
  fflush(stdout);
#endif
}


static void enqueue (int iNode, int iDist, int iPrev)
{
  QITEM *qNew = (QITEM *) malloc(sizeof(QITEM));
  QITEM *qLast = qHead;

  if (!qNew)
    {
      fprintf(stderr, "Out of memory.\n");
      MAGIC_BREAKPOINT;
      while(1);
    }
  qNew->iNode = iNode;
  qNew->iDist = iDist;
  qNew->iPrev = iPrev;
  qNew->qNext = NULL;

  if (!qLast)
    {
      qHead = qNew;
    }
  else
    {
      while (qLast->qNext) qLast = qLast->qNext;
      qLast->qNext = qNew;
    }
  g_qCount++;
  //               ASSERT(g_qCount);
}


static void dequeue (int *piNode, int *piDist, int *piPrev)
{
  QITEM *qKill = qHead;

  if (qHead)
    {
      //                 ASSERT(g_qCount);
      *piNode = qHead->iNode;
      *piDist = qHead->iDist;
      *piPrev = qHead->iPrev;
      qHead = qHead->qNext;
      free(qKill);
      g_qCount--;
    }
}


static int qcount (void)
{
  return(g_qCount);
}

static int dijkstra(int chStart, int chEnd)
{
  int i = 0;

  for (ch = 0; ch < NUM_NODES; ch++)
    {
      rgnNodes[ch].iDist = NONE;
      rgnNodes[ch].iPrev = NONE;
    }
  if (chStart == chEnd)
    {
      printf("Shortest path is 0 in cost. Just stay where you are.\n");
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
      printf("Shortest path is %d in cost. ", rgnNodes[chEnd].iDist);
      printf("Path is: ");
      print_path(rgnNodes, chEnd);
      printf("\n");
    }
  return 0;
}

//int main(int argc, char *argv[]) {
int dijkstra_setup( ) {
  char in_file[20] = "/input.dat";
  int i,j,k;
  FILE *fp;

  printf("Unpacking tar filesystem\nThis may take awhile...\n");
  if(Untar_FromMemory(FileSystemImage, FileSystemImage_size) != 0) {
    printf("Can't unpack tar filesystem\n");
    return -1;
  }

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

  return 0;
}

rtems_task Tasks_Dijkstra(
    rtems_task_argument argument
)
{
  uint32_t          i = 0;
  uint32_t          j = NUM_NODES/2;
  rtems_status_code status;

  /* Barrier: tasks will be released by the init function */
  status = rtems_semaphore_obtain( tasks_complete_sem, 
      RTEMS_DEFAULT_OPTIONS, 0 );

  rtems_task_wake_after( Phases[ argument ] );

  /* active computing */
  /* TODO: condition behavior on parameters */
  /* finds 10 shortest paths between nodes */
  for ( ; i<20; ++i,++j) {
    printf("Computing at %d, %d\n",i,j);
			j = (j % NUM_NODES);
      dijkstra(i,j);
  }
  printf("Done\n");

  /* finish */
  status = rtems_semaphore_obtain( tasks_complete_sem, 
      RTEMS_DEFAULT_OPTIONS, 0 );
  directive_failed( status, "rtems_semaphore_obtain" );
    tasks_completed++;
      printf("Done\n");

    if ( NUM_APERIODIC_TASKS == tasks_completed ) {
      MAGIC_BREAKPOINT;
      rtems_test_exit( 0 );
    }
  status = rtems_semaphore_release( tasks_complete_sem );
  directive_failed( status, "rtems_semaphore_release" );

  /* Should block forever */
  status = rtems_semaphore_obtain( final_barrier, RTEMS_DEFAULT_OPTIONS, 0 );
  directive_failed( status, "rtems_semaphore_obtain" );

  /* Shouldn't reach this */
  printf( "Killing task %d\n", argument);
  status = rtems_task_delete(RTEMS_SELF);
  directive_failed(status, "rtems_task_delete of RTEMS_SELF");
}
