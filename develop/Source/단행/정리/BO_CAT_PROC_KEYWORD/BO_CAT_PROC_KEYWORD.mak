# Microsoft Developer Studio Generated NMAKE File, Based on BO_CAT_PROC_KEYWORD.dsp
!IF "$(CFG)" == ""
CFG=BO_CAT_PROC_KEYWORD - Win32 Debug
!MESSAGE No configuration specified. Defaulting to BO_CAT_PROC_KEYWORD - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "BO_CAT_PROC_KEYWORD - Win32 Release" && "$(CFG)" != "BO_CAT_PROC_KEYWORD - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_CAT_PROC_KEYWORD.mak" CFG="BO_CAT_PROC_KEYWORD - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_CAT_PROC_KEYWORD - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BO_CAT_PROC_KEYWORD - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "BO_CAT_PROC_KEYWORD - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\BO_CAT_PROC_KEYWORD.dll"


CLEAN :
	-@erase "$(INTDIR)\BO_CAT_AUXILIARY_WORD_0100.obj"
	-@erase "$(INTDIR)\BO_CAT_AUXILIARY_WORD_0200.obj"
	-@erase "$(INTDIR)\BO_CAT_AUXILIARY_WORD_0300.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD.pch"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD.res"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD_0000.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS_INPUT.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS_UPDATE.obj"
	-@erase "$(INTDIR)\BO_CAT_WORD_0100.obj"
	-@erase "$(INTDIR)\BO_CAT_WORD_0200.obj"
	-@erase "$(INTDIR)\BO_CAT_WORD_0300.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\BO_CAT_PROC_KEYWORD.dll"
	-@erase "$(OUTDIR)\BO_CAT_PROC_KEYWORD.exp"
	-@erase "$(OUTDIR)\BO_CAT_PROC_KEYWORD.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_PROC_KEYWORD.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_CAT_PROC_KEYWORD.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_CAT_PROC_KEYWORD.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\BO_CAT_PROC_KEYWORD.pdb" /machine:I386 /def:".\BO_CAT_PROC_KEYWORD.def" /out:"$(OUTDIR)\BO_CAT_PROC_KEYWORD.dll" /implib:"$(OUTDIR)\BO_CAT_PROC_KEYWORD.lib" 
DEF_FILE= \
	".\BO_CAT_PROC_KEYWORD.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_CAT_AUXILIARY_WORD_0100.obj" \
	"$(INTDIR)\BO_CAT_AUXILIARY_WORD_0200.obj" \
	"$(INTDIR)\BO_CAT_AUXILIARY_WORD_0300.obj" \
	"$(INTDIR)\BO_CAT_PROC_KEYWORD.obj" \
	"$(INTDIR)\BO_CAT_PROC_KEYWORD_0000.obj" \
	"$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS.obj" \
	"$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS_INPUT.obj" \
	"$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS_UPDATE.obj" \
	"$(INTDIR)\BO_CAT_WORD_0100.obj" \
	"$(INTDIR)\BO_CAT_WORD_0200.obj" \
	"$(INTDIR)\BO_CAT_WORD_0300.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_CAT_PROC_KEYWORD.res"

"$(OUTDIR)\BO_CAT_PROC_KEYWORD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "BO_CAT_PROC_KEYWORD - Win32 Debug"

OUTDIR=.\..\..\..\..\lib
INTDIR=.\Debug

ALL : "..\..\..\..\..\dll\MONO\BO_CAT_PROC_KEYWORD.dll"


CLEAN :
	-@erase "$(INTDIR)\BO_CAT_AUXILIARY_WORD_0100.obj"
	-@erase "$(INTDIR)\BO_CAT_AUXILIARY_WORD_0200.obj"
	-@erase "$(INTDIR)\BO_CAT_AUXILIARY_WORD_0300.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD.pch"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD.res"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD_0000.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS_INPUT.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS_UPDATE.obj"
	-@erase "$(INTDIR)\BO_CAT_WORD_0100.obj"
	-@erase "$(INTDIR)\BO_CAT_WORD_0200.obj"
	-@erase "$(INTDIR)\BO_CAT_WORD_0300.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\BO_CAT_PROC_KEYWORD.exp"
	-@erase "$(OUTDIR)\BO_CAT_PROC_KEYWORD.lib"
	-@erase "$(OUTDIR)\BO_CAT_PROC_KEYWORD.pdb"
	-@erase "..\..\..\..\..\dll\MONO\BO_CAT_PROC_KEYWORD.dll"
	-@erase "..\..\..\..\..\dll\MONO\BO_CAT_PROC_KEYWORD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_PROC_KEYWORD.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_CAT_PROC_KEYWORD.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_CAT_PROC_KEYWORD.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=esl.lib marcedit.lib K2UP_Common.LIB IndexToolKit.lib MngCharSetManagement.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\BO_CAT_PROC_KEYWORD.pdb" /debug /machine:I386 /def:".\BO_CAT_PROC_KEYWORD.def" /out:"..\..\..\..\..\dll\MONO\BO_CAT_PROC_KEYWORD.dll" /implib:"$(OUTDIR)\BO_CAT_PROC_KEYWORD.lib" /pdbtype:sept 
DEF_FILE= \
	".\BO_CAT_PROC_KEYWORD.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_CAT_AUXILIARY_WORD_0100.obj" \
	"$(INTDIR)\BO_CAT_AUXILIARY_WORD_0200.obj" \
	"$(INTDIR)\BO_CAT_AUXILIARY_WORD_0300.obj" \
	"$(INTDIR)\BO_CAT_PROC_KEYWORD.obj" \
	"$(INTDIR)\BO_CAT_PROC_KEYWORD_0000.obj" \
	"$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS.obj" \
	"$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS_INPUT.obj" \
	"$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS_UPDATE.obj" \
	"$(INTDIR)\BO_CAT_WORD_0100.obj" \
	"$(INTDIR)\BO_CAT_WORD_0200.obj" \
	"$(INTDIR)\BO_CAT_WORD_0300.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_CAT_PROC_KEYWORD.res"

"..\..\..\..\..\dll\MONO\BO_CAT_PROC_KEYWORD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("BO_CAT_PROC_KEYWORD.dep")
!INCLUDE "BO_CAT_PROC_KEYWORD.dep"
!ELSE 
!MESSAGE Warning: cannot find "BO_CAT_PROC_KEYWORD.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "BO_CAT_PROC_KEYWORD - Win32 Release" || "$(CFG)" == "BO_CAT_PROC_KEYWORD - Win32 Debug"
SOURCE=.\BO_CAT_AUXILIARY_WORD_0100.cpp

"$(INTDIR)\BO_CAT_AUXILIARY_WORD_0100.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_KEYWORD.pch"


SOURCE=.\BO_CAT_AUXILIARY_WORD_0200.cpp

"$(INTDIR)\BO_CAT_AUXILIARY_WORD_0200.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_KEYWORD.pch"


SOURCE=.\BO_CAT_AUXILIARY_WORD_0300.cpp

"$(INTDIR)\BO_CAT_AUXILIARY_WORD_0300.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_KEYWORD.pch"


SOURCE=.\BO_CAT_PROC_KEYWORD.cpp

"$(INTDIR)\BO_CAT_PROC_KEYWORD.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_KEYWORD.pch"


SOURCE=.\BO_CAT_PROC_KEYWORD.rc

"$(INTDIR)\BO_CAT_PROC_KEYWORD.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\BO_CAT_PROC_KEYWORD_0000.cpp

"$(INTDIR)\BO_CAT_PROC_KEYWORD_0000.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_KEYWORD.pch"


SOURCE=.\BO_CAT_PROC_KEYWORD_UNLESS.cpp

"$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_KEYWORD.pch"


SOURCE=.\BO_CAT_PROC_KEYWORD_UNLESS_INPUT.cpp

"$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS_INPUT.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_KEYWORD.pch"


SOURCE=.\BO_CAT_PROC_KEYWORD_UNLESS_UPDATE.cpp

"$(INTDIR)\BO_CAT_PROC_KEYWORD_UNLESS_UPDATE.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_KEYWORD.pch"


SOURCE=.\BO_CAT_WORD_0100.cpp

"$(INTDIR)\BO_CAT_WORD_0100.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_KEYWORD.pch"


SOURCE=.\BO_CAT_WORD_0200.cpp

"$(INTDIR)\BO_CAT_WORD_0200.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_KEYWORD.pch"


SOURCE=.\BO_CAT_WORD_0300.cpp

"$(INTDIR)\BO_CAT_WORD_0300.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_KEYWORD.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_KEYWORD - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_PROC_KEYWORD.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\BO_CAT_PROC_KEYWORD.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_KEYWORD - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_PROC_KEYWORD.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\BO_CAT_PROC_KEYWORD.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

