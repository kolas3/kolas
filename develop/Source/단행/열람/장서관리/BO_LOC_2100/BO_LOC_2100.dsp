# Microsoft Developer Studio Project File - Name="BO_LOC_2100" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BO_LOC_2100 - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BO_LOC_2100.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_LOC_2100.mak" CFG="BO_LOC_2100 - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_LOC_2100 - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "BO_LOC_2100___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BO_LOC_2100___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\lib"
# PROP Intermediate_Dir "BO_LOC_2100___Win32_Unicode_Debug"
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
# ADD BASE BSC32 /nologo /o"BO_LOC_2100.bsc"
# ADD BSC32 /nologo /o"BO_LOC_2100.bsc"
LINK32=link.exe
# ADD BASE LINK32 ESL.LIB BL_LOC_2100.lib BOBookDetailViewer.lib LOC_CommonAPI.lib SIReportMgr.lib SIReportData.lib CO_STATICS.lib Kolas2up_MobileCommon.lib Kolas2up_CommonControl.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\Mono\BO_LOC_2100.dll" /pdbtype:sept
# ADD LINK32 ESL.LIB BL_LOC_2100.lib BOBookDetailViewer.lib LOC_CommonAPI.lib SIReportMgr.lib SIReportData.lib CO_STATICS.lib Kolas2up_MobileCommon.lib Kolas2up_CommonControl.lib KL_API.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\Mono\BO_LOC_2100.dll" /pdbtype:sept
# Begin Target

# Name "BO_LOC_2100 - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BO_LOC_2100.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2100.def
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2100.rc
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_21001.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_21010.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2110.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2120.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2130.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2140.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2150.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2160.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2170.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2180.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2190.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2190_CLICK_LIST.cpp
# End Source File
# Begin Source File

SOURCE=.\GradientProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\ESL\KOLAS3_MAIN\MainLogo.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BO_LOC_21001.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_21010.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2110.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2120.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2130.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2140.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2150.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2160.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2170.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2180.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2190.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_2190_CLICK_LIST.h
# End Source File
# Begin Source File

SOURCE=.\GradientProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BO_LOC_2100.rc2
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\Kolas2_32.ico
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
