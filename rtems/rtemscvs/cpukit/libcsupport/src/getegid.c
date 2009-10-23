/*
 *  $Id: getegid.c,v 1.3 2004/04/18 06:05:34 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <limits.h>
#include <string.h>
#include <sys/types.h>

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/seterr.h>
#include <rtems/userenv.h>

/*
 * MACRO in userenv.h
gid_t _POSIX_types_Egid = 0;
*/

/*PAGE
 *
 *  4.2.1 Get Real User, Effective User, Ral Group, and Effective Group IDs,
 *        P1003.1b-1993, p. 84
 */

gid_t getegid( void )
{
  return _POSIX_types_Egid;
}
