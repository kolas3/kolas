# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=paintlib - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to paintlib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "paintlib - Win32 Release" && "$(CFG)" !=\
 "paintlib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "paintlib.mak" CFG="paintlib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "paintlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "paintlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "paintlib - Win32 Debug"
CPP=cl.exe

!IF  "$(CFG)" == "paintlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin\Release"
# PROP Intermediate_Dir "..\obj\Release"
# PROP Target_Dir ""
OUTDIR=.\..\bin\Release
INTDIR=.\..\obj\Release

ALL : "$(OUTDIR)\paintlib.lib"

CLEAN : 
	-@erase "$(INTDIR)\Anybmp.obj"
	-@erase "$(INTDIR)\Anydec.obj"
	-@erase "$(INTDIR)\Bitmap.obj"
	-@erase "$(INTDIR)\Bmpdec.obj"
	-@erase "$(INTDIR)\Datasrc.obj"
	-@erase "$(INTDIR)\Except.obj"
	-@erase "$(INTDIR)\Filesrc.obj"
	-@erase "$(INTDIR)\Jmemsrc.obj"
	-@erase "$(INTDIR)\Jpegdec.obj"
	-@erase "$(INTDIR)\paintlib.pch"
	-@erase "$(INTDIR)\Picdec.obj"
	-@erase "$(INTDIR)\Pictdec.obj"
	-@erase "$(INTDIR)\Stdpch.obj"
	-@erase "$(INTDIR)\Tgadec.obj"
	-@erase "$(INTDIR)\tif_msrc.obj"
	-@erase "$(INTDIR)\Tiffdec.obj"
	-@erase "$(INTDIR)\Trace.obj"
	-@erase "$(INTDIR)\winbmp.obj"
	-@erase "$(OUTDIR)\paintlib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /Ob2 /I "." /I "..\..\src" /I "..\libtiff" /I "..\..\..\otherlib\libtiff\libtiff" /I "..\jpeglib" /I "..\..\..\otherlib\jpeglib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdpch.h" /c
CPP_PROJ=/nologo /G5 /MT /W3 /GX /O2 /Ob2 /I "." /I "..\..\src" /I "..\libtiff"\
 /I "..\..\..\otherlib\libtiff\libtiff" /I "..\jpeglib" /I\
 "..\..\..\otherlib\jpeglib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fp"$(INTDIR)/paintlib.pch" /Yu"stdpch.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\obj\Release/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/paintlib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/paintlib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Anybmp.obj" \
	"$(INTDIR)\Anydec.obj" \
	"$(INTDIR)\Bitmap.obj" \
	"$(INTDIR)\Bmpdec.obj" \
	"$(INTDIR)\Datasrc.obj" \
	"$(INTDIR)\Except.obj" \
	"$(INTDIR)\Filesrc.obj" \
	"$(INTDIR)\Jmemsrc.obj" \
	"$(INTDIR)\Jpegdec.obj" \
	"$(INTDIR)\Picdec.obj" \
	"$(INTDIR)\Pictdec.obj" \
	"$(INTDIR)\Stdpch.obj" \
	"$(INTDIR)\Tgadec.obj" \
	"$(INTDIR)\tif_msrc.obj" \
	"$(INTDIR)\Tiffdec.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\winbmp.obj"

"$(OUTDIR)\paintlib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "paintlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin\Debug"
# PROP Intermediate_Dir "..\obj\Debug"
# PROP Target_Dir ""
OUTDIR=.\..\bin\Debug
INTDIR=.\..\obj\Debug

ALL : "$(OUTDIR)\paintlib.lib"

CLEAN : 
	-@erase "$(INTDIR)\Anybmp.obj"
	-@erase "$(INTDIR)\Anydec.obj"
	-@erase "$(INTDIR)\Bitmap.obj"
	-@erase "$(INTDIR)\Bmpdec.obj"
	-@erase "$(INTDIR)\Datasrc.obj"
	-@erase "$(INTDIR)\Except.obj"
	-@erase "$(INTDIR)\Filesrc.obj"
	-@erase "$(INTDIR)\Jmemsrc.obj"
	-@erase "$(INTDIR)\Jpegdec.obj"
	-@erase "$(INTDIR)\paintlib.pch"
	-@erase "$(INTDIR)\Picdec.obj"
	-@erase "$(INTDIR)\Pictdec.obj"
	-@erase "$(INTDIR)\Stdpch.obj"
	-@erase "$(INTDIR)\Tgadec.obj"
	-@erase "$(INTDIR)\tif_msrc.obj"
	-@erase "$(INTDIR)\Tiffdec.obj"
	-@erase "$(INTDIR)\Trace.obj"
	-@erase "$(INTDIR)\winbmp.obj"
	-@erase "$(OUTDIR)\paintlib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /Z7 /Od /I "." /I "..\..\src" /I "..\libtiff" /I "..\..\..\otherlib\libtiff\libtiff" /I "..\jpeglib" /I "..\..\..\otherlib\jpeglib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdpch.h" /Yd /c
CPP_PROJ=/nologo /G5 /MTd /W3 /GX /Z7 /Od /I "." /I "..\..\src" /I "..\libtiff"\
 /I "..\..\..\otherlib\libtiff\libtiff" /I "..\jpeglib" /I\
 "..\..\..\otherlib\jpeglib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fp"$(INTDIR)/paintlib.pch" /Yu"stdpch.h" /Fo"$(INTDIR)/" /Yd /c 
CPP_OBJS=.\..\obj\Debug/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/paintlib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/paintlib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Anybmp.obj" \
	"$(INTDIR)\Anydec.obj" \
	"$(INTDIR)\Bitmap.obj" \
	"$(INTDIR)\Bmpdec.obj" \
	"$(INTDIR)\Datasrc.obj" \
	"$(INTDIR)\Except.obj" \
	"$(INTDIR)\Filesrc.obj" \
	"$(INTDIR)\Jmemsrc.obj" \
	"$(INTDIR)\Jpegdec.obj" \
	"$(INTDIR)\Picdec.obj" \
	"$(INTDIR)\Pictdec.obj" \
	"$(INTDIR)\Stdpch.obj" \
	"$(INTDIR)\Tgadec.obj" \
	"$(INTDIR)\tif_msrc.obj" \
	"$(INTDIR)\Tiffdec.obj" \
	"$(INTDIR)\Trace.obj" \
	"$(INTDIR)\winbmp.obj"

"$(OUTDIR)\paintlib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "paintlib - Win32 Release"
# Name "paintlib - Win32 Debug"

!IF  "$(CFG)" == "paintlib - Win32 Release"

!ELSEIF  "$(CFG)" == "paintlib - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\winbmp.cpp
DEP_CPP_WINBM=\
	"..\..\src\bitmap.h"\
	"..\..\src\except.h"\
	"..\..\src\stdpch.h"\
	".\winbmp.h"\
	

"$(INTDIR)\winbmp.obj" : $(SOURCE) $(DEP_CPP_WINBM) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Trace.cpp
DEP_CPP_TRACE=\
	"..\..\src\stdpch.h"\
	

"$(INTDIR)\Trace.obj" : $(SOURCE) $(DEP_CPP_TRACE) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Anydec.cpp
DEP_CPP_ANYDE=\
	"..\..\..\otherlib\jpeglib\jerror.h"\
	"..\..\..\otherlib\jpeglib\jpegint.h"\
	"..\..\..\otherlib\jpeglib\jpeglib.h"\
	"..\..\Src\anydec.h"\
	"..\..\src\bitmap.h"\
	"..\..\Src\bmpdec.h"\
	"..\..\Src\config.h"\
	"..\..\Src\datasrc.h"\
	"..\..\src\except.h"\
	"..\..\Src\jpegdec.h"\
	"..\..\Src\picdec.h"\
	"..\..\Src\pictdec.h"\
	"..\..\Src\qdraw.h"\
	"..\..\src\stdpch.h"\
	"..\..\Src\tga.h"\
	"..\..\Src\tgadec.h"\
	"..\..\Src\tiffdec.h"\
	"..\..\Src\windefs.h"\
	"..\jpeglib\jconfig.h"\
	"..\jpeglib\jmorecfg.h"\
	

"$(INTDIR)\Anydec.obj" : $(SOURCE) $(DEP_CPP_ANYDE) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Bitmap.cpp
DEP_CPP_BITMA=\
	"..\..\src\bitmap.h"\
	"..\..\src\except.h"\
	"..\..\src\stdpch.h"\
	

"$(INTDIR)\Bitmap.obj" : $(SOURCE) $(DEP_CPP_BITMA) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Bmpdec.cpp
DEP_CPP_BMPDE=\
	"..\..\src\bitmap.h"\
	"..\..\Src\bmpdec.h"\
	"..\..\Src\datasrc.h"\
	"..\..\src\except.h"\
	"..\..\Src\picdec.h"\
	"..\..\src\stdpch.h"\
	"..\..\Src\windefs.h"\
	

"$(INTDIR)\Bmpdec.obj" : $(SOURCE) $(DEP_CPP_BMPDE) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Datasrc.cpp
DEP_CPP_DATAS=\
	"..\..\Src\datasrc.h"\
	"..\..\src\stdpch.h"\
	

"$(INTDIR)\Datasrc.obj" : $(SOURCE) $(DEP_CPP_DATAS) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Except.cpp
DEP_CPP_EXCEP=\
	"..\..\src\except.h"\
	"..\..\src\stdpch.h"\
	

"$(INTDIR)\Except.obj" : $(SOURCE) $(DEP_CPP_EXCEP) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Filesrc.cpp
DEP_CPP_FILES=\
	"..\..\Src\datasrc.h"\
	"..\..\src\except.h"\
	"..\..\Src\filesrc.h"\
	"..\..\src\stdpch.h"\
	

"$(INTDIR)\Filesrc.obj" : $(SOURCE) $(DEP_CPP_FILES) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Jpegdec.cpp
DEP_CPP_JPEGD=\
	"..\..\..\otherlib\jpeglib\jerror.h"\
	"..\..\..\otherlib\jpeglib\jpegint.h"\
	"..\..\..\otherlib\jpeglib\jpeglib.h"\
	"..\..\src\bitmap.h"\
	"..\..\Src\datasrc.h"\
	"..\..\src\except.h"\
	"..\..\Src\jmemsrc.h"\
	"..\..\Src\jpegdec.h"\
	"..\..\Src\picdec.h"\
	"..\..\src\stdpch.h"\
	"..\jpeglib\jconfig.h"\
	"..\jpeglib\jmorecfg.h"\
	

"$(INTDIR)\Jpegdec.obj" : $(SOURCE) $(DEP_CPP_JPEGD) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Picdec.cpp
DEP_CPP_PICDE=\
	"..\..\src\bitmap.h"\
	"..\..\Src\datasrc.h"\
	"..\..\src\except.h"\
	"..\..\Src\filesrc.h"\
	"..\..\Src\picdec.h"\
	"..\..\src\stdpch.h"\
	

"$(INTDIR)\Picdec.obj" : $(SOURCE) $(DEP_CPP_PICDE) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Pictdec.cpp
DEP_CPP_PICTD=\
	"..\..\..\otherlib\jpeglib\jerror.h"\
	"..\..\..\otherlib\jpeglib\jpegint.h"\
	"..\..\..\otherlib\jpeglib\jpeglib.h"\
	"..\..\src\bitmap.h"\
	"..\..\Src\datasrc.h"\
	"..\..\src\except.h"\
	"..\..\Src\jpegdec.h"\
	"..\..\Src\optable.h"\
	"..\..\Src\picdec.h"\
	"..\..\Src\pictdec.h"\
	"..\..\Src\qdraw.h"\
	"..\..\src\stdpch.h"\
	"..\jpeglib\jconfig.h"\
	"..\jpeglib\jmorecfg.h"\
	

"$(INTDIR)\Pictdec.obj" : $(SOURCE) $(DEP_CPP_PICTD) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Tgadec.cpp
DEP_CPP_TGADE=\
	"..\..\src\bitmap.h"\
	"..\..\Src\datasrc.h"\
	"..\..\src\except.h"\
	"..\..\Src\picdec.h"\
	"..\..\src\stdpch.h"\
	"..\..\Src\tga.h"\
	"..\..\Src\tgadec.h"\
	

"$(INTDIR)\Tgadec.obj" : $(SOURCE) $(DEP_CPP_TGADE) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Tiffdec.cpp
DEP_CPP_TIFFD=\
	"..\..\..\otherlib\libtiff\libtiff\tiff.h"\
	"..\..\..\otherlib\libtiff\libtiff\tiffio.h"\
	"..\..\src\bitmap.h"\
	"..\..\Src\datasrc.h"\
	"..\..\src\except.h"\
	"..\..\Src\picdec.h"\
	"..\..\src\stdpch.h"\
	"..\..\Src\tif_msrc.h"\
	"..\..\Src\tiffdec.h"\
	

"$(INTDIR)\Tiffdec.obj" : $(SOURCE) $(DEP_CPP_TIFFD) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Anybmp.cpp
DEP_CPP_ANYBM=\
	"..\..\Src\anybmp.h"\
	"..\..\src\bitmap.h"\
	"..\..\src\except.h"\
	"..\..\src\stdpch.h"\
	

"$(INTDIR)\Anybmp.obj" : $(SOURCE) $(DEP_CPP_ANYBM) "$(INTDIR)"\
 "$(INTDIR)\paintlib.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\Jmemsrc.c
DEP_CPP_JMEMS=\
	"..\..\..\otherlib\jpeglib\jerror.h"\
	"..\..\..\otherlib\jpeglib\jinclude.h"\
	"..\..\..\otherlib\jpeglib\jpegint.h"\
	"..\..\..\otherlib\jpeglib\jpeglib.h"\
	"..\..\Src\jmemsrc.h"\
	"..\jpeglib\jconfig.h"\
	"..\jpeglib\jmorecfg.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "paintlib - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\Jmemsrc.obj" : $(SOURCE) $(DEP_CPP_JMEMS) "$(INTDIR)"
   $(CPP) /nologo /G5 /MT /W3 /GX /O2 /Ob2 /I "." /I "..\..\src" /I\
 "..\libtiff" /I "..\..\..\otherlib\libtiff\libtiff" /I "..\jpeglib" /I\
 "..\..\..\otherlib\jpeglib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "paintlib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\Jmemsrc.obj" : $(SOURCE) $(DEP_CPP_JMEMS) "$(INTDIR)"
   $(CPP) /nologo /G5 /MTd /W3 /GX /Z7 /Od /I "." /I "..\..\src" /I\
 "..\libtiff" /I "..\..\..\otherlib\libtiff\libtiff" /I "..\jpeglib" /I\
 "..\..\..\otherlib\jpeglib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fo"$(INTDIR)/" /Yd /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\Code\Src\tif_msrc.c
DEP_CPP_TIF_M=\
	"..\..\..\otherlib\libtiff\libtiff\port.h"\
	"..\..\..\otherlib\libtiff\libtiff\tif_dir.h"\
	"..\..\..\otherlib\libtiff\libtiff\tiff.h"\
	"..\..\..\otherlib\libtiff\libtiff\tiffcomp.h"\
	"..\..\..\otherlib\libtiff\libtiff\tiffconf.h"\
	"..\..\..\otherlib\libtiff\libtiff\tiffio.h"\
	"..\..\..\otherlib\libtiff\libtiff\tiffiop.h"\
	"..\..\Src\tif_msrc.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "paintlib - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\tif_msrc.obj" : $(SOURCE) $(DEP_CPP_TIF_M) "$(INTDIR)"
   $(CPP) /nologo /G5 /MT /W3 /GX /O2 /Ob2 /I "." /I "..\..\src" /I\
 "..\libtiff" /I "..\..\..\otherlib\libtiff\libtiff" /I "..\jpeglib" /I\
 "..\..\..\otherlib\jpeglib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "paintlib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\tif_msrc.obj" : $(SOURCE) $(DEP_CPP_TIF_M) "$(INTDIR)"
   $(CPP) /nologo /G5 /MTd /W3 /GX /Z7 /Od /I "." /I "..\..\src" /I\
 "..\libtiff" /I "..\..\..\otherlib\libtiff\libtiff" /I "..\jpeglib" /I\
 "..\..\..\otherlib\jpeglib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fo"$(INTDIR)/" /Yd /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Prog\Paintlib\code\src\Stdpch.cpp
DEP_CPP_STDPC=\
	"..\..\src\stdpch.h"\
	

!IF  "$(CFG)" == "paintlib - Win32 Release"

# ADD CPP /Yc"stdpch.h"

BuildCmds= \
	$(CPP) /nologo /G5 /MT /W3 /GX /O2 /Ob2 /I "." /I "..\..\src" /I "..\libtiff"\
 /I "..\..\..\otherlib\libtiff\libtiff" /I "..\jpeglib" /I\
 "..\..\..\otherlib\jpeglib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fp"$(INTDIR)/paintlib.pch" /Yc"stdpch.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\Stdpch.obj" : $(SOURCE) $(DEP_CPP_STDPC) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\paintlib.pch" : $(SOURCE) $(DEP_CPP_STDPC) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "paintlib - Win32 Debug"

# ADD CPP /Yc"stdpch.h"

BuildCmds= \
	$(CPP) /nologo /G5 /MTd /W3 /GX /Z7 /Od /I "." /I "..\..\src" /I "..\libtiff"\
 /I "..\..\..\otherlib\libtiff\libtiff" /I "..\jpeglib" /I\
 "..\..\..\otherlib\jpeglib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fp"$(INTDIR)/paintlib.pch" /Yc"stdpch.h" /Fo"$(INTDIR)/" /Yd /c $(SOURCE) \
	

"$(INTDIR)\Stdpch.obj" : $(SOURCE) $(DEP_CPP_STDPC) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\paintlib.pch" : $(SOURCE) $(DEP_CPP_STDPC) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
