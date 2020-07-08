# Microsoft Developer Studio Project File - Name="BO_CAT_PROC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BO_CAT_PROC - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BO_CAT_PROC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_CAT_PROC.mak" CFG="BO_CAT_PROC - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_CAT_PROC - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "BO_CAT_PROC___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BO_CAT_PROC___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "BO_CAT_PROC___Win32_Unicode_Debug"
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
# ADD BASE LINK32 esl.lib marcedit.lib MNG_CLASS_SIGN_POPUP.lib IndexToolKit.lib K2UP_Common.LIB LOC_CommonAPI.lib Duplicate_Data_Search.lib bo_species_api.lib BO_CAT_PROC_KEYWORD.lib FileImport.lib MarcToolKit.lib MNG_CODE_SHELF.LIB DonatorNameGetter.lib BO_CAT_NUMBER.LIB KOMarcGetter.lib Bo_Marc_Import.lib SIReportMgr.lib SiReportData.lib Light_GatherDataSearch.LIB UI_SEARCH_DLG.LIB BO_CAT_PROC_FIXEDFIELD.LIB CatCodeGetter.lib BO_CAT_DUP_FREE.LIB ShelfCodeGetter.lib Light_BO_CAT_CENTERUPLOAD.lib BusinessPopupSearcher.lib Kolas2up_MobileCommon.lib BO_VOLUME_FORM.lib BO_ACQ_GROUP_MANAGER.lib TOCEDITOR.lib MarcAdjustApi.lib COVERMANAGER.lib IndexToolKit.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\MONO\BO_CAT_PROC.dll" /pdbtype:sept
# ADD LINK32 marcedit.lib MNG_CLASS_SIGN_POPUP.lib K2UP_Common.LIB bo_species_api.lib BO_CAT_PROC_KEYWORD.lib FileImport.lib MarcToolKit.lib MNG_CODE_SHELF.LIB DonatorNameGetter.lib BO_CAT_NUMBER.LIB KOMarcGetter.lib Bo_Marc_Import.lib SiReportData.lib Light_GatherDataSearch.LIB UI_SEARCH_DLG.LIB BO_CAT_PROC_FIXEDFIELD.LIB CatCodeGetter.lib ShelfCodeGetter.lib BusinessPopupSearcher.lib BO_VOLUME_FORM.lib TOCEDITOR.lib COVERMANAGER.lib esl.lib IndexToolKit.lib LOC_CommonAPI.lib Duplicate_Data_Search.lib SIReportMgr.lib BO_CAT_DUP_FREE.LIB Light_BO_CAT_CENTERUPLOAD.lib Kolas2up_MobileCommon.lib BO_ACQ_GROUP_MANAGER.lib MarcAdjustApi.lib CharEncodeMgr.lib KL_Api.lib KolisNet_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\MONO\BO_CAT_PROC.dll" /pdbtype:sept
# Begin Target

# Name "BO_CAT_PROC - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BO_CAT_PROC.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC.def
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC.rc
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0000.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0100.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0200.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0201.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0202.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0202_PRINT.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0203.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0203_01.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0203_02.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0204.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0204_01.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0204_CLICK_LIST.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0300.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0301.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0301_01.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0301_ROLE.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0301_ROLE_01.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0301_ROLE_02.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0310.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0320.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0330.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0332.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0360.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0361.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0362.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0371.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0373.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0380.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0382.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0390.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0400.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0400_08.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0500_09.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0510.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0801.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_1100.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_1200.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_1300.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_AcqNumber.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_Api.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\Kolas2up_Card.cpp
# End Source File
# Begin Source File

SOURCE=.\Kolas2up_CardList.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\recordset.cpp
# End Source File
# Begin Source File

SOURCE=.\SPF.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BO_CAT_PROC_0000.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0100.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0200.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0201.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0202.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0202_PRINT.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0203.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0203_01.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0203_02.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0204.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0204_01.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0204_CLICK_LIST.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0300.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0301.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0301_01.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0301_ROLE.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0301_ROLE_01.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0301_ROLE_02.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0310.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0320.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0330.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0332.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0360.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0361.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0362.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0371.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0373.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0380.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0382.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0390.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0400.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0400_08.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0500_09.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0510.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_0801.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_1100.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_1200.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_1300.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_AcqNumber.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_Api.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\Kolas2up_Card.h
# End Source File
# Begin Source File

SOURCE=.\Kolas2up_CardList.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\recordset.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SPF.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BO_CAT_PROC.rc2
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\cursor2.cur
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section BO_CAT_PROC : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section BO_CAT_PROC : {0ECD9B62-23AA-11D0-B351-00A0C9055D8E}
# 	2:5:Class:CMSHFlexGrid
# 	2:10:HeaderFile:mshflexgrid.h
# 	2:8:ImplFile:mshflexgrid.cpp
# End Section
# Section BO_CAT_PROC : {0000050E-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:CRecordset1
# 	2:10:HeaderFile:recordset.h
# 	2:8:ImplFile:recordset.cpp
# End Section
# Section BO_CAT_PROC : {0ECD9B64-23AA-11D0-B351-00A0C9055D8E}
# 	2:21:DefaultSinkHeaderFile:mshflexgrid.h
# 	2:16:DefaultSinkClass:CMSHFlexGrid
# End Section
# Section BO_CAT_PROC : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
