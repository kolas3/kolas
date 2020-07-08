# Microsoft Developer Studio Project File - Name="MarcAdjust" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MarcAdjust - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MarcAdjust.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MarcAdjust.mak" CFG="MarcAdjust - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MarcAdjust - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "MarcAdjust___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "MarcAdjust___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib/"
# PROP Intermediate_Dir "MarcAdjust___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imm32.lib esl.lib MarcAdjustApi.lib MarcEdit.lib MarcToolKit.lib BO_ACQ_SPECIES_FORM.lib BO_VOLUME_FORM.lib BO_SPECIES_API.lib SE_VOLUME_FORM.lib BO_CAT_PROC_FIXEDFIELD.lib Duplicate_Data_Search.lib SIReportMgr.lib SiReportData.lib K2UP_Common.lib BO_CAT_PROC_KEYWORD.lib BO_CAT_PROC.lib Bo_Marc_Import.lib Light_GatherDataSearch.lib UI_SEARCH_DLG.lib ShelfCodeGetter.lib CatCodeGetter.lib MNG_CLASS_SIGN_POPUP.lib BO_CAT_NUMBER.LIB IndexToolKit.lib BO_ACQ_GROUP_MANAGER.lib TOCEDITOR.lib BO_CAT_REGNO_DLG.lib MPropertyGrid.lib Light_BO_CAT_CENTERUPLOAD.lib ContentsMgr_Editor.lib Kolas2up_commonControl.lib CAT_FIXED_FIELD.lib MngCharSetManagement.lib BO_SPECIES_API.LIB COMMON_EBOOK_API.LIB MNG_AC_MANAGER.lib SE_CAT_REGNO_DLG.lib WEB_HOLD_IMPORT.lib COVERMANAGER.lib SUBJECT_INDEX_SEARCH.lib MOKCHA_CHOROK_API.lib BO_DATA_MANAGER.lib DATA_SORT_MANAGER.lib COMMON_DONATOR_MANAGER.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\COMMON\MarcAdjust.dll" /pdbtype:sept
# ADD LINK32 imm32.lib esl.lib MarcAdjustApi.lib MarcEdit.lib MarcToolKit.lib BO_ACQ_SPECIES_FORM.lib BO_VOLUME_FORM.lib BO_SPECIES_API.lib SE_VOLUME_FORM.lib BO_CAT_PROC_FIXEDFIELD.lib Duplicate_Data_Search.lib SIReportMgr.lib SiReportData.lib K2UP_Common.lib BO_CAT_PROC_KEYWORD.lib BO_CAT_PROC.lib Bo_Marc_Import.lib Light_GatherDataSearch.lib UI_SEARCH_DLG.lib ShelfCodeGetter.lib CatCodeGetter.lib MNG_CLASS_SIGN_POPUP.lib BO_CAT_NUMBER.LIB IndexToolKit.lib BO_ACQ_GROUP_MANAGER.lib TOCEDITOR.lib BO_CAT_REGNO_DLG.lib MPropertyGrid.lib Light_BO_CAT_CENTERUPLOAD.lib ContentsMgr_Editor.lib Kolas2up_commonControl.lib CAT_FIXED_FIELD.lib MngCharSetManagement.lib BO_SPECIES_API.LIB COMMON_EBOOK_API.LIB MNG_AC_MANAGER.lib SE_CAT_REGNO_DLG.lib WEB_HOLD_IMPORT.lib COVERMANAGER.lib SUBJECT_INDEX_SEARCH.lib MOKCHA_CHOROK_API.lib BO_DATA_MANAGER.lib DATA_SORT_MANAGER.lib COMMON_DONATOR_MANAGER.lib CharEncodeMgr.lib CAT_UNION_FIXED_FIELD.lib KolisNet_Api.lib Kolas2up_MobileCommon.lib KL_Api.lib Loc_CommonAPI.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\COMMON\MarcAdjust.dll" /pdbtype:sept
# Begin Target

# Name "MarcAdjust - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BO_CAT_AUTO_REGNO_GIVE.cpp
# End Source File
# Begin Source File

SOURCE=.\CatESL_Mgr.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\HoldInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LocRecordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjCodeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjLauncher.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjMarcEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjMarcGridView.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjMgr_BO.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjMgr_SE.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjMgrMemoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjResourceMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjThread.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjust.def
# End Source File
# Begin Source File

SOURCE=.\MarcAdjust.rc
# End Source File
# Begin Source File

SOURCE=.\MarcAdjVolumeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuItem.cpp
# End Source File
# Begin Source File

SOURCE=.\SpeciesDeletionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TOOLBARBTN.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarItem.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BO_CAT_AUTO_REGNO_GIVE.h
# End Source File
# Begin Source File

SOURCE=.\CatESL_Mgr.h
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\HoldInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\LocRecordDlg.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjCodeDlg.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjComboBox.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjDoc.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjEdit.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjFrame.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjLauncher.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjMarcEditView.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjMarcGridView.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjMgr.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjMgr_BO.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjMgr_SE.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjMgrMemoDlg.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjResourceMgr.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjThread.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjustConstant.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjVolumeDlg.h
# End Source File
# Begin Source File

SOURCE=.\MarcToolBar.h
# End Source File
# Begin Source File

SOURCE=.\MenuItem.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SpeciesDeletionDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TOOLBARBTN.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarItem.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\046_on.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icon32.ICO
# End Source File
# Begin Source File

SOURCE=.\res\MarcAdjust.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
