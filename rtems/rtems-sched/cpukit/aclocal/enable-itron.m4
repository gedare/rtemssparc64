dnl $Id: enable-itron.m4,v 1.5 2009/07/23 21:22:25 ralf Exp $

AC_DEFUN([RTEMS_ENABLE_ITRON],
[
AC_ARG_ENABLE(itron,
AS_HELP_STRING(--enable-itron,enable itron interface),
[case "${enableval}" in 
  yes) RTEMS_HAS_ITRON_API=yes ;;
  no) RTEMS_HAS_ITRON_API=no ;;
  *)  AC_MSG_ERROR(bad value ${enableval} for enable-itron option) ;;
esac],[RTEMS_HAS_ITRON_API=no]) 
])
