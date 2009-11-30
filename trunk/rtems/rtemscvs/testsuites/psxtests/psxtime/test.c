/*
 *  This test exercises the time of day services via the Classic
 *  and POSIX APIs to make sure they are consistent.
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: test.c,v 1.13 2009/10/25 17:51:20 ralf Exp $
 */

#include <tmacros.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <rtems.h>
#include <rtems/libio.h>
#include <sys/time.h>

#if !HAVE_DECL_ADJTIME
extern int adjtime(const struct timeval *delta, struct timeval *olddelta);
#endif

void test_adjtime(void);
void check_a_tod(
  rtems_time_of_day *the_tod
);

/*
 *  List of dates and times to test.
 */
#define NUMBER_OF_DATES   8
rtems_time_of_day Dates[ NUMBER_OF_DATES ] = {
  /* YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, TICKS */
  {  1988,   1,    1,   12,    45,     00,     0 },
  {  1988,  12,   31,    9,    00,     00,     0 },
  {  1999,  12,   31,   23,    55,     59,     0 },
  {  1999,  06,   30,   00,    01,     30,     0 },
  {  2000,   1,    1,    0,    15,     59,     0 },
  {  2005,   2,    2,    5,    10,     59,     0 },
  {  2010,   3,    3,   10,     5,     59,     0 },
  {  2020,   4,    4,   15,     0,     59,     0 },
};

/*
 *  Check out a single date and time
 */

void check_a_tod(
  rtems_time_of_day *the_tod
)
{
  rtems_status_code status;
  rtems_time_of_day new_tod;
  time_t            a_time_t;
  struct timeval    tv;
  struct tm        *a_tm;
  int               result;
  int               i = 0;

  print_time( "rtems_clock_set          ", the_tod, "\n" );
  status = rtems_clock_set( the_tod );
  assert( !status );

  do {
    status = rtems_clock_get_tod( &new_tod );
    assert( !status );
    print_time( "rtems_clock_get_tod          ", &new_tod, "\n" );

    /* now do the posix time gets */
    result = gettimeofday( &tv, 0 );
    assert( result == 0 );
    a_time_t = tv.tv_sec;   /* ctime() takes a time_t */
    printf( "gettimeofday: %s", ctime( &a_time_t) );

    a_time_t = time( 0 );
    printf( "time:         %s", ctime( &a_time_t ) );

    a_tm = localtime( &a_time_t );
    printf( "localtime:    %s", asctime( a_tm ) );

    a_tm = gmtime( &a_time_t );
    printf( "gmtime:       %s\n",  asctime( a_tm ) );

    status = rtems_task_wake_after( 5 * rtems_clock_get_ticks_per_second() );

    i++;

  } while( i < 6 );
}

void test_adjtime(void)
{
  int                sc;
  rtems_status_code  status;
  struct timeval     delta;
  struct timeval     olddelta;
  rtems_time_of_day *the_tod;
  rtems_time_of_day  tod;
  rtems_interval     ticks;
  
  the_tod = &Dates[0];

  print_time( "rtems_clock_set          ", the_tod, "\n" );
  status = rtems_clock_set( the_tod );
  assert( !status );

  delta.tv_sec = 0;
  delta.tv_usec = 0;
  olddelta.tv_sec = 0;
  olddelta.tv_usec = 0;

  puts( "adjtime - NULL delta - EINVAL" );
  sc = adjtime( NULL, &olddelta );
  assert( sc == -1 );
  assert( errno == EINVAL );

  puts( "adjtime - delta out of range - EINVAL" );
  delta.tv_usec = 1000000000; /* 100 seconds worth */
  sc = adjtime( &delta, &olddelta );
  assert( sc == -1 );
  assert( errno == EINVAL );

  puts( "adjtime - delta too small - do nothing" );
  delta.tv_sec = 0;
  delta.tv_usec = 1;
  sc = adjtime( &delta, &olddelta );
  assert( sc == 0 );

  puts( "adjtime - delta too small - do nothing, olddelta=NULL" );
  sc = adjtime( &delta, NULL );
  assert( sc == 0 );

  puts( "adjtime - delta of one second forward" );
  delta.tv_sec = 1;
  delta.tv_usec = 0;
  sc = adjtime( &delta, &olddelta );
  assert( sc == 0 );

  puts( "adjtime - delta of almost two seconds forward" );
  delta.tv_sec = 1;
  delta.tv_usec = 1000000 - 1;
  sc = adjtime( &delta, &olddelta );
  assert( sc == 0 );

  /*
   * spin until over 1/2 of the way to the 
   */
  ticks = rtems_clock_get_ticks_per_second();
  assert( ticks );
  ticks /= 2;
  do {
    status = rtems_clock_get_tod( &tod );
    assert( !status );
  } while ( tod.ticks <= ticks );

  puts( "adjtime - delta of almost one second forward which bumps second" );
  delta.tv_sec = 0;
  delta.tv_usec = 1000000 - 1;
  sc = adjtime( &delta, &olddelta );
  assert( sc == 0 );

  status = rtems_clock_get_tod( &tod );
  assert( !status );
  print_time( "rtems_clock_get_tod          ", &tod, "\n" );
}

/*
 *  main entry point to the test
 */

#if defined(__rtems__)
int test_main(void);

int test_main(void)
#else
int main(
  int    argc,
  char **argv
)
#endif
{
  int i;

  puts( "\n\n*** POSIX TIME OF DAY TEST ***" );

  test_adjtime();

  /*
   * Now test a number of dates
   */

  i = 0;
  while ( i < NUMBER_OF_DATES ) {
    check_a_tod( &Dates[i] );
    i++;
  }

  puts( "\n\n*** END OF TIME OF DAY TEST 01 ***" );
  rtems_test_exit(0);
}
