# Microsoft Developer Studio Project File - Name="MarcManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MarcManager - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MarcManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MarcManager.mak" CFG="MarcManager - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MarcManager - Win32 Unicode Debug" (based on "Win32 (x86) Application")
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
# PROP BASE Output_Dir "MarcManager___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "MarcManager___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MarcManager___Win32_Unicode_Debug"
# PROP Intermediate_Dir "MarcManager___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ESL.lib MarcToolKit.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\..\bin\MarcRefManager.exe" /pdbtype:sept
# ADD LINK32 ESL.lib MarcToolKit.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\..\bin\MarcRefManager.exe" /pdbtype:sept
# Begin Target

# Name "MarcManager - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcManager.rc
# End Source File
# Begin Source File

SOURCE=.\MarcManagerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcManagerEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcManagerEditorInsert.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcManagerGroupInsert.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcManagerGroupManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcManagerGroupSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcManagerIndicator.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcManagerRelation.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcManagerSubfield.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcManagerTagInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcManagerView.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcManagerWorkView.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MarcManager.h
# End Source File
# Begin Source File

SOURCE=.\MarcManagerDoc.h
# End Source File
# Begin Source File

SOURCE=.\MarcManagerEditor.h
# End Source File
# Begin Source File

SOURCE=.\MarcManagerEditorInsert.h
# End Source File
# Begin Source File

SOURCE=.\MarcManagerGroupInsert.h
# End Source File
# Begin Source File

SOURCE=.\MarcManagerGroupManager.h
# End Source File
# Begin Source File

SOURCE=.\MarcManagerGroupSelect.h
# End Source File
# Begin Source File

SOURCE=.\MarcManagerIndicator.h
# End Source File
# Begin Source File

SOURCE=.\MarcManagerRelation.h
# End Source File
# Begin Source File

SOURCE=.\MarcManagerSubfield.h
# End Source File
# Begin Source File

SOURCE=.\MarcManagerTagInfo.h
# End Source File
# Begin Source File

SOURCE=.\MarcManagerView.h
# End Source File
# Begin Source File

SOURCE=.\MarcManagerWorkView.h
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.h
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

SOURCE=.\res\MarcManager.ico
# End Source File
# Begin Source File

SOURCE=.\res\MarcManager.rc2
# End Source File
# Begin Source File

SOURCE=.\res\MarcManagerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
