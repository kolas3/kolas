# Microsoft Developer Studio Project File - Name="BO_LOC_ARREAR" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BO_LOC_ARREAR - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BO_LOC_ARREAR.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_LOC_ARREAR.mak" CFG="BO_LOC_ARREAR - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_LOC_ARREAR - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "Unicode Debug"
# PROP BASE Intermediate_Dir "Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\lib"
# PROP Intermediate_Dir "Unicode Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ESL.lib LOC_CommonAPI.lib kl_api.lib Kolas2up_MobileCommon.lib SIReportMgr.lib SIReportData.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\mono\BO_LOC_ARREAR.dll" /pdbtype:sept
# Begin Target

# Name "BO_LOC_ARREAR - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ArrearAllDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ArrearInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ArrearMainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ArrearPaymentDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ArrearStatisticsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ArrearUnpaidDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ArrearViewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_ARREAR.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_ARREAR.def
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_ARREAR.rc
# End Source File
# Begin Source File

SOURCE=.\PaymentRunDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ArrearAllDlg.h
# End Source File
# Begin Source File

SOURCE=.\ArrearInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\ArrearMainDlg.h
# End Source File
# Begin Source File

SOURCE=.\ArrearPaymentDlg.h
# End Source File
# Begin Source File

SOURCE=.\ArrearStatisticsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ArrearUnpaidDlg.h
# End Source File
# Begin Source File

SOURCE=.\ArrearViewDlg.h
# End Source File
# Begin Source File

SOURCE=.\PaymentRunDlg.h
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

SOURCE=.\res\BO_LOC_ARREAR.rc2
# End Source File
# Begin Source File

SOURCE=.\res\cube_icon_small_32.ico
# End Source File
# Begin Source File

SOURCE=.\res\ExpressTouch.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
