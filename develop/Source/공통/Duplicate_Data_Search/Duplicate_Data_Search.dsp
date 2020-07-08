# Microsoft Developer Studio Project File - Name="Duplicate_Data_Search" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Duplicate_Data_Search - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Duplicate_Data_Search.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Duplicate_Data_Search.mak" CFG="Duplicate_Data_Search - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Duplicate_Data_Search - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "Duplicate_Data_Search___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "Duplicate_Data_Search___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib"
# PROP Intermediate_Dir "Duplicate_Data_Search___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /D "_WINDLL" /D "_MBCS" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D _WIN32_WINNT>=0x0501 /D _WIN32_IE>=0x0600 /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /D "_DEBUG" /D _WIN32_WINNT>=0x0501 /D _WIN32_IE>=0x0600 /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib ocimgr.lib K2UP_Common.lib IndexToolKit.lib marcedit.lib BO_SPECIES_API.lib SIReportMgr.lib SIReportData.lib BO_ACQ_GROUP_MANAGER.lib BO_VOLUME_FORM.lib BO_ACQ_API.lib COMMON_CONVERSION_MGR.lib BO_ACQ_PRINT_MGR.lib EPCQueryService.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../../dll/common/Duplicate_Data_Search.dll" /pdbtype:sept
# ADD LINK32 esl.lib ocimgr.lib K2UP_Common.lib IndexToolKit.lib marcedit.lib BO_SPECIES_API.lib SIReportMgr.lib SIReportData.lib BO_ACQ_GROUP_MANAGER.lib BO_VOLUME_FORM.lib BO_ACQ_API.lib COMMON_CONVERSION_MGR.lib BO_ACQ_PRINT_MGR.lib EPCQueryService.lib CharEncodeMgr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../../dll/common/Duplicate_Data_Search.dll" /pdbtype:sept
# Begin Target

# Name "Duplicate_Data_Search - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BO_HOLDING_ALL_DUP_SEARCH.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_HOLDING_DUP_DETAIL_INFO.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_INPUT_DUP_DETAIL_INFO.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_Request_Duplicate_DetailInfo_Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_WORK_DUP_DETAIL_INFO.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_WORK_DUP_SEARCH.cpp
# End Source File
# Begin Source File

SOURCE=.\CatSearchQueryMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\DUP_KEY_SETTING.cpp
# End Source File
# Begin Source File

SOURCE=.\DupKeyMakeMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\DupKeyMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\DupKeyMakerByDM.cpp
# End Source File
# Begin Source File

SOURCE=.\DupKeyMakerByGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\DupKeyMakerByIndividualInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DupKeyMakerByRequestInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DupKeyMakerByTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\Duplicate_Data_Search.cpp
# End Source File
# Begin Source File

SOURCE=.\Duplicate_Data_Search.def
# End Source File
# Begin Source File

SOURCE=.\Duplicate_Data_Search.rc
# End Source File
# Begin Source File

SOURCE=.\DuplicateSearch_Interface.cpp
# End Source File
# Begin Source File

SOURCE=.\DuplicateSearchField.cpp
# End Source File
# Begin Source File

SOURCE=.\DuplicateSearchKey.cpp
# End Source File
# Begin Source File

SOURCE=.\DuplicateSearchMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ECOSearchQueryMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\QueryMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BO_HOLDING_ALL_DUP_SEARCH.h
# End Source File
# Begin Source File

SOURCE=.\BO_HOLDING_DUP_DETAIL_INFO.h
# End Source File
# Begin Source File

SOURCE=.\BO_INPUT_DUP_DETAIL_INFO.h
# End Source File
# Begin Source File

SOURCE=.\BO_Request_Duplicate_DetailInfo_Dlg.h
# End Source File
# Begin Source File

SOURCE=.\BO_WORK_DUP_DETAIL_INFO.h
# End Source File
# Begin Source File

SOURCE=.\BO_WORK_DUP_SEARCH.h
# End Source File
# Begin Source File

SOURCE=.\CatSearchQueryMaker.h
# End Source File
# Begin Source File

SOURCE=.\DUP_KEY_SETTING.h
# End Source File
# Begin Source File

SOURCE=.\DupKeyMakeMgr.h
# End Source File
# Begin Source File

SOURCE=.\DupKeyMaker.h
# End Source File
# Begin Source File

SOURCE=.\DupKeyMakerByDM.h
# End Source File
# Begin Source File

SOURCE=.\DupKeyMakerByGrid.h
# End Source File
# Begin Source File

SOURCE=.\DupKeyMakerByIndividualInfo.h
# End Source File
# Begin Source File

SOURCE=.\DupKeyMakerByRequestInfo.h
# End Source File
# Begin Source File

SOURCE=.\DupKeyMakerByTitle.h
# End Source File
# Begin Source File

SOURCE=.\DuplicateSearch_Interface.h
# End Source File
# Begin Source File

SOURCE=.\DuplicateSearchField.h
# End Source File
# Begin Source File

SOURCE=.\DuplicateSearchKey.h
# End Source File
# Begin Source File

SOURCE=.\DuplicateSearchMgr.h
# End Source File
# Begin Source File

SOURCE=.\ECOSearchQueryMaker.h
# End Source File
# Begin Source File

SOURCE=.\QueryMaker.h
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

SOURCE=.\res\Duplicate_Data_Search.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
