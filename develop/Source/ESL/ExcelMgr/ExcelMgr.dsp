# Microsoft Developer Studio Project File - Name="ExcelMgr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ExcelMgr - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ExcelMgr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ExcelMgr.mak" CFG="ExcelMgr - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ExcelMgr - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "Unicode Debug"
# PROP BASE Intermediate_Dir "Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\LIB"
# PROP Intermediate_Dir "Unicode Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\DLL\COMMON\ExcelMgr.dll" /pdbtype:sept
# Begin Target

# Name "ExcelMgr - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\excel8.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcelMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcelMgr.def
# End Source File
# Begin Source File

SOURCE=.\ExcelMgr.odl
# End Source File
# Begin Source File

SOURCE=.\ExcelMgr.rc
# End Source File
# Begin Source File

SOURCE=.\ExportToCSV.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportToExcel.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportToHCell.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdioFileEx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "hcell"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CApplication.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CAreas.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CBorder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CBorders.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CComment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CComments.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CFont0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CName.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CNames.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\Cnterior.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CPageSetup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CRange.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CSheets.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CWindow0.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CWindows.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CWorkbook.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CWorkbooks.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CWorksheet.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\hcell\CWorksheets.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\include\Common\excel8.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\ExportToCSV.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\ExportToExcel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\ExportToHCell.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Common\HCellInclude.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StdioFileEx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ExcelMgr.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
