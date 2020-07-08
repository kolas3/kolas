# Microsoft Developer Studio Generated NMAKE File, Based on MarcToolKit.dsp
!IF "$(CFG)" == ""
CFG=MarcToolKit - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MarcToolKit - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MarcToolKit - Win32 Release" && "$(CFG)" != "MarcToolKit - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MarcToolKit.mak" CFG="MarcToolKit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MarcToolKit - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MarcToolKit - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "MarcToolKit - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MarcToolKit.dll"


CLEAN :
	-@erase "$(INTDIR)\ApplyMarcRelationDlg.obj"
	-@erase "$(INTDIR)\CreateAutorCode.obj"
	-@erase "$(INTDIR)\MarcAliasToFile.obj"
	-@erase "$(INTDIR)\MarcDefaultValueConfigDlg.obj"
	-@erase "$(INTDIR)\MarcImpExpMgr.obj"
	-@erase "$(INTDIR)\MarcRelationToText.obj"
	-@erase "$(INTDIR)\MarcToolKit.obj"
	-@erase "$(INTDIR)\MarcToolKit.pch"
	-@erase "$(INTDIR)\MarcToolKit.res"
	-@erase "$(INTDIR)\ReadCfgFile.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MarcToolKit.dll"
	-@erase "$(OUTDIR)\MarcToolKit.exp"
	-@erase "$(OUTDIR)\MarcToolKit.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcToolKit.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MarcToolKit.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MarcToolKit.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\MarcToolKit.pdb" /machine:I386 /def:".\MarcToolKit.def" /out:"$(OUTDIR)\MarcToolKit.dll" /implib:"$(OUTDIR)\MarcToolKit.lib" 
DEF_FILE= \
	".\MarcToolKit.def"
LINK32_OBJS= \
	"$(INTDIR)\ApplyMarcRelationDlg.obj" \
	"$(INTDIR)\CreateAutorCode.obj" \
	"$(INTDIR)\MarcAliasToFile.obj" \
	"$(INTDIR)\MarcDefaultValueConfigDlg.obj" \
	"$(INTDIR)\MarcImpExpMgr.obj" \
	"$(INTDIR)\MarcRelationToText.obj" \
	"$(INTDIR)\MarcToolKit.obj" \
	"$(INTDIR)\ReadCfgFile.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MarcToolKit.res"

"$(OUTDIR)\MarcToolKit.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MarcToolKit - Win32 Debug"

OUTDIR=.\..\..\..\..\lib
INTDIR=.\Debug

ALL : "..\..\..\..\..\dll\common\MarcToolKit.dll"


CLEAN :
	-@erase "$(INTDIR)\ApplyMarcRelationDlg.obj"
	-@erase "$(INTDIR)\CreateAutorCode.obj"
	-@erase "$(INTDIR)\MarcAliasToFile.obj"
	-@erase "$(INTDIR)\MarcDefaultValueConfigDlg.obj"
	-@erase "$(INTDIR)\MarcImpExpMgr.obj"
	-@erase "$(INTDIR)\MarcRelationToText.obj"
	-@erase "$(INTDIR)\MarcToolKit.obj"
	-@erase "$(INTDIR)\MarcToolKit.pch"
	-@erase "$(INTDIR)\MarcToolKit.res"
	-@erase "$(INTDIR)\ReadCfgFile.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\MarcToolKit.exp"
	-@erase "$(OUTDIR)\MarcToolKit.lib"
	-@erase "$(OUTDIR)\MarcToolKit.pdb"
	-@erase "..\..\..\..\..\dll\common\MarcToolKit.dll"
	-@erase "..\..\..\..\..\dll\common\MarcToolKit.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcToolKit.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MarcToolKit.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MarcToolKit.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=esl.lib MarcEdit.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\MarcToolKit.pdb" /debug /machine:I386 /def:".\MarcToolKit.def" /out:"..\..\..\..\..\dll\common\MarcToolKit.dll" /implib:"$(OUTDIR)\MarcToolKit.lib" /pdbtype:sept 
DEF_FILE= \
	".\MarcToolKit.def"
LINK32_OBJS= \
	"$(INTDIR)\ApplyMarcRelationDlg.obj" \
	"$(INTDIR)\CreateAutorCode.obj" \
	"$(INTDIR)\MarcAliasToFile.obj" \
	"$(INTDIR)\MarcDefaultValueConfigDlg.obj" \
	"$(INTDIR)\MarcImpExpMgr.obj" \
	"$(INTDIR)\MarcRelationToText.obj" \
	"$(INTDIR)\MarcToolKit.obj" \
	"$(INTDIR)\ReadCfgFile.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MarcToolKit.res"

"..\..\..\..\..\dll\common\MarcToolKit.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("MarcToolKit.dep")
!INCLUDE "MarcToolKit.dep"
!ELSE 
!MESSAGE Warning: cannot find "MarcToolKit.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MarcToolKit - Win32 Release" || "$(CFG)" == "MarcToolKit - Win32 Debug"
SOURCE=.\ApplyMarcRelationDlg.cpp

"$(INTDIR)\ApplyMarcRelationDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcToolKit.pch"


SOURCE=.\CreateAutorCode.cpp

"$(INTDIR)\CreateAutorCode.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcToolKit.pch"


SOURCE=.\MarcAliasToFile.cpp

"$(INTDIR)\MarcAliasToFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcToolKit.pch"


SOURCE=.\MarcDefaultValueConfigDlg.cpp

"$(INTDIR)\MarcDefaultValueConfigDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcToolKit.pch"


SOURCE=.\MarcImpExpMgr.cpp

"$(INTDIR)\MarcImpExpMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcToolKit.pch"


SOURCE=.\MarcRelationToText.cpp

"$(INTDIR)\MarcRelationToText.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcToolKit.pch"


SOURCE=.\MarcToolKit.cpp

"$(INTDIR)\MarcToolKit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcToolKit.pch"


SOURCE=.\MarcToolKit.rc

"$(INTDIR)\MarcToolKit.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ReadCfgFile.cpp

"$(INTDIR)\ReadCfgFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcToolKit.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "MarcToolKit - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcToolKit.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MarcToolKit.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "MarcToolKit - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MarcToolKit.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MarcToolKit.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

