# Microsoft Developer Studio Project File - Name="BO_STATICS_1100" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BO_STATICS_1100 - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BO_STATICS_1100.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_STATICS_1100.mak" CFG="BO_STATICS_1100 - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_STATICS_1100 - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "BO_STATICS_1100___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BO_STATICS_1100___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "BO_STATICS_1100___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ESL.LIB LOC_CommonAPI.lib CO_STATICS.lib Kolas2up_MobileCommon.lib SIReportMgr.lib SIReportData.lib BO_STATICS_5163.lib BO_STATICS_ND.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mono\BO_STATICS_1100.dll" /pdbtype:sept
# ADD LINK32 ESL.LIB LOC_CommonAPI.lib CO_STATICS.lib Kolas2up_MobileCommon.lib SIReportMgr.lib SIReportData.lib BO_STATICS_5163.lib BO_STATICS_ND.lib KL_API.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mono\BO_STATICS_1100.dll" /pdbtype:sept
# Begin Target

# Name "BO_STATICS_1100 - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BO_LOAN_CNT_STATICS.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOAN_JOURNAL_STATICS.CPP
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_1100.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_1100.def
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_1100.rc
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_11001.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_1110.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_1111.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_1112.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_1113.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_1120.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_LIST.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TmpDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\XLAutomationForJournal.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BO_LOAN_CNT_STATICS.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOAN_JOURNAL_STATICS.H
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_11001.h
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_1110.h
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_1111.h
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_1112.h
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_1113.h
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_1120.h
# End Source File
# Begin Source File

SOURCE=.\BO_STATICS_LIST.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TmpDialog.h
# End Source File
# Begin Source File

SOURCE=.\XLAutomationForJournal.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\286_dimd.bmp
# End Source File
# Begin Source File

SOURCE=.\286_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BO_STATICS_1100.rc2
# End Source File
# Begin Source File

SOURCE=.\Kolas2_32.ico
# End Source File
# Begin Source File

SOURCE=".\Resize of 286_on.bmp"
# End Source File
# Begin Source File

SOURCE=.\run_d.bmp
# End Source File
# Begin Source File

SOURCE=.\run_s.bmp
# End Source File
# Begin Source File

SOURCE=.\run_sd1.bmp
# End Source File
# Begin Source File

SOURCE=.\run_sd2.bmp
# End Source File
# Begin Source File

SOURCE=.\run_sd3.bmp
# End Source File
# Begin Source File

SOURCE=.\run_su1.bmp
# End Source File
# Begin Source File

SOURCE=.\run_su2.bmp
# End Source File
# Begin Source File

SOURCE=.\run_su3.bmp
# End Source File
# Begin Source File

SOURCE=.\run_u.bmp
# End Source File
# Begin Source File

SOURCE=.\treebitm.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
