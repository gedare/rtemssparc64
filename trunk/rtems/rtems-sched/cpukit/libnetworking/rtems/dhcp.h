/*
  ------------------------------------------------------------------------
  $Id: dhcp.h,v 1.3 2009/11/30 15:57:50 ralf Exp $
  ------------------------------------------------------------------------

  Copyright Cybertec Pty Ltd, 2005
  All rights reserved Cybertec Pty Ltd, 2005

  This software with is provided ``as is'' and with NO WARRANTY.

  ------------------------------------------------------------------------
 */

/**
  @file

  DHCP Server interface.
*/

#if !defined (__RTEMS_DHCP_H__)
#define __RTEMS_DHCP_H__

#if __cplusplus
extern "C"
{
#endif

/*
 * Perform DHCP.
 */
void rtems_bsdnet_do_dhcp (void);
int rtems_bsdnet_do_dhcp_timeout (void);
void rtems_bsdnet_dhcp_down (void);

/*
 * Maintain a DHCP offer that has already been accepted.
 */
void rtems_bsdnet_do_dhcp_refresh_only (unsigned long xid,
                                        unsigned long lease_time,
                                        unsigned long elapsed_time,
                                        unsigned long ip_address,
                                        unsigned long srv_address,
                                        const char    *hostname);

#if __cplusplus
}
#endif

#endif
