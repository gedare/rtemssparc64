/*  address.h
 *
 *  This include file contains the information required to manipulate
 *  physical addresses.
 *
 *  COPYRIGHT (c) 1988-2002.
 *  On-Line Applications Research Corporation (OAR).
 *  All rights reserved.
 *
 *  $Id: address.h,v 1.7 2007/12/07 02:58:44 ralf Exp $
 */

#ifndef __ADDRESSES_h
#define __ADDRESSES_h

#include <stddef.h>

/*
 *  _Addresses_Add_offset
 *
 *  DESCRIPTION:
 *
 *  This function is used to add an offset to a base address.
 *  It returns the resulting address.  This address is typically
 *  converted to an access type before being used further.
 */

STATIC INLINE void *_Addresses_Add_offset ( 
  void       *base, 
  size_t      offset
);

#include "address.inl"

#endif
/* end of include file */
