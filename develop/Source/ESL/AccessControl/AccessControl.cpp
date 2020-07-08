// AccessControl.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AccessControl.h"

#include "MainFrm.h"
#include "AccessControlDoc.h"
#include "AccessControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlApp

BEGIN_MESSAGE_MAP(CAccessControlApp, CWinApp)
	//{{AFX_MSG_MAP(CAccessControlApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlApp construction

CAccessControlApp::CAccessControlApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAccessControlApp object

CAccessControlApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAccessControlApp initialization

BOOL CAccessControlApp::InitInstance()
{
	AfxEnableControlContainer();

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
		RUNTIME_CLASS(CAccessControlDoc),
		RUNTIME_CLASS(CMainFrame),       // _tmain SDI frame window
		RUNTIME_CLASS(CAccessControlView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

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
	DWORD count;
	CWnd * GetWnd(VOID);

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg VOID OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	count = 0;
}

VOID CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
VOID CAccessControlApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAccessControlApp message handlers


VOID CAboutDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0) {
		GetDlgItem(IDC_STATIC1)->ShowWindow(false);
		GetDlgItem(IDC_STATIC2)->ShowWindow(false);
		GetDlgItem(IDC_STATIC3)->ShowWindow(false);
		GetDlgItem(IDC_STATIC4)->ShowWindow(false);
		GetDlgItem(IDC_STATIC5)->ShowWindow(false);
		GetDlgItem(IDC_STATIC6)->ShowWindow(false);
		GetDlgItem(IDC_STATIC7)->ShowWindow(false);
		GetDlgItem(IDC_STATIC8)->ShowWindow(false);
		GetDlgItem(IDC_STATIC9)->ShowWindow(false);
		GetDlgItem(IDC_STATIC10)->ShowWindow(false);
		
		count++;

		GetWnd()->ShowWindow(true);
	}
	CDialog::OnTimer(nIDEvent);
}

CWnd * CAboutDlg::GetWnd(VOID)
{
	if (count == 10) count = 0;
	if (count % 10 == 1-1) return GetDlgItem(IDC_STATIC1);
	if (count % 10 == 2-1) return GetDlgItem(IDC_STATIC2);
	if (count % 10 == 3-1) return GetDlgItem(IDC_STATIC3);
	if (count % 10 == 4-1) return GetDlgItem(IDC_STATIC4);
	if (count % 10 == 5-1) return GetDlgItem(IDC_STATIC5);
	if (count % 10 == 6-1) return GetDlgItem(IDC_STATIC6);
	if (count % 10 == 7-1) return GetDlgItem(IDC_STATIC7);
	if (count % 10 == 8-1) return GetDlgItem(IDC_STATIC8);
	if (count % 10 == 9-1) return GetDlgItem(IDC_STATIC9);
	if (count % 10 == 10-1) return GetDlgItem(IDC_STATIC10);

	return NULL;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	INT x = 15;
	INT y = 20;

	GetDlgItem(IDC_STATIC1)->MoveWindow(x, y, 32, 32, true);
	GetDlgItem(IDC_STATIC2)->MoveWindow(x, y, 32, 32, true);
	GetDlgItem(IDC_STATIC3)->MoveWindow(x, y, 32, 32, true);
	GetDlgItem(IDC_STATIC4)->MoveWindow(x, y, 32, 32, true);
	GetDlgItem(IDC_STATIC5)->MoveWindow(x, y, 32, 32, true);
	GetDlgItem(IDC_STATIC6)->MoveWindow(x, y, 32, 32, true);
	GetDlgItem(IDC_STATIC7)->MoveWindow(x, y, 32, 32, true);
	GetDlgItem(IDC_STATIC8)->MoveWindow(x, y, 32, 32, true);
	GetDlgItem(IDC_STATIC9)->MoveWindow(x, y, 32, 32, true);
	GetDlgItem(IDC_STATIC10)->MoveWindow(x, y, 32, 32, true);

	GetDlgItem(IDC_STATIC1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC3)->ShowWindow(false);
	GetDlgItem(IDC_STATIC4)->ShowWindow(false);
	GetDlgItem(IDC_STATIC5)->ShowWindow(false);
	GetDlgItem(IDC_STATIC6)->ShowWindow(false);
	GetDlgItem(IDC_STATIC7)->ShowWindow(false);
	GetDlgItem(IDC_STATIC8)->ShowWindow(false);
	GetDlgItem(IDC_STATIC9)->ShowWindow(false);
	GetDlgItem(IDC_STATIC10)->ShowWindow(false);

	GetDlgItem(IDC_STATIC1)->ShowWindow(true);

	SetTimer(0, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
