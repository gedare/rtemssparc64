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

#define NUM_NODES                          100
#define NONE                               9999

struct _NODE
{
  int iNode;
  int iDist;
  int iPrev;
  int qFlag;
};
typedef struct _NODE NODE;

int AdjMatrix[NUM_NODES][NUM_NODES];

int g_qCount = 0;
NODE rgnNodes[NUM_NODES];
int ch;
int iPrev, iNode;
int iCost, iDist;

void print_path (NODE *rgnNodes, int chNode)
{
  if (rgnNodes[chNode].iPrev != NONE)
    {
      print_path(rgnNodes, rgnNodes[chNode].iPrev);
    }
  printf (" %d", chNode);
  fflush(stdout);
}

void enqueue (int iNode, int iDist, int iPrev)
{
  NODE *qNew = &rgnNodes[iNode];

  if (qNew->qFlag) {
    HWDS_EXTRACT ( 4, qNew );
    g_qCount--;
  }

  qNew->iDist = iDist;
  qNew->iPrev = iPrev;
  qNew->qFlag = 1;
  HWDS_ENQUEUE ( 4, iDist, qNew );
  g_qCount++;
  //               ASSERT(g_qCount);
}

void dequeue (int *piNode, int *piDist, int *piPrev)
{
  NODE *qKill = NULL;

  HWDS_FIRST( 4, qKill );

  if (qKill)
    {
      //                 ASSERT(g_qCount);
      *piNode = qKill->iNode;
      *piDist = qKill->iDist;
      *piPrev = qKill->iPrev;
      HWDS_EXTRACT ( 4, qKill );
      qKill->qFlag = 0;
      g_qCount--;
    } 
}

int qcount (void)
{
  return(g_qCount);
}

int dijkstra(int chStart, int chEnd)
{
  int i = 0;
  for (ch = 0; ch < NUM_NODES; ch++)
  {
    rgnNodes[ch].iNode = ch;
    rgnNodes[ch].iDist = NONE;
    rgnNodes[ch].iPrev = NONE;
    rgnNodes[ch].qFlag = 0;
  }

  if (chStart == chEnd)
  {
    //      printf("Shortest path is 0 in cost. Just stay where you are.\n");
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

