## Process this file with autoconf to produce a configure script.
##
## $Id: configure.ac,v 1.27 2009/11/28 06:28:39 ralf Exp $

AC_PREREQ(2.60)
AC_INIT([rtems-c-src-lib-libbsp-i386-ts_386ex],[_RTEMS_VERSION],[http://www.rtems.org/bugzilla])
AC_CONFIG_SRCDIR([bsp_specs])
RTEMS_TOP(../../../../../..)

RTEMS_CANONICAL_TARGET_CPU
AM_INIT_AUTOMAKE([no-define nostdinc foreign 1.10])
RTEMS_BSP_CONFIGURE

RTEMS_PROG_CC_FOR_TARGET
RTEMS_CANONICALIZE_TOOLS
RTEMS_PROG_CCAS

RTEMS_CHECK_NETWORKING

## if this is an i386, does gas have good code16 support?
RTEMS_I386_GAS_CODE16
AM_CONDITIONAL(RTEMS_GAS_CODE16,test "$RTEMS_GAS_CODE16" = "yes")

AM_CONDITIONAL(HAS_NETWORKING,test "$HAS_NETWORKING" = "yes")

RTEMS_CONFIG_BUILD_SUBDIRS(tools)

# bsp-specific options
RTEMS_BSPOPTS_SET([LOAD_RTC_AT_START],[*],[1])
RTEMS_BSPOPTS_HELP([LOAD_RTC_AT_START],
[Initialize RTC and load into system clock at startup. This will break
some tests.])

RTEMS_BSPOPTS_SET([BSP_DEBUG],[*],[])
RTEMS_BSPOPTS_HELP([BSP_DEBUG],
[Enables some debugging in the BSP package.])

RTEMS_BSPOPTS_SET([UARTDEBUG],[*],[])
RTEMS_BSPOPTS_HELP([UARTDEBUG],
[Enables some debugging in the BSP's uart.])

RTEMS_BSP_CLEANUP_OPTIONS(0, 0)

# Explicitly list all Makefiles here
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
