# Microsoft Developer Studio Project File - Name="ContentsMgr_SE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ContentsMgr_SE - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ContentsMgr_SE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ContentsMgr_SE.mak" CFG="ContentsMgr_SE - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ContentsMgr_SE - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "ContentsMgr_SE___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "ContentsMgr_SE___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Lib"
# PROP Intermediate_Dir "ContentsMgr_SE___Win32_Unicode_Debug"
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
# ADD BASE LINK32 ESL.LIB TocEditor.lib K2UP_Common.LIB IndexToolKit.lib Kolas2up_CommonControl.lib SIReportMgr.lib SiReportData.lib MarcEdit.lib CAT_FIXED_FIELD.lib BO_CAT_PROC_KEYWORD.lib MarcToolkit.lib ContentsMgr_Editor.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mng\ContentsMgr_SE.dll" /pdbtype:sept
# ADD LINK32 ESL.LIB TocEditor.lib K2UP_Common.LIB IndexToolKit.lib Kolas2up_CommonControl.lib SIReportMgr.lib SiReportData.lib MarcEdit.lib CAT_UNION_FIXED_FIELD.lib BO_CAT_PROC_KEYWORD.lib MarcToolkit.lib ContentsMgr_Editor.lib CharEncodeMgr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mng\ContentsMgr_SE.dll" /pdbtype:sept
# Begin Target

# Name "ContentsMgr_SE - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CONTENTMGR_SE_INDEX_SEARCH.cpp
# End Source File
# Begin Source File

SOURCE=.\ContentsMgr_SE.cpp
# End Source File
# Begin Source File

SOURCE=.\ContentsMgr_SE.def
# End Source File
# Begin Source File

SOURCE=.\ContentsMgr_SE.rc
# End Source File
# Begin Source File

SOURCE=.\KisaMarcImport.cpp
# End Source File
# Begin Source File

SOURCE=.\MakeKisaMarc.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CONTENTS_01.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CONTENTS_EDIT.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CONTENTS_SEARCH.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CREATE_ARTICLE1.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CONTENTMGR_SE_INDEX_SEARCH.h
# End Source File
# Begin Source File

SOURCE=.\KisaMarcImport.h
# End Source File
# Begin Source File

SOURCE=.\MakeKisaMarc.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SE_CONTENTS_01.h
# End Source File
# Begin Source File

SOURCE=.\SE_CONTENTS_EDIT.h
# End Source File
# Begin Source File

SOURCE=.\SE_CONTENTS_SEARCH.h
# End Source File
# Begin Source File

SOURCE=.\SE_CREATE_ARTICLE1.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ContentsMgr_SE.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section ContentsMgr_SE : {25EF9ED8-A86D-4D92-B857-A1CF0A8F4AFE}
# 	1:18:IDD_KISAMARCIMPORT:8006
# 	2:16:Resource Include:resource.h
# 	2:18:IDD_KISAMARCIMPORT:IDD_KISAMARCIMPORT
# 	2:16:KisaMarcImport.h:KisaMarcImport.h
# 	2:10:ENUM: enum:enum
# 	2:22:CLASS: CKisaMarcImport:CKisaMarcImport
# 	2:18:KisaMarcImport.cpp:KisaMarcImport.cpp
# 	2:19:Application Include:contentsmgr_se.h
# End Section
# Section ContentsMgr_SE : {68DFF73E-10CD-4274-AD61-85BDB6090FBD}
# 	1:21:IDD_SE_CREATE_ARTICLE:8010
# 	2:16:Resource Include:resource.h
# 	2:19:SE_CREATE_ARTICLE.h:SE_CREATE_ARTICLE1.h
# 	2:25:CLASS: CSE_CREATE_ARTICLE:CSE_CREATE_ARTICLE
# 	2:10:ENUM: enum:enum
# 	2:21:IDD_SE_CREATE_ARTICLE:IDD_SE_CREATE_ARTICLE
# 	2:19:Application Include:contentsmgr_se.h
# 	2:21:SE_CREATE_ARTICLE.cpp:SE_CREATE_ARTICLE1.cpp
# End Section
