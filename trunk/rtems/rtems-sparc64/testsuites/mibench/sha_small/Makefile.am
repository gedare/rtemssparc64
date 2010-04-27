##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = sha_small
sha_small_SOURCES = init.c sha.c FileSystemImage.c

dist_rtems_tests_DATA = sha.scn
dist_rtems_tests_DATA += sha.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

sha_small_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(sha_small_OBJECTS) $(sha_small_LDADD)
LINK_LIBS = $(sha_small_LDLIBS)

sha_small$(EXEEXT): $(sha_small_OBJECTS) $(sha_small_DEPENDENCIES)
	@rm -f sha_small$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
