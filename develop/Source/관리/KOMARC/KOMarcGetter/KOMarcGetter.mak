# Microsoft Developer Studio Generated NMAKE File, Based on KOMarcGetter.dsp
!IF "$(CFG)" == ""
CFG=KOMarcGetter - Win32 Debug
!MESSAGE No configuration specified. Defaulting to KOMarcGetter - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "KOMarcGetter - Win32 Release" && "$(CFG)" != "KOMarcGetter - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KOMarcGetter.mak" CFG="KOMarcGetter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KOMarcGetter - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "KOMarcGetter - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "KOMarcGetter - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\KOMarcGetter.dll"


CLEAN :
	-@erase "$(INTDIR)\KOMarcGetter.obj"
	-@erase "$(INTDIR)\KOMarcGetter.pch"
	-@erase "$(INTDIR)\KOMarcGetter.res"
	-@erase "$(INTDIR)\KOMarcGetterDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\KOMarcGetter.dll"
	-@erase "$(OUTDIR)\KOMarcGetter.exp"
	-@erase "$(OUTDIR)\KOMarcGetter.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\KOMarcGetter.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\KOMarcGetter.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\KOMarcGetter.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\KOMarcGetter.pdb" /machine:I386 /def:".\KOMarcGetter.def" /out:"$(OUTDIR)\KOMarcGetter.dll" /implib:"$(OUTDIR)\KOMarcGetter.lib" 
DEF_FILE= \
	".\KOMarcGetter.def"
LINK32_OBJS= \
	"$(INTDIR)\KOMarcGetter.obj" \
	"$(INTDIR)\KOMarcGetterDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\KOMarcGetter.res"

"$(OUTDIR)\KOMarcGetter.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "KOMarcGetter - Win32 Debug"

OUTDIR=.\..\..\..\..\lib
INTDIR=.\Debug

ALL : "..\..\..\..\..\dll\mng\KOMarcGetter.dll"


CLEAN :
	-@erase "$(INTDIR)\KOMarcGetter.obj"
	-@erase "$(INTDIR)\KOMarcGetter.pch"
	-@erase "$(INTDIR)\KOMarcGetter.res"
	-@erase "$(INTDIR)\KOMarcGetterDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\KOMarcGetter.exp"
	-@erase "$(OUTDIR)\KOMarcGetter.lib"
	-@erase "$(OUTDIR)\KOMarcGetter.pdb"
	-@erase "..\..\..\..\..\dll\mng\KOMarcGetter.dll"
	-@erase "..\..\..\..\..\dll\mng\KOMarcGetter.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\KOMarcGetter.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\KOMarcGetter.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\KOMarcGetter.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=esl.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\KOMarcGetter.pdb" /debug /machine:I386 /def:".\KOMarcGetter.def" /out:"..\..\..\..\..\dll\mng\KOMarcGetter.dll" /implib:"$(OUTDIR)\KOMarcGetter.lib" /pdbtype:sept 
DEF_FILE= \
	".\KOMarcGetter.def"
LINK32_OBJS= \
	"$(INTDIR)\KOMarcGetter.obj" \
	"$(INTDIR)\KOMarcGetterDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\KOMarcGetter.res"

"..\..\..\..\..\dll\mng\KOMarcGetter.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("KOMarcGetter.dep")
!INCLUDE "KOMarcGetter.dep"
!ELSE 
!MESSAGE Warning: cannot find "KOMarcGetter.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "KOMarcGetter - Win32 Release" || "$(CFG)" == "KOMarcGetter - Win32 Debug"
SOURCE=.\KOMarcGetter.cpp

"$(INTDIR)\KOMarcGetter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOMarcGetter.pch"


SOURCE=.\KOMarcGetter.rc

"$(INTDIR)\KOMarcGetter.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\KOMarcGetterDlg.cpp

"$(INTDIR)\KOMarcGetterDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOMarcGetter.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "KOMarcGetter - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\KOMarcGetter.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\KOMarcGetter.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "KOMarcGetter - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\KOMarcGetter.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\KOMarcGetter.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

