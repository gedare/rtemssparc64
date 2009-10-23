/**
 * @file rtems/rtems_bsdnet.h
 */
 
/*
 *  $Id: servers.h,v 1.1 2005/02/03 05:34:01 ralf Exp $
 */

#ifndef _RTEMS_BSDNET_SERVERS_H
#define _RTEMS_BSDNET_SERVERS_H

extern struct in_addr *rtems_bsdnet_ntpserver;
extern int rtems_bsdnet_ntpserver_count;

/*
 * Network configuration
 */
extern struct in_addr *rtems_bsdnet_nameserver;
extern int rtems_bsdnet_nameserver_count;

#endif /* _RTEMS_BSDNET_SERVERS_H */
