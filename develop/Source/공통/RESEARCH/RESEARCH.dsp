# Microsoft Developer Studio Project File - Name="RESEARCH" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RESEARCH - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RESEARCH.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RESEARCH.mak" CFG="RESEARCH - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RESEARCH - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "RESEARCH___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "RESEARCH___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RESEARCH___Win32_Unicode_Debug"
# PROP Intermediate_Dir "RESEARCH___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib MSCharMgr.lib wininet.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\mono\RESEARCH.dll" /pdbtype:sept
# ADD LINK32 esl.lib MSCharMgr.lib wininet.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\mono\RESEARCH.dll" /pdbtype:sept
# Begin Target

# Name "RESEARCH - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ECO_HtmlViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\RESEARCH.cpp
# End Source File
# Begin Source File

SOURCE=.\RESEARCH.def
# End Source File
# Begin Source File

SOURCE=.\RESEARCH.rc
# End Source File
# Begin Source File

SOURCE=.\RESEARCH_DLG_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\RESEARCH_Mgr.cpp
# End Source File
# Begin Source File

SOURCE=.\RESEARCH_NEWMAKE_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\RESEARCH_PREVIEW_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\RESEARCH_RESULT_DESCTYPE.cpp
# End Source File
# Begin Source File

SOURCE=.\RESEARCH_RESULT_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ECO_HtmlViewer.h
# End Source File
# Begin Source File

SOURCE=.\RESEARCH_DLG_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\RESEARCH_Mgr.h
# End Source File
# Begin Source File

SOURCE=.\RESEARCH_NEWMAKE_DLG.h
# End Source File
# Begin Source File

SOURCE=.\RESEARCH_PREVIEW_DLG.h
# End Source File
# Begin Source File

SOURCE=.\RESEARCH_RESULT_DESCTYPE.h
# End Source File
# Begin Source File

SOURCE=.\RESEARCH_RESULT_DLG.h
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

SOURCE=.\res\RESEARCH.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section RESEARCH : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
# Section RESEARCH : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
