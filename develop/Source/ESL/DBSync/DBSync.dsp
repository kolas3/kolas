# Microsoft Developer Studio Project File - Name="DBSync" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DBSync - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DBSync.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DBSync.mak" CFG="DBSync - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DBSync - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "DBSync - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DBSync - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ocimgr.lib esl.lib /nologo /subsystem:windows /debug /machine:I386 /out:"\gas\bin\DBSync_mbcs.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "DBSync - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DBSync___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "DBSync___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DBSync___Win32_Unicode_Debug"
# PROP Intermediate_Dir "DBSync___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"\gas\bin\DBSync.exe" /pdbtype:sept
# ADD LINK32 ocimgr.lib esl.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"\gas\bin\DBSync_uni.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DBSync - Win32 Debug"
# Name "DBSync - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DBSync.cpp
# End Source File
# Begin Source File

SOURCE=.\DBSync.rc
# End Source File
# Begin Source File

SOURCE=.\DBSyncDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\DBSyncDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\RefEditor\ESL_Grid.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\RefEditor\string_fn.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DBSync.h
# End Source File
# Begin Source File

SOURCE=.\DBSyncDetail.h
# End Source File
# Begin Source File

SOURCE=.\DBSyncDlg.h
# End Source File
# Begin Source File

SOURCE=..\RefEditor\ESL_Grid.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\RefEditor\string_fn.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CLOSED.bmp
# End Source File
# Begin Source File

SOURCE=.\CLOSEU.bmp
# End Source File
# Begin Source File

SOURCE=.\CLOSEX.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DBSync.ico
# End Source File
# Begin Source File

SOURCE=.\res\DBSync.rc2
# End Source File
# Begin Source File

SOURCE=.\DETAILD.bmp
# End Source File
# Begin Source File

SOURCE=.\DETAILU.bmp
# End Source File
# Begin Source File

SOURCE=.\DETAILX.bmp
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\NEXTD.bmp
# End Source File
# Begin Source File

SOURCE=.\NEXTU.bmp
# End Source File
# Begin Source File

SOURCE=.\NEXTX.bmp
# End Source File
# Begin Source File

SOURCE=.\OKD.bmp
# End Source File
# Begin Source File

SOURCE=.\OKU.bmp
# End Source File
# Begin Source File

SOURCE=.\OKX.bmp
# End Source File
# Begin Source File

SOURCE=.\PREVD.bmp
# End Source File
# Begin Source File

SOURCE=.\PREVU.bmp
# End Source File
# Begin Source File

SOURCE=.\PREVX.bmp
# End Source File
# Begin Source File

SOURCE=.\RUND.bmp
# End Source File
# Begin Source File

SOURCE=.\RUNU.bmp
# End Source File
# Begin Source File

SOURCE=.\RUNX.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
