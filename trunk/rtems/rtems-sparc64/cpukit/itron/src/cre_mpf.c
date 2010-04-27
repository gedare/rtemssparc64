/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: cre_mpf.c,v 1.2 2009/01/01 15:13:07 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/itron.h>

#include <rtems/itron/fmempool.h>

/*
 *  cre_mpf - Create Fixed-Size Memorypool
 */

ER cre_mpf(
  ID      mpfid __attribute__((unused)),
  T_CMPF *pk_cmpf __attribute__((unused))
)
{
  return E_OK;
}
