# Microsoft Developer Studio Project File - Name="BO_LOC_1300" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BO_LOC_1300 - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BO_LOC_1300.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_LOC_1300.mak" CFG="BO_LOC_1300 - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_LOC_1300 - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "BO_LOC_1300___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BO_LOC_1300___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\lib"
# PROP Intermediate_Dir "BO_LOC_1300___Win32_Unicode_Debug"
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
# ADD BASE BSC32 /nologo /o"BO_LOC_1300.bsc"
# ADD BSC32 /nologo /o"BO_LOC_1300.bsc"
LINK32=link.exe
# ADD BASE LINK32 ESL.LIB BL_LOC.lib BOBookDetailViewer.lib LOC_CommonAPI.lib SIReportMgr.lib SIReportData.lib Kolas2up_MobileCommon.lib Kolas2up_CommonControl.lib K2UP_Common.lib IndexToolKit.lib Imm32.lib bo_species_api.lib MarcAdjustApi.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\Mono\BO_LOC_1300.dll" /pdbtype:sept
# ADD LINK32 ESL.LIB BL_LOC.lib BOBookDetailViewer.lib LOC_CommonAPI.lib SIReportMgr.lib SIReportData.lib Kolas2up_MobileCommon.lib Kolas2up_CommonControl.lib K2UP_Common.lib IndexToolKit.lib Imm32.lib bo_species_api.lib MarcAdjustApi.lib CharEncodeMgr.lib KL_Api.lib KolisNet_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\Mono\BO_LOC_1300.dll" /pdbtype:sept
# Begin Target

# Name "BO_LOC_1300 - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BO_LOC_1300.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_1300.def
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_1300.rc
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_13001.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_1310.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_1320.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_1330.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_1340.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_1350.cpp
# End Source File
# Begin Source File

SOURCE=.\ShelfChangeReasonList.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BO_LOC_13001.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_1310.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_1320.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_1330.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_1340.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_1350.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ShelfChangeReasonList.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BO_LOC_1300.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
