dnl $Id: enable-rtemsbsp.m4,v 1.3 2008/02/20 13:07:42 ralf Exp $

dnl Override the set of BSPs to be built.
dnl used by the toplevel configure script
dnl RTEMS_ENABLE_RTEMSBSP(rtems_bsp_list)
AC_DEFUN([RTEMS_ENABLE_RTEMSBSP],
[
AC_BEFORE([$0], [RTEMS_ENV_RTEMSBSP])
AC_ARG_ENABLE(rtemsbsp,
[AS_HELP_STRING([--enable-rtemsbsp="bsp1 bsp2 .."],
[BSPs to include in build])],
[case "${enableval}" in
  yes|no) AC_MSG_ERROR([missing argument to --enable-rtemsbsp="bsp1 bsp2"]);;
  *) $1=$enableval;;
esac],[$1=""])
])
