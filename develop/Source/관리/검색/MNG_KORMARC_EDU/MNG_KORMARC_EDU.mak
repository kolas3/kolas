# Microsoft Developer Studio Generated NMAKE File, Based on MNG_KORMARC_EDU.dsp
!IF "$(CFG)" == ""
CFG=MNG_KORMARC_EDU - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MNG_KORMARC_EDU - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MNG_KORMARC_EDU - Win32 Release" && "$(CFG)" != "MNG_KORMARC_EDU - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MNG_KORMARC_EDU.mak" CFG="MNG_KORMARC_EDU - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MNG_KORMARC_EDU - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MNG_KORMARC_EDU - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "MNG_KORMARC_EDU - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MNG_KORMARC_EDU.dll"


CLEAN :
	-@erase "$(INTDIR)\MNG_KORMARC_EDU.obj"
	-@erase "$(INTDIR)\MNG_KORMARC_EDU.pch"
	-@erase "$(INTDIR)\MNG_KORMARC_EDU.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\webbrowser2.obj"
	-@erase "$(INTDIR)\WorkDataSearchDlg.obj"
	-@erase "$(OUTDIR)\MNG_KORMARC_EDU.dll"
	-@erase "$(OUTDIR)\MNG_KORMARC_EDU.exp"
	-@erase "$(OUTDIR)\MNG_KORMARC_EDU.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MNG_KORMARC_EDU.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MNG_KORMARC_EDU.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MNG_KORMARC_EDU.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\MNG_KORMARC_EDU.pdb" /machine:I386 /def:".\MNG_KORMARC_EDU.def" /out:"$(OUTDIR)\MNG_KORMARC_EDU.dll" /implib:"$(OUTDIR)\MNG_KORMARC_EDU.lib" 
DEF_FILE= \
	".\MNG_KORMARC_EDU.def"
LINK32_OBJS= \
	"$(INTDIR)\MNG_KORMARC_EDU.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MNG_KORMARC_EDU.res" \
	"$(INTDIR)\WorkDataSearchDlg.obj" \
	"$(INTDIR)\webbrowser2.obj"

"$(OUTDIR)\MNG_KORMARC_EDU.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MNG_KORMARC_EDU - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\..\..\dll\mng\MNG_KORMARC_EDU.dll"


CLEAN :
	-@erase "$(INTDIR)\MNG_KORMARC_EDU.obj"
	-@erase "$(INTDIR)\MNG_KORMARC_EDU.pch"
	-@erase "$(INTDIR)\MNG_KORMARC_EDU.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\webbrowser2.obj"
	-@erase "$(INTDIR)\WorkDataSearchDlg.obj"
	-@erase "$(OUTDIR)\MNG_KORMARC_EDU.exp"
	-@erase "$(OUTDIR)\MNG_KORMARC_EDU.lib"
	-@erase "$(OUTDIR)\MNG_KORMARC_EDU.pdb"
	-@erase "..\..\..\..\..\dll\mng\MNG_KORMARC_EDU.dll"
	-@erase "..\..\..\..\..\dll\mng\MNG_KORMARC_EDU.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MNG_KORMARC_EDU.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MNG_KORMARC_EDU.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MNG_KORMARC_EDU.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=esl.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\MNG_KORMARC_EDU.pdb" /debug /machine:I386 /def:".\MNG_KORMARC_EDU.def" /out:"..\..\..\..\..\dll\mng\MNG_KORMARC_EDU.dll" /implib:"$(OUTDIR)\MNG_KORMARC_EDU.lib" /pdbtype:sept 
DEF_FILE= \
	".\MNG_KORMARC_EDU.def"
LINK32_OBJS= \
	"$(INTDIR)\MNG_KORMARC_EDU.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MNG_KORMARC_EDU.res" \
	"$(INTDIR)\WorkDataSearchDlg.obj" \
	"$(INTDIR)\webbrowser2.obj"

"..\..\..\..\..\dll\mng\MNG_KORMARC_EDU.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("MNG_KORMARC_EDU.dep")
!INCLUDE "MNG_KORMARC_EDU.dep"
!ELSE 
!MESSAGE Warning: cannot find "MNG_KORMARC_EDU.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MNG_KORMARC_EDU - Win32 Release" || "$(CFG)" == "MNG_KORMARC_EDU - Win32 Debug"
SOURCE=.\MNG_KORMARC_EDU.cpp

"$(INTDIR)\MNG_KORMARC_EDU.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MNG_KORMARC_EDU.pch"


SOURCE=.\MNG_KORMARC_EDU.rc

"$(INTDIR)\MNG_KORMARC_EDU.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "MNG_KORMARC_EDU - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MNG_KORMARC_EDU.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MNG_KORMARC_EDU.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "MNG_KORMARC_EDU - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MNG_KORMARC_EDU.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MNG_KORMARC_EDU.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\webbrowser2.cpp

"$(INTDIR)\webbrowser2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MNG_KORMARC_EDU.pch"


SOURCE=.\WorkDataSearchDlg.cpp

"$(INTDIR)\WorkDataSearchDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MNG_KORMARC_EDU.pch"



!ENDIF 

