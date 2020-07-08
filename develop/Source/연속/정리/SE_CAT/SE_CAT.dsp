# Microsoft Developer Studio Project File - Name="SE_CAT" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SE_CAT - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SE_CAT.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SE_CAT.mak" CFG="SE_CAT - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SE_CAT - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "SE_CAT___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "SE_CAT___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "SE_CAT___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib MarcAdjust.lib MarcAdjustApi.lib MarcEdit.lib IndexToolKit.lib K2UP_Common.lib SIReportData.lib SIReportMgr.lib Duplicate_Data_Search.lib BO_CAT_DUP_FREE.LIB BO_CAT_PROC.lib SE_CAT_MARC_IMPORT.lib SE_CAT_MARC_EXPORT.lib MarcToolkit.lib BO_ACQ_GROUP_MANAGER.lib Light_BO_CAT_CENTERUPLOAD.lib CAT_PRINT.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib BO_SPECIES_API.LIB SE_API.lib COVERMANAGER.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\serial\SE_CAT.dll" /pdbtype:sept
# ADD LINK32 esl.lib MarcAdjust.lib MarcAdjustApi.lib MarcEdit.lib IndexToolKit.lib K2UP_Common.lib SIReportData.lib SIReportMgr.lib Duplicate_Data_Search.lib BO_CAT_DUP_FREE.LIB BO_CAT_PROC.lib SE_CAT_MARC_IMPORT.lib SE_CAT_MARC_EXPORT.lib MarcToolkit.lib BO_ACQ_GROUP_MANAGER.lib Light_BO_CAT_CENTERUPLOAD.lib CAT_PRINT.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib BO_SPECIES_API.LIB SE_API.lib COVERMANAGER.lib CharEncodeMgr.lib REG_NO_CHANGER.lib CAT_DCM_MANAGER.lib KL_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\serial\SE_CAT.dll" /pdbtype:sept
# Begin Target

# Name "SE_CAT - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MainDefaultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT.def
# End Source File
# Begin Source File

SOURCE=.\SE_CAT.rc
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_SearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MainDefaultDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_SearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\SE_CAT.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
