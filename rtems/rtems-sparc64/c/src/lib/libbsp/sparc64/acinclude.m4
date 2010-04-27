# RTEMS_CHECK_BSPDIR(RTEMS_BSP_FAMILY)
AC_DEFUN([RTEMS_CHECK_BSPDIR],
[
  case "$1" in
  sun4v )
    AC_CONFIG_SUBDIRS([sun4v]);;
  *)
    AC_MSG_ERROR([Invalid BSP]);;
  esac
])
