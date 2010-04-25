dnl
dnl $Id: project-root.m4,v 1.3 2004/02/19 05:40:02 ralf Exp $
dnl 

AC_DEFUN([RTEMS_PROJECT_ROOT],
[dnl
AC_REQUIRE([RTEMS_TOP])

PACKHEX="\$(PROJECT_TOPdir)/tools/build/packhex"
AC_SUBST(PACKHEX)
])

