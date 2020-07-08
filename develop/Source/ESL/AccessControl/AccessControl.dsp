# Microsoft Developer Studio Project File - Name="AccessControl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=AccessControl - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AccessControl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AccessControl.mak" CFG="AccessControl - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AccessControl - Win32 Unicode Debug" (based on "Win32 (x86) Application")
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
# PROP BASE Output_Dir "AccessControl___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "AccessControl___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AccessControl___Win32_Unicode_Debug"
# PROP Intermediate_Dir "AccessControl___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\bin\AccessControl.exe" /pdbtype:sept
# ADD LINK32 esl.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\bin\AccessControl.exe" /pdbtype:sept
# Begin Target

# Name "AccessControl - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AccessControl.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControl.rc
# End Source File
# Begin Source File

SOURCE=.\AccessControlChangeMethodDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlDeleteMethodDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlDeleteUserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlMenuInsertDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlNewMenuGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlNewMethodDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlSelectionListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlSelectionMenuListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlSelectionMenuModifyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlSmartIDMenuListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlSmartIDMenuListInsertDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlSmartIDMenuListModify.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlToolBarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlToolBarInsertDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlToolGroupInsertDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlUserDuplicate.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlUserSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlView.cpp
# End Source File
# Begin Source File

SOURCE=.\AccessControlWorkView.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\NewButton.cpp
# End Source File
# Begin Source File

SOURCE=.\NewButtonList.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\recordset.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UniqueMsgList.cpp
# End Source File
# Begin Source File

SOURCE=.\UserList.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccessControl.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlChangeMethodDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlDeleteMethodDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlDeleteUserDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlDoc.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlMenuInsertDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlNewMenuGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlNewMethodDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlSelectionListDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlSelectionMenuListDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlSelectionMenuModifyDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlSmartIDMenuListDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlSmartIDMenuListInsertDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlSmartIDMenuListModify.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlToolBarDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlToolBarInsertDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlToolGroupInsertDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlUserDuplicate.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlUserSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlView.h
# End Source File
# Begin Source File

SOURCE=.\AccessControlWorkView.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\NewButton.h
# End Source File
# Begin Source File

SOURCE=.\NewButtonList.h
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

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UniqueMsgList.h
# End Source File
# Begin Source File

SOURCE=.\UserList.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AccessControl.ico
# End Source File
# Begin Source File

SOURCE=.\res\AccessControl.rc2
# End Source File
# Begin Source File

SOURCE=.\res\AccessControlDoc.ico
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\CANCELD.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\CANCELU.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\CANCELX.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\CLOSED.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\CLOSEU.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\CLOSEX.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\DELETED.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\DELETEU.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\DELETEX.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\INSERTD.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\INSERTU.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\INSERTX.bmp
# End Source File
# Begin Source File

SOURCE=.\LOCKUP.ICO
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\OKD.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\OKU.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\OKX.bmp
# End Source File
# Begin Source File

SOURCE=.\icons\smile01.ico
# End Source File
# Begin Source File

SOURCE=.\icons\smile02.ico
# End Source File
# Begin Source File

SOURCE=.\icons\smile03.ico
# End Source File
# Begin Source File

SOURCE=.\icons\smile04.ico
# End Source File
# Begin Source File

SOURCE=.\icons\smile05.ico
# End Source File
# Begin Source File

SOURCE=.\icons\smile06.ico
# End Source File
# Begin Source File

SOURCE=.\icons\smile07.ico
# End Source File
# Begin Source File

SOURCE=.\icons\smile08.ico
# End Source File
# Begin Source File

SOURCE=.\icons\smile09.ico
# End Source File
# Begin Source File

SOURCE=.\icons\smile10.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\UPDATED.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\UPDATEU.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\UPDATEX.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section AccessControl : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section AccessControl : {0ECD9B62-23AA-11D0-B351-00A0C9055D8E}
# 	2:5:Class:CMSHFlexGrid
# 	2:10:HeaderFile:mshflexgrid.h
# 	2:8:ImplFile:mshflexgrid.cpp
# End Section
# Section AccessControl : {0000050E-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:CRecordset1
# 	2:10:HeaderFile:recordset.h
# 	2:8:ImplFile:recordset.cpp
# End Section
# Section AccessControl : {0ECD9B64-23AA-11D0-B351-00A0C9055D8E}
# 	2:21:DefaultSinkHeaderFile:mshflexgrid.h
# 	2:16:DefaultSinkClass:CMSHFlexGrid
# End Section
# Section AccessControl : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
