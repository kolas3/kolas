# Microsoft Developer Studio Project File - Name="libpng" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libpng - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "libpng.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "libpng.mak" CFG="libpng - Win32 Release"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "libpng - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libpng - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libpng - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\Release"
# PROP Intermediate_Dir "..\..\obj\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MD /W3 /GX /O2 /Ob2 /I "." /I "..\..\otherlib\libpng" /I "..\..\otherlib\zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\Debug"
# PROP Intermediate_Dir "..\..\obj\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MDd /W3 /GX /Z7 /Od /I "." /I "..\..\otherlib\libpng" /I "..\..\otherlib\zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF

# Begin Target

# Name "libpng - Win32 Release"
# Name "libpng - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\..\otherlib\zLib\deflate.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\infblock.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\infcodes.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\inffast.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\inffixed.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\inftrees.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\infutil.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\png.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngconf.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\trees.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\zconf.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\zlib.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\zutil.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "libPng"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\otherlib\libpng\png.c
DEP_CPP_PNG_C=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNG_C=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngerror.c
DEP_CPP_PNGER=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGER=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngget.c
DEP_CPP_PNGGE=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGGE=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngmem.c
DEP_CPP_PNGME=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGME=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngpread.c
DEP_CPP_PNGPR=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGPR=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngread.c
DEP_CPP_PNGRE=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGRE=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngrio.c
DEP_CPP_PNGRI=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGRI=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngrtran.c
DEP_CPP_PNGRT=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGRT=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngrutil.c
DEP_CPP_PNGRU=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngasmrd.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGRU=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngset.c
DEP_CPP_PNGSE=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGSE=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngtest.c
DEP_CPP_PNGTE=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGTE=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngtrans.c
DEP_CPP_PNGTR=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGTR=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngwio.c
DEP_CPP_PNGWI=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGWI=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngwrite.c
DEP_CPP_PNGWR=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGWR=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngwtran.c
DEP_CPP_PNGWT=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGWT=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libpng\pngwutil.c
DEP_CPP_PNGWU=\
  "..\..\otherlib\libpng\png.h"\
  "..\..\otherlib\libpng\pngconf.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

NODEP_CPP_PNGWU=\
  "..\..\otherlib\libpng\alloc.h"\

# End Source File
# End Group
# Begin Group "zLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\otherlib\zLib\adler32.c
DEP_CPP_ADLER=\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\crc32.c
DEP_CPP_CRC32=\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\deflate.c
DEP_CPP_DEFLA=\
  "..\..\otherlib\zLib\deflate.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\
  "..\..\otherlib\zLib\zutil.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\infblock.c
DEP_CPP_INFBL=\
  "..\..\otherlib\zLib\infblock.h"\
  "..\..\otherlib\zLib\infcodes.h"\
  "..\..\otherlib\zLib\inftrees.h"\
  "..\..\otherlib\zLib\infutil.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\
  "..\..\otherlib\zLib\zutil.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\infcodes.c
DEP_CPP_INFCO=\
  "..\..\otherlib\zLib\infblock.h"\
  "..\..\otherlib\zLib\infcodes.h"\
  "..\..\otherlib\zLib\inffast.h"\
  "..\..\otherlib\zLib\inftrees.h"\
  "..\..\otherlib\zLib\infutil.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\
  "..\..\otherlib\zLib\zutil.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\inffast.c
DEP_CPP_INFFA=\
  "..\..\otherlib\zLib\infblock.h"\
  "..\..\otherlib\zLib\infcodes.h"\
  "..\..\otherlib\zLib\inffast.h"\
  "..\..\otherlib\zLib\inftrees.h"\
  "..\..\otherlib\zLib\infutil.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\
  "..\..\otherlib\zLib\zutil.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\inflate.c
DEP_CPP_INFLA=\
  "..\..\otherlib\zLib\infblock.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\
  "..\..\otherlib\zLib\zutil.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\inftrees.c
DEP_CPP_INFTR=\
  "..\..\otherlib\zLib\inffixed.h"\
  "..\..\otherlib\zLib\inftrees.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\
  "..\..\otherlib\zLib\zutil.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\infutil.c
DEP_CPP_INFUT=\
  "..\..\otherlib\zLib\infblock.h"\
  "..\..\otherlib\zLib\infcodes.h"\
  "..\..\otherlib\zLib\inftrees.h"\
  "..\..\otherlib\zLib\infutil.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\
  "..\..\otherlib\zLib\zutil.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\trees.c
DEP_CPP_TREES=\
  "..\..\otherlib\zLib\deflate.h"\
  "..\..\otherlib\zLib\trees.h"\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\
  "..\..\otherlib\zLib\zutil.h"\

# End Source File
# Begin Source File

SOURCE=..\..\otherlib\zLib\zutil.c
DEP_CPP_ZUTIL=\
  "..\..\otherlib\zLib\zconf.h"\
  "..\..\otherlib\zLib\zlib.h"\
  "..\..\otherlib\zLib\zutil.h"\

# End Source File
# End Group
# End Target
# End Project
