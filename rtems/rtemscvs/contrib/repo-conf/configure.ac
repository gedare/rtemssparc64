#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.64)
AC_INIT([rtems-]_RTEMS_API[-repo-conf],[0.23],[http://www.rtems.org/bugzilla],
  [rtems-]_RTEMS_API[-repo-conf])
RTEMS_VERSIONING

AC_CONFIG_SRCDIR([gpg])
AC_CONFIG_AUX_DIR([.])
AC_SUBST([RTEMS_API],[_RTEMS_API])
AM_INIT_AUTOMAKE([dist-bzip2 no-dist-gzip dist-xz 1.10])

AC_ARG_ENABLE(osname,AS_HELP_STRING(
[--enable-osname=<dir> ... os name]),
[osname="$enable_osname"],
[osname="fedora-11"])

case $osname in
fedora-10* | fedora-11* | fedora-12* )
  os_name="Fedora"
  os_subdir="fedora"
  os_vers=`echo $osname| sed -e 's,^.*-,,'`
  yum_verdir="\$\$releasever"
  apt_verdir="\$\$(VERSION)"
  ;;
centos-5* | redhat-el5* | fedora-7* | fedora-8* | fedora-9* )
  os_name="CentOS"
  os_subdir="centos"
  os_vers=`echo $osname| sed -e 's,^.*-,,'`
  yum_verdir="5"
  apt_verdir="5"
  ;;
centos-4* | redhat-el4* | fedora-5* | fedora-6* )
  os_name="CentOS"
  os_subdir="centos"
  os_vers=`echo $osname| sed -e 's,^.*-,,'`
  yum_verdir="4"
  apt_verdir="4"
  ;;
suse-*)
  os_name="openSUSE"
  os_subdir="suse"
  os_vers=`echo $osname| sed -e 's,^.*-,,'`
  yum_verdir="\$\$releasever"
  apt_verdir="\$\$(VERSION)"
  ;;
*)
  AC_MSG_ERROR([Unsupported OS])
esac

AC_SUBST([OS_NAME],[$os_name])
AC_SUBST([OS_SUBDIR],[$os_subdir])
AC_SUBST([OS_VERS],[$os_vers])
AC_SUBST([YUM_VERDIR],[$yum_verdir])
AC_SUBST([APT_VERDIR],[$apt_verdir])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
