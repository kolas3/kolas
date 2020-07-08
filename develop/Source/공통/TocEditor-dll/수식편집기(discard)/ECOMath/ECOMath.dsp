# Microsoft Developer Studio Project File - Name="ECOMath" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ECOMath - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ECOMath.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ECOMath.mak" CFG="ECOMath - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ECOMath - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ECOMath - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ECOMath - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Imm32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ECOMath - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Imm32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../../../../../EXE/ECOMath.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ECOMath - Win32 Release"
# Name "ECOMath - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ArrowObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ASCIIObject.cpp
# End Source File
# Begin Source File

SOURCE=.\BarObject.cpp
# End Source File
# Begin Source File

SOURCE=.\BiOpObject.cpp
# End Source File
# Begin Source File

SOURCE=.\BlankObject.cpp
# End Source File
# Begin Source File

SOURCE=.\CBmpMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\EcoDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ECOMath.cpp
# End Source File
# Begin Source File

SOURCE=.\ECOMath.rc
# End Source File
# Begin Source File

SOURCE=.\ECOMathDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ECOMathView.cpp
# End Source File
# Begin Source File

SOURCE=.\FenceObject.cpp
# End Source File
# Begin Source File

SOURCE=.\FractionObject.cpp
# End Source File
# Begin Source File

SOURCE=.\GreekObject.cpp
# End Source File
# Begin Source File

SOURCE=.\InitDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\IntegralObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ListOfTexObject.cpp
# End Source File
# Begin Source File

SOURCE=.\LogicalObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MiscObject.cpp
# End Source File
# Begin Source File

SOURCE=.\RadicalObject.cpp
# End Source File
# Begin Source File

SOURCE=.\registry.cpp
# End Source File
# Begin Source File

SOURCE=.\RelationObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SetObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SourceView.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticButton.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SumObject.cpp
# End Source File
# Begin Source File

SOURCE=.\TexObject.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ArrowObject.h
# End Source File
# Begin Source File

SOURCE=.\ASCIIObject.h
# End Source File
# Begin Source File

SOURCE=.\BarObject.h
# End Source File
# Begin Source File

SOURCE=.\BiOpObject.h
# End Source File
# Begin Source File

SOURCE=.\BlankObject.h
# End Source File
# Begin Source File

SOURCE=.\CBmpMenu.h
# End Source File
# Begin Source File

SOURCE=.\EcoDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\ECOMath.h
# End Source File
# Begin Source File

SOURCE=.\ECOMathDoc.h
# End Source File
# Begin Source File

SOURCE=.\ECOMathView.h
# End Source File
# Begin Source File

SOURCE=.\FenceObject.h
# End Source File
# Begin Source File

SOURCE=.\FractionObject.h
# End Source File
# Begin Source File

SOURCE=.\GreekObject.h
# End Source File
# Begin Source File

SOURCE=.\InitDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\IntegralObject.h
# End Source File
# Begin Source File

SOURCE=.\ListOfTexObject.h
# End Source File
# Begin Source File

SOURCE=.\LogicalObject.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MiscObject.h
# End Source File
# Begin Source File

SOURCE=.\RadicalObject.h
# End Source File
# Begin Source File

SOURCE=.\registry.h
# End Source File
# Begin Source File

SOURCE=.\RelationObject.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScriptObject.h
# End Source File
# Begin Source File

SOURCE=.\SetObject.h
# End Source File
# Begin Source File

SOURCE=.\SharedMemory.h
# End Source File
# Begin Source File

SOURCE=.\SourceView.h
# End Source File
# Begin Source File

SOURCE=.\StaticButton.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SumObject.h
# End Source File
# Begin Source File

SOURCE=.\TexObject.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\arrows.bmp
# End Source File
# Begin Source File

SOURCE=.\res\arrows_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bar_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ECOMath.ico
# End Source File
# Begin Source File

SOURCE=.\res\ECOMath.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ECOMathDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\fence.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fence_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fraction.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fraction_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\greek.bmp
# End Source File
# Begin Source File

SOURCE=.\res\greek_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\integral.bmp
# End Source File
# Begin Source File

SOURCE=.\res\integral_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\labeled.bmp
# End Source File
# Begin Source File

SOURCE=.\res\labeled_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logical.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logical_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\misc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\misc_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ne.bmp
# End Source File
# Begin Source File

SOURCE=.\res\opertator.bmp
# End Source File
# Begin Source File

SOURCE=.\res\opertator_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\radical.bmp
# End Source File
# Begin Source File

SOURCE=.\res\radical_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\relation.bmp
# End Source File
# Begin Source File

SOURCE=.\res\relation_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\script.bmp
# End Source File
# Begin Source File

SOURCE=.\res\script_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\set.bmp
# End Source File
# Begin Source File

SOURCE=.\res\set_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sum.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sum_on.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\upgreek.bmp
# End Source File
# Begin Source File

SOURCE=.\res\upgreek_on.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
