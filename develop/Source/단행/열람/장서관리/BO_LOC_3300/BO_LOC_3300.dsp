# Microsoft Developer Studio Project File - Name="BO_LOC_3300" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BO_LOC_3300 - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BO_LOC_3300.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_LOC_3300.mak" CFG="BO_LOC_3300 - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_LOC_3300 - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "BO_LOC_3300___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BO_LOC_3300___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\lib"
# PROP Intermediate_Dir "BO_LOC_3300___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"BO_LOC_3300.bsc"
# ADD BSC32 /nologo /o"BO_LOC_3300.bsc"
LINK32=link.exe
# ADD BASE LINK32 ESL.LIB BL_LOC_USER.lib BL_LOC_LOAN.lib BOBookDetailViewer.lib SearchZipCodeAPI.lib LOC_CommonAPI.lib CLocMailMgr.lib MailMgr.lib SIReportMgr.lib SIReportData.lib Kolas2up_MobileCommon.lib CO_CENTER_USER_MANAGE.lib CO_USER_CARD.lib PaintLib.lib libjpeg.lib Check_Dup_Api.lib Kolas2up_CommonControl.lib Check_Dup_Api.lib LOAN_INFO_SHARE_MANAGER.lib LoanCardFormatManager.lib LOC_CommonAPI.lib CAT_PRINT.lib Imm32.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"..\..\..\..\..\..\dll\Mono\BO_LOC_3300.dll" /pdbtype:sept
# ADD LINK32 ESL.LIB BL_LOC_USER.lib BL_LOC_LOAN.lib BOBookDetailViewer.lib SearchZipCodeAPI.lib LOC_CommonAPI.lib CLocMailMgr.lib MailMgr.lib SIReportMgr.lib SIReportData.lib Kolas2up_MobileCommon.lib CO_CENTER_USER_MANAGE.lib CO_USER_CARD.lib PaintLib.lib libjpeg.lib Check_Dup_Api.lib Kolas2up_CommonControl.lib Check_Dup_Api.lib LOAN_INFO_SHARE_MANAGER.lib LoanCardFormatManager.lib LOC_CommonAPI.lib CAT_PRINT.lib Imm32.lib CharEncodeMgr.lib kl_api.lib BO_LOC_ARREAR.lib BO_LOC_3000.lib ExcelMgr.lib CSMSContentsEditor.lib CXroshotManager.lib CO_SMS.lib RFID_JOB.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"..\..\..\..\..\..\dll\Mono\BO_LOC_3300.dll" /pdbtype:sept
# Begin Target

# Name "BO_LOC_3300 - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BO_LOC_3300.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3300.def
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3300.rc
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_33001.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3310.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3370.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3380.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3390.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3391.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3395.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3396.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3397.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3398.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3399.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_SECEDE.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcelHeaderManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcelHeaderSetter.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpireUserSms.cpp
# End Source File
# Begin Source File

SOURCE=.\GridEdt.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertPassword.cpp
# End Source File
# Begin Source File

SOURCE=.\LoanUserManage.cpp
# End Source File
# Begin Source File

SOURCE=.\PersonInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdioFileEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StringTokenizer.cpp
# End Source File
# Begin Source File

SOURCE=.\UserDelSearchDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BO_LOC_33001.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3310.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3370.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3380.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3390.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3391.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3395.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3396.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3397.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3398.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3399.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_SECEDE.h
# End Source File
# Begin Source File

SOURCE=.\ExcelHeaderManager.h
# End Source File
# Begin Source File

SOURCE=.\ExcelHeaderSetter.h
# End Source File
# Begin Source File

SOURCE=.\ExpireUserSms.h
# End Source File
# Begin Source File

SOURCE=.\GridEdt.h
# End Source File
# Begin Source File

SOURCE=.\InsertPassword.h
# End Source File
# Begin Source File

SOURCE=.\LoanUserManage.h
# End Source File
# Begin Source File

SOURCE=.\PersonInfo.h
# End Source File
# Begin Source File

SOURCE=.\ResizableDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StdioFileEx.h
# End Source File
# Begin Source File

SOURCE=.\StringTokenizer.h
# End Source File
# Begin Source File

SOURCE=.\UserDelSearchDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BO_LOC_3300.rc2
# End Source File
# Begin Source File

SOURCE=.\Kolas2_32.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
