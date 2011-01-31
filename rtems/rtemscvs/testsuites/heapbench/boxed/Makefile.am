##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = boxed
boxed_SOURCES = init.c system.h bounds.c boxed.c boxed.h checkpoint.c collide.c com_inertia.c epoch.c event_heap.c hash.c hash.h heap.c heap.h integrate.c lu_decomp.c overlaps.c pairwise.c polyhedron.c quaternion.c quaternion.h recipes.h red_black.c red_black.h simulator.c simulator.h stdstuff.h vclip.c vclip.h Vector4.h Vector.h

#dist_rtems_tests_DATA = boxed.scn
#dist_rtems_tests_DATA += boxed.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

boxed_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
boxed_LDLIBS = -lm

LINK_OBJS = $(boxed_OBJECTS) $(boxed_LDADD)
LINK_LIBS = $(boxed_LDLIBS)

boxed$(EXEEXT): $(boxed_OBJECTS) $(boxed_DEPENDENCIES)
	@rm -f boxed$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
