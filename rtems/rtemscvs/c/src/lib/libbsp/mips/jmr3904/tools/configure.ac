## Process this file with autoconf to produce a configure script.
##
## $Id: configure.ac,v 1.14 2009/11/28 06:28:40 ralf Exp $

AC_PREREQ(2.60)
AC_INIT([rtems-c-src-lib-libbsp-mips-jmr3904-tools],[_RTEMS_VERSION],[http://www.rtems.org/bugzilla])
AC_CONFIG_SRCDIR([runtest.in])
RTEMS_TOP(../../../../../../..)

RTEMS_CANONICAL_TARGET_CPU

AM_INIT_AUTOMAKE([no-define foreign 1.10])
AM_MAINTAINER_MODE

RTEMS_ENV_RTEMSBSP

RTEMS_PATH_KSH

RTEMS_PROJECT_ROOT
RTEMS_TOOLPATHS

# Explicitly list all Makefiles here
AC_CONFIG_FILES([Makefile
runtest])
AC_OUTPUT
