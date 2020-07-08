# Microsoft Developer Studio Project File - Name="SPF" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SPF - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SPF.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SPF.mak" CFG="SPF - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SPF - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SPF - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SPF - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SPF - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ESL.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\bin\SPF.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SPF - Win32 Release"
# Name "SPF - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CoolDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupInsert.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\OleMListDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\PathInsert.cpp
# End Source File
# Begin Source File

SOURCE=.\PathObject1.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SGroupObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SPathObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SPF.cpp
# End Source File
# Begin Source File

SOURCE=.\SPF.rc
# End Source File
# Begin Source File

SOURCE=.\SPF_NEW_PROJECT.cpp
# End Source File
# Begin Source File

SOURCE=.\SPFButtonInsert.cpp
# End Source File
# Begin Source File

SOURCE=.\SPFDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SPFInsertTask.cpp
# End Source File
# Begin Source File

SOURCE=.\SPFMenuInsert.cpp
# End Source File
# Begin Source File

SOURCE=.\SPFObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SPFOpenProject.cpp
# End Source File
# Begin Source File

SOURCE=.\SPFView.cpp
# End Source File
# Begin Source File

SOURCE=.\SStateObject.cpp
# End Source File
# Begin Source File

SOURCE=.\StateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StateInsert.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToolDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CoolDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\GroupInsert.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\mshflexgrid.h
# End Source File
# Begin Source File

SOURCE=.\OleMListDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\PathInsert.h
# End Source File
# Begin Source File

SOURCE=.\PathObject1.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.h
# End Source File
# Begin Source File

SOURCE=.\recordset.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SGroupObject.h
# End Source File
# Begin Source File

SOURCE=.\SObject.h
# End Source File
# Begin Source File

SOURCE=.\SPathObject.h
# End Source File
# Begin Source File

SOURCE=.\SPF.h
# End Source File
# Begin Source File

SOURCE=.\SPF_NEW_PROJECT.h
# End Source File
# Begin Source File

SOURCE=.\SPFButtonInsert.h
# End Source File
# Begin Source File

SOURCE=.\SPFDoc.h
# End Source File
# Begin Source File

SOURCE=.\SPFInsertTask.h
# End Source File
# Begin Source File

SOURCE=.\SPFMenuInsert.h
# End Source File
# Begin Source File

SOURCE=.\SPFObject.h
# End Source File
# Begin Source File

SOURCE=.\SPFOpenProject.h
# End Source File
# Begin Source File

SOURCE=.\SPFView.h
# End Source File
# Begin Source File

SOURCE=.\SStateObject.h
# End Source File
# Begin Source File

SOURCE=.\StateDlg.h
# End Source File
# Begin Source File

SOURCE=.\StateInsert.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\list_sta.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SPF.ico
# End Source File
# Begin Source File

SOURCE=.\res\SPF.rc2
# End Source File
# Begin Source File

SOURCE=.\res\SPFDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section SPF : {0ECD9B62-23AA-11D0-B351-00A0C9055D8E}
# 	2:5:Class:CMSHFlexGrid
# 	2:10:HeaderFile:mshflexgrid.h
# 	2:8:ImplFile:mshflexgrid.cpp
# End Section
# Section SPF : {0000050E-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:CRecordset1
# 	2:10:HeaderFile:recordset.h
# 	2:8:ImplFile:recordset.cpp
# End Section
# Section SPF : {0ECD9B64-23AA-11D0-B351-00A0C9055D8E}
# 	2:21:DefaultSinkHeaderFile:mshflexgrid.h
# 	2:16:DefaultSinkClass:CMSHFlexGrid
# End Section
# Section SPF : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section SPF : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
