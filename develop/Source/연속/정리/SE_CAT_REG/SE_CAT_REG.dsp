# Microsoft Developer Studio Project File - Name="SE_CAT_REG" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SE_CAT_REG - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SE_CAT_REG.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SE_CAT_REG.mak" CFG="SE_CAT_REG - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SE_CAT_REG - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "SE_CAT_REG___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "SE_CAT_REG___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SE_CAT_REG___Win32_Unicode_Debug"
# PROP Intermediate_Dir "SE_CAT_REG___Win32_Unicode_Debug"
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
# ADD BASE LINK32 esl.lib BO_CAT_LAST_NUMBER.lib K2UP_Common.LIB IndexToolKit.lib Kolas2up_CommonControl.lib SIReportData.lib SIReportMgr.lib bo_cat_regno_give.lib BO_CAT_REG.lib se_api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\SERIAL\SE_CAT_REG.dll" /pdbtype:sept
# ADD LINK32 esl.lib BO_CAT_LAST_NUMBER.lib K2UP_Common.LIB IndexToolKit.lib Kolas2up_CommonControl.lib SIReportData.lib SIReportMgr.lib bo_cat_regno_give.lib BO_CAT_REG.lib se_api.lib CharEncodeMgr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\SERIAL\SE_CAT_REG.dll" /pdbtype:sept
# Begin Target

# Name "SE_CAT_REG - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BO_CAT_REG_0510_01.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0800.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiSortDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_CODE_CHANGE.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_GIVE_REGNO.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_GROUP_INSERT.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_GROUP_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_MAKE_preGROUP.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_PREGROUP_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_REG.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_REG.def
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_REG.rc
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_REG_0400.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_REG_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_REG_VIEW.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_SEARCH_MGR.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_SEARCH_MGR_GROUP.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_SEARCH_MGR_PRE.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_preGROUP_MNG.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_REG_BIND_LIST.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BO_CAT_REG_0510_01.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_0800.h
# End Source File
# Begin Source File

SOURCE=.\MultiSortDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_CODE_CHANGE.h
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_GIVE_REGNO.h
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_GROUP_INSERT.h
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_GROUP_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_MAKE_preGROUP.h
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_PREGROUP_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_REG_0400.h
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_REG_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_REG_VIEW.h
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_SEARCH_MGR.h
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_SEARCH_MGR_GROUP.h
# End Source File
# Begin Source File

SOURCE=.\SE_CAT_SEARCH_MGR_PRE.h
# End Source File
# Begin Source File

SOURCE=.\SE_preGROUP_MNG.h
# End Source File
# Begin Source File

SOURCE=.\SE_REG_BIND_LIST.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\SE_CAT_REG.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
