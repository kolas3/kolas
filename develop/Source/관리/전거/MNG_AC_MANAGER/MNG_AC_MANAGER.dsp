# Microsoft Developer Studio Project File - Name="MNG_AC_MANAGER" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MNG_AC_MANAGER - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MNG_AC_MANAGER.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MNG_AC_MANAGER.mak" CFG="MNG_AC_MANAGER - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MNG_AC_MANAGER - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "MNG_AC_MANAGER___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "MNG_AC_MANAGER___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "MNG_AC_MANAGER___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib CAT_FIXED_FIELD.lib ESL.lib MarcToolkit.lib MarcEdit.lib IndexToolKit.lib BO_CAT_PROC_KEYWORD.lib K2UP_Common.lib MarcAdjustApi.lib SIReportData.lib SIReportMgr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mng\MNG_AC_MANAGER.dll" /pdbtype:sept
# ADD LINK32 esl.lib CAT_FIXED_FIELD.lib ESL.lib MarcToolkit.lib MarcEdit.lib IndexToolKit.lib BO_CAT_PROC_KEYWORD.lib K2UP_Common.lib MarcAdjustApi.lib SIReportData.lib SIReportMgr.lib CharEncodeMgr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mng\MNG_AC_MANAGER.dll" /pdbtype:sept
# Begin Target

# Name "MNG_AC_MANAGER - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ACMARCGetterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcExportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcImportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER.def
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER.rc
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER_01.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER_02.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER_03.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER_04.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER_04_1.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER_DOBORA.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_SELMODE.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowACMarcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ACMARCGetterDlg.h
# End Source File
# Begin Source File

SOURCE=.\MarcExportDlg.h
# End Source File
# Begin Source File

SOURCE=.\MarcImportDlg.h
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER_01.h
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER_02.h
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER_03.h
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER_04.h
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER_04_1.h
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_MANAGER_DOBORA.h
# End Source File
# Begin Source File

SOURCE=.\MNG_AC_SELMODE.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ShowACMarcDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\MNG_AC_MANAGER.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section MNG_AC_MANAGER : {0ECD9B62-23AA-11D0-B351-00A0C9055D8E}
# 	2:5:Class:CMSHFlexGrid
# 	2:10:HeaderFile:mshflexgrid.h
# 	2:8:ImplFile:mshflexgrid.cpp
# End Section
# Section MNG_AC_MANAGER : {A8BA81F1-E769-4D02-B0E8-6B573C2BD82C}
# 	1:17:IDD_ShowACMarcDlg:22019
# 	2:16:Resource Include:resource.h
# 	2:21:CLASS: CShowACMarcDlg:CShowACMarcDlg
# 	2:17:IDD_ShowACMarcDlg:IDD_ShowACMarcDlg
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MNG_AC_MANAGER.h
# 	2:17:ShowACMarcDlg.cpp:ShowACMarcDlg1.cpp
# 	2:15:ShowACMarcDlg.h:ShowACMarcDlg1.h
# End Section
# Section MNG_AC_MANAGER : {0000050E-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:CRecordset1
# 	2:10:HeaderFile:recordset.h
# 	2:8:ImplFile:recordset.cpp
# End Section
# Section MNG_AC_MANAGER : {95BF997E-DDF7-4CBA-880E-8AAFC19A64DF}
# 	1:19:IDD_ACMARCGetterDlg:22018
# 	2:16:Resource Include:resource.h
# 	2:19:ACMARCGetterDlg.cpp:ACMARCGetterDlg1.cpp
# 	2:10:ENUM: enum:enum
# 	2:17:ACMARCGetterDlg.h:ACMARCGetterDlg1.h
# 	2:23:CLASS: CACMARCGetterDlg:CACMARCGetterDlg
# 	2:19:Application Include:MNG_AC_MANAGER.h
# 	2:19:IDD_ACMARCGetterDlg:IDD_ACMARCGetterDlg
# End Section
# Section MNG_AC_MANAGER : {0ECD9B64-23AA-11D0-B351-00A0C9055D8E}
# 	2:21:DefaultSinkHeaderFile:mshflexgrid.h
# 	2:16:DefaultSinkClass:CMSHFlexGrid
# End Section
# Section MNG_AC_MANAGER : {210EB1F3-0E1C-4857-BBE5-36E7192FA285}
# 	1:19:IDD_MARC_IMPORT_CAT:32028
# 	2:16:Resource Include:resource.h
# 	2:17:MarcImportDlg.cpp:MarcImportDlg.cpp
# 	2:21:CLASS: CMarcImportDlg:CMarcImportDlg
# 	2:10:ENUM: enum:enum
# 	2:15:MarcImportDlg.h:MarcImportDlg.h
# 	2:19:IDD_MARC_IMPORT_CAT:IDD_MARC_IMPORT_CAT
# 	2:19:Application Include:mng_ac_manager.h
# End Section
# Section MNG_AC_MANAGER : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section MNG_AC_MANAGER : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section MNG_AC_MANAGER : {4A648494-6608-48AA-903D-EFF4217D8B0B}
# 	1:19:IDD_MARC_EXPORT_CAT:32020
# 	2:16:Resource Include:resource.h
# 	2:15:MarcExportDlg.h:MarcExportDlg.h
# 	2:21:CLASS: CMarcExportDlg:CMarcExportDlg
# 	2:19:IDD_MARC_EXPORT_CAT:IDD_MARC_EXPORT_CAT
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mng_ac_manager.h
# 	2:17:MarcExportDlg.cpp:MarcExportDlg.cpp
# End Section
# Section MNG_AC_MANAGER : {D19FDE78-B5E5-4E0C-BD61-63F5D4A69C49}
# 	1:18:IDD_REVISION_PRINT:32029
# 	2:16:Resource Include:resource.h
# 	2:20:RevisionPrintDlg.cpp:RevisionPrintDlg.cpp
# 	2:24:CLASS: CRevisionPrintDlg:CRevisionPrintDlg
# 	2:18:RevisionPrintDlg.h:RevisionPrintDlg.h
# 	2:10:ENUM: enum:enum
# 	2:18:IDD_REVISION_PRINT:IDD_REVISION_PRINT
# 	2:19:Application Include:mng_ac_manager.h
# End Section
