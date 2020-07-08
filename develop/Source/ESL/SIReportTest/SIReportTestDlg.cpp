// SIReportTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIReportTest.h"
#include "SIReportTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\\SIReportMgr\\SIReportManager.h"

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
// CSIReportTestDlg dialog

CSIReportTestDlg::CSIReportTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSIReportTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSIReportTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

VOID CSIReportTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSIReportTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSIReportTestDlg, CDialog)
	//{{AFX_MSG_MAP(CSIReportTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bPRINT, OnbPRINT)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bPRINT2, OnbPRINT2)
	ON_BN_CLICKED(IDC_bTEST, OnbTEST)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestDlg message handlers

BOOL CSIReportTestDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
	m_pDM = new CESL_DataMgr;
	m_pDM->SetCONNECTION_INFO(_T("kolas2@kolas2/kolas2#[]"));
	m_pDM->DB_MGR_ALIAS = _T("DM_테스트");
	m_pDM->TBL_NAME = _T("ESL_TEST");

	m_pDM->InitDataMgrRef(7);
	m_pDM->SetDataMgrRef(0, _T("아이디"),	_T("ID"),			_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(1, _T("암호"),		_T("PASSWORD"),		_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(2, _T("이름"),		_T("NAME"),			_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(3, _T("주소"),		_T("ADDRESS"),		_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(4, _T("전화번호"), _T("TEL"),			_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(5, _T("생일"),		_T("BIRTHDAY"),		_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(6, _T("직급"),		_T("GRADE"),		_T("str"), _T(""), _T(""));

	//OnbSEARCH();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CSIReportTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CSIReportTestDlg::OnPaint() 
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
HCURSOR CSIReportTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CSIReportTestDlg::OnbPRINT() 
{
	CSIReportManager *pSIReportManager = new CSIReportManager;
	pSIReportManager->m_pOwnerWnd = this;
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return;
	}
	pSIReportManager->m_strNoMoreStatement = _T("No more statement");
	if (m_pDM->GetRowCount() == 0) {
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);
		m_pDM->InsertRow(-1);

		m_pDM->SetCellData(0, 0, _T("wjchoi"));
		m_pDM->SetCellData(0, 1, _T("1234"));
		m_pDM->SetCellData(0, 2, _T("최원종"));
		m_pDM->SetCellData(0, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(1, 0, _T("dkjang"));
		m_pDM->SetCellData(1, 1, _T("1234"));
		m_pDM->SetCellData(1, 2, _T("장동건"));
		m_pDM->SetCellData(1, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(2, 0, _T("jwkim"));
		m_pDM->SetCellData(2, 1, _T("1234"));
		m_pDM->SetCellData(2, 2, _T("김재원"));
		m_pDM->SetCellData(2, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(3, 0, _T("mjgu"));
		m_pDM->SetCellData(3, 1, _T("1234"));
		m_pDM->SetCellData(3, 2, _T("구마적"));
		m_pDM->SetCellData(3, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(4, 0, _T("mjshin"));
		m_pDM->SetCellData(4, 1, _T("1234"));
		m_pDM->SetCellData(4, 2, _T("신마적"));
		m_pDM->SetCellData(4, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(5, 0, _T("ysha"));
		m_pDM->SetCellData(5, 1, _T("1234"));
		m_pDM->SetCellData(5, 2, _T("하야시"));
		m_pDM->SetCellData(5, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(6, 0, _T("dhkim"));
		m_pDM->SetCellData(6, 1, _T("1234"));
		m_pDM->SetCellData(6, 2, _T("김두한"));
		m_pDM->SetCellData(6, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(7, 0, _T("mjpal"));
		m_pDM->SetCellData(7, 1, _T("1234"));
		m_pDM->SetCellData(7, 2, _T("팔마적"));
		m_pDM->SetCellData(7, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(8, 0, _T("mokim"));
		m_pDM->SetCellData(8, 1, _T("1234"));
		m_pDM->SetCellData(8, 2, _T("김무옥"));
		m_pDM->SetCellData(8, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(9, 0, _T("yckim"));
		m_pDM->SetCellData(9, 1, _T("1234"));
		m_pDM->SetCellData(9, 2, _T("김영철"));
		m_pDM->SetCellData(9, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(10, 0, _T("ycmun"));
		m_pDM->SetCellData(10, 1, _T("1234"));
		m_pDM->SetCellData(10, 2, _T("문영철"));
		m_pDM->SetCellData(10, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(11, 0, _T("jyjung"));
		m_pDM->SetCellData(11, 1, _T("1234"));
		m_pDM->SetCellData(11, 2, _T("정진영"));
		m_pDM->SetCellData(11, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(12, 0, _T("jjkim"));
		m_pDM->SetCellData(12, 1, _T("1234"));
		m_pDM->SetCellData(12, 2, _T("김좌진"));
		m_pDM->SetCellData(12, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(13, 0, _T("sulhyang"));
		m_pDM->SetCellData(13, 1, _T("1234"));
		m_pDM->SetCellData(13, 2, _T("설향"));
		m_pDM->SetCellData(13, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(14, 0, _T("irah"));
		m_pDM->SetCellData(14, 1, _T("1234"));
		m_pDM->SetCellData(14, 2, _T("아이란"));
		m_pDM->SetCellData(14, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(15, 0, _T("erchoi"));
		m_pDM->SetCellData(15, 1, _T("1234"));
		m_pDM->SetCellData(15, 2, _T("최동렬"));
		m_pDM->SetCellData(15, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(16, 0, _T("sibaru"));
		m_pDM->SetCellData(16, 1, _T("1234"));
		m_pDM->SetCellData(16, 2, _T("시바루"));
		m_pDM->SetCellData(16, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

		m_pDM->SetCellData(17, 0, _T("miura"));
		m_pDM->SetCellData(17, 1, _T("1234"));
		m_pDM->SetCellData(17, 2, _T("미우라"));
		m_pDM->SetCellData(17, 3, _T("서울특별시 강남구 논현동 49번지 선양빌딩 3층 (주)ECO"));

	}
	INT ids;
	ids = pSIReportManager->LoadWorkForm();	
	pSIReportManager->m_pDM = m_pDM;
	pSIReportManager->m_DMCursor = 0;

	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
}

VOID CSIReportTestDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pDM) {
		delete m_pDM;
		m_pDM = NULL;
	}
}

VOID CSIReportTestDlg::OnbSEARCH() 
{
	INT ids = m_pDM->RefreshDataManager(_T(""));
	CString msg;
	if (ids < 0) {			// 검색 에러
		msg.Format(_T("검색중 에러 발생 [%d]"), ids);
	} else {
		msg.Format(_T("검색된 자료 총 %d건"), m_pDM->GetRowCount());
	}

	GetDlgItem(IDC_STATIC1)->SetWindowText(msg);
}

VOID CSIReportTestDlg::OnbPRINT2() 
{
	INT t = sizeof(REPORT_BLOCK);
	// Report Manager 사용하기
	CReportManager*	pReportManager = NULL;
	pReportManager = new CReportManager;

	INT ids = pReportManager->Create( AfxGetApp(), this, 1 , &m_pDM, ( INT( CWnd::* )( REPORT_EVENT_INFO *EventInfo  ) )EventHandler );
	if( ids) 
	{
		AfxMessageBox( _T("CReportManager Create() Error!!!") );
		return;
	}
	
	ids = pReportManager->Print( _T("..\\temp\\ReportEditor2.rpt") );

	if( pReportManager != NULL )
	{
		delete pReportManager;
		pReportManager = NULL;
		return;
	}
	// Report Manager 사용하기 끝	
}

INT CSIReportTestDlg::EventHandler( REPORT_EVENT_INFO * EventInfo )
{
	return 0;
}

INT CSIReportTestDlg::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	CString msg;
	msg.Format(_T("%d"), EventInfo->m_nPage);
	pSIReportManager->SetTableCellText(_T("페이지번호"), 0, 0, msg, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	return 0;
}

VOID CSIReportTestDlg::OnbTEST() 
{
	CWorkSheet *pWorkSheet = new CWorkSheet;

	CDC *pDC = this->GetDC();
	INT i, ids; 
	ids = pWorkSheet->Load(_T("..\\temp\\ReportEditor.rpt"));
	CString msg;
	for (i = 0; i < pWorkSheet->m_pSheet->object_count; i++) {
		if (OBJECT_GRID == pWorkSheet->m_pSheet->object[i]->kind) {
			//AfxMessageBox(_T("GRID"));
			pWorkSheet->DrawObject(pDC, i);
		}
	}
	
	if (pWorkSheet) {
		delete pWorkSheet;
		pWorkSheet = NULL;
	}
}
