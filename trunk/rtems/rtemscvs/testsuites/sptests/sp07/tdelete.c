/*  Task_delete_extension
 *
 *  This routine is the delete user extension.
 *
 *  Input parameters:
 *    running_task - pointer to running TCB
 *    deleted_task - pointer to TCB deleted
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: tdelete.c,v 1.13 2009/09/26 15:47:16 joel Exp $
 */

#include "system.h"

void Task_delete_extension(
  rtems_tcb *running_task,
  rtems_tcb *deleted_task
)
{
  char line[80];
  rtems_name name;

  if ( task_number( running_task->Object.id ) > 0 ) {
    name = Task_name[ task_number( running_task->Object.id ) ];
    sprintf( line, "TASK_DELETE - %c%c%c%c TASK_DELETE ",
      (name >> 24) & 0xff,
      (name >> 16) & 0xff,
      (name >> 8) & 0xff,
      name & 0xff
    );
    buffered_io_add_string( line );
  }
  if ( task_number( deleted_task->Object.id ) > 0 ) {
    name = Task_name[ task_number( deleted_task->Object.id ) ];
    sprintf( line, "deleting - %c%c%c%c\n",
      (name >> 24) & 0xff,
      (name >> 16) & 0xff,
      (name >> 8) & 0xff,
      name & 0xff
    );
    buffered_io_add_string( line );
  }
}
