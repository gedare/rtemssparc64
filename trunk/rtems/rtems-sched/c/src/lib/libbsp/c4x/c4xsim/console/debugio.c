/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: debugio.c,v 1.5 2008/09/19 15:50:44 joel Exp $
 */

#include <bsp.h>
#include <rtems/bspIo.h>

extern void C4X_BSP_output_char(char c);

BSP_output_char_function_type 		BSP_output_char = C4X_BSP_output_char;
BSP_polling_getchar_function_type 	BSP_poll_char;
