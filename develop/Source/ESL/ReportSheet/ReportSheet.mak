# Microsoft Developer Studio Generated NMAKE File, Based on ReportSheet.dsp
!IF "$(CFG)" == ""
CFG=ReportSheet - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ReportSheet - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ReportSheet - Win32 Release" && "$(CFG)" !=\
 "ReportSheet - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ReportSheet.mak" CFG="ReportSheet - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ReportSheet - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ReportSheet - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ReportSheet - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ReportSheet.dll"

!ELSE 

ALL : "$(OUTDIR)\ReportSheet.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ReportSheet.obj"
	-@erase "$(INTDIR)\ReportSheet.pch"
	-@erase "$(INTDIR)\ReportSheet.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\WorkSheet.obj"
	-@erase "$(OUTDIR)\ReportSheet.dll"
	-@erase "$(OUTDIR)\ReportSheet.exp"
	-@erase "$(OUTDIR)\ReportSheet.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\ReportSheet.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ReportSheet.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ReportSheet.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\ReportSheet.pdb" /machine:I386 /def:".\ReportSheet.def"\
 /out:"$(OUTDIR)\ReportSheet.dll" /implib:"$(OUTDIR)\ReportSheet.lib" 
DEF_FILE= \
	".\ReportSheet.def"
LINK32_OBJS= \
	"$(INTDIR)\ReportSheet.obj" \
	"$(INTDIR)\ReportSheet.res" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WorkSheet.obj"

"$(OUTDIR)\ReportSheet.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ReportSheet - Win32 Debug"

OUTDIR=\통합정보시스템\develop\lib
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\IIS_EXE\ReportSheet.dll"

!ELSE 

ALL : "..\..\..\..\IIS_EXE\ReportSheet.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ReportSheet.obj"
	-@erase "$(INTDIR)\ReportSheet.pch"
	-@erase "$(INTDIR)\ReportSheet.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\WorkSheet.obj"
	-@erase "$(OUTDIR)\ReportSheet.exp"
	-@erase "$(OUTDIR)\ReportSheet.lib"
	-@erase "$(OUTDIR)\ReportSheet.pdb"
	-@erase "..\..\..\..\IIS_EXE\ReportSheet.dll"
	-@erase "..\..\..\..\IIS_EXE\ReportSheet.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\ReportSheet.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ReportSheet.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ReportSheet.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\ReportSheet.pdb" /debug /machine:I386 /def:".\ReportSheet.def"\
 /out:"\통합정보시스템\IIS_EXE\ReportSheet.dll" /implib:"$(OUTDIR)\ReportSheet.lib"\
 /pdbtype:sept 
DEF_FILE= \
	".\ReportSheet.def"
LINK32_OBJS= \
	"$(INTDIR)\ReportSheet.obj" \
	"$(INTDIR)\ReportSheet.res" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WorkSheet.obj"

"..\..\..\..\IIS_EXE\ReportSheet.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(CFG)" == "ReportSheet - Win32 Release" || "$(CFG)" ==\
 "ReportSheet - Win32 Debug"
SOURCE=.\ReportSheet.cpp

!IF  "$(CFG)" == "ReportSheet - Win32 Release"

DEP_CPP_REPOR=\
	".\StdAfx.h"\
	

"$(INTDIR)\ReportSheet.obj" : $(SOURCE) $(DEP_CPP_REPOR) "$(INTDIR)"\
 "$(INTDIR)\ReportSheet.pch"


!ELSEIF  "$(CFG)" == "ReportSheet - Win32 Debug"


"$(INTDIR)\ReportSheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ReportSheet.pch"


!ENDIF 

SOURCE=.\ReportSheet.rc
DEP_RSC_REPORT=\
	".\res\ReportSheet.rc2"\
	

"$(INTDIR)\ReportSheet.res" : $(SOURCE) $(DEP_RSC_REPORT) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "ReportSheet - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\ReportSheet.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ReportSheet.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ReportSheet - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT"\
 /Fp"$(INTDIR)\ReportSheet.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ReportSheet.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\WorkSheet.cpp

!IF  "$(CFG)" == "ReportSheet - Win32 Release"

DEP_CPP_WORKS=\
	".\StdAfx.h"\
	{$(INCLUDE)}"ReportStruct.h"\
	{$(INCLUDE)}"rgbcolor.h"\
	{$(INCLUDE)}"WorkSheet.h"\
	

"$(INTDIR)\WorkSheet.obj" : $(SOURCE) $(DEP_CPP_WORKS) "$(INTDIR)"\
 "$(INTDIR)\ReportSheet.pch"


!ELSEIF  "$(CFG)" == "ReportSheet - Win32 Debug"

DEP_CPP_WORKS=\
	{$(INCLUDE)}"ReportStruct.h"\
	{$(INCLUDE)}"rgbcolor.h"\
	{$(INCLUDE)}"WorkSheet.h"\
	

"$(INTDIR)\WorkSheet.obj" : $(SOURCE) $(DEP_CPP_WORKS) "$(INTDIR)"\
 "$(INTDIR)\ReportSheet.pch"


!ENDIF 


!ENDIF 

