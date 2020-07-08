# Microsoft Developer Studio Project File - Name="FlowMaker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FlowMaker - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FlowMaker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FlowMaker.mak" CFG="FlowMaker - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FlowMaker - Win32 Unicode Debug" (based on "Win32 (x86) Application")
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
# PROP BASE Output_Dir "FlowMaker___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "FlowMaker___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FlowMaker___Win32_Unicode_Debug"
# PROP Intermediate_Dir "FlowMaker___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ESL.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\bin\FlowMaker.exe" /pdbtype:sept
# ADD LINK32 ESL.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\bin\FlowMaker.exe" /pdbtype:sept
# Begin Target

# Name "FlowMaker - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FlowMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\FlowMaker.rc
# End Source File
# Begin Source File

SOURCE=.\FlowMakerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SPF_MENU_INSERT.cpp
# End Source File
# Begin Source File

SOURCE=.\SPF_NEW_GROUP.cpp
# End Source File
# Begin Source File

SOURCE=.\SPF_NEW_PATH.cpp
# End Source File
# Begin Source File

SOURCE=.\SPF_NEW_PROJECT.cpp
# End Source File
# Begin Source File

SOURCE=.\SPF_SCREEN_MGR.cpp
# End Source File
# Begin Source File

SOURCE=.\SPF_TOOL_INSERT.cpp
# End Source File
# Begin Source File

SOURCE=.\SPFOpenProject.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FlowMaker.h
# End Source File
# Begin Source File

SOURCE=.\FlowMakerDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SPF_MENU_INSERT.h
# End Source File
# Begin Source File

SOURCE=.\SPF_NEW_GROUP.h
# End Source File
# Begin Source File

SOURCE=.\SPF_NEW_PATH.h
# End Source File
# Begin Source File

SOURCE=.\SPF_NEW_PROJECT.h
# End Source File
# Begin Source File

SOURCE=.\SPF_SCREEN_MGR.h
# End Source File
# Begin Source File

SOURCE=.\SPF_TOOL_INSERT.h
# End Source File
# Begin Source File

SOURCE=.\SPFOpenProject.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\FlowMaker.ico
# End Source File
# Begin Source File

SOURCE=.\res\FlowMaker.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
