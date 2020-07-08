
MAINTAINERCLEANFILES = Makefile.in 
EXTRADIST = paintlib_version.h.in

#
# Build the libpaintlib library
#
lib_LTLIBRARIES = libpaintlib.la
libpaintlib_la_SOURCES = version.cpp

# -no-undefinef needed for libtool for contruct dlls correctly con windows
# using cygwin tools.
libpaintlib_la_LDGFLAGS = -no-undefined 
libpaintlib_la_LIBADD  =  ../common/libcommon.la ../common/Filter/libfilter.la 
