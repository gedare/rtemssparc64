/*
 *  COPYRIGHT (c) 2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: stringtounsignedlonglong.c,v 1.1 2009/07/22 14:26:25 joel Exp $
 */

/*
 *  Instantiate an error checking wrapper for strtoull (unsigned long long)
 */
#define STRING_TO_INTEGER
#define STRING_TO_TYPE unsigned long long
#define STRING_TO_NAME rtems_string_to_unsigned_long_long
#define STRING_TO_METHOD strtoull
#define STRING_TO_MAX ULONG_LONG_MAX
#include "stringto_template.h"