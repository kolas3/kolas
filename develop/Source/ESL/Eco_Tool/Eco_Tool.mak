# Microsoft Developer Studio Generated NMAKE File, Based on Eco_Tool.dsp
!IF "$(CFG)" == ""
CFG=Eco_Tool - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Eco_Tool - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Eco_Tool - Win32 Release" && "$(CFG)" !=\
 "Eco_Tool - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Eco_Tool.mak" CFG="Eco_Tool - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Eco_Tool - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Eco_Tool - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "Eco_Tool - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Eco_Tool.dll"

!ELSE 

ALL : "$(OUTDIR)\Eco_Tool.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Eco_FileIO.obj"
	-@erase "$(INTDIR)\Eco_Time.obj"
	-@erase "$(INTDIR)\Eco_Tool.obj"
	-@erase "$(INTDIR)\Eco_Tool.pch"
	-@erase "$(INTDIR)\Eco_Tool.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Eco_Tool.dll"
	-@erase "$(OUTDIR)\Eco_Tool.exp"
	-@erase "$(OUTDIR)\Eco_Tool.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\Eco_Tool.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Eco_Tool.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Eco_Tool.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\Eco_Tool.pdb" /machine:I386 /def:".\Eco_Tool.def"\
 /out:"$(OUTDIR)\Eco_Tool.dll" /implib:"$(OUTDIR)\Eco_Tool.lib" 
DEF_FILE= \
	".\Eco_Tool.def"
LINK32_OBJS= \
	"$(INTDIR)\Eco_FileIO.obj" \
	"$(INTDIR)\Eco_Time.obj" \
	"$(INTDIR)\Eco_Tool.obj" \
	"$(INTDIR)\Eco_Tool.res" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\Eco_Tool.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Eco_Tool - Win32 Debug"

OUTDIR=\통합정보시스템\develop\lib
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\iis_exe\Eco_Tool.dll"

!ELSE 

ALL : "..\..\..\..\iis_exe\Eco_Tool.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Eco_FileIO.obj"
	-@erase "$(INTDIR)\Eco_Time.obj"
	-@erase "$(INTDIR)\Eco_Tool.obj"
	-@erase "$(INTDIR)\Eco_Tool.pch"
	-@erase "$(INTDIR)\Eco_Tool.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Eco_Tool.exp"
	-@erase "$(OUTDIR)\Eco_Tool.lib"
	-@erase "$(OUTDIR)\Eco_Tool.pdb"
	-@erase "..\..\..\..\iis_exe\Eco_Tool.dll"
	-@erase "..\..\..\..\iis_exe\Eco_Tool.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\Eco_Tool.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Eco_Tool.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Eco_Tool.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=exception.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\Eco_Tool.pdb" /debug /machine:I386 /def:".\Eco_Tool.def"\
 /out:"\통합정보시스템\iis_exe\Eco_Tool.dll" /implib:"$(OUTDIR)\Eco_Tool.lib"\
 /pdbtype:sept 
DEF_FILE= \
	".\Eco_Tool.def"
LINK32_OBJS= \
	"$(INTDIR)\Eco_FileIO.obj" \
	"$(INTDIR)\Eco_Time.obj" \
	"$(INTDIR)\Eco_Tool.obj" \
	"$(INTDIR)\Eco_Tool.res" \
	"$(INTDIR)\StdAfx.obj"

"..\..\..\..\iis_exe\Eco_Tool.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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


!IF "$(CFG)" == "Eco_Tool - Win32 Release" || "$(CFG)" ==\
 "Eco_Tool - Win32 Debug"
SOURCE=.\Eco_FileIO.cpp
DEP_CPP_ECO_F=\
	".\StdAfx.h"\
	{$(INCLUDE)}"Eco_FileIO.h"\
	{$(INCLUDE)}"EXCEPT.H"\
	

"$(INTDIR)\Eco_FileIO.obj" : $(SOURCE) $(DEP_CPP_ECO_F) "$(INTDIR)"\
 "$(INTDIR)\Eco_Tool.pch"


SOURCE=.\Eco_Time.cpp
DEP_CPP_ECO_T=\
	".\StdAfx.h"\
	{$(INCLUDE)}"Eco_Time.h"\
	{$(INCLUDE)}"EXCEPT.H"\
	

"$(INTDIR)\Eco_Time.obj" : $(SOURCE) $(DEP_CPP_ECO_T) "$(INTDIR)"\
 "$(INTDIR)\Eco_Tool.pch"


SOURCE=.\Eco_Tool.cpp
DEP_CPP_ECO_TO=\
	".\StdAfx.h"\
	{$(INCLUDE)}"EXCEPT.H"\
	

"$(INTDIR)\Eco_Tool.obj" : $(SOURCE) $(DEP_CPP_ECO_TO) "$(INTDIR)"\
 "$(INTDIR)\Eco_Tool.pch"


SOURCE=.\Eco_Tool.rc
DEP_RSC_ECO_TOO=\
	".\res\Eco_Tool.rc2"\
	

"$(INTDIR)\Eco_Tool.res" : $(SOURCE) $(DEP_RSC_ECO_TOO) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "Eco_Tool - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\Eco_Tool.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Eco_Tool.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Eco_Tool - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fp"$(INTDIR)\Eco_Tool.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Eco_Tool.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

