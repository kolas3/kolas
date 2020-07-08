# Microsoft Developer Studio Project File - Name="SIReportEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SIReportEditor - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SIReportEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SIReportEditor.mak" CFG="SIReportEditor - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SIReportEditor - Win32 Unicode Debug" (based on "Win32 (x86) Application")
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
# PROP BASE Output_Dir "SIReportEditor___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "SIReportEditor___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "SIReportEditor___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SIReportData.lib esl.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\bin\SIReportEditor.exe" /pdbtype:sept
# ADD LINK32 SIReportData.lib esl.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\bin\SIReportEditor.exe" /pdbtype:sept
# Begin Target

# Name "SIReportEditor - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ColourPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgColumnChange.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgControlBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDefaultDMSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPageSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRptFileManage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRptFileManageModify.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRptSave.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSheetView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTab.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTableProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MSheetDesigner.cpp
# End Source File
# Begin Source File

SOURCE=.\OleMListDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\OleMSheetDesignerDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SIReportEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\SIReportEditor.rc
# End Source File
# Begin Source File

SOURCE=.\SIReportEditorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SIReportEditorView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ColourPicker.h
# End Source File
# Begin Source File

SOURCE=.\ColourPopup.h
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\DlgColumnChange.h
# End Source File
# Begin Source File

SOURCE=.\DlgControlBox.h
# End Source File
# Begin Source File

SOURCE=.\DlgDefaultDMSetup.h
# End Source File
# Begin Source File

SOURCE=.\DlgPageSetup.h
# End Source File
# Begin Source File

SOURCE=.\DlgProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgRptFileManage.h
# End Source File
# Begin Source File

SOURCE=.\DlgRptFileManageModify.h
# End Source File
# Begin Source File

SOURCE=.\DlgRptSave.h
# End Source File
# Begin Source File

SOURCE=.\DlgSheetView.h
# End Source File
# Begin Source File

SOURCE=.\DlgTab.h
# End Source File
# Begin Source File

SOURCE=.\DlgTableProperty.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MSheetDesigner.h
# End Source File
# Begin Source File

SOURCE=.\OleMListDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\OleMSheetDesignerDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\PropertyListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SIReportEditor.h
# End Source File
# Begin Source File

SOURCE=.\SIReportEditorDoc.h
# End Source File
# Begin Source File

SOURCE=.\SIReportEditorView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\control1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\Icon16.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Icon32.ICO
# End Source File
# Begin Source File

SOURCE=.\res\SIReportEditor.ico
# End Source File
# Begin Source File

SOURCE=.\res\SIReportEditor.rc2
# End Source File
# Begin Source File

SOURCE=.\res\SIReportEditorDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
