# Microsoft Developer Studio Project File - Name="BL_LOC_LOAN" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BL_LOC_LOAN - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BL_LOC_LOAN.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BL_LOC_LOAN.mak" CFG="BL_LOC_LOAN - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BL_LOC_LOAN - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "BL_LOC_LOAN___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BL_LOC_LOAN___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\lib"
# PROP Intermediate_Dir "BL_LOC_LOAN___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"BL_LOC_LOAN.bsc"
# ADD BSC32 /nologo /o"BL_LOC_LOAN.bsc"
LINK32=link.exe
# ADD BASE LINK32 ESL.LIB BL_LOC_USER.lib LOC_CommonAPI.lib BOBookDetailViewer.lib SearchZipCodeAPI.lib CLocMailMgr.lib MailMgr.lib MNG_LIB_HOLIDAY_MGR.lib FileManager.lib Kolas2up_MobileCommon.lib CO_CENTER_USER_MANAGE.lib Kolas2up_CommonControl.lib K2UP_Common.lib IndexToolKit.lib SEBookDetailViewer.lib Check_Dup_Api.lib LOAN_INFO_SHARE_MANAGER.lib SIReportMgr.lib SIReportData.lib PrnWTP100.lib CSMSContentsEditor.lib CXroshotManager.lib CO_SMS.lib EPCQueryService.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\mono\BL_LOC_LOAN.dll" /pdbtype:sept
# ADD LINK32 ESL.LIB Imm32.lib LOC_CommonAPI.lib BOBookDetailViewer.lib SearchZipCodeAPI.lib CLocMailMgr.lib MailMgr.lib MNG_LIB_HOLIDAY_MGR.lib FileManager.lib Kolas2up_MobileCommon.lib CO_CENTER_USER_MANAGE.lib Kolas2up_CommonControl.lib K2UP_Common.lib IndexToolKit.lib SEBookDetailViewer.lib Check_Dup_Api.lib LOAN_INFO_SHARE_MANAGER.lib SIReportMgr.lib SIReportData.lib PrnWTP100.lib CSMSContentsEditor.lib CXroshotManager.lib CO_SMS.lib EPCQueryService.lib CharEncodeMgr.lib BL_LOC_USER.lib KL_Api.lib BO_LOC_ARREAR.lib BO_LOC_3000.lib Certify_Api.lib BO_ACQ_PRINT_MGR.lib SIReportMgr.lib SIReportData.lib COMMON_CONVERSION_MGR.lib RFID_JOB.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\..\dll\mono\BL_LOC_LOAN.dll" /pdbtype:sept
# Begin Target

# Name "BL_LOC_LOAN - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AutoRichEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BL_LOC_LOAN.cpp
# End Source File
# Begin Source File

SOURCE=.\BL_LOC_LOAN.def
# End Source File
# Begin Source File

SOURCE=.\BL_LOC_LOAN.rc
# End Source File
# Begin Source File

SOURCE=.\BL_LOC_LOAN_PROC.cpp
# End Source File
# Begin Source File

SOURCE=.\BL_LOC_NON_DB_BOOK_PROC.cpp
# End Source File
# Begin Source File

SOURCE=.\BL_LOC_RESERVATOIN_PROC.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3130.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3133.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3170.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3170_ReserveInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_LOAN_DATE.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_RECEIPT_PREVIEW_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_RECEIPT_PRINT_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\CardpasswdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CfgFileApi.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckLoanBook.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl_ColorGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctrl_StrongStaticBox.cpp
# End Source File
# Begin Source File

SOURCE=.\FeeArrearReceiptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FeeLoanStop.cpp
# End Source File
# Begin Source File

SOURCE=.\GipyoPrintAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupLibListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_API_APPENDIX_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_APL_3100.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_LOAN.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_RESERVE_CONVERT_LIB_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_RETURN.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_RETURN_OTHER.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_RETURN_SEARCH_BOOK.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_RETURN_WE.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH_BOOK.cpp
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH_BOOK_LIST.cpp
# End Source File
# Begin Source File

SOURCE=.\InputReservatoinExpireDay.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertPasswordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_DEFAULT.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_DELIVERY.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_DETAILVIEW.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_EDIT_MEMO.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_OFFER.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_REQUEST.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_SELECT_BOOK.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_PaymentListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_SearchLoanInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_SelectLibraryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_UserImportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_UserImportSetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_UserSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KL_UserSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KlCertifyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KlOtherLoanListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KlUserSwitchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KlUserSyncDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LibListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoanAppendixListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoanBookListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoanHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\LoanInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LonaManageValue.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewButton.cpp
# End Source File
# Begin Source File

SOURCE=.\NewButtonList.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressEx.cpp
# End Source File
# Begin Source File

SOURCE=.\RECEIPT_PRINT_SET_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\RECEIPT_SERARCH_DLG.cpp
# End Source File
# Begin Source File

SOURCE=.\ReservationInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReserveComplete.cpp
# End Source File
# Begin Source File

SOURCE=.\ReserveLinkBookInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReturnAppendixListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\searchbooklistdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSPreviewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\YSButton.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AutoRichEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BL_LOC_LOAN_PROC.h
# End Source File
# Begin Source File

SOURCE=.\BL_LOC_NON_DB_BOOK_PROC.h
# End Source File
# Begin Source File

SOURCE=.\BL_LOC_RESERVATOIN_PROC.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3130.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3133.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3170.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_3170_ReserveInfo.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_LOAN_DATE.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_RECEIPT_PREVIEW_DLG.h
# End Source File
# Begin Source File

SOURCE=.\BO_LOC_RECEIPT_PRINT_DLG.h
# End Source File
# Begin Source File

SOURCE=.\CardpasswdDlg.h
# End Source File
# Begin Source File

SOURCE=.\CfgFileApi.h
# End Source File
# Begin Source File

SOURCE=.\CheckLoanBook.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl_ColorGroupBox.h
# End Source File
# Begin Source File

SOURCE=.\Ctrl_StrongStaticBox.h
# End Source File
# Begin Source File

SOURCE=.\FeeArrearReceiptDlg.h
# End Source File
# Begin Source File

SOURCE=.\FeeLoanStop.h
# End Source File
# Begin Source File

SOURCE=.\GipyoPrintAPI.h
# End Source File
# Begin Source File

SOURCE=.\GroupLibListDlg.h
# End Source File
# Begin Source File

SOURCE=.\ILL_API_APPENDIX_DLG.h
# End Source File
# Begin Source File

SOURCE=.\ILL_APL_3100.h
# End Source File
# Begin Source File

SOURCE=.\ILL_LOAN.h
# End Source File
# Begin Source File

SOURCE=.\ILL_RESERVE_CONVERT_LIB_DLG.h
# End Source File
# Begin Source File

SOURCE=.\ILL_RETURN.h
# End Source File
# Begin Source File

SOURCE=.\ILL_RETURN_OTHER.h
# End Source File
# Begin Source File

SOURCE=.\ILL_RETURN_SEARCH_BOOK.h
# End Source File
# Begin Source File

SOURCE=.\ILL_RETURN_WE.h
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH_BOOK.h
# End Source File
# Begin Source File

SOURCE=.\ILL_SEARCH_BOOK_LIST.h
# End Source File
# Begin Source File

SOURCE=.\InputReservatoinExpireDay.h
# End Source File
# Begin Source File

SOURCE=.\InsertPasswordDlg.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_DEFAULT.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_DELIVERY.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_DETAILVIEW.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_EDIT_MEMO.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_OFFER.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_REQUEST.h
# End Source File
# Begin Source File

SOURCE=.\KL_OTHER_RETURN_SELECT_BOOK.h
# End Source File
# Begin Source File

SOURCE=.\KL_PaymentListDlg.h
# End Source File
# Begin Source File

SOURCE=.\KL_SearchLoanInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\KL_SelectLibraryDlg.h
# End Source File
# Begin Source File

SOURCE=.\KL_UserImportDlg.h
# End Source File
# Begin Source File

SOURCE=.\KL_UserImportSetDlg.h
# End Source File
# Begin Source File

SOURCE=.\KL_UserSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\KL_UserSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\KlCertifyDlg.h
# End Source File
# Begin Source File

SOURCE=.\KlOtherLoanListDlg.h
# End Source File
# Begin Source File

SOURCE=.\KlUserSwitchDlg.h
# End Source File
# Begin Source File

SOURCE=.\KlUserSyncDlg.h
# End Source File
# Begin Source File

SOURCE=.\LibListDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoanAppendixListDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoanBookListDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoanHistory.h
# End Source File
# Begin Source File

SOURCE=.\LoanInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\LonaManageValue.h
# End Source File
# Begin Source File

SOURCE=.\MessageDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewButton.h
# End Source File
# Begin Source File

SOURCE=.\NewButtonList.h
# End Source File
# Begin Source File

SOURCE=.\ProgressEx.h
# End Source File
# Begin Source File

SOURCE=.\RECEIPT_PRINT_SET_DLG.h
# End Source File
# Begin Source File

SOURCE=.\RECEIPT_SERARCH_DLG.h
# End Source File
# Begin Source File

SOURCE=.\ReservationInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReserveComplete.h
# End Source File
# Begin Source File

SOURCE=.\ReserveLinkBookInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ReturnAppendixListDlg.h
# End Source File
# Begin Source File

SOURCE=.\SearchBookListDlg.h
# End Source File
# Begin Source File

SOURCE=.\SMSPreviewDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\YSButton.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BL_LOC_LOAN.rc2
# End Source File
# Begin Source File

SOURCE=.\data_search.ico
# End Source File
# Begin Source File

SOURCE=.\HP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\KL_MSG.BMP
# End Source File
# Begin Source File

SOURCE=.\Kolas2_32.ico
# End Source File
# Begin Source File

SOURCE=.\Kolas2_32_reserve.ico
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

SOURCE=.\res\taginfo_bg.bmp
# End Source File
# Begin Source File

SOURCE=".\res\대출반납2_16.ico"
# End Source File
# Begin Source File

SOURCE=".\res\책.bmp"
# End Source File
# Begin Source File

SOURCE=".\책.bmp"
# End Source File
# Begin Source File

SOURCE="..\UnionUserManager\책.bmp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\..\..\TestTool\BeeperTest\책.bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
