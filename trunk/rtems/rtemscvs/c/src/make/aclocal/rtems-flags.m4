## $Id: rtems-flags.m4,v 1.1 2005/01/05 17:57:05 ralf Exp $
## 
## Some hacks to set up RTEMS_*FLAGS
## Internal macro, not supposed to be explictly used in configure.ac's

AC_DEFUN([_RTEMS_FLAGS],[
AS_IF([test -n "[$]{$1}"],
  [RTEMS_$1=[$]{$1}],
  [RTEMS_$1=$2])
AC_SUBST([RTEMS_$1])
])
