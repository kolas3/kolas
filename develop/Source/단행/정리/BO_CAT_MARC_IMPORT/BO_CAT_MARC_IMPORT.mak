# Microsoft Developer Studio Generated NMAKE File, Based on CAT_MARC_IMPORT.dsp
!IF "$(CFG)" == ""
CFG=CAT_MARC_IMPORT - Win32 Debug
!MESSAGE No configuration specified. Defaulting to CAT_MARC_IMPORT - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CAT_MARC_IMPORT - Win32 Release" && "$(CFG)" != "CAT_MARC_IMPORT - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CAT_MARC_IMPORT.mak" CFG="CAT_MARC_IMPORT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CAT_MARC_IMPORT - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CAT_MARC_IMPORT - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "CAT_MARC_IMPORT - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\CAT_MARC_IMPORT.dll"


CLEAN :
	-@erase "$(INTDIR)\CAT_MARC_IMPORT.obj"
	-@erase "$(INTDIR)\CAT_MARC_IMPORT.pch"
	-@erase "$(INTDIR)\CAT_MARC_IMPORT.res"
	-@erase "$(INTDIR)\MarcEditDlg.obj"
	-@erase "$(INTDIR)\MarcImportDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TextProgressCtrl.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\CAT_MARC_IMPORT.dll"
	-@erase "$(OUTDIR)\CAT_MARC_IMPORT.exp"
	-@erase "$(OUTDIR)\CAT_MARC_IMPORT.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\CAT_MARC_IMPORT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\CAT_MARC_IMPORT.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CAT_MARC_IMPORT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\CAT_MARC_IMPORT.pdb" /machine:I386 /def:".\CAT_MARC_IMPORT.def" /out:"$(OUTDIR)\CAT_MARC_IMPORT.dll" /implib:"$(OUTDIR)\CAT_MARC_IMPORT.lib" 
DEF_FILE= \
	".\CAT_MARC_IMPORT.def"
LINK32_OBJS= \
	"$(INTDIR)\CAT_MARC_IMPORT.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\CAT_MARC_IMPORT.res" \
	"$(INTDIR)\MarcImportDlg.obj" \
	"$(INTDIR)\TextProgressCtrl.obj" \
	"$(INTDIR)\MarcEditDlg.obj"

"$(OUTDIR)\CAT_MARC_IMPORT.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CAT_MARC_IMPORT - Win32 Debug"

OUTDIR=.\../../../lib
INTDIR=.\Debug

ALL : "..\..\..\..\dll\COMMON\CAT_MARC_IMPORT.dll"


CLEAN :
	-@erase "$(INTDIR)\CAT_MARC_IMPORT.obj"
	-@erase "$(INTDIR)\CAT_MARC_IMPORT.pch"
	-@erase "$(INTDIR)\CAT_MARC_IMPORT.res"
	-@erase "$(INTDIR)\MarcEditDlg.obj"
	-@erase "$(INTDIR)\MarcImportDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TextProgressCtrl.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\CAT_MARC_IMPORT.exp"
	-@erase "$(OUTDIR)\CAT_MARC_IMPORT.lib"
	-@erase "$(OUTDIR)\CAT_MARC_IMPORT.pdb"
	-@erase "..\..\..\..\dll\COMMON\CAT_MARC_IMPORT.dll"
	-@erase "..\..\..\..\dll\COMMON\CAT_MARC_IMPORT.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\CAT_MARC_IMPORT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\CAT_MARC_IMPORT.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CAT_MARC_IMPORT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ESL.lib MarcAdjust.lib MarcAdjustApi.lib MarcToolkit.lib MarcEdit.lib IndexToolKit.lib BO_CAT_PROC_KEYWORD.lib K2UP_Common.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\CAT_MARC_IMPORT.pdb" /debug /machine:I386 /def:".\CAT_MARC_IMPORT.def" /out:"..\..\..\..\dll\COMMON\CAT_MARC_IMPORT.dll" /implib:"$(OUTDIR)\CAT_MARC_IMPORT.lib" /pdbtype:sept 
DEF_FILE= \
	".\CAT_MARC_IMPORT.def"
LINK32_OBJS= \
	"$(INTDIR)\CAT_MARC_IMPORT.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\CAT_MARC_IMPORT.res" \
	"$(INTDIR)\MarcImportDlg.obj" \
	"$(INTDIR)\TextProgressCtrl.obj" \
	"$(INTDIR)\MarcEditDlg.obj"

"..\..\..\..\dll\COMMON\CAT_MARC_IMPORT.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("CAT_MARC_IMPORT.dep")
!INCLUDE "CAT_MARC_IMPORT.dep"
!ELSE 
!MESSAGE Warning: cannot find "CAT_MARC_IMPORT.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CAT_MARC_IMPORT - Win32 Release" || "$(CFG)" == "CAT_MARC_IMPORT - Win32 Debug"
SOURCE=.\CAT_MARC_IMPORT.cpp

"$(INTDIR)\CAT_MARC_IMPORT.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_MARC_IMPORT.pch"


SOURCE=.\CAT_MARC_IMPORT.rc

"$(INTDIR)\CAT_MARC_IMPORT.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\MarcEditDlg.cpp

"$(INTDIR)\MarcEditDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_MARC_IMPORT.pch"


SOURCE=.\MarcImportDlg.cpp

"$(INTDIR)\MarcImportDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_MARC_IMPORT.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "CAT_MARC_IMPORT - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\CAT_MARC_IMPORT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\CAT_MARC_IMPORT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CAT_MARC_IMPORT - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\CAT_MARC_IMPORT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\CAT_MARC_IMPORT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TextProgressCtrl.cpp

"$(INTDIR)\TextProgressCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_MARC_IMPORT.pch"



!ENDIF 

