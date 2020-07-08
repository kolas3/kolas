# Microsoft Developer Studio Project File - Name="libungif" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libungif - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "libungif.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "libungif.mak" CFG="libungif - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "libungif - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libungif - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/giflib-4.1.0/Lib", THCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libungif - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\Release"
# PROP Intermediate_Dir "..\..\obj\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W2 /GX /O2 /Ob2 /D "NDEBUG" /D "WIN32" /D "_LIB" /D "_OPEN_BINARY" /D "HAVE_IO_H" /D CDECL=__cdecl /D open=_open /D close=_close /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libungif - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\Debug"
# PROP Intermediate_Dir "..\..\obj\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W2 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_LIB" /D "_OPEN_BINARY" /D "HAVE_IO_H" /D CDECL=__cdecl /D open=_open /D close=_close /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF

# Begin Target

# Name "libungif - Win32 Release"
# Name "libungif - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\dev2gif.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\dgif_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\egif_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\getarg.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\gif_err.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\gif_font.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\gifalloc.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\qprintf.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\quantize.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\getarg.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\gif_hash.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\gif_lib.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\gif_lib_private.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\lib\GifWin.h
# End Source File
# End Group
# End Target
# End Project
