# Microsoft Developer Studio Generated NMAKE File, Based on MPropertyGrid.dsp
!IF "$(CFG)" == ""
CFG=MPropertyGrid - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MPropertyGrid - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MPropertyGrid - Win32 Release" && "$(CFG)" != "MPropertyGrid - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MPropertyGrid.mak" CFG="MPropertyGrid - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MPropertyGrid - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MPropertyGrid - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "MPropertyGrid - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MPropertyGrid.dll"


CLEAN :
	-@erase "$(INTDIR)\MPropertyComboBox.obj"
	-@erase "$(INTDIR)\MPropertyDateTimeCtrl.obj"
	-@erase "$(INTDIR)\MPropertyEdit.obj"
	-@erase "$(INTDIR)\MPropertyGrid.obj"
	-@erase "$(INTDIR)\MPropertyGrid.pch"
	-@erase "$(INTDIR)\MPropertyGrid.res"
	-@erase "$(INTDIR)\MPropertyGridCtrl.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MPropertyGrid.dll"
	-@erase "$(OUTDIR)\MPropertyGrid.exp"
	-@erase "$(OUTDIR)\MPropertyGrid.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MPropertyGrid.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MPropertyGrid.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MPropertyGrid.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\MPropertyGrid.pdb" /machine:I386 /def:".\MPropertyGrid.def" /out:"$(OUTDIR)\MPropertyGrid.dll" /implib:"$(OUTDIR)\MPropertyGrid.lib" 
DEF_FILE= \
	".\MPropertyGrid.def"
LINK32_OBJS= \
	"$(INTDIR)\MPropertyComboBox.obj" \
	"$(INTDIR)\MPropertyEdit.obj" \
	"$(INTDIR)\MPropertyGrid.obj" \
	"$(INTDIR)\MPropertyGridCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MPropertyGrid.res" \
	"$(INTDIR)\MPropertyDateTimeCtrl.obj"

"$(OUTDIR)\MPropertyGrid.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MPropertyGrid - Win32 Debug"

OUTDIR=.\..\..\..\..\lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..\..\..\..\lib
# End Custom Macros

ALL : "..\..\..\..\..\dll\COMMON\MPropertyGrid.dll" "$(OUTDIR)\MPropertyGrid.bsc"


CLEAN :
	-@erase "$(INTDIR)\MPropertyComboBox.obj"
	-@erase "$(INTDIR)\MPropertyComboBox.sbr"
	-@erase "$(INTDIR)\MPropertyDateTimeCtrl.obj"
	-@erase "$(INTDIR)\MPropertyDateTimeCtrl.sbr"
	-@erase "$(INTDIR)\MPropertyEdit.obj"
	-@erase "$(INTDIR)\MPropertyEdit.sbr"
	-@erase "$(INTDIR)\MPropertyGrid.obj"
	-@erase "$(INTDIR)\MPropertyGrid.pch"
	-@erase "$(INTDIR)\MPropertyGrid.res"
	-@erase "$(INTDIR)\MPropertyGrid.sbr"
	-@erase "$(INTDIR)\MPropertyGridCtrl.obj"
	-@erase "$(INTDIR)\MPropertyGridCtrl.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\MPropertyGrid.bsc"
	-@erase "$(OUTDIR)\MPropertyGrid.exp"
	-@erase "$(OUTDIR)\MPropertyGrid.lib"
	-@erase "$(OUTDIR)\MPropertyGrid.pdb"
	-@erase "..\..\..\..\..\dll\COMMON\MPropertyGrid.dll"
	-@erase "..\..\..\..\..\dll\COMMON\MPropertyGrid.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\MPropertyGrid.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MPropertyGrid.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MPropertyGrid.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\MPropertyComboBox.sbr" \
	"$(INTDIR)\MPropertyEdit.sbr" \
	"$(INTDIR)\MPropertyGrid.sbr" \
	"$(INTDIR)\MPropertyGridCtrl.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\MPropertyDateTimeCtrl.sbr"

"$(OUTDIR)\MPropertyGrid.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=esl.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\MPropertyGrid.pdb" /debug /machine:I386 /def:".\MPropertyGrid.def" /out:"..\..\..\..\..\dll\COMMON\MPropertyGrid.dll" /implib:"$(OUTDIR)\MPropertyGrid.lib" /pdbtype:sept 
DEF_FILE= \
	".\MPropertyGrid.def"
LINK32_OBJS= \
	"$(INTDIR)\MPropertyComboBox.obj" \
	"$(INTDIR)\MPropertyEdit.obj" \
	"$(INTDIR)\MPropertyGrid.obj" \
	"$(INTDIR)\MPropertyGridCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MPropertyGrid.res" \
	"$(INTDIR)\MPropertyDateTimeCtrl.obj"

"..\..\..\..\..\dll\COMMON\MPropertyGrid.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("MPropertyGrid.dep")
!INCLUDE "MPropertyGrid.dep"
!ELSE 
!MESSAGE Warning: cannot find "MPropertyGrid.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MPropertyGrid - Win32 Release" || "$(CFG)" == "MPropertyGrid - Win32 Debug"
SOURCE=.\MPropertyComboBox.cpp

!IF  "$(CFG)" == "MPropertyGrid - Win32 Release"


"$(INTDIR)\MPropertyComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MPropertyGrid.pch"


!ELSEIF  "$(CFG)" == "MPropertyGrid - Win32 Debug"


"$(INTDIR)\MPropertyComboBox.obj"	"$(INTDIR)\MPropertyComboBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MPropertyGrid.pch"


!ENDIF 

SOURCE=.\MPropertyDateTimeCtrl.cpp

!IF  "$(CFG)" == "MPropertyGrid - Win32 Release"


"$(INTDIR)\MPropertyDateTimeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MPropertyGrid.pch"


!ELSEIF  "$(CFG)" == "MPropertyGrid - Win32 Debug"


"$(INTDIR)\MPropertyDateTimeCtrl.obj"	"$(INTDIR)\MPropertyDateTimeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MPropertyGrid.pch"


!ENDIF 

SOURCE=.\MPropertyEdit.cpp

!IF  "$(CFG)" == "MPropertyGrid - Win32 Release"


"$(INTDIR)\MPropertyEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MPropertyGrid.pch"


!ELSEIF  "$(CFG)" == "MPropertyGrid - Win32 Debug"


"$(INTDIR)\MPropertyEdit.obj"	"$(INTDIR)\MPropertyEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MPropertyGrid.pch"


!ENDIF 

SOURCE=.\MPropertyGrid.cpp

!IF  "$(CFG)" == "MPropertyGrid - Win32 Release"


"$(INTDIR)\MPropertyGrid.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MPropertyGrid.pch"


!ELSEIF  "$(CFG)" == "MPropertyGrid - Win32 Debug"


"$(INTDIR)\MPropertyGrid.obj"	"$(INTDIR)\MPropertyGrid.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MPropertyGrid.pch"


!ENDIF 

SOURCE=.\MPropertyGrid.rc

"$(INTDIR)\MPropertyGrid.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\MPropertyGridCtrl.cpp

!IF  "$(CFG)" == "MPropertyGrid - Win32 Release"


"$(INTDIR)\MPropertyGridCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MPropertyGrid.pch"


!ELSEIF  "$(CFG)" == "MPropertyGrid - Win32 Debug"


"$(INTDIR)\MPropertyGridCtrl.obj"	"$(INTDIR)\MPropertyGridCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MPropertyGrid.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "MPropertyGrid - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MPropertyGrid.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MPropertyGrid.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "MPropertyGrid - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\MPropertyGrid.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\MPropertyGrid.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

