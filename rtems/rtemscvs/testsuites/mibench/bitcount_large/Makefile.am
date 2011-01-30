##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = bitcount_large
bitcount_large_SOURCES = init.c bitarray.c bitcnt_1.c bitcnt_2.c bitcnt_3.c bitcnt_4.c bitcnts.c bitfiles.c bitstrng.c bstr_i.c bitops.h  conio.h  extkword.h  sniptype.h  system.h

#dist_rtems_tests_DATA = bitcount_large.scn
#dist_rtems_tests_DATA += bitcount_large.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

bitcount_large_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

LINK_OBJS = $(bitcount_large_OBJECTS) $(bitcount_large_LDADD)
LINK_LIBS = $(bitcount_large_LDLIBS)

bitcount_large$(EXEEXT): $(bitcount_large_OBJECTS) $(bitcount_large_DEPENDENCIES)
	@rm -f bitcount_large$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
