// KLRFIDServiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KLRFIDService.h"
#include "KLRFIDServiceDlg.h"
#include "StdioFileEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
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
// CKLRFIDServiceDlg dialog

CKLRFIDServiceDlg::CKLRFIDServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKLRFIDServiceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKLRFIDServiceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKLRFIDServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKLRFIDServiceDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKLRFIDServiceDlg, CDialog)
	//{{AFX_MSG_MAP(CKLRFIDServiceDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+1024, OnTrayIconNotify)
	ON_COMMAND_RANGE(ID_1, ID_7, OnMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKLRFIDServiceDlg message handlers

BOOL CKLRFIDServiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	ShowWindow(FALSE);

	INT nPort = 10062;
	CString strLine, strAlias, strData;
	CStdioFileEx File;
	if(File.Open(_T("..\\cfg\\KL_ServiceInfo.cfg"), CFile::typeText|CFile::modeRead))
	{
		while(File.ReadString(strLine))
		{
			AfxExtractSubString(strAlias, strLine, 0, '=');
			AfxExtractSubString(strData, strLine, 1, '=');
			if(_T("PORT") == strAlias)
			{
				nPort = _ttoi(strData);
			}
		}
		File.Close();
	}

	m_Tray.Create((CWnd*)this, WM_USER+1024, _T("통합회원 RFID등록 서비스"), m_hIcon, IDR_MAINFRAME);	
	m_Service.StartService(nPort);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKLRFIDServiceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CKLRFIDServiceDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKLRFIDServiceDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LONG CKLRFIDServiceDlg::OnTrayIconNotify(WPARAM wParam, LPARAM lParam)
{
// 	switch(lParam)
// 	{
// 	case WM_LBUTTONDOWN :	
// 		{
// 			UINT uMenuID = IDR_LMENU;
// 			POINT	point;
//  			CMenu  *sMenu = NULL, *sMenu2 = NULL;
// 			UINT nCheck = MF_UNCHECKED;
// 
// 			CMenu Menu;
// 			Menu.LoadMenu(uMenuID);
// 			sMenu = Menu.GetSubMenu(0);
// 
// 			GetCursorPos(&point);
// 
// 			::SetForegroundWindow(this->m_hWnd);  
// 			::TrackPopupMenu(sMenu->m_hMenu, 0, point.x, point.y, 0, this->m_hWnd, NULL);
// 			sMenu->DestroyMenu();
// 		}
// 		break;
// 
// 	case WM_RBUTTONDOWN:		
// 		m_Tray.RemoveIcon();
// 		PostMessage(WM_QUIT, NULL, NULL);
// 		break;
// 
// 	default:
// 		break;
// 	}	

	return TRUE;
}

void CKLRFIDServiceDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
	lpwndpos->flags &= ~SWP_SHOWWINDOW; 
}

LRESULT CKLRFIDServiceDlg::OnMenu(WPARAM wParam, LPARAM lParam)
{
	return 0;
}