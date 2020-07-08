# Microsoft Developer Studio Project File - Name="ESL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ESL - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ESL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ESL.mak" CFG="ESL - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ESL - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ESL___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "ESL___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\LIB"
# PROP Intermediate_Dir "Unicode Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
GSC=SPP_GramComp.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_AFXEXT" /D "_WINDLL" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "USE_DIRECT_ARIA_PROVIDER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libmySQL.lib imm32.lib ocimgr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\bin\ESL.dll" /implib:"..\..\lib\ESL.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 IMM32.LIB OCIMGR.LIB HTMLHELP.LIB KOLISNetWebSearch.lib UI_SEARCH_DLG.lib CharEncodeMgr.lib ECO_SearchMgr.lib KL_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\DLL\COMMON\ESL.DLL" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "ESL - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\_excelmancontrol.cpp
# End Source File
# Begin Source File

SOURCE=.\COM_File_Mgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ConvertToHanjaDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\des.cpp
# End Source File
# Begin Source File

SOURCE=.\desenc.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgClassCode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHanjaWordInputModify.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHanjaWordManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ECO_ControlAutoList.cpp
# End Source File
# Begin Source File

SOURCE=.\ECO_ControlDateEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ECO_ControlDateEditElem.cpp
# End Source File
# Begin Source File

SOURCE=.\ECO_ControlHourEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ECO_ControlHourEditElem.cpp
# End Source File
# Begin Source File

SOURCE=.\ECO_FILE_API.cpp
# End Source File
# Begin Source File

SOURCE=.\ECO_FileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\EFS.cpp
# End Source File
# Begin Source File

SOURCE=.\EFSDMException.cpp
# End Source File
# Begin Source File

SOURCE=.\EFSReportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL.def
# End Source File
# Begin Source File

SOURCE=.\ESL.rc
# End Source File
# Begin Source File

SOURCE=.\ESL_CharSetMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_CodeMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_Control_Element.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_ControlComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_ControlEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_ControlMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_ControlMultiComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_DataMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_DataMgr1.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_DataVerifyMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_DateTimeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_DVM_VIEWER.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_Grid.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_Grid2.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_GridColumnInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_GridData.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_GridSelEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_Information.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_MBCSFileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_Mgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_SearchEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_UnicodeFileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\esl_worksheet_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_Confirm.cpp
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_DM_Viewer.cpp
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_Error.cpp
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_ExcelExportWait.cpp
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_ExportType.cpp
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_FindWord.cpp
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_GridConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_GridConfig_INSERT.cpp
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_UserExportReason.cpp
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_Work.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcelManager.cpp
# End Source File
# Begin Source File

SOURCE=.\EXCEPT.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\FWDES.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellDateTime.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellNumeric.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridURLCell.cpp
# End Source File
# Begin Source File

SOURCE=.\hancelmanager.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\JComboBx.cpp
# End Source File
# Begin Source File

SOURCE=.\LIB_F2KEY_SpecialChar.cpp
# End Source File
# Begin Source File

SOURCE=.\LIB_F2KEY_SpecialCharZoom.cpp
# End Source File
# Begin Source File

SOURCE=.\Marc.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcInstruction.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcOperation.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcRelation.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcSimpleRelation.cpp
# End Source File
# Begin Source File

SOURCE=.\mshflexgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\MSHFlexGridInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\query_field.cpp
# End Source File
# Begin Source File

SOURCE=.\recordset.cpp
# End Source File
# Begin Source File

SOURCE=.\rowcursor.cpp
# End Source File
# Begin Source File

SOURCE=.\SPF_Work.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Subfield.cpp
# End Source File
# Begin Source File

SOURCE=.\Tag.cpp
# End Source File
# Begin Source File

SOURCE=.\ThemeUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\XLAutomation.cpp
# End Source File
# Begin Source File

SOURCE=.\XLEzAutomation.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\include\ESL\_excelmancontrol.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\COM_File_Mgr.h
# End Source File
# Begin Source File

SOURCE=.\ConvertToHanjaDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\des.h
# End Source File
# Begin Source File

SOURCE=.\DlgClassCode.h
# End Source File
# Begin Source File

SOURCE=.\DlgHanjaWordInputModify.h
# End Source File
# Begin Source File

SOURCE=.\DlgHanjaWordManager.h
# End Source File
# Begin Source File

SOURCE=.\ECO_ControlAutoList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ECO_ControlDateEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ECO_ControlDateEditElem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ECO_ControlHourEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ECO_ControlHourEditElem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ECO_FILE_API.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ECO_FileIO.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\EFS.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\EFSDMException.h
# End Source File
# Begin Source File

SOURCE=.\EFSReportDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_CharSetMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_CodeMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_Constant.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_Control_Element.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_ControlComboBox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_ControlEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_ControlMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_ControlMultiComboBox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_DataMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_DataVerifyMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_DateTimeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ESL_DVM_VIEWER.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_Grid.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_Grid2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_GridColumnInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_GridData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_GridSelEditor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_Information.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_MBCSFileIO.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_Mgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_SearchEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESL_UnicodeFileIO.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Esl\esl_worksheet_dlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESLDlg_Confirm.h
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_Confirm.h
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_DM_Viewer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESLDlg_Error.h
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_Error.h
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_ExcelExportWait.h
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_ExportType.h
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_FindWord.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESLDlg_GridConfig.h
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_GridConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESLDlg_GridConfig_INSERT.h
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_GridConfig_INSERT.h
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_UserExportReason.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ESLDlg_Work.h
# End Source File
# Begin Source File

SOURCE=.\ESLDlg_Work.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\ExcelManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\Except.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\font.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\FWDES.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\GridCell.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\GridCellCheck.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\GridCellCombo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\GridCellDateTime.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\GridCellLite.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\GridCellNumeric.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\GridURLCell.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\InPlaceList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\JComboBx.h
# End Source File
# Begin Source File

SOURCE=.\LIB_F2KEY_SpecialChar.h
# End Source File
# Begin Source File

SOURCE=.\LIB_F2KEY_SpecialCharZoom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\Marc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\MarcConstant.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\MarcInstruction.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\MarcMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\MarcOperation.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\MarcRelation.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\MarcSimpleRelation.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\MemDC.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\mshflexgrid.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\MSHFlexGridInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\picture.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\query_field.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\recordset.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\rowcursor.h
# End Source File
# Begin Source File

SOURCE=.\Schemadef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\SPF_Work.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\Subfield.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\Tag.h
# End Source File
# Begin Source File

SOURCE=.\ThemeUtil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\TitleTip.h
# End Source File
# Begin Source File

SOURCE=.\Tmschema.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\Win32api.h
# End Source File
# Begin Source File

SOURCE=.\XLAutomation.h
# End Source File
# Begin Source File

SOURCE=.\XLEzAutomation.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\BMP\ADDD.bmp
# End Source File
# Begin Source File

SOURCE=.\BMP\CANCELD.bmp
# End Source File
# Begin Source File

SOURCE=.\BMP\CLOSED.bmp
# End Source File
# Begin Source File

SOURCE=.\BMP\DELETED.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ESL.rc2
# End Source File
# Begin Source File

SOURCE=.\INFO.ICO
# End Source File
# Begin Source File

SOURCE=.\BMP\LEFTD.bmp
# End Source File
# Begin Source File

SOURCE=.\BMP\OKD.bmp
# End Source File
# Begin Source File

SOURCE=.\BMP\RIGHTD.bmp
# End Source File
# Begin Source File

SOURCE=.\BMP\SAVED.bmp
# End Source File
# Begin Source File

SOURCE=.\BMP\UPDATED.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=".\Text\@CESL_CodeMgr.txt"
# End Source File
# Begin Source File

SOURCE=".\Text\@CESL_ControlMultiComboBox.txt"
# End Source File
# Begin Source File

SOURCE=".\Text\@CESL_DataMgr.txt"
# End Source File
# Begin Source File

SOURCE=".\Text\@CESL_Grid.txt"
# End Source File
# Begin Source File

SOURCE=".\Text\CESL_MgrÀÛ¼º¹ý.txt"
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section ESL : {0ECD9B64-23AA-11D0-B351-00A0C9055D8E}
# 	2:21:DefaultSinkHeaderFile:mshflexgrid.h
# 	2:16:DefaultSinkClass:CMSHFlexGrid
# End Section
# Section ESL : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	Section TEST1 : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section TEST1 : {6262D3A0-531B-11CF-91F6-C2863C385E30}
# 	2:21:DefaultSinkHeaderFile:msflexgrid.h
# 	2:16:DefaultSinkClass:CMSFlexGrid
# End Section
