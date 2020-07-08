# Microsoft Developer Studio Generated NMAKE File, Based on MLoader.dsp
!IF "$(CFG)" == ""
CFG=MLoader - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MLoader - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MLoader - Win32 Release" && "$(CFG)" != "MLoader - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MLoader.mak" CFG="MLoader - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MLoader - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MLoader - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "MLoader - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MLoader.exe"


CLEAN :
	-@erase "$(INTDIR)\MLoader.obj"
	-@erase "$(INTDIR)\MLoader.pch"
	-@erase "$(INTDIR)\MLoader.res"
	-@erase "$(INTDIR)\MLoaderDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MLoader.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\MLoader.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MLoader.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MLoader.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\MLoader.pdb" /machine:I386 /out:"$(OUTDIR)\MLoader.exe" 
LINK32_OBJS= \
	"$(INTDIR)\MLoader.obj" \
	"$(INTDIR)\MLoaderDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MLoader.res"

"$(OUTDIR)\MLoader.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MLoader - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\..\bin\MLoader.exe" "$(OUTDIR)\MLoader.bsc"


CLEAN :
	-@erase "$(INTDIR)\MLoader.obj"
	-@erase "$(INTDIR)\MLoader.pch"
	-@erase "$(INTDIR)\MLoader.res"
	-@erase "$(INTDIR)\MLoader.sbr"
	-@erase "$(INTDIR)\MLoaderDlg.obj"
	-@erase "$(INTDIR)\MLoaderDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\MLoader.bsc"
	-@erase "$(OUTDIR)\MLoader.pdb"
	-@erase "..\..\..\..\bin\MLoader.exe"
	-@erase "..\..\..\..\bin\MLoader.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\MLoader.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MLoader.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MLoader.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\MLoader.sbr" \
	"$(INTDIR)\MLoaderDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\MLoader.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\MLoader.pdb" /debug /machine:I386 /out:"..\..\..\..\bin\MLoader.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\MLoader.obj" \
	"$(INTDIR)\MLoaderDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MLoader.res"

"..\..\..\..\bin\MLoader.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("MLoader.dep")
!INCLUDE "MLoader.dep"
!ELSE 
!MESSAGE Warning: cannot find "MLoader.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MLoader - Win32 Release" || "$(CFG)" == "MLoader - Win32 Debug"
SOURCE=.\MLoader.cpp

!IF  "$(CFG)" == "MLoader - Win32 Release"


"$(INTDIR)\MLoader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MLoader.pch"


!ELSEIF  "$(CFG)" == "MLoader - Win32 Debug"


"$(INTDIR)\MLoader.obj"	"$(INTDIR)\MLoader.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MLoader.pch"


!ENDIF 

SOURCE=.\MLoader.rc

"$(INTDIR)\MLoader.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\MLoaderDlg.cpp

!IF  "$(CFG)" == "MLoader - Win32 Release"


"$(INTDIR)\MLoaderDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MLoader.pch"


!ELSEIF  "$(CFG)" == "MLoader - Win32 Debug"


"$(INTDIR)\MLoaderDlg.obj"	"$(INTDIR)\MLoaderDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MLoader.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "MLoader - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\MLoader.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MLoader.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "MLoader - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\MLoader.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\MLoader.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

