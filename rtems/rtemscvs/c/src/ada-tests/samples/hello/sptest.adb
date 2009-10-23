--
--  SPTEST / BODY
--
--  DESCRIPTION:
--
--  This package is the implementation of Test 1 of the RTEMS
--  Single Processor Test Suite.
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
--  $Id: sptest.adb,v 1.5 2005/05/03 21:41:44 joel Exp $
--

with INTERFACES; use INTERFACES;
with RTEMS;
with TEST_SUPPORT;
with TEXT_IO;

package body SPTEST is

--PAGE
-- 
--  INIT
--

   procedure INIT (
      ARGUMENT : in     RTEMS.TASK_ARGUMENT
   ) is
   begin

      TEXT_IO.NEW_LINE( 2 );
      TEXT_IO.PUT_LINE( "*** HELLO WORLD TEST ***" );
      TEXT_IO.PUT_LINE( "Hello World" );
      TEXT_IO.PUT_LINE( "*** END OF HELLO WORLD TEST ***" );

      RTEMS.SHUTDOWN_EXECUTIVE( 0 );

   end INIT;

end SPTEST;
