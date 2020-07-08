# Microsoft Developer Studio Project File - Name="MarcEdit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MarcEdit - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MarcEdit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MarcEdit.mak" CFG="MarcEdit - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MarcEdit - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MarcEdit___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "MarcEdit___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\lib"
# PROP Intermediate_Dir "MarcEdit___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /D "_DEBUG" /D "DEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ESL.lib CharEncodeMgr.lib HTMLHELP.LIB /nologo /subsystem:windows /dll /incremental:yes /debug /machine:I386 /out:"..\..\..\..\..\dll\common\MarcEdit.dll"
# Begin Target

# Name "MarcEdit - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ColorListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMarcSyntaxError.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSymbolColor.cpp
# End Source File
# Begin Source File

SOURCE=.\ECO_MarcAutoList.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\GridEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MARC_ANALYZER.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcEdit.def
# End Source File
# Begin Source File

SOURCE=.\MarcEdit.rc
# End Source File
# Begin Source File

SOURCE=.\MarcEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\mshflexgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ColorListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ColorPicker.h
# End Source File
# Begin Source File

SOURCE=.\ColorPopup.h
# End Source File
# Begin Source File

SOURCE=.\DlgMarcSyntaxError.h
# End Source File
# Begin Source File

SOURCE=.\DlgSymbolColor.h
# End Source File
# Begin Source File

SOURCE=.\ECO_MarcAutoList.h
# End Source File
# Begin Source File

SOURCE=.\GridCombo.h
# End Source File
# Begin Source File

SOURCE=.\GridEdit.h
# End Source File
# Begin Source File

SOURCE=.\MARC_ANALYZER.h
# End Source File
# Begin Source File

SOURCE=.\MarcEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MarcEditor.h
# End Source File
# Begin Source File

SOURCE=.\MarcGridCtrl.h
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

SOURCE=.\res\linkhand.cur
# End Source File
# Begin Source File

SOURCE=.\res\MarcEdit.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
