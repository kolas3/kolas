# Microsoft Developer Studio Generated NMAKE File, Based on BO_SPECIES_FORM.dsp
!IF "$(CFG)" == ""
CFG=BO_SPECIES_FORM - Win32 Debug
!MESSAGE No configuration specified. Defaulting to BO_SPECIES_FORM - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "BO_SPECIES_FORM - Win32 Release" && "$(CFG)" != "BO_SPECIES_FORM - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_SPECIES_FORM.mak" CFG="BO_SPECIES_FORM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_SPECIES_FORM - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BO_SPECIES_FORM - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\BO_SPECIES_FORM.dll"


CLEAN :
	-@erase "$(INTDIR)\BO_SPECIES_FORM.obj"
	-@erase "$(INTDIR)\BO_SPECIES_FORM.pch"
	-@erase "$(INTDIR)\BO_SPECIES_FORM.res"
	-@erase "$(INTDIR)\BOAppendixDlg.obj"
	-@erase "$(INTDIR)\BOFormApi.obj"
	-@erase "$(INTDIR)\BOISBNAdditionDlg.obj"
	-@erase "$(INTDIR)\BOMarcEditDlg.obj"
	-@erase "$(INTDIR)\BOSpeciesDlg.obj"
	-@erase "$(INTDIR)\BOVolumeDlg.obj"
	-@erase "$(INTDIR)\NoteCtrl.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\BO_SPECIES_FORM.dll"
	-@erase "$(OUTDIR)\BO_SPECIES_FORM.exp"
	-@erase "$(OUTDIR)\BO_SPECIES_FORM.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_SPECIES_FORM.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_SPECIES_FORM.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_SPECIES_FORM.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\BO_SPECIES_FORM.pdb" /machine:I386 /def:".\BO_SPECIES_FORM.def" /out:"$(OUTDIR)\BO_SPECIES_FORM.dll" /implib:"$(OUTDIR)\BO_SPECIES_FORM.lib" 
DEF_FILE= \
	".\BO_SPECIES_FORM.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_SPECIES_FORM.obj" \
	"$(INTDIR)\BOAppendixDlg.obj" \
	"$(INTDIR)\BOFormApi.obj" \
	"$(INTDIR)\BOISBNAdditionDlg.obj" \
	"$(INTDIR)\BOMarcEditDlg.obj" \
	"$(INTDIR)\BOSpeciesDlg.obj" \
	"$(INTDIR)\BOVolumeDlg.obj" \
	"$(INTDIR)\NoteCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_SPECIES_FORM.res"

"$(OUTDIR)\BO_SPECIES_FORM.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Debug"

OUTDIR=.\../../../lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\../../../lib/
# End Custom Macros

ALL : "..\..\..\..\DLL\COMMON\BO_SPECIES_FORM.dll" "$(OUTDIR)\BO_SPECIES_FORM.bsc"


CLEAN :
	-@erase "$(INTDIR)\BO_SPECIES_FORM.obj"
	-@erase "$(INTDIR)\BO_SPECIES_FORM.pch"
	-@erase "$(INTDIR)\BO_SPECIES_FORM.res"
	-@erase "$(INTDIR)\BO_SPECIES_FORM.sbr"
	-@erase "$(INTDIR)\BOAppendixDlg.obj"
	-@erase "$(INTDIR)\BOAppendixDlg.sbr"
	-@erase "$(INTDIR)\BOFormApi.obj"
	-@erase "$(INTDIR)\BOFormApi.sbr"
	-@erase "$(INTDIR)\BOISBNAdditionDlg.obj"
	-@erase "$(INTDIR)\BOISBNAdditionDlg.sbr"
	-@erase "$(INTDIR)\BOMarcEditDlg.obj"
	-@erase "$(INTDIR)\BOMarcEditDlg.sbr"
	-@erase "$(INTDIR)\BOSpeciesDlg.obj"
	-@erase "$(INTDIR)\BOSpeciesDlg.sbr"
	-@erase "$(INTDIR)\BOVolumeDlg.obj"
	-@erase "$(INTDIR)\BOVolumeDlg.sbr"
	-@erase "$(INTDIR)\NoteCtrl.obj"
	-@erase "$(INTDIR)\NoteCtrl.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\BO_SPECIES_FORM.bsc"
	-@erase "$(OUTDIR)\BO_SPECIES_FORM.exp"
	-@erase "$(OUTDIR)\BO_SPECIES_FORM.lib"
	-@erase "$(OUTDIR)\BO_SPECIES_FORM.pdb"
	-@erase "..\..\..\..\DLL\COMMON\BO_SPECIES_FORM.dll"
	-@erase "..\..\..\..\DLL\COMMON\BO_SPECIES_FORM.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\BO_SPECIES_FORM.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_SPECIES_FORM.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_SPECIES_FORM.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BO_SPECIES_FORM.sbr" \
	"$(INTDIR)\BOAppendixDlg.sbr" \
	"$(INTDIR)\BOFormApi.sbr" \
	"$(INTDIR)\BOISBNAdditionDlg.sbr" \
	"$(INTDIR)\BOMarcEditDlg.sbr" \
	"$(INTDIR)\BOSpeciesDlg.sbr" \
	"$(INTDIR)\BOVolumeDlg.sbr" \
	"$(INTDIR)\NoteCtrl.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\BO_SPECIES_FORM.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=esl.lib marcedit.lib BO_SPECIES_API.lib BO_MARC_IMPORT.lib Duplicate_Data_Search.lib K2UP_Common.lib Light_GatherDataSearch.lib UI_SEARCH_DLG.lib SIReportData.lib SIReportMgr.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\BO_SPECIES_FORM.pdb" /debug /machine:I386 /def:".\BO_SPECIES_FORM.def" /out:"..\..\..\..\DLL\COMMON\BO_SPECIES_FORM.dll" /implib:"$(OUTDIR)\BO_SPECIES_FORM.lib" /pdbtype:sept 
DEF_FILE= \
	".\BO_SPECIES_FORM.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_SPECIES_FORM.obj" \
	"$(INTDIR)\BOAppendixDlg.obj" \
	"$(INTDIR)\BOFormApi.obj" \
	"$(INTDIR)\BOISBNAdditionDlg.obj" \
	"$(INTDIR)\BOMarcEditDlg.obj" \
	"$(INTDIR)\BOSpeciesDlg.obj" \
	"$(INTDIR)\BOVolumeDlg.obj" \
	"$(INTDIR)\NoteCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_SPECIES_FORM.res"

"..\..\..\..\DLL\COMMON\BO_SPECIES_FORM.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("BO_SPECIES_FORM.dep")
!INCLUDE "BO_SPECIES_FORM.dep"
!ELSE 
!MESSAGE Warning: cannot find "BO_SPECIES_FORM.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "BO_SPECIES_FORM - Win32 Release" || "$(CFG)" == "BO_SPECIES_FORM - Win32 Debug"
SOURCE=.\BO_SPECIES_FORM.cpp

!IF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Release"


"$(INTDIR)\BO_SPECIES_FORM.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ELSEIF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Debug"


"$(INTDIR)\BO_SPECIES_FORM.obj"	"$(INTDIR)\BO_SPECIES_FORM.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ENDIF 

SOURCE=.\BO_SPECIES_FORM.rc

"$(INTDIR)\BO_SPECIES_FORM.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\BOAppendixDlg.cpp

!IF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Release"


"$(INTDIR)\BOAppendixDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ELSEIF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Debug"


"$(INTDIR)\BOAppendixDlg.obj"	"$(INTDIR)\BOAppendixDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ENDIF 

SOURCE=.\BOFormApi.cpp

!IF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Release"


"$(INTDIR)\BOFormApi.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ELSEIF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Debug"


"$(INTDIR)\BOFormApi.obj"	"$(INTDIR)\BOFormApi.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ENDIF 

SOURCE=.\BOISBNAdditionDlg.cpp

!IF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Release"


"$(INTDIR)\BOISBNAdditionDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ELSEIF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Debug"


"$(INTDIR)\BOISBNAdditionDlg.obj"	"$(INTDIR)\BOISBNAdditionDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ENDIF 

SOURCE=.\BOMarcEditDlg.cpp

!IF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Release"


"$(INTDIR)\BOMarcEditDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ELSEIF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Debug"


"$(INTDIR)\BOMarcEditDlg.obj"	"$(INTDIR)\BOMarcEditDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ENDIF 

SOURCE=.\BOSpeciesDlg.cpp

!IF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Release"


"$(INTDIR)\BOSpeciesDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ELSEIF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Debug"


"$(INTDIR)\BOSpeciesDlg.obj"	"$(INTDIR)\BOSpeciesDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ENDIF 

SOURCE=.\BOVolumeDlg.cpp

!IF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Release"


"$(INTDIR)\BOVolumeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ELSEIF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Debug"


"$(INTDIR)\BOVolumeDlg.obj"	"$(INTDIR)\BOVolumeDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ENDIF 

SOURCE=.\NoteCtrl.cpp

!IF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Release"


"$(INTDIR)\NoteCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ELSEIF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Debug"


"$(INTDIR)\NoteCtrl.obj"	"$(INTDIR)\NoteCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_SPECIES_FORM.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_SPECIES_FORM.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\BO_SPECIES_FORM.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "BO_SPECIES_FORM - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\BO_SPECIES_FORM.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\BO_SPECIES_FORM.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

