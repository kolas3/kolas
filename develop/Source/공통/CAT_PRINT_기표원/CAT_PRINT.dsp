# Microsoft Developer Studio Project File - Name="CAT_PRINT" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CAT_PRINT - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CAT_PRINT.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CAT_PRINT.mak" CFG="CAT_PRINT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CAT_PRINT - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CAT_PRINT - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CAT_PRINT - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "CAT_PRINT - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib/"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 esl.lib MarcAdjustApi.lib SIReportMgr.lib SIReportData.lib IndexToolKit.lib MarcEdit.lib K2UP_COMMON.LIB winspool.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\COMMON\CAT_PRINT.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CAT_PRINT - Win32 Release"
# Name "CAT_PRINT - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\barcod.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_SERIAL_LABEL_CONFIG.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_SERIAL_NORMAL_CONFIG.cpp
# End Source File
# Begin Source File

SOURCE=.\CAT_PRINT.cpp
# End Source File
# Begin Source File

SOURCE=.\CAT_PRINT.def
# End Source File
# Begin Source File

SOURCE=.\CAT_PRINT.rc
# End Source File
# Begin Source File

SOURCE=.\EquipPrintDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EquipPrintPropertyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RevisionPrintDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialLabelPreview.cpp
# End Source File
# Begin Source File

SOURCE=.\SIReportPrintObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SIReportPrintObjectMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\barcod.h
# End Source File
# Begin Source File

SOURCE=.\BO_CAT_SERIAL_LABEL_CONFIG.h
# End Source File
# Begin Source File

SOURCE=.\BO_SERIAL_NORMAL_CONFIG.h
# End Source File
# Begin Source File

SOURCE=.\EquipPrintDlg.h
# End Source File
# Begin Source File

SOURCE=.\EquipPrintPropertyDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RevisionPrintDlg.h
# End Source File
# Begin Source File

SOURCE=.\SerialLabelPreview.h
# End Source File
# Begin Source File

SOURCE=.\SIReportPrintObject.h
# End Source File
# Begin Source File

SOURCE=.\SIReportPrintObjectMgr.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\b10.bmp
# End Source File
# Begin Source File

SOURCE=.\b11.bmp
# End Source File
# Begin Source File

SOURCE=.\b12.bmp
# End Source File
# Begin Source File

SOURCE=.\b13.bmp
# End Source File
# Begin Source File

SOURCE=.\b14.bmp
# End Source File
# Begin Source File

SOURCE=.\b2.bmp
# End Source File
# Begin Source File

SOURCE=.\b3.bmp
# End Source File
# Begin Source File

SOURCE=.\b4.bmp
# End Source File
# Begin Source File

SOURCE=.\b5.bmp
# End Source File
# Begin Source File

SOURCE=.\b6.bmp
# End Source File
# Begin Source File

SOURCE=.\b7.bmp
# End Source File
# Begin Source File

SOURCE=.\b8.bmp
# End Source File
# Begin Source File

SOURCE=.\b9.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CAT_PRINT.rc2
# End Source File
# Begin Source File

SOURCE=.\Kolas2_32.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section CAT_PRINT : {F140EE70-6CF1-486A-BF76-72D4268AB724}
# 	2:10:barcod.cpp:barcod.cpp
# 	2:19:Application Include:CAT_PRINT.h
# 	2:8:barcod.h:barcod.h
# 	2:14:CLASS: CBarcod:CBarcod
# End Section
