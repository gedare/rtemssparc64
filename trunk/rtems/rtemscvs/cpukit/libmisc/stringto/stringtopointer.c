/*
 *  COPYRIGHT (c) 2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: stringtopointer.c,v 1.1 2009/07/23 14:32:34 joel Exp $
 */

/*
 *  Instantiate an error checking wrapper for strtoul which is
 *  used to input a (void *)
 *
 *  NOTE: This is only an appropriate implementation when unsigned long
 *        can represent a void *
 */
#define STRING_TO_POINTER
#define STRING_TO_TYPE void *
#define STRING_TO_INPUT_TYPE unsigned long
#define STRING_TO_NAME rtems_string_to_pointer
#define STRING_TO_METHOD strtoul
#define STRING_TO_MAX ULONG_MAX
#include "stringto_template.h"
