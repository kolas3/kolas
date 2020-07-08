# Microsoft Developer Studio Generated NMAKE File, Based on BO_CAT_PROC_FIXEDFIELD.dsp
!IF "$(CFG)" == ""
CFG=BO_CAT_PROC_FIXEDFIELD - Win32 Debug
!MESSAGE No configuration specified. Defaulting to BO_CAT_PROC_FIXEDFIELD - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "BO_CAT_PROC_FIXEDFIELD - Win32 Release" && "$(CFG)" != "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_CAT_PROC_FIXEDFIELD.mak" CFG="BO_CAT_PROC_FIXEDFIELD - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_CAT_PROC_FIXEDFIELD - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BO_CAT_PROC_FIXEDFIELD - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.dll"


CLEAN :
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_01.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_01.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_02.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_03.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_04.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_05.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_06.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_07.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_08.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_09.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_10.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_11.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_12.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0321.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0322.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0323.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0324.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0325.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"
	-@erase "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.dll"
	-@erase "$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.exp"
	-@erase "$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.pdb" /machine:I386 /def:".\BO_CAT_PROC_FIXEDFIELD.def" /out:"$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.dll" /implib:"$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.lib" 
DEF_FILE= \
	".\BO_CAT_PROC_FIXEDFIELD.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_CAT_PROC_0320_01.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_01.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_02.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_03.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_04.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_05.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_06.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_07.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_08.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_09.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_10.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_11.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_12.obj" \
	"$(INTDIR)\BO_CAT_PROC_0321.obj" \
	"$(INTDIR)\BO_CAT_PROC_0322.obj" \
	"$(INTDIR)\BO_CAT_PROC_0323.obj" \
	"$(INTDIR)\BO_CAT_PROC_0324.obj" \
	"$(INTDIR)\BO_CAT_PROC_0325.obj" \
	"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.res"

"$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"

OUTDIR=.\..\..\..\..\lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..\..\..\..\lib
# End Custom Macros

ALL : "..\..\..\..\..\dll\MONO\BO_CAT_PROC_FIXEDFIELD.dll" "$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.bsc"


CLEAN :
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_01.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_01.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_01.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_01.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_02.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_02.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_03.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_03.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_04.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_04.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_05.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_05.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_06.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_06.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_07.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_07.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_08.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_08.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_09.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_09.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_10.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_10.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_11.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_11.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_12.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0320_FORM_12.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0321.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0321.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0322.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0322.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0323.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0323.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0324.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0324.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0325.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0325.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"
	-@erase "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.res"
	-@erase "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.bsc"
	-@erase "$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.exp"
	-@erase "$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.lib"
	-@erase "$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.pdb"
	-@erase "..\..\..\..\..\dll\MONO\BO_CAT_PROC_FIXEDFIELD.dll"
	-@erase "..\..\..\..\..\dll\MONO\BO_CAT_PROC_FIXEDFIELD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BO_CAT_PROC_0320_01.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_01.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_02.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_03.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_04.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_05.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_06.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_07.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_08.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_09.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_10.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_11.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_12.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0321.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0322.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0323.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0324.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0325.sbr" \
	"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=esl.lib marcedit.lib K2UP_Common.LIB IndexToolKit.lib KOMarcGetter.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.pdb" /debug /machine:I386 /def:".\BO_CAT_PROC_FIXEDFIELD.def" /out:"..\..\..\..\..\dll\MONO\BO_CAT_PROC_FIXEDFIELD.dll" /implib:"$(OUTDIR)\BO_CAT_PROC_FIXEDFIELD.lib" /pdbtype:sept 
DEF_FILE= \
	".\BO_CAT_PROC_FIXEDFIELD.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_CAT_PROC_0320_01.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_01.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_02.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_03.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_04.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_05.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_06.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_07.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_08.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_09.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_10.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_11.obj" \
	"$(INTDIR)\BO_CAT_PROC_0320_FORM_12.obj" \
	"$(INTDIR)\BO_CAT_PROC_0321.obj" \
	"$(INTDIR)\BO_CAT_PROC_0322.obj" \
	"$(INTDIR)\BO_CAT_PROC_0323.obj" \
	"$(INTDIR)\BO_CAT_PROC_0324.obj" \
	"$(INTDIR)\BO_CAT_PROC_0325.obj" \
	"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.res"

"..\..\..\..\..\dll\MONO\BO_CAT_PROC_FIXEDFIELD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("BO_CAT_PROC_FIXEDFIELD.dep")
!INCLUDE "BO_CAT_PROC_FIXEDFIELD.dep"
!ELSE 
!MESSAGE Warning: cannot find "BO_CAT_PROC_FIXEDFIELD.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release" || "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"
SOURCE=.\BO_CAT_PROC_0320_01.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_01.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_01.obj"	"$(INTDIR)\BO_CAT_PROC_0320_01.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0320_FORM.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_FORM.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_FORM.obj"	"$(INTDIR)\BO_CAT_PROC_0320_FORM.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0320_FORM_01.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_01.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_01.obj"	"$(INTDIR)\BO_CAT_PROC_0320_FORM_01.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0320_FORM_02.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_02.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_02.obj"	"$(INTDIR)\BO_CAT_PROC_0320_FORM_02.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0320_FORM_03.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_03.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_03.obj"	"$(INTDIR)\BO_CAT_PROC_0320_FORM_03.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0320_FORM_04.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_04.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_04.obj"	"$(INTDIR)\BO_CAT_PROC_0320_FORM_04.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0320_FORM_05.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_05.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_05.obj"	"$(INTDIR)\BO_CAT_PROC_0320_FORM_05.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0320_FORM_06.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_06.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_06.obj"	"$(INTDIR)\BO_CAT_PROC_0320_FORM_06.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0320_FORM_07.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_07.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_07.obj"	"$(INTDIR)\BO_CAT_PROC_0320_FORM_07.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0320_FORM_08.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_08.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_08.obj"	"$(INTDIR)\BO_CAT_PROC_0320_FORM_08.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0320_FORM_09.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_09.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_09.obj"	"$(INTDIR)\BO_CAT_PROC_0320_FORM_09.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0320_FORM_10.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_10.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_10.obj"	"$(INTDIR)\BO_CAT_PROC_0320_FORM_10.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0320_FORM_11.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_11.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_11.obj"	"$(INTDIR)\BO_CAT_PROC_0320_FORM_11.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0320_FORM_12.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_12.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0320_FORM_12.obj"	"$(INTDIR)\BO_CAT_PROC_0320_FORM_12.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0321.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0321.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0321.obj"	"$(INTDIR)\BO_CAT_PROC_0321.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0322.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0322.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0322.obj"	"$(INTDIR)\BO_CAT_PROC_0322.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0323.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0323.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0323.obj"	"$(INTDIR)\BO_CAT_PROC_0323.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0324.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0324.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0324.obj"	"$(INTDIR)\BO_CAT_PROC_0324.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0325.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0325.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0325.obj"	"$(INTDIR)\BO_CAT_PROC_0325.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_FIXEDFIELD.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.obj"	"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_FIXEDFIELD.rc

"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "BO_CAT_PROC_FIXEDFIELD - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\BO_CAT_PROC_FIXEDFIELD.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

