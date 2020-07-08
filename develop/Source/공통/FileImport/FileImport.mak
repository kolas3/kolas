# Microsoft Developer Studio Generated NMAKE File, Based on FileImport.dsp
!IF "$(CFG)" == ""
CFG=FileImport - Win32 Debug
!MESSAGE No configuration specified. Defaulting to FileImport - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "FileImport - Win32 Release" && "$(CFG)" != "FileImport - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FileImport.mak" CFG="FileImport - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FileImport - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FileImport - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "FileImport - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\FileImport.dll"


CLEAN :
	-@erase "$(INTDIR)\FileImport.obj"
	-@erase "$(INTDIR)\FileImport.pch"
	-@erase "$(INTDIR)\FileImport.res"
	-@erase "$(INTDIR)\FileImportDlg.obj"
	-@erase "$(INTDIR)\MarcEditDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TextProgressCtrl.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\FileImport.dll"
	-@erase "$(OUTDIR)\FileImport.exp"
	-@erase "$(OUTDIR)\FileImport.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\FileImport.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\FileImport.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\FileImport.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\FileImport.pdb" /machine:I386 /def:".\FileImport.def" /out:"$(OUTDIR)\FileImport.dll" /implib:"$(OUTDIR)\FileImport.lib" 
DEF_FILE= \
	".\FileImport.def"
LINK32_OBJS= \
	"$(INTDIR)\FileImport.obj" \
	"$(INTDIR)\FileImportDlg.obj" \
	"$(INTDIR)\MarcEditDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TextProgressCtrl.obj" \
	"$(INTDIR)\FileImport.res"

"$(OUTDIR)\FileImport.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "FileImport - Win32 Debug"

OUTDIR=.\../../../lib
INTDIR=.\Debug

ALL : "..\..\..\..\dll\COMMON\FileImport.dll"


CLEAN :
	-@erase "$(INTDIR)\FileImport.obj"
	-@erase "$(INTDIR)\FileImport.pch"
	-@erase "$(INTDIR)\FileImport.res"
	-@erase "$(INTDIR)\FileImportDlg.obj"
	-@erase "$(INTDIR)\MarcEditDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TextProgressCtrl.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\FileImport.exp"
	-@erase "$(OUTDIR)\FileImport.lib"
	-@erase "$(OUTDIR)\FileImport.pdb"
	-@erase "..\..\..\..\dll\COMMON\FileImport.dll"
	-@erase "..\..\..\..\dll\COMMON\FileImport.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\FileImport.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\FileImport.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\FileImport.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ESL.lib MarcEdit.lib MarcToolKit.lib BO_SPECIES_API.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\FileImport.pdb" /debug /machine:I386 /def:".\FileImport.def" /out:"..\..\..\..\dll\COMMON\FileImport.dll" /implib:"$(OUTDIR)\FileImport.lib" /pdbtype:sept 
DEF_FILE= \
	".\FileImport.def"
LINK32_OBJS= \
	"$(INTDIR)\FileImport.obj" \
	"$(INTDIR)\FileImportDlg.obj" \
	"$(INTDIR)\MarcEditDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TextProgressCtrl.obj" \
	"$(INTDIR)\FileImport.res"

"..\..\..\..\dll\COMMON\FileImport.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("FileImport.dep")
!INCLUDE "FileImport.dep"
!ELSE 
!MESSAGE Warning: cannot find "FileImport.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "FileImport - Win32 Release" || "$(CFG)" == "FileImport - Win32 Debug"
SOURCE=.\FileImport.cpp

"$(INTDIR)\FileImport.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FileImport.pch"


SOURCE=.\FileImport.rc

"$(INTDIR)\FileImport.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\FileImportDlg.cpp

"$(INTDIR)\FileImportDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FileImport.pch"


SOURCE=.\MarcEditDlg.cpp

"$(INTDIR)\MarcEditDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FileImport.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "FileImport - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\FileImport.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\FileImport.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "FileImport - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\FileImport.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\FileImport.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TextProgressCtrl.cpp

"$(INTDIR)\TextProgressCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FileImport.pch"



!ENDIF 

