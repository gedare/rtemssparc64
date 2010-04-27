--
--  Unsigned32_IO / Specification
--
--  DESCRIPTION:
--
--  This package instantiates the IO routines necessary to 
--  perform IO on data of the type RTEMS.Unsigned32.
--
--  DEPENDENCIES: 
--
--  
--
--  COPYRIGHT (c) 1989-1997.
--  On-Line Applications Research Corporation (OAR).
--
--  The license and distribution terms for this file may in
--  the file LICENSE in this distribution or at
--  http://www.rtems.com/license/LICENSE.
--
--  $Id: unsigned32_io.ads,v 1.4 2003/09/04 18:50:40 joel Exp $
--

with RTEMS;
with Text_IO;

package Unsigned32_IO is new Text_IO.Modular_IO( RTEMS.Unsigned32 );

