# Microsoft Developer Studio Project File - Name="Kalendar" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Kalendar - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Kalendar.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Kalendar.mak" CFG="Kalendar - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Kalendar - Win32 Unicode Debug" (based on "Win32 (x86) Application")
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
# PROP BASE Output_Dir "Kalendar___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "Kalendar___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Kalendar___Win32_Unicode_Debug"
# PROP Intermediate_Dir "Kalendar___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 shlwapi.lib esl.lib /nologo /subsystem:windows /debug /machine:I386 /out:"G:\Kolas2up_uni\bin\Kalendar.exe" /pdbtype:sept
# ADD LINK32 shlwapi.lib esl.lib trackbar.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\bin\Kalendar.exe" /pdbtype:sept
# Begin Target

# Name "Kalendar - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AlarmTime.cpp
# End Source File
# Begin Source File

SOURCE=.\AlarmTimeMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonMonth.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonMonthEng.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonYear.cpp
# End Source File
# Begin Source File

SOURCE=.\CalFileMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Data.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDay.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgListView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMemo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSEQ.cpp
# End Source File
# Begin Source File

SOURCE=.\CHTMLLIstCtrl\DrawHTML.C
# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\CHTMLLIstCtrl\HTMLListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Kalendar.cpp
# End Source File
# Begin Source File

SOURCE=.\Kalendar.rc
# End Source File
# Begin Source File

SOURCE=.\KalendarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KoreanDate.cpp
# End Source File
# Begin Source File

SOURCE=.\Memo.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TabDialog\TabDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Tfxdatatip.cpp
# End Source File
# Begin Source File

SOURCE=.\YSButton\YSButton.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AlarmTime.h
# End Source File
# Begin Source File

SOURCE=.\AlarmTimeMgr.h
# End Source File
# Begin Source File

SOURCE=.\ButtonMonth.h
# End Source File
# Begin Source File

SOURCE=.\ButtonMonthEng.h
# End Source File
# Begin Source File

SOURCE=.\ButtonYear.h
# End Source File
# Begin Source File

SOURCE=.\CalFileMgr.h
# End Source File
# Begin Source File

SOURCE=.\Data.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarm.h
# End Source File
# Begin Source File

SOURCE=.\DlgDay.h
# End Source File
# Begin Source File

SOURCE=.\DlgListView.h
# End Source File
# Begin Source File

SOURCE=.\DlgMemo.h
# End Source File
# Begin Source File

SOURCE=.\DlgSEQ.h
# End Source File
# Begin Source File

SOURCE=.\CHTMLLIstCtrl\DrawHTML.h
# End Source File
# Begin Source File

SOURCE=.\CHTMLLIstCtrl\HTMLListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Kalendar.h
# End Source File
# Begin Source File

SOURCE=.\KalendarDlg.h
# End Source File
# Begin Source File

SOURCE=.\KoreanDate.h
# End Source File
# Begin Source File

SOURCE=.\CHTMLLIstCtrl\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\Memo.h
# End Source File
# Begin Source File

SOURCE=.\ReadCfgFile.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TabDialog\TabDialog.h
# End Source File
# Begin Source File

SOURCE=.\Tfxdatatip.h
# End Source File
# Begin Source File

SOURCE=.\YSButton\YSButton.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\num\0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\year\0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\0_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\0_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\year\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month\10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month_eng\10_oct.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month\11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month_eng\11_nov.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month\12.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month_eng\12_dec.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month_eng\1_jan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\1_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\1_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\year\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month_eng\2_feb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\2_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\2_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\year\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month_eng\3_mar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\3_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\3_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month\4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\year\4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month_eng\4_apr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\4_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\4_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month\5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\year\5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month_eng\5_may.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\5_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\5_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month\6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\year\6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month_eng\6_jun.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\6_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\6_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month\7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\year\7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month_eng\7_jul.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\7_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\7_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month\8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\year\8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month_eng\8_aug.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\8_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\8_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month\9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\year\9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\9_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\9_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\month_eng\9_sep.bmp
# End Source File
# Begin Source File

SOURCE=..\bmp\Kalendar\bg.BMP
# End Source File
# Begin Source File

SOURCE=.\res\bg.BMP
# End Source File
# Begin Source File

SOURCE=.\res\popup\bg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tab\bg_top.bmp
# End Source File
# Begin Source File

SOURCE=.\res\popup\bgalarm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\popup\bglist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\popup\close.bmp
# End Source File
# Begin Source File

SOURCE=.\res\close_kalendar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\popup\closered.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button\DivisionMode_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button\DivisionNo_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dropdown_B.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dropdown_L.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dropdown_R.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dropdown_T.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dropdown_u.bmp
# End Source File
# Begin Source File

SOURCE=.\res\memorial\Holiday.bmp
# End Source File
# Begin Source File

SOURCE=..\bmp\Kalendar\icon\k_01_icon.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon\k_01_icon.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon\k_01_icon_16.ico
# End Source File
# Begin Source File

SOURCE=.\res\Kalendar.ico
# End Source File
# Begin Source File

SOURCE=.\res\Kalendar.rc2
# End Source File
# Begin Source File

SOURCE=.\res\listbg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button\ListView_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\memo_bg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button\MemoMode_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button\mini_search.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button\NextMonth_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\num\NULL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button\PrevMonth_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\popup\SEARCH.bmp
# End Source File
# Begin Source File

SOURCE=.\res\search.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button\Search_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\memorial\Today.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button\Today_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\memorial\TodayHoliday.bmp
# End Source File
# Begin Source File

SOURCE=.\res\popup\xpbtn.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
