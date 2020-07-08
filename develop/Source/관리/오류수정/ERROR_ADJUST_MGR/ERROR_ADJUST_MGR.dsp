# Microsoft Developer Studio Project File - Name="ERROR_ADJUST_MGR" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ERROR_ADJUST_MGR - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ERROR_ADJUST_MGR.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ERROR_ADJUST_MGR.mak" CFG="ERROR_ADJUST_MGR - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ERROR_ADJUST_MGR - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "ERROR_ADJUST_MGR___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "ERROR_ADJUST_MGR___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "ERROR_ADJUST_MGR___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_MBCS" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D _WIN32_WINNT>=0x0501 /D _WIN32_IE>=0x0600 /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_AFXEXT" /D "_DEBUG" /D _WIN32_WINNT>=0x0501 /D _WIN32_IE>=0x0600 /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib IndexToolKit.lib MarcEdit.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\common\ERROR_ADJUST_MGR.dll" /pdbtype:sept
# ADD LINK32 esl.lib IndexToolKit.lib MarcEdit.lib CharEncodeMgr.lib BO_LOC_3000.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\common\ERROR_ADJUST_MGR.dll" /pdbtype:sept
# Begin Target

# Name "ERROR_ADJUST_MGR - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AccessionNoSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyCodeMarcInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CurrencyCodeMarcInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DataProcessingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DropRegNoApplyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ERROR_ADJUST_MGR.cpp
# End Source File
# Begin Source File

SOURCE=.\ERROR_ADJUST_MGR.def
# End Source File
# Begin Source File

SOURCE=.\ERROR_ADJUST_MGR.rc
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustApi.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustDlgMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustLauncher.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustResourceMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustSearchMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustThread.cpp
# End Source File
# Begin Source File

SOURCE=.\IndexErrorAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\IndexRestructSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManageNoErrorAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\ManageNoErrorSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjustCondition.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcAdjustDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcDataErrorAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuItem.cpp
# End Source File
# Begin Source File

SOURCE=.\PubPlaceMarcInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PurchaseInfoErrorAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\RecycleDataDetailInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\RecycleDataRestoreDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RecycleDataSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RequestErrorAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\SetUpRegNoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpeciesErrorDataSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpeciesInfoErrorAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToolBarItem.cpp
# End Source File
# Begin Source File

SOURCE=.\VolIndexErrorAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\WasteDataErrorAdjust1.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccessionNoSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\CopyCodeMarcInfo.h
# End Source File
# Begin Source File

SOURCE=.\CurrencyCodeMarcInfo.h
# End Source File
# Begin Source File

SOURCE=.\DataProcessingDlg.h
# End Source File
# Begin Source File

SOURCE=.\DropRegNoApplyDlg.h
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustApi.h
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustDlg.h
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustDlgMgr.h
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustDoc.h
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustFormView.h
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustFrame.h
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustLauncher.h
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustMgr.h
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustResourceMgr.h
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustSearchMgr.h
# End Source File
# Begin Source File

SOURCE=.\ErrorAdjustThread.h
# End Source File
# Begin Source File

SOURCE=.\IndexErrorAdjust.h
# End Source File
# Begin Source File

SOURCE=.\IndexRestructSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManageNoErrorAdjust.h
# End Source File
# Begin Source File

SOURCE=.\ManageNoErrorSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjustCondition.h
# End Source File
# Begin Source File

SOURCE=.\MarcAdjustDlg.h
# End Source File
# Begin Source File

SOURCE=.\MarcDataErrorAdjust.h
# End Source File
# Begin Source File

SOURCE=.\MarcInfo.h
# End Source File
# Begin Source File

SOURCE=.\MenuItem.h
# End Source File
# Begin Source File

SOURCE=.\PubPlaceMarcInfo.h
# End Source File
# Begin Source File

SOURCE=.\PurchaseInfoErrorAdjust.h
# End Source File
# Begin Source File

SOURCE=.\RecycleDataDetailInfo.h
# End Source File
# Begin Source File

SOURCE=.\RecycleDataRestoreDlg.h
# End Source File
# Begin Source File

SOURCE=.\RecycleDataSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\RequestErrorAdjust.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetUpRegNoDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpeciesErrorDataSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpeciesInfoErrorAdjust.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarItem.h
# End Source File
# Begin Source File

SOURCE=.\VolIndexErrorAdjust.h
# End Source File
# Begin Source File

SOURCE=.\WasteDataErrorAdjust1.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ERROR_ADJUST_MGR.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
