# Microsoft Developer Studio Project File - Name="K2UP_Common" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=K2UP_Common - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "K2UP_Common.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "K2UP_Common.mak" CFG="K2UP_Common - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "K2UP_Common - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "K2UP_Common___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "K2UP_Common___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib"
# PROP Intermediate_Dir "K2UP_Common___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_MBCS" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D _WIN32_WINNT>=0x0501 /D _WIN32_IE>=0x0600 /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_AFXEXT" /D "_DEBUG" /D _WIN32_WINNT>=0x0501 /D _WIN32_IE>=0x0600 /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib IndexToolKit.lib SIReportMgr.lib SIReportData.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\COMMON\K2UP_Common.dll" /pdbtype:sept
# ADD LINK32 esl.lib IndexToolKit.lib SIReportMgr.lib SIReportData.lib CharEncodeMgr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\COMMON\K2UP_Common.dll" /pdbtype:sept
# Begin Target

# Name "K2UP_Common - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BasicPrintMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\DataProcessingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataProcessingThread.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcelExporter.cpp
# End Source File
# Begin Source File

SOURCE=.\ISBNChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\K2UP_Common.cpp
# End Source File
# Begin Source File

SOURCE=.\K2UP_Common.def
# End Source File
# Begin Source File

SOURCE=.\K2UP_Common.rc
# End Source File
# Begin Source File

SOURCE=.\Keyword.cpp
# End Source File
# Begin Source File

SOURCE=.\LastNumberManager.cpp
# End Source File
# Begin Source File

SOURCE=.\LibInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\LibInfoMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MakeSearchData.cpp
# End Source File
# Begin Source File

SOURCE=.\MulitComboBoxManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintDataInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ReplaceCodeToDesc.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Trim.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkingStatus.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BasicPrintMgr.h
# End Source File
# Begin Source File

SOURCE=.\DataProcessingDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataProcessingThread.h
# End Source File
# Begin Source File

SOURCE=.\ExcelExporter.h
# End Source File
# Begin Source File

SOURCE=.\ISBNChecker.h
# End Source File
# Begin Source File

SOURCE=.\Keyword.h
# End Source File
# Begin Source File

SOURCE=.\LastNumberManager.h
# End Source File
# Begin Source File

SOURCE=.\LibInfo.h
# End Source File
# Begin Source File

SOURCE=.\LibInfoMgr.h
# End Source File
# Begin Source File

SOURCE=.\MakeSearchData.h
# End Source File
# Begin Source File

SOURCE=.\MulitComboBoxManager.h
# End Source File
# Begin Source File

SOURCE=.\PrintDataInfo.h
# End Source File
# Begin Source File

SOURCE=.\ReplaceCodeToDesc.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SpecialCharacter.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Trim.h
# End Source File
# Begin Source File

SOURCE=.\WorkingStatus.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\K2UP_Common.rc2
# End Source File
# Begin Source File

SOURCE=.\WaitProgress.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
