# Microsoft Developer Studio Generated NMAKE File, Based on MarcAdjustApi.dsp
!IF "$(CFG)" == ""
CFG=MarcAdjustApi - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MarcAdjustApi - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MarcAdjustApi - Win32 Release" && "$(CFG)" != "MarcAdjustApi - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MarcAdjustApi.mak" CFG="MarcAdjustApi - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MarcAdjustApi - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MarcAdjustApi - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "MarcAdjustApi - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MarcAdjustApi.dll"


CLEAN :
	-@erase "$(INTDIR)\CatApi.obj"
	-@erase "$(INTDIR)\MarcAdjustApi.obj"
	-@erase "$(INTDIR)\MarcAdjustApi.pch"
	-@erase "$(INTDIR)\MarcAdjustApi.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MarcAdjustApi.dll"
	-@erase "$(OUTDIR)\MarcAdjustApi.exp"
	-@erase "$(OUTDIR)\MarcAdjustApi.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcAdjustApi.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MarcAdjustApi.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MarcAdjustApi.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\MarcAdjustApi.pdb" /machine:I386 /def:".\MarcAdjustApi.def" /out:"$(OUTDIR)\MarcAdjustApi.dll" /implib:"$(OUTDIR)\MarcAdjustApi.lib" 
DEF_FILE= \
	".\MarcAdjustApi.def"
LINK32_OBJS= \
	"$(INTDIR)\MarcAdjustApi.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MarcAdjustApi.res" \
	"$(INTDIR)\CatApi.obj"

"$(OUTDIR)\MarcAdjustApi.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MarcAdjustApi - Win32 Debug"

OUTDIR=.\../../../lib
INTDIR=.\Debug

ALL : "..\..\..\..\dll\COMMON\MarcAdjustApi.dll"


CLEAN :
	-@erase "$(INTDIR)\CatApi.obj"
	-@erase "$(INTDIR)\MarcAdjustApi.obj"
	-@erase "$(INTDIR)\MarcAdjustApi.pch"
	-@erase "$(INTDIR)\MarcAdjustApi.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\MarcAdjustApi.exp"
	-@erase "$(OUTDIR)\MarcAdjustApi.lib"
	-@erase "$(OUTDIR)\MarcAdjustApi.pdb"
	-@erase "..\..\..\..\dll\COMMON\MarcAdjustApi.dll"
	-@erase "..\..\..\..\dll\COMMON\MarcAdjustApi.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcAdjustApi.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MarcAdjustApi.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MarcAdjustApi.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=esl.lib IndexToolKit.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\MarcAdjustApi.pdb" /debug /machine:I386 /def:".\MarcAdjustApi.def" /out:"..\..\..\..\dll\COMMON\MarcAdjustApi.dll" /implib:"$(OUTDIR)\MarcAdjustApi.lib" /pdbtype:sept 
DEF_FILE= \
	".\MarcAdjustApi.def"
LINK32_OBJS= \
	"$(INTDIR)\MarcAdjustApi.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MarcAdjustApi.res" \
	"$(INTDIR)\CatApi.obj"

"..\..\..\..\dll\COMMON\MarcAdjustApi.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("MarcAdjustApi.dep")
!INCLUDE "MarcAdjustApi.dep"
!ELSE 
!MESSAGE Warning: cannot find "MarcAdjustApi.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MarcAdjustApi - Win32 Release" || "$(CFG)" == "MarcAdjustApi - Win32 Debug"
SOURCE=.\CatApi.cpp

"$(INTDIR)\CatApi.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjustApi.pch"


SOURCE=.\MarcAdjustApi.cpp

"$(INTDIR)\MarcAdjustApi.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjustApi.pch"


SOURCE=.\MarcAdjustApi.rc

"$(INTDIR)\MarcAdjustApi.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "MarcAdjustApi - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcAdjustApi.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MarcAdjustApi.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "MarcAdjustApi - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcAdjustApi.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MarcAdjustApi.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

