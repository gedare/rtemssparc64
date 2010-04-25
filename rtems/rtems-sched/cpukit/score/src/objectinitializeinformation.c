/*
 *  Object Handler
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: objectinitializeinformation.c,v 1.16 2009/09/11 14:54:29 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/address.h>
#include <rtems/score/chain.h>
#include <rtems/score/object.h>
#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/score/objectmp.h>
#endif
#include <rtems/score/thread.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/sysstate.h>
#include <rtems/score/isr.h>

/*PAGE
 *
 *  _Objects_Initialize_information
 *
 *  This routine initializes all object information related data structures.
 *
 *  Input parameters:
 *    information         - object information table
 *    maximum             - maximum objects of this class
 *    size                - size of this object's control block
 *    is_string           - true if names for this object are strings
 *    maximum_name_length - maximum length of each object's name
 *    When multiprocessing is configured,
 *      supports_global     - true if this is a global object class
 *      extract_callout     - pointer to threadq extract callout
 *
 *  Output parameters:  NONE
 */

void _Objects_Initialize_information(
  Objects_Information *information,
  Objects_APIs         the_api,
  uint32_t             the_class,
  uint32_t             maximum,
  uint16_t             size,
  bool                 is_string,
  uint32_t             maximum_name_length
#if defined(RTEMS_MULTIPROCESSING)
  ,
  bool                 supports_global,
  Objects_Thread_queue_Extract_callout extract
#endif
)
{
  static Objects_Control *null_local_table = NULL;
  uint32_t                minimum_index;
  uint32_t                name_length;
#if defined(RTEMS_MULTIPROCESSING)
  uint32_t                index;
#endif

  information->the_api            = the_api;
  information->the_class          = the_class;
#if defined(RTEMS_SCORE_OBJECT_ENABLE_STRING_NAMES)
  information->is_string          = is_string;
#endif

  information->local_table        = 0;
  information->inactive_per_block = 0;
  information->object_blocks      = 0;

  information->inactive           = 0;

  /*
   *  Set the entry in the object information table.
   */

  _Objects_Information_table[ the_api ][ the_class ] = information;

  /*
   *  Set the size of the object
   */

  information->size = size;

  /*
   *  Are we operating in unlimited, or auto-extend mode
   */

  information->auto_extend =
        (maximum & OBJECTS_UNLIMITED_OBJECTS) ? true : false;
  maximum                 &= ~OBJECTS_UNLIMITED_OBJECTS;

  /*
   *  The allocation unit is the maximum value
   */

  information->allocation_size = maximum;

  /*
   *  Provide a null local table entry for the case of any empty table.
   */

  information->local_table = &null_local_table;

  /*
   *  Calculate minimum and maximum Id's
   */

  if ( maximum == 0 ) minimum_index = 0;
  else                minimum_index = 1;

  information->minimum_id =
    _Objects_Build_id( the_api, the_class, _Objects_Local_node, minimum_index );

  /*
   *  Calculate the maximum name length
   */

  name_length = maximum_name_length;

  if ( name_length & (OBJECTS_NAME_ALIGNMENT-1) )
    name_length = (name_length + OBJECTS_NAME_ALIGNMENT) &
                  ~(OBJECTS_NAME_ALIGNMENT-1);

  information->name_length = name_length;

  _Chain_Initialize_empty( &information->Inactive );

  /*
   *  Initialize objects .. if there are any
   */

  if ( maximum ) {

    /*
     *  Reset the maximum value. It will be updated when the information is
     *  extended.
     */

    information->maximum = 0;

    /*
     *  Always have the maximum size available so the current performance
     *  figures are create are met.  If the user moves past the maximum
     *  number then a performance hit is taken.
     */

    _Objects_Extend_information( information );

  }

  /*
   *  Take care of multiprocessing
   */

#if defined(RTEMS_MULTIPROCESSING)
  information->extract = extract;

  if ( (supports_global == true) && _System_state_Is_multiprocessing ) {

    information->global_table =
      (Chain_Control *) _Workspace_Allocate_or_fatal_error(
        (_Objects_Maximum_nodes + 1) * sizeof(Chain_Control)
      );

    for ( index=1; index <= _Objects_Maximum_nodes ; index++ )
      _Chain_Initialize_empty( &information->global_table[ index ] );
   }
   else
     information->global_table = NULL;
#endif
}
