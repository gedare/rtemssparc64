## Process this file with autoconf to produce a configure script.
##
## $Id: configure.ac,v 1.2 2009/11/28 06:28:40 ralf Exp $

AC_PREREQ(2.60)
AC_INIT([rtems-c-src-lib-libbsp-lm32],[_RTEMS_VERSION],[http://www.rtems.org/bugzilla])
AC_CONFIG_SRCDIR([lm32_evr])
RTEMS_TOP(../../../../..)

RTEMS_CANONICAL_TARGET_CPU
AM_INIT_AUTOMAKE([no-define foreign 1.10])
AM_MAINTAINER_MODE

RTEMS_ENV_RTEMSBSP
RTEMS_PROJECT_ROOT

RTEMS_CHECK_BSPDIR([$RTEMS_BSP_FAMILY])

# Explicitly list all Makefiles here
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
