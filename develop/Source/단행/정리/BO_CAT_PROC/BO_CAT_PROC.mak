# Microsoft Developer Studio Generated NMAKE File, Based on BO_CAT_PROC.dsp
!IF "$(CFG)" == ""
CFG=BO_CAT_PROC - Win32 Debug
!MESSAGE No configuration specified. Defaulting to BO_CAT_PROC - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "BO_CAT_PROC - Win32 Release" && "$(CFG)" != "BO_CAT_PROC - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BO_CAT_PROC.mak" CFG="BO_CAT_PROC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BO_CAT_PROC - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BO_CAT_PROC - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\BO_CAT_PROC.dll"


CLEAN :
	-@erase "$(INTDIR)\BO_CAT_PROC.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC.pch"
	-@erase "$(INTDIR)\BO_CAT_PROC.res"
	-@erase "$(INTDIR)\BO_CAT_PROC_0000.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0100.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0200.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0201.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0202.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0202_PRINT.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0203.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0203_01.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0203_02.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0204.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0204_01.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0300.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301_01.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301_ROLE.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301_ROLE_01.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301_ROLE_02.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0330.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0332.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0373.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0380.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0382.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0390.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0400.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0400_08.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0500_09.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0510.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0801.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_1100.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_1200.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_1300.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_AcqNumber.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_Api.obj"
	-@erase "$(INTDIR)\Kolas2up_Card.obj"
	-@erase "$(INTDIR)\Kolas2up_CardList.obj"
	-@erase "$(INTDIR)\SPF.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\BO_CAT_PROC.dll"
	-@erase "$(OUTDIR)\BO_CAT_PROC.exp"
	-@erase "$(OUTDIR)\BO_CAT_PROC.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_PROC.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_CAT_PROC.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_CAT_PROC.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\BO_CAT_PROC.pdb" /machine:I386 /def:".\BO_CAT_PROC.def" /out:"$(OUTDIR)\BO_CAT_PROC.dll" /implib:"$(OUTDIR)\BO_CAT_PROC.lib" 
DEF_FILE= \
	".\BO_CAT_PROC.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_CAT_PROC.obj" \
	"$(INTDIR)\BO_CAT_PROC_0000.obj" \
	"$(INTDIR)\BO_CAT_PROC_0100.obj" \
	"$(INTDIR)\BO_CAT_PROC_0200.obj" \
	"$(INTDIR)\BO_CAT_PROC_0201.obj" \
	"$(INTDIR)\BO_CAT_PROC_0202.obj" \
	"$(INTDIR)\BO_CAT_PROC_0202_PRINT.obj" \
	"$(INTDIR)\BO_CAT_PROC_0203.obj" \
	"$(INTDIR)\BO_CAT_PROC_0203_01.obj" \
	"$(INTDIR)\BO_CAT_PROC_0203_02.obj" \
	"$(INTDIR)\BO_CAT_PROC_0204.obj" \
	"$(INTDIR)\BO_CAT_PROC_0204_01.obj" \
	"$(INTDIR)\BO_CAT_PROC_0300.obj" \
	"$(INTDIR)\BO_CAT_PROC_0301.obj" \
	"$(INTDIR)\BO_CAT_PROC_0301_01.obj" \
	"$(INTDIR)\BO_CAT_PROC_0301_ROLE.obj" \
	"$(INTDIR)\BO_CAT_PROC_0301_ROLE_01.obj" \
	"$(INTDIR)\BO_CAT_PROC_0301_ROLE_02.obj" \
	"$(INTDIR)\BO_CAT_PROC_0330.obj" \
	"$(INTDIR)\BO_CAT_PROC_0332.obj" \
	"$(INTDIR)\BO_CAT_PROC_0373.obj" \
	"$(INTDIR)\BO_CAT_PROC_0380.obj" \
	"$(INTDIR)\BO_CAT_PROC_0382.obj" \
	"$(INTDIR)\BO_CAT_PROC_0390.obj" \
	"$(INTDIR)\BO_CAT_PROC_0400.obj" \
	"$(INTDIR)\BO_CAT_PROC_0400_08.obj" \
	"$(INTDIR)\BO_CAT_PROC_0500_09.obj" \
	"$(INTDIR)\BO_CAT_PROC_0510.obj" \
	"$(INTDIR)\BO_CAT_PROC_0801.obj" \
	"$(INTDIR)\BO_CAT_PROC_1100.obj" \
	"$(INTDIR)\BO_CAT_PROC_1200.obj" \
	"$(INTDIR)\BO_CAT_PROC_1300.obj" \
	"$(INTDIR)\BO_CAT_PROC_AcqNumber.obj" \
	"$(INTDIR)\BO_CAT_PROC_Api.obj" \
	"$(INTDIR)\Kolas2up_Card.obj" \
	"$(INTDIR)\Kolas2up_CardList.obj" \
	"$(INTDIR)\SPF.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_CAT_PROC.res"

"$(OUTDIR)\BO_CAT_PROC.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"

OUTDIR=.\..\..\..\..\lib
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..\..\..\..\lib
# End Custom Macros

ALL : "..\..\..\..\..\dll\MONO\BO_CAT_PROC.dll" "$(OUTDIR)\BO_CAT_PROC.bsc"


CLEAN :
	-@erase "$(INTDIR)\BO_CAT_PROC.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC.pch"
	-@erase "$(INTDIR)\BO_CAT_PROC.res"
	-@erase "$(INTDIR)\BO_CAT_PROC.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0000.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0000.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0100.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0100.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0200.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0200.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0201.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0201.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0202.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0202.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0202_PRINT.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0202_PRINT.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0203.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0203.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0203_01.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0203_01.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0203_02.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0203_02.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0204.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0204.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0204_01.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0204_01.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0300.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0300.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301_01.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301_01.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301_ROLE.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301_ROLE.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301_ROLE_01.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301_ROLE_01.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301_ROLE_02.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0301_ROLE_02.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0330.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0330.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0332.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0332.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0373.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0373.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0380.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0380.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0382.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0382.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0390.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0390.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0400.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0400.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0400_08.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0400_08.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0500_09.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0500_09.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0510.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0510.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_0801.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_0801.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_1100.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_1100.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_1200.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_1200.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_1300.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_1300.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_AcqNumber.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_AcqNumber.sbr"
	-@erase "$(INTDIR)\BO_CAT_PROC_Api.obj"
	-@erase "$(INTDIR)\BO_CAT_PROC_Api.sbr"
	-@erase "$(INTDIR)\Kolas2up_Card.obj"
	-@erase "$(INTDIR)\Kolas2up_Card.sbr"
	-@erase "$(INTDIR)\Kolas2up_CardList.obj"
	-@erase "$(INTDIR)\Kolas2up_CardList.sbr"
	-@erase "$(INTDIR)\SPF.obj"
	-@erase "$(INTDIR)\SPF.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\BO_CAT_PROC.bsc"
	-@erase "$(OUTDIR)\BO_CAT_PROC.exp"
	-@erase "$(OUTDIR)\BO_CAT_PROC.lib"
	-@erase "$(OUTDIR)\BO_CAT_PROC.pdb"
	-@erase "..\..\..\..\..\dll\MONO\BO_CAT_PROC.dll"
	-@erase "..\..\..\..\..\dll\MONO\BO_CAT_PROC.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\BO_CAT_PROC.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\BO_CAT_PROC.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BO_CAT_PROC.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BO_CAT_PROC.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0000.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0100.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0200.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0201.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0202.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0202_PRINT.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0203.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0203_01.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0203_02.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0204.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0204_01.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0300.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0301.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0301_01.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0301_ROLE.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0301_ROLE_01.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0301_ROLE_02.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0330.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0332.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0373.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0380.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0382.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0390.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0400.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0400_08.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0500_09.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0510.sbr" \
	"$(INTDIR)\BO_CAT_PROC_0801.sbr" \
	"$(INTDIR)\BO_CAT_PROC_1100.sbr" \
	"$(INTDIR)\BO_CAT_PROC_1200.sbr" \
	"$(INTDIR)\BO_CAT_PROC_1300.sbr" \
	"$(INTDIR)\BO_CAT_PROC_AcqNumber.sbr" \
	"$(INTDIR)\BO_CAT_PROC_Api.sbr" \
	"$(INTDIR)\Kolas2up_Card.sbr" \
	"$(INTDIR)\Kolas2up_CardList.sbr" \
	"$(INTDIR)\SPF.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\BO_CAT_PROC.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=esl.lib marcedit.lib MNG_CLASS_SIGN_POPUP.lib IndexToolKit.lib K2UP_Common.LIB LOC_CommonAPI.lib Duplicate_Data_Search.lib bo_species_api.lib BO_CAT_PROC_KEYWORD.lib FileImport.lib MarcToolKit.lib MNG_CODE_SHELF.LIB DonatorNameGetter.lib BO_CAT_NUMBER.LIB KOMarcGetter.lib Bo_Marc_Import.lib SIReportMgr.lib SiReportData.lib Light_GatherDataSearch.LIB UI_SEARCH_DLG.LIB BO_CAT_PROC_FIXEDFIELD.LIB CatCodeGetter.lib BO_CAT_DUP_FREE.LIB ShelfCodeGetter.lib Light_BO_CAT_CENTERUPLOAD.lib BusinessPopupSearcher.lib Kolas2up_MobileCommon.lib BO_VOLUME_FORM.lib BO_ACQ_GROUP_MANAGER.lib TOCEDITOR.lib MarcAdjustApi.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\BO_CAT_PROC.pdb" /debug /machine:I386 /def:".\BO_CAT_PROC.def" /out:"..\..\..\..\..\dll\MONO\BO_CAT_PROC.dll" /implib:"$(OUTDIR)\BO_CAT_PROC.lib" /pdbtype:sept 
DEF_FILE= \
	".\BO_CAT_PROC.def"
LINK32_OBJS= \
	"$(INTDIR)\BO_CAT_PROC.obj" \
	"$(INTDIR)\BO_CAT_PROC_0000.obj" \
	"$(INTDIR)\BO_CAT_PROC_0100.obj" \
	"$(INTDIR)\BO_CAT_PROC_0200.obj" \
	"$(INTDIR)\BO_CAT_PROC_0201.obj" \
	"$(INTDIR)\BO_CAT_PROC_0202.obj" \
	"$(INTDIR)\BO_CAT_PROC_0202_PRINT.obj" \
	"$(INTDIR)\BO_CAT_PROC_0203.obj" \
	"$(INTDIR)\BO_CAT_PROC_0203_01.obj" \
	"$(INTDIR)\BO_CAT_PROC_0203_02.obj" \
	"$(INTDIR)\BO_CAT_PROC_0204.obj" \
	"$(INTDIR)\BO_CAT_PROC_0204_01.obj" \
	"$(INTDIR)\BO_CAT_PROC_0300.obj" \
	"$(INTDIR)\BO_CAT_PROC_0301.obj" \
	"$(INTDIR)\BO_CAT_PROC_0301_01.obj" \
	"$(INTDIR)\BO_CAT_PROC_0301_ROLE.obj" \
	"$(INTDIR)\BO_CAT_PROC_0301_ROLE_01.obj" \
	"$(INTDIR)\BO_CAT_PROC_0301_ROLE_02.obj" \
	"$(INTDIR)\BO_CAT_PROC_0330.obj" \
	"$(INTDIR)\BO_CAT_PROC_0332.obj" \
	"$(INTDIR)\BO_CAT_PROC_0373.obj" \
	"$(INTDIR)\BO_CAT_PROC_0380.obj" \
	"$(INTDIR)\BO_CAT_PROC_0382.obj" \
	"$(INTDIR)\BO_CAT_PROC_0390.obj" \
	"$(INTDIR)\BO_CAT_PROC_0400.obj" \
	"$(INTDIR)\BO_CAT_PROC_0400_08.obj" \
	"$(INTDIR)\BO_CAT_PROC_0500_09.obj" \
	"$(INTDIR)\BO_CAT_PROC_0510.obj" \
	"$(INTDIR)\BO_CAT_PROC_0801.obj" \
	"$(INTDIR)\BO_CAT_PROC_1100.obj" \
	"$(INTDIR)\BO_CAT_PROC_1200.obj" \
	"$(INTDIR)\BO_CAT_PROC_1300.obj" \
	"$(INTDIR)\BO_CAT_PROC_AcqNumber.obj" \
	"$(INTDIR)\BO_CAT_PROC_Api.obj" \
	"$(INTDIR)\Kolas2up_Card.obj" \
	"$(INTDIR)\Kolas2up_CardList.obj" \
	"$(INTDIR)\SPF.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BO_CAT_PROC.res"

"..\..\..\..\..\dll\MONO\BO_CAT_PROC.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("BO_CAT_PROC.dep")
!INCLUDE "BO_CAT_PROC.dep"
!ELSE 
!MESSAGE Warning: cannot find "BO_CAT_PROC.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "BO_CAT_PROC - Win32 Release" || "$(CFG)" == "BO_CAT_PROC - Win32 Debug"
SOURCE=.\BO_CAT_PROC.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC.obj"	"$(INTDIR)\BO_CAT_PROC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC.rc

"$(INTDIR)\BO_CAT_PROC.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\BO_CAT_PROC_0000.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0000.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0000.obj"	"$(INTDIR)\BO_CAT_PROC_0000.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0100.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0100.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0100.obj"	"$(INTDIR)\BO_CAT_PROC_0100.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0200.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0200.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0200.obj"	"$(INTDIR)\BO_CAT_PROC_0200.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0201.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0201.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0201.obj"	"$(INTDIR)\BO_CAT_PROC_0201.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0202.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0202.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0202.obj"	"$(INTDIR)\BO_CAT_PROC_0202.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0202_PRINT.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0202_PRINT.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0202_PRINT.obj"	"$(INTDIR)\BO_CAT_PROC_0202_PRINT.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0203.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0203.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0203.obj"	"$(INTDIR)\BO_CAT_PROC_0203.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0203_01.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0203_01.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0203_01.obj"	"$(INTDIR)\BO_CAT_PROC_0203_01.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0203_02.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0203_02.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0203_02.obj"	"$(INTDIR)\BO_CAT_PROC_0203_02.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0204.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0204.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0204.obj"	"$(INTDIR)\BO_CAT_PROC_0204.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0204_01.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0204_01.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0204_01.obj"	"$(INTDIR)\BO_CAT_PROC_0204_01.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0300.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0300.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0300.obj"	"$(INTDIR)\BO_CAT_PROC_0300.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0301.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0301.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0301.obj"	"$(INTDIR)\BO_CAT_PROC_0301.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0301_01.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0301_01.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0301_01.obj"	"$(INTDIR)\BO_CAT_PROC_0301_01.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0301_ROLE.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0301_ROLE.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0301_ROLE.obj"	"$(INTDIR)\BO_CAT_PROC_0301_ROLE.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0301_ROLE_01.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0301_ROLE_01.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0301_ROLE_01.obj"	"$(INTDIR)\BO_CAT_PROC_0301_ROLE_01.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0301_ROLE_02.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0301_ROLE_02.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0301_ROLE_02.obj"	"$(INTDIR)\BO_CAT_PROC_0301_ROLE_02.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0330.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0330.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0330.obj"	"$(INTDIR)\BO_CAT_PROC_0330.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0332.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0332.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0332.obj"	"$(INTDIR)\BO_CAT_PROC_0332.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0373.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0373.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0373.obj"	"$(INTDIR)\BO_CAT_PROC_0373.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0380.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0380.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0380.obj"	"$(INTDIR)\BO_CAT_PROC_0380.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0382.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0382.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0382.obj"	"$(INTDIR)\BO_CAT_PROC_0382.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0390.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0390.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0390.obj"	"$(INTDIR)\BO_CAT_PROC_0390.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0400.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0400.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0400.obj"	"$(INTDIR)\BO_CAT_PROC_0400.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0400_08.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0400_08.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0400_08.obj"	"$(INTDIR)\BO_CAT_PROC_0400_08.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0500_09.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0500_09.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0500_09.obj"	"$(INTDIR)\BO_CAT_PROC_0500_09.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0510.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0510.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0510.obj"	"$(INTDIR)\BO_CAT_PROC_0510.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_0801.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_0801.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_0801.obj"	"$(INTDIR)\BO_CAT_PROC_0801.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_1100.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_1100.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_1100.obj"	"$(INTDIR)\BO_CAT_PROC_1100.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_1200.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_1200.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_1200.obj"	"$(INTDIR)\BO_CAT_PROC_1200.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_1300.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_1300.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_1300.obj"	"$(INTDIR)\BO_CAT_PROC_1300.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_AcqNumber.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_AcqNumber.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_AcqNumber.obj"	"$(INTDIR)\BO_CAT_PROC_AcqNumber.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\BO_CAT_PROC_Api.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\BO_CAT_PROC_Api.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\BO_CAT_PROC_Api.obj"	"$(INTDIR)\BO_CAT_PROC_Api.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\Kolas2up_Card.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\Kolas2up_Card.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\Kolas2up_Card.obj"	"$(INTDIR)\Kolas2up_Card.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\Kolas2up_CardList.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\Kolas2up_CardList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\Kolas2up_CardList.obj"	"$(INTDIR)\Kolas2up_CardList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\SPF.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"


"$(INTDIR)\SPF.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"


"$(INTDIR)\SPF.obj"	"$(INTDIR)\SPF.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BO_CAT_PROC.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "BO_CAT_PROC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\BO_CAT_PROC.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\BO_CAT_PROC.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "BO_CAT_PROC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\BO_CAT_PROC.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\BO_CAT_PROC.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

