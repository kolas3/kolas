
AUTOMAKE_OPTIONS        = dist-zip

EXTRA_DIST = doc/*.html doc/pics/*.gif \
             win/TIFFEX/*.* \
             win/VBPaintX/*.* \
             win/VCPaintX/*.* win/VCPaintX/res/*.* \
             win/libpng/*.* \
             win/libungif/*.* \
             win/piclook/*.*  win/piclook/res/*.* \
             win/testdec/*.* win/testdec/res/*.* \
             win/PaintX/*.* win/PaintX/ExcelTest/*.* \
             win/libjpeg/*.* \
             win/libtiff/*.* \
             win/paintlib/*.* \
             win/test/*.* \
             win/testdib/*.* win/testdib/res/*.* win/testdib/pics/*.* \
             common/*.vc common/*.cpp

dist-hook:
	rm $(distdir)/common/config.h
	rm $(distdir)/common/plpixeldefs.h

MAINTAINERCLEANFILES =

# Directories to process
SUBDIRS = common lib gnu

DIST_SUBDIRS = common lib gnu

bin_SCRIPTS = paintlib-config
