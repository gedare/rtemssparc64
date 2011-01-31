##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = rijndael_small
rijndael_small_SOURCES = FileSystemImage.c aes.c aes.h aestab.h aesxam.c  system.h init.c 

#dist_rtems_tests_DATA = rijndael_small.scn
#dist_rtems_tests_DATA += rijndael_small.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

rijndael_small_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(rijndael_small_OBJECTS) $(rijndael_small_LDADD)
LINK_LIBS = $(rijndael_small_LDLIBS)

rijndael_small$(EXEEXT): $(rijndael_small_OBJECTS) $(rijndael_small_DEPENDENCIES)
	@rm -f rijndael_small$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
