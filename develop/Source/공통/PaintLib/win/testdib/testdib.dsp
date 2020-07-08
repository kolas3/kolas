# Microsoft Developer Studio Project File - Name="testdib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=testdib - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "testdib.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "testdib.mak" CFG="testdib - Win32 Release"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "testdib - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "testdib - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "testdib - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin\Release"
# PROP Intermediate_Dir "..\..\obj\Release\testdib"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "..\..\Common" /I "..\paintlib" /D "NDEBUG" /D "TEST_DDRAW" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 paintlib.lib libjpeg.lib libtiff.lib winmm.lib ddraw.lib libungif.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\..\lib\release"
# SUBTRACT LINK32 /profile /map /debug

!ELSEIF  "$(CFG)" == "testdib - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin\Debug"
# PROP Intermediate_Dir "..\..\obj\Debug\testdib"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\..\common" /I "..\paintlib" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "TEST_DDRAW" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 paintlib.lib libjpeg.lib libtiff.lib winmm.lib ddraw.lib libungif.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"..\..\lib\debug"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF

# Begin Target

# Name "testdib - Win32 Release"
# Name "testdib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\dibgrit.CPP
# End Source File
# Begin Source File

SOURCE=.\Dibwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\drawhelp.cpp
# End Source File
# Begin Source File

SOURCE=.\grarray.cpp
# End Source File
# Begin Source File

SOURCE=.\Gritem.cpp
# End Source File
# Begin Source File

SOURCE=.\Maindlg.cpp
# End Source File
# Begin Source File

SOURCE=.\mtdibwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\shadow.CPP
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\testdib.cpp
# End Source File
# Begin Source File

SOURCE=.\testdib.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\dibgrit.h
# End Source File
# Begin Source File

SOURCE=.\dibwnd.h
# End Source File
# Begin Source File

SOURCE=.\drawhelp.h
# End Source File
# Begin Source File

SOURCE=.\grarray.h
# End Source File
# Begin Source File

SOURCE=.\gritem.h
# End Source File
# Begin Source File

SOURCE=.\maindlg.h
# End Source File
# Begin Source File

SOURCE=.\mtdibwnd.h
# End Source File
# Begin Source File

SOURCE=.\shadow.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\testdib.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\dibtest.ico
# End Source File
# Begin Source File

SOURCE=.\res\dibtest.rc2
# End Source File
# Begin Source File

SOURCE=.\res\test3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\test4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\test8.bmp
# End Source File
# End Group
# End Target
# End Project
