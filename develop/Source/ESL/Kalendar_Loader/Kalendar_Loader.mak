# Microsoft Developer Studio Generated NMAKE File, Based on Kalendar_Loader.dsp
!IF "$(CFG)" == ""
CFG=Kalendar_Loader - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Kalendar_Loader - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Kalendar_Loader - Win32 Release" && "$(CFG)" != "Kalendar_Loader - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Kalendar_Loader.mak" CFG="Kalendar_Loader - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Kalendar_Loader - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Kalendar_Loader - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "Kalendar_Loader - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Kalendar_Loader.exe"


CLEAN :
	-@erase "$(INTDIR)\Kalendar_Loader.obj"
	-@erase "$(INTDIR)\Kalendar_Loader.pch"
	-@erase "$(INTDIR)\Kalendar_Loader.res"
	-@erase "$(INTDIR)\Kalendar_LoaderDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Kalendar_Loader.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Kalendar_Loader.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Kalendar_Loader.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Kalendar_Loader.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Kalendar_Loader.pdb" /machine:I386 /out:"$(OUTDIR)\Kalendar_Loader.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Kalendar_Loader.obj" \
	"$(INTDIR)\Kalendar_LoaderDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Kalendar_Loader.res"

"$(OUTDIR)\Kalendar_Loader.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Kalendar_Loader - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\..\bin\Kalendar_Loader.exe"


CLEAN :
	-@erase "$(INTDIR)\Kalendar_Loader.obj"
	-@erase "$(INTDIR)\Kalendar_Loader.pch"
	-@erase "$(INTDIR)\Kalendar_Loader.res"
	-@erase "$(INTDIR)\Kalendar_LoaderDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Kalendar_Loader.pdb"
	-@erase "..\..\..\..\bin\Kalendar_Loader.exe"
	-@erase "..\..\..\..\bin\Kalendar_Loader.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Kalendar_Loader.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Kalendar_Loader.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Kalendar_Loader.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Kalendar_Loader.pdb" /debug /machine:I386 /out:"..\..\..\..\bin\Kalendar_Loader.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Kalendar_Loader.obj" \
	"$(INTDIR)\Kalendar_LoaderDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Kalendar_Loader.res"

"..\..\..\..\bin\Kalendar_Loader.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("Kalendar_Loader.dep")
!INCLUDE "Kalendar_Loader.dep"
!ELSE 
!MESSAGE Warning: cannot find "Kalendar_Loader.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Kalendar_Loader - Win32 Release" || "$(CFG)" == "Kalendar_Loader - Win32 Debug"
SOURCE=.\Kalendar_Loader.cpp

"$(INTDIR)\Kalendar_Loader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Kalendar_Loader.pch"


SOURCE=.\Kalendar_Loader.rc

"$(INTDIR)\Kalendar_Loader.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Kalendar_LoaderDlg.cpp

"$(INTDIR)\Kalendar_LoaderDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Kalendar_Loader.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Kalendar_Loader - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Kalendar_Loader.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Kalendar_Loader.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Kalendar_Loader - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Kalendar_Loader.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Kalendar_Loader.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

