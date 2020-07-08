# Microsoft Developer Studio Project File - Name="UnionUserManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=UnionUserManager - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UnionUserManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UnionUserManager.mak" CFG="UnionUserManager - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UnionUserManager - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "UnionUserManager___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "UnionUserManager___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\lib"
# PROP Intermediate_Dir "UnionUserManager___Win32_Unicode_Debug"
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
# ADD BASE LINK32 ESL.LIB Kolas2up_CommonControl.lib Check_Dup_Api.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib K2UP_Common.lib IndexToolKit.lib BL_LOC_LOAN.lib LOAN_INFO_SHARE_MANAGER.lib KOLAS1004C.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\mono\UnionUserManager.dll" /pdbtype:sept
# ADD LINK32 ESL.LIB Kolas2up_CommonControl.lib Check_Dup_Api.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib K2UP_Common.lib IndexToolKit.lib BL_LOC_LOAN.lib LOAN_INFO_SHARE_MANAGER.lib kl_api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\mono\UnionUserManager.dll" /pdbtype:sept
# Begin Target

# Name "UnionUserManager - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\LoanLogInfoTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NoticeMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UnionReturnDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UnionUserInfoViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\UnionUserManager.cpp
# End Source File
# Begin Source File

SOURCE=.\UnionUserManager.def
# End Source File
# Begin Source File

SOURCE=.\UnionUserManager.rc
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\051_on.bmp
# End Source File
# Begin Source File

SOURCE=.\052_on.bmp
# End Source File
# Begin Source File

SOURCE=.\076_on.bmp
# End Source File
# Begin Source File

SOURCE=.\354_on.bmp
# End Source File
# Begin Source File

SOURCE=.\D.bmp
# End Source File
# Begin Source File

SOURCE=.\INFO.ICO
# End Source File
# Begin Source File

SOURCE=.\Kolas2_32.ico
# End Source File
# Begin Source File

SOURCE=.\R.bmp
# End Source File
# Begin Source File

SOURCE=.\S.bmp
# End Source File
# Begin Source File

SOURCE=.\S_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\UnionUserManager.rc2
# End Source File
# Begin Source File

SOURCE="..\BL_LOC_LOAN\책.bmp"
# End Source File
# Begin Source File

SOURCE="..\BL_LOC_USER\책.bmp"
# End Source File
# Begin Source File

SOURCE="..\BO_LOC_3100\책.bmp"
# End Source File
# Begin Source File

SOURCE="..\BO_LOC_3200\책.bmp"
# End Source File
# Begin Source File

SOURCE=".\책.bmp"
# End Source File
# Begin Source File

SOURCE="..\..\..\통계\UNION_RETURN_STATICS\책.bmp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\LoanLogInfoTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\NoticeMessage.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UnionReturnDlg.h
# End Source File
# Begin Source File

SOURCE=.\UnionUserInfoViewer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
