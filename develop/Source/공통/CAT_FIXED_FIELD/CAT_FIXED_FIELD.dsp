# Microsoft Developer Studio Project File - Name="CAT_FIXED_FIELD" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CAT_FIXED_FIELD - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CAT_FIXED_FIELD.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CAT_FIXED_FIELD.mak" CFG="CAT_FIXED_FIELD - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CAT_FIXED_FIELD - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "CAT_FIXED_FIELD___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "CAT_FIXED_FIELD___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib/"
# PROP Intermediate_Dir "CAT_FIXED_FIELD___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
GSC=SPP_GramComp.exe
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 esl.lib KOMarcGetter.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\common\CAT_FIXED_FIELD.dll" /pdbtype:sept
# ADD LINK32 esl.lib KOMarcGetter.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\dll\common\CAT_FIXED_FIELD.dll" /pdbtype:sept
# Begin Target

# Name "CAT_FIXED_FIELD - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Cat007FieldContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_A.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_C.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_D.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_G.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_H.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_K.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_M.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_O.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_S.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_T.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_V.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_Z.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedFieldMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_AC.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_CF.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_MP.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_MU.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_O.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_SE.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_TX.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_VM.cpp
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedFieldMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\CAT_FIXED_FIELD.cpp
# End Source File
# Begin Source File

SOURCE=.\CAT_FIXED_FIELD.def
# End Source File
# Begin Source File

SOURCE=.\CAT_FIXED_FIELD.rc
# End Source File
# Begin Source File

SOURCE=.\FixedFieldInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Cat007FieldContainer.h
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_A.h
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_C.h
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_D.h
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_G.h
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_H.h
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_K.h
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_M.h
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_O.h
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_S.h
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_T.h
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_V.h
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedField_Z.h
# End Source File
# Begin Source File

SOURCE=.\Cat007FixedFieldMgr.h
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_AC.h
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_CF.h
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_MP.h
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_MU.h
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_O.h
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_SE.h
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_TX.h
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedField_VM.h
# End Source File
# Begin Source File

SOURCE=.\Cat008FixedFieldMgr.h
# End Source File
# Begin Source File

SOURCE=.\FixedFieldInfo.h
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

SOURCE=.\res\CAT_FIXED_FIELD.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
