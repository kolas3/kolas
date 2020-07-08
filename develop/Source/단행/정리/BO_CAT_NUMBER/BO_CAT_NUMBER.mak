# Microsoft Developer Studio Generated NMAKE File, Based on BO_CAT_NUMBER.dsp
!IF "$(CFG)" == ""
CFG=BO_CAT_NUMBER - Win32 Debug
!MESSAGE No configuration specified. Defaulting to BO_CAT_NUMBER - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "BO_CAT_NUMBER - Win32 Release" && "$(CFG)" != "BO_CAT_NUMBER - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_CAT_NUMBER.mak" CFG="BO_CAT_NUMBER - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_CAT_NUMBER - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BO_CAT_NUMBER - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\BO_CAT_NUMBER.dll"


CLEAN :
	-@erase "$(INTDIR)\BO_CAT_ACQUISIT_NO.obj"
	-@erase "$(INTDIR)\BO_CAT_ACQUISIT_NO_0100.obj"
	-@erase "$(INTDIR)\BO_CAT_ACQUISIT_NO_0200.obj"
	-@erase "$(INTDIR)\BO_CAT_NUMBER.obj"
	-@erase "$(INTDIR)\BO_CAT_NUMBER.pch"
	-@erase "$(INTDIR)\BO_CAT_NUMBER.res"
	-@erase "$(INTDIR)\BO_CAT_NUMBER_0000.obj"
	-@erase "$(INTDIR)\BO_CAT_NUMBER_0100.obj"
	-@erase "$(INTDIR)\BO_CAT_NUMBER_0200.obj"
	-@erase "$(INTDIR)\BO_CAT_NUMBER_0300.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\BO_CAT_NUMBER.dll"
	-@erase "$(OUTDIR)\BO_CAT_NUMBER.exp"
	-@erase "$(OUTDIR)\BO_CAT_NUMBER.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_NUMBER.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_CAT_NUMBER.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_CAT_NUMBER.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\BO_CAT_NUMBER.pdb" /machine:I386 /def:".\BO_CAT_NUMBER.def" /out:"$(OUTDIR)\BO_CAT_NUMBER.dll" /implib:"$(OUTDIR)\BO_CAT_NUMBER.lib" 
DEF_FILE= \
	".\BO_CAT_NUMBER.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_CAT_ACQUISIT_NO.obj" \
	"$(INTDIR)\BO_CAT_ACQUISIT_NO_0100.obj" \
	"$(INTDIR)\BO_CAT_ACQUISIT_NO_0200.obj" \
	"$(INTDIR)\BO_CAT_NUMBER.obj" \
	"$(INTDIR)\BO_CAT_NUMBER_0000.obj" \
	"$(INTDIR)\BO_CAT_NUMBER_0100.obj" \
	"$(INTDIR)\BO_CAT_NUMBER_0200.obj" \
	"$(INTDIR)\BO_CAT_NUMBER_0300.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_CAT_NUMBER.res"

"$(OUTDIR)\BO_CAT_NUMBER.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Debug"

OUTDIR=.\..\..\..\..\lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..\..\..\..\lib
# End Custom Macros

ALL : "..\..\..\..\..\dll\MONO\BO_CAT_NUMBER.dll" "$(OUTDIR)\BO_CAT_NUMBER.bsc"


CLEAN :
	-@erase "$(INTDIR)\BO_CAT_ACQUISIT_NO.obj"
	-@erase "$(INTDIR)\BO_CAT_ACQUISIT_NO.sbr"
	-@erase "$(INTDIR)\BO_CAT_ACQUISIT_NO_0100.obj"
	-@erase "$(INTDIR)\BO_CAT_ACQUISIT_NO_0100.sbr"
	-@erase "$(INTDIR)\BO_CAT_ACQUISIT_NO_0200.obj"
	-@erase "$(INTDIR)\BO_CAT_ACQUISIT_NO_0200.sbr"
	-@erase "$(INTDIR)\BO_CAT_NUMBER.obj"
	-@erase "$(INTDIR)\BO_CAT_NUMBER.pch"
	-@erase "$(INTDIR)\BO_CAT_NUMBER.res"
	-@erase "$(INTDIR)\BO_CAT_NUMBER.sbr"
	-@erase "$(INTDIR)\BO_CAT_NUMBER_0000.obj"
	-@erase "$(INTDIR)\BO_CAT_NUMBER_0000.sbr"
	-@erase "$(INTDIR)\BO_CAT_NUMBER_0100.obj"
	-@erase "$(INTDIR)\BO_CAT_NUMBER_0100.sbr"
	-@erase "$(INTDIR)\BO_CAT_NUMBER_0200.obj"
	-@erase "$(INTDIR)\BO_CAT_NUMBER_0200.sbr"
	-@erase "$(INTDIR)\BO_CAT_NUMBER_0300.obj"
	-@erase "$(INTDIR)\BO_CAT_NUMBER_0300.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\BO_CAT_NUMBER.bsc"
	-@erase "$(OUTDIR)\BO_CAT_NUMBER.exp"
	-@erase "$(OUTDIR)\BO_CAT_NUMBER.lib"
	-@erase "$(OUTDIR)\BO_CAT_NUMBER.pdb"
	-@erase "..\..\..\..\..\dll\MONO\BO_CAT_NUMBER.dll"
	-@erase "..\..\..\..\..\dll\MONO\BO_CAT_NUMBER.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\BO_CAT_NUMBER.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_CAT_NUMBER.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_CAT_NUMBER.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BO_CAT_ACQUISIT_NO.sbr" \
	"$(INTDIR)\BO_CAT_ACQUISIT_NO_0100.sbr" \
	"$(INTDIR)\BO_CAT_ACQUISIT_NO_0200.sbr" \
	"$(INTDIR)\BO_CAT_NUMBER.sbr" \
	"$(INTDIR)\BO_CAT_NUMBER_0000.sbr" \
	"$(INTDIR)\BO_CAT_NUMBER_0100.sbr" \
	"$(INTDIR)\BO_CAT_NUMBER_0200.sbr" \
	"$(INTDIR)\BO_CAT_NUMBER_0300.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\BO_CAT_NUMBER.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=esl.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\BO_CAT_NUMBER.pdb" /debug /machine:I386 /def:".\BO_CAT_NUMBER.def" /out:"..\..\..\..\..\dll\MONO\BO_CAT_NUMBER.dll" /implib:"$(OUTDIR)\BO_CAT_NUMBER.lib" /pdbtype:sept 
DEF_FILE= \
	".\BO_CAT_NUMBER.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_CAT_ACQUISIT_NO.obj" \
	"$(INTDIR)\BO_CAT_ACQUISIT_NO_0100.obj" \
	"$(INTDIR)\BO_CAT_ACQUISIT_NO_0200.obj" \
	"$(INTDIR)\BO_CAT_NUMBER.obj" \
	"$(INTDIR)\BO_CAT_NUMBER_0000.obj" \
	"$(INTDIR)\BO_CAT_NUMBER_0100.obj" \
	"$(INTDIR)\BO_CAT_NUMBER_0200.obj" \
	"$(INTDIR)\BO_CAT_NUMBER_0300.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_CAT_NUMBER.res"

"..\..\..\..\..\dll\MONO\BO_CAT_NUMBER.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("BO_CAT_NUMBER.dep")
!INCLUDE "BO_CAT_NUMBER.dep"
!ELSE 
!MESSAGE Warning: cannot find "BO_CAT_NUMBER.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "BO_CAT_NUMBER - Win32 Release" || "$(CFG)" == "BO_CAT_NUMBER - Win32 Debug"
SOURCE=.\BO_CAT_ACQUISIT_NO.cpp

!IF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Release"


"$(INTDIR)\BO_CAT_ACQUISIT_NO.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Debug"


"$(INTDIR)\BO_CAT_ACQUISIT_NO.obj"	"$(INTDIR)\BO_CAT_ACQUISIT_NO.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ENDIF 

SOURCE=.\BO_CAT_ACQUISIT_NO_0100.cpp

!IF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Release"


"$(INTDIR)\BO_CAT_ACQUISIT_NO_0100.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Debug"


"$(INTDIR)\BO_CAT_ACQUISIT_NO_0100.obj"	"$(INTDIR)\BO_CAT_ACQUISIT_NO_0100.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ENDIF 

SOURCE=.\BO_CAT_ACQUISIT_NO_0200.cpp

!IF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Release"


"$(INTDIR)\BO_CAT_ACQUISIT_NO_0200.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Debug"


"$(INTDIR)\BO_CAT_ACQUISIT_NO_0200.obj"	"$(INTDIR)\BO_CAT_ACQUISIT_NO_0200.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ENDIF 

SOURCE=.\BO_CAT_NUMBER.cpp

!IF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Release"


"$(INTDIR)\BO_CAT_NUMBER.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Debug"


"$(INTDIR)\BO_CAT_NUMBER.obj"	"$(INTDIR)\BO_CAT_NUMBER.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ENDIF 

SOURCE=.\BO_CAT_NUMBER.rc

"$(INTDIR)\BO_CAT_NUMBER.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\BO_CAT_NUMBER_0000.cpp

!IF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Release"


"$(INTDIR)\BO_CAT_NUMBER_0000.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Debug"


"$(INTDIR)\BO_CAT_NUMBER_0000.obj"	"$(INTDIR)\BO_CAT_NUMBER_0000.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ENDIF 

SOURCE=.\BO_CAT_NUMBER_0100.cpp

!IF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Release"


"$(INTDIR)\BO_CAT_NUMBER_0100.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Debug"


"$(INTDIR)\BO_CAT_NUMBER_0100.obj"	"$(INTDIR)\BO_CAT_NUMBER_0100.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ENDIF 

SOURCE=.\BO_CAT_NUMBER_0200.cpp

!IF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Release"


"$(INTDIR)\BO_CAT_NUMBER_0200.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Debug"


"$(INTDIR)\BO_CAT_NUMBER_0200.obj"	"$(INTDIR)\BO_CAT_NUMBER_0200.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ENDIF 

SOURCE=.\BO_CAT_NUMBER_0300.cpp

!IF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Release"


"$(INTDIR)\BO_CAT_NUMBER_0300.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Debug"


"$(INTDIR)\BO_CAT_NUMBER_0300.obj"	"$(INTDIR)\BO_CAT_NUMBER_0300.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_NUMBER.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_NUMBER.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\BO_CAT_NUMBER.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "BO_CAT_NUMBER - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\BO_CAT_NUMBER.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\BO_CAT_NUMBER.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

