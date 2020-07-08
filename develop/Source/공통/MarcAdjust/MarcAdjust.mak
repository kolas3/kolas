# Microsoft Developer Studio Generated NMAKE File, Based on MarcAdjust.dsp
!IF "$(CFG)" == ""
CFG=MarcAdjust - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MarcAdjust - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MarcAdjust - Win32 Release" && "$(CFG)" != "MarcAdjust - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MarcAdjust.mak" CFG="MarcAdjust - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MarcAdjust - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MarcAdjust - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "MarcAdjust - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MarcAdjust.dll"


CLEAN :
	-@erase "$(INTDIR)\CatESL_Mgr.obj"
	-@erase "$(INTDIR)\CoolDialogBar.obj"
	-@erase "$(INTDIR)\HoldInfoDlg.obj"
	-@erase "$(INTDIR)\LocRecordDlg.obj"
	-@erase "$(INTDIR)\MarcAdjCodeDlg.obj"
	-@erase "$(INTDIR)\MarcAdjComboBox.obj"
	-@erase "$(INTDIR)\MarcAdjDoc.obj"
	-@erase "$(INTDIR)\MarcAdjEdit.obj"
	-@erase "$(INTDIR)\MarcAdjFrame.obj"
	-@erase "$(INTDIR)\MarcAdjLauncher.obj"
	-@erase "$(INTDIR)\MarcAdjMarcEditView.obj"
	-@erase "$(INTDIR)\MarcAdjMarcGridView.obj"
	-@erase "$(INTDIR)\MarcAdjMgr.obj"
	-@erase "$(INTDIR)\MarcAdjMgrMemoDlg.obj"
	-@erase "$(INTDIR)\MarcAdjResourceMgr.obj"
	-@erase "$(INTDIR)\MarcAdjThread.obj"
	-@erase "$(INTDIR)\MarcAdjust.obj"
	-@erase "$(INTDIR)\MarcAdjust.pch"
	-@erase "$(INTDIR)\MarcAdjust.res"
	-@erase "$(INTDIR)\MarcAdjVolumeDlg.obj"
	-@erase "$(INTDIR)\MenuItem.obj"
	-@erase "$(INTDIR)\SpeciesDeletionDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\ToolBarItem.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MarcAdjust.dll"
	-@erase "$(OUTDIR)\MarcAdjust.exp"
	-@erase "$(OUTDIR)\MarcAdjust.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcAdjust.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MarcAdjust.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MarcAdjust.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\MarcAdjust.pdb" /machine:I386 /def:".\MarcAdjust.def" /out:"$(OUTDIR)\MarcAdjust.dll" /implib:"$(OUTDIR)\MarcAdjust.lib" 
DEF_FILE= \
	".\MarcAdjust.def"
LINK32_OBJS= \
	"$(INTDIR)\CatESL_Mgr.obj" \
	"$(INTDIR)\CoolDialogBar.obj" \
	"$(INTDIR)\HoldInfoDlg.obj" \
	"$(INTDIR)\LocRecordDlg.obj" \
	"$(INTDIR)\MarcAdjCodeDlg.obj" \
	"$(INTDIR)\MarcAdjComboBox.obj" \
	"$(INTDIR)\MarcAdjDoc.obj" \
	"$(INTDIR)\MarcAdjEdit.obj" \
	"$(INTDIR)\MarcAdjFrame.obj" \
	"$(INTDIR)\MarcAdjLauncher.obj" \
	"$(INTDIR)\MarcAdjMarcEditView.obj" \
	"$(INTDIR)\MarcAdjMarcGridView.obj" \
	"$(INTDIR)\MarcAdjMgr.obj" \
	"$(INTDIR)\MarcAdjMgrMemoDlg.obj" \
	"$(INTDIR)\MarcAdjResourceMgr.obj" \
	"$(INTDIR)\MarcAdjThread.obj" \
	"$(INTDIR)\MarcAdjust.obj" \
	"$(INTDIR)\MarcAdjVolumeDlg.obj" \
	"$(INTDIR)\MenuItem.obj" \
	"$(INTDIR)\SpeciesDeletionDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ToolBarItem.obj" \
	"$(INTDIR)\MarcAdjust.res"

"$(OUTDIR)\MarcAdjust.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MarcAdjust - Win32 Debug"

OUTDIR=.\../../../lib
INTDIR=.\Debug

ALL : "..\..\..\..\dll\COMMON\MarcAdjust.dll"


CLEAN :
	-@erase "$(INTDIR)\CatESL_Mgr.obj"
	-@erase "$(INTDIR)\CoolDialogBar.obj"
	-@erase "$(INTDIR)\HoldInfoDlg.obj"
	-@erase "$(INTDIR)\LocRecordDlg.obj"
	-@erase "$(INTDIR)\MarcAdjCodeDlg.obj"
	-@erase "$(INTDIR)\MarcAdjComboBox.obj"
	-@erase "$(INTDIR)\MarcAdjDoc.obj"
	-@erase "$(INTDIR)\MarcAdjEdit.obj"
	-@erase "$(INTDIR)\MarcAdjFrame.obj"
	-@erase "$(INTDIR)\MarcAdjLauncher.obj"
	-@erase "$(INTDIR)\MarcAdjMarcEditView.obj"
	-@erase "$(INTDIR)\MarcAdjMarcGridView.obj"
	-@erase "$(INTDIR)\MarcAdjMgr.obj"
	-@erase "$(INTDIR)\MarcAdjMgrMemoDlg.obj"
	-@erase "$(INTDIR)\MarcAdjResourceMgr.obj"
	-@erase "$(INTDIR)\MarcAdjThread.obj"
	-@erase "$(INTDIR)\MarcAdjust.obj"
	-@erase "$(INTDIR)\MarcAdjust.pch"
	-@erase "$(INTDIR)\MarcAdjust.res"
	-@erase "$(INTDIR)\MarcAdjVolumeDlg.obj"
	-@erase "$(INTDIR)\MenuItem.obj"
	-@erase "$(INTDIR)\SpeciesDeletionDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\ToolBarItem.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\MarcAdjust.exp"
	-@erase "$(OUTDIR)\MarcAdjust.lib"
	-@erase "$(OUTDIR)\MarcAdjust.pdb"
	-@erase "..\..\..\..\dll\COMMON\MarcAdjust.dll"
	-@erase "..\..\..\..\dll\COMMON\MarcAdjust.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcAdjust.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MarcAdjust.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MarcAdjust.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=imm32.lib esl.lib MarcAdjustApi.lib MarcEdit.lib MarcToolKit.lib BO_ACQ_SPECIES_FORM.lib BO_VOLUME_FORM.lib BO_SPECIES_API.lib SE_VOLUME_FORM.lib BO_CAT_PROC_FIXEDFIELD.lib Duplicate_Data_Search.lib SIReportMgr.lib SiReportData.lib K2UP_Common.lib BO_CAT_PROC_KEYWORD.lib BO_CAT_PROC.lib Bo_Marc_Import.lib Light_GatherDataSearch.lib UI_SEARCH_DLG.lib ShelfCodeGetter.lib CatCodeGetter.lib MNG_CLASS_SIGN_POPUP.lib BO_CAT_NUMBER.LIB IndexToolKit.lib DonatorNameGetter.lib BO_ACQ_GROUP_MANAGER.lib TOCEDITOR.lib BO_CAT_REGNO_DLG.lib MPropertyGrid.lib Light_BO_CAT_CENTERUPLOAD.lib ContentsMgr_Editor.lib Kolas2up_commonControl.lib CAT_FIXED_FIELD.lib MngCharSetManagement.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\MarcAdjust.pdb" /debug /machine:I386 /def:".\MarcAdjust.def" /out:"..\..\..\..\dll\COMMON\MarcAdjust.dll" /implib:"$(OUTDIR)\MarcAdjust.lib" /pdbtype:sept 
DEF_FILE= \
	".\MarcAdjust.def"
LINK32_OBJS= \
	"$(INTDIR)\CatESL_Mgr.obj" \
	"$(INTDIR)\CoolDialogBar.obj" \
	"$(INTDIR)\HoldInfoDlg.obj" \
	"$(INTDIR)\LocRecordDlg.obj" \
	"$(INTDIR)\MarcAdjCodeDlg.obj" \
	"$(INTDIR)\MarcAdjComboBox.obj" \
	"$(INTDIR)\MarcAdjDoc.obj" \
	"$(INTDIR)\MarcAdjEdit.obj" \
	"$(INTDIR)\MarcAdjFrame.obj" \
	"$(INTDIR)\MarcAdjLauncher.obj" \
	"$(INTDIR)\MarcAdjMarcEditView.obj" \
	"$(INTDIR)\MarcAdjMarcGridView.obj" \
	"$(INTDIR)\MarcAdjMgr.obj" \
	"$(INTDIR)\MarcAdjMgrMemoDlg.obj" \
	"$(INTDIR)\MarcAdjResourceMgr.obj" \
	"$(INTDIR)\MarcAdjThread.obj" \
	"$(INTDIR)\MarcAdjust.obj" \
	"$(INTDIR)\MarcAdjVolumeDlg.obj" \
	"$(INTDIR)\MenuItem.obj" \
	"$(INTDIR)\SpeciesDeletionDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ToolBarItem.obj" \
	"$(INTDIR)\MarcAdjust.res"

"..\..\..\..\dll\COMMON\MarcAdjust.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("MarcAdjust.dep")
!INCLUDE "MarcAdjust.dep"
!ELSE 
!MESSAGE Warning: cannot find "MarcAdjust.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MarcAdjust - Win32 Release" || "$(CFG)" == "MarcAdjust - Win32 Debug"
SOURCE=.\CatESL_Mgr.cpp

"$(INTDIR)\CatESL_Mgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\CoolDialogBar.cpp

"$(INTDIR)\CoolDialogBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\HoldInfoDlg.cpp

"$(INTDIR)\HoldInfoDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\LocRecordDlg.cpp

"$(INTDIR)\LocRecordDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjCodeDlg.cpp

"$(INTDIR)\MarcAdjCodeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjComboBox.cpp

"$(INTDIR)\MarcAdjComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjDoc.cpp

"$(INTDIR)\MarcAdjDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjEdit.cpp

"$(INTDIR)\MarcAdjEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjFrame.cpp

"$(INTDIR)\MarcAdjFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjLauncher.cpp

"$(INTDIR)\MarcAdjLauncher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjMarcEditView.cpp

"$(INTDIR)\MarcAdjMarcEditView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjMarcGridView.cpp

"$(INTDIR)\MarcAdjMarcGridView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjMgr.cpp

"$(INTDIR)\MarcAdjMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjMgrMemoDlg.cpp

"$(INTDIR)\MarcAdjMgrMemoDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjResourceMgr.cpp

"$(INTDIR)\MarcAdjResourceMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjThread.cpp

"$(INTDIR)\MarcAdjThread.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjust.cpp

"$(INTDIR)\MarcAdjust.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MarcAdjust.rc

"$(INTDIR)\MarcAdjust.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\MarcAdjVolumeDlg.cpp

"$(INTDIR)\MarcAdjVolumeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\MenuItem.cpp

"$(INTDIR)\MenuItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\SpeciesDeletionDlg.cpp

"$(INTDIR)\SpeciesDeletionDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "MarcAdjust - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcAdjust.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MarcAdjust.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "MarcAdjust - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcAdjust.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MarcAdjust.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\ToolBarItem.cpp

"$(INTDIR)\ToolBarItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcAdjust.pch"



!ENDIF 

