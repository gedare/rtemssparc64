##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = bitcount_small
bitcount_small_SOURCES = init.c bitarray.c bitcnt_1.c bitcnt_2.c bitcnt_3.c bitcnt_4.c bitcnts.c bitfiles.c bitstrng.c bstr_i.c bitops.h  conio.h  extkword.h  sniptype.h  system.h

#dist_rtems_tests_DATA = bitcount_small.scn
#dist_rtems_tests_DATA += bitcount_small.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

bitcount_small_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(bitcount_small_OBJECTS) $(bitcount_small_LDADD)
LINK_LIBS = $(bitcount_small_LDLIBS)

bitcount_small$(EXEEXT): $(bitcount_small_OBJECTS) $(bitcount_small_DEPENDENCIES)
	@rm -f bitcount_small$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
