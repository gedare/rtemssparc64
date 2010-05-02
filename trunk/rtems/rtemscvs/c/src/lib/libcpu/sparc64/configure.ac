## Process this file with autoconf to produce a configure script.
## 
## $Id$

AC_PREREQ(2.60)
AC_INIT([rtems-c-src-lib-libcpu-sparc64],[_RTEMS_VERSION],[http://www.rtems.org/bugzilla])
AC_CONFIG_SRCDIR([sun4v])
RTEMS_TOP([../../../../..],[../../..])

RTEMS_CANONICAL_TARGET_CPU

AM_INIT_AUTOMAKE([no-define foreign subdir-objects 1.10])
AM_MAINTAINER_MODE

RTEMS_ENV_RTEMSBSP

RTEMS_PROJECT_ROOT

RTEMS_PROG_CC_FOR_TARGET
AM_PROG_CC_C_O
RTEMS_CANONICALIZE_TOOLS
RTEMS_PROG_CCAS

AM_CONDITIONAL(shared, test x"$RTEMS_CPU_MODEL" = x"sun4v" || \
                       test x"$RTEMS_CPU_MODEL" = x"m5sim") 

AM_CONDITIONAL([sun4v],[test x"$RTEMS_CPU_MODEL" = x"sun4v"])
AM_CONDITIONAL([m5sim],[test x"$RTEMS_CPU_MODEL" = x"m5sim"])


RTEMS_AMPOLISH3

# Explicitly list all Makefiles here
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

