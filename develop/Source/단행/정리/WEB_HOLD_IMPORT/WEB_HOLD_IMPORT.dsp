# Microsoft Developer Studio Project File - Name="WEB_HOLD_IMPORT" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=WEB_HOLD_IMPORT - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WEB_HOLD_IMPORT.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WEB_HOLD_IMPORT.mak" CFG="WEB_HOLD_IMPORT - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WEB_HOLD_IMPORT - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "WEB_HOLD_IMPORT___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "WEB_HOLD_IMPORT___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "WEB_HOLD_IMPORT___Win32_Unicode_Debug"
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
# ADD BASE LINK32 esl.lib MarcAdjustApi.lib MarcToolkit.lib MarcEdit.lib IndexToolKit.lib MarcAdjustApi.lib K2UP_Common.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\MONO\WEB_HOLD_IMPORT.dll" /pdbtype:sept
# ADD LINK32 esl.lib MarcAdjustApi.lib MarcToolkit.lib MarcEdit.lib IndexToolKit.lib MarcAdjustApi.lib K2UP_Common.lib CharEncodeMgr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\MONO\WEB_HOLD_IMPORT.dll" /pdbtype:sept
# Begin Target

# Name "WEB_HOLD_IMPORT - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WEB_HOLD_IMPORT.cpp
# End Source File
# Begin Source File

SOURCE=.\WEB_HOLD_IMPORT.def
# End Source File
# Begin Source File

SOURCE=.\WEB_HOLD_IMPORT.rc
# End Source File
# Begin Source File

SOURCE=.\WebHoldImport.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WebHoldImport.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\WEB_HOLD_IMPORT.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section WEB_HOLD_IMPORT : {FB811FAB-353B-4FF5-822B-2DFF35433883}
# 	1:26:IDD_BO_CAT_AUTO_REGNO_GIVE:12006
# 	2:16:Resource Include:resource.h
# 	2:24:BO_CAT_AUTO_REGNO_GIVE.h:BO_CAT_AUTO_REGNO_GIVE.h
# 	2:10:ENUM: enum:enum
# 	2:26:IDD_BO_CAT_AUTO_REGNO_GIVE:IDD_BO_CAT_AUTO_REGNO_GIVE
# 	2:26:BO_CAT_AUTO_REGNO_GIVE.cpp:BO_CAT_AUTO_REGNO_GIVE.cpp
# 	2:19:Application Include:WEB_HOLD_IMPORT.h
# 	2:30:CLASS: CBO_CAT_AUTO_REGNO_GIVE:CBO_CAT_AUTO_REGNO_GIVE
# End Section
