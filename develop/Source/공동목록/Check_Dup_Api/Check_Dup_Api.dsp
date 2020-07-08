# Microsoft Developer Studio Project File - Name="Check_Dup_Api" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Check_Dup_Api - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Check_Dup_Api.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Check_Dup_Api.mak" CFG="Check_Dup_Api - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Check_Dup_Api - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "Check_Dup_Api___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "Check_Dup_Api___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\LIB"
# PROP Intermediate_Dir "Check_Dup_Api___Win32_Unicode_Debug"
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
# ADD BASE LINK32 ESL.lib ocimgr.lib marcedit.lib RSMgr.lib SearchMgr.lib IndexToolKit.lib K2UP_Common.lib rs_common.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\UC\Check_Dup_Api.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ESL.lib ocimgr.lib marcedit.lib IndexToolKit.lib K2UP_Common.lib CharEncodeMgr.lib KolisNet_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\UC\Check_Dup_Api.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "Check_Dup_Api - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Bo_MarcImport_Tool.cpp
# End Source File
# Begin Source File

SOURCE=.\check_dup.cpp
# End Source File
# Begin Source File

SOURCE=.\Check_Dup_Api.cpp
# End Source File
# Begin Source File

SOURCE=.\Check_Dup_Api.def
# End Source File
# Begin Source File

SOURCE=.\Check_Dup_Api.rc
# End Source File
# Begin Source File

SOURCE=.\CU_ComApi.cpp
# End Source File
# Begin Source File

SOURCE=.\Get_Duplicate_Key.cpp
# End Source File
# Begin Source File

SOURCE=.\LOCAL_UPDATE.cpp
# End Source File
# Begin Source File

SOURCE=.\MarcConversion.cpp
# End Source File
# Begin Source File

SOURCE=.\PlusIN049Tag.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RS_SQL.cpp
# End Source File
# Begin Source File

SOURCE=.\SE_CENTERUPLOAD_API.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchMake.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ViewUploadMarc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\include\ESL\BO_MARCIMPORT_TOOL.h
# End Source File
# Begin Source File

SOURCE=.\Check_Dup.h
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\CU_ComApi.h"
# End Source File
# Begin Source File

SOURCE=.\Get_Duplicate_Key.h
# End Source File
# Begin Source File

SOURCE=.\LOCAL_UPDATE.h
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\MarcConversion.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\PlusIN049Tag.h
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RS_SQL.h
# End Source File
# Begin Source File

SOURCE=.\SE_CENTERUPLOAD_API.h
# End Source File
# Begin Source File

SOURCE=.\SearchMake.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ViewUploadMarc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Check_Dup_Api.rc2
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
