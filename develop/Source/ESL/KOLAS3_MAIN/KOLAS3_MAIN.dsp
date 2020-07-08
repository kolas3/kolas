# Microsoft Developer Studio Project File - Name="KOLAS3_MAIN" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=KOLAS3_MAIN - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KOLAS3_MAIN.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KOLAS3_MAIN.mak" CFG="KOLAS3_MAIN - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KOLAS3_MAIN - Win32 Unicode Debug" (based on "Win32 (x86) Application")
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
# PROP BASE Output_Dir "KOLAS3_MAIN___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "KOLAS3_MAIN___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "KOLAS3_MAIN___Win32_Unicode_Debug"
# PROP Intermediate_Dir "KOLAS3_MAIN___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imm32.lib esl.lib ocimgr.lib Ws2_32.lib IndexToolKit.lib MENU_TOOLBAR_MGR.lib UC_NOTIFY_POPUP.lib UC_KOLISNetUseCheck.lib ECO_NOTICE_POPUP.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\bin\KOLAS II UP.exe" /pdbtype:sept
# ADD LINK32 imm32.lib esl.lib MENU_TOOLBAR_MGR.lib indextoolkit.lib KOLISNetWebSearch.lib MenuBar.lib CharEncodeMgr.lib UC_NOTIFY_POPUP.lib NetApi32.lib Iphlpapi.lib kl_api.lib LOC_CommonAPI.lib GPKI.lib /nologo /stack:0x1e8480 /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\bin\KOLASIII.exe" /pdbtype:sept
# Begin Target

# Name "KOLAS3_MAIN - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AutoCompleteList.cpp
# End Source File
# Begin Source File

SOURCE=.\BalloonHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboBoxEx2.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ECO_HtmlViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\ESL_ReferenceMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Explorer.cpp
# End Source File
# Begin Source File

SOURCE=.\Form.cpp
# End Source File
# Begin Source File

SOURCE=.\InternetNavigator.cpp
# End Source File
# Begin Source File

SOURCE=.\KIISystemInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\KOLAS3_MAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\KOLAS3_MAIN.rc
# End Source File
# Begin Source File

SOURCE=.\KOLAS3_MAINApi.cpp
# End Source File
# Begin Source File

SOURCE=.\KOLAS3_MAINDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\KOLAS3_MAINView.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBmp.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuItem.cpp
# End Source File
# Begin Source File

SOURCE=.\MToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkInfoMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\NewButton.cpp
# End Source File
# Begin Source File

SOURCE=.\RCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RCFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\RCThread.cpp
# End Source File
# Begin Source File

SOURCE=.\RCView.cpp
# End Source File
# Begin Source File

SOURCE=.\RegistryEx.cpp
# End Source File
# Begin Source File

SOURCE=.\RENEWAL_PASSWORD.cpp
# End Source File
# Begin Source File

SOURCE=.\SetAcceleratorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetAccleratorDlg_Modify.cpp
# End Source File
# Begin Source File

SOURCE=.\shockwaveflash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarBtnDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarItem.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\XPTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\YSButton.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AutoCompleteList.h
# End Source File
# Begin Source File

SOURCE=.\BalloonHelp.h
# End Source File
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\ComboBoxEx2.h
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\ECO_HtmlViewer.h
# End Source File
# Begin Source File

SOURCE=.\ESL_ReferenceMgr.h
# End Source File
# Begin Source File

SOURCE=.\Explorer.h
# End Source File
# Begin Source File

SOURCE=.\Form.h
# End Source File
# Begin Source File

SOURCE=.\InternetNavigator.h
# End Source File
# Begin Source File

SOURCE=.\KIISystemInfo.h
# End Source File
# Begin Source File

SOURCE=.\KOLAS3_MAIN.h
# End Source File
# Begin Source File

SOURCE=.\KOLAS3_MAINApi.h
# End Source File
# Begin Source File

SOURCE=.\KOLAS3_MAINDoc.h
# End Source File
# Begin Source File

SOURCE=.\KOLAS3_MAINView.h
# End Source File
# Begin Source File

SOURCE=.\LoadBmp.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MainObject.h
# End Source File
# Begin Source File

SOURCE=.\MenuBar.h
# End Source File
# Begin Source File

SOURCE=.\MenuItem.h
# End Source File
# Begin Source File

SOURCE=.\MenuTreeBar.h
# End Source File
# Begin Source File

SOURCE=.\MToolBar.h
# End Source File
# Begin Source File

SOURCE=.\NetworkInfoMgr.h
# End Source File
# Begin Source File

SOURCE=.\NewButton.h
# End Source File
# Begin Source File

SOURCE=.\RCDlg.h
# End Source File
# Begin Source File

SOURCE=.\RCFrame.h
# End Source File
# Begin Source File

SOURCE=.\RCThread.h
# End Source File
# Begin Source File

SOURCE=.\RCView.h
# End Source File
# Begin Source File

SOURCE=.\RegistryEx.h
# End Source File
# Begin Source File

SOURCE=.\RENEWAL_PASSWORD.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ESL\SBL.h
# End Source File
# Begin Source File

SOURCE=.\SetAcceleratorDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetAccleratorDlg_Modify.h
# End Source File
# Begin Source File

SOURCE=.\shockwaveflash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarBtnDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarItem.h
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.h
# End Source File
# Begin Source File

SOURCE=.\XPTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\YSButton.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\006_on.bmp
# End Source File
# Begin Source File

SOURCE=.\008.bmp
# End Source File
# Begin Source File

SOURCE=.\046_on.ico
# End Source File
# Begin Source File

SOURCE=.\1.bmp
# End Source File
# Begin Source File

SOURCE=.\16.ico
# End Source File
# Begin Source File

SOURCE=.\32.ico
# End Source File
# Begin Source File

SOURCE=.\res\32.ico
# End Source File
# Begin Source File

SOURCE=.\res\Background.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cube_icon_small_16.ico
# End Source File
# Begin Source File

SOURCE=.\res\cube_icon_small_32.ico
# End Source File
# Begin Source File

SOURCE=".\ecolas-s.ico"
# End Source File
# Begin Source File

SOURCE=.\res\ETLogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ExpressDesk.ico
# End Source File
# Begin Source File

SOURCE=.\res\ExpressTouch.ico
# End Source File
# Begin Source File

SOURCE=.\res\HelpMessage.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=".\res\icon_16 copy.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\icon_32 copy.bmp"
# End Source File
# Begin Source File

SOURCE=.\icon_kolasii_16.ico
# End Source File
# Begin Source File

SOURCE=.\icon_kolasii_32.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_k4.ico
# End Source File
# Begin Source File

SOURCE=.\kolas2_16.ico
# End Source File
# Begin Source File

SOURCE=.\Kolas2_32.ico
# End Source File
# Begin Source File

SOURCE=.\Kolas2_Light_32.ico
# End Source File
# Begin Source File

SOURCE=.\res\KOLAS3_MAIN.ico
# End Source File
# Begin Source File

SOURCE=.\res\KOLAS3_MAIN.rc2
# End Source File
# Begin Source File

SOURCE=.\res\KOLAS3_MAINDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\maintool.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MenuBarBk.bmp
# End Source File
# Begin Source File

SOURCE=.\RSC.bmp
# End Source File
# Begin Source File

SOURCE=.\slash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TabIcon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tl_icon_small_32.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarBk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\XPTabApp.bmp
# End Source File
# Begin Source File

SOURCE=".\▶.bmp"
# End Source File
# Begin Source File

SOURCE=".\구입.bmp"
# End Source File
# Begin Source File

SOURCE=".\구입자료관리.bmp"
# End Source File
# Begin Source File

SOURCE=".\단행.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\대출반납2_16.ico"
# End Source File
# Begin Source File

SOURCE=".\res\사본 - ToolbarMask.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\아이콘32사이즈모음.bmp"
# End Source File
# Begin Source File

SOURCE="..\..\@컴파일\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\ESL\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\HtmlEditor\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE=".\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\SMS\CXroshotManager\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\공동목록\CenterUpload_Api\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\공동목록\IRClient\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\공동목록\KOLISNetWebSearch\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\공동목록\Light_UC_CHANGE_01\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\공동목록\UC_NOTIFY_POPUP\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\공통\BO_ACQ_SPECIES_FORM\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\공통\BO_SPECIES_API\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\공통\CMarcChanger\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\공통\MarcAdjust\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\공통\MarcAdjustApi\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\관리\검색\MNG_KORMARC_EDU\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\관리\도서관관리\MNG_LIBINFO_MGR\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\관리\도서관관리\MNG_USEINFO_MGR\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\단행\수서\BO_ACQ_BASIC_DATA_MANAGER\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\단행\수서\BO_ACQ_PURCHASE_DATA_MANAGER\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\단행\열람\장서관리\BL_LOC_LOAN\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\단행\열람\장서관리\BO_LOC_3100\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\단행\열람\장서관리\BOOK_COOPERATIVE_GROUP\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\단행\정리\BO_CAT_REG\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\설치_설정\Kolas2up_ServerInstall\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\설치_설정\KOLAS2UP_SETUP\즐겨찾기.ico"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\TestTool\HtmlEditor\즐겨찾기.ico"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\XPTabApp.manifest
# End Source File
# End Target
# End Project
# Section KOLAS3_MAIN : {D27CDB6C-AE6D-11CF-96B8-444553540000}
# 	2:5:Class:CShockwaveFlash
# 	2:10:HeaderFile:shockwaveflash.h
# 	2:8:ImplFile:shockwaveflash.cpp
# End Section
# Section KOLAS3_MAIN : {D27CDB6E-AE6D-11CF-96B8-444553540000}
# 	2:21:DefaultSinkHeaderFile:shockwaveflash.h
# 	2:16:DefaultSinkClass:CShockwaveFlash
# End Section
# Section KOLAS3_MAIN : {68D5CDEE-38B9-46A5-9C5F-FE60B149E839}
# 	2:17:KIISystemInfo.cpp:KIISystemInfo.cpp
# 	2:21:CLASS: CKIISystemInfo:CKIISystemInfo
# 	2:15:KIISystemInfo.h:KIISystemInfo.h
# 	2:19:Application Include:KOLAS3_MAIN.h
# End Section
