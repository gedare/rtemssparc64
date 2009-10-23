/*
 *  Exercise Classic API Workspace Wrappers
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: init.c,v 1.2 2009/05/14 14:29:40 joel Exp $
 */

#include <tmacros.h>


rtems_task Init(
  rtems_task_argument argument
)
{
  void                   *p1;
  bool                    retbool;
  Heap_Information_block  info;

  puts( "\n\n*** TEST WORKSPACE CLASSIC API ***" );

  puts( "rtems_workspace_get_information - null pointer" );
  retbool = rtems_workspace_get_information( NULL );
  assert( retbool == false );

  puts( "rtems_workspace_get_information - OK" );
  retbool = rtems_workspace_get_information( &info );
  assert( retbool == true );

  puts( "rtems_workspace_allocate - null pointer" );
  retbool = rtems_workspace_allocate( 42, NULL ); 
  assert( retbool == false );

  puts( "rtems_workspace_allocate - 0 bytes" );
  retbool = rtems_workspace_allocate( 0, &p1 ); 
  assert( retbool == false );

  puts( "rtems_workspace_allocate - too many bytes" );
  retbool = rtems_workspace_allocate( info.Free.largest * 2, &p1 ); 
  assert( retbool == false );

  puts( "rtems_workspace_allocate - 42 bytes" );
  retbool = rtems_workspace_allocate( 42, &p1 ); 
  assert( retbool == true );
  assert( p1 != NULL );

  puts( "rtems_workspace_free - NULL" );
  retbool = rtems_workspace_free( NULL ); 
  assert( retbool == false );

  puts( "rtems_workspace_free - previous pointer to 42 bytes" );
  retbool = rtems_workspace_free( p1 ); 
  assert( retbool == true );

  puts( "*** END OF TEST WORKSPACE CLASSIC API ***" );
  rtems_test_exit( 0 );
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MAXIMUM_TASKS             1

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
