# Microsoft Developer Studio Generated NMAKE File, Based on SE_VOLUME_FORM.dsp
!IF "$(CFG)" == ""
CFG=SE_VOLUME_FORM - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SE_VOLUME_FORM - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SE_VOLUME_FORM - Win32 Release" && "$(CFG)" != "SE_VOLUME_FORM - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SE_VOLUME_FORM.mak" CFG="SE_VOLUME_FORM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SE_VOLUME_FORM - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SE_VOLUME_FORM - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "SE_VOLUME_FORM - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\SE_VOLUME_FORM.dll"


CLEAN :
	-@erase "$(INTDIR)\SE_VOLUME_FORM.obj"
	-@erase "$(INTDIR)\SE_VOLUME_FORM.pch"
	-@erase "$(INTDIR)\SE_VOLUME_FORM.res"
	-@erase "$(INTDIR)\SEVolumeDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SE_VOLUME_FORM.dll"
	-@erase "$(OUTDIR)\SE_VOLUME_FORM.exp"
	-@erase "$(OUTDIR)\SE_VOLUME_FORM.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\SE_VOLUME_FORM.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SE_VOLUME_FORM.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SE_VOLUME_FORM.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\SE_VOLUME_FORM.pdb" /machine:I386 /def:".\SE_VOLUME_FORM.def" /out:"$(OUTDIR)\SE_VOLUME_FORM.dll" /implib:"$(OUTDIR)\SE_VOLUME_FORM.lib" 
DEF_FILE= \
	".\SE_VOLUME_FORM.def"
LINK32_OBJS= \
	"$(INTDIR)\SE_VOLUME_FORM.obj" \
	"$(INTDIR)\SEVolumeDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SE_VOLUME_FORM.res"

"$(OUTDIR)\SE_VOLUME_FORM.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SE_VOLUME_FORM - Win32 Debug"

OUTDIR=.\..\..\..\..\lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..\..\..\..\lib
# End Custom Macros

ALL : "..\..\..\..\..\dll\serial\SE_VOLUME_FORM.dll" "$(OUTDIR)\SE_VOLUME_FORM.bsc"


CLEAN :
	-@erase "$(INTDIR)\SE_VOLUME_FORM.obj"
	-@erase "$(INTDIR)\SE_VOLUME_FORM.pch"
	-@erase "$(INTDIR)\SE_VOLUME_FORM.res"
	-@erase "$(INTDIR)\SE_VOLUME_FORM.sbr"
	-@erase "$(INTDIR)\SEVolumeDlg.obj"
	-@erase "$(INTDIR)\SEVolumeDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SE_VOLUME_FORM.bsc"
	-@erase "$(OUTDIR)\SE_VOLUME_FORM.exp"
	-@erase "$(OUTDIR)\SE_VOLUME_FORM.lib"
	-@erase "$(OUTDIR)\SE_VOLUME_FORM.pdb"
	-@erase "..\..\..\..\..\dll\serial\SE_VOLUME_FORM.dll"
	-@erase "..\..\..\..\..\dll\serial\SE_VOLUME_FORM.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SE_VOLUME_FORM.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SE_VOLUME_FORM.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SE_VOLUME_FORM.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\SE_VOLUME_FORM.sbr" \
	"$(INTDIR)\SEVolumeDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\SE_VOLUME_FORM.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=esl.lib MPropertyGrid.lib MarcAdjustApi.lib IndexToolKit.lib ContentsMgr_Editor.lib Kolas2up_commonControl.lib TOCEDITOR.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\SE_VOLUME_FORM.pdb" /debug /machine:I386 /def:".\SE_VOLUME_FORM.def" /out:"..\..\..\..\..\dll\serial\SE_VOLUME_FORM.dll" /implib:"$(OUTDIR)\SE_VOLUME_FORM.lib" /pdbtype:sept 
DEF_FILE= \
	".\SE_VOLUME_FORM.def"
LINK32_OBJS= \
	"$(INTDIR)\SE_VOLUME_FORM.obj" \
	"$(INTDIR)\SEVolumeDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SE_VOLUME_FORM.res"

"..\..\..\..\..\dll\serial\SE_VOLUME_FORM.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("SE_VOLUME_FORM.dep")
!INCLUDE "SE_VOLUME_FORM.dep"
!ELSE 
!MESSAGE Warning: cannot find "SE_VOLUME_FORM.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SE_VOLUME_FORM - Win32 Release" || "$(CFG)" == "SE_VOLUME_FORM - Win32 Debug"
SOURCE=.\SE_VOLUME_FORM.cpp

!IF  "$(CFG)" == "SE_VOLUME_FORM - Win32 Release"


"$(INTDIR)\SE_VOLUME_FORM.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SE_VOLUME_FORM.pch"


!ELSEIF  "$(CFG)" == "SE_VOLUME_FORM - Win32 Debug"


"$(INTDIR)\SE_VOLUME_FORM.obj"	"$(INTDIR)\SE_VOLUME_FORM.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SE_VOLUME_FORM.pch"


!ENDIF 

SOURCE=.\SE_VOLUME_FORM.rc

"$(INTDIR)\SE_VOLUME_FORM.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\SEVolumeDlg.cpp

!IF  "$(CFG)" == "SE_VOLUME_FORM - Win32 Release"


"$(INTDIR)\SEVolumeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SE_VOLUME_FORM.pch"


!ELSEIF  "$(CFG)" == "SE_VOLUME_FORM - Win32 Debug"


"$(INTDIR)\SEVolumeDlg.obj"	"$(INTDIR)\SEVolumeDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SE_VOLUME_FORM.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "SE_VOLUME_FORM - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\SE_VOLUME_FORM.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SE_VOLUME_FORM.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SE_VOLUME_FORM - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SE_VOLUME_FORM.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\SE_VOLUME_FORM.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

