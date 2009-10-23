dnl $Id: enable-cxx.m4,v 1.7 2008/02/20 13:09:29 ralf Exp $

AC_DEFUN([RTEMS_ENABLE_CXX],
[
AC_ARG_ENABLE(cxx,
[AS_HELP_STRING([--enable-cxx],
[enable C++ support and build the rtems++ library])],
[case "${enable_cxx}" in
  yes) RTEMS_HAS_CPLUSPLUS=yes ;;
  no) RTEMS_HAS_CPLUSPLUS=no   ;;
  *)  AC_MSG_ERROR(bad value ${enableval} for enable-cxx option) ;;
esac], [RTEMS_HAS_CPLUSPLUS=no])
])
