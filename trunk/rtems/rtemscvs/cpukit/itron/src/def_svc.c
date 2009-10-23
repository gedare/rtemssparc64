/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: def_svc.c,v 1.1 2007/04/17 20:32:12 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/itron.h>

#include <rtems/itron/sysmgmt.h>

/*
 *  def_svc - Define Extended SVC Handler
 */

ER def_svc(
  FN      s_fncd,
  T_DSVC *pk_dsvc
)
{
  return E_OK;
}
