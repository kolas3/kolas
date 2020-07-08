# Microsoft Developer Studio Project File - Name="UI_SEARCH_DLG" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=UI_SEARCH_DLG - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UI_SEARCH_DLG.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UI_SEARCH_DLG.mak" CFG="UI_SEARCH_DLG - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UI_SEARCH_DLG - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "UI_SEARCH_DLG___Win32_Unicode_Debug0"
# PROP BASE Intermediate_Dir "UI_SEARCH_DLG___Win32_Unicode_Debug0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\LIB"
# PROP Intermediate_Dir "UI_SEARCH_DLG___Win32_Unicode_Debug0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ESL.lib MarcEdit.lib SearchMgr.lib KOMarcGetter.lib K2UP_Common.lib IndexToolKit.lib Imm32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\UC\UI_SEARCH_DLG.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ESL.lib MarcEdit.lib KOMarcGetter.lib K2UP_Common.lib IndexToolKit.lib Imm32.lib CharEncodeMgr.lib KolisNet_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\UC\UI_SEARCH_DLG.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "UI_SEARCH_DLG - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Cmd_Search_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Code_Search_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeDecision.cpp
# End Source File
# Begin Source File

SOURCE=.\Detail_Search_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Fetch.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib_Code_Search_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Major_Search_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Parse.cpp
# End Source File
# Begin Source File

SOURCE=.\Query.cpp
# End Source File
# Begin Source File

SOURCE=.\Search.cpp
# End Source File
# Begin Source File

SOURCE=.\Search_Main_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchCond.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\UI_SEARCH_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\UI_SEARCH_DLG.def
# End Source File
# Begin Source File

SOURCE=.\UI_SEARCH_DLG.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Cmd_Search_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Code_Search_Dlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\CodeDecision.h
# End Source File
# Begin Source File

SOURCE=.\Detail_Search_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Lib_Code_Search_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Major_Search_Dlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\Parse.h
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\Query.h"
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\Search.h"
# End Source File
# Begin Source File

SOURCE=.\Search_Main_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\SearchCond.h
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

SOURCE=.\res\UI_SEARCH_DLG.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Light_GatherDataSearch : {0ECD9B64-23AA-11D0-B351-00A0C9055D8E}
# 	2:21:DefaultSinkHeaderFile:mshflexgrid.h
# 	2:16:DefaultSinkClass:CMSHFlexGrid
# End Section
# Section Light_GatherDataSearch : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section Light_GatherDataSearch : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section Light_GatherDataSearch : {0ECD9B62-23AA-11D0-B351-00A0C9055D8E}
# 	2:5:Class:CMSHFlexGrid
# 	2:10:HeaderFile:mshflexgrid.h
# 	2:8:ImplFile:mshflexgrid.cpp
# End Section
# Section Light_GatherDataSearch : {0000050E-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:CRecordset1
# 	2:10:HeaderFile:recordset.h
# 	2:8:ImplFile:recordset.cpp
# End Section
