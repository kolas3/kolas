# Microsoft Developer Studio Generated NMAKE File, Based on MngCharSetManagement.dsp
!IF "$(CFG)" == ""
CFG=MngCharSetManagement - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MngCharSetManagement - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MngCharSetManagement - Win32 Release" && "$(CFG)" != "MngCharSetManagement - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MngCharSetManagement.mak" CFG="MngCharSetManagement - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MngCharSetManagement - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MngCharSetManagement - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "MngCharSetManagement - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MngCharSetManagement.dll"


CLEAN :
	-@erase "$(INTDIR)\CatCharSetManagementDlg.obj"
	-@erase "$(INTDIR)\CharSetInputModifyDlg.obj"
	-@erase "$(INTDIR)\CharSetManagementDlg.obj"
	-@erase "$(INTDIR)\MngCharSetManagement.obj"
	-@erase "$(INTDIR)\MngCharSetManagement.pch"
	-@erase "$(INTDIR)\MngCharSetManagement.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MngCharSetManagement.dll"
	-@erase "$(OUTDIR)\MngCharSetManagement.exp"
	-@erase "$(OUTDIR)\MngCharSetManagement.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MngCharSetManagement.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MngCharSetManagement.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MngCharSetManagement.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\MngCharSetManagement.pdb" /machine:I386 /def:".\MngCharSetManagement.def" /out:"$(OUTDIR)\MngCharSetManagement.dll" /implib:"$(OUTDIR)\MngCharSetManagement.lib" 
DEF_FILE= \
	".\MngCharSetManagement.def"
LINK32_OBJS= \
	"$(INTDIR)\CatCharSetManagementDlg.obj" \
	"$(INTDIR)\CharSetInputModifyDlg.obj" \
	"$(INTDIR)\CharSetManagementDlg.obj" \
	"$(INTDIR)\MngCharSetManagement.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MngCharSetManagement.res"

"$(OUTDIR)\MngCharSetManagement.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MngCharSetManagement - Win32 Debug"

OUTDIR=.\..\..\..\..\lib
INTDIR=.\Debug

ALL : "..\..\..\..\..\dll\mng\MngCharSetManagement.dll"


CLEAN :
	-@erase "$(INTDIR)\CatCharSetManagementDlg.obj"
	-@erase "$(INTDIR)\CharSetInputModifyDlg.obj"
	-@erase "$(INTDIR)\CharSetManagementDlg.obj"
	-@erase "$(INTDIR)\MngCharSetManagement.obj"
	-@erase "$(INTDIR)\MngCharSetManagement.pch"
	-@erase "$(INTDIR)\MngCharSetManagement.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\MngCharSetManagement.exp"
	-@erase "$(OUTDIR)\MngCharSetManagement.lib"
	-@erase "$(OUTDIR)\MngCharSetManagement.pdb"
	-@erase "..\..\..\..\..\dll\mng\MngCharSetManagement.dll"
	-@erase "..\..\..\..\..\dll\mng\MngCharSetManagement.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MngCharSetManagement.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MngCharSetManagement.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MngCharSetManagement.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=esl.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\MngCharSetManagement.pdb" /debug /machine:I386 /def:".\MngCharSetManagement.def" /out:"..\..\..\..\..\dll\mng\MngCharSetManagement.dll" /implib:"$(OUTDIR)\MngCharSetManagement.lib" /pdbtype:sept 
DEF_FILE= \
	".\MngCharSetManagement.def"
LINK32_OBJS= \
	"$(INTDIR)\CatCharSetManagementDlg.obj" \
	"$(INTDIR)\CharSetInputModifyDlg.obj" \
	"$(INTDIR)\CharSetManagementDlg.obj" \
	"$(INTDIR)\MngCharSetManagement.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MngCharSetManagement.res"

"..\..\..\..\..\dll\mng\MngCharSetManagement.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("MngCharSetManagement.dep")
!INCLUDE "MngCharSetManagement.dep"
!ELSE 
!MESSAGE Warning: cannot find "MngCharSetManagement.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MngCharSetManagement - Win32 Release" || "$(CFG)" == "MngCharSetManagement - Win32 Debug"
SOURCE=.\CatCharSetManagementDlg.cpp

"$(INTDIR)\CatCharSetManagementDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MngCharSetManagement.pch"


SOURCE=.\CharSetInputModifyDlg.cpp

"$(INTDIR)\CharSetInputModifyDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MngCharSetManagement.pch"


SOURCE=.\CharSetManagementDlg.cpp

"$(INTDIR)\CharSetManagementDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MngCharSetManagement.pch"


SOURCE=.\MngCharSetManagement.cpp

"$(INTDIR)\MngCharSetManagement.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MngCharSetManagement.pch"


SOURCE=.\MngCharSetManagement.rc

"$(INTDIR)\MngCharSetManagement.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "MngCharSetManagement - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MngCharSetManagement.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MngCharSetManagement.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "MngCharSetManagement - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\MngCharSetManagement.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MngCharSetManagement.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

