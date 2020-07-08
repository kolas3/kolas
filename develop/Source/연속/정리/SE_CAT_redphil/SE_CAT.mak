# Microsoft Developer Studio Generated NMAKE File, Based on SE_CAT.dsp
!IF "$(CFG)" == ""
CFG=SE_CAT - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SE_CAT - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SE_CAT - Win32 Release" && "$(CFG)" != "SE_CAT - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SE_CAT.mak" CFG="SE_CAT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SE_CAT - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SE_CAT - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "SE_CAT - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\SE_CAT.dll"


CLEAN :
	-@erase "$(INTDIR)\MainDefaultDlg.obj"
	-@erase "$(INTDIR)\SE_CAT.obj"
	-@erase "$(INTDIR)\SE_CAT.pch"
	-@erase "$(INTDIR)\SE_CAT.res"
	-@erase "$(INTDIR)\SE_CAT_SearchDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SE_CAT.dll"
	-@erase "$(OUTDIR)\SE_CAT.exp"
	-@erase "$(OUTDIR)\SE_CAT.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\SE_CAT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SE_CAT.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SE_CAT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\SE_CAT.pdb" /machine:I386 /def:".\SE_CAT.def" /out:"$(OUTDIR)\SE_CAT.dll" /implib:"$(OUTDIR)\SE_CAT.lib" 
DEF_FILE= \
	".\SE_CAT.def"
LINK32_OBJS= \
	"$(INTDIR)\MainDefaultDlg.obj" \
	"$(INTDIR)\SE_CAT.obj" \
	"$(INTDIR)\SE_CAT_SearchDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SE_CAT.res"

"$(OUTDIR)\SE_CAT.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SE_CAT - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\..\..\dll\serial\SE_CAT.dll" "$(OUTDIR)\SE_CAT.bsc"


CLEAN :
	-@erase "$(INTDIR)\MainDefaultDlg.obj"
	-@erase "$(INTDIR)\MainDefaultDlg.sbr"
	-@erase "$(INTDIR)\SE_CAT.obj"
	-@erase "$(INTDIR)\SE_CAT.pch"
	-@erase "$(INTDIR)\SE_CAT.res"
	-@erase "$(INTDIR)\SE_CAT.sbr"
	-@erase "$(INTDIR)\SE_CAT_SearchDlg.obj"
	-@erase "$(INTDIR)\SE_CAT_SearchDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SE_CAT.bsc"
	-@erase "$(OUTDIR)\SE_CAT.exp"
	-@erase "$(OUTDIR)\SE_CAT.lib"
	-@erase "$(OUTDIR)\SE_CAT.pdb"
	-@erase "..\..\..\..\..\dll\serial\SE_CAT.dll"
	-@erase "..\..\..\..\..\dll\serial\SE_CAT.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SE_CAT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SE_CAT.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SE_CAT.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\MainDefaultDlg.sbr" \
	"$(INTDIR)\SE_CAT.sbr" \
	"$(INTDIR)\SE_CAT_SearchDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\SE_CAT.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=esl.lib MarcAdjust.lib MarcAdjustApi.lib MarcEdit.lib IndexToolKit.lib K2UP_Common.lib SIReportData.lib SIReportMgr.lib Duplicate_Data_Search.lib BO_CAT_DUP_FREE.LIB BO_CAT_PROC.lib CAT_MARC_IMPORT.lib CAT_MARC_EXPORT.lib MarcToolkit.lib BO_ACQ_GROUP_MANAGER.lib Light_BO_CAT_CENTERUPLOAD.lib CAT_PRINT.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\SE_CAT.pdb" /debug /machine:I386 /def:".\SE_CAT.def" /out:"..\..\..\..\..\dll\serial\SE_CAT.dll" /implib:"$(OUTDIR)\SE_CAT.lib" /pdbtype:sept 
DEF_FILE= \
	".\SE_CAT.def"
LINK32_OBJS= \
	"$(INTDIR)\MainDefaultDlg.obj" \
	"$(INTDIR)\SE_CAT.obj" \
	"$(INTDIR)\SE_CAT_SearchDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SE_CAT.res"

"..\..\..\..\..\dll\serial\SE_CAT.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("SE_CAT.dep")
!INCLUDE "SE_CAT.dep"
!ELSE 
!MESSAGE Warning: cannot find "SE_CAT.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SE_CAT - Win32 Release" || "$(CFG)" == "SE_CAT - Win32 Debug"
SOURCE=.\MainDefaultDlg.cpp

!IF  "$(CFG)" == "SE_CAT - Win32 Release"


"$(INTDIR)\MainDefaultDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SE_CAT.pch"


!ELSEIF  "$(CFG)" == "SE_CAT - Win32 Debug"


"$(INTDIR)\MainDefaultDlg.obj"	"$(INTDIR)\MainDefaultDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SE_CAT.pch"


!ENDIF 

SOURCE=.\SE_CAT.cpp

!IF  "$(CFG)" == "SE_CAT - Win32 Release"


"$(INTDIR)\SE_CAT.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SE_CAT.pch"


!ELSEIF  "$(CFG)" == "SE_CAT - Win32 Debug"


"$(INTDIR)\SE_CAT.obj"	"$(INTDIR)\SE_CAT.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SE_CAT.pch"


!ENDIF 

SOURCE=.\SE_CAT.rc

"$(INTDIR)\SE_CAT.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\SE_CAT_SearchDlg.cpp

!IF  "$(CFG)" == "SE_CAT - Win32 Release"


"$(INTDIR)\SE_CAT_SearchDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SE_CAT.pch"


!ELSEIF  "$(CFG)" == "SE_CAT - Win32 Debug"


"$(INTDIR)\SE_CAT_SearchDlg.obj"	"$(INTDIR)\SE_CAT_SearchDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SE_CAT.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "SE_CAT - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\SE_CAT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SE_CAT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SE_CAT - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SE_CAT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\SE_CAT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

