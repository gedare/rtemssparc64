##
##  $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = cfrac
cfrac_SOURCES = init.c system.h asm16bit.h atop.c cfrac.c errorp.c getopt.c getopt.h itop.c ltop.c pabs.c padd.c pcfrac.c pcmp.c pconst.c pcvt.h pdefs.h pdivmod.c pfactor.h pfloat.c pgcd.c phalf.c picmp.c pidiv.c pimod.c pio.c pmul.c pneg.c podd.c pops.c ppowmod.c precision.h primes.c primes.h psqrt.c psub.c ptoa.c ptob.c ptou.c seive.h utop.c

#dist_rtems_tests_DATA = cfrac.scn
#dist_rtems_tests_DATA += cfrac.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

cfrac_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)
cfrac_LDLIBS = -lm

LINK_OBJS = $(cfrac_OBJECTS) $(cfrac_LDADD)
LINK_LIBS = $(cfrac_LDLIBS)

cfrac$(EXEEXT): $(cfrac_OBJECTS) $(cfrac_DEPENDENCIES)
	@rm -f cfrac$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
