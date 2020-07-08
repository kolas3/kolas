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

ALL_H = pltest.h pltester.h testfilterresizebilinear.h testpsddecoder.h \
        pltestdecoders.h pltestfilters.h

ALL_CPP = pltester.cpp testfilterresizebilinear.cpp testpsddecoder.cpp \
          pltest.cpp pltestdecoders.cpp pltestfilters.cpp 

# Libraries to generate
noinst_LTLIBRARIES = libfilter.la
# -no-undefinef needed for libtool for contruct dlls correctly with windows
# using cygwin tools.
libfilter_la_LDGFLAGS = -no-undefined
libfilter_la_SOURCES = $(ALL_CPP) $(ALL_H)

paintlibdir = $(includedir)/paintlib
testdir = $(paintlibdir)/test
test_HEADERS = $(ALL_H)


noinst_PROGRAMS=testpaintlib
testpaintlib_SOURCES=testpaintlib.cpp
testpaintlib_LDFLAGS=$(SYS_LDFLAGS_LIBPNG) $(SYS_LDFLAGS_LIBJPEG) $(SYS_LDFLAGS_LIBTIFF) $(SYS_LDFLAGS_LIBUNGIF)
testpaintlib_LDADD=$(TEST_OBJ_DIR)pltester.o $(TEST_OBJ_DIR)testfilterresizebilinear.o \
      $(TEST_OBJ_DIR)testpsddecoder.o $(TEST_OBJ_DIR)pltest.o \
      $(TEST_OBJ_DIR)pltestdecoders.o $(TEST_OBJ_DIR)pltestfilters.o \
      ../libcommon.la ../Filter/libfilter.la $(testpaintlib_LDFLAGS)
