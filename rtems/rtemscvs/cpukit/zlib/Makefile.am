#
# $Id: Makefile.am,v 1.4 2010/03/22 18:42:49 joel Exp $
#

include $(top_srcdir)/automake/compile.am

project_lib_LIBRARIES = libz.a

libz_a_SOURCES = adler32.c
libz_a_SOURCES += compress.c
libz_a_SOURCES += crc32.c crc32.h
libz_a_SOURCES += deflate.c deflate.h
libz_a_SOURCES += gzclose.c gzguts.h gzlib.c gzread.c gzwrite.c
libz_a_SOURCES += infback.c
libz_a_SOURCES += inffast.c inffast.h
libz_a_SOURCES += inffixed.h
libz_a_SOURCES += inflate.c inflate.h
libz_a_SOURCES += inftrees.c inftrees.h
libz_a_SOURCES += trees.c trees.h
libz_a_SOURCES += uncompr.c
libz_a_SOURCES += zconf.h
libz_a_SOURCES += zlib.h
libz_a_SOURCES += zutil.c zutil.h

include_HEADERS = zlib.h zconf.h

include $(srcdir)/preinstall.am
include $(top_srcdir)/automake/local.am
