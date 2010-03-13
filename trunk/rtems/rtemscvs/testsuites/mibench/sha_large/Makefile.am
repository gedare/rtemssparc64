##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = sha_large
sha_large_SOURCES = init.c sha.c FileSystemImage.c

dist_rtems_tests_DATA = sha.scn
dist_rtems_tests_DATA += sha.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

sha_large_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(sha_large_OBJECTS) $(sha_large_LDADD)
LINK_LIBS = $(sha_large_LDLIBS)

sha_large$(EXEEXT): $(sha_large_OBJECTS) $(sha_large_DEPENDENCIES)
	@rm -f sha_large$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
