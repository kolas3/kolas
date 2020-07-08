# Microsoft Developer Studio Project File - Name="LOAN_INFO_SHARE_MANAGER" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=LOAN_INFO_SHARE_MANAGER - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LOAN_INFO_SHARE_MANAGER.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LOAN_INFO_SHARE_MANAGER.mak" CFG="LOAN_INFO_SHARE_MANAGER - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LOAN_INFO_SHARE_MANAGER - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "LOAN_INFO_SHARE_MANAGER___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "LOAN_INFO_SHARE_MANAGER___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "LOAN_INFO_SHARE_MANAGER___Win32_Unicode_Debug"
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
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib Check_Dup_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\common\LOAN_INFO_SHARE_MANAGER.dll" /pdbtype:sept
# ADD LINK32 esl.lib Check_Dup_Api.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib KL_Api.lib BO_LOC_3000.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\common\LOAN_INFO_SHARE_MANAGER.dll" /pdbtype:sept
# Begin Target

# Name "LOAN_INFO_SHARE_MANAGER - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CommonDelayEnvCenterUp.cpp
# End Source File
# Begin Source File

SOURCE=.\HolydayInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\LOAN_INFO_SHARE_MANAGER.cpp
# End Source File
# Begin Source File

SOURCE=.\LOAN_INFO_SHARE_MANAGER.def
# End Source File
# Begin Source File

SOURCE=.\LOAN_INFO_SHARE_MANAGER.rc
# End Source File
# Begin Source File

SOURCE=.\LoanInfoCenterUp.cpp
# End Source File
# Begin Source File

SOURCE=.\LoanShareManager.cpp
# End Source File
# Begin Source File

SOURCE=.\LoanShareWaitMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\LoanUserCenterUp.cpp
# End Source File
# Begin Source File

SOURCE=.\ShareInfoCenterUp.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TableFieldMapping.cpp
# End Source File
# Begin Source File

SOURCE=.\TestShare.cpp
# End Source File
# Begin Source File

SOURCE=.\UCaRDelayCenterUp.cpp
# End Source File
# Begin Source File

SOURCE=.\UClassDelayInfoCenterUp.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CommonDelayEnvCenterUp.h
# End Source File
# Begin Source File

SOURCE=.\HolydayInfo.h
# End Source File
# Begin Source File

SOURCE=.\LoanInfoCenterUp.h
# End Source File
# Begin Source File

SOURCE=.\LoanShareManager.h
# End Source File
# Begin Source File

SOURCE=.\LoanShareWaitMessage.h
# End Source File
# Begin Source File

SOURCE=.\LoanUserCenterUp.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ShareInfoCenterUp.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TableFieldMapping.h
# End Source File
# Begin Source File

SOURCE=.\TestShare.h
# End Source File
# Begin Source File

SOURCE=.\UCaRDelayCenterUp.h
# End Source File
# Begin Source File

SOURCE=.\UClassDelayInfoCenterUp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\LOAN_INFO_SHARE_MANAGER.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
