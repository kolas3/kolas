# Microsoft Developer Studio Project File - Name="TocSyntaxCheck" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TocSyntaxCheck - Win32 Debug_Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TocSyntaxCheck.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TocSyntaxCheck.mak" CFG="TocSyntaxCheck - Win32 Debug_Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TocSyntaxCheck - Win32 Debug_Unicode" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "Debug_Unicode"
# PROP BASE Intermediate_Dir "Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\lib"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../../../include/ecolas-c" /I "../../../../../include/syntaxcheck" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug_Unicode/TocSyntaxCheck.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Eco_Tool.lib /nologo /subsystem:windows /dll /pdb:"Debug_Unicode/TocSyntaxCheck.pdb" /debug /machine:I386 /out:"../..\..\..\..\..\DLL\����\TocSyntaxCheck.dll" /pdbtype:sept /libpath:"../../../../../lib"
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "TocSyntaxCheck - Win32 Debug_Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ErrorMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\Parse.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TocMetaData.cpp
# End Source File
# Begin Source File

SOURCE=.\TocSyntaxCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\TocSyntaxCheck.def
# End Source File
# Begin Source File

SOURCE=.\TocSyntaxCheck.rc
# End Source File
# Begin Source File

SOURCE=.\TocSyntaxChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\TocTag.cpp
# End Source File
# Begin Source File

SOURCE=.\TocTagInfo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\..\include\SyntaxCheck\ErrorMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\include\SyntaxCheck\ErrorQueue.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\include\SyntaxCheck\Parse.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\include\SyntaxCheck\TocMetaData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\include\SyntaxCheck\TocSyntaxChecker.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\include\SyntaxCheck\TocTag.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\include\SyntaxCheck\TocTagInfo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\TocSyntaxCheck.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
