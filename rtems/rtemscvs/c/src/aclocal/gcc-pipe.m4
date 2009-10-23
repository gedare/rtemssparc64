dnl
dnl $Id: gcc-pipe.m4,v 1.2 2003/11/26 06:18:03 ralf Exp $
dnl
dnl Check whether the target compiler accepts -pipe
dnl

AC_DEFUN([RTEMS_GCC_PIPE],
[AC_REQUIRE([RTEMS_PROG_CC]) 
AC_REQUIRE([AC_CANONICAL_HOST])
AC_CACHE_CHECK(whether $CC accepts --pipe,rtems_cv_gcc_pipe,
[
rtems_cv_gcc_pipe=no
if test x"$GCC" = x"yes"; then
    echo 'void f(){}' >conftest.c
    if test -z "`${CC} --pipe -c conftest.c 2>&1`";then
      rtems_cv_gcc_pipe=yes
    fi
    rm -f conftest*
fi
])
])
