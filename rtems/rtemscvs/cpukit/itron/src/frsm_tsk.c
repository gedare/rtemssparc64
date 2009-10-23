/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: frsm_tsk.c,v 1.11 2007/10/26 20:19:00 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/itron.h>

#include <rtems/score/thread.h>
#include <rtems/score/userext.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/apiext.h>
#include <rtems/score/sysstate.h>

#include <rtems/itron/task.h>


/*
 *  frsm_tsk - Forcibly Resume Suspended Task
 */

ER frsm_tsk(
  ID tskid
)
{
  register Thread_Control *the_thread;
  Objects_Locations        location;

  the_thread = _ITRON_Task_Get( tskid, &location );
  switch ( location ) {
#if defined(RTEMS_MULTIPROCESSING)
    case OBJECTS_REMOTE:
#endif
    case OBJECTS_ERROR:
      return _ITRON_Task_Clarify_get_id_error( tskid );

    case OBJECTS_LOCAL:
      if ( _Thread_Is_executing( the_thread ) )
        _ITRON_return_errorno( E_OBJ );

      if (_States_Is_dormant( the_thread->current_state ))
        _ITRON_return_errorno( E_OBJ );

      if ( ! _States_Is_suspended(the_thread->current_state) )
        _ITRON_return_errorno( E_OK );

      _Thread_Resume( the_thread, TRUE );
      break;
  }

 _ITRON_return_errorno( E_OK );

}
