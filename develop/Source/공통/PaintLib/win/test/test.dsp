# Microsoft Developer Studio Project File - Name="test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "test.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "test.mak" CFG="test - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "test - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin\Release"
# PROP Intermediate_Dir "..\..\obj\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\common" /I "..\paintlib" /I "..\..\common\filter" /I "..\libtiff" /I "..\..\otherlib\libtiff\libtiff" /I "..\libjpeg" /I "..\..\otherlib\libjpeg" /I "..\..\otherlib\libpng" /I "..\..\otherlib\zlib" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /fo"..\..\obj\Release\test/test.res" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 paintlib.lib libjpeg.lib libtiff.lib libpng.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib ole32.lib oleaut32.lib advapi32.lib shell32.lib /nologo /subsystem:console /pdb:none /machine:I386 /libpath:"../bin/release" /libpath:"..\..\lib\release"

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin\Debug"
# PROP Intermediate_Dir "..\..\obj\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\common" /I "..\paintlib" /I "..\..\common\filter" /I "..\libtiff" /I "..\..\otherlib\libtiff\libtiff" /I "..\libjpeg" /I "..\..\otherlib\libjpeg" /I "..\..\otherlib\libpng" /I "..\..\otherlib\zlib" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_WINDOWS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /fo"..\..\obj\Debug\test/test.res" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 paintlib.lib libjpeg.lib libtiff.lib libpng.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib ole32.lib oleaut32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../bin/debug" /libpath:"..\..\lib\debug"

!ENDIF

# Begin Target

# Name "test - Win32 Release"
# Name "test - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\common\test\pltest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\test\pltestdecoders.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\test\pltester.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\test\pltestfilters.cpp
# End Source File
# Begin Source File

SOURCE=.\test.rc
# End Source File
# Begin Source File

SOURCE=..\..\common\test\testfilterresizebilinear.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\test\testpaintlib.cpp

!IF  "$(CFG)" == "test - Win32 Release"

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\..\common\test\testpsddecoder.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\common\test\pltest.h
# End Source File
# Begin Source File

SOURCE=..\..\common\test\pltestdecoders.h
# End Source File
# Begin Source File

SOURCE=..\..\common\test\pltester.h
# End Source File
# Begin Source File

SOURCE=..\..\common\test\pltestfilters.h
# End Source File
# Begin Source File

SOURCE=..\..\common\test\testfilterresizebilinear.h
# End Source File
# Begin Source File

SOURCE=..\..\common\test\testpsddecoder.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\test.bmp
# End Source File
# End Group
# End Target
# End Project
