# Microsoft Developer Studio Generated NMAKE File, Based on CAT_FIXED_FIELD.dsp
!IF "$(CFG)" == ""
CFG=CAT_FIXED_FIELD - Win32 Debug
!MESSAGE No configuration specified. Defaulting to CAT_FIXED_FIELD - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CAT_FIXED_FIELD - Win32 Release" && "$(CFG)" != "CAT_FIXED_FIELD - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CAT_FIXED_FIELD.mak" CFG="CAT_FIXED_FIELD - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CAT_FIXED_FIELD - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CAT_FIXED_FIELD - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "CAT_FIXED_FIELD - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\CAT_FIXED_FIELD.dll"


CLEAN :
	-@erase "$(INTDIR)\Cat007FieldContainer.obj"
	-@erase "$(INTDIR)\Cat007FixedField_A.obj"
	-@erase "$(INTDIR)\Cat007FixedField_C.obj"
	-@erase "$(INTDIR)\Cat007FixedField_D.obj"
	-@erase "$(INTDIR)\Cat007FixedField_G.obj"
	-@erase "$(INTDIR)\Cat007FixedField_H.obj"
	-@erase "$(INTDIR)\Cat007FixedField_K.obj"
	-@erase "$(INTDIR)\Cat007FixedField_M.obj"
	-@erase "$(INTDIR)\Cat007FixedField_O.obj"
	-@erase "$(INTDIR)\Cat007FixedField_S.obj"
	-@erase "$(INTDIR)\Cat007FixedField_T.obj"
	-@erase "$(INTDIR)\Cat007FixedField_V.obj"
	-@erase "$(INTDIR)\Cat007FixedField_Z.obj"
	-@erase "$(INTDIR)\Cat007FixedFieldMgr.obj"
	-@erase "$(INTDIR)\Cat008FixedField_AC.obj"
	-@erase "$(INTDIR)\Cat008FixedField_CF.obj"
	-@erase "$(INTDIR)\Cat008FixedField_MP.obj"
	-@erase "$(INTDIR)\Cat008FixedField_MU.obj"
	-@erase "$(INTDIR)\Cat008FixedField_O.obj"
	-@erase "$(INTDIR)\Cat008FixedField_SE.obj"
	-@erase "$(INTDIR)\Cat008FixedField_TX.obj"
	-@erase "$(INTDIR)\Cat008FixedField_VM.obj"
	-@erase "$(INTDIR)\Cat008FixedFieldMgr.obj"
	-@erase "$(INTDIR)\CAT_FIXED_FIELD.obj"
	-@erase "$(INTDIR)\CAT_FIXED_FIELD.pch"
	-@erase "$(INTDIR)\CAT_FIXED_FIELD.res"
	-@erase "$(INTDIR)\FixedFieldInfo.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\CAT_FIXED_FIELD.dll"
	-@erase "$(OUTDIR)\CAT_FIXED_FIELD.exp"
	-@erase "$(OUTDIR)\CAT_FIXED_FIELD.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\CAT_FIXED_FIELD.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\CAT_FIXED_FIELD.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CAT_FIXED_FIELD.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\CAT_FIXED_FIELD.pdb" /machine:I386 /def:".\CAT_FIXED_FIELD.def" /out:"$(OUTDIR)\CAT_FIXED_FIELD.dll" /implib:"$(OUTDIR)\CAT_FIXED_FIELD.lib" 
DEF_FILE= \
	".\CAT_FIXED_FIELD.def"
LINK32_OBJS= \
	"$(INTDIR)\Cat007FieldContainer.obj" \
	"$(INTDIR)\Cat007FixedField_A.obj" \
	"$(INTDIR)\Cat007FixedField_C.obj" \
	"$(INTDIR)\Cat007FixedField_D.obj" \
	"$(INTDIR)\Cat007FixedField_G.obj" \
	"$(INTDIR)\Cat007FixedField_H.obj" \
	"$(INTDIR)\Cat007FixedField_K.obj" \
	"$(INTDIR)\Cat007FixedField_M.obj" \
	"$(INTDIR)\Cat007FixedField_O.obj" \
	"$(INTDIR)\Cat007FixedField_S.obj" \
	"$(INTDIR)\Cat007FixedField_T.obj" \
	"$(INTDIR)\Cat007FixedField_V.obj" \
	"$(INTDIR)\Cat007FixedField_Z.obj" \
	"$(INTDIR)\Cat007FixedFieldMgr.obj" \
	"$(INTDIR)\Cat008FixedField_AC.obj" \
	"$(INTDIR)\Cat008FixedField_CF.obj" \
	"$(INTDIR)\Cat008FixedField_MP.obj" \
	"$(INTDIR)\Cat008FixedField_MU.obj" \
	"$(INTDIR)\Cat008FixedField_O.obj" \
	"$(INTDIR)\Cat008FixedField_SE.obj" \
	"$(INTDIR)\Cat008FixedField_TX.obj" \
	"$(INTDIR)\Cat008FixedField_VM.obj" \
	"$(INTDIR)\Cat008FixedFieldMgr.obj" \
	"$(INTDIR)\CAT_FIXED_FIELD.obj" \
	"$(INTDIR)\FixedFieldInfo.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\CAT_FIXED_FIELD.res"

"$(OUTDIR)\CAT_FIXED_FIELD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CAT_FIXED_FIELD - Win32 Debug"

OUTDIR=.\../../../lib
INTDIR=.\Debug

ALL : "..\..\..\..\dll\common\CAT_FIXED_FIELD.dll"


CLEAN :
	-@erase "$(INTDIR)\Cat007FieldContainer.obj"
	-@erase "$(INTDIR)\Cat007FixedField_A.obj"
	-@erase "$(INTDIR)\Cat007FixedField_C.obj"
	-@erase "$(INTDIR)\Cat007FixedField_D.obj"
	-@erase "$(INTDIR)\Cat007FixedField_G.obj"
	-@erase "$(INTDIR)\Cat007FixedField_H.obj"
	-@erase "$(INTDIR)\Cat007FixedField_K.obj"
	-@erase "$(INTDIR)\Cat007FixedField_M.obj"
	-@erase "$(INTDIR)\Cat007FixedField_O.obj"
	-@erase "$(INTDIR)\Cat007FixedField_S.obj"
	-@erase "$(INTDIR)\Cat007FixedField_T.obj"
	-@erase "$(INTDIR)\Cat007FixedField_V.obj"
	-@erase "$(INTDIR)\Cat007FixedField_Z.obj"
	-@erase "$(INTDIR)\Cat007FixedFieldMgr.obj"
	-@erase "$(INTDIR)\Cat008FixedField_AC.obj"
	-@erase "$(INTDIR)\Cat008FixedField_CF.obj"
	-@erase "$(INTDIR)\Cat008FixedField_MP.obj"
	-@erase "$(INTDIR)\Cat008FixedField_MU.obj"
	-@erase "$(INTDIR)\Cat008FixedField_O.obj"
	-@erase "$(INTDIR)\Cat008FixedField_SE.obj"
	-@erase "$(INTDIR)\Cat008FixedField_TX.obj"
	-@erase "$(INTDIR)\Cat008FixedField_VM.obj"
	-@erase "$(INTDIR)\Cat008FixedFieldMgr.obj"
	-@erase "$(INTDIR)\CAT_FIXED_FIELD.obj"
	-@erase "$(INTDIR)\CAT_FIXED_FIELD.pch"
	-@erase "$(INTDIR)\CAT_FIXED_FIELD.res"
	-@erase "$(INTDIR)\FixedFieldInfo.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\CAT_FIXED_FIELD.exp"
	-@erase "$(OUTDIR)\CAT_FIXED_FIELD.lib"
	-@erase "$(OUTDIR)\CAT_FIXED_FIELD.pdb"
	-@erase "..\..\..\..\dll\common\CAT_FIXED_FIELD.dll"
	-@erase "..\..\..\..\dll\common\CAT_FIXED_FIELD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\CAT_FIXED_FIELD.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\CAT_FIXED_FIELD.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CAT_FIXED_FIELD.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=esl.lib KOMarcGetter.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\CAT_FIXED_FIELD.pdb" /debug /machine:I386 /def:".\CAT_FIXED_FIELD.def" /out:"..\..\..\..\dll\common\CAT_FIXED_FIELD.dll" /implib:"$(OUTDIR)\CAT_FIXED_FIELD.lib" /pdbtype:sept 
DEF_FILE= \
	".\CAT_FIXED_FIELD.def"
LINK32_OBJS= \
	"$(INTDIR)\Cat007FieldContainer.obj" \
	"$(INTDIR)\Cat007FixedField_A.obj" \
	"$(INTDIR)\Cat007FixedField_C.obj" \
	"$(INTDIR)\Cat007FixedField_D.obj" \
	"$(INTDIR)\Cat007FixedField_G.obj" \
	"$(INTDIR)\Cat007FixedField_H.obj" \
	"$(INTDIR)\Cat007FixedField_K.obj" \
	"$(INTDIR)\Cat007FixedField_M.obj" \
	"$(INTDIR)\Cat007FixedField_O.obj" \
	"$(INTDIR)\Cat007FixedField_S.obj" \
	"$(INTDIR)\Cat007FixedField_T.obj" \
	"$(INTDIR)\Cat007FixedField_V.obj" \
	"$(INTDIR)\Cat007FixedField_Z.obj" \
	"$(INTDIR)\Cat007FixedFieldMgr.obj" \
	"$(INTDIR)\Cat008FixedField_AC.obj" \
	"$(INTDIR)\Cat008FixedField_CF.obj" \
	"$(INTDIR)\Cat008FixedField_MP.obj" \
	"$(INTDIR)\Cat008FixedField_MU.obj" \
	"$(INTDIR)\Cat008FixedField_O.obj" \
	"$(INTDIR)\Cat008FixedField_SE.obj" \
	"$(INTDIR)\Cat008FixedField_TX.obj" \
	"$(INTDIR)\Cat008FixedField_VM.obj" \
	"$(INTDIR)\Cat008FixedFieldMgr.obj" \
	"$(INTDIR)\CAT_FIXED_FIELD.obj" \
	"$(INTDIR)\FixedFieldInfo.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\CAT_FIXED_FIELD.res"

"..\..\..\..\dll\common\CAT_FIXED_FIELD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("CAT_FIXED_FIELD.dep")
!INCLUDE "CAT_FIXED_FIELD.dep"
!ELSE 
!MESSAGE Warning: cannot find "CAT_FIXED_FIELD.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CAT_FIXED_FIELD - Win32 Release" || "$(CFG)" == "CAT_FIXED_FIELD - Win32 Debug"
SOURCE=.\Cat007FieldContainer.cpp

"$(INTDIR)\Cat007FieldContainer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat007FixedField_A.cpp

"$(INTDIR)\Cat007FixedField_A.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat007FixedField_C.cpp

"$(INTDIR)\Cat007FixedField_C.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat007FixedField_D.cpp

"$(INTDIR)\Cat007FixedField_D.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat007FixedField_G.cpp

"$(INTDIR)\Cat007FixedField_G.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat007FixedField_H.cpp

"$(INTDIR)\Cat007FixedField_H.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat007FixedField_K.cpp

"$(INTDIR)\Cat007FixedField_K.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat007FixedField_M.cpp

"$(INTDIR)\Cat007FixedField_M.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat007FixedField_O.cpp

"$(INTDIR)\Cat007FixedField_O.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat007FixedField_S.cpp

"$(INTDIR)\Cat007FixedField_S.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat007FixedField_T.cpp

"$(INTDIR)\Cat007FixedField_T.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat007FixedField_V.cpp

"$(INTDIR)\Cat007FixedField_V.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat007FixedField_Z.cpp

"$(INTDIR)\Cat007FixedField_Z.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat007FixedFieldMgr.cpp

"$(INTDIR)\Cat007FixedFieldMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat008FixedField_AC.cpp

"$(INTDIR)\Cat008FixedField_AC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat008FixedField_CF.cpp

"$(INTDIR)\Cat008FixedField_CF.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat008FixedField_MP.cpp

"$(INTDIR)\Cat008FixedField_MP.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat008FixedField_MU.cpp

"$(INTDIR)\Cat008FixedField_MU.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat008FixedField_O.cpp

"$(INTDIR)\Cat008FixedField_O.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat008FixedField_SE.cpp

"$(INTDIR)\Cat008FixedField_SE.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat008FixedField_TX.cpp

"$(INTDIR)\Cat008FixedField_TX.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat008FixedField_VM.cpp

"$(INTDIR)\Cat008FixedField_VM.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\Cat008FixedFieldMgr.cpp

"$(INTDIR)\Cat008FixedFieldMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\CAT_FIXED_FIELD.cpp

"$(INTDIR)\CAT_FIXED_FIELD.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\CAT_FIXED_FIELD.rc

"$(INTDIR)\CAT_FIXED_FIELD.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\FixedFieldInfo.cpp

"$(INTDIR)\FixedFieldInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\CAT_FIXED_FIELD.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "CAT_FIXED_FIELD - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\CAT_FIXED_FIELD.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\CAT_FIXED_FIELD.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CAT_FIXED_FIELD - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\CAT_FIXED_FIELD.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\CAT_FIXED_FIELD.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

