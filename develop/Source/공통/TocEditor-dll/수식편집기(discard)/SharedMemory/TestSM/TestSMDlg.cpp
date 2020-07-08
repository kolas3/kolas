// TestSMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestSM.h"
#include "TestSMDlg.h"

#include "registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "TocEditorDef.h"
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

/////////////////////////////////////////////////////////////////////////////
// CTestSMDlg dialog

CTestSMDlg::CTestSMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestSMDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestSMDlg)
	m_sWrite = _T("");
	m_sRead = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// KOL.UDF.022 시큐어코딩 보완
	m_hwnd = NULL;
}

VOID CTestSMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestSMDlg)
	DDX_Text(pDX, IDC_EDIT_WRITEVALUE, m_sWrite);
	DDX_Text(pDX, IDC_EDIT_READVALUE, m_sRead);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestSMDlg, CDialog)
	//{{AFX_MSG_MAP(CTestSMDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CREATE, OnButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, OnButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_READ, OnButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_WRITETOQUEUE, OnButtonWritetoqueue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestSMDlg message handlers

BOOL CTestSMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add _T("About...") menu item to _tsystem menu.

	// IDM_ABOUTBOX must be in the _tsystem command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	
/*
	try {
		CRegistry regConfig;
		DWORD nLen;
		DWORD dwSomeDWData;

		regConfig.Open( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\(주)ECO\\EcoMath\\") );

		regConfig.Read(( LPBYTE )&dwSomeDWData, ( nLen=sizeof( dwSomeDWData ), &nLen ), NULL, _T("ID") );
		m_hwnd = (HWND)dwSomeDWData;
		regConfig.Close();
	}
	catch( CRegistryException *regError ) {
		// display the error
		regError->ReportError();

		// now delete the handle
		regError->Delete();

		return false;
	}
*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CTestSMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CTestSMDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		INT cxIcon = GetSystemMetrics(SM_CXICON);
		INT cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		INT x = (rect.Width() - cxIcon + 1) / 2;
		INT y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The _tsystem calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestSMDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CTestSMDlg::OnButtonCreate() 
{

	if(ExecuteEcoMath() == true) {
		if(WaitForInputIdle( m_pi.hProcess, INFINITE ) != 0) {
			AfxMessageBox(_T("Wait Fail"));
			return;
		}

#define ECOMATH_CONNECT	0
#define ECOMATH_INIT	1
#define ECOMATH_INITOK	2
#define ECOMATH_GETLEN	3
#define ECOMATH_GETMATH	4
#define ECOMATH_EXIT	99

		if(SendMessageToProcess(&m_pi, WM_ECOMATH, ECOMATH_CONNECT, 0) == -1) {
			AfxMessageBox(_T("Connect Error"));
			return;
		}

		if(WaitForInputIdle( m_pi.hProcess, INFINITE ) != 0) {
			AfxMessageBox(_T("Wait Fail"));
			return;
		}

		if(PostMessageToProcess(&m_pi, WM_ECOMATH, ECOMATH_INIT, 0) == -1) {
			AfxMessageBox(_T("Connect Error"));
			return;
		}

		m_cSharedMemory.Open(_T("TestPool"), 20);

		if(PostMessageToProcess(&m_pi, WM_ECOMATH, ECOMATH_INITOK, 0) == -1) {
			AfxMessageBox(_T("INIT Fail"));
			return;
		}
/*
		if(PostMessageToProcess(&m_pi, WM_ECOMATH, ECOMATH_GETMATH, 0) == -1) {
			AfxMessageBox(_T("ECOMATH_GETMATH Error"));
			return;
		}

		OnButtonRead();
		/// ECOMATH를 종료함.
		if(SendMessageToProcess(&m_pi, WM_ECOMATH, ECOMATH_EXIT, 0) == -1) {
			AfxMessageBox(_T("Exit Error"));
			return;
		}
*/
	}
}

VOID CTestSMDlg::OnButtonRemove() 
{
	m_cSharedMemory.Close();
}

VOID CTestSMDlg::OnButtonWrite() 
{
	UpdateData(true);

	switch(m_cSharedMemory.Write(m_sWrite.GetBuffer(1), 20, 5000))
	{
	case CSharedMemory::MEM_SUCCESS:
		break;
	case CSharedMemory::MEM_ERROR_CLOSED:
		AfxMessageBox(_T("Not opened"));
		break;
	case CSharedMemory::MEM_ERROR_TIMEOUT:
		AfxMessageBox(_T("Timeout"));
		break;
	case CSharedMemory::MEM_ERROR_OTHERPARTY:
		AfxMessageBox(_T("Other party disconnected"));
		m_cSharedMemory.Close();
		break;
	case CSharedMemory::MEM_ERROR_UNKNOWN:
		AfxMessageBox(_T("Unknown"));
		break;
	}
}

VOID CTestSMDlg::OnButtonWritetoqueue() 
{
	UpdateData(true);

	switch(m_cSharedMemory.WriteToQueue(m_sWrite.GetBuffer(1), 20))
	{
	case CSharedMemory::MEM_SUCCESS:
		break;
	case CSharedMemory::MEM_ERROR_CLOSED:
		AfxMessageBox(_T("Not opened"));
		break;
	case CSharedMemory::MEM_ERROR_OTHERPARTY:
		AfxMessageBox(_T("Other party disconnected"));
		m_cSharedMemory.Close();
		break;
	case CSharedMemory::MEM_ERROR_UNKNOWN:
		AfxMessageBox(_T("Unknown"));
		break;
	}
}

VOID CTestSMDlg::OnButtonRead() 
{
	TCHAR	sBuf[21];
	INT len;
	INT len2read = 0;

	if(WaitForInputIdle( m_pi.hProcess, INFINITE ) != 0) {
		AfxMessageBox(_T("Wait Fail"));
		return;
	}

	len = SendMessageToProcess(&m_pi, WM_ECOMATH, ECOMATH_GETLEN, 0);

	if(PostMessageToProcess(&m_pi, WM_ECOMATH, ECOMATH_GETMATH, 0) == -1) {
		AfxMessageBox(_T("ECOMATH_GETMATH Error"));
		return;
	}

	m_sRead.Empty();

	len2read = 20;

	for(INT loop = 0; loop < (len / 20 + 1); loop++) {
		if(loop == (len / 20)) {
			len2read = len%20;
		}
		wmemset(sBuf, 0, sizeof(sBuf));
		switch (m_cSharedMemory.Read(sBuf, len2read, 10000))
		{
		case CSharedMemory::MEM_SUCCESS:
			m_sRead += sBuf;
			break;
		case CSharedMemory::MEM_ERROR_CLOSED:
			AfxMessageBox(_T("Not opened"));
			break;
		case CSharedMemory::MEM_ERROR_TIMEOUT:
			AfxMessageBox(_T("Timeout"));
			break;
		case CSharedMemory::MEM_ERROR_OTHERPARTY:
			AfxMessageBox(_T("Other party disconnected"));
			m_cSharedMemory.Close();
			break;
		case CSharedMemory::MEM_ERROR_UNKNOWN:
			AfxMessageBox(_T("Unknown"));
			break;
		}
	}
	UpdateData(false);
}

bool CTestSMDlg::ExecuteEcoMath()
{

    STARTUPINFO si;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &m_pi, sizeof(m_pi) );

    // Start the child process. 
    if( !CreateProcess( NULL,		// No module name (use command line). 
//        _T("c:\\개발\\Tex\\EcoMath\\Debug\\EcoMath.exe"),	// Command line. 
        _T("\\kolis\\EXE\\EcoMath.exe"),	// Command line. 
        NULL,						// Process handle not inheritable. 
        NULL,						// Thread handle not inheritable. 
        FALSE,						// Set handle inheritance to FALSE. 
        0,							// No creation flags. 
        NULL,						// Use parent's environment block. 
        NULL,						// Use parent's starting directory. 
        &si,						// Pointer to STARTUPINFO structure.
        &m_pi )						// Pointer to PROCESS_INFORMATION structure.
    ) 
    {
        AfxMessageBox( _T("CreateProcess failed.") );
    }

	return true;
}

typedef struct _FINDWINDOWHANDLESTRUCT {
LPPROCESS_INFORMATION ProcessInfo;
HWND hWndFound; 
} FINDWINDOWHANDLESTRUCT;

INT CTestSMDlg::SendMessageToProcess(LPPROCESS_INFORMATION lpProcessInformation, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	FINDWINDOWHANDLESTRUCT fwhs;
	fwhs.ProcessInfo = lpProcessInformation;
	fwhs.hWndFound  = NULL;

	EnumWindows ( EnumWindowCallBack, (LPARAM)&fwhs ) ;

	return ::SendMessage ( fwhs.hWndFound, Msg, wParam, lParam );
}


INT CTestSMDlg::PostMessageToProcess(LPPROCESS_INFORMATION lpProcessInformation, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	FINDWINDOWHANDLESTRUCT fwhs;
	fwhs.ProcessInfo = lpProcessInformation;
	fwhs.hWndFound  = NULL;

	EnumWindows ( EnumWindowCallBack, (LPARAM)&fwhs ) ;

	return ::PostMessage ( fwhs.hWndFound, Msg, wParam, lParam );
}

BOOL CALLBACK CTestSMDlg::EnumWindowCallBack(HWND hwnd, LPARAM lParam)
{
	FINDWINDOWHANDLESTRUCT * pfwhs = (FINDWINDOWHANDLESTRUCT * )lParam; 
	DWORD ProcessId; 
	CString Title; 
	GetWindowThreadProcessId ( hwnd, &ProcessId ); 

	// note: In order to make sure we have the MainFrame, verify that the title 
	// has Zero-Length. Under Windows 98, sometimes the Client Window ( which doesn't 
	// have a title ) is enumerated before the MainFrame 

	CWnd::FromHandle( hwnd )->GetWindowText(Title);
	if ( ProcessId  == pfwhs->ProcessInfo->dwProcessId && Title.GetLength() != 0) 
	{ 
		pfwhs->hWndFound = hwnd; 
		return false; 
	} 
	else 
	{ 
		// Keep enumerating 
		return true; 
	} 
}

