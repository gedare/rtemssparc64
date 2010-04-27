/*
 *  RTEMS Object Helper -- Obtain Name of Object as String
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: rtemsobjectgetname.c,v 1.3 2009/11/30 15:59:55 ralf Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/object.h>

/*
 *  This method obtain the name of an object and returns its name
 *  in the form of a C string.  It attempts to be careful about
 *  overflowing the user's string and about returning unprintable characters.
 */
char *rtems_object_get_name(
  Objects_Id     id,
  size_t         length,
  char          *name
)
{
  return _Objects_Get_name_as_string( id, length, name );
}
