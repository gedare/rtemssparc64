dnl $Id: enable-shttpd.m4,v 1.1 2010/04/29 05:12:49 ralf Exp $

AC_DEFUN([RTEMS_ENABLE_SHTTPD],
[
AC_ARG_ENABLE([shttpd],
AS_HELP_STRING([--enable-shttpd],[enable (small httpd) shttpd (DEPRECATED)]),
[case "${enableval}" in 
  yes) enable_shttpd=yes ;;
  no) enable_shttpd=no ;;
  *)  AC_MSG_ERROR(bad value ${enableval} for enable-shttpd option) ;;
esac],[enable_shttpd=no]) 
])
