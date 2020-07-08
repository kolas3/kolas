# Microsoft Developer Studio Project File - Name="BL_LOC_USER" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BL_LOC_USER - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BL_LOC_USER.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BL_LOC_USER.mak" CFG="BL_LOC_USER - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BL_LOC_USER - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "BL_LOC_USER___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BL_LOC_USER___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\lib"
# PROP Intermediate_Dir "BL_LOC_USER___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"BL_LOC_USER.bsc"
# ADD BSC32 /nologo /o"BL_LOC_USER.bsc"
LINK32=link.exe
# ADD BASE LINK32 ESL.LIB SearchZipCodeAPI.lib LOC_CommonAPI.lib Imm32.lib CLocMailMgr.lib MailMgr.lib Kolas2up_MobileCommon.lib CO_CENTER_USER_MANAGE.lib PaintLib.lib libjpeg.lib Kolas2up_ImageEditor.lib Kolas2up_CommonControl.lib Check_Dup_Api.lib Check_Dup_Api.lib LOAN_INFO_SHARE_MANAGER.lib IndexToolKit.lib CO_USER_GROUP_MNG.lib SIReportMgr.lib SIReportData.lib BO_LOC_3100.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\mono\BL_LOC_USER.dll" /pdbtype:sept
# ADD LINK32 ESL.LIB SearchZipCodeAPI.lib LOC_CommonAPI.lib Imm32.lib CLocMailMgr.lib MailMgr.lib Kolas2up_MobileCommon.lib CO_CENTER_USER_MANAGE.lib PaintLib.lib libjpeg.lib Kolas2up_ImageEditor.lib Kolas2up_CommonControl.lib Check_Dup_Api.lib Check_Dup_Api.lib LOAN_INFO_SHARE_MANAGER.lib IndexToolKit.lib CO_USER_GROUP_MNG.lib SIReportMgr.lib SIReportData.lib BO_LOC_3100.lib CharEncodeMgr.lib PicManager.lib FileManager.lib BL_LOC_LOAN.LIB KL_API.lib BO_LOC_3000.lib Certify_Api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\mono\BL_LOC_USER.dll" /pdbtype:sept
# Begin Target

# Name "BL_LOC_USER - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BL_LOC_USER.cpp
# End Source File
# Begin Source File

SOURCE=.\BL_LOC_USER_PROC.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3320.cpp
# End Source File
# Begin Source File

SOURCE=.\CertifyMainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CertifyMypinView.cpp
# End Source File
# Begin Source File

SOURCE=.\CertifyPhoneInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CertifyPhoneView.cpp
# End Source File
# Begin Source File

SOURCE=.\CertifyView.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl_ColorGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl_MarkStaticBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl_RedMarkStaticBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl_StrongStaticBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DUP_USER_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\FAMILY_ID.cpp
# End Source File
# Begin Source File

SOURCE=.\FAMILY_LOAN_HIS.cpp
# End Source File
# Begin Source File

SOURCE=.\FamilyAddDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FamilyPreexistenceListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FamilyPreexistenceSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FamilySearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FeeReCard.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_SearchCurrentLoanInfoDetailDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_SearchCurrentLoanInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_SecedeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_STATEMENT_IMAGE.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_StatementDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_UserCardHistoryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_UserManagerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KlEditIdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KlEditPassDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KlIdPassViewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoanStopDateSet.cpp
# End Source File
# Begin Source File

SOURCE=.\LoanUserListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MypinCertifyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewButton.cpp
# End Source File
# Begin Source File

SOURCE=.\NewButtonList.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewImage.cpp
# End Source File
# Begin Source File

SOURCE=.\SECEDE_USER_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UserCardHistoryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserCertifyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserDuplecateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserEditSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UsernoRenumberDlg.cpp
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

SOURCE=.\BL_LOC_USER.def
# End Source File
# Begin Source File

SOURCE=.\BL_LOC_USER_PROC.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3320.h
# End Source File
# Begin Source File

SOURCE=.\CertifyMainDlg.h
# End Source File
# Begin Source File

SOURCE=.\CertifyMypinView.h
# End Source File
# Begin Source File

SOURCE=.\CertifyPhoneInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\CertifyPhoneView.h
# End Source File
# Begin Source File

SOURCE=.\CertifyView.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl_ColorGroupBox.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl_MarkStaticBox.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl_RedMarkStaticBox.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl_StrongStaticBox.h
# End Source File
# Begin Source File

SOURCE=.\DUP_USER_DLG.h
# End Source File
# Begin Source File

SOURCE=.\FAMILY_ID.h
# End Source File
# Begin Source File

SOURCE=.\FAMILY_LOAN_HIS.h
# End Source File
# Begin Source File

SOURCE=.\FamilyAddDlg.h
# End Source File
# Begin Source File

SOURCE=.\FamilyPreexistenceListDlg.h
# End Source File
# Begin Source File

SOURCE=.\FamilyPreexistenceSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\FamilySearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\FeeReCard.h
# End Source File
# Begin Source File

SOURCE=.\KL_SearchCurrentLoanInfoDetailDlg.h
# End Source File
# Begin Source File

SOURCE=.\KL_SearchCurrentLoanInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\KL_SecedeDlg.h
# End Source File
# Begin Source File

SOURCE=.\KL_STATEMENT_IMAGE.h
# End Source File
# Begin Source File

SOURCE=.\KL_StatementDlg.h
# End Source File
# Begin Source File

SOURCE=.\KL_UserCardHistoryDlg.h
# End Source File
# Begin Source File

SOURCE=.\KL_UserManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\KlEditIdDlg.h
# End Source File
# Begin Source File

SOURCE=.\KlEditPassDlg.h
# End Source File
# Begin Source File

SOURCE=.\KlIdPassViewDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoanStopDateSet.h
# End Source File
# Begin Source File

SOURCE=.\LoanUserListDlg.h
# End Source File
# Begin Source File

SOURCE=.\MypinCertifyDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewButton.h
# End Source File
# Begin Source File

SOURCE=.\NewButtonList.h
# End Source File
# Begin Source File

SOURCE=.\PreviewImage.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SECEDE_USER_DLG.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UserCardHistoryDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserCertifyDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserDuplecateDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserEditSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\UsernoRenumberDlg.h
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

SOURCE=.\BL_LOC_USER.rc
# End Source File
# Begin Source File

SOURCE=.\res\BL_LOC_USER.rc2
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

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\ItemMark.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ItemRedMark.bmp
# End Source File
# Begin Source File

SOURCE=.\res\KL_MSG.BMP
# End Source File
# Begin Source File

SOURCE=.\res\off.bmp
# End Source File
# Begin Source File

SOURCE=.\res\on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\on_r.bmp
# End Source File
# Begin Source File

SOURCE=.\res\UnityUserIcon.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\CertifyPhoneView.asp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
