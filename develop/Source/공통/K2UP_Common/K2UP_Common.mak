# Microsoft Developer Studio Generated NMAKE File, Based on K2UP_Common.dsp
!IF "$(CFG)" == ""
CFG=K2UP_Common - Win32 Debug
!MESSAGE No configuration specified. Defaulting to K2UP_Common - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "K2UP_Common - Win32 Release" && "$(CFG)" != "K2UP_Common - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "K2UP_Common.mak" CFG="K2UP_Common - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "K2UP_Common - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "K2UP_Common - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\K2UP_Common.dll"


CLEAN :
	-@erase "$(INTDIR)\BasicPrintMgr.obj"
	-@erase "$(INTDIR)\BO_ACQ_MANAGE.obj"
	-@erase "$(INTDIR)\BO_DELETE.obj"
	-@erase "$(INTDIR)\BO_DELETE_Mgr.obj"
	-@erase "$(INTDIR)\DataProcessingDlg.obj"
	-@erase "$(INTDIR)\ISBNChecker.obj"
	-@erase "$(INTDIR)\K2UP_Common.obj"
	-@erase "$(INTDIR)\K2UP_Common.pch"
	-@erase "$(INTDIR)\K2UP_Common.res"
	-@erase "$(INTDIR)\Keyword.obj"
	-@erase "$(INTDIR)\MakeSearchData.obj"
	-@erase "$(INTDIR)\MulitComboBoxManager.obj"
	-@erase "$(INTDIR)\PRICE_SUM_LIGHT.obj"
	-@erase "$(INTDIR)\PriceSum.obj"
	-@erase "$(INTDIR)\ReplaceCodeToDesc.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Trim.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\K2UP_Common.dll"
	-@erase "$(OUTDIR)\K2UP_Common.exp"
	-@erase "$(OUTDIR)\K2UP_Common.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\K2UP_Common.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\K2UP_Common.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\K2UP_Common.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\K2UP_Common.pdb" /machine:I386 /def:".\K2UP_Common.def" /out:"$(OUTDIR)\K2UP_Common.dll" /implib:"$(OUTDIR)\K2UP_Common.lib" 
DEF_FILE= \
	".\K2UP_Common.def"
LINK32_OBJS= \
	"$(INTDIR)\BasicPrintMgr.obj" \
	"$(INTDIR)\BO_ACQ_MANAGE.obj" \
	"$(INTDIR)\BO_DELETE.obj" \
	"$(INTDIR)\BO_DELETE_Mgr.obj" \
	"$(INTDIR)\DataProcessingDlg.obj" \
	"$(INTDIR)\ISBNChecker.obj" \
	"$(INTDIR)\K2UP_Common.obj" \
	"$(INTDIR)\Keyword.obj" \
	"$(INTDIR)\MakeSearchData.obj" \
	"$(INTDIR)\MulitComboBoxManager.obj" \
	"$(INTDIR)\PRICE_SUM_LIGHT.obj" \
	"$(INTDIR)\PriceSum.obj" \
	"$(INTDIR)\ReplaceCodeToDesc.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Trim.obj" \
	"$(INTDIR)\K2UP_Common.res"

"$(OUTDIR)\K2UP_Common.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"

OUTDIR=.\../../../lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\../../../lib/
# End Custom Macros

ALL : "..\..\..\..\dll\COMMON\K2UP_Common.dll" "$(OUTDIR)\K2UP_Common.bsc"


CLEAN :
	-@erase "$(INTDIR)\BasicPrintMgr.obj"
	-@erase "$(INTDIR)\BasicPrintMgr.sbr"
	-@erase "$(INTDIR)\BO_ACQ_MANAGE.obj"
	-@erase "$(INTDIR)\BO_ACQ_MANAGE.sbr"
	-@erase "$(INTDIR)\BO_DELETE.obj"
	-@erase "$(INTDIR)\BO_DELETE.sbr"
	-@erase "$(INTDIR)\BO_DELETE_Mgr.obj"
	-@erase "$(INTDIR)\BO_DELETE_Mgr.sbr"
	-@erase "$(INTDIR)\DataProcessingDlg.obj"
	-@erase "$(INTDIR)\DataProcessingDlg.sbr"
	-@erase "$(INTDIR)\ISBNChecker.obj"
	-@erase "$(INTDIR)\ISBNChecker.sbr"
	-@erase "$(INTDIR)\K2UP_Common.obj"
	-@erase "$(INTDIR)\K2UP_Common.pch"
	-@erase "$(INTDIR)\K2UP_Common.res"
	-@erase "$(INTDIR)\K2UP_Common.sbr"
	-@erase "$(INTDIR)\Keyword.obj"
	-@erase "$(INTDIR)\Keyword.sbr"
	-@erase "$(INTDIR)\MakeSearchData.obj"
	-@erase "$(INTDIR)\MakeSearchData.sbr"
	-@erase "$(INTDIR)\MulitComboBoxManager.obj"
	-@erase "$(INTDIR)\MulitComboBoxManager.sbr"
	-@erase "$(INTDIR)\PRICE_SUM_LIGHT.obj"
	-@erase "$(INTDIR)\PRICE_SUM_LIGHT.sbr"
	-@erase "$(INTDIR)\PriceSum.obj"
	-@erase "$(INTDIR)\PriceSum.sbr"
	-@erase "$(INTDIR)\ReplaceCodeToDesc.obj"
	-@erase "$(INTDIR)\ReplaceCodeToDesc.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Trim.obj"
	-@erase "$(INTDIR)\Trim.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\K2UP_Common.bsc"
	-@erase "$(OUTDIR)\K2UP_Common.exp"
	-@erase "$(OUTDIR)\K2UP_Common.lib"
	-@erase "$(OUTDIR)\K2UP_Common.pdb"
	-@erase "..\..\..\..\dll\COMMON\K2UP_Common.dll"
	-@erase "..\..\..\..\dll\COMMON\K2UP_Common.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\K2UP_Common.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\K2UP_Common.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\K2UP_Common.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BasicPrintMgr.sbr" \
	"$(INTDIR)\BO_ACQ_MANAGE.sbr" \
	"$(INTDIR)\BO_DELETE.sbr" \
	"$(INTDIR)\BO_DELETE_Mgr.sbr" \
	"$(INTDIR)\DataProcessingDlg.sbr" \
	"$(INTDIR)\ISBNChecker.sbr" \
	"$(INTDIR)\K2UP_Common.sbr" \
	"$(INTDIR)\Keyword.sbr" \
	"$(INTDIR)\MakeSearchData.sbr" \
	"$(INTDIR)\MulitComboBoxManager.sbr" \
	"$(INTDIR)\PRICE_SUM_LIGHT.sbr" \
	"$(INTDIR)\PriceSum.sbr" \
	"$(INTDIR)\ReplaceCodeToDesc.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Trim.sbr"

"$(OUTDIR)\K2UP_Common.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=esl.lib IndexToolKit.lib BO_SPECIES_API.lib SIReportMgr.lib SIReportData.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\K2UP_Common.pdb" /debug /machine:I386 /def:".\K2UP_Common.def" /out:"..\..\..\..\dll\COMMON\K2UP_Common.dll" /implib:"$(OUTDIR)\K2UP_Common.lib" /pdbtype:sept 
DEF_FILE= \
	".\K2UP_Common.def"
LINK32_OBJS= \
	"$(INTDIR)\BasicPrintMgr.obj" \
	"$(INTDIR)\BO_ACQ_MANAGE.obj" \
	"$(INTDIR)\BO_DELETE.obj" \
	"$(INTDIR)\BO_DELETE_Mgr.obj" \
	"$(INTDIR)\DataProcessingDlg.obj" \
	"$(INTDIR)\ISBNChecker.obj" \
	"$(INTDIR)\K2UP_Common.obj" \
	"$(INTDIR)\Keyword.obj" \
	"$(INTDIR)\MakeSearchData.obj" \
	"$(INTDIR)\MulitComboBoxManager.obj" \
	"$(INTDIR)\PRICE_SUM_LIGHT.obj" \
	"$(INTDIR)\PriceSum.obj" \
	"$(INTDIR)\ReplaceCodeToDesc.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Trim.obj" \
	"$(INTDIR)\K2UP_Common.res"

"..\..\..\..\dll\COMMON\K2UP_Common.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("K2UP_Common.dep")
!INCLUDE "K2UP_Common.dep"
!ELSE 
!MESSAGE Warning: cannot find "K2UP_Common.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "K2UP_Common - Win32 Release" || "$(CFG)" == "K2UP_Common - Win32 Debug"
SOURCE=.\BasicPrintMgr.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\BasicPrintMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\BasicPrintMgr.obj"	"$(INTDIR)\BasicPrintMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 

SOURCE=.\BO_ACQ_MANAGE.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\BO_ACQ_MANAGE.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\BO_ACQ_MANAGE.obj"	"$(INTDIR)\BO_ACQ_MANAGE.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 

SOURCE=.\BO_DELETE.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\BO_DELETE.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\BO_DELETE.obj"	"$(INTDIR)\BO_DELETE.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 

SOURCE=.\BO_DELETE_Mgr.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\BO_DELETE_Mgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\BO_DELETE_Mgr.obj"	"$(INTDIR)\BO_DELETE_Mgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 

SOURCE=.\DataProcessingDlg.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\DataProcessingDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\DataProcessingDlg.obj"	"$(INTDIR)\DataProcessingDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 

SOURCE=.\ISBNChecker.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\ISBNChecker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\ISBNChecker.obj"	"$(INTDIR)\ISBNChecker.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 

SOURCE=.\K2UP_Common.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\K2UP_Common.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\K2UP_Common.obj"	"$(INTDIR)\K2UP_Common.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 

SOURCE=.\K2UP_Common.rc

"$(INTDIR)\K2UP_Common.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Keyword.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\Keyword.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\Keyword.obj"	"$(INTDIR)\Keyword.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 

SOURCE=.\MakeSearchData.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\MakeSearchData.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\MakeSearchData.obj"	"$(INTDIR)\MakeSearchData.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 

SOURCE=.\MulitComboBoxManager.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\MulitComboBoxManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\MulitComboBoxManager.obj"	"$(INTDIR)\MulitComboBoxManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 

SOURCE=.\PRICE_SUM_LIGHT.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\PRICE_SUM_LIGHT.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\PRICE_SUM_LIGHT.obj"	"$(INTDIR)\PRICE_SUM_LIGHT.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 

SOURCE=.\PriceSum.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\PriceSum.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\PriceSum.obj"	"$(INTDIR)\PriceSum.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 

SOURCE=.\ReplaceCodeToDesc.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\ReplaceCodeToDesc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\ReplaceCodeToDesc.obj"	"$(INTDIR)\ReplaceCodeToDesc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\K2UP_Common.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\K2UP_Common.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\K2UP_Common.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\K2UP_Common.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Trim.cpp

!IF  "$(CFG)" == "K2UP_Common - Win32 Release"


"$(INTDIR)\Trim.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ELSEIF  "$(CFG)" == "K2UP_Common - Win32 Debug"


"$(INTDIR)\Trim.obj"	"$(INTDIR)\Trim.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\K2UP_Common.pch"


!ENDIF 


!ENDIF 

