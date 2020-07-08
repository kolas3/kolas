# Microsoft Developer Studio Project File - Name="KOLAS2UP_SETUP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=KOLAS2UP_SETUP - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KOLAS2UP_SETUP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KOLAS2UP_SETUP.mak" CFG="KOLAS2UP_SETUP - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KOLAS2UP_SETUP - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "KOLAS2UP_SETUP___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "KOLAS2UP_SETUP___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "KOLAS2UP_SETUP___Win32_Unicode_Debug"
# PROP Intermediate_Dir "KOLAS2UP_SETUP___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /i "_LIGHT" /d "_DEBUG"
# ADD RSC /l 0x412 /i "_LIGHT" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ocimgr.lib ESL.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\BIN\KOLAS2UP_SETUP.exe" /pdbtype:sept
# ADD LINK32 ocimgr.lib ESL.lib CharEncodeMgr.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\BIN\KOLASIII_SETUP.exe" /pdbtype:sept
# Begin Target

# Name "KOLAS2UP_SETUP - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ColorPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectSetupPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DaemonSetupPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EtcSetupPage.cpp
# End Source File
# Begin Source File

SOURCE=.\GridSetupPage.cpp
# End Source File
# Begin Source File

SOURCE=.\KOLAS2UP_SETUP.cpp
# End Source File
# Begin Source File

SOURCE=.\KOLAS2UP_SETUP.rc
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RegistryEx.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupMainSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TnsSetupPage.cpp
# End Source File
# Begin Source File

SOURCE=.\UserInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\VerInfo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ColorPicker.h
# End Source File
# Begin Source File

SOURCE=.\ColorPopup.h
# End Source File
# Begin Source File

SOURCE=.\ConnectSetupPage.h
# End Source File
# Begin Source File

SOURCE=.\DaemonSetupPage.h
# End Source File
# Begin Source File

SOURCE=.\EtcSetupPage.h
# End Source File
# Begin Source File

SOURCE=.\GridSetupPage.h
# End Source File
# Begin Source File

SOURCE=.\KOLAS2UP_SETUP.h
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.h
# End Source File
# Begin Source File

SOURCE=.\RegistryEx.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetupMainSheet.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TnsSetupPage.h
# End Source File
# Begin Source File

SOURCE=.\UserInfo.h
# End Source File
# Begin Source File

SOURCE=.\VerInfo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ESLUserConfig.ico
# End Source File
# Begin Source File

SOURCE=.\res\KOLAS2UP_SETUP.ico
# End Source File
# Begin Source File

SOURCE=.\res\KOLAS2UP_SETUP.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
