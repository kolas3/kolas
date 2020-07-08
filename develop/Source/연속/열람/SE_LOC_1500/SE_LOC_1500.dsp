# Microsoft Developer Studio Project File - Name="SE_LOC_1500" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SE_LOC_1500 - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SE_LOC_1500.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SE_LOC_1500.mak" CFG="SE_LOC_1500 - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SE_LOC_1500 - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "SE_LOC_1500___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "SE_LOC_1500___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "SE_LOC_1500___Win32_Unicode_Debug"
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
# ADD BASE BSC32 /nologo /o"SE_LOC_1500.bsc"
# ADD BSC32 /nologo /o"SE_LOC_1500.bsc"
LINK32=link.exe
# ADD BASE LINK32 ESL.LIB BL_SE_LOC_1500.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib Kolas2up_CommonControl.lib CAT_PRINT.lib SIReportMgr.lib SIReportData.lib SEBookDetailViewer.lib CReceiptNumViwer.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\Serial\SE_LOC_1500.dll" /pdbtype:sept
# ADD LINK32 ESL.LIB BL_SE_LOC_1500.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib Kolas2up_CommonControl.lib CAT_PRINT.lib SIReportMgr.lib SIReportData.lib SEBookDetailViewer.lib CReceiptNumViwer.lib CharEncodeMgr.lib KL_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\Serial\SE_LOC_1500.dll" /pdbtype:sept
# Begin Target

# Name "SE_LOC_1500 - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\EquipPrintDlg_SE.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1500.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1500.def
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1500.rc
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_15001.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1510.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1520.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1530.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1540.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1550.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1550_5163.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1560.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1580.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1590.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1595.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1597.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1598.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1599_EX.cpp
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

SOURCE=.\EquipPrintDlg_SE.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_15001.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1510.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1520.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1530.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1540.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1550.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1550_5163.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1560.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1580.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1590.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1595.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1597.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1598.h
# End Source File
# Begin Source File

SOURCE=.\SE_LOC_1599_EX.h
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

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SE_LOC_1500.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
