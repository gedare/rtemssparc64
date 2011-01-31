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
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#define CONFIGURE_INIT
#include "system.h"

int main(int argc, char *argv[]);

rtems_task Init(
  rtems_task_argument ignored
)
{
  /* no file input for cfrac */

  printf( "\n\n*** cfrac benchmark ***\n" );

/*

Test inputs to cfrac program, from easiest (quickest) to hardest:

cfrac 23533
output is: 23533 = 233 * 101

cfrac 1000000001930000000057
output is: 1000000001930000000057 = 100000000003 * 10000000019

cfrac 327905606740421458831903
output is: 327905606740421458831903 = 349394839499 * 938495849597


cfrac 4175764634412486014593803028771
output is: 4175764634412486014593803028771 = 493849349348447 * 8455543456565693

cfrac 41757646344123832613190542166099121
output is: 41757646344123832613190542166099121 = 49384934934843479 * 845554345656569399

*/

#define CFRAC1 "23533"
#define CFRAC2 "1000000001930000000057"
#define CFRAC3 "327905606740421458831903"
#define CFRAC4 "4175764634412486014593803028771"
#define CFRAC5 "41757646344123832613190542166099121"
  char *argv[] = {"cfrac", CFRAC3};
  main(2,argv);
  
  printf( "*** end of cfrac benchmark ***\n" );
  exit( 0 );
}

