# Microsoft Developer Studio Generated NMAKE File, Based on ESL.dsp
!IF "$(CFG)" == ""
CFG=ESL - Win32 Unicode Debug
!MESSAGE No configuration specified. Defaulting to ESL - Win32 Unicode Debug.
!ENDIF 

!IF "$(CFG)" != "ESL - Win32 Release" && "$(CFG)" != "ESL - Win32 Debug" && "$(CFG)" != "ESL - Win32 Unicode Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ESL.mak" CFG="ESL - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ESL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ESL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ESL - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "ESL - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\bin\ESL.dll"


CLEAN :
	-@erase "$(INTDIR)\_excelmancontrol.obj"
	-@erase "$(INTDIR)\CaptureScreen.obj"
	-@erase "$(INTDIR)\ConvertToHanjaDlg.obj"
	-@erase "$(INTDIR)\DlgClassCode.obj"
	-@erase "$(INTDIR)\DlgHanjaWordInputModify.obj"
	-@erase "$(INTDIR)\DlgHanjaWordManager.obj"
	-@erase "$(INTDIR)\ECO_ControlDateEdit.obj"
	-@erase "$(INTDIR)\ECO_ControlDateEditElem.obj"
	-@erase "$(INTDIR)\EFS.obj"
	-@erase "$(INTDIR)\EFSDMException.obj"
	-@erase "$(INTDIR)\EFSReportDlg.obj"
	-@erase "$(INTDIR)\ESL.obj"
	-@erase "$(INTDIR)\ESL.pch"
	-@erase "$(INTDIR)\ESL.res"
	-@erase "$(INTDIR)\ESL_CharSetMgr.obj"
	-@erase "$(INTDIR)\ESL_CodeMgr.obj"
	-@erase "$(INTDIR)\ESL_Control_Element.obj"
	-@erase "$(INTDIR)\ESL_ControlComboBox.obj"
	-@erase "$(INTDIR)\ESL_ControlEdit.obj"
	-@erase "$(INTDIR)\ESL_ControlMgr.obj"
	-@erase "$(INTDIR)\ESL_ControlMultiComboBox.obj"
	-@erase "$(INTDIR)\ESL_DataMgr.obj"
	-@erase "$(INTDIR)\ESL_DataVerifyMgr.obj"
	-@erase "$(INTDIR)\ESL_DateTimeCtrl.obj"
	-@erase "$(INTDIR)\ESL_DVM_VIEWER.obj"
	-@erase "$(INTDIR)\ESL_Grid.obj"
	-@erase "$(INTDIR)\ESL_GridColumnInfo.obj"
	-@erase "$(INTDIR)\ESL_GridData.obj"
	-@erase "$(INTDIR)\ESL_GridSelEditor.obj"
	-@erase "$(INTDIR)\ESL_Information.obj"
	-@erase "$(INTDIR)\ESL_MBCSFileIO.obj"
	-@erase "$(INTDIR)\ESL_Mgr.obj"
	-@erase "$(INTDIR)\ESL_UnicodeFileIO.obj"
	-@erase "$(INTDIR)\esl_worksheet_dlg.obj"
	-@erase "$(INTDIR)\ESLDlg_Confirm.obj"
	-@erase "$(INTDIR)\ESLDlg_DM_Viewer.obj"
	-@erase "$(INTDIR)\ESLDlg_Error.obj"
	-@erase "$(INTDIR)\ESLDlg_GridConfig.obj"
	-@erase "$(INTDIR)\ESLDlg_GridConfig_INSERT.obj"
	-@erase "$(INTDIR)\ESLDlg_Work.obj"
	-@erase "$(INTDIR)\ExcelManager.obj"
	-@erase "$(INTDIR)\EXCEPT.obj"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\JComboBx.obj"
	-@erase "$(INTDIR)\LIB_F2KEY_SpecialChar.obj"
	-@erase "$(INTDIR)\LIB_F2KEY_SpecialCharZoom.obj"
	-@erase "$(INTDIR)\Marc.obj"
	-@erase "$(INTDIR)\MarcInstruction.obj"
	-@erase "$(INTDIR)\MarcMgr.obj"
	-@erase "$(INTDIR)\MarcOperation.obj"
	-@erase "$(INTDIR)\MarcRelation.obj"
	-@erase "$(INTDIR)\MarcSimpleRelation.obj"
	-@erase "$(INTDIR)\mshflexgrid.obj"
	-@erase "$(INTDIR)\picture.obj"
	-@erase "$(INTDIR)\recordset.obj"
	-@erase "$(INTDIR)\rowcursor.obj"
	-@erase "$(INTDIR)\SPF_Work.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Subfield.obj"
	-@erase "$(INTDIR)\Tag.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ESL.exp"
	-@erase "$(OUTDIR)\ESL.lib"
	-@erase "..\..\..\bin\ESL.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ESL.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ESL.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ESL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=libmySQL.lib imm32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\ESL.pdb" /machine:I386 /def:".\ESL.def" /out:"..\..\..\bin\ESL.dll" /implib:"$(OUTDIR)\ESL.lib" 
DEF_FILE= \
	".\ESL.def"
LINK32_OBJS= \
	"$(INTDIR)\_excelmancontrol.obj" \
	"$(INTDIR)\CaptureScreen.obj" \
	"$(INTDIR)\ConvertToHanjaDlg.obj" \
	"$(INTDIR)\DlgClassCode.obj" \
	"$(INTDIR)\DlgHanjaWordInputModify.obj" \
	"$(INTDIR)\DlgHanjaWordManager.obj" \
	"$(INTDIR)\ECO_ControlDateEdit.obj" \
	"$(INTDIR)\ECO_ControlDateEditElem.obj" \
	"$(INTDIR)\EFS.obj" \
	"$(INTDIR)\EFSReportDlg.obj" \
	"$(INTDIR)\ESL.obj" \
	"$(INTDIR)\ESL_CharSetMgr.obj" \
	"$(INTDIR)\ESL_CodeMgr.obj" \
	"$(INTDIR)\ESL_Control_Element.obj" \
	"$(INTDIR)\ESL_ControlComboBox.obj" \
	"$(INTDIR)\ESL_ControlEdit.obj" \
	"$(INTDIR)\ESL_ControlMgr.obj" \
	"$(INTDIR)\ESL_ControlMultiComboBox.obj" \
	"$(INTDIR)\ESL_DataMgr.obj" \
	"$(INTDIR)\ESL_DataVerifyMgr.obj" \
	"$(INTDIR)\ESL_DateTimeCtrl.obj" \
	"$(INTDIR)\ESL_DVM_VIEWER.obj" \
	"$(INTDIR)\ESL_Grid.obj" \
	"$(INTDIR)\ESL_GridColumnInfo.obj" \
	"$(INTDIR)\ESL_GridData.obj" \
	"$(INTDIR)\ESL_GridSelEditor.obj" \
	"$(INTDIR)\ESL_Information.obj" \
	"$(INTDIR)\ESL_MBCSFileIO.obj" \
	"$(INTDIR)\ESL_Mgr.obj" \
	"$(INTDIR)\ESL_UnicodeFileIO.obj" \
	"$(INTDIR)\esl_worksheet_dlg.obj" \
	"$(INTDIR)\ESLDlg_Confirm.obj" \
	"$(INTDIR)\ESLDlg_DM_Viewer.obj" \
	"$(INTDIR)\ESLDlg_Error.obj" \
	"$(INTDIR)\ESLDlg_GridConfig.obj" \
	"$(INTDIR)\ESLDlg_GridConfig_INSERT.obj" \
	"$(INTDIR)\ESLDlg_Work.obj" \
	"$(INTDIR)\ExcelManager.obj" \
	"$(INTDIR)\EXCEPT.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\JComboBx.obj" \
	"$(INTDIR)\LIB_F2KEY_SpecialChar.obj" \
	"$(INTDIR)\LIB_F2KEY_SpecialCharZoom.obj" \
	"$(INTDIR)\Marc.obj" \
	"$(INTDIR)\MarcInstruction.obj" \
	"$(INTDIR)\MarcMgr.obj" \
	"$(INTDIR)\MarcOperation.obj" \
	"$(INTDIR)\MarcRelation.obj" \
	"$(INTDIR)\MarcSimpleRelation.obj" \
	"$(INTDIR)\mshflexgrid.obj" \
	"$(INTDIR)\picture.obj" \
	"$(INTDIR)\recordset.obj" \
	"$(INTDIR)\rowcursor.obj" \
	"$(INTDIR)\SPF_Work.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Subfield.obj" \
	"$(INTDIR)\Tag.obj" \
	"$(INTDIR)\ESL.res" \
	"$(INTDIR)\EFSDMException.obj"

"..\..\..\bin\ESL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"

OUTDIR=.\../../../lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\../../../lib/
# End Custom Macros

ALL : "..\..\..\..\dll\COMMON\ESL.dll" "$(OUTDIR)\ESL.bsc"


CLEAN :
	-@erase "$(INTDIR)\_excelmancontrol.obj"
	-@erase "$(INTDIR)\_excelmancontrol.sbr"
	-@erase "$(INTDIR)\CaptureScreen.obj"
	-@erase "$(INTDIR)\CaptureScreen.sbr"
	-@erase "$(INTDIR)\ConvertToHanjaDlg.obj"
	-@erase "$(INTDIR)\ConvertToHanjaDlg.sbr"
	-@erase "$(INTDIR)\DlgClassCode.obj"
	-@erase "$(INTDIR)\DlgClassCode.sbr"
	-@erase "$(INTDIR)\DlgHanjaWordInputModify.obj"
	-@erase "$(INTDIR)\DlgHanjaWordInputModify.sbr"
	-@erase "$(INTDIR)\DlgHanjaWordManager.obj"
	-@erase "$(INTDIR)\DlgHanjaWordManager.sbr"
	-@erase "$(INTDIR)\ECO_ControlDateEdit.obj"
	-@erase "$(INTDIR)\ECO_ControlDateEdit.sbr"
	-@erase "$(INTDIR)\ECO_ControlDateEditElem.obj"
	-@erase "$(INTDIR)\ECO_ControlDateEditElem.sbr"
	-@erase "$(INTDIR)\EFS.obj"
	-@erase "$(INTDIR)\EFS.sbr"
	-@erase "$(INTDIR)\EFSDMException.obj"
	-@erase "$(INTDIR)\EFSDMException.sbr"
	-@erase "$(INTDIR)\EFSReportDlg.obj"
	-@erase "$(INTDIR)\EFSReportDlg.sbr"
	-@erase "$(INTDIR)\ESL.obj"
	-@erase "$(INTDIR)\ESL.pch"
	-@erase "$(INTDIR)\ESL.res"
	-@erase "$(INTDIR)\ESL.sbr"
	-@erase "$(INTDIR)\ESL_CharSetMgr.obj"
	-@erase "$(INTDIR)\ESL_CharSetMgr.sbr"
	-@erase "$(INTDIR)\ESL_CodeMgr.obj"
	-@erase "$(INTDIR)\ESL_CodeMgr.sbr"
	-@erase "$(INTDIR)\ESL_Control_Element.obj"
	-@erase "$(INTDIR)\ESL_Control_Element.sbr"
	-@erase "$(INTDIR)\ESL_ControlComboBox.obj"
	-@erase "$(INTDIR)\ESL_ControlComboBox.sbr"
	-@erase "$(INTDIR)\ESL_ControlEdit.obj"
	-@erase "$(INTDIR)\ESL_ControlEdit.sbr"
	-@erase "$(INTDIR)\ESL_ControlMgr.obj"
	-@erase "$(INTDIR)\ESL_ControlMgr.sbr"
	-@erase "$(INTDIR)\ESL_ControlMultiComboBox.obj"
	-@erase "$(INTDIR)\ESL_ControlMultiComboBox.sbr"
	-@erase "$(INTDIR)\ESL_DataMgr.obj"
	-@erase "$(INTDIR)\ESL_DataMgr.sbr"
	-@erase "$(INTDIR)\ESL_DataVerifyMgr.obj"
	-@erase "$(INTDIR)\ESL_DataVerifyMgr.sbr"
	-@erase "$(INTDIR)\ESL_DateTimeCtrl.obj"
	-@erase "$(INTDIR)\ESL_DateTimeCtrl.sbr"
	-@erase "$(INTDIR)\ESL_DVM_VIEWER.obj"
	-@erase "$(INTDIR)\ESL_DVM_VIEWER.sbr"
	-@erase "$(INTDIR)\ESL_Grid.obj"
	-@erase "$(INTDIR)\ESL_Grid.sbr"
	-@erase "$(INTDIR)\ESL_GridColumnInfo.obj"
	-@erase "$(INTDIR)\ESL_GridColumnInfo.sbr"
	-@erase "$(INTDIR)\ESL_GridData.obj"
	-@erase "$(INTDIR)\ESL_GridData.sbr"
	-@erase "$(INTDIR)\ESL_GridSelEditor.obj"
	-@erase "$(INTDIR)\ESL_GridSelEditor.sbr"
	-@erase "$(INTDIR)\ESL_Information.obj"
	-@erase "$(INTDIR)\ESL_Information.sbr"
	-@erase "$(INTDIR)\ESL_MBCSFileIO.obj"
	-@erase "$(INTDIR)\ESL_MBCSFileIO.sbr"
	-@erase "$(INTDIR)\ESL_Mgr.obj"
	-@erase "$(INTDIR)\ESL_Mgr.sbr"
	-@erase "$(INTDIR)\ESL_UnicodeFileIO.obj"
	-@erase "$(INTDIR)\ESL_UnicodeFileIO.sbr"
	-@erase "$(INTDIR)\esl_worksheet_dlg.obj"
	-@erase "$(INTDIR)\esl_worksheet_dlg.sbr"
	-@erase "$(INTDIR)\ESLDlg_Confirm.obj"
	-@erase "$(INTDIR)\ESLDlg_Confirm.sbr"
	-@erase "$(INTDIR)\ESLDlg_DM_Viewer.obj"
	-@erase "$(INTDIR)\ESLDlg_DM_Viewer.sbr"
	-@erase "$(INTDIR)\ESLDlg_Error.obj"
	-@erase "$(INTDIR)\ESLDlg_Error.sbr"
	-@erase "$(INTDIR)\ESLDlg_GridConfig.obj"
	-@erase "$(INTDIR)\ESLDlg_GridConfig.sbr"
	-@erase "$(INTDIR)\ESLDlg_GridConfig_INSERT.obj"
	-@erase "$(INTDIR)\ESLDlg_GridConfig_INSERT.sbr"
	-@erase "$(INTDIR)\ESLDlg_Work.obj"
	-@erase "$(INTDIR)\ESLDlg_Work.sbr"
	-@erase "$(INTDIR)\ExcelManager.obj"
	-@erase "$(INTDIR)\ExcelManager.sbr"
	-@erase "$(INTDIR)\EXCEPT.obj"
	-@erase "$(INTDIR)\EXCEPT.sbr"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\font.sbr"
	-@erase "$(INTDIR)\JComboBx.obj"
	-@erase "$(INTDIR)\JComboBx.sbr"
	-@erase "$(INTDIR)\LIB_F2KEY_SpecialChar.obj"
	-@erase "$(INTDIR)\LIB_F2KEY_SpecialChar.sbr"
	-@erase "$(INTDIR)\LIB_F2KEY_SpecialCharZoom.obj"
	-@erase "$(INTDIR)\LIB_F2KEY_SpecialCharZoom.sbr"
	-@erase "$(INTDIR)\Marc.obj"
	-@erase "$(INTDIR)\Marc.sbr"
	-@erase "$(INTDIR)\MarcInstruction.obj"
	-@erase "$(INTDIR)\MarcInstruction.sbr"
	-@erase "$(INTDIR)\MarcMgr.obj"
	-@erase "$(INTDIR)\MarcMgr.sbr"
	-@erase "$(INTDIR)\MarcOperation.obj"
	-@erase "$(INTDIR)\MarcOperation.sbr"
	-@erase "$(INTDIR)\MarcRelation.obj"
	-@erase "$(INTDIR)\MarcRelation.sbr"
	-@erase "$(INTDIR)\MarcSimpleRelation.obj"
	-@erase "$(INTDIR)\MarcSimpleRelation.sbr"
	-@erase "$(INTDIR)\mshflexgrid.obj"
	-@erase "$(INTDIR)\mshflexgrid.sbr"
	-@erase "$(INTDIR)\picture.obj"
	-@erase "$(INTDIR)\picture.sbr"
	-@erase "$(INTDIR)\recordset.obj"
	-@erase "$(INTDIR)\recordset.sbr"
	-@erase "$(INTDIR)\rowcursor.obj"
	-@erase "$(INTDIR)\rowcursor.sbr"
	-@erase "$(INTDIR)\SPF_Work.obj"
	-@erase "$(INTDIR)\SPF_Work.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Subfield.obj"
	-@erase "$(INTDIR)\Subfield.sbr"
	-@erase "$(INTDIR)\Tag.obj"
	-@erase "$(INTDIR)\Tag.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ESL.bsc"
	-@erase "$(OUTDIR)\ESL.exp"
	-@erase "$(OUTDIR)\ESL.lib"
	-@erase "$(OUTDIR)\ESL.pdb"
	-@erase "..\..\..\..\dll\COMMON\ESL.dll"
	-@erase "..\..\..\..\dll\COMMON\ESL.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_MBCS" /D "_WINDLL" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ESL.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ESL.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ESL.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_excelmancontrol.sbr" \
	"$(INTDIR)\CaptureScreen.sbr" \
	"$(INTDIR)\ConvertToHanjaDlg.sbr" \
	"$(INTDIR)\DlgClassCode.sbr" \
	"$(INTDIR)\DlgHanjaWordInputModify.sbr" \
	"$(INTDIR)\DlgHanjaWordManager.sbr" \
	"$(INTDIR)\ECO_ControlDateEdit.sbr" \
	"$(INTDIR)\ECO_ControlDateEditElem.sbr" \
	"$(INTDIR)\EFS.sbr" \
	"$(INTDIR)\EFSReportDlg.sbr" \
	"$(INTDIR)\ESL.sbr" \
	"$(INTDIR)\ESL_CharSetMgr.sbr" \
	"$(INTDIR)\ESL_CodeMgr.sbr" \
	"$(INTDIR)\ESL_Control_Element.sbr" \
	"$(INTDIR)\ESL_ControlComboBox.sbr" \
	"$(INTDIR)\ESL_ControlEdit.sbr" \
	"$(INTDIR)\ESL_ControlMgr.sbr" \
	"$(INTDIR)\ESL_ControlMultiComboBox.sbr" \
	"$(INTDIR)\ESL_DataMgr.sbr" \
	"$(INTDIR)\ESL_DataVerifyMgr.sbr" \
	"$(INTDIR)\ESL_DateTimeCtrl.sbr" \
	"$(INTDIR)\ESL_DVM_VIEWER.sbr" \
	"$(INTDIR)\ESL_Grid.sbr" \
	"$(INTDIR)\ESL_GridColumnInfo.sbr" \
	"$(INTDIR)\ESL_GridData.sbr" \
	"$(INTDIR)\ESL_GridSelEditor.sbr" \
	"$(INTDIR)\ESL_Information.sbr" \
	"$(INTDIR)\ESL_MBCSFileIO.sbr" \
	"$(INTDIR)\ESL_Mgr.sbr" \
	"$(INTDIR)\ESL_UnicodeFileIO.sbr" \
	"$(INTDIR)\esl_worksheet_dlg.sbr" \
	"$(INTDIR)\ESLDlg_Confirm.sbr" \
	"$(INTDIR)\ESLDlg_DM_Viewer.sbr" \
	"$(INTDIR)\ESLDlg_Error.sbr" \
	"$(INTDIR)\ESLDlg_GridConfig.sbr" \
	"$(INTDIR)\ESLDlg_GridConfig_INSERT.sbr" \
	"$(INTDIR)\ESLDlg_Work.sbr" \
	"$(INTDIR)\ExcelManager.sbr" \
	"$(INTDIR)\EXCEPT.sbr" \
	"$(INTDIR)\font.sbr" \
	"$(INTDIR)\JComboBx.sbr" \
	"$(INTDIR)\LIB_F2KEY_SpecialChar.sbr" \
	"$(INTDIR)\LIB_F2KEY_SpecialCharZoom.sbr" \
	"$(INTDIR)\Marc.sbr" \
	"$(INTDIR)\MarcInstruction.sbr" \
	"$(INTDIR)\MarcMgr.sbr" \
	"$(INTDIR)\MarcOperation.sbr" \
	"$(INTDIR)\MarcRelation.sbr" \
	"$(INTDIR)\MarcSimpleRelation.sbr" \
	"$(INTDIR)\mshflexgrid.sbr" \
	"$(INTDIR)\picture.sbr" \
	"$(INTDIR)\recordset.sbr" \
	"$(INTDIR)\rowcursor.sbr" \
	"$(INTDIR)\SPF_Work.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Subfield.sbr" \
	"$(INTDIR)\Tag.sbr" \
	"$(INTDIR)\EFSDMException.sbr"

"$(OUTDIR)\ESL.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=libmySQL.lib imm32.lib ocimgr.lib ijl15l.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ESL.pdb" /debug /machine:I386 /def:".\ESL.def" /out:"..\..\..\..\dll\COMMON\ESL.dll" /implib:"$(OUTDIR)\ESL.lib" /pdbtype:sept 
DEF_FILE= \
	".\ESL.def"
LINK32_OBJS= \
	"$(INTDIR)\_excelmancontrol.obj" \
	"$(INTDIR)\CaptureScreen.obj" \
	"$(INTDIR)\ConvertToHanjaDlg.obj" \
	"$(INTDIR)\DlgClassCode.obj" \
	"$(INTDIR)\DlgHanjaWordInputModify.obj" \
	"$(INTDIR)\DlgHanjaWordManager.obj" \
	"$(INTDIR)\ECO_ControlDateEdit.obj" \
	"$(INTDIR)\ECO_ControlDateEditElem.obj" \
	"$(INTDIR)\EFS.obj" \
	"$(INTDIR)\EFSReportDlg.obj" \
	"$(INTDIR)\ESL.obj" \
	"$(INTDIR)\ESL_CharSetMgr.obj" \
	"$(INTDIR)\ESL_CodeMgr.obj" \
	"$(INTDIR)\ESL_Control_Element.obj" \
	"$(INTDIR)\ESL_ControlComboBox.obj" \
	"$(INTDIR)\ESL_ControlEdit.obj" \
	"$(INTDIR)\ESL_ControlMgr.obj" \
	"$(INTDIR)\ESL_ControlMultiComboBox.obj" \
	"$(INTDIR)\ESL_DataMgr.obj" \
	"$(INTDIR)\ESL_DataVerifyMgr.obj" \
	"$(INTDIR)\ESL_DateTimeCtrl.obj" \
	"$(INTDIR)\ESL_DVM_VIEWER.obj" \
	"$(INTDIR)\ESL_Grid.obj" \
	"$(INTDIR)\ESL_GridColumnInfo.obj" \
	"$(INTDIR)\ESL_GridData.obj" \
	"$(INTDIR)\ESL_GridSelEditor.obj" \
	"$(INTDIR)\ESL_Information.obj" \
	"$(INTDIR)\ESL_MBCSFileIO.obj" \
	"$(INTDIR)\ESL_Mgr.obj" \
	"$(INTDIR)\ESL_UnicodeFileIO.obj" \
	"$(INTDIR)\esl_worksheet_dlg.obj" \
	"$(INTDIR)\ESLDlg_Confirm.obj" \
	"$(INTDIR)\ESLDlg_DM_Viewer.obj" \
	"$(INTDIR)\ESLDlg_Error.obj" \
	"$(INTDIR)\ESLDlg_GridConfig.obj" \
	"$(INTDIR)\ESLDlg_GridConfig_INSERT.obj" \
	"$(INTDIR)\ESLDlg_Work.obj" \
	"$(INTDIR)\ExcelManager.obj" \
	"$(INTDIR)\EXCEPT.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\JComboBx.obj" \
	"$(INTDIR)\LIB_F2KEY_SpecialChar.obj" \
	"$(INTDIR)\LIB_F2KEY_SpecialCharZoom.obj" \
	"$(INTDIR)\Marc.obj" \
	"$(INTDIR)\MarcInstruction.obj" \
	"$(INTDIR)\MarcMgr.obj" \
	"$(INTDIR)\MarcOperation.obj" \
	"$(INTDIR)\MarcRelation.obj" \
	"$(INTDIR)\MarcSimpleRelation.obj" \
	"$(INTDIR)\mshflexgrid.obj" \
	"$(INTDIR)\picture.obj" \
	"$(INTDIR)\recordset.obj" \
	"$(INTDIR)\rowcursor.obj" \
	"$(INTDIR)\SPF_Work.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Subfield.obj" \
	"$(INTDIR)\Tag.obj" \
	"$(INTDIR)\ESL.res" \
	"$(INTDIR)\EFSDMException.obj"

"..\..\..\..\dll\COMMON\ESL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"

OUTDIR=.\Unicode Debug
INTDIR=.\Unicode Debug
# Begin Custom Macros
OutDir=.\Unicode Debug
# End Custom Macros

ALL : "..\..\..\..\bin\ESL.dll" "$(OUTDIR)\ESL.bsc"


CLEAN :
	-@erase "$(INTDIR)\_excelmancontrol.obj"
	-@erase "$(INTDIR)\_excelmancontrol.sbr"
	-@erase "$(INTDIR)\CaptureScreen.obj"
	-@erase "$(INTDIR)\CaptureScreen.sbr"
	-@erase "$(INTDIR)\ConvertToHanjaDlg.obj"
	-@erase "$(INTDIR)\ConvertToHanjaDlg.sbr"
	-@erase "$(INTDIR)\DlgClassCode.obj"
	-@erase "$(INTDIR)\DlgClassCode.sbr"
	-@erase "$(INTDIR)\DlgHanjaWordInputModify.obj"
	-@erase "$(INTDIR)\DlgHanjaWordInputModify.sbr"
	-@erase "$(INTDIR)\DlgHanjaWordManager.obj"
	-@erase "$(INTDIR)\DlgHanjaWordManager.sbr"
	-@erase "$(INTDIR)\ECO_ControlDateEdit.obj"
	-@erase "$(INTDIR)\ECO_ControlDateEdit.sbr"
	-@erase "$(INTDIR)\ECO_ControlDateEditElem.obj"
	-@erase "$(INTDIR)\ECO_ControlDateEditElem.sbr"
	-@erase "$(INTDIR)\EFS.obj"
	-@erase "$(INTDIR)\EFS.sbr"
	-@erase "$(INTDIR)\EFSDMException.obj"
	-@erase "$(INTDIR)\EFSDMException.sbr"
	-@erase "$(INTDIR)\EFSReportDlg.obj"
	-@erase "$(INTDIR)\EFSReportDlg.sbr"
	-@erase "$(INTDIR)\ESL.obj"
	-@erase "$(INTDIR)\ESL.pch"
	-@erase "$(INTDIR)\ESL.res"
	-@erase "$(INTDIR)\ESL.sbr"
	-@erase "$(INTDIR)\ESL_CharSetMgr.obj"
	-@erase "$(INTDIR)\ESL_CharSetMgr.sbr"
	-@erase "$(INTDIR)\ESL_CodeMgr.obj"
	-@erase "$(INTDIR)\ESL_CodeMgr.sbr"
	-@erase "$(INTDIR)\ESL_Control_Element.obj"
	-@erase "$(INTDIR)\ESL_Control_Element.sbr"
	-@erase "$(INTDIR)\ESL_ControlComboBox.obj"
	-@erase "$(INTDIR)\ESL_ControlComboBox.sbr"
	-@erase "$(INTDIR)\ESL_ControlEdit.obj"
	-@erase "$(INTDIR)\ESL_ControlEdit.sbr"
	-@erase "$(INTDIR)\ESL_ControlMgr.obj"
	-@erase "$(INTDIR)\ESL_ControlMgr.sbr"
	-@erase "$(INTDIR)\ESL_ControlMultiComboBox.obj"
	-@erase "$(INTDIR)\ESL_ControlMultiComboBox.sbr"
	-@erase "$(INTDIR)\ESL_DataMgr.obj"
	-@erase "$(INTDIR)\ESL_DataMgr.sbr"
	-@erase "$(INTDIR)\ESL_DataVerifyMgr.obj"
	-@erase "$(INTDIR)\ESL_DataVerifyMgr.sbr"
	-@erase "$(INTDIR)\ESL_DateTimeCtrl.obj"
	-@erase "$(INTDIR)\ESL_DateTimeCtrl.sbr"
	-@erase "$(INTDIR)\ESL_DVM_VIEWER.obj"
	-@erase "$(INTDIR)\ESL_DVM_VIEWER.sbr"
	-@erase "$(INTDIR)\ESL_Grid.obj"
	-@erase "$(INTDIR)\ESL_Grid.sbr"
	-@erase "$(INTDIR)\ESL_GridColumnInfo.obj"
	-@erase "$(INTDIR)\ESL_GridColumnInfo.sbr"
	-@erase "$(INTDIR)\ESL_GridData.obj"
	-@erase "$(INTDIR)\ESL_GridData.sbr"
	-@erase "$(INTDIR)\ESL_GridSelEditor.obj"
	-@erase "$(INTDIR)\ESL_GridSelEditor.sbr"
	-@erase "$(INTDIR)\ESL_Information.obj"
	-@erase "$(INTDIR)\ESL_Information.sbr"
	-@erase "$(INTDIR)\ESL_MBCSFileIO.obj"
	-@erase "$(INTDIR)\ESL_MBCSFileIO.sbr"
	-@erase "$(INTDIR)\ESL_Mgr.obj"
	-@erase "$(INTDIR)\ESL_Mgr.sbr"
	-@erase "$(INTDIR)\ESL_UnicodeFileIO.obj"
	-@erase "$(INTDIR)\ESL_UnicodeFileIO.sbr"
	-@erase "$(INTDIR)\esl_worksheet_dlg.obj"
	-@erase "$(INTDIR)\esl_worksheet_dlg.sbr"
	-@erase "$(INTDIR)\ESLDlg_Confirm.obj"
	-@erase "$(INTDIR)\ESLDlg_Confirm.sbr"
	-@erase "$(INTDIR)\ESLDlg_DM_Viewer.obj"
	-@erase "$(INTDIR)\ESLDlg_DM_Viewer.sbr"
	-@erase "$(INTDIR)\ESLDlg_Error.obj"
	-@erase "$(INTDIR)\ESLDlg_Error.sbr"
	-@erase "$(INTDIR)\ESLDlg_GridConfig.obj"
	-@erase "$(INTDIR)\ESLDlg_GridConfig.sbr"
	-@erase "$(INTDIR)\ESLDlg_GridConfig_INSERT.obj"
	-@erase "$(INTDIR)\ESLDlg_GridConfig_INSERT.sbr"
	-@erase "$(INTDIR)\ESLDlg_Work.obj"
	-@erase "$(INTDIR)\ESLDlg_Work.sbr"
	-@erase "$(INTDIR)\ExcelManager.obj"
	-@erase "$(INTDIR)\ExcelManager.sbr"
	-@erase "$(INTDIR)\EXCEPT.obj"
	-@erase "$(INTDIR)\EXCEPT.sbr"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\font.sbr"
	-@erase "$(INTDIR)\JComboBx.obj"
	-@erase "$(INTDIR)\JComboBx.sbr"
	-@erase "$(INTDIR)\LIB_F2KEY_SpecialChar.obj"
	-@erase "$(INTDIR)\LIB_F2KEY_SpecialChar.sbr"
	-@erase "$(INTDIR)\LIB_F2KEY_SpecialCharZoom.obj"
	-@erase "$(INTDIR)\LIB_F2KEY_SpecialCharZoom.sbr"
	-@erase "$(INTDIR)\Marc.obj"
	-@erase "$(INTDIR)\Marc.sbr"
	-@erase "$(INTDIR)\MarcInstruction.obj"
	-@erase "$(INTDIR)\MarcInstruction.sbr"
	-@erase "$(INTDIR)\MarcMgr.obj"
	-@erase "$(INTDIR)\MarcMgr.sbr"
	-@erase "$(INTDIR)\MarcOperation.obj"
	-@erase "$(INTDIR)\MarcOperation.sbr"
	-@erase "$(INTDIR)\MarcRelation.obj"
	-@erase "$(INTDIR)\MarcRelation.sbr"
	-@erase "$(INTDIR)\MarcSimpleRelation.obj"
	-@erase "$(INTDIR)\MarcSimpleRelation.sbr"
	-@erase "$(INTDIR)\mshflexgrid.obj"
	-@erase "$(INTDIR)\mshflexgrid.sbr"
	-@erase "$(INTDIR)\picture.obj"
	-@erase "$(INTDIR)\picture.sbr"
	-@erase "$(INTDIR)\recordset.obj"
	-@erase "$(INTDIR)\recordset.sbr"
	-@erase "$(INTDIR)\rowcursor.obj"
	-@erase "$(INTDIR)\rowcursor.sbr"
	-@erase "$(INTDIR)\SPF_Work.obj"
	-@erase "$(INTDIR)\SPF_Work.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Subfield.obj"
	-@erase "$(INTDIR)\Subfield.sbr"
	-@erase "$(INTDIR)\Tag.obj"
	-@erase "$(INTDIR)\Tag.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ESL.bsc"
	-@erase "$(OUTDIR)\ESL.pdb"
	-@erase "..\..\..\..\bin\ESL.dll"
	-@erase "..\..\..\..\bin\ESL.ilk"
	-@erase "..\..\lib\ESL.exp"
	-@erase "..\..\lib\ESL.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ESL.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ESL.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ESL.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\_excelmancontrol.sbr" \
	"$(INTDIR)\CaptureScreen.sbr" \
	"$(INTDIR)\ConvertToHanjaDlg.sbr" \
	"$(INTDIR)\DlgClassCode.sbr" \
	"$(INTDIR)\DlgHanjaWordInputModify.sbr" \
	"$(INTDIR)\DlgHanjaWordManager.sbr" \
	"$(INTDIR)\ECO_ControlDateEdit.sbr" \
	"$(INTDIR)\ECO_ControlDateEditElem.sbr" \
	"$(INTDIR)\EFS.sbr" \
	"$(INTDIR)\EFSReportDlg.sbr" \
	"$(INTDIR)\ESL.sbr" \
	"$(INTDIR)\ESL_CharSetMgr.sbr" \
	"$(INTDIR)\ESL_CodeMgr.sbr" \
	"$(INTDIR)\ESL_Control_Element.sbr" \
	"$(INTDIR)\ESL_ControlComboBox.sbr" \
	"$(INTDIR)\ESL_ControlEdit.sbr" \
	"$(INTDIR)\ESL_ControlMgr.sbr" \
	"$(INTDIR)\ESL_ControlMultiComboBox.sbr" \
	"$(INTDIR)\ESL_DataMgr.sbr" \
	"$(INTDIR)\ESL_DataVerifyMgr.sbr" \
	"$(INTDIR)\ESL_DateTimeCtrl.sbr" \
	"$(INTDIR)\ESL_DVM_VIEWER.sbr" \
	"$(INTDIR)\ESL_Grid.sbr" \
	"$(INTDIR)\ESL_GridColumnInfo.sbr" \
	"$(INTDIR)\ESL_GridData.sbr" \
	"$(INTDIR)\ESL_GridSelEditor.sbr" \
	"$(INTDIR)\ESL_Information.sbr" \
	"$(INTDIR)\ESL_MBCSFileIO.sbr" \
	"$(INTDIR)\ESL_Mgr.sbr" \
	"$(INTDIR)\ESL_UnicodeFileIO.sbr" \
	"$(INTDIR)\esl_worksheet_dlg.sbr" \
	"$(INTDIR)\ESLDlg_Confirm.sbr" \
	"$(INTDIR)\ESLDlg_DM_Viewer.sbr" \
	"$(INTDIR)\ESLDlg_Error.sbr" \
	"$(INTDIR)\ESLDlg_GridConfig.sbr" \
	"$(INTDIR)\ESLDlg_GridConfig_INSERT.sbr" \
	"$(INTDIR)\ESLDlg_Work.sbr" \
	"$(INTDIR)\ExcelManager.sbr" \
	"$(INTDIR)\EXCEPT.sbr" \
	"$(INTDIR)\font.sbr" \
	"$(INTDIR)\JComboBx.sbr" \
	"$(INTDIR)\LIB_F2KEY_SpecialChar.sbr" \
	"$(INTDIR)\LIB_F2KEY_SpecialCharZoom.sbr" \
	"$(INTDIR)\Marc.sbr" \
	"$(INTDIR)\MarcInstruction.sbr" \
	"$(INTDIR)\MarcMgr.sbr" \
	"$(INTDIR)\MarcOperation.sbr" \
	"$(INTDIR)\MarcRelation.sbr" \
	"$(INTDIR)\MarcSimpleRelation.sbr" \
	"$(INTDIR)\mshflexgrid.sbr" \
	"$(INTDIR)\picture.sbr" \
	"$(INTDIR)\recordset.sbr" \
	"$(INTDIR)\rowcursor.sbr" \
	"$(INTDIR)\SPF_Work.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Subfield.sbr" \
	"$(INTDIR)\Tag.sbr" \
	"$(INTDIR)\EFSDMException.sbr"

"$(OUTDIR)\ESL.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=libmySQL.lib imm32.lib ocimgr.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ESL.pdb" /debug /machine:I386 /def:".\ESL.def" /out:"..\..\..\..\bin\ESL.dll" /implib:"..\..\lib\ESL.lib" /pdbtype:sept 
DEF_FILE= \
	".\ESL.def"
LINK32_OBJS= \
	"$(INTDIR)\_excelmancontrol.obj" \
	"$(INTDIR)\CaptureScreen.obj" \
	"$(INTDIR)\ConvertToHanjaDlg.obj" \
	"$(INTDIR)\DlgClassCode.obj" \
	"$(INTDIR)\DlgHanjaWordInputModify.obj" \
	"$(INTDIR)\DlgHanjaWordManager.obj" \
	"$(INTDIR)\ECO_ControlDateEdit.obj" \
	"$(INTDIR)\ECO_ControlDateEditElem.obj" \
	"$(INTDIR)\EFS.obj" \
	"$(INTDIR)\EFSReportDlg.obj" \
	"$(INTDIR)\ESL.obj" \
	"$(INTDIR)\ESL_CharSetMgr.obj" \
	"$(INTDIR)\ESL_CodeMgr.obj" \
	"$(INTDIR)\ESL_Control_Element.obj" \
	"$(INTDIR)\ESL_ControlComboBox.obj" \
	"$(INTDIR)\ESL_ControlEdit.obj" \
	"$(INTDIR)\ESL_ControlMgr.obj" \
	"$(INTDIR)\ESL_ControlMultiComboBox.obj" \
	"$(INTDIR)\ESL_DataMgr.obj" \
	"$(INTDIR)\ESL_DataVerifyMgr.obj" \
	"$(INTDIR)\ESL_DateTimeCtrl.obj" \
	"$(INTDIR)\ESL_DVM_VIEWER.obj" \
	"$(INTDIR)\ESL_Grid.obj" \
	"$(INTDIR)\ESL_GridColumnInfo.obj" \
	"$(INTDIR)\ESL_GridData.obj" \
	"$(INTDIR)\ESL_GridSelEditor.obj" \
	"$(INTDIR)\ESL_Information.obj" \
	"$(INTDIR)\ESL_MBCSFileIO.obj" \
	"$(INTDIR)\ESL_Mgr.obj" \
	"$(INTDIR)\ESL_UnicodeFileIO.obj" \
	"$(INTDIR)\esl_worksheet_dlg.obj" \
	"$(INTDIR)\ESLDlg_Confirm.obj" \
	"$(INTDIR)\ESLDlg_DM_Viewer.obj" \
	"$(INTDIR)\ESLDlg_Error.obj" \
	"$(INTDIR)\ESLDlg_GridConfig.obj" \
	"$(INTDIR)\ESLDlg_GridConfig_INSERT.obj" \
	"$(INTDIR)\ESLDlg_Work.obj" \
	"$(INTDIR)\ExcelManager.obj" \
	"$(INTDIR)\EXCEPT.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\JComboBx.obj" \
	"$(INTDIR)\LIB_F2KEY_SpecialChar.obj" \
	"$(INTDIR)\LIB_F2KEY_SpecialCharZoom.obj" \
	"$(INTDIR)\Marc.obj" \
	"$(INTDIR)\MarcInstruction.obj" \
	"$(INTDIR)\MarcMgr.obj" \
	"$(INTDIR)\MarcOperation.obj" \
	"$(INTDIR)\MarcRelation.obj" \
	"$(INTDIR)\MarcSimpleRelation.obj" \
	"$(INTDIR)\mshflexgrid.obj" \
	"$(INTDIR)\picture.obj" \
	"$(INTDIR)\recordset.obj" \
	"$(INTDIR)\rowcursor.obj" \
	"$(INTDIR)\SPF_Work.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Subfield.obj" \
	"$(INTDIR)\Tag.obj" \
	"$(INTDIR)\ESL.res" \
	"$(INTDIR)\EFSDMException.obj"

"..\..\..\..\bin\ESL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("ESL.dep")
!INCLUDE "ESL.dep"
!ELSE 
!MESSAGE Warning: cannot find "ESL.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ESL - Win32 Release" || "$(CFG)" == "ESL - Win32 Debug" || "$(CFG)" == "ESL - Win32 Unicode Debug"
SOURCE=.\_excelmancontrol.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\_excelmancontrol.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\_excelmancontrol.obj"	"$(INTDIR)\_excelmancontrol.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\_excelmancontrol.obj"	"$(INTDIR)\_excelmancontrol.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\CaptureScreen.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\CaptureScreen.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\CaptureScreen.obj"	"$(INTDIR)\CaptureScreen.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\CaptureScreen.obj"	"$(INTDIR)\CaptureScreen.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ConvertToHanjaDlg.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ConvertToHanjaDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ConvertToHanjaDlg.obj"	"$(INTDIR)\ConvertToHanjaDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ConvertToHanjaDlg.obj"	"$(INTDIR)\ConvertToHanjaDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\DlgClassCode.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\DlgClassCode.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\DlgClassCode.obj"	"$(INTDIR)\DlgClassCode.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\DlgClassCode.obj"	"$(INTDIR)\DlgClassCode.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\DlgHanjaWordInputModify.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\DlgHanjaWordInputModify.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\DlgHanjaWordInputModify.obj"	"$(INTDIR)\DlgHanjaWordInputModify.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\DlgHanjaWordInputModify.obj"	"$(INTDIR)\DlgHanjaWordInputModify.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\DlgHanjaWordManager.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\DlgHanjaWordManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\DlgHanjaWordManager.obj"	"$(INTDIR)\DlgHanjaWordManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\DlgHanjaWordManager.obj"	"$(INTDIR)\DlgHanjaWordManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ECO_ControlDateEdit.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ECO_ControlDateEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ECO_ControlDateEdit.obj"	"$(INTDIR)\ECO_ControlDateEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ECO_ControlDateEdit.obj"	"$(INTDIR)\ECO_ControlDateEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ECO_ControlDateEditElem.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ECO_ControlDateEditElem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ECO_ControlDateEditElem.obj"	"$(INTDIR)\ECO_ControlDateEditElem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ECO_ControlDateEditElem.obj"	"$(INTDIR)\ECO_ControlDateEditElem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\EFS.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\EFS.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\EFS.obj"	"$(INTDIR)\EFS.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\EFS.obj"	"$(INTDIR)\EFS.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\EFSDMException.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\EFSDMException.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\EFSDMException.obj"	"$(INTDIR)\EFSDMException.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\EFSDMException.obj"	"$(INTDIR)\EFSDMException.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\EFSReportDlg.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\EFSReportDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\EFSReportDlg.obj"	"$(INTDIR)\EFSReportDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\EFSReportDlg.obj"	"$(INTDIR)\EFSReportDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL.obj"	"$(INTDIR)\ESL.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL.obj"	"$(INTDIR)\ESL.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL.rc

"$(INTDIR)\ESL.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ESL_CharSetMgr.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_CharSetMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_CharSetMgr.obj"	"$(INTDIR)\ESL_CharSetMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_CharSetMgr.obj"	"$(INTDIR)\ESL_CharSetMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_CodeMgr.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_CodeMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_CodeMgr.obj"	"$(INTDIR)\ESL_CodeMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_CodeMgr.obj"	"$(INTDIR)\ESL_CodeMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_Control_Element.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_Control_Element.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_Control_Element.obj"	"$(INTDIR)\ESL_Control_Element.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_Control_Element.obj"	"$(INTDIR)\ESL_Control_Element.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_ControlComboBox.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_ControlComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_ControlComboBox.obj"	"$(INTDIR)\ESL_ControlComboBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_ControlComboBox.obj"	"$(INTDIR)\ESL_ControlComboBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_ControlEdit.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_ControlEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_ControlEdit.obj"	"$(INTDIR)\ESL_ControlEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_ControlEdit.obj"	"$(INTDIR)\ESL_ControlEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_ControlMgr.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_ControlMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_ControlMgr.obj"	"$(INTDIR)\ESL_ControlMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_ControlMgr.obj"	"$(INTDIR)\ESL_ControlMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_ControlMultiComboBox.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_ControlMultiComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_ControlMultiComboBox.obj"	"$(INTDIR)\ESL_ControlMultiComboBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_ControlMultiComboBox.obj"	"$(INTDIR)\ESL_ControlMultiComboBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_DataMgr.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_DataMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_DataMgr.obj"	"$(INTDIR)\ESL_DataMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_DataMgr.obj"	"$(INTDIR)\ESL_DataMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_DataVerifyMgr.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_DataVerifyMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_DataVerifyMgr.obj"	"$(INTDIR)\ESL_DataVerifyMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_DataVerifyMgr.obj"	"$(INTDIR)\ESL_DataVerifyMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_DateTimeCtrl.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_DateTimeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_DateTimeCtrl.obj"	"$(INTDIR)\ESL_DateTimeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_DateTimeCtrl.obj"	"$(INTDIR)\ESL_DateTimeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_DVM_VIEWER.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_DVM_VIEWER.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_DVM_VIEWER.obj"	"$(INTDIR)\ESL_DVM_VIEWER.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_DVM_VIEWER.obj"	"$(INTDIR)\ESL_DVM_VIEWER.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_Grid.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_Grid.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_Grid.obj"	"$(INTDIR)\ESL_Grid.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_Grid.obj"	"$(INTDIR)\ESL_Grid.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_GridColumnInfo.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_GridColumnInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_GridColumnInfo.obj"	"$(INTDIR)\ESL_GridColumnInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_GridColumnInfo.obj"	"$(INTDIR)\ESL_GridColumnInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_GridData.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_GridData.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_GridData.obj"	"$(INTDIR)\ESL_GridData.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_GridData.obj"	"$(INTDIR)\ESL_GridData.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_GridSelEditor.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_GridSelEditor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_GridSelEditor.obj"	"$(INTDIR)\ESL_GridSelEditor.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_GridSelEditor.obj"	"$(INTDIR)\ESL_GridSelEditor.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_Information.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_Information.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_Information.obj"	"$(INTDIR)\ESL_Information.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_Information.obj"	"$(INTDIR)\ESL_Information.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_MBCSFileIO.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_MBCSFileIO.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_MBCSFileIO.obj"	"$(INTDIR)\ESL_MBCSFileIO.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_MBCSFileIO.obj"	"$(INTDIR)\ESL_MBCSFileIO.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_Mgr.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_Mgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_Mgr.obj"	"$(INTDIR)\ESL_Mgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_Mgr.obj"	"$(INTDIR)\ESL_Mgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESL_UnicodeFileIO.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESL_UnicodeFileIO.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESL_UnicodeFileIO.obj"	"$(INTDIR)\ESL_UnicodeFileIO.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESL_UnicodeFileIO.obj"	"$(INTDIR)\ESL_UnicodeFileIO.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\esl_worksheet_dlg.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\esl_worksheet_dlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\esl_worksheet_dlg.obj"	"$(INTDIR)\esl_worksheet_dlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\esl_worksheet_dlg.obj"	"$(INTDIR)\esl_worksheet_dlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESLDlg_Confirm.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESLDlg_Confirm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESLDlg_Confirm.obj"	"$(INTDIR)\ESLDlg_Confirm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESLDlg_Confirm.obj"	"$(INTDIR)\ESLDlg_Confirm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESLDlg_DM_Viewer.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESLDlg_DM_Viewer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESLDlg_DM_Viewer.obj"	"$(INTDIR)\ESLDlg_DM_Viewer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESLDlg_DM_Viewer.obj"	"$(INTDIR)\ESLDlg_DM_Viewer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESLDlg_Error.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESLDlg_Error.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESLDlg_Error.obj"	"$(INTDIR)\ESLDlg_Error.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESLDlg_Error.obj"	"$(INTDIR)\ESLDlg_Error.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESLDlg_GridConfig.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESLDlg_GridConfig.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESLDlg_GridConfig.obj"	"$(INTDIR)\ESLDlg_GridConfig.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESLDlg_GridConfig.obj"	"$(INTDIR)\ESLDlg_GridConfig.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESLDlg_GridConfig_INSERT.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESLDlg_GridConfig_INSERT.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESLDlg_GridConfig_INSERT.obj"	"$(INTDIR)\ESLDlg_GridConfig_INSERT.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESLDlg_GridConfig_INSERT.obj"	"$(INTDIR)\ESLDlg_GridConfig_INSERT.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ESLDlg_Work.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ESLDlg_Work.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ESLDlg_Work.obj"	"$(INTDIR)\ESLDlg_Work.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ESLDlg_Work.obj"	"$(INTDIR)\ESLDlg_Work.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\ExcelManager.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\ExcelManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\ExcelManager.obj"	"$(INTDIR)\ExcelManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\ExcelManager.obj"	"$(INTDIR)\ExcelManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\EXCEPT.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\EXCEPT.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\EXCEPT.obj"	"$(INTDIR)\EXCEPT.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\EXCEPT.obj"	"$(INTDIR)\EXCEPT.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\font.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\font.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\font.obj"	"$(INTDIR)\font.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\font.obj"	"$(INTDIR)\font.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\JComboBx.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\JComboBx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\JComboBx.obj"	"$(INTDIR)\JComboBx.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\JComboBx.obj"	"$(INTDIR)\JComboBx.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\LIB_F2KEY_SpecialChar.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\LIB_F2KEY_SpecialChar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\LIB_F2KEY_SpecialChar.obj"	"$(INTDIR)\LIB_F2KEY_SpecialChar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\LIB_F2KEY_SpecialChar.obj"	"$(INTDIR)\LIB_F2KEY_SpecialChar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\LIB_F2KEY_SpecialCharZoom.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\LIB_F2KEY_SpecialCharZoom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\LIB_F2KEY_SpecialCharZoom.obj"	"$(INTDIR)\LIB_F2KEY_SpecialCharZoom.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\LIB_F2KEY_SpecialCharZoom.obj"	"$(INTDIR)\LIB_F2KEY_SpecialCharZoom.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\Marc.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\Marc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\Marc.obj"	"$(INTDIR)\Marc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\Marc.obj"	"$(INTDIR)\Marc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\MarcInstruction.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\MarcInstruction.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\MarcInstruction.obj"	"$(INTDIR)\MarcInstruction.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\MarcInstruction.obj"	"$(INTDIR)\MarcInstruction.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\MarcMgr.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\MarcMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\MarcMgr.obj"	"$(INTDIR)\MarcMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\MarcMgr.obj"	"$(INTDIR)\MarcMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\MarcOperation.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\MarcOperation.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\MarcOperation.obj"	"$(INTDIR)\MarcOperation.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\MarcOperation.obj"	"$(INTDIR)\MarcOperation.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\MarcRelation.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\MarcRelation.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\MarcRelation.obj"	"$(INTDIR)\MarcRelation.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\MarcRelation.obj"	"$(INTDIR)\MarcRelation.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\MarcSimpleRelation.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\MarcSimpleRelation.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\MarcSimpleRelation.obj"	"$(INTDIR)\MarcSimpleRelation.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\MarcSimpleRelation.obj"	"$(INTDIR)\MarcSimpleRelation.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\mshflexgrid.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\mshflexgrid.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\mshflexgrid.obj"	"$(INTDIR)\mshflexgrid.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\mshflexgrid.obj"	"$(INTDIR)\mshflexgrid.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\picture.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\picture.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\picture.obj"	"$(INTDIR)\picture.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\picture.obj"	"$(INTDIR)\picture.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\recordset.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\recordset.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\recordset.obj"	"$(INTDIR)\recordset.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\recordset.obj"	"$(INTDIR)\recordset.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\rowcursor.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\rowcursor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\rowcursor.obj"	"$(INTDIR)\rowcursor.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\rowcursor.obj"	"$(INTDIR)\rowcursor.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\SPF_Work.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\SPF_Work.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\SPF_Work.obj"	"$(INTDIR)\SPF_Work.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\SPF_Work.obj"	"$(INTDIR)\SPF_Work.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\ESL.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ESL.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_MBCS" /D "_WINDLL" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ESL.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ESL.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ESL.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ESL.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Subfield.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\Subfield.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\Subfield.obj"	"$(INTDIR)\Subfield.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\Subfield.obj"	"$(INTDIR)\Subfield.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 

SOURCE=.\Tag.cpp

!IF  "$(CFG)" == "ESL - Win32 Release"


"$(INTDIR)\Tag.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Debug"


"$(INTDIR)\Tag.obj"	"$(INTDIR)\Tag.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ELSEIF  "$(CFG)" == "ESL - Win32 Unicode Debug"


"$(INTDIR)\Tag.obj"	"$(INTDIR)\Tag.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ESL.pch"


!ENDIF 


!ENDIF 

