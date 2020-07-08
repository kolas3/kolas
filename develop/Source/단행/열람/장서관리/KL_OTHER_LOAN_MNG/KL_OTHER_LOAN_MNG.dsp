# Microsoft Developer Studio Project File - Name="KL_OTHER_LOAN_MNG" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=KL_OTHER_LOAN_MNG - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KL_OTHER_LOAN_MNG.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KL_OTHER_LOAN_MNG.mak" CFG="KL_OTHER_LOAN_MNG - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KL_OTHER_LOAN_MNG - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Output_Dir "..\..\..\..\..\lib"
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
# ADD LINK32 ESL.lib BL_LOC_LOAN.lib kl_api.lib LOAN_INFO_SHARE_MANAGER.lib LOC_CommonAPI.lib Kolas2up_MobileCommon.lib Check_Dup_Api.lib BO_ACQ_PRINT_MGR.lib SIReportMgr.lib SIReportData.lib CO_SMS.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\mono\KL_OTHER_LOAN_MNG.dll" /pdbtype:sept
# Begin Target

# Name "KL_OTHER_LOAN_MNG - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\KL_OTHER_APPENDIX_LIST.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_DEFAULT.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_EDIT_MEMO.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_MNG.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_MNG.def
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_MNG.rc
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_OFFER.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_REQUEST.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_SEND_REFUSE.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_USER_DETAILVIEW.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\KL_OTHER_APPENDIX_LIST.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_DEFAULT.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_DLG.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_EDIT_MEMO.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_OFFER.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_REQUEST.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_SEND_REFUSE.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_LOAN_USER_DETAILVIEW.h
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

SOURCE=.\res\KL_OTHER_LOAN_MNG.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
