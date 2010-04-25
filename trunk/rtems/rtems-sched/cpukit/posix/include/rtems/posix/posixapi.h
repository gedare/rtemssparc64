/**
 * @file rtems/posix/posixapi.h
 */

/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: posixapi.h,v 1.10 2008/12/15 19:21:01 joel Exp $
 */

#ifndef _RTEMS_POSIX_POSIXAPI_H
#define _RTEMS_POSIX_POSIXAPI_H

#include <rtems/config.h>

/**
 *  @brief Initialize POSIX API
 *
 *  This method is responsible for initializing each of the POSIX
 *  API managers.
 */
void _POSIX_API_Initialize(void);

#endif
/* end of include file */
