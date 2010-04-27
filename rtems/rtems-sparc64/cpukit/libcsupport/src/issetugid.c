/*
 * Dummy version of BSD routine
 *
 *  $Id: issetugid.c,v 1.2 2009/09/30 08:20:27 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(RTEMS_NEWLIB) && !defined(HAVE_ISSETUGID)
int
issetugid (void)
{
	return 0;
}
#endif
