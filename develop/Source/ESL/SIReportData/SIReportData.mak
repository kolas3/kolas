# Microsoft Developer Studio Generated NMAKE File, Based on SIReportData.dsp
!IF "$(CFG)" == ""
CFG=SIReportData - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SIReportData - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SIReportData - Win32 Release" && "$(CFG)" != "SIReportData - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SIReportData.mak" CFG="SIReportData - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SIReportData - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SIReportData - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "SIReportData - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\SIReportData.dll"


CLEAN :
	-@erase "$(INTDIR)\LoadBmp.obj"
	-@erase "$(INTDIR)\SIBarcodeCtrl.obj"
	-@erase "$(INTDIR)\SIReportData.obj"
	-@erase "$(INTDIR)\SIReportData.pch"
	-@erase "$(INTDIR)\SIReportData.res"
	-@erase "$(INTDIR)\SIReportEventInfo.obj"
	-@erase "$(INTDIR)\SIReportObject.obj"
	-@erase "$(INTDIR)\SIReportObject_Barcode.obj"
	-@erase "$(INTDIR)\SIReportObject_Bitmap.obj"
	-@erase "$(INTDIR)\SIReportObject_Line.obj"
	-@erase "$(INTDIR)\SIReportObject_Table.obj"
	-@erase "$(INTDIR)\SIReportObject_Table_Cell.obj"
	-@erase "$(INTDIR)\SIReportObject_Text.obj"
	-@erase "$(INTDIR)\SIReportObject_Text_Box.obj"
	-@erase "$(INTDIR)\SIReportReadBroker.obj"
	-@erase "$(INTDIR)\SIReportWorkForm.obj"
	-@erase "$(INTDIR)\SIReportWorkSheet.obj"
	-@erase "$(INTDIR)\SIReportXMLIOMgr.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SIReportData.dll"
	-@erase "$(OUTDIR)\SIReportData.exp"
	-@erase "$(OUTDIR)\SIReportData.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\SIReportData.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SIReportData.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SIReportData.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\SIReportData.pdb" /machine:I386 /def:".\SIReportData.def" /out:"$(OUTDIR)\SIReportData.dll" /implib:"$(OUTDIR)\SIReportData.lib" 
DEF_FILE= \
	".\SIReportData.def"
LINK32_OBJS= \
	"$(INTDIR)\LoadBmp.obj" \
	"$(INTDIR)\SIBarcodeCtrl.obj" \
	"$(INTDIR)\SIReportData.obj" \
	"$(INTDIR)\SIReportEventInfo.obj" \
	"$(INTDIR)\SIReportObject.obj" \
	"$(INTDIR)\SIReportObject_Barcode.obj" \
	"$(INTDIR)\SIReportObject_Bitmap.obj" \
	"$(INTDIR)\SIReportObject_Line.obj" \
	"$(INTDIR)\SIReportObject_Table.obj" \
	"$(INTDIR)\SIReportObject_Table_Cell.obj" \
	"$(INTDIR)\SIReportObject_Text.obj" \
	"$(INTDIR)\SIReportObject_Text_Box.obj" \
	"$(INTDIR)\SIReportReadBroker.obj" \
	"$(INTDIR)\SIReportWorkForm.obj" \
	"$(INTDIR)\SIReportWorkSheet.obj" \
	"$(INTDIR)\SIReportXMLIOMgr.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SIReportData.res"

"$(OUTDIR)\SIReportData.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SIReportData - Win32 Debug"

OUTDIR=.\..\..\..\lib
INTDIR=.\Debug

ALL : "..\..\..\..\DLL\COMMON\SIReportData.dll"


CLEAN :
	-@erase "$(INTDIR)\LoadBmp.obj"
	-@erase "$(INTDIR)\SIBarcodeCtrl.obj"
	-@erase "$(INTDIR)\SIReportData.obj"
	-@erase "$(INTDIR)\SIReportData.pch"
	-@erase "$(INTDIR)\SIReportData.res"
	-@erase "$(INTDIR)\SIReportEventInfo.obj"
	-@erase "$(INTDIR)\SIReportObject.obj"
	-@erase "$(INTDIR)\SIReportObject_Barcode.obj"
	-@erase "$(INTDIR)\SIReportObject_Bitmap.obj"
	-@erase "$(INTDIR)\SIReportObject_Line.obj"
	-@erase "$(INTDIR)\SIReportObject_Table.obj"
	-@erase "$(INTDIR)\SIReportObject_Table_Cell.obj"
	-@erase "$(INTDIR)\SIReportObject_Text.obj"
	-@erase "$(INTDIR)\SIReportObject_Text_Box.obj"
	-@erase "$(INTDIR)\SIReportReadBroker.obj"
	-@erase "$(INTDIR)\SIReportWorkForm.obj"
	-@erase "$(INTDIR)\SIReportWorkSheet.obj"
	-@erase "$(INTDIR)\SIReportXMLIOMgr.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SIReportData.exp"
	-@erase "$(OUTDIR)\SIReportData.lib"
	-@erase "$(OUTDIR)\SIReportData.pdb"
	-@erase "..\..\..\..\DLL\COMMON\SIReportData.dll"
	-@erase "..\..\..\..\DLL\COMMON\SIReportData.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\SIReportData.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SIReportData.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SIReportData.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\SIReportData.pdb" /debug /machine:I386 /def:".\SIReportData.def" /out:"..\..\..\..\DLL\COMMON\SIReportData.dll" /implib:"$(OUTDIR)\SIReportData.lib" /pdbtype:sept 
DEF_FILE= \
	".\SIReportData.def"
LINK32_OBJS= \
	"$(INTDIR)\LoadBmp.obj" \
	"$(INTDIR)\SIBarcodeCtrl.obj" \
	"$(INTDIR)\SIReportData.obj" \
	"$(INTDIR)\SIReportEventInfo.obj" \
	"$(INTDIR)\SIReportObject.obj" \
	"$(INTDIR)\SIReportObject_Barcode.obj" \
	"$(INTDIR)\SIReportObject_Bitmap.obj" \
	"$(INTDIR)\SIReportObject_Line.obj" \
	"$(INTDIR)\SIReportObject_Table.obj" \
	"$(INTDIR)\SIReportObject_Table_Cell.obj" \
	"$(INTDIR)\SIReportObject_Text.obj" \
	"$(INTDIR)\SIReportObject_Text_Box.obj" \
	"$(INTDIR)\SIReportReadBroker.obj" \
	"$(INTDIR)\SIReportWorkForm.obj" \
	"$(INTDIR)\SIReportWorkSheet.obj" \
	"$(INTDIR)\SIReportXMLIOMgr.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SIReportData.res"

"..\..\..\..\DLL\COMMON\SIReportData.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("SIReportData.dep")
!INCLUDE "SIReportData.dep"
!ELSE 
!MESSAGE Warning: cannot find "SIReportData.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SIReportData - Win32 Release" || "$(CFG)" == "SIReportData - Win32 Debug"
SOURCE=.\LoadBmp.cpp

"$(INTDIR)\LoadBmp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIBarcodeCtrl.cpp

"$(INTDIR)\SIBarcodeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportData.cpp

"$(INTDIR)\SIReportData.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportData.rc

"$(INTDIR)\SIReportData.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\SIReportEventInfo.cpp

"$(INTDIR)\SIReportEventInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportObject.cpp

"$(INTDIR)\SIReportObject.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportObject_Barcode.cpp

"$(INTDIR)\SIReportObject_Barcode.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportObject_Bitmap.cpp

"$(INTDIR)\SIReportObject_Bitmap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportObject_Line.cpp

"$(INTDIR)\SIReportObject_Line.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportObject_Table.cpp

"$(INTDIR)\SIReportObject_Table.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportObject_Table_Cell.cpp

"$(INTDIR)\SIReportObject_Table_Cell.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportObject_Text.cpp

"$(INTDIR)\SIReportObject_Text.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportObject_Text_Box.cpp

"$(INTDIR)\SIReportObject_Text_Box.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportReadBroker.cpp

"$(INTDIR)\SIReportReadBroker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportWorkForm.cpp

"$(INTDIR)\SIReportWorkForm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportWorkSheet.cpp

"$(INTDIR)\SIReportWorkSheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\SIReportXMLIOMgr.cpp

"$(INTDIR)\SIReportXMLIOMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SIReportData.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "SIReportData - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\SIReportData.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SIReportData.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SIReportData - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\SIReportData.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SIReportData.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

