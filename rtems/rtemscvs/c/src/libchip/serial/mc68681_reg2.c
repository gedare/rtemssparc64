/*
 *  This file contains a typical set of register access routines which may be
 *  used with the mc68681 chip if accesses to the chip are as follows:
 *
 *    + registers are accessed as bytes
 *    + registers are on 16-bit boundaries
 *
 *  COPYRIGHT (c) 1989-1997.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: mc68681_reg2.c,v 1.7 2004/04/20 10:43:39 ralf Exp $
 */

#define _MC68681_MULTIPLIER 2
#define _MC68681_NAME(_X) _X##_2
#define _MC68681_TYPE uint8_t

#include "mc68681_reg.c"
