
MAINTAINERCLEANFILES = Makefile.in 

#
# There is PNG Support from the library ?
#
if SYS_LIBPNG
SYS_PNGDIR=im2png
else
SYS_PNGDIR=
endif

#
# There is Tiff Support from the library ?
#
if SYS_LIBTIFF
SYS_TIFFDIR=im2tiff
else
SYS_TIFFDIR=
endif

# There is JPEG Support from the library ?
#
if SYS_LIBJPEG
SYS_JPEGDIR=im2jpeg
else
SYS_JPEGDIR=
endif

  
SUBDIRS=im2bmp $(SYS_JPEGDIR) $(SYS_TIFFDIR) $(SYS_PNGDIR)
