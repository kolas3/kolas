# Microsoft Developer Studio Project File - Name="MNG_USER_MANAGER" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MNG_USER_MANAGER - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MNG_USER_MANAGER.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MNG_USER_MANAGER.mak" CFG="MNG_USER_MANAGER - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MNG_USER_MANAGER - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "MNG_USER_MANAGER___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "MNG_USER_MANAGER___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MNG_USER_MANAGER___Win32_Unicode_Debug"
# PROP Intermediate_Dir "MNG_USER_MANAGER___Win32_Unicode_Debug"
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
# ADD BASE LINK32 esl.lib SearchZipCodeAPI.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib MNG_ACCESS_CONTROL.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mng\MNG_USER_MANAGER.dll" /pdbtype:sept
# ADD LINK32 esl.lib SearchZipCodeAPI.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib MNG_ACCESS_CONTROL.lib KL_Api.lib GPKI.lib /nologo /subsystem:windows /dll /pdb:"..\..\..\..\lib/MNG_USER_MANAGER.pdb" /debug /machine:I386 /out:"..\..\..\..\..\dll\mng\MNG_USER_MANAGER.dll" /implib:"..\..\..\..\lib/MNG_USER_MANAGER.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "MNG_USER_MANAGER - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MNG_PRIVACY_LOG_SEARCH_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_PRIVACY_PRINT_LOG_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_CHANGEPASSWORD.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER.def
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER.rc
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_01.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_02.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_03.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_03_GPKI_REG.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_04.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_05.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_06.cpp
# End Source File
# Begin Source File

SOURCE=.\PermisionCFGDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PMS_UPDATE.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UserCfgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserRegIPDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserRegTimeoutDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MNG_PRIVACY_LOG_SEARCH_DLG.h
# End Source File
# Begin Source File

SOURCE=.\MNG_PRIVACY_PRINT_LOG_DLG.h
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_CHANGEPASSWORD.h
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_01.h
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_02.h
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_03.h
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_03_GPKI_REG.h
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_04.h
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_05.h
# End Source File
# Begin Source File

SOURCE=.\MNG_USER_MANAGER_06.h
# End Source File
# Begin Source File

SOURCE=.\PermisionCFGDlg.h
# End Source File
# Begin Source File

SOURCE=.\PMS_UPDATE.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UserCfgDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserRegIPDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserRegTimeoutDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\MNG_USER_MANAGER.rc2
# End Source File
# Begin Source File

SOURCE=.\sheet.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
