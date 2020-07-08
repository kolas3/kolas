// SHELF_LOC_REGISTERDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SHELF_LOC_REGISTER.h"
#include "SHELF_LOC_REGISTERDlg.h"
#include "..\..\공통\IndexToolKit\IndexTool.h"

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSHELF_LOC_REGISTERDlg dialog

CSHELF_LOC_REGISTERDlg::CSHELF_LOC_REGISTERDlg(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSHELF_LOC_REGISTERDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_initShow = 0;
}
CSHELF_LOC_REGISTERDlg::~CSHELF_LOC_REGISTERDlg()
{
	
}

void CSHELF_LOC_REGISTERDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSHELF_LOC_REGISTERDlg)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSHELF_LOC_REGISTERDlg, CDialog)
	//{{AFX_MSG_MAP(CSHELF_LOC_REGISTERDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSHELF_LOC_REGISTERDlg message handlers

BOOL CSHELF_LOC_REGISTERDlg::OnInitDialog()
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

	m_pInfo = new CESL_Information;
	// IndexToolKit을 붙여준다.
//	CIndexTool *pIndexTool = new CIndexTool;
//	m_pInfo->pIndexTool = (CObject*)pIndexTool;


	CStdioFile file_stdio;
	CString strLine;
	if(file_stdio.Open(_T("..\\CFG\\서가위치정보\\DMINFO.cfg"), CStdioFile::modeReadWrite|CStdioFile::typeBinary|CStdioFile::shareDenyNone))
	{
		TCHAR cUni;
		file_stdio.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file_stdio.SeekToBegin();
		}
		file_stdio.ReadString(strLine);
	}
	else
	{
		AfxMessageBox(_T("DB접속 정보가 잘못되었습니다."));
	}

	m_pInfo->CONNECTION_INFO = (strLine);	

	if (InitESL_Mgr(_T("SM_SHELF_LOC_REG")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_strSMName=_T("SM_SHELF_LOC_REG");

	m_tab.InsertItem(SHELF_LOC_REG, _T(" 서가 등록 ") );
	m_tab.InsertItem(SHELF_LOC_SEARCH, _T(" 서가 조회 ") );

	CreateDlgs();
	
	m_DM.SetCONNECTION_INFO(strLine);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSHELF_LOC_REGISTERDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSHELF_LOC_REGISTERDlg::OnPaint() 
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
HCURSOR CSHELF_LOC_REGISTERDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


HBRUSH CSHELF_LOC_REGISTERDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

VOID CSHELF_LOC_REGISTERDlg::CreateDlgs()
{
	m_ShelfLocRegDlg = new CShelfLocRegDlg( this );
	m_ShelfLocRegDlg->Create( IDD_SHELF_LOC_REG, this );

	m_ShelfSearchDlg = new CShelfSearchDlg(this);
	m_ShelfSearchDlg->Create( IDD_SHELF_SEARCH,this );
}

void CSHELF_LOC_REGISTERDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_tab.SetCurSel( m_initShow );
	ShowDlg(m_initShow);
}

void CSHELF_LOC_REGISTERDlg::ShowDlg( int idx )
{
	const int x = 14;
	const int y = 31;
	const int length = 525;
	const int height = 250;
	CRect rect( x, y, x + length, y + height );
	
	switch( idx )
 	{
 		case 0 :
 				
 			m_ShelfLocRegDlg->MoveWindow( rect );
 			m_ShelfLocRegDlg->ShowWindow( true );
			m_ShelfLocRegDlg->SetFocus();
 
			m_ShelfSearchDlg->ShowWindow( false );
 
 			break;
 
 		case 1 :
 
   			m_ShelfSearchDlg->MoveWindow( rect );
   			m_ShelfSearchDlg->ShowWindow( true );
			m_ShelfSearchDlg->SetFocus();
  
  			m_ShelfLocRegDlg->ShowWindow( false );

			m_ShelfSearchDlg->m_strManageCode = this->m_strManageCode;
			m_ShelfSearchDlg->SetManageCode();
 
 			break;
	}
}

void CSHELF_LOC_REGISTERDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ShowDlg( m_tab.GetCurSel() );
	*pResult = 0;
}

void CSHELF_LOC_REGISTERDlg::DlgQuit()
{
	this->OnOK();
}

BOOL CSHELF_LOC_REGISTERDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->lParam == VK_RETURN )
		{
			return TRUE;
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);
}
