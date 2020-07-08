// KOLAS3_MAIN.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"

#include "MainFrm.h"
#include "KOLAS3_MAINDoc.h"
#include "KOLAS3_MAINView.h"

#include <tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKOLAS3_MAINApp

BEGIN_MESSAGE_MAP(CKOLAS3_MAINApp, CWinApp)
	//{{AFX_MSG_MAP(CKOLAS3_MAINApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	//ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKOLAS3_MAINApp construction




CKOLAS3_MAINApp::CKOLAS3_MAINApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
//	_crtBreakAlloc = 124876;
}

CKOLAS3_MAINApp::~CKOLAS3_MAINApp()
{
	
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKOLAS3_MAINApp object

CKOLAS3_MAINApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CKOLAS3_MAINApp initialization

BOOL CKOLAS3_MAINApp::InitInstance()
{
	// 2008.12.26 REM BY PDJ : 두개 이상 실행할 수 있도록 협의함.	
	//if ( CheckRun() ) return FALSE;

	DeleteJunkFile();
	
	AfxEnableControlContainer();

    WNDCLASS wndcls;
    //HINSTANCE hInst = AfxGetInstanceHandle();
    HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, GRIDCTRL_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
#ifndef _WIN32_WCE_NO_CURSOR
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
#else
        wndcls.hCursor          = 0;
#endif
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = GRIDCTRL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }



	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should _tremove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CKOLAS3_MAINDoc),
		RUNTIME_CLASS(CMainFrame),       // _tmain SDI frame window
		RUNTIME_CLASS(CKOLAS3_MAINView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);		

	// Dispatch commands specified on the command line
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;
    cmdInfo.m_strFileName= _T("");

	if (!ProcessShellCommand(cmdInfo))		
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	HWND hwnd = GetDesktopWindow();
	CRect RectDesktop;
	GetWindowRect(hwnd, &RectDesktop);
	if (RectDesktop.Height() <= 600) 
		m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	else 
		m_pMainWnd->ShowWindow(SW_SHOW);

	m_pMainWnd->UpdateWindow();

	// 리치에디트 이닛
	//AfxInitRichEdit();
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual VOID DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

VOID CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
VOID CKOLAS3_MAINApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CKOLAS3_MAINApp message handlers

BOOL CKOLAS3_MAINApp::PreTranslateMessage(MSG* pMsg) 
{		
	/*
	FILE *fp;

	fp = _tfopen (_T("WinMainMsg.txt"), _T("a+b"));
	if (fp == NULL)
	{
		return TRUE;
	}
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(fp, 0, SEEK_END);		
		if( 0 == ftell(fp) )
		{
			fputc(0xFF, fp);
			fputc(0xFE, fp);
		}
		// ------------------------------------------------------------------------------
	
	_ftprintf (fp, _T("%h, %d, %l\r\n"), pMsg->message, pMsg->wParam, pMsg->lParam);

	fclose(fp);
	*/

	if ( this->GetMainWnd() == NULL ) 
	{
		return TRUE;
	}
		
	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL CKOLAS3_MAINApp::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	/*
	FILE *fp;

	fp = _tfopen (_T("WinMainCmd.txt"), _T("a+b"));
	if (fp == NULL)
	{
		return TRUE;
	}
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(fp, 0, SEEK_END);		
		if( 0 == ftell(fp) )
		{
			fputc(0xFF, fp);
			fputc(0xFE, fp);
		}
		// ------------------------------------------------------------------------------
	
	_ftprintf (fp, _T("%d, %d\r\n"), nID, nCode);

	fclose(fp);	
	*/
	
	return CWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


INT CKOLAS3_MAINApp::GetExcuteProgramCount(CString strProgramName)
{
	HANDLE         hProcessSnap = NULL; 
	BOOL           bRet      = FALSE; 
	PROCESSENTRY32 pe32      = {0}; 
	INT nCount;
	
	//  Take a snapshot of all processes in the system. 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	
	if (hProcessSnap != INVALID_HANDLE_VALUE) 
	{		
		//  Fill in the size of the structure before using it. 
		pe32.dwSize = sizeof(PROCESSENTRY32); 
		
		//  Walk the snapshot of the processes, and for each process, 
		//  display information.     

		nCount = 0;
		if (Process32First(hProcessSnap, &pe32)) 
		{ 
			do 
			{ 					
				if(_tcsicmp(pe32.szExeFile, strProgramName)==0)
				{
					nCount++;
				} 				
			} 
			while (Process32Next(hProcessSnap, &pe32)); 
		}
		
		// Do not forget to clean up the snapshot object. 
		CloseHandle (hProcessSnap); 
	}

	return nCount;
}

INT CKOLAS3_MAINApp::DeleteJunkFile()
{
	HANDLE         hProcessSnap = NULL; 
	BOOL           bRet      = FALSE; 
	PROCESSENTRY32 pe32      = {0}; 
	INT nCount;
	CStringArray astrPID;
	CString strPID;

	//  Take a snapshot of all processes in the system. 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	
	if (hProcessSnap != INVALID_HANDLE_VALUE) 
	{		
		//  Fill in the size of the structure before using it. 
		pe32.dwSize = sizeof(PROCESSENTRY32); 
		
		//  Walk the snapshot of the processes, and for each process, 
		//  display information.     

		nCount = 0;
		if (Process32First(hProcessSnap, &pe32)) 
		{ 
			do 
			{ 					
				if(_tcsicmp(pe32.szExeFile, _T("Kalendar.exe")) == 0 ||
					_tcsicmp(pe32.szExeFile, _T("KOLASIII.exe"))==0 || 
					_tcsicmp(pe32.szExeFile, _T("RefEditor_uni.exe"))==0 ||
					_tcsicmp(pe32.szExeFile, _T("KOLASIII MOBILE.exe"))==0 ||
					_tcsicmp(pe32.szExeFile, _T("KIIITL_AutoSyncer.exe"))==0 ||				
					_tcsicmp(pe32.szExeFile, _T("ETLAS.exe"))==0
					)
				{
					strPID.Format(_T("(%u)"),  pe32.th32ProcessID);
					astrPID.Add(strPID);
					nCount++;
				} 				
			} 
			while (Process32Next(hProcessSnap, &pe32)); 
		}
		
		// Do not forget to clean up the snapshot object. 
		CloseHandle (hProcessSnap); 
	}
		
	CString strNotDeleteFile, strFileName, strFilePath;
	BOOL bWorking = FALSE, bDelete = FALSE;
	
	CFileFind FF;
	FF.FindFile(_T("..\\cfg\\TouchPad\\*(*)*"));
	
	const INT nDirectoryCnt = 2;
	const INT nArrCnt = astrPID.GetSize();
	CString astrFile[nDirectoryCnt] = 
	{
		_T("..\\cfg\\TouchPad\\*(*)*"),
			_T("..\\cfg\\EFS\\*(*)*")
	};
		
	for(INT nCnt = 0; nCnt < nDirectoryCnt; nCnt++)
	{
		bWorking = FF.FindFile(astrFile[nCnt]);		
		while (bWorking)
		{
			bWorking = FF.FindNextFile();
			
			if(FF.IsDots() || FF.IsDirectory())
			{
				continue;
			}
			
			bDelete = TRUE;
			strFileName = FF.GetFileName();
			for(INT nCnt2 = 0; nCnt2 < nArrCnt; nCnt2++)
			{
				strNotDeleteFile = astrPID.GetAt(nCnt2);
				if(-1 != strFileName.Find(strNotDeleteFile))
				{
					bDelete = FALSE;
					break;
				}
			}
			if(bDelete)
			{
				strFilePath = FF.GetFilePath();
				::DeleteFile(strFilePath);
			}
		}
	}


	if(1 >= astrPID.GetSize())
	{
		::DeleteFile(_T("..\\cfg\\TouchPad\\Run"));
	}
	

	return nCount;
}
