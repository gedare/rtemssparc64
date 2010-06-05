# RTEMS_CHECK_BSPDIR(RTEMS_BSP_FAMILY)
AC_DEFUN([RTEMS_CHECK_BSPDIR],
[
  case "$1" in
  m5sim )
    AC_CONFIG_SUBDIRS([m5sim]);;
  niagara )
    AC_CONFIG_SUBDIRS([niagara]);;
  usiii )
    AC_CONFIG_SUBDIRS([usiii]);;
  *)
    AC_MSG_ERROR([Invalid BSP]);;
  esac
])
