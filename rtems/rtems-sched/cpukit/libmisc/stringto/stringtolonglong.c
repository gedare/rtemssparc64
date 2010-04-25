/*
 *  COPYRIGHT (c) 2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: stringtolonglong.c,v 1.1 2009/07/22 14:26:25 joel Exp $
 */

/*
 *  Instantiate an error checking wrapper for strtoll (long long)
 */
#define STRING_TO_INTEGER
#define STRING_TO_TYPE long long
#define STRING_TO_NAME rtems_string_to_long_long
#define STRING_TO_METHOD strtoll
#define STRING_TO_MIN LONG_LONG_MIN
#define STRING_TO_MAX LONG_LONG_MAX
#include "stringto_template.h"
