dnl $Id: enable-itron.m4,v 1.7 2010/04/29 07:45:01 ralf Exp $

AC_DEFUN([RTEMS_ENABLE_ITRON],
[
AC_ARG_ENABLE([itron],
AS_HELP_STRING([--enable-itron],[enable itron interface (DEPRECATED)]),
[case "${enableval}" in 
  yes) enable_itron=yes ;;
  no) enable_itron=no ;;
  *)  AC_MSG_ERROR(bad value ${enableval} for enable-itron option) ;;
esac],[enable_itron=no]) 
])
