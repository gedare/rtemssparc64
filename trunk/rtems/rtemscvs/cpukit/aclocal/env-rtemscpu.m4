dnl $Id: env-rtemscpu.m4,v 1.21 2005/01/05 16:07:20 ralf Exp $

AC_DEFUN([RTEMS_ENV_RTEMSCPU],
[
  AC_REQUIRE([RTEMS_ENABLE_MULTILIB])

  AM_CONDITIONAL([MULTILIB],[test x"$multilib" = x"yes"])
])
