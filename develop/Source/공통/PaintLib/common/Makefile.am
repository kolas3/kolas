
SUBDIRS= Filter . test

ALL_H = planybmp.h pldatasrc.h pljpegdec.h plpgm.h plpngdec.h tif_msrc.h \
        planydec.h pldebug.h pljpegenc.h plpgmdec.h plpngenc.h pltiffdec.h \
        plbitmap.h plexcept.h plmemsink.h plpicdec.h plprognot.h plbmpdec.h \
        plfilesink.h plmemsrc.h plpicenc.h qdraw.h pltiffenc.h plbmpenc.h \
        plfilesrc.h ploptable.h plpictdec.h pltiffencex.h jmemdest.h plpcx.h \
        plobject.h pltga.h plwindefs.h pldatasink.h jmemsrc.h plpcxdec.h plpoint.h \
        pltgadec.h plpaintlibdefs.h plpixeldefs.h plgifdec.h plpixel32.h \
        plstdpch.h plrect.h config.h plpixel24.h plpixel8.h plppmdec.h plppm.h \
        plpsddec.h plbmpinfo.h

# Paintlib common code static library
COMMON = planybmp.cpp \
pldatasrc.cpp \
plpicenc.cpp \
plpictdec.cpp \
pltgadec.cpp \
planydec.cpp \
plexcept.cpp \
plmemsink.cpp \
plbitmap.cpp \
plfilesink.cpp \
plmemsrc.cpp \
plobject.cpp \
plbmpdec.cpp \
plfilesrc.cpp \
plpcxdec.cpp \
plbmpenc.cpp \
plpgmdec.cpp \
pldatasink.cpp \
plpicdec.cpp \
plstdpch.cpp \
pltrace.cpp \
plppmdec.cpp \
plpsddec.cpp \
plbmpinfo.cpp


# libpng and libz are currently installed in the system so we add
# support for PNG
if SYS_LIBPNG
SYS_PNG = plpngdec.cpp \
plpngenc.cpp
else
SYS_PNG=
endif

# libtiff is currently installed in the system so we add support for tiff
if SYS_LIBTIFF
SYS_TIFF=pltiffenc.cpp \
pltiffdec.cpp \
pltiffencex.cpp \
tif_msrc.c
else
SYS_TIFF=
endif

if SYS_LIBJPEG
SYS_JPEG = pljpegenc.cpp \
jmemdest.cpp \
pljpegdec.cpp \
jmemsrc.cpp
else
SYS_JPEG=
endif

if SYS_LIBUNGIF
SYS_UNGIF = plgifdec.cpp
else
SYS_UNGIF=
endif

INCLUDES = -I$(top_srcdir)/common/Filter -I$(top_srcdir)/gnu/libtiff

# Libraries to generate
noinst_LTLIBRARIES     = libcommon.la
# -no-undefinef needed for libtool for contruct dlls correctly with windows
# using cygwin tools.
libcommon_la_LDGFLAGS= -no-undefined
libcommon_la_SOURCES= $(COMMON) $(SYS_PNG) $(SYS_TIFF) $(SYS_JPEG) $(SYS_UNGIF) $(ALL_H)

paintlibdir = $(includedir)/paintlib
paintlib_HEADERS = $(ALL_H)
