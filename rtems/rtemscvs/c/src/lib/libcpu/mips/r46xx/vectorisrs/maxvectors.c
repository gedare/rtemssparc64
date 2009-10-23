/* 
 *  This file contains the maximum number of vectors.  This can not
 *  be determined without knowing the RTEMS CPU model. 
 *
 *  COPYRIGHT (c) 1989-2000.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: maxvectors.c,v 1.3 2004/04/15 13:33:56 ralf Exp $
 */

/*
 *  Reserve first 32 for exceptions.
 */

unsigned int mips_interrupt_number_of_vectors = 32 + 8;
