# Microsoft Developer Studio Project File - Name="paintlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=paintlib - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "paintlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "paintlib.mak" CFG="paintlib - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "paintlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "paintlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/paintlib", CAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "paintlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\Release"
# PROP Intermediate_Dir "..\..\obj\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /O2 /Ob2 /I "." /I "..\..\common" /I "..\..\common\filter" /I "..\libtiff" /I "..\..\otherlib\libtiff\libtiff" /I "..\libjpeg" /I "..\..\otherlib\libjpeg" /I "..\..\otherlib\libpng" /I "..\..\otherlib\zlib" /I "..\..\otherlib\libungif\lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"plstdpch.h" /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "paintlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\Debug"
# PROP Intermediate_Dir "..\..\obj\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MDd /W3 /Gi /GR /GX /ZI /Od /I "." /I "..\..\common" /I "..\..\common\filter" /I "..\libtiff" /I "..\..\otherlib\libtiff\libtiff" /I "..\libjpeg" /I "..\..\otherlib\libjpeg" /I "..\..\otherlib\libpng" /I "..\..\otherlib\zlib" /I "..\..\otherlib\libungif\lib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"plstdpch.h" /Yd /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "paintlib - Win32 Release"
# Name "paintlib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Group "Filter."

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\Filter\PLFilter.cpp
DEP_CPP_PLFIL=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\planybmp.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterContrast.cpp
DEP_CPP_PLFILT=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterContrast.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterCrop.cpp
DEP_CPP_PLFILTE=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterCrop.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterFill.cpp
DEP_CPP_PLFILTER=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterFill.h"\
	"..\..\common\Filter\PLFilterFillRect.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterFlip.cpp
DEP_CPP_PLFILTERF=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterFlip.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterGetAlpha.cpp
DEP_CPP_PLFILTERG=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterGetAlpha.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterGrayscale.cpp
DEP_CPP_PLFILTERGR=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterGrayscale.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterIntensity.cpp
DEP_CPP_PLFILTERI=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterIntensity.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterLightness.cpp
DEP_CPP_PLFILTERL=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterLightness.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterMirror.cpp
DEP_CPP_PLFILTERM=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterMirror.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterQuantize.cpp
DEP_CPP_PLFILTERQ=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterQuantize.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResize.cpp
DEP_CPP_PLFILTERR=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterResize.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeBilinear.cpp
DEP_CPP_PLFILTERRE=\
	"..\..\common\Filter\pl2PassScale.h"\
	"..\..\common\Filter\plcontribdefs.h"\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterResize.h"\
	"..\..\common\Filter\PLFilterResizeBilinear.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeBox.cpp
DEP_CPP_PLFILTERRES=\
	"..\..\common\Filter\pl2PassScale.h"\
	"..\..\common\Filter\plcontribdefs.h"\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterResize.h"\
	"..\..\common\Filter\PLFilterResizeBox.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeGaussian.cpp
DEP_CPP_PLFILTERRESI=\
	"..\..\common\Filter\pl2PassScale.h"\
	"..\..\common\Filter\plcontribdefs.h"\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterResize.h"\
	"..\..\common\Filter\PLFilterResizeGaussian.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeHamming.cpp
DEP_CPP_PLFILTERRESIZ=\
	"..\..\common\Filter\pl2PassScale.h"\
	"..\..\common\Filter\plcontribdefs.h"\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterResize.h"\
	"..\..\common\Filter\PLFilterResizeHamming.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterThreshold.cpp
DEP_CPP_PLFILTERT=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterThreshold.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\plfiltervideoinvert.cpp
DEP_CPP_PLFILTERV=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterVideoInvert.h"\
	"..\..\common\Filter\plhsvconvert.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\plhsvconvert.cpp
DEP_CPP_PLHSV=\
	"..\..\common\Filter\plhsvconvert.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# End Group
# Begin Group "Win."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\plddhelper.cpp
DEP_CPP_PLDDH=\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plstdpch.h"\
	".\plddhelper.h"\
	
# End Source File
# Begin Source File

SOURCE=.\plddrawbmp.cpp
DEP_CPP_PLDDR=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	".\plddhelper.h"\
	".\plddrawbmp.h"\
	
# End Source File
# Begin Source File

SOURCE=.\pldibsect.cpp
DEP_CPP_PLDIB=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	".\pldibsect.h"\
	".\plwinbmp.h"\
	
# End Source File
# Begin Source File

SOURCE=.\plressrc.cpp
DEP_CPP_PLRES=\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	".\plressrc.h"\
	
# End Source File
# Begin Source File

SOURCE=.\plStreamSink.cpp
DEP_CPP_PLSTR=\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	".\plStreamSink.h"\
	
# End Source File
# Begin Source File

SOURCE=.\plwemfdec.cpp
DEP_CPP_PLWEM=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plfilesrc.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	".\plwemfdec.h"\
	".\plwinbmp.h"\
	
# End Source File
# Begin Source File

SOURCE=.\plwinbmp.cpp
DEP_CPP_PLWIN=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	".\plwinbmp.h"\
	
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\common\jmemdest.cpp
DEP_CPP_JMEMD=\
	"..\..\common\jmemdest.h"\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\jmemsrc.cpp
DEP_CPP_JMEMS=\
	"..\..\Common\jmemsrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plstdpch.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\planybmp.cpp
DEP_CPP_PLANY=\
	"..\..\common\planybmp.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\planydec.cpp
DEP_CPP_PLANYD=\
	"..\..\common\planydec.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpdec.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plgifdec.h"\
	"..\..\common\pljpegdec.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpcx.h"\
	"..\..\common\plpcxdec.h"\
	"..\..\common\plpgm.h"\
	"..\..\common\plpgmdec.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpictdec.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpngdec.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plppm.h"\
	"..\..\common\plppmdec.h"\
	"..\..\common\plpsddec.h"\
	"..\..\common\plstdpch.h"\
	"..\..\common\pltga.h"\
	"..\..\common\pltgadec.h"\
	"..\..\common\pltiffdec.h"\
	"..\..\common\plwindefs.h"\
	"..\..\common\qdraw.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	".\plwemfdec.h"\
	
NODEP_CPP_PLANYD=\
	"..\..\common\png.h"\
	"..\..\common\tiff.h"\
	"..\..\common\tiffio.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plbitmap.cpp
DEP_CPP_PLBIT=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterQuantize.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	

!IF  "$(CFG)" == "paintlib - Win32 Release"

!ELSEIF  "$(CFG)" == "paintlib - Win32 Debug"

# SUBTRACT CPP /X

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\common\plbmpdec.cpp
DEP_CPP_PLBMP=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpdec.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\common\plwindefs.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plbmpenc.cpp
DEP_CPP_PLBMPE=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpenc.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicenc.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\common\plwindefs.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plbmpinfo.cpp
DEP_CPP_PLBMPI=\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\pldatasink.cpp
DEP_CPP_PLDAT=\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\pldatasrc.cpp
DEP_CPP_PLDATA=\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plprognot.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plexcept.cpp
DEP_CPP_PLEXC=\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plfilesink.cpp
DEP_CPP_PLFILE=\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plfilesink.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plfilesrc.cpp
DEP_CPP_PLFILES=\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plfilesrc.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\pljpegdec.cpp
DEP_CPP_PLJPE=\
	"..\..\Common\jmemsrc.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\pljpegdec.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	"..\libjpeg\jmorecfg.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\pljpegenc.cpp
DEP_CPP_PLJPEG=\
	"..\..\common\jmemdest.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\pljpegenc.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicenc.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	"..\libjpeg\jmorecfg.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plmemsink.cpp
DEP_CPP_PLMEM=\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plmemsink.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plmemsrc.cpp
DEP_CPP_PLMEMS=\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plmemsrc.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\PLObject.cpp
DEP_CPP_PLOBJ=\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpcxdec.cpp
DEP_CPP_PLPCX=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpcx.h"\
	"..\..\common\plpcxdec.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpgmdec.cpp
DEP_CPP_PLPGM=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpgm.h"\
	"..\..\common\plpgmdec.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpicdec.cpp
DEP_CPP_PLPIC=\
	"..\..\common\planybmp.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plfilesrc.h"\
	"..\..\common\plmemsrc.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	".\plressrc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpicenc.cpp
DEP_CPP_PLPICE=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plfilesink.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpicenc.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpictdec.cpp
DEP_CPP_PLPICT=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\pljpegdec.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\ploptable.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpictdec.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\common\qdraw.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plppmdec.cpp
DEP_CPP_PLPPM=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plppm.h"\
	"..\..\common\plppmdec.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpsddec.cpp
DEP_CPP_PLPSD=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plfilesrc.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixel8.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plpsddec.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plstdpch.cpp
DEP_CPP_PLSTD=\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plstdpch.h"\
	
# ADD CPP /Yc"plstdpch.h"
# End Source File
# Begin Source File

SOURCE=..\..\common\pltrace.cpp
DEP_CPP_PLTRA=\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Group "Filter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\Filter\pl2PassScale.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\plcontribdefs.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilter.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterContrast.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterCrop.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterFill.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterFillRect.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterFlip.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterGetAlpha.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterGrayscale.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterIntensity.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterLightness.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterMirror.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\plfilterpixel.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterQuantize.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResize.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeBilinear.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeBox.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeGaussian.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeHamming.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterThreshold.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterVideoInvert.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\plhsvconvert.h
# End Source File
# End Group
# Begin Group "Win"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\plddhelper.h
# End Source File
# Begin Source File

SOURCE=.\plddrawbmp.h
# End Source File
# Begin Source File

SOURCE=.\pldibsect.h
# End Source File
# Begin Source File

SOURCE=.\plressrc.h
# End Source File
# Begin Source File

SOURCE=.\plStreamSink.h
# End Source File
# Begin Source File

SOURCE=.\plwemfdec.h
# End Source File
# Begin Source File

SOURCE=.\plwinbmp.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Common\config.h
# End Source File
# Begin Source File

SOURCE=..\..\common\jmemdest.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\jmemsrc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\planybmp.h
# End Source File
# Begin Source File

SOURCE=..\..\common\planydec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plbitmap.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plbmpdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plbmpenc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plbmpinfo.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pldatasink.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pldatasrc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pldebug.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plexcept.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plfilesink.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plfilesrc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pljpegdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pljpegenc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plmemsink.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plmemsrc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\PLObject.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpaintlibdefs.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpcx.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpcxdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpgm.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpgmdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpicdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpicenc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpictdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpixel24.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpixel32.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpixel8.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpixeldefs.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpoint.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plppm.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plppmdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plprognot.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpsddec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plrect.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plstdpch.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plwindefs.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\common\config.vc

!IF  "$(CFG)" == "paintlib - Win32 Release"

# Begin Custom Build
InputPath=..\..\common\config.vc

"..\..\common\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\common\config.vc ..\..\common\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "paintlib - Win32 Debug"

# Begin Custom Build - Copying config.vc
InputPath=..\..\common\config.vc

"..\..\common\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\common\config.vc ..\..\common\config.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\common\plpixeldefs.vc

!IF  "$(CFG)" == "paintlib - Win32 Release"

# Begin Custom Build
InputPath=..\..\common\plpixeldefs.vc

"..\..\common\plpixeldefs.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\common\plpixeldefs.vc ..\..\common\plpixeldefs.h

# End Custom Build

!ELSEIF  "$(CFG)" == "paintlib - Win32 Debug"

# Begin Custom Build - Copying plpixeldefs.h
InputPath=..\..\common\plpixeldefs.vc

"..\..\common\plpixeldefs.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\common\plpixeldefs.vc ..\..\common\plpixeldefs.h

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
