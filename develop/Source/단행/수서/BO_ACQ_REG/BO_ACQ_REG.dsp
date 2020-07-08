# Microsoft Developer Studio Project File - Name="BO_ACQ_REG" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BO_ACQ_REG - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BO_ACQ_REG.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_ACQ_REG.mak" CFG="BO_ACQ_REG - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_ACQ_REG - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BO_ACQ_REG - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BO_ACQ_REG - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BO_ACQ_REG - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "BO_ACQ_REG - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"..\..\..\..\lib/BO_ACQ_REG.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 esl.lib K2UP_Common.LIB IndexToolKit.lib BO_CAT_REGNO_GIVE.LIB BO_CAT_REGNO_CANCEL.LIB BO_CAT_LAST_NUMBER.lib BO_ACQ_GROUP_MANAGER.lib BO_ACQ_SPECIES_FORM.lib BO_ACQ_API.lib BO_SPECIES_API.lib FileImport.lib MarcToolKit.lib SIReportData.lib SIReportMgr.lib MarcAdjustApi.lib COMMON_CONVERSION_MGR.lib BO_DATA_CHANGE_MANAGER.LIB BO_ACQ_COMMON.LIB BO_CAT_REG.LIB /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\MONO\BO_ACQ_REG.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "BO_ACQ_REG - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BO_ACQ_REG___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BO_ACQ_REG___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "BO_ACQ_REG___Win32_Unicode_Debug"
# PROP Intermediate_Dir "BO_ACQ_REG___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"..\..\..\..\lib/BO_ACQ_REG.bsc"
# ADD BSC32 /nologo /o"..\..\..\..\lib/BO_ACQ_REG.bsc"
LINK32=link.exe
# ADD BASE LINK32 esl.lib K2UP_Common.LIB IndexToolKit.lib BO_CAT_REGNO_GIVE.LIB BO_CAT_REGNO_CANCEL.LIB BO_CAT_LAST_NUMBER.lib BO_ACQ_GROUP_MANAGER.lib BO_ACQ_SPECIES_FORM.lib BO_ACQ_API.lib BO_SPECIES_API.lib FileImport.lib MarcToolKit.lib SIReportData.lib SIReportMgr.lib MarcAdjustApi.lib COMMON_CONVERSION_MGR.lib BO_DATA_CHANGE_MANAGER.LIB BO_ACQ_COMMON.LIB BO_CAT_REG.LIB /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\MONO\BO_ACQ_REG.dll" /pdbtype:sept
# ADD LINK32 esl.lib K2UP_Common.LIB IndexToolKit.lib BO_CAT_REGNO_GIVE.LIB BO_CAT_REGNO_CANCEL.LIB BO_CAT_LAST_NUMBER.lib BO_ACQ_GROUP_MANAGER.lib BO_ACQ_SPECIES_FORM.lib BO_ACQ_API.lib BO_SPECIES_API.lib FileImport.lib MarcToolKit.lib SIReportData.lib SIReportMgr.lib MarcAdjustApi.lib COMMON_CONVERSION_MGR.lib BO_DATA_CHANGE_MANAGER.LIB BO_ACQ_COMMON.LIB BO_CAT_REG.LIB /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\dll\MONO\BO_ACQ_REG.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "BO_ACQ_REG - Win32 Release"
# Name "BO_ACQ_REG - Win32 Debug"
# Name "BO_ACQ_REG - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BO_ACQ_REG.cpp
# End Source File
# Begin Source File

SOURCE=.\BO_ACQ_REG.def
# End Source File
# Begin Source File

SOURCE=.\BO_ACQ_REG.rc
# End Source File
# Begin Source File

SOURCE="..\..\沥府\BO_CAT_REG\BO_CAT_REG_0410.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\沥府\BO_CAT_REG\BO_CAT_REG_0420.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\沥府\BO_CAT_REG\BO_CAT_REG_MAIN.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\沥府\BO_CAT_REG\BO_CAT_SEARCH.cpp"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="..\..\沥府\BO_CAT_REG\BO_CAT_REG_0410.h"
# End Source File
# Begin Source File

SOURCE="..\..\沥府\BO_CAT_REG\BO_CAT_REG_0420.h"
# End Source File
# Begin Source File

SOURCE="..\..\沥府\BO_CAT_REG\BO_CAT_REG_MAIN.h"
# End Source File
# Begin Source File

SOURCE="..\..\沥府\BO_CAT_REG\BO_CAT_SEARCH.h"
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

SOURCE=.\res\BO_ACQ_REG.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
