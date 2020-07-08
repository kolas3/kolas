# Microsoft Developer Studio Project File - Name="BO_DATA_MANAGER" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BO_DATA_MANAGER - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BO_DATA_MANAGER.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_DATA_MANAGER.mak" CFG="BO_DATA_MANAGER - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_DATA_MANAGER - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "BO_DATA_MANAGER___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BO_DATA_MANAGER___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib/"
# PROP Intermediate_Dir "BO_DATA_MANAGER___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_MBCS" /D "_AFXEXT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D _WIN32_WINNT>=0x0501 /D _WIN32_IE>=0x0600 /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_WINDLL" /D "_AFXEXT" /D "_DEBUG" /D _WIN32_WINNT>=0x0501 /D _WIN32_IE>=0x0600 /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib IndexToolKit.lib K2UP_Common.lib COMMON_CONVERSION_MGR.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../../dll/common/BO_DATA_MANAGER.dll" /pdbtype:sept
# ADD LINK32 esl.lib IndexToolKit.lib K2UP_Common.lib COMMON_CONVERSION_MGR.lib CharEncodeMgr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../../dll/common/BO_DATA_MANAGER.dll" /pdbtype:sept
# Begin Target

# Name "BO_DATA_MANAGER - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AcqGroupInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_BookData.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_BookDataSet.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_BusinessData.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_DATA_MANAGER.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_DATA_MANAGER.def
# End Source File
# Begin Source File

SOURCE=.\BO_DATA_MANAGER.rc
# End Source File
# Begin Source File

SOURCE=.\BO_DataConverter.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_DataManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_DataSetManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_DonateInfoData.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_DonateInfoDataSet.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_IndexData.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_IndexDataSet.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_PurchaseInfoData.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_PurchaseInfoDataSet.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_SpeciesData.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_SpeciesDataSet.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_VolumeData.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_VolumeDataSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DivisionNoInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\IndexDataMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ReceiptNoInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WorkNoCollection.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkNoInfo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AcqGroupInfo.h
# End Source File
# Begin Source File

SOURCE=.\BO_BookData.h
# End Source File
# Begin Source File

SOURCE=.\BO_BookDataSet.h
# End Source File
# Begin Source File

SOURCE=.\BO_BusinessData.h
# End Source File
# Begin Source File

SOURCE=.\BO_DataConverter.h
# End Source File
# Begin Source File

SOURCE=.\BO_DataManager.h
# End Source File
# Begin Source File

SOURCE=.\BO_DataSetManager.h
# End Source File
# Begin Source File

SOURCE=.\BO_DonateInfoData.h
# End Source File
# Begin Source File

SOURCE=.\BO_DonateInfoDataSet.h
# End Source File
# Begin Source File

SOURCE=.\BO_IndexData.h
# End Source File
# Begin Source File

SOURCE=.\BO_IndexDataSet.h
# End Source File
# Begin Source File

SOURCE=.\BO_PurchaseInfoData.h
# End Source File
# Begin Source File

SOURCE=.\BO_PurchaseInfoDataSet.h
# End Source File
# Begin Source File

SOURCE=.\BO_SpeciesData.h
# End Source File
# Begin Source File

SOURCE=.\BO_SpeciesDataSet.h
# End Source File
# Begin Source File

SOURCE=.\BO_VolumeData.h
# End Source File
# Begin Source File

SOURCE=.\BO_VolumeDataSet.h
# End Source File
# Begin Source File

SOURCE=.\DivisionNoInfo.h
# End Source File
# Begin Source File

SOURCE=.\GroupInfo.h
# End Source File
# Begin Source File

SOURCE=.\IndexDataMgr.h
# End Source File
# Begin Source File

SOURCE=.\ReceiptNoInfo.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WorkNoCollection.h
# End Source File
# Begin Source File

SOURCE=.\WorkNoInfo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BO_DATA_MANAGER.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
