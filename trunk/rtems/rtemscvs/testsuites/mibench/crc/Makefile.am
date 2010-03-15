##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = crc
crc_SOURCES = init.c crc_32.c FileSystemImage.c

#dist_rtems_tests_DATA = crc.scn
#dist_rtems_tests_DATA += crc.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

crc_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(crc_OBJECTS) $(crc_LDADD)
LINK_LIBS = $(crc_LDLIBS)

crc$(EXEEXT): $(crc_OBJECTS) $(crc_DEPENDENCIES)
	@rm -f crc$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
