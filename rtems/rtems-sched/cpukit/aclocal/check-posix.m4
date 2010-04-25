dnl $Id: check-posix.m4,v 1.5 2009/10/14 12:25:59 ralf Exp $
dnl
AC_DEFUN([RTEMS_CHECK_POSIX_API],
[dnl
AC_REQUIRE([RTEMS_CANONICAL_TARGET_CPU])dnl
AC_REQUIRE([RTEMS_ENABLE_POSIX])dnl

AC_CACHE_CHECK([whether CPU supports libposix],
  rtems_cv_HAS_POSIX_API,
  [dnl
    case "$RTEMS_CPU" in
    *)
      if test "${RTEMS_HAS_POSIX_API}" = "yes"; then
        rtems_cv_HAS_POSIX_API="yes";
      else
        rtems_cv_HAS_POSIX_API="disabled";
      fi
      ;;
    esac])
])
