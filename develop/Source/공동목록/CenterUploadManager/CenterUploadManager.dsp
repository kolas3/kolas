# Microsoft Developer Studio Project File - Name="CenterUploadManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CenterUploadManager - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CenterUploadManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CenterUploadManager.mak" CFG="CenterUploadManager - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CenterUploadManager - Win32 Unicode Debug" (based on "Win32 (x86) Application")
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
# PROP BASE Output_Dir "CenterUploadManager___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "CenterUploadManager___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CenterUploadManager___Win32_Unicode_Debug"
# PROP Intermediate_Dir "CenterUploadManager___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ESL.lib Check_Dup_Api.lib rsmgr.lib searchmgr.lib BO_CAT_PROC.lib UC_IT_01_UPDATE.lib MarcToolKit.lib indextoolkit.lib MarcAdjust.lib MarcAdjustApi.lib CenterUpload_Api.lib UC_KOLISNetUseCheck.lib IndexToolKit.lib /nologo /subsystem:windows /debug /machine:I386 /out:"F:\work\Kolas2up\bin\CenterUploadManager.exe" /pdbtype:sept
# ADD LINK32 ESL.lib Check_Dup_Api.lib BO_CAT_PROC.lib MarcToolKit.lib indextoolkit.lib MarcAdjust.lib MarcAdjustApi.lib CenterUpload_Api.lib UC_KOLISNetUseCheck.lib IndexToolKit.lib NetApi32.lib Iphlpapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\BIN\CenterUploadManager.exe" /pdbtype:sept
# Begin Target

# Name "CenterUploadManager - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CenterUploadManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CenterUploadManager.rc
# End Source File
# Begin Source File

SOURCE=.\CenterUploadManagerDlg.cpp
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

SOURCE=.\CenterUploadManager.h
# End Source File
# Begin Source File

SOURCE=.\CenterUploadManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
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

SOURCE=.\res\287_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\287_on.bmp.ico
# End Source File
# Begin Source File

SOURCE=.\res\287_on.ico
# End Source File
# Begin Source File

SOURCE=.\res\CenterUploadManager.ico
# End Source File
# Begin Source File

SOURCE=.\res\CenterUploadManager.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
