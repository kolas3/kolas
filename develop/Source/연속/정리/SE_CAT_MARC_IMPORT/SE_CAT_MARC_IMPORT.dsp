# Microsoft Developer Studio Project File - Name="SE_CAT_MARC_IMPORT" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SE_CAT_MARC_IMPORT - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SE_CAT_MARC_IMPORT.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SE_CAT_MARC_IMPORT.mak" CFG="SE_CAT_MARC_IMPORT - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SE_CAT_MARC_IMPORT - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "SE_CAT_MARC_IMPORT___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "SE_CAT_MARC_IMPORT___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "SE_CAT_MARC_IMPORT___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ESL.lib MarcAdjustApi.lib MarcToolkit.lib MarcEdit.lib IndexToolKit.lib BO_CAT_PROC_KEYWORD.lib K2UP_Common.lib BO_SPECIES_API.lib MarcAdjust.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\serial\SE_CAT_MARC_IMPORT.dll" /pdbtype:sept
# ADD LINK32 ESL.lib MarcAdjustApi.lib MarcToolkit.lib MarcEdit.lib IndexToolKit.lib BO_CAT_PROC_KEYWORD.lib K2UP_Common.lib BO_SPECIES_API.lib MarcAdjust.lib CharEncodeMgr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\serial\SE_CAT_MARC_IMPORT.dll" /pdbtype:sept
# Begin Target

# Name "SE_CAT_MARC_IMPORT - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MarcEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcImportDlg_SE.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_MARC_IMPORT.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_MARC_IMPORT.def
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_MARC_IMPORT.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MarcEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\MarcImportDlg_SE.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\SE_CAT_MARC_IMPORT.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section SE_CAT_MARC_IMPORT : {7BB7D426-EAB9-4E1B-94BC-8D1F10A3D01B}
# 	1:24:IDD_MARC_IMPORT_CAT_UNIV:9001
# 	2:16:Resource Include:resource.h
# 	2:17:MarcImportDlg.cpp:MarcImportDlg.cpp
# 	2:24:IDD_MARC_IMPORT_CAT_UNIV:IDD_MARC_IMPORT_CAT_UNIV
# 	2:21:CLASS: CMarcImportDlg:CMarcImportDlg
# 	2:10:ENUM: enum:enum
# 	2:15:MarcImportDlg.h:MarcImportDlg.h
# 	2:19:Application Include:SE_CAT_MARC_IMPORT.h
# End Section
# Section SE_CAT_MARC_IMPORT : {92A490FE-B3AD-47AE-87F5-832DFAB2E308}
# 	2:13:CLASS: CMemDC:CMemDC
# 	2:20:TextProgressCtrl.cpp:TextProgressCtrl.cpp
# 	2:19:Application Include:SE_CAT_MARC_IMPORT.h
# End Section
# Section SE_CAT_MARC_IMPORT : {5D78F3A7-4F1F-4A25-BEE4-D17B26DDFF34}
# 	2:24:CLASS: CTextProgressCtrl:CTextProgressCtrl
# 	2:20:TextProgressCtrl.cpp:TextProgressCtrl1.cpp
# 	2:18:TextProgressCtrl.h:TextProgressCtrl.h
# 	2:19:Application Include:SE_CAT_MARC_IMPORT.h
# End Section
# Section SE_CAT_MARC_IMPORT : {8B81D3BB-E3F4-457E-A272-5861A006BEA5}
# 	1:17:IDD_MARC_EDIT_CAT:9000
# 	2:16:Resource Include:resource.h
# 	2:17:IDD_MARC_EDIT_CAT:IDD_MARC_EDIT_CAT
# 	2:15:MarcEditDlg.cpp:MarcEditDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:13:MarcEditDlg.h:MarcEditDlg.h
# 	2:19:CLASS: CMarcEditDlg:CMarcEditDlg
# 	2:19:Application Include:SE_CAT_MARC_IMPORT.h
# End Section
