# Microsoft Developer Studio Generated NMAKE File, Based on exception.dsp
!IF "$(CFG)" == ""
CFG=exception - Win32 Debug
!MESSAGE No configuration specified. Defaulting to exception - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "exception - Win32 Release" && "$(CFG)" !=\
 "exception - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "exception.mak" CFG="exception - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "exception - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "exception - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "exception - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\exception.dll"

!ELSE 

ALL : "$(OUTDIR)\exception.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\EXCEPT.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\exception.dll"
	-@erase "$(OUTDIR)\exception.exp"
	-@erase "$(OUTDIR)\exception.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\exception.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\exception.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\exception.pdb" /machine:I386 /out:"$(OUTDIR)\exception.dll"\
 /implib:"$(OUTDIR)\exception.lib" 
LINK32_OBJS= \
	"$(INTDIR)\EXCEPT.obj"

"$(OUTDIR)\exception.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "exception - Win32 Debug"

OUTDIR=\통합정보시스템\develop\lib
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\iis_exe\exception.dll"

!ELSE 

ALL : "..\..\..\..\iis_exe\exception.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\EXCEPT.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\exception.exp"
	-@erase "$(OUTDIR)\exception.lib"
	-@erase "$(OUTDIR)\exception.pdb"
	-@erase "..\..\..\..\iis_exe\exception.dll"
	-@erase "..\..\..\..\iis_exe\exception.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\exception.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\exception.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\exception.pdb" /debug /machine:I386\
 /out:"\통합정보시스템\iis_exe\exception.dll" /implib:"$(OUTDIR)\exception.lib"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\EXCEPT.obj"

"..\..\..\..\iis_exe\exception.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(CFG)" == "exception - Win32 Release" || "$(CFG)" ==\
 "exception - Win32 Debug"
SOURCE=.\EXCEPT.cpp
DEP_CPP_EXCEP=\
	{$(INCLUDE)}"stdafx.h"\
	

"$(INTDIR)\EXCEPT.obj" : $(SOURCE) $(DEP_CPP_EXCEP) "$(INTDIR)"



!ENDIF 

