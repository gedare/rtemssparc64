ACLOCAL_AMFLAGS = -I aclocal
CLEANFILES =

aptconfdir = $(sysconfdir)

nobase_aptconf_DATA =

nobase_aptconf_DATA += apt/sources.list.d/rtems-@RTEMS_API@.list
nobase_aptconf_DATA += apt/vendors.list.d/rtems.list
apt/sources.list.d/rtems-@RTEMS_API@.list: apt/sources.list.d/rtems.list.in
	@$(mkdir_p) apt/sources.list.d
	sed -e 's,[@]RTEMS_API[@],@RTEMS_API@,' \
	  -e 's,[@]OS_SUBDIR[@],@OS_SUBDIR@,' \
	  -e 's,[@]OS_NAME[@],@OS_NAME@,' \
	  -e 's,[@]APT_VERDIR[@],@APT_VERDIR@,' \
	  < $(srcdir)/apt/sources.list.d/rtems.list.in \
	  > apt/sources.list.d/rtems-@RTEMS_API@.list
CLEANFILES += apt/sources.list.d/rtems-@RTEMS_API@.list
EXTRA_DIST = $(srcdir)/apt/sources.list.d/*.list.in
EXTRA_DIST += $(srcdir)/apt/vendors.list.d/*.list

yumconfdir = $(sysconfdir)
nobase_yumconf_DATA =

nobase_yumconf_DATA += yum.repos.d/rtems-@RTEMS_API@.repo
yum.repos.d/rtems-@RTEMS_API@.repo: yum.repos.d/rtems.repo.in
	@$(mkdir_p) yum.repos.d
	sed -e 's,[@]sysconfdir[@],$(sysconfdir),' \
	  -e 's,[@]RTEMS_API[@],@RTEMS_API@,' \
	  -e 's,[@]OS_SUBDIR[@],@OS_SUBDIR@,' \
	  -e 's,[@]OS_NAME[@],@OS_NAME@,' \
	  -e 's,[@]OS_VERS[@],@OS_VERS@,' \
	  -e 's,[@]YUM_VERDIR[@],@YUM_VERDIR@,' \
	  < $(srcdir)/yum.repos.d/rtems.repo.in \
	  > yum.repos.d/rtems-@RTEMS_API@.repo
CLEANFILES += yum.repos.d/rtems-@RTEMS_API@.repo
EXTRA_DIST += $(srcdir)/yum.repos.d/*repo.in

gpg/gpg-pubkey-69ce4a83-44cc2b30-@OS_SUBDIR@-@OS_VERS@: gpg/gpg-pubkey-69ce4a83-44cc2b30
	@$(mkdir_p) gpg
	cat $(srcdir)/gpg/gpg-pubkey-69ce4a83-44cc2b30 \
	  > gpg/gpg-pubkey-69ce4a83-44cc2b30-@OS_SUBDIR@-@OS_VERS@
CLEANFILES += gpg/gpg-pubkey-69ce4a83-44cc2b30-@OS_SUBDIR@-@OS_VERS@
EXTRA_DIST += $(srcdir)/gpg/gpg-pubkey-69ce4a83-44cc2b30


gpg/gpg-pubkey-eac29b6f-3fe1f458-@OS_SUBDIR@-@OS_VERS@: gpg/gpg-pubkey-eac29b6f-3fe1f458
	@$(mkdir_p) gpg
	cat $(srcdir)/gpg/gpg-pubkey-eac29b6f-3fe1f458 \
	  > gpg/gpg-pubkey-eac29b6f-3fe1f458-@OS_SUBDIR@-@OS_VERS@
CLEANFILES += gpg/gpg-pubkey-eac29b6f-3fe1f458-@OS_SUBDIR@-@OS_VERS@
EXTRA_DIST += $(srcdir)/gpg/gpg-pubkey-eac29b6f-3fe1f458

yum_gpgdir = $(sysconfdir)/pki/rpm-gpg
yum_gpg_DATA =
yum_gpg_DATA += gpg/gpg-pubkey-69ce4a83-44cc2b30-@OS_SUBDIR@-@OS_VERS@
yum_gpg_DATA += gpg/gpg-pubkey-eac29b6f-3fe1f458-@OS_SUBDIR@-@OS_VERS@

apt_gpgdir = $(sysconfdir)/apt/gpg
apt_gpg_DATA =
apt_gpg_DATA += gpg/gpg-pubkey-69ce4a83-44cc2b30-@OS_SUBDIR@-@OS_VERS@
apt_gpg_DATA += gpg/gpg-pubkey-eac29b6f-3fe1f458-@OS_SUBDIR@-@OS_VERS@

noinst_HEADERS = rtems-@RTEMS_API@-repo-conf.spec
rtems-@RTEMS_API@-repo-conf.spec: rtems-repo-conf.spec.in config.status
	./config.status --file=rtems-@RTEMS_API@-repo-conf.spec:rtems-repo-conf.spec.in
EXTRA_DIST += rtems-repo-conf.spec.in
