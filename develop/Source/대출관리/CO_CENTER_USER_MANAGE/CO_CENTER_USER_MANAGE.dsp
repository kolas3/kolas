# Microsoft Developer Studio Project File - Name="CO_CENTER_USER_MANAGE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CO_CENTER_USER_MANAGE - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CO_CENTER_USER_MANAGE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CO_CENTER_USER_MANAGE.mak" CFG="CO_CENTER_USER_MANAGE - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CO_CENTER_USER_MANAGE - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "CO_CENTER_USER_MANAGE___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "CO_CENTER_USER_MANAGE___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "CO_CENTER_USER_MANAGE___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ESL.LIB BOBookDetailViewer.lib SearchZipCodeAPI.lib LOC_CommonAPI.lib CLocMailMgr.lib MailMgr.lib SIReportMgr.lib SIReportData.lib Kolas2up_MobileCommon.lib CO_USER_CARD.lib PaintLib.lib libjpeg.lib Check_Dup_Api.lib MNG_LIB_CENTER_MANAGER.lib Kolas2up_CommonControl.lib LOAN_INFO_SHARE_MANAGER.lib IndexToolKit.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\common\CO_CENTER_USER_MANAGE.dll" /pdbtype:sept
# ADD LINK32 ESL.LIB BOBookDetailViewer.lib SearchZipCodeAPI.lib LOC_CommonAPI.lib CLocMailMgr.lib MailMgr.lib SIReportMgr.lib SIReportData.lib Kolas2up_MobileCommon.lib CO_USER_CARD.lib PaintLib.lib libjpeg.lib Check_Dup_Api.lib MNG_LIB_CENTER_MANAGER.lib Kolas2up_CommonControl.lib LOAN_INFO_SHARE_MANAGER.lib IndexToolKit.lib CharEncodeMgr.lib BL_LOC_USER.lib KL_API.lib BO_LOC_3000.lib CSMSContentsEditor.lib CXroshotManager.lib CO_SMS.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\common\CO_CENTER_USER_MANAGE.dll" /pdbtype:sept
# Begin Target

# Name "CO_CENTER_USER_MANAGE - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CO_CENTER_USER_CHANGE_HISTORY.cpp
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_DetailViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_DETAILVIEWER2.cpp
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_DUP_ID_LIST.cpp
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_MANAGE.cpp
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_MANAGE.def
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_MANAGE.rc
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_MANAGE_03.cpp
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_MANAGE_04.cpp
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_MANAGE_05.cpp
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_MANAGER_01.cpp
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_MANAGER_02.cpp
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_UPLOAD.cpp
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_USE.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CO_CENTER_USER_CHANGE_HISTORY.h
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_DetailViewer.h
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_DETAILVIEWER2.h
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_DUP_ID_LIST.h
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_MANAGE_03.h
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_MANAGE_04.h
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_MANAGE_05.h
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_MANAGER_01.h
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_MANAGER_02.h
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_UPLOAD.h
# End Source File
# Begin Source File

SOURCE=.\CO_CENTER_USER_USE.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CO_CENTER_USER_MANAGE.rc2
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
