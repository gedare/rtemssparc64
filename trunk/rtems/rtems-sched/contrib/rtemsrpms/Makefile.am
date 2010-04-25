##
## $Id: Makefile.am,v 1.2 2006/08/18 11:28:17 ccj Exp $
##

TARGETS=@TARGETS@
RTEMS_API=@RTEMS_API@
RPMDBPATH=@RPMDBPATH@

all:
	export PATH=$(prefix)/bin:$$PATH; \
	for t in $(TARGETS) ; \
	 do \
	  rpmbuild -v $(RPMDBPATH) -ba $(srcdir)/rtems.spec --target=$$t-rtems$(RTEMS_API) ; \
          if [ $$? -ne 0 ]; then \
           exit 1 ; \
          fi \
	 done
