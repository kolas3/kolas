// IndexGeneratorDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "IndexGenerator.h"
#include "IndexGeneratorDlg.h"

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
// CIndexGeneratorDlg dialog

CIndexGeneratorDlg::CIndexGeneratorDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CIndexGeneratorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexGeneratorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pSearchMonoDlg = NULL;
	m_pSearchSerialDlg = NULL;
	m_Dlg_brush.CreateSolidBrush(RGB(244,243,238));
}

CIndexGeneratorDlg::~CIndexGeneratorDlg()
{
	if( m_pSearchMonoDlg != NULL)
	{
		delete m_pSearchMonoDlg;
		m_pSearchMonoDlg = NULL;
	}

	if( m_pSearchSerialDlg != NULL)
	{
		delete m_pSearchSerialDlg;
		m_pSearchSerialDlg = NULL;
	}

	if( NULL != m_pInfo )
	{
		delete m_pInfo;
		m_pInfo = NULL;
	}

	m_Dlg_brush.DeleteObject();
}

void CIndexGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexGeneratorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP	
}

BEGIN_MESSAGE_MAP(CIndexGeneratorDlg, CDialog)
	//{{AFX_MSG_MAP(CIndexGeneratorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabMonoSerial, OnSelchangetabMonoSerial)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(DLG_CLOSE, CloseDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexGeneratorDlg message handlers

BOOL CIndexGeneratorDlg::OnInitDialog()
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

	m_pInfo = new CESL_Information();

	// TODO: Add extra initialization here
	CTabCtrl *ptabMonoSerial = (CTabCtrl*)GetDlgItem(IDC_tabMonoSerial);
	if (ptabMonoSerial != NULL)
	{
		CString strItemData;
		strItemData.Format( _T("단행"));
		ptabMonoSerial->InsertItem(0, strItemData);
//		strItemData.Format( _T("연속"));
//		ptabMonoSerial->InsertItem(1, strItemData);

		ptabMonoSerial->SetCurSel(0);
	}

	InitSearchDlg();

	//EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIndexGeneratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIndexGeneratorDlg::OnPaint() 
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
HCURSOR CIndexGeneratorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIndexGeneratorDlg::OnSelchangetabMonoSerial(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CTabCtrl *ptabMonoSerial = (CTabCtrl*)GetDlgItem(IDC_tabMonoSerial);
	if (ptabMonoSerial == NULL)
	{
		return;
	}
	
	INT nCurSel = ptabMonoSerial->GetCurSel();

//	CESL_Grid *pgridCollectMarclist = (CESL_Grid*)GetDlgItem(IDC_gridCollectMarclist);
//	CESL_Grid *pgridFailList = (CESL_Grid*)GetDlgItem(IDC_gridFailList);
//	CEdit *pstcCollectMarclist = (CEdit*)GetDlgItem(IDC_stcCollectMarclist);
//	CEdit *pstcMarcView = (CEdit*)GetDlgItem(IDC_stcMarcView);
//	CRichEditCtrl* prichSuccessMarc = (CRichEditCtrl*)GetDlgItem(IDC_richSuccessMarc);
//	CRichEditCtrl* prichTargetMarc = (CRichEditCtrl*)GetDlgItem(IDC_richTargetMarc);
//
//	if ( pgridCollectMarclist == NULL || pstcCollectMarclist == NULL || pstcMarcView == NULL ||
//		 prichSuccessMarc == NULL || prichTargetMarc == NULL ) 
//	{
//		return;
//	}

	if (nCurSel == 0)
	{
		m_pSearchMonoDlg->ShowWindow(SW_SHOW);
		m_pSearchSerialDlg->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 1)
	{
		m_pSearchMonoDlg->ShowWindow(SW_HIDE);
		m_pSearchSerialDlg->ShowWindow(SW_SHOW);
	}
	
	*pResult = 0;
}


VOID CIndexGeneratorDlg::InitSearchDlg()
{
	CString strConnectionInfo;
	strConnectionInfo = GetConnectionInfo();
	if(!m_pSearchMonoDlg)
	{
		m_pSearchMonoDlg = new CSearchMono(this);
	}
	if(!m_pSearchMonoDlg->m_Create)
	{
		m_pSearchMonoDlg->SetConnectInfo( strConnectionInfo );
		m_pSearchMonoDlg->Create(this);	
	}

	if(!m_pSearchSerialDlg)
	{
		m_pSearchSerialDlg = new CSearchSerial(this);
	}
	if(!m_pSearchSerialDlg->m_Create)
	{
		m_pSearchSerialDlg->SetConnectInfo( strConnectionInfo );
		m_pSearchSerialDlg->Create(this);		
	}

	CTabCtrl *ptabMonoSerial = (CTabCtrl*)GetDlgItem(IDC_tabMonoSerial);

//	m_pSearchMonoDlg->MoveWindow( 10, 25,  m_MainRect.right - m_MainRect.left - 30, m_MainRect.bottom - m_MainRect.top - 70 );
	m_pSearchMonoDlg->MoveWindow( 0, 0,  m_MainRect.right - m_MainRect.left - 5, m_MainRect.bottom - m_MainRect.top - 55 );
	m_pSearchSerialDlg->MoveWindow( 10, 25,  m_MainRect.right - m_MainRect.left - 30, 135 );

//	CButton *ppicEcoConfidential = (CButton*)GetDlgItem(IDC_BmpEcoConfidential);
////	if( ppicEcoConfidential->GetSafeHwnd() == NULL) return;
//	CButton->MoveWindow( 0, m_MainRect - 50, cx, 50);

////	if( ptabMonoSerial->GetSafeHwnd() == NULL) return;
//	ptabMonoSerial->MoveWindow( 5, 5,  m_MainRect.right - m_MainRect.left - 15, m_MainRect.bottom - m_MainRect.top - 40 );
////	ptabMonoSerial->MoveWindow( 5, m_MainRect.top,  m_MainRect.right - 60, m_MainRect.bottom - 5 );
////	ptabMonoSerial->MoveWindow( 5, 0,  m_MainRect.right - m_MainRect.left - 15, m_MainRect.bottom - m_MainRect.top - 40 );
//
////	m_pSearchMonoDlg->MoveWindow( 10, 20,  m_MainRect.right - m_MainRect.left - 30, 135 );
//	
//	m_pSearchSerialDlg->MoveWindow( 10, 25,  m_MainRect.right - m_MainRect.left - 30, 135 );

	ShowSearchDlg();

}

VOID CIndexGeneratorDlg::ShowSearchDlg()
{
//	if(!m_pSearchMonoDlg)
//	{
//		m_pSearchMonoDlg = new CIndexReproduceSearch(this);
//	}
//	if(!m_pSearchSerialDlg->m_Create)
//	{
//		m_pSearchSerialDlg->Create(this);
//	}

	CTabCtrl *ptabMonoSerial = (CTabCtrl*)GetDlgItem(IDC_tabMonoSerial);
	INT nCurSel = ptabMonoSerial->GetCurSel();
	if ( 0 == nCurSel )
	{
		m_pSearchMonoDlg->ShowWindow(SW_SHOW);
		m_pSearchSerialDlg->ShowWindow(SW_HIDE);
//		m_pSearchSerialDlg->UpdateWindow();
//		m_pSearchSerialDlg->CenterWindow();
	}
	else
	{
		m_pSearchMonoDlg->ShowWindow(SW_HIDE);
		m_pSearchSerialDlg->ShowWindow(SW_SHOW);
	}
}

void CIndexGeneratorDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	GetWindowRect( m_MainRect );
//	CRect ClientRect;
	CTabCtrl *ptabMonoSerial = (CTabCtrl*)GetDlgItem(IDC_tabMonoSerial);
	if( ptabMonoSerial->GetSafeHwnd() == NULL) return;
	ptabMonoSerial->MoveWindow( 5, 5,  m_MainRect.right - m_MainRect.left - 15, m_MainRect.bottom - m_MainRect.top - 40 );

//	m_pSearchMonoDlg->MoveWindow( 10, 25,  m_MainRect.right - m_MainRect.left - 30, m_MainRect.bottom - m_MainRect.top - 70 );
//	m_pSearchMonoDlg->MoveWindow( 0, 0,  m_MainRect.right - m_MainRect.left - 5, m_MainRect.bottom - m_MainRect.top - 30 );
	m_pSearchMonoDlg->MoveWindow( 0, 0,  m_MainRect.right - m_MainRect.left - 5, m_MainRect.bottom - m_MainRect.top - 55 );

	CButton *ppicEcoConfidential = (CButton*)GetDlgItem(IDC_BmpEcoConfidential);
	if( ppicEcoConfidential->GetSafeHwnd() == NULL) return;
	CRect ClientRect;
	ppicEcoConfidential->GetClientRect(ClientRect);
	ppicEcoConfidential->MoveWindow( 0, cy - 21, ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top );
	



//	m_pSearchMonoDlg->MoveWindow( 10, 20,  m_MainRect.right - m_MainRect.left - 30, 135 );
	//m_pSearchSerialDlg->MoveWindow( 10, 20,  m_MainRect.right - m_MainRect.left - 30, 135 );

}


//void CIndexGeneratorDlg::ReSize(UINT nType, int cx, int cy) 
//{
//	CDialog::OnSize(nType, cx, cy);
//	
//	// TODO: Add your message handler code here
//	CTabCtrl *ptabMonoSerial = (CTabCtrl*)GetDlgItem(IDC_tabMonoSerial);
//	if( ptabMonoSerial->GetSafeHwnd() == NULL) return;
//
//	if( m_pSearchMonoDlg->GetSafeHwnd() == NULL) return;
//	m_pSearchMonoDlg->MoveWindow(50, 50, cx - 10, 50);
//
//	if( m_pSearchSerialDlg->GetSafeHwnd() == NULL) return;
//	m_pSearchSerialDlg->MoveWindow(50, 50, cx - 10, 50);
//}


VOID CIndexGeneratorDlg::EnableThemeDialogTexture(HWND hwndDlg)
{
	HINSTANCE hDll=LoadLibrary(_T("UxTheme.dll"));							// 'UxTheme.dll'
	if(hDll==NULL)
		return;	// the DLL won't be available on anything except Windows XP
	ULONG (PASCAL *lpfnEnableTheme)(HWND, DWORD);
	(FARPROC&)lpfnEnableTheme=GetProcAddress(hDll,"EnableThemeDialogTexture");	// 'EnableThemeDialogTexture'
	if(lpfnEnableTheme)
		lpfnEnableTheme(hwndDlg, 6);		// ETDT_ENABLETAB = (ETDT_ENABLE | ETDT_USETABTEXTURE) = (2|4) = 6
	else ASSERT(FALSE);
	FreeLibrary(hDll);
}

HBRUSH CIndexGeneratorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG) 
	{	
		pDC->SetBkMode(OPAQUE);		
		pDC->SetBkColor(RGB(244,243,238));	
		return m_Dlg_brush;		
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


CString CIndexGeneratorDlg::GetConnectionInfo(VOID)
{
	CString strconnectionInfo;

	CStdioFile	file;
	CString strLineData;
	CString strFilePath;
	strFilePath.Format( _T("..\\CFG\\IndexGeneratorEnv\\DbConnectInfo.txt") );
	if (!file.Open(strFilePath, CFile::modeRead | CFile::typeBinary))
	{
		CString strErrorFileName;
		strErrorFileName.Format(_T("%s 파일을 여는데 실패했습니다."),strFilePath);
		AfxMessageBox(strErrorFileName);
		return _T("");
	}

	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	while( file.ReadString(strLineData) )
	{
		strLineData.Replace(_T("\r\n"), _T(""));

		strLineData.Replace(_T("\r"), _T(""));
		strLineData.Replace(_T("\n"), _T(""));

		strLineData.TrimLeft();	strLineData.TrimRight();
		if( !strLineData.IsEmpty() )
		{
			break;
		}
	}
	file.Close();
	if( !strLineData.IsEmpty() )
	{
		strconnectionInfo = strLineData;
	}
	else
	{
		strconnectionInfo.Format(_T("kolas2@k3/kolas2#[]"));
	}
	//===================================================
	
	return strconnectionInfo;
}

VOID CIndexGeneratorDlg::CloseDialog()
{
//	m_pSearchMonoDlg->CloseWindow();
//	m_pSearchSerialDlg->CloseWindow();
	m_pSearchMonoDlg->DestroyWindow();
	m_pSearchSerialDlg->DestroyWindow();
	CDialog::OnCancel();
//	CDialog::OnClose();
}

BOOL CIndexGeneratorDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if( VK_ESCAPE == pMsg->wParam   )
		{
			CloseDialog();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


//HBRUSH CIndexGeneratorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
//{	
//	int getdlgid = pWnd->GetDlgCtrlID();
//	
//
//
//	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG) 
//	{	
//		pDC->SetBkMode(OPAQUE);		
//		pDC->SetBkColor(RGB(244,243,238));	
//		return m_Dlg_brush;		
//	}
//
//	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
//}