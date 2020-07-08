# Microsoft Developer Studio Project File - Name="CXroshotManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CXroshotManager - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CXroshotManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CXroshotManager.mak" CFG="CXroshotManager - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CXroshotManager - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "CXroshotManager___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "CXroshotManager___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "CXroshotManager___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL" /d "_WIN32_DCOM"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL" /d "_WIN32_DCOM"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 x_msggate_d.lib ESL.LIB Kolas2up_MobileCommon.lib LOC_CommonAPI.lib CO_SMS.lib ole32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\mono\CXroshotManager.dll" /pdbtype:sept
# ADD LINK32 x_msggate_d.lib ESL.LIB CO_SMS.lib ole32.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib KL_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\mono\CXroshotManager.dll" /pdbtype:sept
# Begin Target

# Name "CXroshotManager - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ArreoSMSManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CXroshotManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CXroshotManager.def
# End Source File
# Begin Source File

SOURCE=.\CXroshotManager.rc
# End Source File
# Begin Source File

SOURCE=.\EmmaSMSManager.cpp
# End Source File
# Begin Source File

SOURCE=.\GridUpdateEdt.cpp
# End Source File
# Begin Source File

SOURCE=.\ImoSMSManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PajuSMSManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SamsungNetworks.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSMessageConvert.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSPureDLL.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSSendDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\XroshorSMSSender.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ArreoSMSManager.h
# End Source File
# Begin Source File

SOURCE=.\EmmaSMSManager.h
# End Source File
# Begin Source File

SOURCE=.\GridUpdateEdt.h
# End Source File
# Begin Source File

SOURCE=.\ImoSMSManager.h
# End Source File
# Begin Source File

SOURCE=.\PajuSMSManager.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SamsungNetworks.h
# End Source File
# Begin Source File

SOURCE=.\SMS_List.h
# End Source File
# Begin Source File

SOURCE=.\SMSMessageConvert.h
# End Source File
# Begin Source File

SOURCE=.\SMSPureDLL.h
# End Source File
# Begin Source File

SOURCE=.\SMSSendDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\x_msggate.h
# End Source File
# Begin Source File

SOURCE=.\XroshorSMSSender.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CXroshotManager.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
