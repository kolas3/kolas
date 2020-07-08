# Microsoft Developer Studio Project File - Name="Light_CENTERUPLOAD" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Light_CENTERUPLOAD - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Light_CENTERUPLOAD.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Light_CENTERUPLOAD.mak" CFG="Light_CENTERUPLOAD - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Light_CENTERUPLOAD - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "Light_CENTERUPLOAD___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "Light_CENTERUPLOAD___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\LIB"
# PROP Intermediate_Dir "Light_CENTERUPLOAD___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ESL.lib MarcEdit.lib Check_Dup_Api.lib rsmgr.lib searchmgr.lib BO_CAT_PROC.lib SIReportMgr.lib SiReportData.lib UC_IT_01_UPDATE.lib MarcToolKit.lib indextoolkit.lib MarcAdjust.lib MarcAdjustApi.lib CenterUpload_Api.lib UC_KOLISNetUseCheck.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\UC\Light_CENTERUPLOAD.dll" /pdbtype:sept
# ADD LINK32 ESL.lib MarcEdit.lib Check_Dup_Api.lib BO_CAT_PROC.lib SIReportMgr.lib SiReportData.lib MarcToolKit.lib indextoolkit.lib MarcAdjust.lib MarcAdjustApi.lib CenterUpload_Api.lib UC_KOLISNetUseCheck.lib CharEncodeMgr.lib KolisNet_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\UC\Light_CENTERUPLOAD.dll" /pdbtype:sept
# Begin Target

# Name "Light_CENTERUPLOAD - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CENTER_UPLOAD_01.cpp
# End Source File
# Begin Source File

SOURCE=.\CENTER_UPLOAD_02.cpp
# End Source File
# Begin Source File

SOURCE=.\CENTER_UPLOAD_03.cpp
# End Source File
# Begin Source File

SOURCE=.\CENTER_UPLOAD_04.cpp
# End Source File
# Begin Source File

SOURCE=.\CENTER_UPLOAD_PRINT.cpp
# End Source File
# Begin Source File

SOURCE=.\CENTER_UPLOAD_Search.cpp
# End Source File
# Begin Source File

SOURCE=.\Light_CENTERUPLOAD.cpp
# End Source File
# Begin Source File

SOURCE=.\Light_CENTERUPLOAD.def
# End Source File
# Begin Source File

SOURCE=.\Light_CENTERUPLOAD.rc
# End Source File
# Begin Source File

SOURCE=.\StatusChange.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TitleString.cpp
# End Source File
# Begin Source File

SOURCE=.\UploadMarcExportDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CENTER_UPLOAD_01.h
# End Source File
# Begin Source File

SOURCE=.\CENTER_UPLOAD_02.h
# End Source File
# Begin Source File

SOURCE=.\CENTER_UPLOAD_03.h
# End Source File
# Begin Source File

SOURCE=.\CENTER_UPLOAD_04.h
# End Source File
# Begin Source File

SOURCE=.\CENTER_UPLOAD_PRINT.h
# End Source File
# Begin Source File

SOURCE=.\CENTER_UPLOAD_Search.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StatusChange.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TitleString.h
# End Source File
# Begin Source File

SOURCE=.\UploadMarcExportDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Light_CENTERUPLOAD.rc2
# End Source File
# Begin Source File

SOURCE=".\å.bmp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\å.bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Light_CENTERUPLOAD : {3E12F56A-2645-43FF-AC8C-BC0416B44FC4}
# 	1:27:IDD_UPLOADMARCEXPORT_DIALOG:27000
# 	2:16:Resource Include:resource.h
# 	2:27:IDD_UPLOADMARCEXPORT_DIALOG:IDD_UPLOADMARCEXPORT_DIALOG
# 	2:23:UploadMarcExportDlg.cpp:UploadMarcExportDlg.cpp
# 	2:21:UploadMarcExportDlg.h:UploadMarcExportDlg.h
# 	2:10:ENUM: enum:enum
# 	2:27:CLASS: CUploadMarcExportDlg:CUploadMarcExportDlg
# 	2:19:Application Include:Light_CENTERUPLOAD.h
# End Section
