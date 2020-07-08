# Microsoft Developer Studio Project File - Name="UC_NOTIFY_POPUP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=UC_NOTIFY_POPUP - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UC_NOTIFY_POPUP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UC_NOTIFY_POPUP.mak" CFG="UC_NOTIFY_POPUP - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UC_NOTIFY_POPUP - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "UC_NOTIFY_POPUP___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "UC_NOTIFY_POPUP___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib/"
# PROP Intermediate_Dir "UC_NOTIFY_POPUP___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib rs_common.lib rsmgr.lib UC_KOLISNetUseCheck.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\UC\UC_NOTIFY_POPUP.dll" /pdbtype:sept
# ADD LINK32 esl.lib UC_KOLISNetUseCheck.lib KolisNet_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\UC\UC_NOTIFY_POPUP.dll" /pdbtype:sept
# Begin Target

# Name "UC_NOTIFY_POPUP - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UC_NOTIFY_POPUP.cpp
# End Source File
# Begin Source File

SOURCE=.\UC_NOTIFY_POPUP.def
# End Source File
# Begin Source File

SOURCE=.\UC_NOTIFY_POPUP.rc
# End Source File
# Begin Source File

SOURCE=.\UC_NotifyPopup_FileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UC_NotifyPopupDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UC_NotifyPopup_FileDlg.h
# End Source File
# Begin Source File

SOURCE=.\UC_NotifyPopupDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\NL_LOGO.bmp
# End Source File
# Begin Source File

SOURCE=.\res\UC_NOTIFY_POPUP.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section UC_NOTIFY_POPUP : {9ABC7541-81F1-4E57-B351-AEBCD73E3AE5}
# 	1:16:IDD_UC_NOTIFY_03:20049
# 	2:16:Resource Include:resource.h
# 	2:22:Light_UC_NOTIFY_03.cpp:Light_UC_NOTIFY_03.cpp
# 	2:10:ENUM: enum:enum
# 	2:16:IDD_UC_NOTIFY_03:IDD_UC_NOTIFY_03
# 	2:26:CLASS: CLight_UC_NOTIFY_03:CLight_UC_NOTIFY_03
# 	2:19:Application Include:UC_NOTIFY_POPUP.h
# 	2:20:Light_UC_NOTIFY_03.h:Light_UC_NOTIFY_03.h
# End Section
