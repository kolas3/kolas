# Microsoft Developer Studio Generated NMAKE File, Based on Duplicate_Data_Search.dsp
!IF "$(CFG)" == ""
CFG=Duplicate_Data_Search - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Duplicate_Data_Search - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Duplicate_Data_Search - Win32 Release" && "$(CFG)" != "Duplicate_Data_Search - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Duplicate_Data_Search.mak" CFG="Duplicate_Data_Search - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Duplicate_Data_Search - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Duplicate_Data_Search - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Duplicate_Data_Search.dll"


CLEAN :
	-@erase "$(INTDIR)\BO_HOLDING_ALL_DUP_SEARCH.obj"
	-@erase "$(INTDIR)\BO_HOLDING_DUP_DETAIL_INFO.obj"
	-@erase "$(INTDIR)\BO_INPUT_DUP_DETAIL_INFO.obj"
	-@erase "$(INTDIR)\BO_WORK_DUP_DETAIL_INFO.obj"
	-@erase "$(INTDIR)\BO_WORK_DUP_SEARCH.obj"
	-@erase "$(INTDIR)\DUP_KEY_SETTING.obj"
	-@erase "$(INTDIR)\DUPBOVolumeDlg.obj"
	-@erase "$(INTDIR)\Duplicate_Data_Search.obj"
	-@erase "$(INTDIR)\Duplicate_Data_Search.pch"
	-@erase "$(INTDIR)\Duplicate_Data_Search.res"
	-@erase "$(INTDIR)\DuplicateSearch_Interface.obj"
	-@erase "$(INTDIR)\DuplicateSearchField.obj"
	-@erase "$(INTDIR)\DuplicateSearchKey.obj"
	-@erase "$(INTDIR)\DuplicateSearchMgr.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Duplicate_Data_Search.dll"
	-@erase "$(OUTDIR)\Duplicate_Data_Search.exp"
	-@erase "$(OUTDIR)\Duplicate_Data_Search.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\Duplicate_Data_Search.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Duplicate_Data_Search.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Duplicate_Data_Search.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\Duplicate_Data_Search.pdb" /machine:I386 /def:".\Duplicate_Data_Search.def" /out:"$(OUTDIR)\Duplicate_Data_Search.dll" /implib:"$(OUTDIR)\Duplicate_Data_Search.lib" 
DEF_FILE= \
	".\Duplicate_Data_Search.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_HOLDING_ALL_DUP_SEARCH.obj" \
	"$(INTDIR)\BO_HOLDING_DUP_DETAIL_INFO.obj" \
	"$(INTDIR)\BO_INPUT_DUP_DETAIL_INFO.obj" \
	"$(INTDIR)\BO_WORK_DUP_DETAIL_INFO.obj" \
	"$(INTDIR)\BO_WORK_DUP_SEARCH.obj" \
	"$(INTDIR)\DUP_KEY_SETTING.obj" \
	"$(INTDIR)\DUPBOVolumeDlg.obj" \
	"$(INTDIR)\Duplicate_Data_Search.obj" \
	"$(INTDIR)\DuplicateSearch_Interface.obj" \
	"$(INTDIR)\DuplicateSearchField.obj" \
	"$(INTDIR)\DuplicateSearchKey.obj" \
	"$(INTDIR)\DuplicateSearchMgr.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Duplicate_Data_Search.res"

"$(OUTDIR)\Duplicate_Data_Search.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"

OUTDIR=.\../../../lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\../../../lib/
# End Custom Macros

ALL : "..\..\..\..\dll\common\Duplicate_Data_Search.dll" "$(OUTDIR)\Duplicate_Data_Search.bsc"


CLEAN :
	-@erase "$(INTDIR)\BO_HOLDING_ALL_DUP_SEARCH.obj"
	-@erase "$(INTDIR)\BO_HOLDING_ALL_DUP_SEARCH.sbr"
	-@erase "$(INTDIR)\BO_HOLDING_DUP_DETAIL_INFO.obj"
	-@erase "$(INTDIR)\BO_HOLDING_DUP_DETAIL_INFO.sbr"
	-@erase "$(INTDIR)\BO_INPUT_DUP_DETAIL_INFO.obj"
	-@erase "$(INTDIR)\BO_INPUT_DUP_DETAIL_INFO.sbr"
	-@erase "$(INTDIR)\BO_WORK_DUP_DETAIL_INFO.obj"
	-@erase "$(INTDIR)\BO_WORK_DUP_DETAIL_INFO.sbr"
	-@erase "$(INTDIR)\BO_WORK_DUP_SEARCH.obj"
	-@erase "$(INTDIR)\BO_WORK_DUP_SEARCH.sbr"
	-@erase "$(INTDIR)\DUP_KEY_SETTING.obj"
	-@erase "$(INTDIR)\DUP_KEY_SETTING.sbr"
	-@erase "$(INTDIR)\DUPBOVolumeDlg.obj"
	-@erase "$(INTDIR)\DUPBOVolumeDlg.sbr"
	-@erase "$(INTDIR)\Duplicate_Data_Search.obj"
	-@erase "$(INTDIR)\Duplicate_Data_Search.pch"
	-@erase "$(INTDIR)\Duplicate_Data_Search.res"
	-@erase "$(INTDIR)\Duplicate_Data_Search.sbr"
	-@erase "$(INTDIR)\DuplicateSearch_Interface.obj"
	-@erase "$(INTDIR)\DuplicateSearch_Interface.sbr"
	-@erase "$(INTDIR)\DuplicateSearchField.obj"
	-@erase "$(INTDIR)\DuplicateSearchField.sbr"
	-@erase "$(INTDIR)\DuplicateSearchKey.obj"
	-@erase "$(INTDIR)\DuplicateSearchKey.sbr"
	-@erase "$(INTDIR)\DuplicateSearchMgr.obj"
	-@erase "$(INTDIR)\DuplicateSearchMgr.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Duplicate_Data_Search.bsc"
	-@erase "$(OUTDIR)\Duplicate_Data_Search.exp"
	-@erase "$(OUTDIR)\Duplicate_Data_Search.lib"
	-@erase "$(OUTDIR)\Duplicate_Data_Search.pdb"
	-@erase "..\..\..\..\dll\common\Duplicate_Data_Search.dll"
	-@erase "..\..\..\..\dll\common\Duplicate_Data_Search.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Duplicate_Data_Search.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Duplicate_Data_Search.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Duplicate_Data_Search.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BO_HOLDING_ALL_DUP_SEARCH.sbr" \
	"$(INTDIR)\BO_HOLDING_DUP_DETAIL_INFO.sbr" \
	"$(INTDIR)\BO_INPUT_DUP_DETAIL_INFO.sbr" \
	"$(INTDIR)\BO_WORK_DUP_DETAIL_INFO.sbr" \
	"$(INTDIR)\BO_WORK_DUP_SEARCH.sbr" \
	"$(INTDIR)\DUP_KEY_SETTING.sbr" \
	"$(INTDIR)\DUPBOVolumeDlg.sbr" \
	"$(INTDIR)\Duplicate_Data_Search.sbr" \
	"$(INTDIR)\DuplicateSearch_Interface.sbr" \
	"$(INTDIR)\DuplicateSearchField.sbr" \
	"$(INTDIR)\DuplicateSearchKey.sbr" \
	"$(INTDIR)\DuplicateSearchMgr.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\Duplicate_Data_Search.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=esl.lib K2UP_Common.lib IndexToolKit.lib marcedit.lib BO_SPECIES_API.lib SIReportMgr.lib SIReportData.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\Duplicate_Data_Search.pdb" /debug /machine:I386 /def:".\Duplicate_Data_Search.def" /out:"../../../../dll/common/Duplicate_Data_Search.dll" /implib:"$(OUTDIR)\Duplicate_Data_Search.lib" /pdbtype:sept 
DEF_FILE= \
	".\Duplicate_Data_Search.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_HOLDING_ALL_DUP_SEARCH.obj" \
	"$(INTDIR)\BO_HOLDING_DUP_DETAIL_INFO.obj" \
	"$(INTDIR)\BO_INPUT_DUP_DETAIL_INFO.obj" \
	"$(INTDIR)\BO_WORK_DUP_DETAIL_INFO.obj" \
	"$(INTDIR)\BO_WORK_DUP_SEARCH.obj" \
	"$(INTDIR)\DUP_KEY_SETTING.obj" \
	"$(INTDIR)\DUPBOVolumeDlg.obj" \
	"$(INTDIR)\Duplicate_Data_Search.obj" \
	"$(INTDIR)\DuplicateSearch_Interface.obj" \
	"$(INTDIR)\DuplicateSearchField.obj" \
	"$(INTDIR)\DuplicateSearchKey.obj" \
	"$(INTDIR)\DuplicateSearchMgr.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Duplicate_Data_Search.res"

"..\..\..\..\dll\common\Duplicate_Data_Search.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("Duplicate_Data_Search.dep")
!INCLUDE "Duplicate_Data_Search.dep"
!ELSE 
!MESSAGE Warning: cannot find "Duplicate_Data_Search.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Duplicate_Data_Search - Win32 Release" || "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"
SOURCE=.\BO_HOLDING_ALL_DUP_SEARCH.cpp

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"


"$(INTDIR)\BO_HOLDING_ALL_DUP_SEARCH.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"


"$(INTDIR)\BO_HOLDING_ALL_DUP_SEARCH.obj"	"$(INTDIR)\BO_HOLDING_ALL_DUP_SEARCH.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ENDIF 

SOURCE=.\BO_HOLDING_DUP_DETAIL_INFO.cpp

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"


"$(INTDIR)\BO_HOLDING_DUP_DETAIL_INFO.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"


"$(INTDIR)\BO_HOLDING_DUP_DETAIL_INFO.obj"	"$(INTDIR)\BO_HOLDING_DUP_DETAIL_INFO.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ENDIF 

SOURCE=.\BO_INPUT_DUP_DETAIL_INFO.cpp

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"


"$(INTDIR)\BO_INPUT_DUP_DETAIL_INFO.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"


"$(INTDIR)\BO_INPUT_DUP_DETAIL_INFO.obj"	"$(INTDIR)\BO_INPUT_DUP_DETAIL_INFO.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ENDIF 

SOURCE=.\BO_WORK_DUP_DETAIL_INFO.cpp

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"


"$(INTDIR)\BO_WORK_DUP_DETAIL_INFO.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"


"$(INTDIR)\BO_WORK_DUP_DETAIL_INFO.obj"	"$(INTDIR)\BO_WORK_DUP_DETAIL_INFO.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ENDIF 

SOURCE=.\BO_WORK_DUP_SEARCH.cpp

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"


"$(INTDIR)\BO_WORK_DUP_SEARCH.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"


"$(INTDIR)\BO_WORK_DUP_SEARCH.obj"	"$(INTDIR)\BO_WORK_DUP_SEARCH.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ENDIF 

SOURCE=.\DUP_KEY_SETTING.cpp

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"


"$(INTDIR)\DUP_KEY_SETTING.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"


"$(INTDIR)\DUP_KEY_SETTING.obj"	"$(INTDIR)\DUP_KEY_SETTING.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ENDIF 

SOURCE=.\DUPBOVolumeDlg.cpp

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"


"$(INTDIR)\DUPBOVolumeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"


"$(INTDIR)\DUPBOVolumeDlg.obj"	"$(INTDIR)\DUPBOVolumeDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ENDIF 

SOURCE=.\Duplicate_Data_Search.cpp

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"


"$(INTDIR)\Duplicate_Data_Search.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"


"$(INTDIR)\Duplicate_Data_Search.obj"	"$(INTDIR)\Duplicate_Data_Search.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ENDIF 

SOURCE=.\Duplicate_Data_Search.rc

"$(INTDIR)\Duplicate_Data_Search.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\DuplicateSearch_Interface.cpp

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"


"$(INTDIR)\DuplicateSearch_Interface.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"


"$(INTDIR)\DuplicateSearch_Interface.obj"	"$(INTDIR)\DuplicateSearch_Interface.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ENDIF 

SOURCE=.\DuplicateSearchField.cpp

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"


"$(INTDIR)\DuplicateSearchField.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"


"$(INTDIR)\DuplicateSearchField.obj"	"$(INTDIR)\DuplicateSearchField.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ENDIF 

SOURCE=.\DuplicateSearchKey.cpp

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"


"$(INTDIR)\DuplicateSearchKey.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"


"$(INTDIR)\DuplicateSearchKey.obj"	"$(INTDIR)\DuplicateSearchKey.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ENDIF 

SOURCE=.\DuplicateSearchMgr.cpp

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"


"$(INTDIR)\DuplicateSearchMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"


"$(INTDIR)\DuplicateSearchMgr.obj"	"$(INTDIR)\DuplicateSearchMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Duplicate_Data_Search.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Duplicate_Data_Search - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\Duplicate_Data_Search.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Duplicate_Data_Search.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Duplicate_Data_Search - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Duplicate_Data_Search.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Duplicate_Data_Search.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

