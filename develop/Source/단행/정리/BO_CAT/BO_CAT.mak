# Microsoft Developer Studio Generated NMAKE File, Based on BO_CAT.dsp
!IF "$(CFG)" == ""
CFG=BO_CAT - Win32 Debug
!MESSAGE No configuration specified. Defaulting to BO_CAT - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "BO_CAT - Win32 Release" && "$(CFG)" != "BO_CAT - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_CAT.mak" CFG="BO_CAT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_CAT - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BO_CAT - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "BO_CAT - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\BO_CAT.dll"


CLEAN :
	-@erase "$(INTDIR)\BO_CAT.obj"
	-@erase "$(INTDIR)\BO_CAT.pch"
	-@erase "$(INTDIR)\BO_CAT.res"
	-@erase "$(INTDIR)\BO_CAT_SearchDlg.obj"
	-@erase "$(INTDIR)\BO_ClassCodeVolumeDlg.obj"
	-@erase "$(INTDIR)\MainDefaultDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\BO_CAT.dll"
	-@erase "$(OUTDIR)\BO_CAT.exp"
	-@erase "$(OUTDIR)\BO_CAT.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_CAT.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_CAT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\BO_CAT.pdb" /machine:I386 /def:".\BO_CAT.def" /out:"$(OUTDIR)\BO_CAT.dll" /implib:"$(OUTDIR)\BO_CAT.lib" 
DEF_FILE= \
	".\BO_CAT.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_CAT.obj" \
	"$(INTDIR)\BO_CAT_SearchDlg.obj" \
	"$(INTDIR)\BO_ClassCodeVolumeDlg.obj" \
	"$(INTDIR)\MainDefaultDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_CAT.res"

"$(OUTDIR)\BO_CAT.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "BO_CAT - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\..\..\dll\MONO\BO_CAT.dll" "$(OUTDIR)\BO_CAT.bsc"


CLEAN :
	-@erase "$(INTDIR)\BO_CAT.obj"
	-@erase "$(INTDIR)\BO_CAT.pch"
	-@erase "$(INTDIR)\BO_CAT.res"
	-@erase "$(INTDIR)\BO_CAT.sbr"
	-@erase "$(INTDIR)\BO_CAT_SearchDlg.obj"
	-@erase "$(INTDIR)\BO_CAT_SearchDlg.sbr"
	-@erase "$(INTDIR)\BO_ClassCodeVolumeDlg.obj"
	-@erase "$(INTDIR)\BO_ClassCodeVolumeDlg.sbr"
	-@erase "$(INTDIR)\MainDefaultDlg.obj"
	-@erase "$(INTDIR)\MainDefaultDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\BO_CAT.bsc"
	-@erase "$(OUTDIR)\BO_CAT.exp"
	-@erase "$(OUTDIR)\BO_CAT.lib"
	-@erase "$(OUTDIR)\BO_CAT.pdb"
	-@erase "..\..\..\..\..\dll\MONO\BO_CAT.dll"
	-@erase "..\..\..\..\..\dll\MONO\BO_CAT.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\BO_CAT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_CAT.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_CAT.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BO_CAT.sbr" \
	"$(INTDIR)\BO_CAT_SearchDlg.sbr" \
	"$(INTDIR)\BO_ClassCodeVolumeDlg.sbr" \
	"$(INTDIR)\MainDefaultDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\BO_CAT.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=esl.lib MarcAdjust.lib MarcAdjustApi.lib MarcEdit.lib IndexToolKit.lib K2UP_Common.lib SIReportData.lib SIReportMgr.lib Duplicate_Data_Search.lib BO_CAT_DUP_FREE.LIB BO_CAT_PROC.lib CAT_MARC_IMPORT.lib CAT_MARC_EXPORT.lib MarcToolkit.lib BO_ACQ_GROUP_MANAGER.lib Light_BO_CAT_CENTERUPLOAD.lib CAT_PRINT.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\BO_CAT.pdb" /debug /machine:I386 /def:".\BO_CAT.def" /out:"..\..\..\..\..\dll\MONO\BO_CAT.dll" /implib:"$(OUTDIR)\BO_CAT.lib" /pdbtype:sept 
DEF_FILE= \
	".\BO_CAT.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_CAT.obj" \
	"$(INTDIR)\BO_CAT_SearchDlg.obj" \
	"$(INTDIR)\BO_ClassCodeVolumeDlg.obj" \
	"$(INTDIR)\MainDefaultDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_CAT.res"

"..\..\..\..\..\dll\MONO\BO_CAT.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("BO_CAT.dep")
!INCLUDE "BO_CAT.dep"
!ELSE 
!MESSAGE Warning: cannot find "BO_CAT.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "BO_CAT - Win32 Release" || "$(CFG)" == "BO_CAT - Win32 Debug"
SOURCE=.\BO_CAT.cpp

!IF  "$(CFG)" == "BO_CAT - Win32 Release"


"$(INTDIR)\BO_CAT.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT.pch"


!ELSEIF  "$(CFG)" == "BO_CAT - Win32 Debug"


"$(INTDIR)\BO_CAT.obj"	"$(INTDIR)\BO_CAT.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT.pch"


!ENDIF 

SOURCE=.\BO_CAT.rc

"$(INTDIR)\BO_CAT.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\BO_CAT_SearchDlg.cpp

!IF  "$(CFG)" == "BO_CAT - Win32 Release"


"$(INTDIR)\BO_CAT_SearchDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT.pch"


!ELSEIF  "$(CFG)" == "BO_CAT - Win32 Debug"


"$(INTDIR)\BO_CAT_SearchDlg.obj"	"$(INTDIR)\BO_CAT_SearchDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT.pch"


!ENDIF 

SOURCE=.\BO_ClassCodeVolumeDlg.cpp

!IF  "$(CFG)" == "BO_CAT - Win32 Release"


"$(INTDIR)\BO_ClassCodeVolumeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT.pch"


!ELSEIF  "$(CFG)" == "BO_CAT - Win32 Debug"


"$(INTDIR)\BO_ClassCodeVolumeDlg.obj"	"$(INTDIR)\BO_ClassCodeVolumeDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT.pch"


!ENDIF 

SOURCE=.\MainDefaultDlg.cpp

!IF  "$(CFG)" == "BO_CAT - Win32 Release"


"$(INTDIR)\MainDefaultDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT.pch"


!ELSEIF  "$(CFG)" == "BO_CAT - Win32 Debug"


"$(INTDIR)\MainDefaultDlg.obj"	"$(INTDIR)\MainDefaultDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "BO_CAT - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\BO_CAT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "BO_CAT - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\BO_CAT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\BO_CAT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

