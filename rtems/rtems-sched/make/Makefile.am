##
## $Id: Makefile.am,v 1.39 2009/10/16 14:28:46 ralf Exp $
##

rtems_makedir = $(prefix)/make

dist_rtems_make_DATA = main.cfg leaf.cfg

EXTRA_DIST = host.cfg.in

rtems_make_Templatesdir = $(rtems_makedir)/Templates

dist_rtems_make_Templates_DATA = Templates/Makefile.dir \
    Templates/Makefile.leaf Templates/Makefile.lib

EXTRA_DIST += compilers/gcc-no_bsp.cfg \
    compilers/gcc-target-default.cfg

rtems_make_customdir = $(rtems_makedir)/custom

dist_rtems_make_custom_DATA = custom/default.cfg

include $(top_srcdir)/automake/host.am
