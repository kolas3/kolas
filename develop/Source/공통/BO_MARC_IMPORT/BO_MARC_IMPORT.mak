# Microsoft Developer Studio Generated NMAKE File, Based on BO_MARC_IMPORT.dsp
!IF "$(CFG)" == ""
CFG=BO_MARC_IMPORT - Win32 Debug
!MESSAGE No configuration specified. Defaulting to BO_MARC_IMPORT - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "BO_MARC_IMPORT - Win32 Release" && "$(CFG)" != "BO_MARC_IMPORT - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_MARC_IMPORT.mak" CFG="BO_MARC_IMPORT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_MARC_IMPORT - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BO_MARC_IMPORT - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "BO_MARC_IMPORT - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\BO_MARC_IMPORT.dll"


CLEAN :
	-@erase "$(INTDIR)\BO_MARC_IMPORT.obj"
	-@erase "$(INTDIR)\BO_MARC_IMPORT.pch"
	-@erase "$(INTDIR)\BO_MARC_IMPORT.res"
	-@erase "$(INTDIR)\MARC_IMPORT.obj"
	-@erase "$(INTDIR)\MARC_VIEW.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\BO_MARC_IMPORT.dll"
	-@erase "$(OUTDIR)\BO_MARC_IMPORT.exp"
	-@erase "$(OUTDIR)\BO_MARC_IMPORT.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_MARC_IMPORT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_MARC_IMPORT.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_MARC_IMPORT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\BO_MARC_IMPORT.pdb" /machine:I386 /def:".\BO_MARC_IMPORT.def" /out:"$(OUTDIR)\BO_MARC_IMPORT.dll" /implib:"$(OUTDIR)\BO_MARC_IMPORT.lib" 
DEF_FILE= \
	".\BO_MARC_IMPORT.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_MARC_IMPORT.obj" \
	"$(INTDIR)\MARC_IMPORT.obj" \
	"$(INTDIR)\MARC_VIEW.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_MARC_IMPORT.res"

"$(OUTDIR)\BO_MARC_IMPORT.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "BO_MARC_IMPORT - Win32 Debug"

OUTDIR=.\../../../lib
INTDIR=.\Debug

ALL : "..\..\..\..\DLL\COMMON\BO_MARC_IMPORT.dll"


CLEAN :
	-@erase "$(INTDIR)\BO_MARC_IMPORT.obj"
	-@erase "$(INTDIR)\BO_MARC_IMPORT.pch"
	-@erase "$(INTDIR)\BO_MARC_IMPORT.res"
	-@erase "$(INTDIR)\MARC_IMPORT.obj"
	-@erase "$(INTDIR)\MARC_VIEW.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\BO_MARC_IMPORT.exp"
	-@erase "$(OUTDIR)\BO_MARC_IMPORT.lib"
	-@erase "$(OUTDIR)\BO_MARC_IMPORT.pdb"
	-@erase "..\..\..\..\DLL\COMMON\BO_MARC_IMPORT.dll"
	-@erase "..\..\..\..\DLL\COMMON\BO_MARC_IMPORT.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_MARC_IMPORT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_MARC_IMPORT.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_MARC_IMPORT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=esl.lib marcedit.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\BO_MARC_IMPORT.pdb" /debug /machine:I386 /def:".\BO_MARC_IMPORT.def" /out:"..\..\..\..\DLL\COMMON\BO_MARC_IMPORT.dll" /implib:"$(OUTDIR)\BO_MARC_IMPORT.lib" /pdbtype:sept 
DEF_FILE= \
	".\BO_MARC_IMPORT.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_MARC_IMPORT.obj" \
	"$(INTDIR)\MARC_IMPORT.obj" \
	"$(INTDIR)\MARC_VIEW.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_MARC_IMPORT.res"

"..\..\..\..\DLL\COMMON\BO_MARC_IMPORT.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("BO_MARC_IMPORT.dep")
!INCLUDE "BO_MARC_IMPORT.dep"
!ELSE 
!MESSAGE Warning: cannot find "BO_MARC_IMPORT.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "BO_MARC_IMPORT - Win32 Release" || "$(CFG)" == "BO_MARC_IMPORT - Win32 Debug"
SOURCE=.\BO_MARC_IMPORT.cpp

"$(INTDIR)\BO_MARC_IMPORT.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_MARC_IMPORT.pch"


SOURCE=.\BO_MARC_IMPORT.rc

"$(INTDIR)\BO_MARC_IMPORT.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\MARC_IMPORT.cpp

"$(INTDIR)\MARC_IMPORT.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_MARC_IMPORT.pch"


SOURCE=.\MARC_VIEW.cpp

"$(INTDIR)\MARC_VIEW.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_MARC_IMPORT.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "BO_MARC_IMPORT - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_MARC_IMPORT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\BO_MARC_IMPORT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "BO_MARC_IMPORT - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_MARC_IMPORT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\BO_MARC_IMPORT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

