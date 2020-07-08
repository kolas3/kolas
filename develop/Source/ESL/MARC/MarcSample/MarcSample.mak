# Microsoft Developer Studio Generated NMAKE File, Based on MarcSample.dsp
!IF "$(CFG)" == ""
CFG=MarcSample - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MarcSample - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MarcSample - Win32 Release" && "$(CFG)" != "MarcSample - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MarcSample.mak" CFG="MarcSample - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MarcSample - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MarcSample - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "MarcSample - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\MarcSample.exe"

!ELSE 

ALL : "MarcEdit - Win32 Release" "$(OUTDIR)\MarcSample.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"MarcEdit - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\MarcSample.obj"
	-@erase "$(INTDIR)\MarcSample.pch"
	-@erase "$(INTDIR)\MarcSample.res"
	-@erase "$(INTDIR)\MarcSampleDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MarcSample.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\MarcSample.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MarcSample.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MarcSample.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\MarcSample.pdb" /machine:I386 /out:"$(OUTDIR)\MarcSample.exe" 
LINK32_OBJS= \
	"$(INTDIR)\MarcSample.obj" \
	"$(INTDIR)\MarcSampleDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MarcSample.res" \
	"..\MarcEdit\Release\MarcEdit.lib"

"$(OUTDIR)\MarcSample.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MarcSample - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\..\..\bin\MarcSample.exe"

!ELSE 

ALL : "MarcEdit - Win32 Debug" "..\..\..\..\..\bin\MarcSample.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"MarcEdit - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\MarcSample.obj"
	-@erase "$(INTDIR)\MarcSample.pch"
	-@erase "$(INTDIR)\MarcSample.res"
	-@erase "$(INTDIR)\MarcSampleDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\MarcSample.pdb"
	-@erase "..\..\..\..\..\bin\MarcSample.exe"
	-@erase "..\..\..\..\..\bin\MarcSample.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\MarcSample.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\MarcSample.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MarcSample.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ESL.lib MarcEdit.lib MarcToolKit.lib IndexToolKit.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\MarcSample.pdb" /debug /machine:I386 /out:"..\..\..\..\..\bin\MarcSample.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\MarcSample.obj" \
	"$(INTDIR)\MarcSampleDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\MarcSample.res" \
	"..\..\..\..\lib\MarcEdit.lib"

"..\..\..\..\..\bin\MarcSample.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("MarcSample.dep")
!INCLUDE "MarcSample.dep"
!ELSE 
!MESSAGE Warning: cannot find "MarcSample.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MarcSample - Win32 Release" || "$(CFG)" == "MarcSample - Win32 Debug"
SOURCE=.\MarcSample.cpp

"$(INTDIR)\MarcSample.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcSample.pch"


SOURCE=.\MarcSample.rc

"$(INTDIR)\MarcSample.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\MarcSampleDlg.cpp

"$(INTDIR)\MarcSampleDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MarcSample.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "MarcSample - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\MarcSample.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MarcSample.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "MarcSample - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\MarcSample.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MarcSample.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "MarcSample - Win32 Release"

"MarcEdit - Win32 Release" : 
   cd "\KOLAS2UP\Develop\Source\ESL\MARC\MarcEdit"
   $(MAKE) /$(MAKEFLAGS) /F .\MarcEdit.mak CFG="MarcEdit - Win32 Release" 
   cd "..\MarcSample"

"MarcEdit - Win32 ReleaseCLEAN" : 
   cd "\KOLAS2UP\Develop\Source\ESL\MARC\MarcEdit"
   $(MAKE) /$(MAKEFLAGS) /F .\MarcEdit.mak CFG="MarcEdit - Win32 Release" RECURSE=1 CLEAN 
   cd "..\MarcSample"

!ELSEIF  "$(CFG)" == "MarcSample - Win32 Debug"

"MarcEdit - Win32 Debug" : 
   cd "\KOLAS2UP\Develop\Source\ESL\MARC\MarcEdit"
   $(MAKE) /$(MAKEFLAGS) /F .\MarcEdit.mak CFG="MarcEdit - Win32 Debug" 
   cd "..\MarcSample"

"MarcEdit - Win32 DebugCLEAN" : 
   cd "\KOLAS2UP\Develop\Source\ESL\MARC\MarcEdit"
   $(MAKE) /$(MAKEFLAGS) /F .\MarcEdit.mak CFG="MarcEdit - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\MarcSample"

!ENDIF 


!ENDIF 

