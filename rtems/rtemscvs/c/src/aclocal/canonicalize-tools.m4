dnl
dnl $Id: canonicalize-tools.m4,v 1.5 2004/03/09 09:53:08 ralf Exp $
dnl
dnl Set target tools
dnl

AC_DEFUN([RTEMS_CANONICALIZE_TOOLS],
[AC_REQUIRE([RTEMS_PROG_CC])dnl

dnl FIXME: What shall be done if these tools are not available?
  RTEMS_CHECK_TOOL(AR,ar,no)
  RTEMS_CHECK_TOOL(AS,as,no)
  RTEMS_CHECK_TOOL(LD,ld,no)

dnl special treatment of ranlib
  RTEMS_CHECK_TOOL(RANLIB,ranlib,:)
])
