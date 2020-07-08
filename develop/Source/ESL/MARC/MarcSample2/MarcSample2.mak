# Microsoft Developer Studio Generated NMAKE File, Based on MarcSample2.dsp
!IF "$(CFG)" == ""
CFG=MarcSample2 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MarcSample2 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MarcSample2 - Win32 Release" && "$(CFG)" != "MarcSample2 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MarcSample2.mak" CFG="MarcSample2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MarcSample2 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MarcSample2 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "MarcSample2 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MarcSample2.dll"


CLEAN :
	-@erase "$(INTDIR)\MarcSample2.obj"
	-@erase "$(INTDIR)\MarcSample2.pch"
	-@erase "$(INTDIR)\MarcSample2.res"
	-@erase "$(INTDIR)\MarcSampleDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MarcSample2.dll"
	-@erase "$(OUTDIR)\MarcSample2.exp"
	-@erase "$(OUTDIR)\MarcSample2.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcSample2.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MarcSample2.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MarcSample2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\MarcSample2.pdb" /machine:I386 /def:".\MarcSample2.def" /out:"$(OUTDIR)\MarcSample2.dll" /implib:"$(OUTDIR)\MarcSample2.lib" 
DEF_FILE= \
	".\MarcSample2.def"
LINK32_OBJS= \
	"$(INTDIR)\MarcSample2.obj" \
	"$(INTDIR)\MarcSampleDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MarcSample2.res"

"$(OUTDIR)\MarcSample2.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MarcSample2 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\..\..\dll\common\MarcSample2.dll"


CLEAN :
	-@erase "$(INTDIR)\MarcSample2.obj"
	-@erase "$(INTDIR)\MarcSample2.pch"
	-@erase "$(INTDIR)\MarcSample2.res"
	-@erase "$(INTDIR)\MarcSampleDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\MarcSample2.exp"
	-@erase "$(OUTDIR)\MarcSample2.lib"
	-@erase "$(OUTDIR)\MarcSample2.pdb"
	-@erase "..\..\..\..\..\dll\common\MarcSample2.dll"
	-@erase "..\..\..\..\..\dll\common\MarcSample2.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcSample2.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MarcSample2.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MarcSample2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=esl.lib MarcToolKit.lib FileImport.lib BO_SPECIES_API.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\MarcSample2.pdb" /debug /machine:I386 /def:".\MarcSample2.def" /out:"..\..\..\..\..\dll\common\MarcSample2.dll" /implib:"$(OUTDIR)\MarcSample2.lib" /pdbtype:sept 
DEF_FILE= \
	".\MarcSample2.def"
LINK32_OBJS= \
	"$(INTDIR)\MarcSample2.obj" \
	"$(INTDIR)\MarcSampleDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MarcSample2.res"

"..\..\..\..\..\dll\common\MarcSample2.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("MarcSample2.dep")
!INCLUDE "MarcSample2.dep"
!ELSE 
!MESSAGE Warning: cannot find "MarcSample2.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MarcSample2 - Win32 Release" || "$(CFG)" == "MarcSample2 - Win32 Debug"
SOURCE=.\MarcSample2.cpp

"$(INTDIR)\MarcSample2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcSample2.pch"


SOURCE=.\MarcSample2.rc

"$(INTDIR)\MarcSample2.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\MarcSampleDlg.cpp

"$(INTDIR)\MarcSampleDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcSample2.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "MarcSample2 - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcSample2.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MarcSample2.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "MarcSample2 - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcSample2.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MarcSample2.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

