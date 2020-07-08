# Microsoft Developer Studio Project File - Name="Eco_Tool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Eco_Tool - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Eco_Tool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Eco_Tool.mak" CFG="Eco_Tool - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Eco_Tool - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "Eco_Tool___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "Eco_Tool___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\LIB"
# PROP Intermediate_Dir "UnicodeDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 exception.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"..\..\..\..\dll\COMMON\Eco_Tool.dll" /pdbtype:sept
# ADD LINK32 EXCEPTION.LIB /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\common\ECO_TOOL.DLL" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no
# Begin Target

# Name "Eco_Tool - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ECO_COMMON_API.cpp
# End Source File
# Begin Source File

SOURCE=.\ECO_FILE_API.cpp
# End Source File
# Begin Source File

SOURCE=.\ECO_FileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\ECO_STANDARD_FUNC.cpp
# End Source File
# Begin Source File

SOURCE=.\ECO_Time.cpp
# End Source File
# Begin Source File

SOURCE=.\Eco_Tool.cpp
# End Source File
# Begin Source File

SOURCE=.\Eco_Tool.def
# End Source File
# Begin Source File

SOURCE=.\Eco_Tool.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Win32api.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\include\ECO_COMMON_API.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ECO_FILE_API.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ECO_FileIO.h
# End Source File
# Begin Source File

SOURCE=.\ECO_STANDARD_FUNC.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ECO_Time.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Win32api.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Eco_Tool.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
