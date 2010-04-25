/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: h8bdinstallirq.c,v 1.1 2008/09/19 15:50:26 joel Exp $
 */

#include <rtems.h>

void H8BD_Install_IRQ(
  uint32_t  	vector,
  proc_ptr	new_handler,
  proc_ptr	*old_handler )
{ /* empty */
}
