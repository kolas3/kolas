
INCLUDES=-I$(top_srcdir)/common

ALL_H = plcontribdefs.h plfiltergetalpha.h plfilterresizebilinear.h pl2passscale.h plfiltergrayscale.h plfilterresizebox.h plfilterthreshold.h plfilter.h plfilterquantize.h plfilterresizegaussian.h plfiltervideoinvert.h plfiltercrop.h plfilterresize.h plfilterresizehamming.h plhsvconvert.h plfiltercontrast.h plfilterintensity.h plfilterlightness.h plfilterflip.h plfiltermirror.h plfilterfill.h plfilterfillrect.h plfilterpixel.h

ALL_CPP = plfilter.cpp \
plfilterresize.cpp \
plfiltercrop.cpp \
plfilterresizebilinear.cpp \
plfilterthreshold.cpp \
plfiltergetalpha.cpp \
plfilterresizebox.cpp \
plfiltervideoinvert.cpp \
plfiltergrayscale.cpp \
plfilterresizegaussian.cpp \
plhsvconvert.cpp \
plfilterquantize.cpp \
plfilterresizehamming.cpp \
plfilterlightness.cpp \
plfiltercontrast.cpp \
plfilterflip.cpp \
plfiltermirror.cpp \
plfilterintensity.cpp \
plfilterfill.cpp


# Libraries to generate
noinst_LTLIBRARIES = libfilter.la
# -no-undefinef needed for libtool for contruct dlls correctly with windows
# using cygwin tools.
libfilter_la_LDGFLAGS = -no-undefined
libfilter_la_CFLAGS = -DPAINTLIB_INTERNAL
libfilter_la_SOURCES = $(ALL_CPP) $(ALL_H)

paintlibdir = $(includedir)/paintlib
filterdir = $(paintlibdir)/Filter
filter_HEADERS = $(ALL_H)
