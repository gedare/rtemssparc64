/*
 *  The license and distribution terms for this file may be
 *  found in found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 * $Id: byteorder.h,v 1.1 2005/11/04 17:50:34 ralf Exp $
 */

#ifndef _LIBCPU_BYTEORDER_H
#define _LIBCPU_BYTEORDER_H

static inline void st_le32(volatile uint32_t   *addr, uint32_t   value)
{
  *(addr)=value ;
}

static inline uint32_t   ld_le32(volatile uint32_t   *addr)
{
  return(*addr);
}

#endif
