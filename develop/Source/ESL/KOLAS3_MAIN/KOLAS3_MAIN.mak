# Microsoft Developer Studio Generated NMAKE File, Based on KOLAS3_MAIN.dsp
!IF "$(CFG)" == ""
CFG=KOLAS3_MAIN - Win32 Debug
!MESSAGE No configuration specified. Defaulting to KOLAS3_MAIN - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "KOLAS3_MAIN - Win32 Release" && "$(CFG)" != "KOLAS3_MAIN - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KOLAS3_MAIN.mak" CFG="KOLAS3_MAIN - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KOLAS3_MAIN - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "KOLAS3_MAIN - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\KOLAS3_MAIN.exe"


CLEAN :
	-@erase "$(INTDIR)\BCMenu.obj"
	-@erase "$(INTDIR)\Form.obj"
	-@erase "$(INTDIR)\KOLAS3_MAIN.obj"
	-@erase "$(INTDIR)\KOLAS3_MAIN.pch"
	-@erase "$(INTDIR)\KOLAS3_MAIN.res"
	-@erase "$(INTDIR)\KOLAS3_MAINApi.obj"
	-@erase "$(INTDIR)\KOLAS3_MAINDoc.obj"
	-@erase "$(INTDIR)\KOLAS3_MAINView.obj"
	-@erase "$(INTDIR)\LoadBmp.obj"
	-@erase "$(INTDIR)\LoginDlg.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainObject.obj"
	-@erase "$(INTDIR)\MenuItem.obj"
	-@erase "$(INTDIR)\MToolBar.obj"
	-@erase "$(INTDIR)\NewButton.obj"
	-@erase "$(INTDIR)\RCDlg.obj"
	-@erase "$(INTDIR)\RCFrame.obj"
	-@erase "$(INTDIR)\RCThread.obj"
	-@erase "$(INTDIR)\RCView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TextProgressCtrl.obj"
	-@erase "$(INTDIR)\ToolBarItem.obj"
	-@erase "$(INTDIR)\TrayIcon.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\KOLAS3_MAIN.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\KOLAS3_MAIN.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\KOLAS3_MAIN.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\KOLAS3_MAIN.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\KOLAS3_MAIN.pdb" /machine:I386 /out:"$(OUTDIR)\KOLAS3_MAIN.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BCMenu.obj" \
	"$(INTDIR)\Form.obj" \
	"$(INTDIR)\KOLAS3_MAIN.obj" \
	"$(INTDIR)\KOLAS3_MAINApi.obj" \
	"$(INTDIR)\KOLAS3_MAINDoc.obj" \
	"$(INTDIR)\KOLAS3_MAINView.obj" \
	"$(INTDIR)\LoadBmp.obj" \
	"$(INTDIR)\LoginDlg.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MainObject.obj" \
	"$(INTDIR)\MenuItem.obj" \
	"$(INTDIR)\MToolBar.obj" \
	"$(INTDIR)\NewButton.obj" \
	"$(INTDIR)\RCDlg.obj" \
	"$(INTDIR)\RCFrame.obj" \
	"$(INTDIR)\RCThread.obj" \
	"$(INTDIR)\RCView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TextProgressCtrl.obj" \
	"$(INTDIR)\ToolBarItem.obj" \
	"$(INTDIR)\TrayIcon.obj" \
	"$(INTDIR)\KOLAS3_MAIN.res"

"$(OUTDIR)\KOLAS3_MAIN.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\..\..\bin\KOLAS II Light.exe" "$(OUTDIR)\KOLAS3_MAIN.bsc"


CLEAN :
	-@erase "$(INTDIR)\BCMenu.obj"
	-@erase "$(INTDIR)\BCMenu.sbr"
	-@erase "$(INTDIR)\Form.obj"
	-@erase "$(INTDIR)\Form.sbr"
	-@erase "$(INTDIR)\KOLAS3_MAIN.obj"
	-@erase "$(INTDIR)\KOLAS3_MAIN.pch"
	-@erase "$(INTDIR)\KOLAS3_MAIN.res"
	-@erase "$(INTDIR)\KOLAS3_MAIN.sbr"
	-@erase "$(INTDIR)\KOLAS3_MAINApi.obj"
	-@erase "$(INTDIR)\KOLAS3_MAINApi.sbr"
	-@erase "$(INTDIR)\KOLAS3_MAINDoc.obj"
	-@erase "$(INTDIR)\KOLAS3_MAINDoc.sbr"
	-@erase "$(INTDIR)\KOLAS3_MAINView.obj"
	-@erase "$(INTDIR)\KOLAS3_MAINView.sbr"
	-@erase "$(INTDIR)\LoadBmp.obj"
	-@erase "$(INTDIR)\LoadBmp.sbr"
	-@erase "$(INTDIR)\LoginDlg.obj"
	-@erase "$(INTDIR)\LoginDlg.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MainObject.obj"
	-@erase "$(INTDIR)\MainObject.sbr"
	-@erase "$(INTDIR)\MenuItem.obj"
	-@erase "$(INTDIR)\MenuItem.sbr"
	-@erase "$(INTDIR)\MToolBar.obj"
	-@erase "$(INTDIR)\MToolBar.sbr"
	-@erase "$(INTDIR)\NewButton.obj"
	-@erase "$(INTDIR)\NewButton.sbr"
	-@erase "$(INTDIR)\RCDlg.obj"
	-@erase "$(INTDIR)\RCDlg.sbr"
	-@erase "$(INTDIR)\RCFrame.obj"
	-@erase "$(INTDIR)\RCFrame.sbr"
	-@erase "$(INTDIR)\RCThread.obj"
	-@erase "$(INTDIR)\RCThread.sbr"
	-@erase "$(INTDIR)\RCView.obj"
	-@erase "$(INTDIR)\RCView.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TextProgressCtrl.obj"
	-@erase "$(INTDIR)\TextProgressCtrl.sbr"
	-@erase "$(INTDIR)\ToolBarItem.obj"
	-@erase "$(INTDIR)\ToolBarItem.sbr"
	-@erase "$(INTDIR)\TrayIcon.obj"
	-@erase "$(INTDIR)\TrayIcon.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\KOLAS II Light.pdb"
	-@erase "$(OUTDIR)\KOLAS3_MAIN.bsc"
	-@erase "..\..\..\..\bin\KOLAS II Light.exe"
	-@erase "..\..\..\..\bin\KOLAS II Light.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\KOLAS3_MAIN.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\KOLAS3_MAIN.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\KOLAS3_MAIN.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BCMenu.sbr" \
	"$(INTDIR)\Form.sbr" \
	"$(INTDIR)\KOLAS3_MAIN.sbr" \
	"$(INTDIR)\KOLAS3_MAINApi.sbr" \
	"$(INTDIR)\KOLAS3_MAINDoc.sbr" \
	"$(INTDIR)\KOLAS3_MAINView.sbr" \
	"$(INTDIR)\LoadBmp.sbr" \
	"$(INTDIR)\LoginDlg.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\MainObject.sbr" \
	"$(INTDIR)\MenuItem.sbr" \
	"$(INTDIR)\MToolBar.sbr" \
	"$(INTDIR)\NewButton.sbr" \
	"$(INTDIR)\RCDlg.sbr" \
	"$(INTDIR)\RCFrame.sbr" \
	"$(INTDIR)\RCThread.sbr" \
	"$(INTDIR)\RCView.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TextProgressCtrl.sbr" \
	"$(INTDIR)\ToolBarItem.sbr" \
	"$(INTDIR)\TrayIcon.sbr"

"$(OUTDIR)\KOLAS3_MAIN.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=imm32.lib esl.lib ocimgr.lib Ws2_32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\KOLAS II Light.pdb" /debug /machine:I386 /out:"..\..\..\..\bin\KOLAS II Light.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\BCMenu.obj" \
	"$(INTDIR)\Form.obj" \
	"$(INTDIR)\KOLAS3_MAIN.obj" \
	"$(INTDIR)\KOLAS3_MAINApi.obj" \
	"$(INTDIR)\KOLAS3_MAINDoc.obj" \
	"$(INTDIR)\KOLAS3_MAINView.obj" \
	"$(INTDIR)\LoadBmp.obj" \
	"$(INTDIR)\LoginDlg.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MainObject.obj" \
	"$(INTDIR)\MenuItem.obj" \
	"$(INTDIR)\MToolBar.obj" \
	"$(INTDIR)\NewButton.obj" \
	"$(INTDIR)\RCDlg.obj" \
	"$(INTDIR)\RCFrame.obj" \
	"$(INTDIR)\RCThread.obj" \
	"$(INTDIR)\RCView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TextProgressCtrl.obj" \
	"$(INTDIR)\ToolBarItem.obj" \
	"$(INTDIR)\TrayIcon.obj" \
	"$(INTDIR)\KOLAS3_MAIN.res"

"..\..\..\..\bin\KOLAS II Light.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("KOLAS3_MAIN.dep")
!INCLUDE "KOLAS3_MAIN.dep"
!ELSE 
!MESSAGE Warning: cannot find "KOLAS3_MAIN.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "KOLAS3_MAIN - Win32 Release" || "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"
SOURCE=.\BCMenu.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\BCMenu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\BCMenu.obj"	"$(INTDIR)\BCMenu.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\Form.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\Form.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\Form.obj"	"$(INTDIR)\Form.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\KOLAS3_MAIN.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\KOLAS3_MAIN.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\KOLAS3_MAIN.obj"	"$(INTDIR)\KOLAS3_MAIN.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\KOLAS3_MAIN.rc

"$(INTDIR)\KOLAS3_MAIN.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\KOLAS3_MAINApi.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\KOLAS3_MAINApi.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\KOLAS3_MAINApi.obj"	"$(INTDIR)\KOLAS3_MAINApi.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\KOLAS3_MAINDoc.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\KOLAS3_MAINDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\KOLAS3_MAINDoc.obj"	"$(INTDIR)\KOLAS3_MAINDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\KOLAS3_MAINView.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\KOLAS3_MAINView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\KOLAS3_MAINView.obj"	"$(INTDIR)\KOLAS3_MAINView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\LoadBmp.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\LoadBmp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\LoadBmp.obj"	"$(INTDIR)\LoadBmp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\LoginDlg.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\LoginDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\LoginDlg.obj"	"$(INTDIR)\LoginDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\MainObject.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\MainObject.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\MainObject.obj"	"$(INTDIR)\MainObject.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\MenuItem.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\MenuItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\MenuItem.obj"	"$(INTDIR)\MenuItem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\MToolBar.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\MToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\MToolBar.obj"	"$(INTDIR)\MToolBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\NewButton.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\NewButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\NewButton.obj"	"$(INTDIR)\NewButton.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\RCDlg.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\RCDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\RCDlg.obj"	"$(INTDIR)\RCDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\RCFrame.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\RCFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\RCFrame.obj"	"$(INTDIR)\RCFrame.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\RCThread.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\RCThread.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\RCThread.obj"	"$(INTDIR)\RCThread.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\RCView.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\RCView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\RCView.obj"	"$(INTDIR)\RCView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\KOLAS3_MAIN.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\KOLAS3_MAIN.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\KOLAS3_MAIN.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\KOLAS3_MAIN.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TextProgressCtrl.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\TextProgressCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\TextProgressCtrl.obj"	"$(INTDIR)\TextProgressCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\ToolBarItem.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\ToolBarItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\ToolBarItem.obj"	"$(INTDIR)\ToolBarItem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 

SOURCE=.\TrayIcon.cpp

!IF  "$(CFG)" == "KOLAS3_MAIN - Win32 Release"


"$(INTDIR)\TrayIcon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ELSEIF  "$(CFG)" == "KOLAS3_MAIN - Win32 Debug"


"$(INTDIR)\TrayIcon.obj"	"$(INTDIR)\TrayIcon.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KOLAS3_MAIN.pch"


!ENDIF 


!ENDIF 

