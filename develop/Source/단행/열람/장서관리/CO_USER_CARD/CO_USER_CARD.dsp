# Microsoft Developer Studio Project File - Name="CO_USER_CARD" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CO_USER_CARD - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CO_USER_CARD.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CO_USER_CARD.mak" CFG="CO_USER_CARD - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CO_USER_CARD - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "CO_USER_CARD___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "CO_USER_CARD___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\lib"
# PROP Intermediate_Dir "CO_USER_CARD___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib SIReportData.lib Kolas2up_ImageEditor.lib PaintLib.lib libjpeg.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\Common\CO_USER_CARD.dll" /pdbtype:sept
# ADD LINK32 esl.lib SIReportData.lib Kolas2up_ImageEditor.lib PaintLib.lib libjpeg.lib CharEncodeMgr.lib KL_API.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib BL_LOC_USER.lib BO_LOC_3000.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\Common\CO_USER_CARD.dll" /pdbtype:sept
# Begin Target

# Name "CO_USER_CARD - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BackGround.cpp
# End Source File
# Begin Source File

SOURCE=.\BarCode.cpp
# End Source File
# Begin Source File

SOURCE=.\BitMap.cpp
# End Source File
# Begin Source File

SOURCE=.\CardPrintDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CO_USER_CARD.cpp
# End Source File
# Begin Source File

SOURCE=.\CO_USER_CARD.def
# End Source File
# Begin Source File

SOURCE=.\CO_USER_CARD.rc
# End Source File
# Begin Source File

SOURCE=.\ControlObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlObjectMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Form.cpp
# End Source File
# Begin Source File

SOURCE=.\Line.cpp
# End Source File
# Begin Source File

SOURCE=.\Preview.cpp
# End Source File
# Begin Source File

SOURCE=.\PreViewCardPrint.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdioFileEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TextBox.cpp
# End Source File
# Begin Source File

SOURCE=.\UserCardSetting.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BackGround.h
# End Source File
# Begin Source File

SOURCE=.\BarCode.h
# End Source File
# Begin Source File

SOURCE=.\BitMap.h
# End Source File
# Begin Source File

SOURCE=.\CardPrintDlg.h
# End Source File
# Begin Source File

SOURCE=.\ControlObject.h
# End Source File
# Begin Source File

SOURCE=.\ControlObjectMgr.h
# End Source File
# Begin Source File

SOURCE=.\Form.h
# End Source File
# Begin Source File

SOURCE=.\Line.h
# End Source File
# Begin Source File

SOURCE=.\Preview.h
# End Source File
# Begin Source File

SOURCE=.\PreViewCardPrint.h
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
# Begin Source File

SOURCE=.\TextBox.h
# End Source File
# Begin Source File

SOURCE=.\UserCardSetting.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CO_USER_CARD.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
