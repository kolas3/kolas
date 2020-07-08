# Microsoft Developer Studio Project File - Name="MNG_WORK_SEARCH" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MNG_WORK_SEARCH - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MNG_WORK_SEARCH.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MNG_WORK_SEARCH.mak" CFG="MNG_WORK_SEARCH - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MNG_WORK_SEARCH - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Output_Dir "..\..\..\..\lib"
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
# ADD LINK32 esl.lib LOC_CommonAPI.lib CallNoPrintManager.lib CharEncodeMgr.lib Imm32.lib Ws2_32.lib indextoolkit.lib MarcEdit.lib Kolas2up_MobileCommon.lib KL_Api.lib KOMarcGetter.lib COMMON_CONVERSION_MGR.lib LOAN_INFO_SHARE_MANAGER.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\mng\MNG_WORK_DATA_SEARCH.dll" /pdbtype:sept
# Begin Target

# Name "MNG_WORK_SEARCH - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CommonProc.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl_MarkStaticBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl_StrongStaticBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_WORK_SEARCH.cpp
# End Source File
# Begin Source File

SOURCE=.\MNG_WORK_SEARCH.def
# End Source File
# Begin Source File

SOURCE=.\MNG_WORK_SEARCH.rc
# End Source File
# Begin Source File

SOURCE=.\PictureEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UserMainBookDetailSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserMainBookSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserMainDetailDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserMainMarcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserMainWait.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CommonProc.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl_MarkStaticBox.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl_StrongStaticBox.h
# End Source File
# Begin Source File

SOURCE=.\PictureEx.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UserMainBookDetailSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserMainBookSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserMainDetailDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserMainMarcDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserMainWait.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ItemMark.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MNG_WORK_SEARCH.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
