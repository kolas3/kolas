# Microsoft Developer Studio Project File - Name="BO_ACQ_SPECIES_FORM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BO_ACQ_SPECIES_FORM - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BO_ACQ_SPECIES_FORM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_ACQ_SPECIES_FORM.mak" CFG="BO_ACQ_SPECIES_FORM - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_ACQ_SPECIES_FORM - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "BO_ACQ_SPECIES_FORM___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BO_ACQ_SPECIES_FORM___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib/"
# PROP Intermediate_Dir "BO_ACQ_SPECIES_FORM___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_MBCS" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D _WIN32_WINNT>=0x0501 /D _WIN32_IE>=0x0600 /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_AFXEXT" /D "_DEBUG" /D _WIN32_WINNT>=0x0501 /D _WIN32_IE>=0x0600 /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib ocimgr.lib imm32.lib BO_ACQ_GROUP_MANAGER.lib K2UP_Common.lib BO_SPECIES_API.lib Duplicate_Data_Search.lib SIReportData.lib SIReportMgr.lib marcedit.lib BO_MARC_IMPORT.lib BO_VOLUME_FORM.lib UI_SEARCH_DLG.lib Light_GatherDataSearch.lib IndexToolKit.lib COMMON_DONATOR_MANAGER.lib TocEditor.lib MOKCHA_CHOROK_API.lib COMMON_CONVERSION_MGR.lib BO_DATA_MANAGER.lib DATA_SORT_MANAGER.lib BO_DATA_CHANGE_MANAGER.lib COVERMANAGER.lib BO_ACQ_LOAN_USER_SEARCH_MGR.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../../dll/common/BO_ACQ_SPECIES_FORM.dll" /pdbtype:sept
# ADD LINK32 esl.lib ocimgr.lib imm32.lib BO_ACQ_GROUP_MANAGER.lib K2UP_Common.lib BO_SPECIES_API.lib Duplicate_Data_Search.lib SIReportData.lib SIReportMgr.lib marcedit.lib BO_MARC_IMPORT.lib BO_VOLUME_FORM.lib UI_SEARCH_DLG.lib Light_GatherDataSearch.lib IndexToolKit.lib COMMON_DONATOR_MANAGER.lib TocEditor.lib MOKCHA_CHOROK_API.lib COMMON_CONVERSION_MGR.lib BO_DATA_MANAGER.lib DATA_SORT_MANAGER.lib BO_DATA_CHANGE_MANAGER.lib COVERMANAGER.lib BO_ACQ_LOAN_USER_SEARCH_MGR.lib CharEncodeMgr.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib KL_API.lib KolisNet_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../../dll/common/BO_ACQ_SPECIES_FORM.dll" /pdbtype:sept
# Begin Target

# Name "BO_ACQ_SPECIES_FORM - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AccessionNoSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BasicInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_ACQ_SPECIES_FORM.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_ACQ_SPECIES_FORM.def
# End Source File
# Begin Source File

SOURCE=.\BO_ACQ_SPECIES_FORM.rc
# End Source File
# Begin Source File

SOURCE=.\BOAppendixDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BOMarcEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DivisionNoInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DonateInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HourEditControl.cpp
# End Source File
# Begin Source File

SOURCE=.\HourEditElem.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Light_DivisionNoInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NoteCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PurchaseInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RequisitionNoInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Species_Form.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccessionNoSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\BasicInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\BOAppendixDlg.h
# End Source File
# Begin Source File

SOURCE=.\BOMarcEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\DivisionNoInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\DonateInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\GroupInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\HourEditControl.h
# End Source File
# Begin Source File

SOURCE=.\HourEditElem.h
# End Source File
# Begin Source File

SOURCE=.\InfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\Light_DivisionNoInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\NoteCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PurchaseInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\RequisitionNoInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Species_Form.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BO_ACQ_SPECIES_FORM.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
