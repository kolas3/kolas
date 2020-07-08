# Microsoft Developer Studio Project File - Name="MNG_KEYWORD_MANAGER" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MNG_KEYWORD_MANAGER - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MNG_KEYWORD_MANAGER.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MNG_KEYWORD_MANAGER.mak" CFG="MNG_KEYWORD_MANAGER - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MNG_KEYWORD_MANAGER - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "MNG_KEYWORD_MANAGER___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "MNG_KEYWORD_MANAGER___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "MNG_KEYWORD_MANAGER___Win32_Unicode_Debug"
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
# ADD BASE LINK32 ESL.lib BO_ACQ_EVIRONMENT_MANAGER.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\MNG\MNG_KEYWORD_MANAGER.dll" /pdbtype:sept
# ADD LINK32 ESL.lib BO_ACQ_EVIRONMENT_MANAGER.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\MNG\MNG_KEYWORD_MANAGER.dll" /pdbtype:sept
# Begin Target

# Name "MNG_KEYWORD_MANAGER - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\EnvironComboBox_MultiState.cpp
# End Source File
# Begin Source File

SOURCE="..\..\분류기호및주제명관리\MNG_KDCDDC_CLASS\MNG_KDCDDC_SEARCH.cpp"
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_DETAIL_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_DUP_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_MAIN_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_MAIN_POPUP.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_MAKE6XXTAG.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_MANAGER.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_MANAGER.def
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_MANAGER.rc
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_SEARCH_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\EnvironComboBox_MultiState.h
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD.h
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_DETAIL_DLG.h
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_DUP_DLG.h
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_MAIN_DLG.h
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_MAIN_POPUP.h
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_MAKE6XXTAG.h
# End Source File
# Begin Source File

SOURCE=.\MNG_KEYWORD_SEARCH_DLG.h
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

SOURCE=.\res\MNG_KEYWORD_MANAGER.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section MNG_KEYWORD_MANAGER : {8E2B6CC8-6286-4A74-A9AB-0A1436B01382}
# 	2:30:EnvironComboBox_MultiState.cpp:EnvironComboBox_MultiState.cpp
# 	2:34:CLASS: CEnvironComboBox_MultiState:CEnvironComboBox_MultiState
# 	2:19:Application Include:mng_keyword_manager.h
# 	2:28:EnvironComboBox_MultiState.h:EnvironComboBox_MultiState.h
# End Section
