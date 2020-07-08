# Microsoft Developer Studio Project File - Name="MNG_KDCDDC_CLASS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MNG_KDCDDC_CLASS - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MNG_KDCDDC_CLASS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MNG_KDCDDC_CLASS.mak" CFG="MNG_KDCDDC_CLASS - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MNG_KDCDDC_CLASS - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "MNG_KDCDDC_CLASS___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "MNG_KDCDDC_CLASS___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "MNG_KDCDDC_CLASS___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\MNG\MNG_KDCDDC_CLASS.dll" /pdbtype:sept
# ADD LINK32 esl.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\MNG\MNG_KDCDDC_CLASS.dll" /pdbtype:sept
# Begin Target

# Name "MNG_KDCDDC_CLASS - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MNG_INDEX_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_KDCDDC_CLASS.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_KDCDDC_CLASS.def
# End Source File
# Begin Source File

SOURCE=.\MNG_KDCDDC_CLASS.rc
# End Source File
# Begin Source File

SOURCE=.\MNG_KDCDDC_HELP_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_KDCDDC_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_KDCDDC_SEARCH.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_NEW_HELP.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_NEW_INDEX.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_NEW_KDCDDC.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_NEW_KEYWORD.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_NEW_RELATED.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MNG_INDEX_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\MNG_KDCDDC_HELP_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\MNG_KDCDDC_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\MNG_KDCDDC_SEARCH.h
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\MNG_NEW_HELP.h
# End Source File
# Begin Source File

SOURCE=.\MNG_NEW_INDEX.h
# End Source File
# Begin Source File

SOURCE=.\MNG_NEW_KDCDDC.h
# End Source File
# Begin Source File

SOURCE=.\MNG_NEW_KEYWORD.h
# End Source File
# Begin Source File

SOURCE=.\MNG_NEW_RELATED.h
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

SOURCE=.\res\MNG_KDCDDC_CLASS.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section MNG_KDCDDC_CLASS : {FF266DEA-9E08-4FD2-A715-DB0973FBF946}
# 	1:18:IDD_KDCDDC_SEARCH1:18008
# 	2:16:Resource Include:resource.h
# 	2:25:CLASS: CMNG_KDCDDC_SEARCH:CMNG_KDCDDC_SEARCH
# 	2:10:ENUM: enum:enum
# 	2:21:MNG_KDCDDC_SEARCH.cpp:MNG_KDCDDC_SEARCH.cpp
# 	2:17:IDD_KDCDDC_SEARCH:IDD_KDCDDC_SEARCH1
# 	2:19:MNG_KDCDDC_SEARCH.h:MNG_KDCDDC_SEARCH.h
# 	2:19:Application Include:mng_kdcddc_class.h
# End Section
