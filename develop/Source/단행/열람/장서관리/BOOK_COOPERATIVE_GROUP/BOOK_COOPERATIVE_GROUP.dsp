# Microsoft Developer Studio Project File - Name="BOOK_COOPERATIVE_GROUP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BOOK_COOPERATIVE_GROUP - Win32 RFID Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BOOK_COOPERATIVE_GROUP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BOOK_COOPERATIVE_GROUP.mak" CFG="BOOK_COOPERATIVE_GROUP - Win32 RFID Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BOOK_COOPERATIVE_GROUP - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BOOK_COOPERATIVE_GROUP - Win32 RFID Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BOOK_COOPERATIVE_GROUP - Win32 Unicode Debug"

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
# ADD LINK32 ESL.LIB Imm32.lib LOC_CommonAPI.lib CLocMailMgr.lib MailMgr.lib kolas2up_mobilecommon.lib k2up_common.lib Check_Dup_Api.lib LOAN_INFO_SHARE_MANAGER.lib CSMSContentsEditor.lib CXroshotManager.lib CO_SMS.lib charencodemgr.lib BL_LOC_LOAN.lib CButtonSTEX.lib kl_api.lib SIReportMgr.lib SIReportData.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\Mono\BOOK_COOPERATIVE_GROUP.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "BOOK_COOPERATIVE_GROUP - Win32 RFID Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BOOK_COOPERATIVE_GROUP___Win32_RFID_Unicode_Debug"
# PROP BASE Intermediate_Dir "BOOK_COOPERATIVE_GROUP___Win32_RFID_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "BOOK_COOPERATIVE_GROUP___Win32_RFID_Unicode_Debug"
# PROP Intermediate_Dir "BOOK_COOPERATIVE_GROUP___Win32_RFID_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ESL.LIB Imm32.lib LOC_CommonAPI.lib CLocMailMgr.lib MailMgr.lib kolas2up_mobilecommon.lib k2up_common.lib Check_Dup_Api.lib LOAN_INFO_SHARE_MANAGER.lib CSMSContentsEditor.lib CXroshotManager.lib CO_SMS.lib charencodemgr.lib BL_LOC_LOAN.lib CButtonSTEX.lib RFID_JOB.lib KOLAS1004C.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\Mono\BOOK_COOPERATIVE_GROUP.dll" /pdbtype:sept
# ADD LINK32 ESL.LIB Imm32.lib LOC_CommonAPI.lib CLocMailMgr.lib MailMgr.lib kolas2up_mobilecommon.lib k2up_common.lib Check_Dup_Api.lib LOAN_INFO_SHARE_MANAGER.lib CSMSContentsEditor.lib CXroshotManager.lib CO_SMS.lib charencodemgr.lib BL_LOC_LOAN.lib CButtonSTEX.lib RFID_JOB.lib KL_Api.lib SIReportData.lib SIReportMgr.lib BO_LOC_3000.lib SIReportMgr.lib SIReportData.lib /nologo /subsystem:windows /dll /pdb:"..\..\..\..\..\lib/BOOK_COOPERATIVE_GROUP.pdb" /debug /machine:I386 /out:"..\..\..\..\..\..\dll\Mono\BOOK_COOPERATIVE_GROUP.dll" /implib:"..\..\..\..\..\lib/BOOK_COOPERATIVE_GROUP.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "BOOK_COOPERATIVE_GROUP - Win32 Unicode Debug"
# Name "BOOK_COOPERATIVE_GROUP - Win32 RFID Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BOOK_COOPERATIVE_GROUP.cpp
# End Source File
# Begin Source File

SOURCE=.\BOOK_COOPERATIVE_GROUP.def
# End Source File
# Begin Source File

SOURCE=.\BOOK_COOPERATIVE_GROUP.rc
# End Source File
# Begin Source File

SOURCE=.\BOOK_COOPERATIVE_GROUP_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\BOOK_COOPERATIVE_GROUP_OFFER.cpp
# End Source File
# Begin Source File

SOURCE=.\BOOK_COOPERATIVE_GROUP_REQUEST.cpp
# End Source File
# Begin Source File

SOURCE=.\BookCooperativeGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmb_GridItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Data.cpp
# End Source File
# Begin Source File

SOURCE=.\Edit_GridItem.cpp
# End Source File
# Begin Source File

SOURCE=.\HourEditControl.cpp
# End Source File
# Begin Source File

SOURCE=.\HourEditElem.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_API_APPENDIX_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_APL.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_APLWEB.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_CHANGE_REGNO.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_COMPARE.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_MODIFY.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_PERMISSION_LIST.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_RFID_SECURITY_CHANGE.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_RFID_SECURITY_CHANGE2.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH_BOOK.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH_BOOK_LIST.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH_USER.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH_USER_LIST.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_VIEW_BOOK_INFO_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_VIEW_LIB_INFO_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_VIEW_STATE_INFO_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_VIEW_USER_INFO_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\REFUSE_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\REFUSE_VIEW_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VIEW_INFO_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.cpp
# End Source File
# Begin Source File

SOURCE=.\XLAutomation.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BOOK_COOPERATIVE_GROUP_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\BOOK_COOPERATIVE_GROUP_OFFER.h
# End Source File
# Begin Source File

SOURCE=.\BOOK_COOPERATIVE_GROUP_REQUEST.h
# End Source File
# Begin Source File

SOURCE=.\BookCooperativeGroup.h
# End Source File
# Begin Source File

SOURCE=.\Cmb_GridItem.h
# End Source File
# Begin Source File

SOURCE=.\Data.h
# End Source File
# Begin Source File

SOURCE=.\Edit_GridItem.h
# End Source File
# Begin Source File

SOURCE=.\HourEditControl.h
# End Source File
# Begin Source File

SOURCE=.\HourEditElem.h
# End Source File
# Begin Source File

SOURCE=.\ILL_API_APPENDIX_DLG.h
# End Source File
# Begin Source File

SOURCE=.\ILL_APL.h
# End Source File
# Begin Source File

SOURCE=.\ILL_APLWEB.h
# End Source File
# Begin Source File

SOURCE=.\ILL_CHANGE_REGNO.h
# End Source File
# Begin Source File

SOURCE=.\ILL_COMPARE.h
# End Source File
# Begin Source File

SOURCE=.\ILL_MODIFY.h
# End Source File
# Begin Source File

SOURCE=.\ILL_PERMISSION_LIST.h
# End Source File
# Begin Source File

SOURCE=.\ILL_RFID_SECURITY_CHANGE.h
# End Source File
# Begin Source File

SOURCE=.\ILL_RFID_SECURITY_CHANGE2.h
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH.h
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH_BOOK.h
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH_BOOK_LIST.h
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH_USER.h
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH_USER_LIST.h
# End Source File
# Begin Source File

SOURCE=.\ILL_VIEW_BOOK_INFO_DLG.h
# End Source File
# Begin Source File

SOURCE=.\ILL_VIEW_LIB_INFO_DLG.h
# End Source File
# Begin Source File

SOURCE=.\ILL_VIEW_STATE_INFO_DLG.h
# End Source File
# Begin Source File

SOURCE=.\ILL_VIEW_USER_INFO_DLG.h
# End Source File
# Begin Source File

SOURCE=.\REFUSE_DLG.h
# End Source File
# Begin Source File

SOURCE=.\REFUSE_VIEW_DLG.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VIEW_INFO_DLG.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.h
# End Source File
# Begin Source File

SOURCE=.\XLAutomation.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BOOK_COOPERATIVE_GROUP.rc2
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
