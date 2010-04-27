dnl
dnl $Id: canonical-target-name.m4,v 1.11 2009/10/14 12:25:59 ralf Exp $
dnl

dnl canonicalize target cpu
dnl NOTE: Most rtems targets do not fullfil autoconf's
dnl target naming conventions "processor-vendor-os"
dnl Therefore autoconf's AC_CANONICAL_TARGET will fail for them
dnl and we have to fix it for rtems ourselves 

AC_DEFUN([RTEMS_CANONICAL_TARGET_CPU],
[AC_REQUIRE([AC_CANONICAL_HOST])
AC_MSG_CHECKING(rtems target cpu)
case "${host}" in
  no_cpu-*rtems*)
        RTEMS_CPU=no_cpu
	;;
  *) 
	RTEMS_CPU=`echo $host | sed 's%^\([[^-]]*\)-\(.*\)$%\1%'`
	;;
esac
AC_SUBST(RTEMS_CPU)
AC_MSG_RESULT($RTEMS_CPU)
])
