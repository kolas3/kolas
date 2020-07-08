# Microsoft Developer Studio Project File - Name="BO_CAT_REG" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BO_CAT_REG - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BO_CAT_REG.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_CAT_REG.mak" CFG="BO_CAT_REG - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_CAT_REG - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "BO_CAT_REG___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BO_CAT_REG___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "BO_CAT_REG___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib K2UP_Common.LIB IndexToolKit.lib BO_CAT_REGNO_GIVE.LIB BO_CAT_REGNO_CANCEL.LIB BO_CAT_LAST_NUMBER.lib BO_ACQ_GROUP_MANAGER.lib BO_ACQ_SPECIES_FORM.lib BO_ACQ_API.lib BO_SPECIES_API.lib FileImport.lib MarcToolKit.lib SIReportData.lib SIReportMgr.lib MarcAdjustApi.lib COMMON_CONVERSION_MGR.lib BO_DATA_CHANGE_MANAGER.LIB BO_ACQ_COMMON.LIB BO_DATA_MANAGER.lib DATA_SORT_MANAGER.lib imm32.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib EPCQueryService.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\MONO\BO_CAT_REG.dll" /pdbtype:sept
# ADD LINK32 esl.lib K2UP_Common.LIB IndexToolKit.lib BO_CAT_REGNO_GIVE.LIB BO_CAT_REGNO_CANCEL.LIB BO_CAT_LAST_NUMBER.lib BO_ACQ_GROUP_MANAGER.lib BO_ACQ_SPECIES_FORM.lib BO_ACQ_API.lib BO_SPECIES_API.lib FileImport.lib MarcToolKit.lib SIReportData.lib SIReportMgr.lib MarcAdjustApi.lib COMMON_CONVERSION_MGR.lib BO_DATA_CHANGE_MANAGER.LIB BO_ACQ_COMMON.LIB BO_DATA_MANAGER.lib DATA_SORT_MANAGER.lib imm32.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib CharEncodeMgr.lib CMarcChanger.lib KL_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\MONO\BO_CAT_REG.dll" /pdbtype:sept
# Begin Target

# Name "BO_CAT_REG - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BO_CAT_GIVE_REGNO.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_GROUP_INSERT.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_GROUP_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_MAKE_preGROUP.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PREGROUP_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_1100.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG.def
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG.rc
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0400.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0410.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0420.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0500.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0510.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0510_01.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0520.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0800.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_PROGRESS_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_SEARCH.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_SEARCH_MGR_GROUP.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_SEARCH_MGR_PRE.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_preGROUP_MNG.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_REG_CODE.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_REG_MGR.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiSortDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BO_CAT_GIVE_REGNO.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_GROUP_INSERT.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_GROUP_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_MAKE_preGROUP.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PREGROUP_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_PROC_1100.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0400.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0410.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0420.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0500.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0510.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0510_01.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0520.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0800.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_PROGRESS_DLG.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_SEARCH.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_SEARCH_MGR_GROUP.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_SEARCH_MGR_PRE.h
# End Source File
# Begin Source File

SOURCE=.\BO_preGROUP_MNG.h
# End Source File
# Begin Source File

SOURCE=.\BO_REG_CODE.h
# End Source File
# Begin Source File

SOURCE=.\BO_REG_MGR.h
# End Source File
# Begin Source File

SOURCE=.\MultiSortDlg.h
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

SOURCE=.\res\BO_CAT_REG.rc2
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
