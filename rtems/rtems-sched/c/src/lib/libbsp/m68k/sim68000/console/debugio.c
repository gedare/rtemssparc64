/*
 *  This routine starts the application.  It includes application,
 *  board, and monitor specific initialization and configuration.
 *  The generic CPU dependent initialization has been performed
 *  before this routine is invoked.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: debugio.c,v 1.1 2008/09/16 19:03:46 joel Exp $
 */

#include <bsp.h>
#include <rtems/bspIo.h>

void debug_putc( char c ) { write( 2, &c, 1 ); }
BSP_output_char_function_type BSP_output_char =  debug_putc;
BSP_polling_getchar_function_type BSP_poll_char = NULL;
