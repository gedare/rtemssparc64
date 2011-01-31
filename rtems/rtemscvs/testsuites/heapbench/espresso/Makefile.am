##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = espresso
espresso_SOURCES = FileSystemImage.c system.h init.c ansi.h cofactor.c cols.c compl.c contain.c copyright.h cubestr.c cvrin.c cvrm.c cvrmisc.c cvrout.c dlmalloc.c dominate.c equiv.c espresso.c espresso.h essen.c exact.c expand.c gasp.c gimpel.c globals.c hack.c indep.c irred.c main.c main.h map.c matrix.c mincov.c mincov.h mincov_int.h opo.c pair.c part.c port.h primes.c reduce.c regex.h rows.c set.c setc.c sharp.c sminterf.c solution.c sparse.c sparse.h sparse_int.h stdlib.h unate.c utility.c utility.h verify.c 

#dist_rtems_tests_DATA = espresso.scn
#dist_rtems_tests_DATA += espresso.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

espresso_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
espresso_LDLIBS = -lm

LINK_OBJS = $(espresso_OBJECTS) $(espresso_LDADD)
LINK_LIBS = $(espresso_LDLIBS)

espresso$(EXEEXT): $(espresso_OBJECTS) $(espresso_DEPENDENCIES)
	@rm -f espresso$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
