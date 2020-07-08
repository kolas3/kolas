# Microsoft Developer Studio Project File - Name="BO_LOC_3100" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BO_LOC_3100 - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BO_LOC_3100.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_LOC_3100.mak" CFG="BO_LOC_3100 - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_LOC_3100 - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "BO_LOC_3100___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BO_LOC_3100___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\lib"
# PROP Intermediate_Dir "BO_LOC_3100___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
GSC=SPP_GramComp.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /GX /ZI /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"BO_LOC_3100.bsc"
# ADD BSC32 /nologo /o"BO_LOC_3100.bsc"
LINK32=link.exe
# ADD BASE LINK32 ESL.LIB BL_LOC_LOAN.lib BO_LOC_3300.lib BOBookDetailViewer.lib LOC_CommonAPI.lib Imm32.lib CLocMailMgr.lib MailMgr.lib Kolas2up_MobileCommon.lib CO_CENTER_USER_MANAGE.lib SIReportMgr.lib SIReportData.lib Kolas2up_CommonControl.lib PaintLib.lib libjpeg.lib eco_dongle.lib UnionUserManager.lib Check_Dup_Api.lib LOAN_INFO_SHARE_MANAGER.lib Kolas2up_ImageEditor.lib CButtonSTEX.lib PrnWTP100.lib BL_LOC_VOL_RESERVE_MNG.lib K2UP_Common.lib IndexToolKit.lib CXroshotManager.lib CO_SMS.lib CSMSContentsEditor.lib sms_06.lib BOBookDetailViewer.lib LOC_CommonAPI.lib Imm32.lib CLocMailMgr.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"..\..\..\..\..\..\dll\Mono\BO_LOC_3100.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 ESL.LIB BL_LOC_LOAN.lib BO_LOC_3300.lib BO_LOC_ARREAR.lib BOBookDetailViewer.lib LOC_CommonAPI.lib Imm32.lib CLocMailMgr.lib MailMgr.lib Kolas2up_MobileCommon.lib CO_CENTER_USER_MANAGE.lib SIReportMgr.lib SIReportData.lib Kolas2up_CommonControl.lib PaintLib.lib libjpeg.lib UnionUserManager.lib Check_Dup_Api.lib LOAN_INFO_SHARE_MANAGER.lib Kolas2up_ImageEditor.lib CButtonSTEX.lib PrnWTP100.lib K2UP_Common.lib IndexToolKit.lib CXroshotManager.lib CO_SMS.lib CSMSContentsEditor.lib sms_06.lib BOBookDetailViewer.lib LOC_CommonAPI.lib Imm32.lib CLocMailMgr.lib CharEncodeMgr.lib MARC_CONVERT.lib TrackBar.lib BL_LOC_USER.LIB SearchZipCodeApi.lib kl_api.lib CO_USER_CARD.LIB BO_LOC_3000.lib BL_LOC_USER.lib CERTIFY_API.lib KL_OTHER_LOAN_MNG.lib RFID_JOB.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\Mono\BO_LOC_3100.dll" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no /nodefaultlib
# Begin Target

# Name "BO_LOC_3100 - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BarcodeFlowSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3100.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3100.def
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3100.rc
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_31001.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3100_PrintReceipt.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3100_USER_INFO_ADDITION.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3100_USER_INFO_SIMPLE.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3110.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3180.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3182.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3183.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3190.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3191.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3192.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3193.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3195.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3197.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3199.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_LOAN_HIS.cpp
# End Source File
# Begin Source File

SOURCE=.\CfgFileApi.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl_ColorGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl_MarkStaticBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl_StrongStaticBox.cpp
# End Source File
# Begin Source File

SOURCE=.\FileLoanDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GridUpdateEdt.cpp
# End Source File
# Begin Source File

SOURCE=.\Lill_Panalty.cpp
# End Source File
# Begin Source File

SOURCE=.\LoanInfoUpdateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NoteCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PersonInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\RegistryEx.cpp
# End Source File
# Begin Source File

SOURCE=.\RemarkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitterBar.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdioFileEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TL_BO_LOC_LOAN_HIS.cpp
# End Source File
# Begin Source File

SOURCE=.\XInfoTip.cpp
# End Source File
# Begin Source File

SOURCE=.\xSkinButton.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BarcodeFlowSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_31001.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3100_PrintReceipt.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3100_USER_INFO_ADDITION.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3100_USER_INFO_SIMPLE.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3110.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3180.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3182.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3183.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3190.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3191.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3192.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3193.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3195.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3197.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3199.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_LOAN_HIS.h
# End Source File
# Begin Source File

SOURCE=.\CfgFileApi.h
# End Source File
# Begin Source File

SOURCE=.\ColorPicker.h
# End Source File
# Begin Source File

SOURCE=.\ColorPopup.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl_ColorGroupBox.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl_MarkStaticBox.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl_StrongStaticBox.h
# End Source File
# Begin Source File

SOURCE=.\FileLoanDlg.h
# End Source File
# Begin Source File

SOURCE=.\GridUpdateEdt.h
# End Source File
# Begin Source File

SOURCE=.\Lill_Panalty.h
# End Source File
# Begin Source File

SOURCE=.\LoanInfoUpdateDlg.h
# End Source File
# Begin Source File

SOURCE=.\NoteCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PersonInfo.h
# End Source File
# Begin Source File

SOURCE=.\RegistryEx.h
# End Source File
# Begin Source File

SOURCE=.\RemarkDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SplitterBar.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StdioFileEx.h
# End Source File
# Begin Source File

SOURCE=.\TL_BO_LOC_LOAN_HIS.h
# End Source File
# Begin Source File

SOURCE=.\XInfoTip.h
# End Source File
# Begin Source File

SOURCE=.\xSkinButton.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BO_LOC_3100.rc2
# End Source File
# Begin Source File

SOURCE=.\BSC_SCAN_R.bmp
# End Source File
# Begin Source File

SOURCE=.\BSC_SCAN_W.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_rfid_connect_test.bmp
# End Source File
# Begin Source File

SOURCE=.\res\certify_fail1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\certify_fail2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\certify_fail3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\certify_fail4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\certify_n1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\certify_n2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\certify_n3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\certify_n4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\certify_y1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\certify_y2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\certify_y3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\certify_y4.bmp
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon16.ico
# End Source File
# Begin Source File

SOURCE=.\icon_bar.ico
# End Source File
# Begin Source File

SOURCE=.\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\ItemMark.bmp
# End Source File
# Begin Source File

SOURCE=.\Kolas2_32.ico
# End Source File
# Begin Source File

SOURCE=.\res\off.bmp
# End Source File
# Begin Source File

SOURCE=.\res\off_width.bmp
# End Source File
# Begin Source File

SOURCE=.\res\on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\on_r.bmp
# End Source File
# Begin Source File

SOURCE=.\res\on_r_width.bmp
# End Source File
# Begin Source File

SOURCE=.\res\on_width.bmp
# End Source File
# Begin Source File

SOURCE=.\res\profile_empty_image.bmp
# End Source File
# Begin Source File

SOURCE=.\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\UnityUserIcon.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section BO_LOC_3100 : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section BO_LOC_3100 : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section BO_LOC_3100 : {0ECD9B62-23AA-11D0-B351-00A0C9055D8E}
# 	2:5:Class:CMSHFlexGrid
# 	2:10:HeaderFile:mshflexgrid.h
# 	2:8:ImplFile:mshflexgrid.cpp
# End Section
# Section BO_LOC_3100 : {0000050E-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:CRecordset1
# 	2:10:HeaderFile:recordset.h
# 	2:8:ImplFile:recordset.cpp
# End Section
