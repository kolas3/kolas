
MAINTAINERCLEANFILES=Makefile.in

#
# There is PNG Support from the library ?
#
if SYS_LIBPNG
SYS_LDFLAGS_LIBPNG=-lz -lpng
else
SYS_LDFLAGS_LIBPNG=
endif

#
# There is Tiff Support from the library ?
#
if SYS_LIBTIFF
SYS_LDFLAGS_LIBTIFF=-ltiff
else
SYS_LDFLAGS_LIBTIFF=
endif

#
# There is JPEG Support from the library ?
#
if SYS_LIBJPEG
SYS_LDFLAGS_LIBJPEG=-ljpeg
else
SYS_LDFLAGS_LIBJPEG=
endif

#
# There is GIF Support from the library ?
#
if SYS_LIBUNGIF
SYS_LDFLAGS_LIBUNGIF=-lungif -lX11
else
SYS_LDFLAGS_LIBUNGIF=
endif

INCLUDES=-I$(top_srcdir)/common/ -I$(top_srcdir)/common/Filter

noinst_PROGRAMS=im2bmp
im2bmp_SOURCES=im2bmp.cpp im2bmp.h
im2bmp_LDFLAGS=$(SYS_LDFLAGS_LIBPNG) $(SYS_LDFLAGS_LIBJPEG) $(SYS_LDFLAGS_LIBTIFF) $(SYS_LDFLAGS_LIBUNGIF)
im2bmp_LDADD=../../lib/libpaintlib.la $(im2bmp_LDFLAGS)
