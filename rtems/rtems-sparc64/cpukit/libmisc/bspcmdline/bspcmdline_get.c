/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspcmdline_get.c,v 1.3 2010/03/27 05:01:30 ralf Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/bspcmdline.h>

extern const char *bsp_boot_cmdline;

const char *rtems_bsp_cmdline_get(void)
{
  return bsp_boot_cmdline;
}
