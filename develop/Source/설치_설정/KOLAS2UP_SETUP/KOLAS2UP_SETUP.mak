# Microsoft Developer Studio Generated NMAKE File, Based on KOLAS2UP_SETUP.dsp
!IF "$(CFG)" == ""
CFG=KOLAS2UP_SETUP - Win32 Debug
!MESSAGE No configuration specified. Defaulting to KOLAS2UP_SETUP - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "KOLAS2UP_SETUP - Win32 Release" && "$(CFG)" != "KOLAS2UP_SETUP - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KOLAS2UP_SETUP.mak" CFG="KOLAS2UP_SETUP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KOLAS2UP_SETUP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "KOLAS2UP_SETUP - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "KOLAS2UP_SETUP - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\KOLAS2UP_SETUP.exe"


CLEAN :
	-@erase "$(INTDIR)\ColorPicker.obj"
	-@erase "$(INTDIR)\ColorPopup.obj"
	-@erase "$(INTDIR)\ConnectSetupPage.obj"
	-@erase "$(INTDIR)\GridSetupPage.obj"
	-@erase "$(INTDIR)\KOLAS2UP_SETUP.obj"
	-@erase "$(INTDIR)\KOLAS2UP_SETUP.pch"
	-@erase "$(INTDIR)\KOLAS2UP_SETUP.res"
	-@erase "$(INTDIR)\SetupMainSheet.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TnsSetupPage.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\KOLAS2UP_SETUP.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\KOLAS2UP_SETUP.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\KOLAS2UP_SETUP.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\KOLAS2UP_SETUP.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\KOLAS2UP_SETUP.pdb" /machine:I386 /out:"$(OUTDIR)\KOLAS2UP_SETUP.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ColorPicker.obj" \
	"$(INTDIR)\ColorPopup.obj" \
	"$(INTDIR)\ConnectSetupPage.obj" \
	"$(INTDIR)\GridSetupPage.obj" \
	"$(INTDIR)\KOLAS2UP_SETUP.obj" \
	"$(INTDIR)\SetupMainSheet.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TnsSetupPage.obj" \
	"$(INTDIR)\KOLAS2UP_SETUP.res"

"$(OUTDIR)\KOLAS2UP_SETUP.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "KOLAS2UP_SETUP - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\..\BIN\KOLAS2UP_SETUP.exe"


CLEAN :
	-@erase "$(INTDIR)\ColorPicker.obj"
	-@erase "$(INTDIR)\ColorPopup.obj"
	-@erase "$(INTDIR)\ConnectSetupPage.obj"
	-@erase "$(INTDIR)\GridSetupPage.obj"
	-@erase "$(INTDIR)\KOLAS2UP_SETUP.obj"
	-@erase "$(INTDIR)\KOLAS2UP_SETUP.pch"
	-@erase "$(INTDIR)\KOLAS2UP_SETUP.res"
	-@erase "$(INTDIR)\SetupMainSheet.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TnsSetupPage.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\KOLAS2UP_SETUP.pdb"
	-@erase "..\..\..\..\BIN\KOLAS2UP_SETUP.exe"
	-@erase "..\..\..\..\BIN\KOLAS2UP_SETUP.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\KOLAS2UP_SETUP.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\KOLAS2UP_SETUP.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\KOLAS2UP_SETUP.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ocimgr.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\KOLAS2UP_SETUP.pdb" /debug /machine:I386 /out:"..\..\..\..\BIN\KOLAS2UP_SETUP.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ColorPicker.obj" \
	"$(INTDIR)\ColorPopup.obj" \
	"$(INTDIR)\ConnectSetupPage.obj" \
	"$(INTDIR)\GridSetupPage.obj" \
	"$(INTDIR)\KOLAS2UP_SETUP.obj" \
	"$(INTDIR)\SetupMainSheet.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TnsSetupPage.obj" \
	"$(INTDIR)\KOLAS2UP_SETUP.res"

"..\..\..\..\BIN\KOLAS2UP_SETUP.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("KOLAS2UP_SETUP.dep")
!INCLUDE "KOLAS2UP_SETUP.dep"
!ELSE 
!MESSAGE Warning: cannot find "KOLAS2UP_SETUP.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "KOLAS2UP_SETUP - Win32 Release" || "$(CFG)" == "KOLAS2UP_SETUP - Win32 Debug"
SOURCE=.\ColorPicker.cpp

"$(INTDIR)\ColorPicker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS2UP_SETUP.pch"


SOURCE=.\ColorPopup.cpp

"$(INTDIR)\ColorPopup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS2UP_SETUP.pch"


SOURCE=.\ConnectSetupPage.cpp

"$(INTDIR)\ConnectSetupPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS2UP_SETUP.pch"


SOURCE=.\GridSetupPage.cpp

"$(INTDIR)\GridSetupPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS2UP_SETUP.pch"


SOURCE=.\KOLAS2UP_SETUP.cpp

"$(INTDIR)\KOLAS2UP_SETUP.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS2UP_SETUP.pch"


SOURCE=.\KOLAS2UP_SETUP.rc

"$(INTDIR)\KOLAS2UP_SETUP.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\SetupMainSheet.cpp

"$(INTDIR)\SetupMainSheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS2UP_SETUP.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "KOLAS2UP_SETUP - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\KOLAS2UP_SETUP.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\KOLAS2UP_SETUP.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "KOLAS2UP_SETUP - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\KOLAS2UP_SETUP.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\KOLAS2UP_SETUP.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TnsSetupPage.cpp

"$(INTDIR)\TnsSetupPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS2UP_SETUP.pch"



!ENDIF 

