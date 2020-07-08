# Microsoft Developer Studio Project File - Name="ContentsMgr_Editor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ContentsMgr_Editor - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ContentsMgr_Editor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ContentsMgr_Editor.mak" CFG="ContentsMgr_Editor - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ContentsMgr_Editor - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "ContentsMgr_Editor___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "ContentsMgr_Editor___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Lib"
# PROP Intermediate_Dir "ContentsMgr_Editor___Win32_Unicode_Debug"
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
# ADD BASE LINK32 ESL.LIB TocEditor.lib K2UP_Common.LIB IndexToolKit.lib light_gatherdatasearch.lib SearchMgr.lib rsmgr.lib Kolas2up_CommonControl.lib ContentsMgr_SE.lib MarcToolkit.lib MarcEdit.lib BO_CAT_PROC_KEYWORD.lib CAT_FIXED_FIELD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mng\ContentsMgr_Editor.dll" /pdbtype:sept
# ADD LINK32 ESL.LIB TocEditor.lib K2UP_Common.LIB IndexToolKit.lib light_gatherdatasearch.lib Kolas2up_CommonControl.lib ContentsMgr_SE.lib MarcToolkit.lib MarcEdit.lib BO_CAT_PROC_KEYWORD.lib CAT_UNION_FIXED_FIELD.lib CharEncodeMgr.lib CAT_DCM_MANAGER.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mng\ContentsMgr_Editor.dll" /pdbtype:sept
# Begin Target

# Name "ContentsMgr_Editor - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CONTENTMGR_BO_EDIT.cpp
# End Source File
# Begin Source File

SOURCE=.\CONTENTMGR_SE_ARTICLE.cpp
# End Source File
# Begin Source File

SOURCE=.\CONTENTMGR_SE_EDIT.cpp
# End Source File
# Begin Source File

SOURCE=.\CONTENTMGR_SE_KISA_EDIT.cpp
# End Source File
# Begin Source File

SOURCE=.\ContentsMgr_Editor.cpp
# End Source File
# Begin Source File

SOURCE=.\ContentsMgr_Editor.def
# End Source File
# Begin Source File

SOURCE=.\ContentsMgr_Editor.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CONTENTMGR_BO_EDIT.h
# End Source File
# Begin Source File

SOURCE=.\CONTENTMGR_SE_ARTICLE.h
# End Source File
# Begin Source File

SOURCE=.\CONTENTMGR_SE_EDIT.h
# End Source File
# Begin Source File

SOURCE=.\CONTENTMGR_SE_KISA_EDIT.h
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

SOURCE=.\res\ContentsMgr_Editor.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
