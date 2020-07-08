# Microsoft Developer Studio Project File - Name="SIReportMgr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SIReportMgr - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SIReportMgr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SIReportMgr.mak" CFG="SIReportMgr - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SIReportMgr - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "SIReportMgr___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "SIReportMgr___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "SIReportMgr___Win32_Unicode_Debug"
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
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SIReportData.lib Esl.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\DLL\COMMON\SIReportMgr.dll" /pdbtype:sept
# ADD LINK32 SIReportData.lib Esl.lib ExcelMgr.lib CharEncodeMgr.lib IMM32.LIB /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\DLL\COMMON\SIReportMgr.dll" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=rem copy ..\..\..\..\DLL\COMMON\SIReportMgr.* f:\dll\common	rem copy ..\..\..\lib\COMMON\SIReportMgr.* f:\develop\lib
# End Special Build Tool
# Begin Target

# Name "SIReportMgr - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ESLDlg_UserExportReason.cpp
# End Source File
# Begin Source File

SOURCE=.\NewButton.cpp
# End Source File
# Begin Source File

SOURCE=.\NewButtonList.cpp
# End Source File
# Begin Source File

SOURCE=.\SIReportManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SIReportMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SIReportMgr.def
# End Source File
# Begin Source File

SOURCE=.\SIReportMgr.rc
# End Source File
# Begin Source File

SOURCE=.\SIReportPreviewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SIReportPreviewView.cpp
# End Source File
# Begin Source File

SOURCE=.\SIReportPrintDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdioFileEx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ESLDlg_UserExportReason.h
# End Source File
# Begin Source File

SOURCE=.\NewButton.h
# End Source File
# Begin Source File

SOURCE=.\NewButtonList.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SIReportManager.h
# End Source File
# Begin Source File

SOURCE=.\SIReportPreviewDlg.h
# End Source File
# Begin Source File

SOURCE=.\SIReportPreviewView.h
# End Source File
# Begin Source File

SOURCE=.\SIReportPrintDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StdioFileEx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap\CLOSED.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\CLOSEU.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\CLOSEX.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\ENDD.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\ENDU.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\ENDX.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\excel_export_d.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\excel_export_f.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\excel_export_n.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\FIRSTD.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\FIRSTU.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\FIRSTX.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\MODIFYD.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\MODIFYF.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\MODIFYFORM_D.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\MODIFYFORM_F.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\MODIFYFORM_X.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\modifyvalue_d.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\modifyvalue_f.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\modifyvalue_x.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\MODIFYX.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\NEXTD.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\NEXTU.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\NEXTX.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\PREVD.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\PREVU.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\PREVX.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\PRINTD.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\printdialog.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\PRINTU.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\PRINTX.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\REFRESHD.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\REFRESHU.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\REFRESHX.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\ZOOMIND.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\ZOOMINU.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap\ZOOMINX.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=".\@Version.txt"
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
