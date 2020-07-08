# Microsoft Developer Studio Generated NMAKE File, Based on SIReportSample.dsp
!IF "$(CFG)" == ""
CFG=SIReportSample - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SIReportSample - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SIReportSample - Win32 Release" && "$(CFG)" != "SIReportSample - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SIReportSample.mak" CFG="SIReportSample - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SIReportSample - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SIReportSample - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "SIReportSample - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\SIReportSample.dll"


CLEAN :
	-@erase "$(INTDIR)\DlgSIReportSample.obj"
	-@erase "$(INTDIR)\SIReportSample.obj"
	-@erase "$(INTDIR)\SIReportSample.pch"
	-@erase "$(INTDIR)\SIReportSample.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SIReportSample.dll"
	-@erase "$(OUTDIR)\SIReportSample.exp"
	-@erase "$(OUTDIR)\SIReportSample.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\SIReportSample.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SIReportSample.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SIReportSample.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\SIReportSample.pdb" /machine:I386 /def:".\SIReportSample.def" /out:"$(OUTDIR)\SIReportSample.dll" /implib:"$(OUTDIR)\SIReportSample.lib" 
DEF_FILE= \
	".\SIReportSample.def"
LINK32_OBJS= \
	"$(INTDIR)\DlgSIReportSample.obj" \
	"$(INTDIR)\SIReportSample.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SIReportSample.res"

"$(OUTDIR)\SIReportSample.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SIReportSample - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\..\dll\common\SIReportSample.dll"


CLEAN :
	-@erase "$(INTDIR)\DlgSIReportSample.obj"
	-@erase "$(INTDIR)\SIReportSample.obj"
	-@erase "$(INTDIR)\SIReportSample.pch"
	-@erase "$(INTDIR)\SIReportSample.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SIReportSample.exp"
	-@erase "$(OUTDIR)\SIReportSample.lib"
	-@erase "$(OUTDIR)\SIReportSample.pdb"
	-@erase "..\..\..\..\dll\common\SIReportSample.dll"
	-@erase "..\..\..\..\dll\common\SIReportSample.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\SIReportSample.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SIReportSample.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SIReportSample.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=esl.lib SIReportMgr.lib SIReportData.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\SIReportSample.pdb" /debug /machine:I386 /def:".\SIReportSample.def" /out:"..\..\..\..\dll\common\SIReportSample.dll" /implib:"$(OUTDIR)\SIReportSample.lib" /pdbtype:sept 
DEF_FILE= \
	".\SIReportSample.def"
LINK32_OBJS= \
	"$(INTDIR)\DlgSIReportSample.obj" \
	"$(INTDIR)\SIReportSample.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SIReportSample.res"

"..\..\..\..\dll\common\SIReportSample.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("SIReportSample.dep")
!INCLUDE "SIReportSample.dep"
!ELSE 
!MESSAGE Warning: cannot find "SIReportSample.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SIReportSample - Win32 Release" || "$(CFG)" == "SIReportSample - Win32 Debug"
SOURCE=.\DlgSIReportSample.cpp

"$(INTDIR)\DlgSIReportSample.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportSample.pch"


SOURCE=.\SIReportSample.cpp

"$(INTDIR)\SIReportSample.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportSample.pch"


SOURCE=.\SIReportSample.rc

"$(INTDIR)\SIReportSample.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "SIReportSample - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\SIReportSample.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SIReportSample.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SIReportSample - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\SIReportSample.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SIReportSample.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

