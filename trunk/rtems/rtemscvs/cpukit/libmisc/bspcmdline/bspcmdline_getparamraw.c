/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspcmdline_getparamraw.c,v 1.3 2009/11/29 12:12:38 ralf Exp $
 */

#include <rtems/bspcmdline.h>

#include <stdio.h>

extern const char *bsp_boot_cmdline;

const char *rtems_bsp_cmdline_get_param_raw(
  const char *name
)
{
  const char *p;

  if ( !name )
    return NULL;

  if ( !bsp_boot_cmdline )
    return NULL;

  p = strstr(bsp_boot_cmdline, name);
  /* printf( "raw: %p (%s)\n", p, p ); */
  return p;
}
