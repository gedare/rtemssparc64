/*  tmacros.h
 *
 *  This include file contains macros which are useful in the RTEMS
 *  test suites.
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: tmacros.h,v 1.34 2009/08/12 20:50:51 joel Exp $
 */

#ifndef __TMACROS_h
#define __TMACROS_h

#ifdef __cplusplus
extern "C" {
#endif

#include <bsp.h>    /* includes <rtems.h> */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <rtems/error.h>

#define FOREVER 1                  /* infinite loop */

#ifdef CONFIGURE_INIT
#define TEST_EXTERN
#else
#define TEST_EXTERN extern
#endif

#include <buffer_test_io.h>

/*
 *  Check that that the dispatch disable level is proper for the
 *  mode/state of the test.  Normally it should be 0 when in task space.
 */

#define check_dispatch_disable_level( _expect ) \
  do { \
    extern volatile uint32_t   _Thread_Dispatch_disable_level; \
    if ( (_expect) != -1 && _Thread_Dispatch_disable_level != (_expect) ) { \
      printf( "\n_Thread_Dispatch_disable_level is (%d) not %d\n", \
              _Thread_Dispatch_disable_level, (_expect) ); \
      FLUSH_OUTPUT(); \
      rtems_test_exit( 1 ); \
    } \
  } while ( 0 )

/*
 *  These macros properly report errors within the Classic API
 */

#define directive_failed( _dirstat, _failmsg )  \
 fatal_directive_status( _dirstat, RTEMS_SUCCESSFUL, _failmsg )

#define directive_failed_with_level( _dirstat, _failmsg, _level )  \
 fatal_directive_status_with_level( \
      _dirstat, RTEMS_SUCCESSFUL, _failmsg, _level )

#define fatal_directive_status( _stat, _desired, _msg ) \
  fatal_directive_status_with_level( _stat, _desired, _msg, 0 )

#define fatal_directive_check_status_only( _stat, _desired, _msg ) \
  do { \
    if ( (_stat) != (_desired) ) { \
      printf( "\n%s FAILED -- expected (%s) got (%s)\n", \
              (_msg), rtems_status_text(_desired), rtems_status_text(_stat) ); \
      FLUSH_OUTPUT(); \
      rtems_test_exit( _stat ); \
    } \
  } while ( 0 )

#define fatal_directive_status_with_level( _stat, _desired, _msg, _level ) \
  do { \
    check_dispatch_disable_level( _level ); \
    fatal_directive_check_status_only( _stat, _desired, _msg ); \
  } while ( 0 )

/*
 *  These macros properly report errors from the POSIX API
 */

#define posix_service_failed( _dirstat, _failmsg )  \
 fatal_posix_service_status( _dirstat, RTEMS_SUCCESSFUL, _failmsg )

#define posix_service_failed_with_level( _dirstat, _failmsg, _level )  \
 fatal_posix_service_status_with_level( \
      _dirstat, RTEMS_SUCCESSFUL, _failmsg, _level )

#define fatal_posix_service_status_errno( _stat, _desired, _msg ) \
  if ( (_stat != -1) && (errno) != (_desired) ) { \
    check_dispatch_disable_level( 0 ); \
    printf( "\n%s FAILED -- expected (%d - %s) got (%d %d - %s)\n", \
	    (_msg), _desired, strerror(_desired), \
            _stat, errno, strerror(errno) ); \
    FLUSH_OUTPUT(); \
    rtems_test_exit( _stat ); \
  }

#define fatal_posix_service_status( _stat, _desired, _msg ) \
  fatal_posix_service_status_with_level( _stat, _desired, _msg, 0 )

#define fatal_posix_service_status_with_level( _stat, _desired, _msg, _level ) \
  do { \
    check_dispatch_disable_level( _level ); \
    if ( (_stat) != (_desired) ) { \
      printf( "\n%s FAILED -- expected (%d - %s) got (%d - %s)\n", \
              (_msg), _desired, strerror(_desired), _stat, strerror(_stat) ); \
      printf( "\n FAILED -- errno (%d - %s)\n", \
              errno, strerror(errno) ); \
      FLUSH_OUTPUT(); \
      rtems_test_exit( _stat ); \
    } \
  } while ( 0 )

/*
 *  Generic integer version of the error reporting
 */

#define int_service_failed( _dirstat, _failmsg )  \
 fatal_int_service_status( _dirstat, RTEMS_SUCCESSFUL, _failmsg )

#define int_service_failed_with_level( _dirstat, _failmsg, _level )  \
 fatal_int_service_status_with_level( \
      _dirstat, RTEMS_SUCCESSFUL, _failmsg, _level )

#define fatal_int_service_status( _stat, _desired, _msg ) \
  fatal_int_service_status_with_level( _stat, _desired, _msg, 0 )

#define fatal_int_service_status_with_level( _stat, _desired, _msg, _level ) \
  do { \
    check_dispatch_disable_level( _level ); \
    if ( (_stat) != (_desired) ) { \
      printf( "\n%s FAILED -- expected (%d) got (%d)\n", \
              (_msg), (_desired), (_stat) ); \
      FLUSH_OUTPUT(); \
      rtems_test_exit( _stat ); \
    } \
  } while ( 0 )


/*
 *  Print the time
 */

#define sprint_time(_str, _s1, _tb, _s2) \
  do { \
    sprintf( (_str), "%s%02d:%02d:%02d   %02d/%02d/%04d%s", \
       _s1, (_tb)->hour, (_tb)->minute, (_tb)->second, \
       (_tb)->month, (_tb)->day, (_tb)->year, _s2 ); \
  } while ( 0 )

#define print_time(_s1, _tb, _s2) \
  do { \
    printf( "%s%02d:%02d:%02d   %02d/%02d/%04d%s", \
       _s1, (_tb)->hour, (_tb)->minute, (_tb)->second, \
       (_tb)->month, (_tb)->day, (_tb)->year, _s2 ); \
  } while ( 0 )

#define put_dot( _c ) \
  do { \
    putchar( _c ); \
    FLUSH_OUTPUT(); \
  } while ( 0 )

#define new_line  puts( "" )

#define puts_nocr printf

#ifdef RTEMS_TEST_NO_PAUSE
#define rtems_test_pause() \
    do { \
      printf( "<pause>\n" ); \
      FLUSH_OUTPUT(); \
  } while ( 0 )

#define rtems_test_pause_and_screen_number( _screen ) \
  do { \
    printf( "<pause - screen %d>\n", (_screen) ); \
    FLUSH_OUTPUT(); \
  } while ( 0 )
#else
#define rtems_test_pause() \
  do { \
    char buffer[ 80 ]; \
    printf( "<pause>" ); \
    FLUSH_OUTPUT(); \
    gets( buffer ); \
    puts( "" ); \
  } while ( 0 )

#define rtems_test_pause_and_screen_number( _screen ) \
  do { \
    char buffer[ 80 ]; \
    printf( "<pause - screen %d>", (_screen) ); \
    FLUSH_OUTPUT(); \
    gets( buffer ); \
    puts( "" ); \
  } while ( 0 )
#endif

#define put_name( name, crlf ) \
{ uint32_t   c0, c1, c2, c3; \
  c0 = (name >> 24) & 0xff; \
  c1 = (name >> 16) & 0xff; \
  c2 = (name >> 8) & 0xff; \
  c3 = name & 0xff; \
  putchar( (isprint(c0)) ? c0 : '*' ); \
  if ( c1 ) putchar( (isprint(c1)) ? c1 : '*' ); \
  if ( c2 ) putchar( (isprint(c2)) ? c2 : '*' ); \
  if ( c3 ) putchar( (isprint(c3)) ? c3 : '*' ); \
  if ( crlf ) \
    putchar( '\n' ); \
}

#ifndef build_time
#define build_time( TB, MON, DAY, YR, HR, MIN, SEC, TK ) \
  { (TB)->year   = YR;  \
    (TB)->month  = MON; \
    (TB)->day    = DAY; \
    (TB)->hour   = HR;  \
    (TB)->minute = MIN; \
    (TB)->second = SEC; \
    (TB)->ticks  = TK; }
#endif

#define task_number( tid ) \
  ( rtems_object_id_get_index( tid ) - \
      rtems_configuration_get_rtems_api_configuration()-> \
        number_of_initialization_tasks )

#define rtems_test_assert(__exp) \
  if (!(__exp)) { \
    printf( "%s: %d %s\n", __FILE__, __LINE__, #__exp ); \
    rtems_test_exit(0); \
  }

#ifdef __cplusplus
}
#endif

#endif
