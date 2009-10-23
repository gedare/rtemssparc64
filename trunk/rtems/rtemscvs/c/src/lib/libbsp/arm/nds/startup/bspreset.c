/*
 * RTEMS for Nintendo DS platform initialization.
 * Copyright (c) 2008 by Matthieu Bucchianeri <mbucchia@gmail.com>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE
 *
 * $Id: bspreset.c,v 1.1 2008/09/22 21:53:35 joel Exp $
 */

#include <bsp.h>
#include <bsp/bootcard.h>
#include <nds.h>

void bsp_reset(void)
{
  swiSoftReset();
}
