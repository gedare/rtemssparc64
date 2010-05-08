/*
 *  COPYRIGHT (c) 2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: stringtofloat.c,v 1.2 2010/03/28 15:20:32 ralf Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 *  Instantiate an error checking wrapper for strtof (float)
 */
#define STRING_TO_FLOAT
#define STRING_TO_TYPE float
#define STRING_TO_NAME rtems_string_to_float
#define STRING_TO_METHOD strtof
#define STRING_TO_MAX HUGE_VALF
#include "stringto_template.h"
