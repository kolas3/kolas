# Microsoft Developer Studio Generated NMAKE File, Based on MarcEdit.dsp
!IF "$(CFG)" == ""
CFG=MarcEdit - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MarcEdit - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MarcEdit - Win32 Release" && "$(CFG)" != "MarcEdit - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MarcEdit.mak" CFG="MarcEdit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MarcEdit - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MarcEdit - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "MarcEdit - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MarcEdit.dll"


CLEAN :
	-@erase "$(INTDIR)\ColorPicker.obj"
	-@erase "$(INTDIR)\ColorPopup.obj"
	-@erase "$(INTDIR)\DlgMarcSyntaxError.obj"
	-@erase "$(INTDIR)\DlgSymbolColor.obj"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\GridCombo.obj"
	-@erase "$(INTDIR)\GridEdit.obj"
	-@erase "$(INTDIR)\MarcEdit.obj"
	-@erase "$(INTDIR)\MarcEdit.pch"
	-@erase "$(INTDIR)\MarcEdit.res"
	-@erase "$(INTDIR)\MarcEditCtrl.obj"
	-@erase "$(INTDIR)\MarcEditor.obj"
	-@erase "$(INTDIR)\MarcGridCtrl.obj"
	-@erase "$(INTDIR)\mshflexgrid.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MarcEdit.dll"
	-@erase "$(OUTDIR)\MarcEdit.exp"
	-@erase "$(OUTDIR)\MarcEdit.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcEdit.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MarcEdit.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MarcEdit.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\MarcEdit.pdb" /machine:I386 /def:".\MarcEdit.def" /out:"$(OUTDIR)\MarcEdit.dll" /implib:"$(OUTDIR)\MarcEdit.lib" 
DEF_FILE= \
	".\MarcEdit.def"
LINK32_OBJS= \
	"$(INTDIR)\ColorPicker.obj" \
	"$(INTDIR)\ColorPopup.obj" \
	"$(INTDIR)\DlgMarcSyntaxError.obj" \
	"$(INTDIR)\DlgSymbolColor.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\GridCombo.obj" \
	"$(INTDIR)\GridEdit.obj" \
	"$(INTDIR)\MarcEdit.obj" \
	"$(INTDIR)\MarcEditCtrl.obj" \
	"$(INTDIR)\MarcEditor.obj" \
	"$(INTDIR)\MarcGridCtrl.obj" \
	"$(INTDIR)\mshflexgrid.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MarcEdit.res"

"$(OUTDIR)\MarcEdit.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MarcEdit - Win32 Debug"

OUTDIR=.\..\..\..\..\lib
INTDIR=.\Debug

ALL : "..\..\..\..\..\dll\common\MarcEdit.dll"


CLEAN :
	-@erase "$(INTDIR)\ColorPicker.obj"
	-@erase "$(INTDIR)\ColorPopup.obj"
	-@erase "$(INTDIR)\DlgMarcSyntaxError.obj"
	-@erase "$(INTDIR)\DlgSymbolColor.obj"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\GridCombo.obj"
	-@erase "$(INTDIR)\GridEdit.obj"
	-@erase "$(INTDIR)\MarcEdit.obj"
	-@erase "$(INTDIR)\MarcEdit.pch"
	-@erase "$(INTDIR)\MarcEdit.res"
	-@erase "$(INTDIR)\MarcEditCtrl.obj"
	-@erase "$(INTDIR)\MarcEditor.obj"
	-@erase "$(INTDIR)\MarcGridCtrl.obj"
	-@erase "$(INTDIR)\mshflexgrid.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\MarcEdit.exp"
	-@erase "$(OUTDIR)\MarcEdit.lib"
	-@erase "$(OUTDIR)\MarcEdit.pdb"
	-@erase "..\..\..\..\..\dll\common\MarcEdit.dll"
	-@erase "..\..\..\..\..\dll\common\MarcEdit.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcEdit.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MarcEdit.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MarcEdit.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ACMarcGetter.lib ESL.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\MarcEdit.pdb" /debug /machine:I386 /def:".\MarcEdit.def" /out:"..\..\..\..\..\dll\common\MarcEdit.dll" /implib:"$(OUTDIR)\MarcEdit.lib" /pdbtype:sept 
DEF_FILE= \
	".\MarcEdit.def"
LINK32_OBJS= \
	"$(INTDIR)\ColorPicker.obj" \
	"$(INTDIR)\ColorPopup.obj" \
	"$(INTDIR)\DlgMarcSyntaxError.obj" \
	"$(INTDIR)\DlgSymbolColor.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\GridCombo.obj" \
	"$(INTDIR)\GridEdit.obj" \
	"$(INTDIR)\MarcEdit.obj" \
	"$(INTDIR)\MarcEditCtrl.obj" \
	"$(INTDIR)\MarcEditor.obj" \
	"$(INTDIR)\MarcGridCtrl.obj" \
	"$(INTDIR)\mshflexgrid.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MarcEdit.res"

"..\..\..\..\..\dll\common\MarcEdit.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("MarcEdit.dep")
!INCLUDE "MarcEdit.dep"
!ELSE 
!MESSAGE Warning: cannot find "MarcEdit.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MarcEdit - Win32 Release" || "$(CFG)" == "MarcEdit - Win32 Debug"
SOURCE=.\ColorPicker.cpp

"$(INTDIR)\ColorPicker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcEdit.pch"


SOURCE=.\ColorPopup.cpp

"$(INTDIR)\ColorPopup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcEdit.pch"


SOURCE=.\DlgMarcSyntaxError.cpp

"$(INTDIR)\DlgMarcSyntaxError.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcEdit.pch"


SOURCE=.\DlgSymbolColor.cpp

"$(INTDIR)\DlgSymbolColor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcEdit.pch"


SOURCE=.\font.cpp

"$(INTDIR)\font.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcEdit.pch"


SOURCE=.\GridCombo.cpp

"$(INTDIR)\GridCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcEdit.pch"


SOURCE=.\GridEdit.cpp

"$(INTDIR)\GridEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcEdit.pch"


SOURCE=.\MarcEdit.cpp

"$(INTDIR)\MarcEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcEdit.pch"


SOURCE=.\MarcEdit.rc

"$(INTDIR)\MarcEdit.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\MarcEditCtrl.cpp

"$(INTDIR)\MarcEditCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcEdit.pch"


SOURCE=.\MarcEditor.cpp

"$(INTDIR)\MarcEditor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcEdit.pch"


SOURCE=.\MarcGridCtrl.cpp

"$(INTDIR)\MarcGridCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcEdit.pch"


SOURCE=.\mshflexgrid.cpp

"$(INTDIR)\mshflexgrid.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcEdit.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "MarcEdit - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcEdit.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MarcEdit.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "MarcEdit - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcEdit.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MarcEdit.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

