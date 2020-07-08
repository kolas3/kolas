# Microsoft Developer Studio Generated NMAKE File, Based on SIReportEditor.dsp
!IF "$(CFG)" == ""
CFG=SIReportEditor - Win32 Release
!MESSAGE No configuration specified. Defaulting to SIReportEditor - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "SIReportEditor - Win32 Release" && "$(CFG)" != "SIReportEditor - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SIReportEditor.mak" CFG="SIReportEditor - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SIReportEditor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SIReportEditor - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "SIReportEditor - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\SIReportEditor.exe"


CLEAN :
	-@erase "$(INTDIR)\ColourPicker.obj"
	-@erase "$(INTDIR)\ColourPopup.obj"
	-@erase "$(INTDIR)\CoolDialogBar.obj"
	-@erase "$(INTDIR)\DlgColumnChange.obj"
	-@erase "$(INTDIR)\DlgControlBox.obj"
	-@erase "$(INTDIR)\DlgDefaultDMSetup.obj"
	-@erase "$(INTDIR)\DlgPageSetup.obj"
	-@erase "$(INTDIR)\DlgProperty.obj"
	-@erase "$(INTDIR)\DlgRptFileManage.obj"
	-@erase "$(INTDIR)\DlgRptFileManageModify.obj"
	-@erase "$(INTDIR)\DlgRptSave.obj"
	-@erase "$(INTDIR)\DlgSheetView.obj"
	-@erase "$(INTDIR)\DlgTab.obj"
	-@erase "$(INTDIR)\DlgTableProperty.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MListCtrl.obj"
	-@erase "$(INTDIR)\MSheetDesigner.obj"
	-@erase "$(INTDIR)\OleMListDropTarget.obj"
	-@erase "$(INTDIR)\OleMSheetDesignerDropTarget.obj"
	-@erase "$(INTDIR)\PropertyListCtrl.obj"
	-@erase "$(INTDIR)\ReadCfgFile.obj"
	-@erase "$(INTDIR)\SIReportEditor.obj"
	-@erase "$(INTDIR)\SIReportEditor.pch"
	-@erase "$(INTDIR)\SIReportEditor.res"
	-@erase "$(INTDIR)\SIReportEditorDoc.obj"
	-@erase "$(INTDIR)\SIReportEditorView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SIReportEditor.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\SIReportEditor.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SIReportEditor.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SIReportEditor.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\SIReportEditor.pdb" /machine:I386 /out:"$(OUTDIR)\SIReportEditor.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ColourPicker.obj" \
	"$(INTDIR)\ColourPopup.obj" \
	"$(INTDIR)\CoolDialogBar.obj" \
	"$(INTDIR)\DlgColumnChange.obj" \
	"$(INTDIR)\DlgControlBox.obj" \
	"$(INTDIR)\DlgDefaultDMSetup.obj" \
	"$(INTDIR)\DlgPageSetup.obj" \
	"$(INTDIR)\DlgProperty.obj" \
	"$(INTDIR)\DlgRptFileManage.obj" \
	"$(INTDIR)\DlgRptFileManageModify.obj" \
	"$(INTDIR)\DlgRptSave.obj" \
	"$(INTDIR)\DlgSheetView.obj" \
	"$(INTDIR)\DlgTab.obj" \
	"$(INTDIR)\DlgTableProperty.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MListCtrl.obj" \
	"$(INTDIR)\MSheetDesigner.obj" \
	"$(INTDIR)\OleMListDropTarget.obj" \
	"$(INTDIR)\OleMSheetDesignerDropTarget.obj" \
	"$(INTDIR)\PropertyListCtrl.obj" \
	"$(INTDIR)\ReadCfgFile.obj" \
	"$(INTDIR)\SIReportEditor.obj" \
	"$(INTDIR)\SIReportEditorDoc.obj" \
	"$(INTDIR)\SIReportEditorView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SIReportEditor.res"

"$(OUTDIR)\SIReportEditor.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SIReportEditor - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\..\bin\SIReportEditor.exe"


CLEAN :
	-@erase "$(INTDIR)\ColourPicker.obj"
	-@erase "$(INTDIR)\ColourPopup.obj"
	-@erase "$(INTDIR)\CoolDialogBar.obj"
	-@erase "$(INTDIR)\DlgColumnChange.obj"
	-@erase "$(INTDIR)\DlgControlBox.obj"
	-@erase "$(INTDIR)\DlgDefaultDMSetup.obj"
	-@erase "$(INTDIR)\DlgPageSetup.obj"
	-@erase "$(INTDIR)\DlgProperty.obj"
	-@erase "$(INTDIR)\DlgRptFileManage.obj"
	-@erase "$(INTDIR)\DlgRptFileManageModify.obj"
	-@erase "$(INTDIR)\DlgRptSave.obj"
	-@erase "$(INTDIR)\DlgSheetView.obj"
	-@erase "$(INTDIR)\DlgTab.obj"
	-@erase "$(INTDIR)\DlgTableProperty.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MListCtrl.obj"
	-@erase "$(INTDIR)\MSheetDesigner.obj"
	-@erase "$(INTDIR)\OleMListDropTarget.obj"
	-@erase "$(INTDIR)\OleMSheetDesignerDropTarget.obj"
	-@erase "$(INTDIR)\PropertyListCtrl.obj"
	-@erase "$(INTDIR)\ReadCfgFile.obj"
	-@erase "$(INTDIR)\SIReportEditor.obj"
	-@erase "$(INTDIR)\SIReportEditor.pch"
	-@erase "$(INTDIR)\SIReportEditor.res"
	-@erase "$(INTDIR)\SIReportEditorDoc.obj"
	-@erase "$(INTDIR)\SIReportEditorView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SIReportEditor.pdb"
	-@erase "..\..\..\..\bin\SIReportEditor.exe"
	-@erase "..\..\..\..\bin\SIReportEditor.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\SIReportEditor.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SIReportEditor.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SIReportEditor.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=SIReportData.lib esl.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\SIReportEditor.pdb" /debug /machine:I386 /out:"..\..\..\..\bin\SIReportEditor.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ColourPicker.obj" \
	"$(INTDIR)\ColourPopup.obj" \
	"$(INTDIR)\CoolDialogBar.obj" \
	"$(INTDIR)\DlgColumnChange.obj" \
	"$(INTDIR)\DlgControlBox.obj" \
	"$(INTDIR)\DlgDefaultDMSetup.obj" \
	"$(INTDIR)\DlgPageSetup.obj" \
	"$(INTDIR)\DlgProperty.obj" \
	"$(INTDIR)\DlgRptFileManage.obj" \
	"$(INTDIR)\DlgRptFileManageModify.obj" \
	"$(INTDIR)\DlgRptSave.obj" \
	"$(INTDIR)\DlgSheetView.obj" \
	"$(INTDIR)\DlgTab.obj" \
	"$(INTDIR)\DlgTableProperty.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MListCtrl.obj" \
	"$(INTDIR)\MSheetDesigner.obj" \
	"$(INTDIR)\OleMListDropTarget.obj" \
	"$(INTDIR)\OleMSheetDesignerDropTarget.obj" \
	"$(INTDIR)\PropertyListCtrl.obj" \
	"$(INTDIR)\ReadCfgFile.obj" \
	"$(INTDIR)\SIReportEditor.obj" \
	"$(INTDIR)\SIReportEditorDoc.obj" \
	"$(INTDIR)\SIReportEditorView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SIReportEditor.res"

"..\..\..\..\bin\SIReportEditor.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("SIReportEditor.dep")
!INCLUDE "SIReportEditor.dep"
!ELSE 
!MESSAGE Warning: cannot find "SIReportEditor.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SIReportEditor - Win32 Release" || "$(CFG)" == "SIReportEditor - Win32 Debug"
SOURCE=.\ColourPicker.cpp

"$(INTDIR)\ColourPicker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\ColourPopup.cpp

"$(INTDIR)\ColourPopup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\CoolDialogBar.cpp

"$(INTDIR)\CoolDialogBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\DlgColumnChange.cpp

"$(INTDIR)\DlgColumnChange.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\DlgControlBox.cpp

"$(INTDIR)\DlgControlBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\DlgDefaultDMSetup.cpp

"$(INTDIR)\DlgDefaultDMSetup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\DlgPageSetup.cpp

"$(INTDIR)\DlgPageSetup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\DlgProperty.cpp

"$(INTDIR)\DlgProperty.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\DlgRptFileManage.cpp

"$(INTDIR)\DlgRptFileManage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\DlgRptFileManageModify.cpp

"$(INTDIR)\DlgRptFileManageModify.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\DlgRptSave.cpp

"$(INTDIR)\DlgRptSave.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\DlgSheetView.cpp

"$(INTDIR)\DlgSheetView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\DlgTab.cpp

"$(INTDIR)\DlgTab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\DlgTableProperty.cpp

"$(INTDIR)\DlgTableProperty.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\MListCtrl.cpp

"$(INTDIR)\MListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\MSheetDesigner.cpp

"$(INTDIR)\MSheetDesigner.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\OleMListDropTarget.cpp

"$(INTDIR)\OleMListDropTarget.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\OleMSheetDesignerDropTarget.cpp

"$(INTDIR)\OleMSheetDesignerDropTarget.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\PropertyListCtrl.cpp

"$(INTDIR)\PropertyListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\ReadCfgFile.cpp

"$(INTDIR)\ReadCfgFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\SIReportEditor.cpp

"$(INTDIR)\SIReportEditor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\SIReportEditor.rc

"$(INTDIR)\SIReportEditor.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\SIReportEditorDoc.cpp

"$(INTDIR)\SIReportEditorDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\SIReportEditorView.cpp

"$(INTDIR)\SIReportEditorView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportEditor.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "SIReportEditor - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\SIReportEditor.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SIReportEditor.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SIReportEditor - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\SIReportEditor.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SIReportEditor.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

