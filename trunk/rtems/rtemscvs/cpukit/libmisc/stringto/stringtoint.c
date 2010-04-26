/*
 *  COPYRIGHT (c) 2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: stringtoint.c,v 1.2 2010/03/28 15:20:32 ralf Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 *  Instantiate an error checking wrapper for strtol (int)
 */
#define STRING_TO_INTEGER
#define STRING_TO_TYPE int
#define STRING_TO_NAME rtems_string_to_int
#define STRING_TO_METHOD strtol
#define STRING_TO_MAX LONG_MAX
#include "stringto_template.h"
