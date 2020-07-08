# Microsoft Developer Studio Project File - Name="BO_ACQ_GROUP_MANAGER" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BO_ACQ_GROUP_MANAGER - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BO_ACQ_GROUP_MANAGER.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_ACQ_GROUP_MANAGER.mak" CFG="BO_ACQ_GROUP_MANAGER - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_ACQ_GROUP_MANAGER - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "BO_ACQ_GROUP_MANAGER___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BO_ACQ_GROUP_MANAGER___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib/"
# PROP Intermediate_Dir "BO_ACQ_GROUP_MANAGER___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_MBCS" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D _WIN32_WINNT>=0x0501 /D _WIN32_IE>=0x0600 /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_AFXEXT" /D "_DEBUG" /D _WIN32_WINNT>=0x0501 /D _WIN32_IE>=0x0600 /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib K2UP_Common.lib IndexToolKit.lib BO_SPECIES_API.lib COMMON_CONVERSION_MGR.lib BO_DATA_MANAGER.lib DATA_SORT_MANAGER.lib COMMON_BUDGET_MGR.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../../dll/common/BO_ACQ_GROUP_MANAGER.dll" /pdbtype:sept
# ADD LINK32 esl.lib K2UP_Common.lib IndexToolKit.lib BO_SPECIES_API.lib COMMON_CONVERSION_MGR.lib BO_DATA_MANAGER.lib DATA_SORT_MANAGER.lib COMMON_BUDGET_MGR.lib CharEncodeMgr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../../dll/common/BO_ACQ_GROUP_MANAGER.dll" /pdbtype:sept
# Begin Target

# Name "BO_ACQ_GROUP_MANAGER - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AcqDonateGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqGroupDlgMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqGroupDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqGroupFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqGroupFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqGroupLauncher.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqGroupMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqGroupResourceMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqGroupSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqGroupThread.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqObjGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqOtherGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqPurchaseGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_ACQ_GROUP_MANAGER.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_ACQ_GROUP_MANAGER.def
# End Source File
# Begin Source File

SOURCE=.\BO_ACQ_GROUP_MANAGER.rc
# End Source File
# Begin Source File

SOURCE=.\BO_BudgetMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckHoldDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_DataView_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_Divide_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_Insert_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_List_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_Mgr.cpp
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_Select_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_Status_Change_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_Update_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DropGroupNoApplyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Group_DataView_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Group_Delete_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Group_Divide_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Group_Insert_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Group_List_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Group_Merge_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Group_Mgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Group_Search.cpp
# End Source File
# Begin Source File

SOURCE=.\Group_Select_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupCollection.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupPair.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuItem.cpp
# End Source File
# Begin Source File

SOURCE=.\MergeGroup_Select_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherGroupNo_DataView_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherGroupNo_Divide_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherGroupNo_Insert_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherGroupNo_List_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherGroupNo_Mgr.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherGroupNo_Select_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RequisitionNo_DataView_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RequisitionNo_Divide_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RequisitionNo_Insert_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RequisitionNo_List_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RequisitionNo_Mgr.cpp
# End Source File
# Begin Source File

SOURCE=.\RequisitionNo_Select_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToolBarItem.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkNoInfo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AcqDonateGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\AcqGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\AcqGroupDlgMgr.h
# End Source File
# Begin Source File

SOURCE=.\AcqGroupDoc.h
# End Source File
# Begin Source File

SOURCE=.\AcqGroupFormView.h
# End Source File
# Begin Source File

SOURCE=.\AcqGroupFrame.h
# End Source File
# Begin Source File

SOURCE=.\AcqGroupLauncher.h
# End Source File
# Begin Source File

SOURCE=.\AcqGroupMgr.h
# End Source File
# Begin Source File

SOURCE=.\AcqGroupResourceMgr.h
# End Source File
# Begin Source File

SOURCE=.\AcqGroupSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\AcqGroupThread.h
# End Source File
# Begin Source File

SOURCE=.\AcqObjGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\AcqOtherGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\AcqPurchaseGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\BO_BudgetMgr.h
# End Source File
# Begin Source File

SOURCE=.\CheckHoldDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_DataView_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_Divide_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_Insert_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_List_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_Mgr.h
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_Select_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_Status_Change_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\DivisionNo_Update_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\DropGroupNoApplyDlg.h
# End Source File
# Begin Source File

SOURCE=.\Group_DataView_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Group_Delete_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Group_Divide_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Group_Insert_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Group_List_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Group_Merge_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Group_Mgr.h
# End Source File
# Begin Source File

SOURCE=.\Group_Search.h
# End Source File
# Begin Source File

SOURCE=.\Group_Select_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\GroupCollection.h
# End Source File
# Begin Source File

SOURCE=.\GroupMgr.h
# End Source File
# Begin Source File

SOURCE=.\GroupPair.h
# End Source File
# Begin Source File

SOURCE=.\MenuItem.h
# End Source File
# Begin Source File

SOURCE=.\MergeGroup_Select_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\OtherGroupNo_DataView_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\OtherGroupNo_Divide_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\OtherGroupNo_Insert_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\OtherGroupNo_List_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\OtherGroupNo_Mgr.h
# End Source File
# Begin Source File

SOURCE=.\OtherGroupNo_Select_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\RequisitionNo_DataView_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\RequisitionNo_Divide_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\RequisitionNo_Insert_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\RequisitionNo_List_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\RequisitionNo_Mgr.h
# End Source File
# Begin Source File

SOURCE=.\RequisitionNo_Select_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarItem.h
# End Source File
# Begin Source File

SOURCE=.\WorkNoInfo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BO_ACQ_GROUP_MANAGER.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
