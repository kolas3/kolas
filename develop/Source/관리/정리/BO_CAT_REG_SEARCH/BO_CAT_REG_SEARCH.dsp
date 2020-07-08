# Microsoft Developer Studio Project File - Name="BO_CAT_REG_SEARCH" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BO_CAT_REG_SEARCH - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BO_CAT_REG_SEARCH.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_CAT_REG_SEARCH.mak" CFG="BO_CAT_REG_SEARCH - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_CAT_REG_SEARCH - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "BO_CAT_REG_SEARCH___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BO_CAT_REG_SEARCH___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "BO_CAT_REG_SEARCH___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib marcedit.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mono\BO_CAT_REG_SEARCH.dll" /pdbtype:sept
# ADD LINK32 esl.lib marcedit.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mono\BO_CAT_REG_SEARCH.dll" /pdbtype:sept
# Begin Target

# Name "BO_CAT_REG_SEARCH - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BO_CAT_DROP_INPUT.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_PROGRESS_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_SEARCH.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_SEARCH.def
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_SEARCH.rc
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_SEARCH1.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_SEARCH_0100.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_SEARCH_POPUP1.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BO_CAT_DROP_INPUT.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_PROGRESS_DLG.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_SEARCH1.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_SEARCH_0100.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_REG_SEARCH_POPUP1.h
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
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section BO_CAT_REG_SEARCH : {06D45234-C5D1-4CD3-8193-D7682738A1CF}
# 	1:27:IDD_BO_CAT_REG_PROGRESS_DLG:2006
# 	2:16:Resource Include:resource.h
# 	2:31:CLASS: CBO_CAT_REG_PROGRESS_DLG:CBO_CAT_REG_PROGRESS_DLG
# 	2:27:IDD_BO_CAT_REG_PROGRESS_DLG:IDD_BO_CAT_REG_PROGRESS_DLG
# 	2:10:ENUM: enum:enum
# 	2:25:BO_CAT_REG_PROGRESS_DLG.h:BO_CAT_REG_PROGRESS_DLG.h
# 	2:27:BO_CAT_REG_PROGRESS_DLG.cpp:BO_CAT_REG_PROGRESS_DLG.cpp
# 	2:19:Application Include:BO_CAT_REG_SEARCH.h
# End Section
# Section BO_CAT_REG_SEARCH : {A545D87F-6097-4079-9E15-83B4047C98F8}
# 	2:31:CLASS: CBO_CAT_REG_SEARCH_POPUP:CBO_CAT_REG_SEARCH_POPUP
# 	2:10:ENUM: enum:enum
# 	2:25:BO_CAT_REG_SEARCH_POPUP.h:BO_CAT_REG_SEARCH_POPUP1.h
# 	2:19:Application Include:BO_CAT_REG_SEARCH.h
# 	2:27:BO_CAT_REG_SEARCH_POPUP.cpp:BO_CAT_REG_SEARCH_POPUP1.cpp
# End Section
# Section BO_CAT_REG_SEARCH : {E9D4F98E-EA60-4F66-BCF1-81F5DFC7A2D6}
# 	1:21:IDD_BO_CAT_DROP_INPUT:2001
# 	2:16:Resource Include:resource.h
# 	2:21:IDD_BO_CAT_DROP_INPUT:IDD_BO_CAT_DROP_INPUT
# 	2:10:ENUM: enum:enum
# 	2:19:BO_CAT_DROP_INPUT.h:BO_CAT_DROP_INPUT1.h
# 	2:21:BO_CAT_DROP_INPUT.cpp:BO_CAT_DROP_INPUT1.cpp
# 	2:25:CLASS: CBO_CAT_DROP_INPUT:CBO_CAT_DROP_INPUT
# 	2:19:Application Include:BO_CAT_REG_SEARCH.h
# End Section
