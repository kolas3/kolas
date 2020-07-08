# Microsoft Developer Studio Generated NMAKE File, Based on IndexToolKit.dsp
!IF "$(CFG)" == ""
CFG=IndexToolKit - Win32 Debug
!MESSAGE No configuration specified. Defaulting to IndexToolKit - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IndexToolKit - Win32 Release" && "$(CFG)" != "IndexToolKit - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IndexToolKit.mak" CFG="IndexToolKit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IndexToolKit - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IndexToolKit - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "IndexToolKit - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\IndexToolKit.dll"


CLEAN :
	-@erase "$(INTDIR)\Index.obj"
	-@erase "$(INTDIR)\Index_Mono.obj"
	-@erase "$(INTDIR)\Index_NonBook.obj"
	-@erase "$(INTDIR)\Index_Serial.obj"
	-@erase "$(INTDIR)\IndexTool.obj"
	-@erase "$(INTDIR)\IndexToolKit.obj"
	-@erase "$(INTDIR)\IndexToolKit.pch"
	-@erase "$(INTDIR)\IndexToolKit.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\IndexToolKit.dll"
	-@erase "$(OUTDIR)\IndexToolKit.exp"
	-@erase "$(OUTDIR)\IndexToolKit.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\IndexToolKit.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IndexToolKit.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IndexToolKit.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\IndexToolKit.pdb" /machine:I386 /def:".\IndexToolKit.def" /out:"$(OUTDIR)\IndexToolKit.dll" /implib:"$(OUTDIR)\IndexToolKit.lib" 
DEF_FILE= \
	".\IndexToolKit.def"
LINK32_OBJS= \
	"$(INTDIR)\Index.obj" \
	"$(INTDIR)\Index_Mono.obj" \
	"$(INTDIR)\Index_NonBook.obj" \
	"$(INTDIR)\Index_Serial.obj" \
	"$(INTDIR)\IndexTool.obj" \
	"$(INTDIR)\IndexToolKit.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IndexToolKit.res"

"$(OUTDIR)\IndexToolKit.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IndexToolKit - Win32 Debug"

OUTDIR=.\../../../lib
INTDIR=.\Debug

ALL : "..\..\..\..\dll\COMMON\IndexToolKit.dll"


CLEAN :
	-@erase "$(INTDIR)\Index.obj"
	-@erase "$(INTDIR)\Index_Mono.obj"
	-@erase "$(INTDIR)\Index_NonBook.obj"
	-@erase "$(INTDIR)\Index_Serial.obj"
	-@erase "$(INTDIR)\IndexTool.obj"
	-@erase "$(INTDIR)\IndexToolKit.obj"
	-@erase "$(INTDIR)\IndexToolKit.pch"
	-@erase "$(INTDIR)\IndexToolKit.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\IndexToolKit.exp"
	-@erase "$(OUTDIR)\IndexToolKit.lib"
	-@erase "$(OUTDIR)\IndexToolKit.pdb"
	-@erase "..\..\..\..\dll\COMMON\IndexToolKit.dll"
	-@erase "..\..\..\..\dll\COMMON\IndexToolKit.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\IndexToolKit.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\IndexToolKit.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\IndexToolKit.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ESL.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\IndexToolKit.pdb" /debug /machine:I386 /def:".\IndexToolKit.def" /out:"..\..\..\..\dll\COMMON\IndexToolKit.dll" /implib:"$(OUTDIR)\IndexToolKit.lib" /pdbtype:sept 
DEF_FILE= \
	".\IndexToolKit.def"
LINK32_OBJS= \
	"$(INTDIR)\Index.obj" \
	"$(INTDIR)\Index_Mono.obj" \
	"$(INTDIR)\Index_NonBook.obj" \
	"$(INTDIR)\Index_Serial.obj" \
	"$(INTDIR)\IndexTool.obj" \
	"$(INTDIR)\IndexToolKit.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\IndexToolKit.res"

"..\..\..\..\dll\COMMON\IndexToolKit.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("IndexToolKit.dep")
!INCLUDE "IndexToolKit.dep"
!ELSE 
!MESSAGE Warning: cannot find "IndexToolKit.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "IndexToolKit - Win32 Release" || "$(CFG)" == "IndexToolKit - Win32 Debug"
SOURCE=.\Index.cpp

"$(INTDIR)\Index.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IndexToolKit.pch"


SOURCE=.\Index_Mono.cpp

"$(INTDIR)\Index_Mono.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IndexToolKit.pch"


SOURCE=.\Index_NonBook.cpp

"$(INTDIR)\Index_NonBook.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IndexToolKit.pch"


SOURCE=.\Index_Serial.cpp

"$(INTDIR)\Index_Serial.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IndexToolKit.pch"


SOURCE=.\IndexTool.cpp

"$(INTDIR)\IndexTool.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IndexToolKit.pch"


SOURCE=.\IndexToolKit.cpp

"$(INTDIR)\IndexToolKit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\IndexToolKit.pch"


SOURCE=.\IndexToolKit.rc

"$(INTDIR)\IndexToolKit.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "IndexToolKit - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\IndexToolKit.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IndexToolKit.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "IndexToolKit - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\IndexToolKit.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\IndexToolKit.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

