# Microsoft Developer Studio Project File - Name="MNG_CLASS_SIGN_MANAGER" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MNG_CLASS_SIGN_MANAGER - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MNG_CLASS_SIGN_MANAGER.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MNG_CLASS_SIGN_MANAGER.mak" CFG="MNG_CLASS_SIGN_MANAGER - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MNG_CLASS_SIGN_MANAGER - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "MNG_CLASS_SIGN_MANAGER___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "MNG_CLASS_SIGN_MANAGER___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "MNG_CLASS_SIGN_MANAGER___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mng\MNG_CLASS_SIGN_MANAGER.dll" /pdbtype:sept
# ADD LINK32 esl.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mng\MNG_CLASS_SIGN_MANAGER.dll" /pdbtype:sept
# Begin Target

# Name "MNG_CLASS_SIGN_MANAGER - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CLASS_SIGN_MANAGER_01.cpp
# End Source File
# Begin Source File

SOURCE=.\CLASS_SIGN_MANAGER_02.cpp
# End Source File
# Begin Source File

SOURCE=.\CLASS_SIGN_MANAGER_03.cpp
# End Source File
# Begin Source File

SOURCE=.\CLASS_SIGN_MANAGER_04.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_CLASS_SIGN_MANAGER.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_CLASS_SIGN_MANAGER.def
# End Source File
# Begin Source File

SOURCE=.\MNG_CLASS_SIGN_MANAGER.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CLASS_SIGN_MANAGER_01.h
# End Source File
# Begin Source File

SOURCE=.\CLASS_SIGN_MANAGER_02.h
# End Source File
# Begin Source File

SOURCE=.\CLASS_SIGN_MANAGER_03.h
# End Source File
# Begin Source File

SOURCE=.\CLASS_SIGN_MANAGER_04.h
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

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MNG_CLASS_SIGN_MANAGER.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section MNG_CLASS_SIGN_MANAGER : {1E53B78D-9F0E-4E65-9367-5CFE1196DC5F}
# 	1:27:IDD_MNG_CLASS_SIGN_POPUP_01:25007
# 	2:16:Resource Include:resource.h
# 	2:25:MNG_CLASS_SIGN_POPUP_01.h:MNG_CLASS_SIGN_POPUP_01.h
# 	2:31:CLASS: CMNG_CLASS_SIGN_POPUP_01:CMNG_CLASS_SIGN_POPUP_01
# 	2:27:IDD_MNG_CLASS_SIGN_POPUP_01:IDD_MNG_CLASS_SIGN_POPUP_01
# 	2:27:MNG_CLASS_SIGN_POPUP_01.cpp:MNG_CLASS_SIGN_POPUP_01.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MNG_CLASS_SIGN_MANAGER.h
# End Section
