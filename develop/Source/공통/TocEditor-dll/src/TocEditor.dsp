# Microsoft Developer Studio Project File - Name="TocEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TocEditor - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TocEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TocEditor.mak" CFG="TocEditor - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TocEditor - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "TocEditor___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "TocEditor___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "TocEditor___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /I "./" /I "../../../../include" /I "../../../../include/TocEditor" /I "./TocSyntaxCheck" /I "../../../../include/Math" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /I "./" /I "../../../../include" /I "../../../../include/TocEditor" /I "./TocSyntaxCheck" /I "../../../../include/Math" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TocSyntaxCheck.lib EcoMathViewerDLL.lib esl.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\\DLL\COMMON\TocEditor.dll" /pdbtype:sept /libpath:"..\..\..\..\..\develop\Lib"
# ADD LINK32 TocSyntaxCheck.lib EcoMathViewerDLL.lib esl.lib CharEncodeMgr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\\DLL\COMMON\TocEditor.dll" /pdbtype:sept /libpath:"..\..\..\..\..\develop\Lib"
# Begin Target

# Name "TocEditor - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BibilographyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\bittooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\BodyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChorokDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChorokEditorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChorokRichEditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageDataObject.cpp
# End Source File
# Begin Source File

SOURCE=.\LineRichEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MathConvertor.cpp
# End Source File
# Begin Source File

SOURCE=.\Mathlist.cpp
# End Source File
# Begin Source File

SOURCE=.\MyListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\SoosikEDitor.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SyntaxCheckDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TocEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\TocEditor.def
# End Source File
# Begin Source File

SOURCE=.\TocEditor.rc
# End Source File
# Begin Source File

SOURCE=.\TocEditorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TocParse.cpp
# End Source File
# Begin Source File

SOURCE=.\TocViewerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\YonChorokSet.cpp
# End Source File
# Begin Source File

SOURCE=.\YonSujiDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BibilographyDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\TocEditor\bittooltip.h
# End Source File
# Begin Source File

SOURCE=.\BodyDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\TocEditor\ChorokDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImageDataObject.h
# End Source File
# Begin Source File

SOURCE=.\MathConvertor.h
# End Source File
# Begin Source File

SOURCE=.\Mathlist.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\TocEditor\RichEditCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\TocEditor\SyntaxCheckDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\TocEditor\TocEditorDef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\TocEditor\TocEditorDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\TocEditor\TocViewerDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\hand.ico
# End Source File
# Begin Source File

SOURCE=.\res\TocEditor.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
