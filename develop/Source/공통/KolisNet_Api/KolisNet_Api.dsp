# Microsoft Developer Studio Project File - Name="KolisNet_Api" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=KolisNet_Api - WIN32 UNICODE DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KolisNet_Api.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KolisNet_Api.mak" CFG="KolisNet_Api - WIN32 UNICODE DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KolisNet_Api - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Output_Dir "../../../lib"
# PROP Intermediate_Dir "Unicode Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32 /wd n
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ESL.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\COMMON\KolisNet_Api.dll" /pdbtype:sept
# Begin Target

# Name "KolisNet_Api - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\KolisNet_Api.cpp
# End Source File
# Begin Source File

SOURCE=.\KolisNet_Api.def
# End Source File
# Begin Source File

SOURCE=.\KolisNet_Api.rc
# End Source File
# Begin Source File

SOURCE=.\KolisNetApi.cpp
# End Source File
# Begin Source File

SOURCE=.\KolisNetService.cpp
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

SOURCE="..\..\..\include\공동목록\KolisNetApi.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\KolisNetService.h"
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

SOURCE=.\res\KolisNet_Api.rc2
# End Source File
# End Group
# Begin Group "json"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\autolink.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\config.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\features.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\forwards.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\json.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\json_batchallocator.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\json_internalarray.inl"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\json_internalmap.inl"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\json_reader.inl"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\json_value.inl"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\json_valueiterator.inl"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\json_writer.inl"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\reader.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\value.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\include\공동목록\json\writer.h"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
