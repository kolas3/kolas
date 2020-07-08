# Microsoft Developer Studio Generated NMAKE File, Based on BO_CAT_REG.dsp
!IF "$(CFG)" == ""
CFG=BO_CAT_REG - Win32 Debug
!MESSAGE No configuration specified. Defaulting to BO_CAT_REG - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "BO_CAT_REG - Win32 Release" && "$(CFG)" != "BO_CAT_REG - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_CAT_REG.mak" CFG="BO_CAT_REG - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_CAT_REG - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BO_CAT_REG - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "BO_CAT_REG - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\BO_CAT_REG.dll"


CLEAN :
	-@erase "$(INTDIR)\BO_CAT_GIVE_REGNO.obj"
	-@erase "$(INTDIR)\BO_CAT_GROUP_INSERT.obj"
	-@erase "$(INTDIR)\BO_CAT_GROUP_MAIN.obj"
	-@erase "$(INTDIR)\BO_CAT_MAKE_preGROUP.obj"
	-@erase "$(INTDIR)\BO_CAT_PREGROUP_MAIN.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_1100.obj"
	-@erase "$(INTDIR)\BO_CAT_REG.obj"
	-@erase "$(INTDIR)\BO_CAT_REG.pch"
	-@erase "$(INTDIR)\BO_CAT_REG.res"
	-@erase "$(INTDIR)\BO_CAT_REG_0400.obj"
	-@erase "$(INTDIR)\BO_CAT_REG_0500.obj"
	-@erase "$(INTDIR)\BO_CAT_REG_0510.obj"
	-@erase "$(INTDIR)\BO_CAT_REG_0510_01.obj"
	-@erase "$(INTDIR)\BO_CAT_REG_0520.obj"
	-@erase "$(INTDIR)\BO_CAT_REG_0800.obj"
	-@erase "$(INTDIR)\BO_CAT_REG_MAIN.obj"
	-@erase "$(INTDIR)\BO_CAT_SEARCH.obj"
	-@erase "$(INTDIR)\BO_CAT_SEARCH_MGR_GROUP.obj"
	-@erase "$(INTDIR)\BO_CAT_SEARCH_MGR_PRE.obj"
	-@erase "$(INTDIR)\BO_preGROUP_MNG.obj"
	-@erase "$(INTDIR)\BO_REG_CODE.obj"
	-@erase "$(INTDIR)\BO_REG_MGR.obj"
	-@erase "$(INTDIR)\MultiSortDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\BO_CAT_REG.dll"
	-@erase "$(OUTDIR)\BO_CAT_REG.exp"
	-@erase "$(OUTDIR)\BO_CAT_REG.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_REG.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_CAT_REG.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_CAT_REG.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\BO_CAT_REG.pdb" /machine:I386 /def:".\BO_CAT_REG.def" /out:"$(OUTDIR)\BO_CAT_REG.dll" /implib:"$(OUTDIR)\BO_CAT_REG.lib" 
DEF_FILE= \
	".\BO_CAT_REG.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_CAT_GIVE_REGNO.obj" \
	"$(INTDIR)\BO_CAT_GROUP_INSERT.obj" \
	"$(INTDIR)\BO_CAT_GROUP_MAIN.obj" \
	"$(INTDIR)\BO_CAT_MAKE_preGROUP.obj" \
	"$(INTDIR)\BO_CAT_PREGROUP_MAIN.obj" \
	"$(INTDIR)\BO_CAT_PROC_1100.obj" \
	"$(INTDIR)\BO_CAT_REG.obj" \
	"$(INTDIR)\BO_CAT_REG_0400.obj" \
	"$(INTDIR)\BO_CAT_REG_0500.obj" \
	"$(INTDIR)\BO_CAT_REG_0510.obj" \
	"$(INTDIR)\BO_CAT_REG_0510_01.obj" \
	"$(INTDIR)\BO_CAT_REG_0520.obj" \
	"$(INTDIR)\BO_CAT_REG_0800.obj" \
	"$(INTDIR)\BO_CAT_REG_MAIN.obj" \
	"$(INTDIR)\BO_CAT_SEARCH.obj" \
	"$(INTDIR)\BO_CAT_SEARCH_MGR_GROUP.obj" \
	"$(INTDIR)\BO_CAT_SEARCH_MGR_PRE.obj" \
	"$(INTDIR)\BO_preGROUP_MNG.obj" \
	"$(INTDIR)\BO_REG_CODE.obj" \
	"$(INTDIR)\BO_REG_MGR.obj" \
	"$(INTDIR)\MultiSortDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_CAT_REG.res"

"$(OUTDIR)\BO_CAT_REG.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "BO_CAT_REG - Win32 Debug"

OUTDIR=.\..\..\..\..\lib
INTDIR=.\Debug

ALL : "..\..\..\..\..\dll\MONO\BO_CAT_REG.dll"


CLEAN :
	-@erase "$(INTDIR)\BO_CAT_GIVE_REGNO.obj"
	-@erase "$(INTDIR)\BO_CAT_GROUP_INSERT.obj"
	-@erase "$(INTDIR)\BO_CAT_GROUP_MAIN.obj"
	-@erase "$(INTDIR)\BO_CAT_MAKE_preGROUP.obj"
	-@erase "$(INTDIR)\BO_CAT_PREGROUP_MAIN.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_1100.obj"
	-@erase "$(INTDIR)\BO_CAT_REG.obj"
	-@erase "$(INTDIR)\BO_CAT_REG.pch"
	-@erase "$(INTDIR)\BO_CAT_REG.res"
	-@erase "$(INTDIR)\BO_CAT_REG_0400.obj"
	-@erase "$(INTDIR)\BO_CAT_REG_0500.obj"
	-@erase "$(INTDIR)\BO_CAT_REG_0510.obj"
	-@erase "$(INTDIR)\BO_CAT_REG_0510_01.obj"
	-@erase "$(INTDIR)\BO_CAT_REG_0520.obj"
	-@erase "$(INTDIR)\BO_CAT_REG_0800.obj"
	-@erase "$(INTDIR)\BO_CAT_REG_MAIN.obj"
	-@erase "$(INTDIR)\BO_CAT_SEARCH.obj"
	-@erase "$(INTDIR)\BO_CAT_SEARCH_MGR_GROUP.obj"
	-@erase "$(INTDIR)\BO_CAT_SEARCH_MGR_PRE.obj"
	-@erase "$(INTDIR)\BO_preGROUP_MNG.obj"
	-@erase "$(INTDIR)\BO_REG_CODE.obj"
	-@erase "$(INTDIR)\BO_REG_MGR.obj"
	-@erase "$(INTDIR)\MultiSortDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\BO_CAT_REG.exp"
	-@erase "$(OUTDIR)\BO_CAT_REG.lib"
	-@erase "$(OUTDIR)\BO_CAT_REG.pdb"
	-@erase "..\..\..\..\..\dll\MONO\BO_CAT_REG.dll"
	-@erase "..\..\..\..\..\dll\MONO\BO_CAT_REG.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_REG.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_CAT_REG.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_CAT_REG.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=esl.lib K2UP_Common.LIB IndexToolKit.lib BO_CAT_REGNO_GIVE.LIB BO_CAT_REGNO_CANCEL.LIB BO_CAT_LAST_NUMBER.lib BO_ACQ_GROUP_MANAGER.lib BO_ACQ_SPECIES_FORM.lib BO_ACQ_API.lib BO_SPECIES_API.lib FileImport.lib MarcToolKit.lib SIReportData.lib SIReportMgr.lib MarcAdjustApi.lib COMMON_CONVERSION_MGR.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\BO_CAT_REG.pdb" /debug /machine:I386 /def:".\BO_CAT_REG.def" /out:"..\..\..\..\..\dll\MONO\BO_CAT_REG.dll" /implib:"$(OUTDIR)\BO_CAT_REG.lib" /pdbtype:sept 
DEF_FILE= \
	".\BO_CAT_REG.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_CAT_GIVE_REGNO.obj" \
	"$(INTDIR)\BO_CAT_GROUP_INSERT.obj" \
	"$(INTDIR)\BO_CAT_GROUP_MAIN.obj" \
	"$(INTDIR)\BO_CAT_MAKE_preGROUP.obj" \
	"$(INTDIR)\BO_CAT_PREGROUP_MAIN.obj" \
	"$(INTDIR)\BO_CAT_PROC_1100.obj" \
	"$(INTDIR)\BO_CAT_REG.obj" \
	"$(INTDIR)\BO_CAT_REG_0400.obj" \
	"$(INTDIR)\BO_CAT_REG_0500.obj" \
	"$(INTDIR)\BO_CAT_REG_0510.obj" \
	"$(INTDIR)\BO_CAT_REG_0510_01.obj" \
	"$(INTDIR)\BO_CAT_REG_0520.obj" \
	"$(INTDIR)\BO_CAT_REG_0800.obj" \
	"$(INTDIR)\BO_CAT_REG_MAIN.obj" \
	"$(INTDIR)\BO_CAT_SEARCH.obj" \
	"$(INTDIR)\BO_CAT_SEARCH_MGR_GROUP.obj" \
	"$(INTDIR)\BO_CAT_SEARCH_MGR_PRE.obj" \
	"$(INTDIR)\BO_preGROUP_MNG.obj" \
	"$(INTDIR)\BO_REG_CODE.obj" \
	"$(INTDIR)\BO_REG_MGR.obj" \
	"$(INTDIR)\MultiSortDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_CAT_REG.res"

"..\..\..\..\..\dll\MONO\BO_CAT_REG.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("BO_CAT_REG.dep")
!INCLUDE "BO_CAT_REG.dep"
!ELSE 
!MESSAGE Warning: cannot find "BO_CAT_REG.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "BO_CAT_REG - Win32 Release" || "$(CFG)" == "BO_CAT_REG - Win32 Debug"
SOURCE=.\BO_CAT_GIVE_REGNO.cpp

"$(INTDIR)\BO_CAT_GIVE_REGNO.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_GROUP_INSERT.cpp

"$(INTDIR)\BO_CAT_GROUP_INSERT.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_GROUP_MAIN.cpp

"$(INTDIR)\BO_CAT_GROUP_MAIN.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_MAKE_preGROUP.cpp

"$(INTDIR)\BO_CAT_MAKE_preGROUP.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_PREGROUP_MAIN.cpp

"$(INTDIR)\BO_CAT_PREGROUP_MAIN.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_PROC_1100.cpp

"$(INTDIR)\BO_CAT_PROC_1100.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_REG.cpp

"$(INTDIR)\BO_CAT_REG.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_REG.rc

"$(INTDIR)\BO_CAT_REG.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\BO_CAT_REG_0400.cpp

"$(INTDIR)\BO_CAT_REG_0400.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_REG_0500.cpp

"$(INTDIR)\BO_CAT_REG_0500.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_REG_0510.cpp

"$(INTDIR)\BO_CAT_REG_0510.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_REG_0510_01.cpp

"$(INTDIR)\BO_CAT_REG_0510_01.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_REG_0520.cpp

"$(INTDIR)\BO_CAT_REG_0520.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_REG_0800.cpp

"$(INTDIR)\BO_CAT_REG_0800.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_REG_MAIN.cpp

"$(INTDIR)\BO_CAT_REG_MAIN.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_SEARCH.cpp

"$(INTDIR)\BO_CAT_SEARCH.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_SEARCH_MGR_GROUP.cpp

"$(INTDIR)\BO_CAT_SEARCH_MGR_GROUP.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_CAT_SEARCH_MGR_PRE.cpp

"$(INTDIR)\BO_CAT_SEARCH_MGR_PRE.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_preGROUP_MNG.cpp

"$(INTDIR)\BO_preGROUP_MNG.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_REG_CODE.cpp

"$(INTDIR)\BO_REG_CODE.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\BO_REG_MGR.cpp

"$(INTDIR)\BO_REG_MGR.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\MultiSortDlg.cpp

"$(INTDIR)\MultiSortDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_REG.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "BO_CAT_REG - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_REG.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\BO_CAT_REG.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "BO_CAT_REG - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_REG.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\BO_CAT_REG.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

