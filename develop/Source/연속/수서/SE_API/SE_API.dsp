# Microsoft Developer Studio Project File - Name="SE_API" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SE_API - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SE_API.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SE_API.mak" CFG="SE_API - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SE_API - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "SE_API___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "SE_API___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "SE_API___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib IndexToolKit.lib MarcEdit.lib MarcToolKit.lib BL_SE_LOC_1500.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib Kolas2up_CommonControl.lib BO_ACQ_GROUP_MANAGER.lib SIReportData.lib SIReportMgr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\serial\SE_API.dll" /pdbtype:sept
# ADD LINK32 esl.lib IndexToolKit.lib MarcEdit.lib MarcToolKit.lib BL_SE_LOC_1500.lib Kolas2up_CommonControl.lib BO_ACQ_GROUP_MANAGER.lib SIReportData.lib SIReportMgr.lib CharEncodeMgr.lib COMMON_CONVERSION_MGR.lib COMMON_BUDGET_MGR.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib KL_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\serial\SE_API.dll" /pdbtype:sept
# Begin Target

# Name "SE_API - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CheckinNewReceipt.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckinReceipt.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckinReceiptBookInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_API.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_API.def
# End Source File
# Begin Source File

SOURCE=.\SE_API.rc
# End Source File
# Begin Source File

SOURCE=.\SeApi.cpp
# End Source File
# Begin Source File

SOURCE=.\SeAppedixExpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SeAppendixExpApi.cpp
# End Source File
# Begin Source File

SOURCE=.\SeDateExpApi.cpp
# End Source File
# Begin Source File

SOURCE=.\SeDuplicatedTitleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SeDuplicatedVolDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SeExpectMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SeExpectVerifier.cpp
# End Source File
# Begin Source File

SOURCE=.\SeFileImport.cpp
# End Source File
# Begin Source File

SOURCE=.\SeFileImportMarcEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SeMarcImport.cpp
# End Source File
# Begin Source File

SOURCE=.\SeMarcImportMarcView.cpp
# End Source File
# Begin Source File

SOURCE=.\SeMemoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SePurchaseNoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SePurchaseNoInsertDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SeSpeciesDelete.cpp
# End Source File
# Begin Source File

SOURCE=.\SeVolExpApi.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CheckinNewReceipt.h
# End Source File
# Begin Source File

SOURCE=.\CheckinReceipt.h
# End Source File
# Begin Source File

SOURCE=.\CheckinReceiptBookInfo.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SeApi.h
# End Source File
# Begin Source File

SOURCE=.\SeAppedixExpDlg.h
# End Source File
# Begin Source File

SOURCE=.\SeAppendixExpApi.h
# End Source File
# Begin Source File

SOURCE=.\SeDateExpApi.h
# End Source File
# Begin Source File

SOURCE=.\SeDuplicatedTitleDlg.h
# End Source File
# Begin Source File

SOURCE=.\SeDuplicatedVolDlg.h
# End Source File
# Begin Source File

SOURCE=.\SeExpectMgr.h
# End Source File
# Begin Source File

SOURCE=.\SeExpectVerifier.h
# End Source File
# Begin Source File

SOURCE=.\SeFileImport.h
# End Source File
# Begin Source File

SOURCE=.\SeFileImportMarcEdit.h
# End Source File
# Begin Source File

SOURCE=.\SeMarcImport.h
# End Source File
# Begin Source File

SOURCE=.\SeMarcImportMarcView.h
# End Source File
# Begin Source File

SOURCE=.\SeMemoDlg.h
# End Source File
# Begin Source File

SOURCE=.\SePurchaseNoDlg.h
# End Source File
# Begin Source File

SOURCE=.\SePurchaseNoInsertDlg.h
# End Source File
# Begin Source File

SOURCE=.\SeSpeciesDelete.h
# End Source File
# Begin Source File

SOURCE=.\SeVolExpApi.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\SE_API.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
