# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=testdib - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to testdib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "testdib - Win32 Release" && "$(CFG)" !=\
 "testdib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "testdib.mak" CFG="testdib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "testdib - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "testdib - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "testdib - Win32 Debug"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "testdib - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "exe\Release"
# PROP Intermediate_Dir "obj\Release"
# PROP Target_Dir ""
OUTDIR=.\exe\Release
INTDIR=.\obj\Release

ALL : "$(OUTDIR)\testdib.exe"

CLEAN : 
	-@erase "$(INTDIR)\dibgrit.obj"
	-@erase "$(INTDIR)\Dibwnd.obj"
	-@erase "$(INTDIR)\drawhelp.obj"
	-@erase "$(INTDIR)\grarray.obj"
	-@erase "$(INTDIR)\Gritem.obj"
	-@erase "$(INTDIR)\Maindlg.obj"
	-@erase "$(INTDIR)\mtdibwnd.obj"
	-@erase "$(INTDIR)\shadow.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\testdib.obj"
	-@erase "$(INTDIR)\testdib.pch"
	-@erase "$(INTDIR)\testdib.res"
	-@erase "$(OUTDIR)\testdib.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /Ob2 /I "..\..\src" /I "..\paintlib" /I "..\..\..\otherlib\libtiff" /I "..\libtiff" /I "..\jpeglib" /I "..\..\..\otherlib\jpeglib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /G5 /MT /W3 /GX /O2 /Ob2 /I "..\..\src" /I "..\paintlib" /I\
 "..\..\..\otherlib\libtiff" /I "..\libtiff" /I "..\jpeglib" /I\
 "..\..\..\otherlib\jpeglib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fp"$(INTDIR)/testdib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\obj\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
RSC_PROJ=/l 0x407 /fo"$(INTDIR)/testdib.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/testdib.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\bin\release\paintlib.lib ..\bin\release\libtiff.lib ..\bin\release\jpeglib.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /profile /map
LINK32_FLAGS=..\bin\release\paintlib.lib ..\bin\release\libtiff.lib\
 ..\bin\release\jpeglib.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/testdib.pdb" /machine:I386 /out:"$(OUTDIR)/testdib.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dibgrit.obj" \
	"$(INTDIR)\Dibwnd.obj" \
	"$(INTDIR)\drawhelp.obj" \
	"$(INTDIR)\grarray.obj" \
	"$(INTDIR)\Gritem.obj" \
	"$(INTDIR)\Maindlg.obj" \
	"$(INTDIR)\mtdibwnd.obj" \
	"$(INTDIR)\shadow.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\testdib.obj" \
	"$(INTDIR)\testdib.res"

"$(OUTDIR)\testdib.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "testdib - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "exe/Debug"
# PROP Intermediate_Dir "obj/Debug"
# PROP Target_Dir ""
OUTDIR=.\exe/Debug
INTDIR=.\obj/Debug

ALL : "$(OUTDIR)\testdib.exe"

CLEAN : 
	-@erase "$(INTDIR)\dibgrit.obj"
	-@erase "$(INTDIR)\Dibwnd.obj"
	-@erase "$(INTDIR)\drawhelp.obj"
	-@erase "$(INTDIR)\grarray.obj"
	-@erase "$(INTDIR)\Gritem.obj"
	-@erase "$(INTDIR)\Maindlg.obj"
	-@erase "$(INTDIR)\mtdibwnd.obj"
	-@erase "$(INTDIR)\shadow.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\testdib.obj"
	-@erase "$(INTDIR)\testdib.pch"
	-@erase "$(INTDIR)\testdib.res"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\testdib.exe"
	-@erase "$(OUTDIR)\testdib.ilk"
	-@erase "$(OUTDIR)\testdib.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /Gi /GX /Zi /Od /I "..\..\src" /I "..\paintlib" /I "..\..\..\otherlib\libtiff" /I "..\libtiff" /I "..\jpeglib" /I "..\..\..\otherlib\jpeglib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /G5 /MTd /W3 /Gm /Gi /GX /Zi /Od /I "..\..\src" /I\
 "..\paintlib" /I "..\..\..\otherlib\libtiff" /I "..\libtiff" /I "..\jpeglib" /I\
 "..\..\..\otherlib\jpeglib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fp"$(INTDIR)/testdib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\obj/Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
RSC_PROJ=/l 0x407 /fo"$(INTDIR)/testdib.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/testdib.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ..\bin\debug\paintlib.lib ..\bin\debug\jpeglib.lib ..\bin\debug\libtiff.lib /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /profile /map
LINK32_FLAGS=..\bin\debug\paintlib.lib ..\bin\debug\jpeglib.lib\
 ..\bin\debug\libtiff.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/testdib.pdb" /debug /machine:I386 /out:"$(OUTDIR)/testdib.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dibgrit.obj" \
	"$(INTDIR)\Dibwnd.obj" \
	"$(INTDIR)\drawhelp.obj" \
	"$(INTDIR)\grarray.obj" \
	"$(INTDIR)\Gritem.obj" \
	"$(INTDIR)\Maindlg.obj" \
	"$(INTDIR)\mtdibwnd.obj" \
	"$(INTDIR)\shadow.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\testdib.obj" \
	"$(INTDIR)\testdib.res"

"$(OUTDIR)\testdib.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
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

# Name "testdib - Win32 Release"
# Name "testdib - Win32 Debug"

!IF  "$(CFG)" == "testdib - Win32 Release"

!ELSEIF  "$(CFG)" == "testdib - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Maindlg.cpp

!IF  "$(CFG)" == "testdib - Win32 Release"

DEP_CPP_MAIND=\
	"..\..\src\anybmp.h"\
	"..\..\src\anydec.h"\
	"..\..\src\bitmap.h"\
	"..\..\src\config.h"\
	"..\..\src\datasrc.h"\
	"..\..\src\except.h"\
	"..\..\src\picdec.h"\
	"..\..\src\pictdec.h"\
	"..\..\src\qdraw.h"\
	"..\paintlib\winbmp.h"\
	".\dibgrit.h"\
	".\dibwnd.h"\
	".\drawhelp.h"\
	".\grarray.h"\
	".\gritem.h"\
	".\maindlg.h"\
	".\mtdibwnd.h"\
	".\shadow.h"\
	".\StdAfx.h"\
	".\testdib.h"\
	

"$(INTDIR)\Maindlg.obj" : $(SOURCE) $(DEP_CPP_MAIND) "$(INTDIR)"\
 "$(INTDIR)\testdib.pch"


!ELSEIF  "$(CFG)" == "testdib - Win32 Debug"

DEP_CPP_MAIND=\
	"..\..\src\anybmp.h"\
	"..\..\src\anydec.h"\
	"..\..\src\bitmap.h"\
	"..\..\src\config.h"\
	"..\..\src\datasrc.h"\
	"..\..\src\except.h"\
	"..\..\src\picdec.h"\
	"..\..\src\pictdec.h"\
	"..\..\src\qdraw.h"\
	"..\paintlib\winbmp.h"\
	".\dibgrit.h"\
	".\dibwnd.h"\
	".\drawhelp.h"\
	".\grarray.h"\
	".\gritem.h"\
	".\maindlg.h"\
	".\mtdibwnd.h"\
	".\shadow.h"\
	".\StdAfx.h"\
	".\testdib.h"\
	

"$(INTDIR)\Maindlg.obj" : $(SOURCE) $(DEP_CPP_MAIND) "$(INTDIR)"\
 "$(INTDIR)\testdib.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dibwnd.cpp
DEP_CPP_DIBWN=\
	"..\..\src\bitmap.h"\
	"..\paintlib\winbmp.h"\
	".\dibwnd.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Dibwnd.obj" : $(SOURCE) $(DEP_CPP_DIBWN) "$(INTDIR)"\
 "$(INTDIR)\testdib.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\mtdibwnd.cpp
DEP_CPP_MTDIB=\
	"..\..\src\bitmap.h"\
	"..\paintlib\winbmp.h"\
	".\dibwnd.h"\
	".\mtdibwnd.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\mtdibwnd.obj" : $(SOURCE) $(DEP_CPP_MTDIB) "$(INTDIR)"\
 "$(INTDIR)\testdib.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\testdib.rc
DEP_RSC_TESTD=\
	".\res\dibtest.ico"\
	".\res\dibtest.rc2"\
	".\res\test3.bmp"\
	".\res\test4.bmp"\
	".\res\test8.bmp"\
	

"$(INTDIR)\testdib.res" : $(SOURCE) $(DEP_RSC_TESTD) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\testdib.cpp

!IF  "$(CFG)" == "testdib - Win32 Release"

DEP_CPP_TESTDI=\
	"..\..\src\bitmap.h"\
	"..\..\src\datasrc.h"\
	"..\..\src\picdec.h"\
	"..\paintlib\winbmp.h"\
	".\dibwnd.h"\
	".\grarray.h"\
	".\gritem.h"\
	".\maindlg.h"\
	".\mtdibwnd.h"\
	".\StdAfx.h"\
	".\testdib.h"\
	

"$(INTDIR)\testdib.obj" : $(SOURCE) $(DEP_CPP_TESTDI) "$(INTDIR)"\
 "$(INTDIR)\testdib.pch"


!ELSEIF  "$(CFG)" == "testdib - Win32 Debug"

DEP_CPP_TESTDI=\
	"..\..\src\bitmap.h"\
	"..\..\src\datasrc.h"\
	"..\..\src\picdec.h"\
	"..\paintlib\winbmp.h"\
	".\dibwnd.h"\
	".\grarray.h"\
	".\gritem.h"\
	".\maindlg.h"\
	".\mtdibwnd.h"\
	".\StdAfx.h"\
	".\testdib.h"\
	

"$(INTDIR)\testdib.obj" : $(SOURCE) $(DEP_CPP_TESTDI) "$(INTDIR)"\
 "$(INTDIR)\testdib.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Gritem.cpp
DEP_CPP_GRITE=\
	"..\..\src\bitmap.h"\
	".\gritem.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Gritem.obj" : $(SOURCE) $(DEP_CPP_GRITE) "$(INTDIR)"\
 "$(INTDIR)\testdib.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dibgrit.CPP
DEP_CPP_DIBGR=\
	"..\..\src\bitmap.h"\
	".\dibgrit.h"\
	".\gritem.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\dibgrit.obj" : $(SOURCE) $(DEP_CPP_DIBGR) "$(INTDIR)"\
 "$(INTDIR)\testdib.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\grarray.cpp
DEP_CPP_GRARR=\
	"..\..\src\bitmap.h"\
	".\grarray.h"\
	".\gritem.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\grarray.obj" : $(SOURCE) $(DEP_CPP_GRARR) "$(INTDIR)"\
 "$(INTDIR)\testdib.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\drawhelp.cpp
DEP_CPP_DRAWH=\
	"..\..\src\bitmap.h"\
	".\dibgrit.h"\
	".\drawhelp.h"\
	".\gritem.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\drawhelp.obj" : $(SOURCE) $(DEP_CPP_DRAWH) "$(INTDIR)"\
 "$(INTDIR)\testdib.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\shadow.CPP
DEP_CPP_SHADO=\
	"..\..\src\bitmap.h"\
	".\gritem.h"\
	".\shadow.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\shadow.obj" : $(SOURCE) $(DEP_CPP_SHADO) "$(INTDIR)"\
 "$(INTDIR)\testdib.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\stdafx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "testdib - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /G5 /MT /W3 /GX /O2 /Ob2 /I "..\..\src" /I "..\paintlib" /I\
 "..\..\..\otherlib\libtiff" /I "..\libtiff" /I "..\jpeglib" /I\
 "..\..\..\otherlib\jpeglib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fp"$(INTDIR)/testdib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\testdib.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "testdib - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /G5 /MTd /W3 /Gm /Gi /GX /Zi /Od /I "..\..\src" /I\
 "..\paintlib" /I "..\..\..\otherlib\libtiff" /I "..\libtiff" /I "..\jpeglib" /I\
 "..\..\..\otherlib\jpeglib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fp"$(INTDIR)/testdib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\testdib.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
