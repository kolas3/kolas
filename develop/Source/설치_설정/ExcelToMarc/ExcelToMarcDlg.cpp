// ExcelToMarcDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "ExcelToMarc.h"
#include "ExcelToMarcDlg.h"

#include "MarcEditDlg.h"
#include "LoginDlg.h"

#include "efs.h"

#include "..\..\공통\BO_SPECIES_API\BO_SPECIES.h"


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
// CExcelToMarcDlg dialog

CExcelToMarcDlg::CExcelToMarcDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CExcelToMarcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExcelToMarcDlg)
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

//	m_pDM_Species = NULL;
	m_strExportFilePath = _T("");

	m_nOpenMode = 1000;
	m_pThreadImport = NULL;
	m_pThreadDisplay = NULL;

	m_bIsThreadRunning = FALSE;
	m_pCatApi = NULL;

	m_pDM_Index = NULL;
	m_pDM_Species = NULL;
	m_pDM_Book = NULL;
//	m_pDM_Main = NULL;

	m_pInfo = NULL;
	m_pCatApi = NULL;
	m_pIndexTool = NULL;

	m_pGrid_OK = NULL;
	m_pGrid_ERR = NULL;
	m_pGrid_SUCCESS = NULL;

	m_pDlgKeyWord = NULL;

	//===================================================
	//2010.06.01 ADD BY PJW : 
	m_pDM_Species_Output = NULL;
	m_pDM_Index_Output = NULL;
	m_pDM_Book_Output = NULL;
//	m_pDM_Volume_Output;
	//===================================================

//	m_lpszSMAlias = _T("SM_정리_마크반입");
//	m_lpszCMAlias = _T("CM_정리_마크반입");
//	m_lpszCMAlias_Code = _T("CM_정리_마크반입_구분코드");	
	m_lpszSMAlias = _T("SM_ExcelToMarc");
	m_lpszCMAlias = _T("CM_ExcelToMarc");
	m_lpszCMAlias_Code = _T("CM_ExcelToMarc_구분코드");	
	m_lpszDMAlias_Index = _T("DM_ExcelToMarc_색인정보");	
	m_lpszDMAlias_Species = _T("DM_ExcelToMarc_종정보");	
	m_lpszDMAlias_Book = _T("DM_ExcelToMarc_책정보");
	
	m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));
}

CExcelToMarcDlg::~CExcelToMarcDlg()
{
	if( NULL != m_pInfo )
	{
		delete m_pInfo;
		m_pInfo = NULL;
	}
	if( NULL != m_pCatApi )
	{
		delete m_pCatApi;
		m_pCatApi = NULL;
	}
	if( NULL != m_pIndexTool )
	{
		delete m_pIndexTool;
		m_pIndexTool = NULL;
	}

}

void CExcelToMarcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExcelToMarcDlg)
	DDX_Text(pDX, IDC_edtExcelFilePath, m_strFilePath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExcelToMarcDlg, CDialog)
	//{{AFX_MSG_MAP(CExcelToMarcDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_BN_CLICKED(IDC_btnExcelFileOpen, OnbtnExcelFileOpen)
	ON_BN_CLICKED(IDC_btnSave, OnbtnSave)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ChkAccessionRec, OnChkAccessionRec)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_eAccessionRecNo, OnKillfocuseAccessionRecNo)
	ON_BN_CLICKED(IDC_ChkGiveAutoRegno, OnChkGiveAutoRegno)
	ON_CBN_SELCHANGE(IDC_cmbMEDIA_CLASS, OnSelchangecmbMEDIACLASS)
	ON_CBN_SELCHANGE(IDC_cmbIMPORTTYPE, OnSelchangecmbIMPORTTYPE)
	ON_BN_CLICKED(IDC_bInit, OnbInit)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	ON_BN_CLICKED(IDC_bNORMALIZATION, OnbNORMALIZATION)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_IMPORT, OnSelchangeTabImport)
	ON_BN_CLICKED(IDC_btnCodeApply, OnbtnCodeApply)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RO_PROGRESS, OnSetProgressCtrl)
	ON_MESSAGE(WM_END_MARC_ADJUST, OnEndMarcAdjust)
	ON_MESSAGE(WM_GRID_DISPLAY, MsgDisplayGrid)
	ON_MESSAGE(WM_SORT_GRID, MsgSortGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExcelToMarcDlg message handlers

BOOL CExcelToMarcDlg::OnInitDialog()
{
	EFS_BEGIN
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
	m_pInfo = new CESL_Information();
	
	CLoginDlg dlg(this);
	dlg.SetConnectInfo(GetConnectionInfo() );
	if ( IDOK != dlg.DoModal() )
	{
		CDialog::OnCancel();
		return FALSE;
	}

	m_pInfo->InitMarcMgr();
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	m_pIndexTool = new CIndexTool;
	m_pInfo->pIndexTool = (CObject*)m_pIndexTool;

	// 종DM을 구성한다.
//	m_pDM_Species = new CESL_DataMgr;
//	if (m_pDM_Species == NULL)  
//	{
//		AfxMessageBox(_T("m_pDM_Species is null"));
//		return -1;
//	}
//
//	m_pDM_Species->SetCONNECTION_INFO(CONNECTION_INFO);
//	m_pDM_Species->SetMarcMgr(m_pInfo->pMarcMgr);
//	m_pDM_Species->InitDMPKFromFile(_T("DM_단행_정리DM_종정보"));
//
//	if( 0 > Init() )
//	{
//		return FALSE;
//	}

//	m_progMarquee.SetWindowText(_T("검색중입니다..."));
//	m_progMarquee.SetMarqueeOptions(15);
//	m_progMarquee.SetWindowText(_T("검색중입니다..."));
//	m_progMarquee.SetMarqueeOptions(15);
	if (InitESL_Mgr(m_lpszSMAlias) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	

	if (Init() < 0)
	{
		AfxMessageBox(_T("Initialization is Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	GetWindowRect(m_rcInitial);

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl)
	{
		pTabCtrl->InsertItem(0, _T("정상"));
		pTabCtrl->InsertItem(1, _T("오류"));
		pTabCtrl->InsertItem(2, _T("성공"));

		pTabCtrl->SetCurFocus(1);
		pTabCtrl->SetCurFocus(0);
	}

	//등록구분 비활성화
	GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(FALSE);

	// Status Bar
	m_wndStatusBarCtrl.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP , CRect(0, 0, 0, 0), this, WM_USER + 1000);
	m_wndStatusBarCtrl.SetMinHeight(22);
	
	if (!m_wndProgressBar.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH, CRect(0,0,100,100), &m_wndStatusBarCtrl, 17298))
		return -1;

	m_wndProgressBar.SetFont(GetFont());

	const INT nParts = 5;
	CRect rect;
	m_wndStatusBarCtrl.GetClientRect(&rect);
	INT widths[nParts] = { 100, 200, 300, 400, -1 };
	m_wndStatusBarCtrl.SetParts(nParts, widths);

	PostMessage(WM_SIZE);

//	( (CButton*) GetDlgItem (IDC_rTXT) )->SetCheck(TRUE);
	m_nFileType = 0;

	DefaultAccessionRecNo();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	EFS_END	
	return FALSE;  // return TRUE  unless you set the focus to a control
}

INT CExcelToMarcDlg::Init()
{
	EFS_BEGIN

	INT ids = -1;

	//===================================================
	//2010.06.01 ADD BY PJW :
	CString strDirectory;
	::GetCurrentDirectory(255, strDirectory.GetBuffer(0));
	SetCurDirectory(strDirectory);
	InitControl();
	//===================================================

	//=======================================================================
	// 1. API/ API 멤버 초기화
	//=======================================================================
	m_pCatApi = new CCatApi_BO(this);
	if( m_pCatApi == NULL )
	{
		AfxMessageBox(_T("CCatApi_BO 초기화 실패"));
		return -1;
	}

	ids = m_pCatApi->InitMemberData();
	if( ids < 0 )
	{
		AfxMessageBox(_T("InitMemberData() Fail"));
		return -1;
	}

	//=======================================================================
	// 2. DM 초기화
	//=======================================================================
	m_lpszSpeciesDMAlias = _T("DM_단행_정리DM_종정보");
	m_lpszIndexDMAlias = _T("DM_단행_정리DM_색인정보");
	m_lpszVolumeDMAlias = _T("DM_단행_정리DM_권정보");
	m_lpszBookDMAlias = _T("DM_단행_정리DM_책정보");

//	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	SetWindowText(_T("단행 마크 반입"));

	CString strHD_Species;
	strHD_Species = _T("BS");

	
	// 종DM을 구성한다.
	m_pDM_Species = new CESL_DataMgr;
	if (m_pDM_Species == NULL) 
	{
		AfxMessageBox(_T("m_pDM_Species is null"));
		return -1;
	}

	m_pDM_Species->InitDMPKFromFile(m_lpszSpeciesDMAlias);
	m_pDM_Species->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	m_pDM_Species->SetMarcMgr(m_pInfo->pMarcMgr);

	// 색인 DM을 구성한다.
	m_pDM_Index = new CESL_DataMgr;
	if (m_pDM_Index == NULL) 
	{
		AfxMessageBox(_T("m_pDM_Index is null"));
		return -1;
	}
	m_pDM_Index->InitDMPKFromFile(m_lpszIndexDMAlias);
	m_pDM_Index->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	m_pDM_Index->SetMarcMgr(m_pInfo->pMarcMgr);

	// 책 DM을 구성한다.
	m_pDM_Book = new CESL_DataMgr;
	if (m_pDM_Book == NULL) 
	{
		AfxMessageBox(_T("m_pDM_Book is null"));
		return -1;
	}
	
	m_pDM_Book->InitDMPKFromFile(m_lpszBookDMAlias);
	m_pDM_Book->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	m_pDM_Book->SetMarcMgr(m_pInfo->pMarcMgr);
	
	//===================================================
	//2009.03.10 ADD BY PJW : 반입받을 데이터 담을 DM
	// 정상자료 DM
	m_pDmGridOk = FindDM(_T("DM_ExcelToMarc_GRID_OK"));
	if( m_pDmGridOk == NULL )
	{
		AfxMessageBox(_T("m_pDmOk is null"));
		return -1;
	}

	// 에러자료 DM
	m_pDmGridError = FindDM(_T("DM_ExcelToMarc_GRID_ERROR"));
	if( m_pDmGridError == NULL )
	{
		AfxMessageBox(_T("m_pDmError is null"));
		return -1;
	}

	// 성공자료 DM
	m_pDmGridSuccess = FindDM(_T("DM_ExcelToMarc_GRID_SUCCESS"));
	if( m_pDmGridSuccess == NULL )
	{
		AfxMessageBox(_T("m_pDmSuccess is null"));
		return -1;
	}
	//===================================================

	// CM
	CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias);
	if (pCM == NULL)
	{
		AfxMessageBox(_T("pCM is null"));
		return -1;
	}

	m_pDM_Species_Output = FindDM(m_lpszDMAlias_Species);
	if( m_pDM_Species_Output == NULL )
	{
		AfxMessageBox(_T("m_pDM_Species_Output is null"));
		return -1;
	}

	// 에러자료 DM
	m_pDM_Index_Output = FindDM(m_lpszDMAlias_Index);
	if( m_pDM_Index_Output == NULL )
	{
		AfxMessageBox(_T("m_pDM_Index_Output is null"));
		return -1;
	}

	// 성공자료 DM
	m_pDM_Book_Output = FindDM(m_lpszDMAlias_Book);
	if( m_pDM_Book_Output == NULL )
	{
		AfxMessageBox(_T("m_pDM_Book_Output is null"));
		return -1;
	}

	m_pDM_Index->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	m_pDM_Species->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	m_pDM_Book->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	m_pDmGridOk->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	m_pDmGridError->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	m_pDmGridSuccess->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	m_pDM_Species_Output->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	m_pDM_Index_Output->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	m_pDM_Book_Output->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	// Grid
	m_pGrid_OK = (CESL_Grid*)pCM->FindControl(_T("Grid_OK"));
	// ERROR GRID
	m_pGrid_ERR = (CESL_Grid*)pCM->FindControl(_T("Grid_ERR"));
	// SUCCESS GRID
	m_pGrid_SUCCESS = (CESL_Grid*)pCM->FindControl(_T("Grid_SUCCESS"));

	if (m_pGrid_OK == NULL || m_pGrid_ERR == NULL || m_pGrid_SUCCESS == NULL)
	{
		AfxMessageBox(_T("Grids is null"));
		return -1;
	}

	// Move Windows
	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);
	
	m_pGrid_OK->MoveWindow(rcGrid);
	
	m_pGrid_ERR->MoveWindow(rcGrid);
	
	m_pGrid_SUCCESS->MoveWindow(rcGrid);

	
	//=======================================================================
	// 3. 그리드 Alias 세팅
	// 선정/시작등록번호/책수/본표제/저작자/발행자/총서표제/발행년/면장수/ISBN/오류
	//=======================================================================
	m_mapGridAlias.SetAt(_T(""), 1);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.049$l"), 2);
	m_mapGridAlias.SetAt( strHD_Species + _T("_파일반입책수"), 3);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.서명"), 4);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.저자"), 5);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.발행자"), 6);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.총서표제"), 7);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.발행년"), 8);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.면장수"), 9);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.020$a"), 10);
	m_mapGridAlias.SetAt( strHD_Species + _T("_파일반입오류"), 11);

	//=======================================================================
	// 4. 레코드 유형과 서지수준 으로 형식구분,매체구분(대표값)결정
	//=======================================================================
	// BK
	m_mapRecord_FormCodeMatCode.SetAt(_T("aa"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ac"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ad"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("am"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ta"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("tc"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("td"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("tm"), _T("BK=PR|OF"));

	// ER
	m_mapRecord_FormCodeMatCode.SetAt(_T("mi"), _T("ER=OB|OA|OD|OH"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("mm"), _T("ER=OB|OA|OD|OH"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ms"), _T("ER=OB|OA|OD|OH"));

	// MP
	m_mapRecord_FormCodeMatCode.SetAt(_T("ea"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("eb"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ec"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ed"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("em"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("es"), _T("MP=NA"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("fa"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("fc"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("fd"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("fm"), _T("MP=NA"));

	// MU
	m_mapRecord_FormCodeMatCode.SetAt(_T("ca"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cb"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cc"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cd"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cm"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cs"), _T("MU=NA"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("da"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("dc"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("dd"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("dm"), _T("MU=NA"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("ia"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ib"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ic"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("id"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("im"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("is"), _T("MU=SB|SD|SG"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("ja"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("jb"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("jc"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("jd"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("jm"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("js"), _T("MU=SB|SD|SG"));

	// CR
	m_mapRecord_FormCodeMatCode.SetAt(_T("ab"), _T("CR=PR"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("ai"), _T("CR=PR"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("as"), _T("CR=PR"));	

	// VM
	m_mapRecord_FormCodeMatCode.SetAt(_T("ga"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gb"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gc"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gd"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gi"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gm"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gs"), _T("VM=VD|VT|VU"));		

	m_mapRecord_FormCodeMatCode.SetAt(_T("ka"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("kb"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("kc"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("kd"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("ki"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("km"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("ks"), _T("VM=LP|LQ|LY|LZ"));	

	m_mapRecord_FormCodeMatCode.SetAt(_T("ra"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rb"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rc"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rd"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ri"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rm"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rs"), _T("VM=VD|VT|VU"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("ya"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("yb"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("yc"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("yd"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("yi"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ym"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ys"), _T("VM=VD|VT|VU"));
	
	// RB
	m_mapRecord_FormCodeMatCode.SetAt(_T("oa"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ob"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("oc"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("od"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("oi"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("om"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("os"), _T("RB=PR"));

	// 복합자료
	m_mapRecord_FormCodeMatCode.SetAt(_T("pa"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pb"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pc"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pd"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pi"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pm"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("ps"), _T("MP="));
	

	// 키워드 생성 다이얼로그
	CMarc marc;
	m_pDlgKeyWord = new CBO_CAT_PROC_KEYWORD_0000(this, marc);
	m_pDlgKeyWord->Create(this);
	m_pDlgKeyWord->ShowWindow(SW_HIDE);

	// 2005.07.01 ADD BY PDJ
	// USMARC반입시 선택항목 추가
//	( (CButton*) GetDlgItem (IDC_rKORMARC) )->SetCheck(TRUE);

	// 반출 Unicode 선택
//	( (CButton*) GetDlgItem (IDC_rUNICODE) )->SetCheck(TRUE);

	// 원부 설정
	OnChkAccessionRec();

	

	return 0;

	EFS_END
	return -1;
}

VOID CExcelToMarcDlg::InitControl()
{
	CString strDir = _T("");
	
	//===================================================
	//2010.05.02 ADD BY PJW : PC의 내문서로 기본 셋팅해준다.
//	CString strMyDoc = _T("");
//	SHGetSpecialFolderPath(NULL, strMyDoc.GetBuffer(0), CSIDL_PERSONAL, TRUE);
//	strDir.Format(_T("%s\\MarcCollector"),strMyDoc); 
//	strMyDoc.ReleaseBuffer(0);
	strDir = GetCurDirectory();
	//===================================================
//	CEdit* pEdtExcelFilePath = (CEdit*)GetDlgItem(IDC_edtExcelFilePath);	
//	pEdtExcelFilePath->SetWindowText(strDir);
}

void CExcelToMarcDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CExcelToMarcDlg::OnPaint() 
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
HCURSOR CExcelToMarcDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

HBRUSH CExcelToMarcDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

VOID CExcelToMarcDlg::EnableThemeDialogTexture(HWND hwndDlg)
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
void CExcelToMarcDlg::OnbtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

INT CExcelToMarcDlg::OpenFile() 
{
	// TODO: Add your control notification handler code here
	CString strFilePath;
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	ZeroMemory(&OFN, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	OFN.lpstrDefExt = _T("XLS");
	OFN.lpstrFilter = _T("XLS파일(*.xls)\0*.xls");
	OFN.lpstrTitle = _T("EXCEL 파일 열기");

	UpdateData(FALSE);

	if (GetOpenFileName(&OFN) != 0)
	{
		strFilePath= OFN.lpstrFile;
		CEdit* pedtExcelFilePath = (CEdit*)GetDlgItem(IDC_edtExcelFilePath);
		pedtExcelFilePath->SetWindowText(strFilePath);
	}
	else
	{
		strFilePath = GetCurDirectory();
		::SetCurrentDirectory(strFilePath);
		UpdateData(TRUE);
		return 1;
	}
	strFilePath = GetCurDirectory();
	::SetCurrentDirectory(strFilePath);
	return 0;
}

INT CExcelToMarcDlg::SaveFile() 
{
	// TODO: Add your control notification handler code here
	INT nIds =0;
	CString strFilePath;
	
	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;
	OFN.lpstrDefExt = _T("TXT");

	OFN.lpstrFilter = _T("TXT파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("TEXT 파일 저장");

	nIds = GetSaveFileName(&OFN);
	
	if (nIds != 0)
	{
		m_strExportFilePath= OFN.lpstrFile;
	}
	strFilePath = GetCurDirectory();
	SetCurrentDirectory(strFilePath);

	return 0;
}

void CExcelToMarcDlg::OnbtnExcelFileOpen() 
{
	// TODO: Add your control notification handler code here
	EFS_BEGIN

	UpdateData();

	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;

	if (m_nFileType == 0)
	{
		OFN.lpstrFilter = _T("XLS파일(*.xls)\0*.xls\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("EXCEL 파일 열기");
	}
	else if(m_nFileType == 1)
	{
		OFN.lpstrFilter = _T("TXT파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("TEXT 파일 열기");
	}

	BOOL bSuccess = GetOpenFileName(&OFN);
	SetCurrentDirectory(szBuffer);
	if (bSuccess)
	{
		m_strFilePath = OFN.lpstrFile;
		UpdateData(FALSE);

		m_wndProgressBar.SetWindowText(_T("파일 패치중..."));
		m_wndProgressBar.UpdateWindow();

		INT ids;
		// 파일 반입 시작
		ids = ImportMarcFromFile();
		if ( ids ) return;
	}
	else
	{
		return;
	}
	
	if ( _T("") != m_strFilePath )
	{
		//반입후 버튼(정리중반입, 파일열기, 등록번호 자동부여, 형식구분)을 Disable
		GetDlgItem(IDC_btnExcelFileOpen)->EnableWindow(FALSE);
		GetDlgItem(IDC_ChkWorkStatus)->EnableWindow(FALSE);
		GetDlgItem(IDC_ChkGiveAutoRegno)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbFORM_CLASS)->EnableWindow(FALSE);
		GetDlgItem(IDC_ChkMULTIBOOK)->EnableWindow(FALSE);

		GetDlgItem(IDC_bMODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnSave)->EnableWindow(TRUE);
//		GetDlgItem(IDC_bIMPORT)->EnableWindow(TRUE);
//		GetDlgItem(IDC_bEXPORT_EDITMARC)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_ChkAccessionRec)->EnableWindow(FALSE);
		GetDlgItem(IDC_eAccessionRecNo)->EnableWindow(FALSE);
	}	

	CheckAccessionRecNo();
	
EFS_END
}

void CExcelToMarcDlg::OnbtnSave() 
{
	// TODO: Add your control notification handler code here
//	SaveFile();
//m_pDM_Species_Output;
//m_pDM_Index_Output;
//m_pDM_Book_Output;
//m_pDM_Volume_Output;
	ExportDmToFile(IDX_FILE_NAME, m_pDM_Index_Output);
	ExportDmToFile(SPECIES_FILE_NAME, m_pDM_Species_Output);
	ExportDmToFile(BOOK_FILE_NAME, m_pDM_Book_Output);

	AfxMessageBox(_T("반출 완료"));
}

INT CExcelToMarcDlg::ImportMarcFromFile()
{
	EFS_BEGIN

	BeginWaitCursor();
	INT ids;

	//=======================================================================
	// 1. DM알리아스 헤더세팅
	//=======================================================================
	CString strHD_Species, strHD_Index, strHD_Book;
	strHD_Species = _T("BS");
	strHD_Book = _T("BB");
	strHD_Index = _T("IBS");

	if (m_strFilePath.IsEmpty()) return -1;

	//=======================================================================
	// 2. 마크 구분별 마크 그룹 세팅
	//=======================================================================
	CString sFormCode, sMarcGroup;
	GetControlData( _T("CM_ExcelToMarc_구분코드"), strHD_Index + _T("_형식구분_코드"), sFormCode );
	
	//마크구분
//	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	//=======================================================================
	// 3. IMPORT/EXPORT MGR 초기화
	//=======================================================================
	// MarcMgr, FileType, FilePath 설정
	// File Type : XLS -- EXCEL 파일, TXT -- TEXT 파일
	CButton * pChkRegNoAutoGive;
	pChkRegNoAutoGive = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);

	m_marcImpExpMgr.Init(this, m_pInfo->pMarcMgr, m_nFileType, m_strFilePath);	
	// 2007. 08.18 ADD BY PDJ
	// 등록번호 자동부여일 경우 049 제거하고 신택스 체크
	if ( pChkRegNoAutoGive->GetCheck() == 1 )
	{
		m_marcImpExpMgr.bRegNoAutoGrant = TRUE;
	}

	// Call Back Func 설정
	m_marcImpExpMgr.SetCallBackMarcCheck((INT (CWnd::*)(CMarc *pMarc, CString &str))CallBackCheckMarc);

	// DM 설정
	m_marcImpExpMgr.InitDMList();
	m_marcImpExpMgr.AddDMPtr(m_pDM_Species);
	m_marcImpExpMgr.AddDMPtr(m_pDM_Index);

	// 오류 체크 받을 DM Alias 설정
	m_marcImpExpMgr.SetErrChkAlias( strHD_Species + _T("_파일반입오류"));

	// 책수 받을 DM Alias 설정
	m_marcImpExpMgr.SetBookCountAlias( strHD_Species +_T("_파일반입책수"));

	// Stream Marc 받을 DM Alias 설정
	m_marcImpExpMgr.SetMarcStreamAlias( strHD_Species +_T("_MARC"));

	// Import 시작
	// 2005.07.01 ADD BY 
	// USMARC일 경우 파라메터 default 0;
	INT nCheck = 0;
//	nCheck = ( (CButton*) GetDlgItem (IDC_rUSMARC) )->GetCheck();		
	ids = m_marcImpExpMgr.ImportFile(_T(""),nCheck);	
	//=======================================================================
	// 4. Display Thread 시작 등록번호 자동부여이면 등록번호 제거
	//=======================================================================
	if (ids >= 0)
	{
		// 소정 수정
		// 등록번호 자동부여 -> 049Tag 삭제		
		if ( pChkRegNoAutoGive->GetCheck() == 1 )
		{
			ids = Delete049TagFromMarc();
			if ( ids < 0 ) return -1;
		}
		m_pThreadDisplay = AfxBeginThread(ThreadFunc_Display, this);
	}
	else
	{
		// 에러시 초기화
		OnbInit();
		return ids;
	}
	
	EndWaitCursor();

	return ids;

	EFS_END
	return -1;
}

//INT CExcelToMarcDlg::ImportMarcFromFile()
//{
//	EFS_BEGIN
//
//	BeginWaitCursor();
//	INT ids;
//
//	CString strFilePath;
//	CEdit* pedtExcelFilePath = (CEdit*)GetDlgItem(IDC_edtExcelFilePath);
//	pedtExcelFilePath->GetWindowText(strFilePath);
//
//	if (strFilePath.IsEmpty()) return -1;
//
//	CString sFormCode, sMarcGroup;
////	GetControlData( _T("CM_BO_FILE_IMPORT_구분"), _T("IBS_형식구분_코드"), sFormCode );
//	
//	// 통합마크로만 적용
////	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
//
//	// MarcMgr, FileType, FilePath 설정
//	// File Type : XLS -- EXCEL 파일, TXT -- TEXT 파일
//	m_marcImpExpMgr.Init(this, m_pInfo->pMarcMgr, 0, strFilePath);
//
//	// DM 설정
//	m_marcImpExpMgr.InitDMList();
//	m_marcImpExpMgr.AddDMPtr(m_pDM_Species);
////	m_marcImpExpMgr.AddDMPtr(m_pDM_Index);
//
//	// 책수를 받을 DM Alias 설정
//	m_marcImpExpMgr.SetBookCountAlias(_T("BS_책수"));
//
//	// 오류 체크 받을 DM Alias 설정
//	m_marcImpExpMgr.SetErrChkAlias(_T("BS_파일반입오류"));
//	
//	// Stream Marc 받을 DM Alias 설정
//	m_marcImpExpMgr.SetMarcStreamAlias(_T("BS_MARC"));
//	
//	// 2006.05.10 ADD BY WEEDBLACK 
//	// USMARC일 경우 파라메터 default 0;
////	INT nCheck;
////	nCheck = ( (CButton*) GetDlgItem (IDC_rUSMARC) )->GetCheck();		
//
//	ids = m_marcImpExpMgr.ImportFile(_T(""),0);
////	if (ids >= 0)
////	{
////		m_pThreadDisplay = AfxBeginThread(ThreadFunc_Display, this);
////	}
//
//	EndWaitCursor();
//
//	return ids;
//
//	EFS_END
//	return -1;
//}
//===================================================
//2010.06.01 ADD BY PJW : 
//INT CExcelToMarcDlg::ExportDmToFile( CString strFileName, CESL_DataMgr* pOutputDm )
//{
//EFS_BEGIN
//
//	BeginWaitCursor();
//	
//	CESL_DataMgr *pDM = pOutputDm;
//
//	if (pDM->GetRowCount() < 1)
//	{
//		AfxMessageBox(_T("반출할 자료가 없습니다."));
//			return 0;
//	}
//
//	// File
//	CStdioFile file;
//	CString strData;
//		
//	CString strFilePath;	
//
//	CString strFieldAlias;
//	CString strFieldName;
//	CString strFieldType;
//	CString strInitValue;
//	CString strDataType;
//
//	CStringArray astrFieldAlias;
//	CStringArray astrFieldName;
//	CStringArray astrFieldType;
//	CStringArray astrInitValue;
//	CStringArray astrDataType;
//
//	astrFieldAlias.RemoveAll();	astrFieldName.RemoveAll();
//	astrFieldType.RemoveAll();	astrInitValue.RemoveAll();
//	astrDataType.RemoveAll();
//
//	INT nRowCount = pDM->GetRowCount();
//	INT nColCount = pDM->GetRefColumnCount();
//
//	INT i = 0;
//	INT j = 0;
//
//	for( i = 0 ; i < nColCount ; i++ )
//	{
//		pDM->GetColumnRefInfo(i, strFieldAlias, strFieldName, strFieldType, strInitValue, strDataType);
//
//		strFieldAlias.MakeUpper();	strFieldName.MakeUpper();
//		strFieldType.MakeUpper();	strInitValue.MakeUpper();
//		strDataType.MakeUpper();
//
//		astrFieldAlias.Add(strFieldAlias);
//		astrFieldName.Add(strFieldName);
//		astrFieldType.Add(strFieldType);
//		astrInitValue.Add(strInitValue);
//		astrDataType.Add(strDataType);
//	}
//
//	//	m_strExportFilePath.Format(_T("..\\data\\IDX_BO_TBL.TXT"));
//	strFilePath = _T("..\\data\\") + strFileName + _T(".ctl");
//	if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
//	{
//		return -1;
//	}
//
//	if( 0 == file.GetLength() )
//	{			
//		TCHAR cUni = 0xFEFF;
//		file.Write( &cUni, sizeof(TCHAR));
//	}
//
////	strData.Format(_T("LOAD DATA\r\nINFILE %s.dat\r\nINSERT\r\nINTO TABLE %s\r\nFIELDS TERMINATED BY ','\r\n("), strFileName, strFileName);
//	strData.Format(_T("LOAD DATA\r\nINFILE %s.dat\r\nAPPEND\r\nINTO TABLE %s\r\nFIELDS TERMINATED BY ','\r\n("), strFileName, strFileName);
//	file.WriteString(strData);
//
//	CString strControlData = _T("");
//	for ( i = 0; i < nColCount; i++)
//	{
//		strData = _T("");
//		if ( _T("UDF") == astrDataType.GetAt(i) )
//		{
//			continue;
//		}
//		if( _T("INT") == astrFieldType.GetAt(i) )
//		{
//			strData.Format(_T("%s integer external"), astrFieldName.GetAt(i));
//		}
//		else
//		{
//			strData.Format(_T("%s char"), astrFieldName.GetAt(i));
//		}
//
//		if( strControlData.IsEmpty() )
//		{
//			strControlData = strData;
//		}
//		else
//		{
//			strControlData = strControlData + _T(",\r\n") +strData;
//		}
//	}
//
//	strControlData = strControlData + _T(")\r\n");
//
//	file.WriteString(strControlData);
//	
//	file.Close();
//
//
//	//	m_strExportFilePath.Format(_T("..\\data\\IDX_BO_TBL.TXT"));
//	strFilePath = _T("..\\data\\") + strFileName + _T(".dat");
//	if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
//	{
//		return -1;
//	}
//
//	if( 0 == file.GetLength() )
//	{			
//		TCHAR cUni = 0xFEFF;
//		file.Write( &cUni, sizeof(TCHAR));
//	}
//
//	CString strInsertData = _T("");
//	BOOL bFirstData = TRUE;
//	for ( i = 0; i < nRowCount; i++)
//	{
//		bFirstData = TRUE;
//		strInsertData = _T("");
//		for ( j = 0; j < nColCount; j++)
//		{
//			strData = _T("");
//			if ( _T("UDF") == astrDataType.GetAt(j) )
//			{
//				continue;
//			}
//
//			strData = pDM->GetCellData(i, j);
//
//			if( TRUE == bFirstData )
//			{
//				strInsertData = strData;
//				bFirstData = FALSE;
//			}
//			else
//			{
//				strInsertData = strInsertData + _T(",") + strData;
//			}
//		}
//		strInsertData = strInsertData + _T("\r\n");
//		file.WriteString(strInsertData);
//	}
//
//	file.Close();
//
//
//
//	//===================================================
//	//2010.06.01 ADD BY PJW : 
////				strQuery.Format (_T("UPDATE IDX_BO_TBL SET IDX_HOLD_PLACE = GET_BO_MAIN_SHELF_CODE(REC_KEY), MAIN_PLACE_INFO = GET_BO_MAIN_SHELF_CODE(REC_KEY), REG_CODE = GET_BO_BOOK_INFO(REC_KEY, 'REG_CODE'), PLACE_INFO = GET_BO_BOOK_INFO(REC_KEY, 'SHELF_LOC_CODE'), IDX_VOL_TITLE = GET_BO_BOOK_INFO(REC_KEY, 'VOL_TITLE_INDEX'), IDX_VOL = GET_BO_BOOK_INFO(REC_KEY, 'VOL_INDEX'), IDX_SE_SHELF_CODE = GET_BO_BOOK_INFO(REC_KEY, 'SEPARATE_SHELF_CODE'), MANAGE_CODE = GET_BO_BOOK_INFO(REC_KEY, 'MANAGE_CODE'), USE_LIMIT_CODE = GET_BO_BOOK_INFO(REC_KEY, 'USE_LIMIT_CODE') WHERE REC_KEY = %s;"), pDlg->m_pDM_Index_Output->GetCellData(_T("")) );
////			pDlg->m_astrUpdate_Output.Add();
//	//===================================================
//	if( IDX_FILE_NAME == strFileName )
//	{
//		strFilePath = _T("..\\data\\") + strFileName + _T("_UPDATE.txt");
//		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
//		{
//			return -1;
//		}
//
//		if( 0 == file.GetLength() )
//		{			
//			TCHAR cUni = 0xFEFF;
//			file.Write( &cUni, sizeof(TCHAR));
//		}
//
//		CString strUpdateData = _T("");
//		CString strRecKey = _T("");
//		BOOL bFirstData = TRUE;
//		for ( i = 0; i < nRowCount; i++)
//		{
//			m_pDM_Index_Output->GetCellData(_T("IBS_REC_KEY"), i, strRecKey);
//			strUpdateData.Format (_T("UPDATE IDX_BO_TBL SET IDX_HOLD_PLACE = GET_BO_MAIN_SHELF_CODE(REC_KEY), MAIN_PLACE_INFO = GET_BO_MAIN_SHELF_CODE(REC_KEY), REG_CODE = GET_BO_BOOK_INFO(REC_KEY, 'REG_CODE'), PLACE_INFO = GET_BO_BOOK_INFO(REC_KEY, 'SHELF_LOC_CODE'), IDX_VOL_TITLE = GET_BO_BOOK_INFO(REC_KEY, 'VOL_TITLE_INDEX'), IDX_VOL = GET_BO_BOOK_INFO(REC_KEY, 'VOL_INDEX'), IDX_SE_SHELF_CODE = GET_BO_BOOK_INFO(REC_KEY, 'SEPARATE_SHELF_CODE'), MANAGE_CODE = GET_BO_BOOK_INFO(REC_KEY, 'MANAGE_CODE'), USE_LIMIT_CODE = GET_BO_BOOK_INFO(REC_KEY, 'USE_LIMIT_CODE') WHERE REC_KEY = %s;"), strRecKey );
//
//			strUpdateData = strUpdateData + _T("\r\n");
//			file.WriteString(strUpdateData);
//		}
//
//		strUpdateData.Format(_T("COMMIT;"));
//		file.WriteString(strUpdateData);
//
//		file.Close();
//	}
//	EndWaitCursor();
//
//	AfxMessageBox(_T("반출 완료"));
//
//	return 0;
//
//	EFS_END
//	return -1;
//}
//
//CString CExcelToMarcDlg::MakeDataUnicodeToAnsi(CString strInData, CHAR*)
//{
//	nNeedByte = m_MarcConvertApi.GetLengthUnicodeToAnsi(strErrMarc);
//	m_MarcConvertApi.GetStrUniCodeToAnsi(strErrMarc);
//	CHAR cTemp;
//	for( INT i = 0; i < nNeedByte-1 ; i++ )
//	{
//		cTemp = *(m_MarcConvertApi.m_cpConvertData+i);
//		file.Write(&cTemp, sizeof(CHAR));
//	}
//	CString strTemp;
//	cTemp = '\r';
//	file.Write(&cTemp, sizeof(CHAR));
//	cTemp = '\n';
//	file.Write(&cTemp, sizeof(CHAR));
//}

INT CExcelToMarcDlg::ExportDmToFile( CString strFileName, CESL_DataMgr* pOutputDm )
{
EFS_BEGIN

	BeginWaitCursor();
	
	CESL_DataMgr *pDM = pOutputDm;

	if (pDM->GetRowCount() < 1)
	{
		AfxMessageBox(_T("반출할 자료가 없습니다."));
			return 0;
	}

	// File
	CStdioFile file;
	CString strData;
		
	CString strFilePath;	

	CString strFieldAlias;
	CString strFieldName;
	CString strFieldType;
	CString strInitValue;
	CString strDataType;

	CStringArray astrFieldAlias;
	CStringArray astrFieldName;
	CStringArray astrFieldType;
	CStringArray astrInitValue;
	CStringArray astrDataType;

	CHAR** pcOutData = NULL;
	INT			nConvertLen;

	astrFieldAlias.RemoveAll();	astrFieldName.RemoveAll();
	astrFieldType.RemoveAll();	astrInitValue.RemoveAll();
	astrDataType.RemoveAll();

	INT nRowCount = pDM->GetRowCount();
	INT nColCount = pDM->GetRefColumnCount();

	INT i = 0;
	INT j = 0;

	for( i = 0 ; i < nColCount ; i++ )
	{
		pDM->GetColumnRefInfo(i, strFieldAlias, strFieldName, strFieldType, strInitValue, strDataType);

		strFieldAlias.MakeUpper();	strFieldName.MakeUpper();
		strFieldType.MakeUpper();	strInitValue.MakeUpper();
		strDataType.MakeUpper();

		astrFieldAlias.Add(strFieldAlias);
		astrFieldName.Add(strFieldName);
		astrFieldType.Add(strFieldType);
		astrInitValue.Add(strInitValue);
		astrDataType.Add(strDataType);
	}

	strFilePath = _T("..\\data\\") + strFileName + _T(".ctl");
	if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		return -1;
	}

//	if( 0 == file.GetLength() )
//	{			
//		TCHAR cUni = 0xFEFF;
//		file.Write( &cUni, sizeof(TCHAR));
//	}

//	strData.Format(_T("LOAD DATA\r\nINFILE %s.dat\r\nAPPEND\r\nINTO TABLE %s\r\nFIELDS TERMINATED BY ','\r\n("), strFileName, strFileName);
	strData.Format(_T("LOAD DATA\r\nINFILE %s.dat\r\nAPPEND\r\nINTO TABLE %s\r\nFIELDS ENCLOSED BY '<start>' AND '<end>'\r\n("), strFileName, strFileName);
	//===================================================
	//2010.06.01 ADD BY PJW : 
//	file.WriteString(strData);
	pcOutData = (CHAR**) malloc (sizeof(CHAR*));
	m_cvt.UnicodeToMultiByte(strData.GetBuffer(0), pcOutData, &nConvertLen );
	strData.ReleaseBuffer();
	file.Write(*pcOutData, nConvertLen);
	free(pcOutData);
	pcOutData = NULL;
	//===================================================
	

	CString strControlData = _T("");
	for ( i = 0; i < nColCount; i++)
	{
		strData = _T("");
		if ( _T("UDF") == astrDataType.GetAt(i) )
		{
			continue;
		}
		if( _T("INT") == astrFieldType.GetAt(i) )
		{
			strData.Format(_T("%s integer external"), astrFieldName.GetAt(i));
		}
		else
		{
			if( _T("MARC") == astrFieldName.GetAt(i) )
			{
				strData.Format(_T("%s char(40000)"), astrFieldName.GetAt(i));
			}
			else
			{
				strData.Format(_T("%s char"), astrFieldName.GetAt(i));
			}
		}

		if( strControlData.IsEmpty() )
		{
			strControlData = strData;
		}
		else
		{
			strControlData = strControlData + _T(",\r\n") +strData;
		}
	}

	strControlData = strControlData + _T(")\r\n");

	//===================================================
	//2010.06.01 ADD BY PJW : 
//	file.WriteString(strControlData);
	pcOutData = (CHAR**) malloc (sizeof(CHAR*));
	m_cvt.UnicodeToMultiByte(strControlData.GetBuffer(0), pcOutData, &nConvertLen );
	strControlData.ReleaseBuffer();
	file.Write(*pcOutData, nConvertLen);
	free(pcOutData);
	pcOutData = NULL;
	//===================================================
	
	
	file.Close();


	//	m_strExportFilePath.Format(_T("..\\data\\IDX_BO_TBL.TXT"));
	strFilePath = _T("..\\data\\") + strFileName + _T(".dat");
	if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		return -1;
	}

//	if( 0 == file.GetLength() )
//	{			
//		TCHAR cUni = 0xFEFF;
//		file.Write( &cUni, sizeof(TCHAR));
//	}

	CString strInsertData = _T("");
	BOOL bFirstData = TRUE;
	for ( i = 0; i < nRowCount; i++)
	{
		bFirstData = TRUE;
		strInsertData = _T("");
		for ( j = 0; j < nColCount; j++)
		{
			strData = _T("");
			if ( _T("UDF") == astrDataType.GetAt(j) )
			{
				continue;
			}

			//===================================================
			//2010.06.01 ADD BY PJW : 
			if( _T("MARC") == astrFieldName.GetAt(j) )
			{
				strData.Format(_T("%s char(40000)"), astrFieldName.GetAt(j));
			}
			//===================================================

			strData = pDM->GetCellData(i, j);

			if( TRUE == bFirstData )
			{
				strInsertData = _T("<start>") + strData + _T("<end>");
				bFirstData = FALSE;
			}
			else
			{//'<start>' AND '<end>'
				strInsertData = strInsertData + _T("<start>") + strData + _T("<end>");
			}
		}
		strInsertData = strInsertData + _T("\r\n");
		//===================================================
		//2010.06.01 ADD BY PJW : 
//		file.WriteString(strInsertData);
		pcOutData = (CHAR**) malloc (sizeof(CHAR*));
		m_cvt.UnicodeToMultiByte(strInsertData.GetBuffer(0), pcOutData, &nConvertLen );
		strInsertData.ReleaseBuffer();
		file.Write(*pcOutData, nConvertLen);
		free(pcOutData);
		pcOutData = NULL;
		//===================================================
		
	}

	file.Close();

	if( IDX_FILE_NAME == strFileName )
	{
		strFilePath = _T("..\\data\\") + strFileName + _T("_UPDATE.txt");
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		{
			return -1;
		}

//		if( 0 == file.GetLength() )
//		{			
//			TCHAR cUni = 0xFEFF;
//			file.Write( &cUni, sizeof(TCHAR));
//		}

		CString strUpdateData = _T("");
		CString strRecKey = _T("");
		BOOL bFirstData = TRUE;
		for ( i = 0; i < nRowCount; i++)
		{
			m_pDM_Index_Output->GetCellData(_T("IBS_REC_KEY"), i, strRecKey);
			strUpdateData.Format (_T("UPDATE IDX_BO_TBL SET IDX_HOLD_PLACE = GET_BO_MAIN_SHELF_CODE(REC_KEY), MAIN_PLACE_INFO = GET_BO_MAIN_SHELF_CODE(REC_KEY), REG_CODE = GET_BO_BOOK_INFO(REC_KEY, 'REG_CODE'), PLACE_INFO = GET_BO_BOOK_INFO(REC_KEY, 'SHELF_LOC_CODE'), IDX_VOL_TITLE = GET_BO_BOOK_INFO(REC_KEY, 'VOL_TITLE_INDEX'), IDX_VOL = GET_BO_BOOK_INFO(REC_KEY, 'VOL_INDEX'), IDX_SE_SHELF_CODE = GET_BO_BOOK_INFO(REC_KEY, 'SEPARATE_SHELF_CODE'), MANAGE_CODE = GET_BO_BOOK_INFO(REC_KEY, 'MANAGE_CODE'), USE_LIMIT_CODE = GET_BO_BOOK_INFO(REC_KEY, 'USE_LIMIT_CODE') WHERE REC_KEY = %s;"), strRecKey );

			strUpdateData = strUpdateData + _T("\r\n");
			//===================================================
			//2010.06.01 ADD BY PJW : 
//			file.WriteString(strUpdateData);
			pcOutData = (CHAR**) malloc (sizeof(CHAR*));
			m_cvt.UnicodeToMultiByte(strUpdateData.GetBuffer(0), pcOutData, &nConvertLen );
			strUpdateData.ReleaseBuffer();
			file.Write(*pcOutData, nConvertLen);
			free(pcOutData);
			pcOutData = NULL;
			//===================================================
			
		}

		strUpdateData.Format(_T("COMMIT;"));
		//===================================================
		//2010.06.01 ADD BY PJW : 
//		file.WriteString(strUpdateData);
		pcOutData = (CHAR**) malloc (sizeof(CHAR*));
		m_cvt.UnicodeToMultiByte(strUpdateData.GetBuffer(0), pcOutData, &nConvertLen );
		strUpdateData.ReleaseBuffer();
		file.Write(*pcOutData, nConvertLen);
		free(pcOutData);
		pcOutData = NULL;
		//===================================================
		

		file.Close();
	}
	EndWaitCursor();

	return 0;

	EFS_END
	return -1;
}
//===================================================

VOID CExcelToMarcDlg::StartProgressCtrl()
{
//	m_progMarquee.ShowWindow(TRUE);
//	m_progMarquee.SetMarquee(TRUE, 10);
}

VOID CExcelToMarcDlg::EndProgressCtrl()
{
//	m_progMarquee.ShowWindow(FALSE);	
//	m_progMarquee.SetMarquee(FALSE, 0);
//	m_progMarquee.SetPos(0);
}
LRESULT CExcelToMarcDlg::OnSetProgressCtrl(WPARAM wParam, LPARAM lParam)
{
	if(0 == lParam)
	{
		StartProgressCtrl();
	}
	else
	{
		EndProgressCtrl();
	}

	return 0;
}

void CExcelToMarcDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
//	if(NULL == m_progMarquee.GetSafeHwnd()) return;
//	CRect rDlg, rProgMarquee;
//	GetWindowRect(rDlg);
//	ScreenToClient(rDlg);
//	m_progMarquee.GetWindowRect(rProgMarquee);
//	m_progMarquee.MoveWindow(rDlg.left-20, rDlg.bottom, rDlg.left, rProgMarquee.Height());	
}


void CExcelToMarcDlg::DefaultAccessionRecNo()
{
	CString strQuery;
	CString strAcqYear;
	CString strAccessionRecNo;
	INT		nAccNum;

	GetControlData( _T("CM_ExcelToMarc_구분코드"), _T("BS_수입년도"), strAcqYear );

	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'ACC_NO' AND MAKE_YEAR = '%s' AND MANAGE_CODE = '%s'"), strAcqYear, m_pInfo->MANAGE_CODE );

	CString sResult;
	m_pDM_Index_Output->GetOneValueQuery( strQuery , sResult );

	nAccNum = _ttoi(sResult.GetBuffer(0));
	m_nAccNum = ++nAccNum;	
	strAccessionRecNo.Format( _T("%d") , nAccNum );
	SetControlData( _T("CM_ExcelToMarc_구분코드") , _T("BB_원부번호") , strAccessionRecNo);

	( (CEdit*) GetDlgItem (IDC_eAccessionRecNo) )->EnableWindow(FALSE);


}

CString CExcelToMarcDlg::GetConnectionInfo()
{
	CString strconnectionInfo;

	CStdioFile	file;
	CString strLineData;
	CString strFilePath;
	strFilePath.Format( _T("..\\CFG\\ExcelToMarcEnv\\DbConnectInfo.txt") );
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


void CExcelToMarcDlg::OnChkAccessionRec() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck;
	
	bCheck = ( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->GetCheck();
	if ( bCheck )
	{
		if ( CheckAccessionRecNo() == FALSE ) 
		{
			( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->SetCheck(FALSE);
			return;
		}
		( (CEdit*)	GetDlgItem	(IDC_eAccessionRecNo	) )->EnableWindow(TRUE);
		( (CButton*) GetDlgItem	(IDC_btnAccessionRecNo	) )->EnableWindow(TRUE);		
		
	}
	else
	{
		( (CEdit*) GetDlgItem	(IDC_eAccessionRecNo	) )->EnableWindow(FALSE);
		( (CButton*) GetDlgItem	(IDC_btnAccessionRecNo	) )->EnableWindow(FALSE);
	}
	
}


BOOL CExcelToMarcDlg::CheckAccessionRecNo()
{
	CString strAccessionRecNo;
	INT	nAccNum;
	BOOL bCheck;
	
	bCheck = ( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->GetCheck();
	if ( bCheck == FALSE ) return FALSE;

	GetControlData( _T("CM_ExcelToMarc_구분코드") , _T("BB_원부번호") , strAccessionRecNo);
	nAccNum = _ttoi(strAccessionRecNo.GetBuffer(0));
	if ( nAccNum > m_nAccNum )
	{		
		AfxMessageBox(_T("마지막 원부번호 +1 보다 큰 원부번호는 사용할 수 없습니다."));		
		return FALSE;
	} 
	else if ( nAccNum <= 0 ) 
	{	
		AfxMessageBox(_T("사용할 수 없는 원부번호 입니다."));		
		return FALSE;
	}	

	return TRUE;
}


VOID CExcelToMarcDlg::OnDestroy() 
{
	EFS_BEGIN

	if( NULL != m_pDM_Index )
	{
		delete m_pDM_Index;
		m_pDM_Index = NULL;
	}

	if( NULL != m_pDM_Species )
	{
		delete m_pDM_Species;
		m_pDM_Species = NULL;
	}

	if( NULL != m_pDM_Book )
	{
		delete m_pDM_Book;
		m_pDM_Book = NULL;
	}

	if( NULL != m_pDlgKeyWord )
	{
		delete m_pDlgKeyWord;
		m_pDlgKeyWord = NULL;
	}

	CDialog::OnDestroy();	


	EFS_END
}

void CExcelToMarcDlg::OnKillfocuseAccessionRecNo() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck;
	
	bCheck = ( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->GetCheck();
	if ( bCheck == FALSE ) return;

	if ( CheckAccessionRecNo() == FALSE )
	{
		( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->SetCheck(FALSE);		
	}
	
}

void CExcelToMarcDlg::OnChkGiveAutoRegno() 
{
	// TODO: Add your control notification handler code here
	CButton * pChkGiveAutoRegno;
	pChkGiveAutoRegno = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
	if ( pChkGiveAutoRegno->GetCheck() == 1 ) {
		GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(TRUE);
		GetDlgItem(IDC_ChkMULTIBOOK)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_ChkMULTIBOOK)->EnableWindow(FALSE);
	}	
}

void CExcelToMarcDlg::OnSelchangecmbMEDIACLASS() 
{
	// TODO: Add your control notification handler code here
	//매체구분 변경했을경우

	CString strHD_Index;
	strHD_Index = _T("IBS");
	

	CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias_Code);
	
	CString sMatCode;
	pCM->GetControlMgrData( strHD_Index + _T("_매체구분_코드") , sMatCode );

	CButton * pChkWorkStatus;	
	pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkWorkStatus);

	if ( sMatCode == _T("OF") )
	{
		if ( pChkWorkStatus->GetCheck() == 1 )
		{
			AfxMessageBox(_T("매체구분이 OnlineBook(OF)일 경우에는 정리중 반입을 할 수 없습니다.\n소장으로 반입됩니다."));

			CButton * pChkWorkStatus;	
			pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkWorkStatus);
			pChkWorkStatus->SetCheck(0);
		}
		GetDlgItem(IDC_ChkWorkStatus)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_ChkWorkStatus)->EnableWindow(TRUE);
	}
}

void CExcelToMarcDlg::OnSelchangecmbIMPORTTYPE() 
{
	// TODO: Add your control notification handler code here
	SetCtrlByAcqCode();	
}


VOID CExcelToMarcDlg::SetCtrlByAcqCode()
{
	
}

INT CExcelToMarcDlg::Delete049TagFromMarc()
{
EFS_BEGIN

	CString strHD_Species = _T("BS");
	CMarc marc;
	CString strStreamMarc;
	CString strEditMarc;
	
	CArray<CString, CString&> pArr049l;
	CString strItemData, strBookCnt;
	
	INT nBookCnt;
	INT ids = -1;

	INT nCount = m_pDM_Species->GetRowCount();
	for (INT i=0; i < nCount; i++ )
	{
		strStreamMarc = m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), i);
		if (strStreamMarc.IsEmpty()) return -1;

		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids < 0) return -1;
		
		
		m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);
		nBookCnt = pArr049l.GetSize();		
		pArr049l.RemoveAll();				
		
		strBookCnt.Format(_T("%d"),nBookCnt);
		m_pDM_Species->SetCellData(_T("UDF_책수"),strBookCnt,i);		

		/*
		// 2007.08.18 ADD BY PDJ
		// 반입시 049를 날리는데 쓰지도 않는 별치를 신택스 체크했음.
		// UDF_책수 세팅후에 날려야 함
		m_pInfo->pMarcMgr->DeleteItem(&marc, _T("049$f") );
		*/

		ids = m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
		if (ids < 0) return -1;

		ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc );
		if (ids < 0) return -1;

		ids = m_pDM_Species->SetCellData( strHD_Species + _T("_MARC"), strStreamMarc, i);
		if (ids < 0) return -1;
		
	}

	return 0;
EFS_END
	return -1;
}

void CExcelToMarcDlg::OnbInit() 
{
	// TODO: Add your control notification handler code here
	//반입후 버튼(정리중반입, 파일열기, 등록번호 자동부여, 형식구분)을 Disable
	GetDlgItem(IDC_btnExcelFileOpen)->EnableWindow(TRUE);
	GetDlgItem(IDC_ChkWorkStatus)->EnableWindow(TRUE);
	GetDlgItem(IDC_ChkGiveAutoRegno)->EnableWindow(TRUE);
	GetDlgItem(IDC_cmbFORM_CLASS)->EnableWindow(TRUE);
	GetDlgItem(IDC_ChkMULTIBOOK)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_ChkGiveAutoRegno))->SetCheck(FALSE);
	/***********************************************************
	 ***********************************************************/
	//20080515 ADD BY PJW : 정리중반입, 파일열기, 등록번호 자동부여, 형태구분 Default값 셋팅되도록 수정
	((CButton*)GetDlgItem(IDC_ChkWorkStatus))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_ChkMULTIBOOK))->SetCheck(FALSE);
	GetDlgItem(IDC_cmbFORM_CLASS)->EnableWindow(TRUE);
	CString strFormCode;
//	m_pInfo->pCodeMgr->GetDefaultCode(_T("형식구분"), strFormCode);
	m_pInfo->pCodeMgr->GetDefaultCode(_T("형식구분"), strFormCode);
	CESL_ControlMultiComboBox * pCMB = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cmbFORM_CLASS);
	pCMB->SelectString(strFormCode);
	/***********************************************************
	 ***********************************************************/
	GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_bMODIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_btnSave)->EnableWindow(FALSE);
//	GetDlgItem(IDC_bIMPORT)->EnableWindow(FALSE);
//	GetDlgItem(IDC_bEXPORT_EDITMARC)->EnableWindow(FALSE);
	
	( (CButton*) GetDlgItem(IDC_ChkAccessionRec) )->SetCheck(FALSE);
	//GetDlgItem(IDC_ChkAccessionRec)->EnableWindow(FALSE);
	GetDlgItem(IDC_ChkAccessionRec)->EnableWindow(TRUE);
	GetDlgItem(IDC_eAccessionRecNo)->EnableWindow(TRUE);	

	//===================================================
	//2009.03.10 ADD BY PJW : 그리드DM을 초기화한다.
	m_pDmGridError->FreeData();
	m_pDmGridOk->FreeData();
	m_pDmGridSuccess->FreeData();
	//===================================================


	m_pDM_Species_Output->FreeData();
	m_pDM_Index_Output->FreeData();
	m_pDM_Book_Output->FreeData();

	AllControlDisplay(_T("CM_ExcelToMarc"));

	DefaultAccessionRecNo();

	m_wndStatusBarCtrl.SetText(_T(""), 1, 0);
	m_wndStatusBarCtrl.SetText(_T(""), 2, 0);
	m_wndStatusBarCtrl.SetText(_T(""), 3, 0);
	
}












UINT ThreadFunc_Import(LPVOID pParam)
{
	//=======================================================================
 	// 1.Initialization Variable
 	//=======================================================================
 	CESL_ControlMgr *pCM = NULL;
 	CESL_Grid *pGrid_OK = NULL;			// 정상인 그리드

	CESL_DataMgr *pDM_OK		= NULL; // 정상인 DM
	CESL_DataMgr *pDM_ERR		= NULL; // 에러난 DM
	CESL_DataMgr *pDM_SUCCESS	= NULL; // 성공한 DM
 	CESL_DataMgr *pDM_TO		= NULL;	// 옮길	  DM

 	// 개수증가시킬 ID
 	UINT nID_TO = -1;
 	// 개수증가시킬 String
 	CString strTO;
 	// DataMgr
 	CString strDMPK;

// 	INT nStopIdx;
 	
 	CExcelToMarcDlg *pDlg = (CExcelToMarcDlg*)pParam;
	// KOL.UDF.022
 	if (&pDlg == NULL)
 	{
 		pDlg->m_bIsThreadRunning = FALSE;
 		return -1;
 	}
 
 	pDlg->m_bIsThreadRunning = TRUE;
 	pDlg->ButtonStatus(0);
 
 	// 종DM
 	CESL_DataMgr DM_Species;
 	DM_Species.InitDMPKFromFile(pDlg->m_lpszSpeciesDMAlias);
	DM_Species.SetCONNECTION_INFO(pDlg->m_pInfo->CONNECTION_INFO);
 	DM_Species.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
 
 	// 색인DM
 	CESL_DataMgr DM_Index;
 	DM_Index.InitDMPKFromFile(pDlg->m_lpszIndexDMAlias);
	DM_Index.SetCONNECTION_INFO(pDlg->m_pInfo->CONNECTION_INFO);
 	DM_Index.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
 	
 	// 책DM
 	CESL_DataMgr DM_Book;
 	DM_Book.InitDMPKFromFile(pDlg->m_lpszBookDMAlias);
	DM_Book.SetCONNECTION_INFO(pDlg->m_pInfo->CONNECTION_INFO);
 	DM_Book.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
 
 	// 권DM
 	CESL_DataMgr DM_Volume;
 	DM_Volume.InitDMPKFromFile(pDlg->m_lpszVolumeDMAlias);
	DM_Volume.SetCONNECTION_INFO(pDlg->m_pInfo->CONNECTION_INFO);
 	DM_Volume.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
 	
 	INT ids = -1;
 	INT idxDM = -1;
	INT nStopCnt = 0;
//  	INT nCount = -1;
 
	INT nSelectRowCnt;
 	INT nColCnt = -1;
 
// 	INT nRowTo = 0;
// 	INT nRowsTo = 0;
 	
 	CString strData;
 	CString strErrorMsg;
	CString nTotalCnt;
	CString strTotalMsg;
	CString nFalseCnt;
 	CString nTrueCnt;
 
 	DWORD start_app, end_app;
 	CString strMsg;		
 	INT		nProcCount = 0;
	INT		nSelectCnt = 0;
 
 	// HSS_040524 헤더세팅
 	CString strHD_Book, strHD_Species, strHD_Index;
 	strHD_Species = _T("BS");
 	strHD_Book = _T("BB");
 	strHD_Index = _T("IBS");
 
 
 	// 그리드 설정
 	pGrid_OK = pDlg->m_pGrid_OK;

	pDM_OK		= pDlg->m_pDmGridOk;
	pDM_ERR		= pDlg->m_pDmGridError;
	pDM_SUCCESS	= pDlg->m_pDmGridSuccess;
 	
 	if ( pGrid_OK == NULL )
 		goto ERR;
 
 	//=====================================================
 	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
 	INT nRegCodeLength;
 	nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
 	//=====================================================
 
 	//=======================================================================
 	// 2. 반입 수행 시간 체크 (체크를 왜하는지 모르겠음.)
 	//=======================================================================
 	FILE *fpTime;
 	fpTime = _tfopen( _T("..\\tunning\\반입수행시간.txt"), _T("a+b") );	
 	
 	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
 	// KOLAS 2 UP UNICODE 수정작업
 	// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
 	fseek(fpTime, 0, SEEK_END);		
 	if( 0 == ftell(fpTime) )
 	{
 		/*UNCHANGE*/fputc(0xFF, fpTime);
 		/*UNCHANGE*/fputc(0xFE, fpTime);
 	}
 	// ------------------------------------------------------------------------------
 		
 	DWORD start_total, end_total;
 	start_total = GetTickCount();
 		
 	//=======================================================================
 	// 3. 선택된 개수만큼 진행바를 초기화한다.
 	//=======================================================================
	pGrid_OK->SelectMakeList();
 	nSelectRowCnt = pGrid_OK->SelectGetCount();
 	
 	pDlg->m_wndProgressBar.SetRange(0, nSelectRowCnt);
 	pDlg->m_wndProgressBar.SetPos(0);
 	pDlg->m_wndProgressBar.SetWindowText(_T(""));
 
 	nColCnt = pDM_OK->GetRefColumnCount();
 
 	// 쓰레드 시작
 	BOOL	bError;	

	INT nSelectIdx;
	INT nChkCnt;
	
	
	nSelectIdx = pGrid_OK->SelectGetHeadPosition();

	for( nChkCnt = 0 ; nChkCnt < nSelectRowCnt ; ++nChkCnt )
	{
		bError = FALSE;
 		start_app = GetTickCount();
		if ( !pDlg->m_bIsThreadRunning ) 
		{
			nStopCnt = nChkCnt;
			break;
		}

		strErrorMsg.Empty();

		strData = pDM_OK->GetCellData(_T("BS_파일반입오류"), nSelectIdx); 
		if( !strData.IsEmpty() )
		{
			nSelectIdx = pGrid_OK->SelectGetNext();
			pDlg->m_wndProgressBar.StepIt();
			continue;
		}

		strData = pDM_OK->GetCellData(_T("순위"), nSelectIdx); 
		idxDM = _ttoi(strData) - 1 ;

		//===================================================================
 		// 4. 선정된 순서대로 마크정리 DM으로 변경한다.
 		//===================================================================
 		ids = pDlg->MakeMarcAdjustDM(pDlg->m_pDM_Species, idxDM, pDlg->m_pDM_Index, idxDM, &DM_Species, &DM_Index, &DM_Book, &DM_Volume);
 		CMarc marc;
	
		//===================================================================
 		// 5. 자동부여가 체크되어 있을경우 자동부여 DM 생성 및 마크를 수정한다.
 		//===================================================================
 		CButton * pChkGiveAutoRegno;
 		pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
 		if ( pChkGiveAutoRegno->GetCheck() == 1 ) 
 		{	
 			INT ids = -1;
 			// 등록구분 가져오기
 			CString sRegCode;
 			pDlg->GetControlData( _T("CM_ExcelToMarc_구분코드"), strHD_Book + _T("_등록구분"), sRegCode );
 
 			// 마지막 번호 가져오기
 			CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = '") + pDlg->m_pInfo->MANAGE_CODE + _T("' AND DIVISION_VALUE = '") + sRegCode + _T("'");
 			CString sLastNum;	
 			DM_Species.GetOneValueQuery( sQuery , sLastNum );
 			INT iLastNum = _ttoi ( sLastNum );
 
 			//등록구분이 없으면 새로추가
 			if ( sLastNum == _T("") ) 
			{
 				iLastNum = 0;
 
 				ids = DM_Species.StartFrame();
 				if ( 0 > ids ) goto ERR;
 				CString sRecKey;
 				ids = DM_Species.MakeRecKey( sRecKey );
 				if ( 0 > ids ) goto ERR;
 				CString sQuery_reg;
 				CTime time = CTime::GetCurrentTime();
 				CString sYear;
 				sYear.Format( _T("%04d"), time.GetYear() );
 				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER, MANAGE_CODE) ")
 								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s', '%s' );") 
 								  , sRecKey , _T("CO") , _T("REG_NO") , sRegCode , sYear , _T("0"), pDlg->m_pInfo->MANAGE_CODE);
 				ids = DM_Species.AddFrame( sQuery_reg );
 				if ( 0 > ids ) goto ERR;
 
 				ids = DM_Species.SendFrame();			
 				if (ids < 0) {
 					AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
 					goto ERR;
 				}
 				ids = DM_Species.EndFrame();
 				if ( 0 > ids ) goto ERR;
 			}
 
 			CESL_DataMgr *pDmLastNum = pDlg->FindDM( _T("DM_마지막번호") );
 			if (pDmLastNum == NULL) 
			{
 				AfxMessageBox( _T("DM_마지막번호 확인") );
 				goto ERR;
 			}
			pDmLastNum->SetCONNECTION_INFO(pDlg->m_pInfo->CONNECTION_INFO);

 			pDmLastNum->FreeData();
 			pDmLastNum->InsertRow(-1);
 			pDmLastNum->SetCellData( strHD_Book + _T("_등록구분_코드"), sRegCode, 0 );
 			
 			//수정
 			CString sMarc = DM_Species.GetCellData( strHD_Species + _T("_MARC"), 0 );
 			pDlg->m_pInfo->pMarcMgr->Decoding(sMarc, &marc);
 			CArray<CString, CString&> pArr049l;
 			CString strItemData;
 			pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);
 
 
 			//선정된 자료의 책수만 하도록 수정
 			//홍수
 			INT iBookCount = DM_Book.GetRowCount();
 
 			for ( INT j = 0 ; j < iBookCount ; j++ ) 
			{
 				iLastNum++;
 				CString sRegNo;
 				sLastNum.Format( _T("%d"), iLastNum );
 				sRegNo = pDlg->m_pInfo->MakeRegNo( sRegCode, sLastNum );

				pDmLastNum->SetCellData( _T("마지막번호"), sLastNum, 0 );
 				
 				CString sItem;
 				if ( pArr049l.GetSize() == iBookCount ) 
				{
 					sItem = pArr049l.GetAt(j);
 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록번호"), sRegNo, j);
 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(nRegCodeLength), j); 					
 					
					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo, pArr049l.GetAt(j) );
 				}
 				else 
				{
 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록번호"), sRegNo, j);
 					ids = DM_Book.SetCellData(strHD_Book + _T("_등록구분_코드"), sRegNo.Left(nRegCodeLength), j);
 					
 					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
 				}
 				pDlg->m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
 				DM_Species.SetCellData( strHD_Species + _T("_MARC"), sMarc, 0 );
 			}
 		}
 
 		//================== 정리중반입, 소장반입 공통적용 ==================
 		
 		//===================================================================
 		// 6. 책DM에 수입구분/등록일/등록번호 적용
 		//===================================================================
 		CString sAcqCode;		
 		pDlg->GetControlData( _T("CM_ExcelToMarc_구분코드"), strHD_Index + _T("_수입구분_코드"), sAcqCode );
 		INT iBookCount = DM_Book.GetRowCount();
 
 		for ( INT i = 0 ; i < iBookCount ; i++ ) 
		{
 			//수입구분 적용
 			DM_Book.SetCellData( strHD_Book + _T("_수입구분_코드"), sAcqCode , i );
 			
 			//등록일 적용
 			CString result;
 			CTime t = CTime::GetCurrentTime();
 			result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
 			ids = DM_Book.SetCellData( strHD_Book + _T("_등록일"), result, i);
 
 			//등록번호 12자리 맨들기 
 			CString strRegNo;
 			ids = DM_Book.GetCellData( strHD_Book + _T("_등록번호"), i, strRegNo);
 			if ( strRegNo.GetLength() > nRegCodeLength && strRegNo.GetLength() < 12 ) 
			{
 				strRegNo = pDlg->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
 			}

 			ids = DM_Book.SetCellData( strHD_Book + _T("_등록번호"), strRegNo, i);
 			// 원부키 설정
 			if ( pDlg->m_strAccessionRecKey != _T("") )
 			{
 				ids = DM_Book.SetCellData( strHD_Book + _T("_원부KEY"), pDlg->m_strAccessionRecKey, i);
 			}
 						
 		}
 
 		//===================================================================
 		// 7. MARC의 049$l값 모두 12자리로 만들기
 		//===================================================================
 		CArray<CString, CString&> arr049Item;
 		CString strItemData;
 		ids = pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arr049Item);
 		for ( INT n049Cnt = 0 ; n049Cnt < arr049Item.GetSize(); n049Cnt++ )
 		{
 			CString str049Item;
 			str049Item = arr049Item.GetAt( n049Cnt );

 			if ( str049Item.GetLength() > nRegCodeLength && str049Item.GetLength() < 12 ) 
			{
 				str049Item = pDlg->m_pInfo->MakeRegNo(str049Item.Left(nRegCodeLength), str049Item.Mid(nRegCodeLength));
 			}

 			pDlg->m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), strItemData, arr049Item.GetAt( n049Cnt ) );
 		}
 		
 		//===================================================================
 		// 8. 종의 정리상태를 1로 세팅
 		//===================================================================
		//===================================================
		//2009.03.17 UPDATE BY PJW : 1. 작업중인 종의 상태만 변경해준다.
//									 2. 마크반입은 청구기호를 생성해주지 않음으로 정리상태가 '0'이 들어간다.
//										0 - 정리안됨, 1 - 정리됨
		//2009.03.25 UPDATE BY PJW : 1. 정리에서의 반입은 정리상태를 1로 셋팅한다. -> 박민경씨 요구사항
// 		for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
//		{
// 			DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("1") , i );
// 		}
		DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("1") , 0 );
		//===================================================
 		
 		//===================================================================
 		// 9. 소장반입이면 작업상태를 소장으로 DB에 INSERT한다. 
 		//===================================================================
 		CButton * pChkWorkStatus;	
 		pChkWorkStatus = (CButton *)pDlg->GetDlgItem(IDC_ChkWorkStatus);
 		if ( pChkWorkStatus->GetCheck() == 0 ) 
 		{
			DM_Species.SetCellData( strHD_Species + _T("_작업상태구분"), "0" , 0 );
			DM_Index.SetCellData( strHD_Index + _T("_작업상태INFO"), "BOL112N" , 0 ); 
			
 			ids = pDlg->ImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume);
 		}
 		//===================================================================
 		// 10. 정리중 반입이면 작업상태를 소장으로/대표책 배가실 부여/ DB에 INSERT한다.
 		//===================================================================
 		else 
 		{
 			CString strRegNo;
 			CString strQuery;
 			CString strResult;			
 			bError = FALSE;
			strResult = _T("");
 			for ( INT i = 0 ; i < DM_Book.GetRowCount() ; i++ ) 
 			{
 				// 2008.04.23 ADD BY PDJ
 				// 정리중 반입일 경우도 등록번호 중복조사
 				// 등록번호 넣기
 				DM_Book.GetCellData( _T("BB_등록번호"), i, strRegNo );
 				
 				strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO = '%s' AND MANAGE_CODE = '%s'"), DM_Book.TBL_NAME, strRegNo, pDlg->m_pInfo->MANAGE_CODE);
 				ids = DM_Species.GetOneValueQuery(strQuery, strResult);	

//				if (ids >= 0 && !strResult.IsEmpty())
// 				{
// 					//등록번호 중복조사
// 					strErrorMsg.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
// 					bError = TRUE;
// 					break;					
// 				}
				if ( !strResult.IsEmpty() )
 				{
 					//등록번호 중복조사
 					strErrorMsg.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
 					bError = TRUE;
 					break;					
 				}

 				DM_Book.SetCellData( strHD_Book + _T("_자료상태"), "BOC113N" , i );
 			}
 
 			if ( bError == FALSE )
 			{		

				DM_Species.SetCellData( strHD_Species + _T("_작업상태구분"), "1" , 0 );
				DM_Index.SetCellData( strHD_Index + _T("_작업상태INFO"), "BOC113N" , 0 );
 				
 				
 				//대표책으로 배가실 부여
 				INT nIdx;
 				pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
 				
 				CString sTmpStreamMarc;
 				CMarc marc;
 				
 				DM_Species.GetCellData( strHD_Species + _T("_MARC"), nIdx, sTmpStreamMarc);
 				pDlg->m_pInfo->pMarcMgr->Decoding(sTmpStreamMarc, &marc );
 				
 				ids = ((CCatApi_BO*)pDlg->m_pCatApi)->ApplyShelfCodeData(&DM_Book, &marc, FALSE);
 				
 				
 				CStringArray m_arrStrAddFrame;
 				
 				// 소정 수정
 				INT nids = -1;
 				nids = DM_Species.StartFrame();
 				if ( 0 > nids ) 
				{
					goto ERR;
				}
 				
 				ids = pDlg->InsertSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume, &DM_Volume, &m_arrStrAddFrame, FALSE);

				//=====================================================
				//2008.12.29 ADD BY PJW : 홈체이지 IDX_BO_TBL에 추가할 쿼리를 SENDFRAME바로 앞으로 뺀다.
				CString strSpeciesKey;
				DM_Species.GetCellData(_T("BS_종KEY"),0,strSpeciesKey);
			 	pDlg->m_pCatApi->ApplyExtraQuery(&DM_Species, &DM_Index, &DM_Book, strSpeciesKey, &m_arrStrAddFrame);
				//=====================================================
 				
 				nids = DM_Species.SendFrame();
 				if (nids < 0) {
 					AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
 					goto ERR;
 				}
 				
 				nids = DM_Species.EndFrame();
 				if ( 0 > nids ) goto ERR;
 			}
 		}		
 		
 		if ( bError == TRUE  || 0 > ids )
 		{
 			strErrorMsg = pDlg->GetLastError();
			if (!strErrorMsg.IsEmpty())
 			{
 				pDM_OK->SetCellData( _T("BS_파일반입오류"), strErrorMsg, nSelectIdx);
 			}
			else
			{				
				pDM_OK->SetCellData(_T("BS_파일반입오류"), _T("마크반입 에러"), nSelectIdx);
			}

 		}
 		//===================================================================
 		// 11. 반입이 완료되면 성공탭에 옮기고 메인 그리드에 반영한다.
 		//===================================================================
 		else
 		{
 			CString strItemData;
 			CString strRecKey;
			INT nRowIdx = 0;
 
 			// 색인 정보 카피 
			nRowIdx = pDlg->m_pDM_Index_Output->GetRowCount();
			pDlg->m_pDM_Index_Output->InsertRow(-1); 			
 			pDlg->CopyDM(&DM_Index, 0, pDlg->m_pDM_Index_Output, nRowIdx);

			//===================================================
			//2010.06.01 ADD BY PJW : 추가 데이터
			pDlg->m_pDM_Index_Output->SetCellData(_T("IBS_종구분값"), _T("1"), nRowIdx);
			pDlg->m_pDM_Index_Output->SetCellData(_T("IBS_소장일자"), _T("2010/06/30"), nRowIdx);
			pDlg->m_pDM_Index_Output->SetCellData(_T("IBS_작업상태INFO"), _T("BOL112N"), nRowIdx);
			//===================================================
 			
 			// 종 정보 카피
			nRowIdx = pDlg->m_pDM_Species_Output->GetRowCount();
			pDlg->m_pDM_Species_Output->InsertRow(-1); 			
 			pDlg->CopyDM(&DM_Species, 0, pDlg->m_pDM_Species_Output, nRowIdx);

			//===================================================
			//2010.06.01 ADD BY PJW : 추가 데이터
			pDlg->m_pDM_Species_Output->SetCellData(_T("BS_작업상태구분"), _T("0"), nRowIdx);
			//===================================================

			// 책 정보 카피
//			CString strItemData = _T("");		
			for( INT i = 0 ; i < DM_Book.GetRowCount() ; i++ )
			{
				nRowIdx = pDlg->m_pDM_Book_Output->GetRowCount();
				pDlg->m_pDM_Book_Output->InsertRow(-1);
 				pDlg->CopyDM(&DM_Book, i, pDlg->m_pDM_Book_Output, nRowIdx);

				//===================================================
				//2010.06.01 ADD BY PJW : 추가 데이터
				pDlg->m_pDM_Book_Output->SetCellData(_T("BB_배가변경일자"), _T("2010/06/30"), nRowIdx);
				pDlg->m_pDM_Book_Output->SetCellData(_T("BB_최초배가일자"), _T("2010/06/30"), nRowIdx);
				pDlg->m_pDM_Book_Output->SetCellData(_T("BB_배가작업자변경자"), pDlg->m_pInfo->USER_ID, nRowIdx);
				pDlg->m_pDM_Book_Output->SetCellData(_T("BB_자료상태"), _T("BOL112N"), nRowIdx);

				ids = pDlg->m_pDM_Book_Output->GetCellData( _T("BB_등록번호"), nRowIdx, strItemData);
				pDlg->m_pDM_Book_Output->SetCellData(_T("BB_등록구분_코드"), strItemData.Left(pDlg->m_pInfo->nREG_CODE_LENGTH), nRowIdx);
				//===================================================
			}

 			// 누락등록번호에 있는 등록번호라면 갱신해준다.
 			CString strRegNo ;
 			CString strGetAccRegNo ;
 			CString strQuery ;
 			CString strRegCode ;
 			INT iRegNumber ;
 			INT nCount ;
 			CString strCount =_T("") ;
 			
 			nCount = DM_Book.GetRowCount () ;
 			DM_Book.GetCellData ( strHD_Book + _T("_등록번호"),idxDM,strRegNo);
 			if(!strRegNo.IsEmpty ())
 			{   
 				strRegCode = strRegNo.Left(nRegCodeLength) ;
 				iRegNumber = _ttoi(strRegNo.Mid (nRegCodeLength)) ;
 			
				strQuery.Format (_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL  WHERE REG_CODE = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
 				DM_Book.StartFrame () ;
 				DM_Book.InitDBFieldData () ;
 				DM_Book.AddFrame (strQuery);
 				DM_Book.SendFrame ();
 				DM_Book.EndFrame () ;
 			}
 		}
 
 		//===================================================================
 		// 12. 상태바의 개수를 설정한다.
 		//===================================================================
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(1);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) - 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(_T("정상 : ") + strData, 1, 0);
// 		
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(nID_TO);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) + 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(strTO + strData, nID_TO, 0);
 
 
 		//2003.10.15 이학중 수정
//          nTotalCnt =  strTO+strData ; 
 		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
 		//===================================================================
 		// 13. 오류및 성공 탭으로 데이터를 옮긴다.
 		//===================================================================
 
 		// 오류 가 있으면 오류 세팅
// 		if (!strErrorMsg.IsEmpty())
// 		{
// 			pDM_OK->SetCellData( _T("BS_파일반입오류"), strErrorMsg, idx);
// 		}

 		//===================================================================
 		// 14. 진행바를 움직여준다.
 		//===================================================================
 		pDlg->m_wndProgressBar.StepIt(); 

 		end_app = GetTickCount();
 		nProcCount++;
 		strMsg.Format(_T("ROW %d \t %d.%03d\n"), nProcCount, (end_app-start_app)/1000, (end_app-start_app)%1000);	
 		_ftprintf( fpTime, strMsg );
		
		nSelectIdx = pGrid_OK->SelectGetNext();
 	}

 	// 전체 반입시간
 	end_total = GetTickCount();
 	strMsg.Format(_T("전체반입시간 \t %d.%03d\n======================================\n"), (end_total-start_total)/1000, (end_total-start_total)%1000);	
 	_ftprintf( fpTime, strMsg );		
 	fclose(fpTime);
 		
 	pDlg->m_wndProgressBar.SetWindowText(_T("반입 완료."));

	nSelectIdx = pGrid_OK->SelectGetTailPosition();
	if ( !pDlg->m_bIsThreadRunning ) 
	{
		for( INT i = 0 ; i < nSelectRowCnt - nStopCnt ; i++ )
		{
			nSelectIdx = pGrid_OK->SelectGetPrev();
		}

		nSelectRowCnt = nStopCnt;
	}

	for( nChkCnt = 0 ; nChkCnt < nSelectRowCnt ; ++nChkCnt )
	{
		INT nRowsTo;

		strData = pDM_OK->GetCellData( _T("BS_파일반입오류"), nSelectIdx);

		if( strData.IsEmpty() )
		{
			pDM_TO = pDM_SUCCESS;
		}
		else
		{
			pDM_TO = pDM_ERR;
		}

		nRowsTo = pDM_TO->GetRowCount();
		pDM_TO->InsertRow(-1);
		
		for (INT col = 0; col < nColCnt; col++)
 		{	
			strData = pDM_OK->GetCellData( nSelectIdx ,col);
 			pDM_TO->SetCellData( nRowsTo ,col,  strData);

 		}

		pDM_OK->DeleteRow( nSelectIdx);
		nSelectIdx = pGrid_OK->SelectGetPrev();
	}

	{
	//===================================================================
 	// 상태바의 개수를 설정한다.
 	
	INT nRowCnt;
	CString strCount;

	nRowCnt = pDM_OK->GetRowCount();
	strCount.Format(_T("정상 : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);	
	
	nRowCnt = pDM_ERR->GetRowCount();

	strCount.Format(_T("오류 : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);

	nRowCnt = pDM_SUCCESS->GetRowCount();
	strCount.Format(_T("성공 : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 3, 0);	
	}
	//===================================================================
 
 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 	//2003.10.20 이학중 수정 
	nTotalCnt = pDlg->m_wndStatusBarCtrl.GetText(0) ;
    nFalseCnt = pDlg->m_wndStatusBarCtrl.GetText(2) ;
 	nTrueCnt  = pDlg->m_wndStatusBarCtrl.GetText(3) ;
 
 	strTotalMsg.Format( _T("반입 완료. \r\n\r\n %s건 \r\n %s건 \r\n %s건"),nTotalCnt,nFalseCnt,nTrueCnt ); 
 	AfxMessageBox(strTotalMsg);
 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
 	pDlg->m_bIsThreadRunning = FALSE;
 	pDlg->ButtonStatus(1);

	::PostMessage(pDlg->GetSafeHwnd(), WM_SORT_GRID, 0, 0);

 	SetEvent(pDlg->m_hThread);
 
 	return 0;
 
 ERR:
 	pDlg->m_wndProgressBar.SetWindowText(_T("반입 실패"));
 	AfxMessageBox(_T("반입 실패"));
 	pDlg->m_bIsThreadRunning = FALSE;
 	pDlg->ButtonStatus(1);
 	return -1;
 }
//===================================================

UINT ThreadFunc_Display(LPVOID pParam)
{
	CExcelToMarcDlg *pDlg = (CExcelToMarcDlg*)pParam;
	// KOL.UDF.022
	if (&pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	pDlg->m_bIsThreadRunning = TRUE;
	pDlg->ButtonStatus(0);

	// Initialization Variable
	CESL_DataMgr *pDM_Species = NULL;
	CESL_ControlMgr *pCM = NULL;

	CESL_DataMgr *pDM_OK		= NULL;
	CESL_DataMgr *pDM_ERR		= NULL;
	CESL_DataMgr *pDM_SUCCESS	= NULL;

	CESL_DataMgr *pTempDM = NULL;

	CString strHD_Species;

	// For Count
	CString strCount;
	
	INT i = 0;
	INT nTotalCount = -1;
	INT nErrCount = 0;
	INT nOKCount = 0;

	INT row = 0;

	CString strAlias;
	CStringArray astrAlias;
	CString strFieldName;
	CString strFieldType;
	CString strDataType;
	CString strData;
	INT nColCnt;

	CString strSortNo;
	strSortNo.Empty();

	//===================================================================
	// 1. 상태바/진행바/그리드를 초기화한다.
	//===================================================================

	// 종정보 DM
	pDM_Species = pDlg->m_pDM_Species;
	if (pDM_Species == NULL)
	{
		goto ERR;
	}
	
	nTotalCount = pDM_Species->GetRowCount() ;
	
	pCM = pDlg->FindCM(pDlg->m_lpszCMAlias);
	if (pCM == NULL)
	{
		goto ERR;
	}

	// TOTAL_COUNT
	strCount.Format(_T("전체 : %d"), nTotalCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 0, 0);
	
	// 소정수정
	//if ( nTotalCount/stepQuota == 0 )
	//	pDlg->m_wndProgressBar.SetRange(0, 1);
	//else 
	pDlg->m_wndProgressBar.SetRange(0, nTotalCount);
	
	pDlg->m_wndProgressBar.SetStep(1);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));
		
	pDM_OK		= pDlg->m_pDmGridOk;
	pDM_ERR		= pDlg->m_pDmGridError;
	pDM_SUCCESS	= pDlg->m_pDmGridSuccess;

	
// 	INT i;
	nColCnt = pDM_OK->GetRefColumnCount();
	for( i = 0 ; i < nColCnt ; i++ )
	{
		pDM_OK->FindColumnFieldName( i , strAlias , strFieldName ,strFieldType,strDataType,0,strData);
		astrAlias.Add(strAlias);
	}


	//========================   Fetch Start  ===========================
	INT nDmRowIdx;
	
	for (row = 0; row < nTotalCount; row++)
	{
		CString sBookCnt;

		//===============================================================
		// 2. 구성된 종DM으로 정상/에러 개수를 카운트 한다.
		//===============================================================
		pDlg->m_wndProgressBar.StepIt();

		if (pDM_Species->GetCellData( _T("BS_파일반입오류"), row).IsEmpty())
		{
			pTempDM = pDM_OK;
		}
		else
		{
			pTempDM = pDM_ERR;
		}
		nDmRowIdx = pTempDM->GetRowCount();
		pTempDM->InsertRow(-1);
		
		if (!pDlg->m_bIsThreadRunning) 
		{
			break;
		}
		
		//===============================================================
		// 3. 책수를 세팅하고 등록번호및 GRID를 세팅한다.
		//===============================================================
		INT nRegCodeLength;
		nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
		for( i = 0 ; i < nColCnt ; i++ )
		{
			strAlias = astrAlias.GetAt(i);
			if( _T("선정") == strAlias ) 
			{
				continue;
			}
			if( _T("순위") == strAlias )
			{
				strData.Format(_T("%d"), row+1);
			}
			else if(  _T("시작등록번호") == strAlias )
			{
				//자동부여 체크일 경우 그리드에 자동부여로 뿌려주기
				CButton * pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
				
				if ( (pChkGiveAutoRegno->GetCheck() == 1) ) 
				{
					strData = _T("자동부여");
				}
				else
				{
					strData = pDM_Species->GetCellData(_T("BS_MARC.049$l"), row);
					if ( strData.GetLength() > nRegCodeLength && strData.GetLength() < 12 ) 
					{
						strData =  pDlg->m_pInfo->MakeRegNo( strData.Left(nRegCodeLength) , strData.Mid(nRegCodeLength) );						
					}
				}
			}
			else if( _T("BS_파일반입책수") == strAlias )
			{
				CButton * pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
				if ( (pChkGiveAutoRegno->GetCheck() == 1) ) 
				{
					CButton * pChkMultiBook = (CButton *)pDlg->GetDlgItem(IDC_ChkMULTIBOOK);
					if( pChkMultiBook->GetCheck() == 1)
					{
						strData = pDM_Species->GetCellData(_T("UDF_책수"),row);
					}
					else
					{
						strData = _T("1");
					}
				}
				else
				{
					strData = pDM_Species->GetCellData(_T("BS_파일반입책수"),row);					
				}

				
			}
			else if( _T("BS_파일반입오류") == strAlias )
			{
				strData = pDM_Species->GetCellData(_T("BS_파일반입오류"),row);
			}
			else if( _T("낱권ISBN") == strAlias )
			{
				strData = pDM_Species->GetCellData(_T("BS_MARC.020$a"),row);
			}
			else
			{
				pDM_Species->GetCellData(("BS_MARC.") + strAlias, row, strData);
			}

			pTempDM->SetCellData( nDmRowIdx, i, strData);
		}
	}

	INT nRowCnt;

	nRowCnt = pDM_OK->GetRowCount();

	strCount.Format(_T("정상 : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);	
	
	nRowCnt = pDM_ERR->GetRowCount();

	strCount.Format(_T("오류 : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);

	pDlg->m_wndStatusBarCtrl.SetText(_T("성공 : 0"), 3, 0);

	pDlg->m_wndProgressBar.SetWindowText(_T("파일패치 완료"));

	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);

	SetEvent(pDlg->m_hThread);

	::PostMessage(pDlg->GetSafeHwnd(), WM_GRID_DISPLAY, 0, 0);
	
	return 0;
ERR:
	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);
	return -1;

	return -1;
}
//===================================================



/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 마크 콜백 함수 (유효성검사)
// [Return Values] 에러면 음수
// [Mini Spec] 
// 1. 필수 태그 선택
// 2. 리더검사
// 3. 소장반입이면 별치/가격(950) 유효성을 검사한다. 
// 4. 653 Tag 유무검사 : 없으면 생성
// 5. 필수태그 검사
// 6. 008 유무검사 : 없으면 생성
// 7. 005 최종수정일 갱신
/////////////////////////////////////////////////////////////////////////////
INT CExcelToMarcDlg::CallBackCheckMarc(CMarc *pMarc, CString &strCheckErrMsg)
{
	EFS_BEGIN

	if (pMarc == NULL) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nCount = -1;
	
	CString strItem;
	CString strTemp;
	CString strItemData;
	CString strErrMsg;
	CStringArray arrStrErrMsg;
	
	CArray<CString, CString&> arrItem;
	
	CStringArray arrStrEssentialTag;

	CButton * pChkWorkStatus;	
	CButton * pChkRegNoAutoGive;

	strCheckErrMsg.Empty();

	//=======================================================================
	// 1. 필수 태그 선택
	//=======================================================================
	arrStrEssentialTag.Add(_T("245"));

	//자동부여 부여 버튼
	pChkRegNoAutoGive = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
	if ( pChkRegNoAutoGive->GetCheck() == 0 ) {
		arrStrEssentialTag.Add(_T("049"));
	}

	//정리중 반입 버튼
	pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkWorkStatus);
	if ( pChkWorkStatus->GetCheck() == 0 ) {
		arrStrEssentialTag.Add(_T("008"));

		//arrStrEssentialTag.Add(_T("090"));

		//2004.12.13 반입반을 땐 분류기호 반드시 체크 하지 않아도 됨
		//분류기호는 
		//arrStrEssentialTag.Add(_T("056"));
		arrStrEssentialTag.Add(_T("260"));
		arrStrEssentialTag.Add(_T("300"));
	}

	//=======================================================================
	// 2. 리더검사
	//=======================================================================
	CString strLeader;
	pMarc->GetLeader(strLeader);
	TCHAR ch07 = strLeader.GetAt(7);
	if (ch07 == ' ') 
	{
		strLeader.SetAt(7, 'm');
	}
	else 
	{
		if (!(ch07 == 'a' || ch07 == 'c' || ch07 == 'm'))
		{
			strErrMsg = _T("단행 서지수준이 아닙니다.. 수정해주세요");
			arrStrErrMsg.Add(strErrMsg);
		}
	}

	//=======================================================================
	// 3. 소장반입이면 별치/가격(950) 유효성을 검사한다. 
	//=======================================================================
	if ( pChkWorkStatus->GetCheck() == 0 ) {
		// 별치기호 내용검사
		if ( pChkRegNoAutoGive->GetCheck() == 0 )
		{
			ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("049$f"), strItemData, &arrItem);
			if (ids >= 0)
			{
				ids = m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strItemData, strItemData);
				if (ids < 0) 
				{
					strErrMsg = _T("별치기호 값이 잘못됬습니다. 수정해주세요");
					arrStrErrMsg.Add(strErrMsg);
				}
			}
		}

		// 950$b 에 화폐구분 있는지 가격 검사 & 화폐구분 내용검사
		ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$b"), strItemData);
		if (ids >= 0)
		{
			// 화폐구분이 있는지 검사
			strItemData = GetPriceCode(strItemData, strTemp);
			if (strItemData.IsEmpty()) 
			{
				strErrMsg = _T("[950$b] 에 화폐구분이 없습니다. 수정해주세요");
				arrStrErrMsg.Add(strErrMsg);
			}

			// 화폐구분이 유효한지 검사
			ids = m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("화폐부호관리"), strItemData, strItemData);
			if (ids < 0)
			{
				strErrMsg = _T("[950$b] 의 화폐구분 값이 잘못됬습니다. 수정해주세요");
				arrStrErrMsg.Add(strErrMsg);
			}
		}

		/*-----------------------------------------------------------------------
		// 청구기호 유무 검사
		ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("090$a"), strItemData);
		if (ids < 0)
		{
			strErrMsg = _T("청구기호가 없습니다. 수정해주세요");
			arrStrErrMsg.Add(strErrMsg);
		}
		*/
	}

	//=======================================================================
	// 4. 653 Tag 유무검사 : 없으면 생성
	//=======================================================================
	CButton * pChk653Tag;
	pChk653Tag = (CButton *)GetDlgItem(IDC_Create653Tag);
	if ( pChk653Tag->GetCheck() == 1 ) {	
		ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("653$a"), strItemData);	
		if (ids < 0)
		{
			// TODO : CREATE KEY WORD			
			CString strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
			m_pDlgKeyWord->Init(strStreamMarc);
			m_pDlgKeyWord->MakeKeyWord();
			*pMarc = m_pDlgKeyWord->m_marc;		
			
			//arrStrErrMsg.Add(_T("[653] Tag가 없습니다."));	
		}
	}

	//=======================================================================
	// 5. 필수태그 검사
	//=======================================================================
	nCount = arrStrEssentialTag.GetSize();
	for (idx = 0; idx < nCount; idx++)
	{
		strItem = arrStrEssentialTag.GetAt(idx);
		if (pMarc->FindTag(strItem) == NULL)
		{
			strErrMsg.Format(_T("[%s] Tag을 입력해주세요"), strItem);
			arrStrErrMsg.Add(strErrMsg);
		}
	}

	//=======================================================================
	// 6. 008 유무검사 : 없으면 생성
	//=======================================================================
	ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("008@0-5"), strItemData);
	CIndex::TrimAll(strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		strItemData = CTime::GetCurrentTime().Format(_T("%y%m%d"));
		m_pInfo->pMarcMgr->SetItem(pMarc, _T("008@0-5"), strItemData);
	}

	//=======================================================================
	// 7. 005 최종수정일 갱신
	//=======================================================================
	CTime time = CTime::GetCurrentTime();	
	strItem.Format(_T("%04d%02d%02d%02d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	m_pInfo->pMarcMgr->DeleteField(pMarc, _T("005"));
	m_pInfo->pMarcMgr->InsertField(pMarc, _T("005") + strItem);
	
	nCount = arrStrErrMsg.GetSize();
	for (idx = 0; idx < nCount; idx ++)
	{
		strCheckErrMsg += arrStrErrMsg.GetAt(idx) + _T("\n");
	}

	strCheckErrMsg.TrimLeft();
	strCheckErrMsg.TrimRight();

	if (nCount > 0)
		return -1;
	else
		return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  책수를 구한다. 
// [Parameters] 책DM/알리아스헤더/등록구분/볼륨명
// [Return Values] 성공 0 에러 -1
// [Mini Spec] 
// 책수를 구한다.
/////////////////////////////////////////////////////////////////////////////
INT CExcelToMarcDlg::GetBookCount(CESL_DataMgr *pDM_Book, 
						  CString strToAliasHeader /*= _T("")*/, 
						  CString strRegCode /*= _T("")*/, 
						  LPCTSTR lpszVol /*=NULL*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));

	INT nReturnCnt = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 책과 권이 아니면 넘어간다.
		if (!IsBook(pDM_Book, idx, strToAliasHeader, lpszVol)) continue;

		// 등록구분 비교
		if (!strRegCode.IsEmpty())
			if (pDM_Book->GetCellData( strToAliasHeader + _T("_등록구분_코드"), idx) != strRegCode) continue;
		
		nReturnCnt++;
	}

	return nReturnCnt;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  대표책 세팅 
// [Parameters] 책DM/알리아스헤더/인덱스
// [Return Values] 성공 0 에러 -1
// [Mini Spec] 
// 대표책을 구한다.
// 대표책이 없으면 첫번째 책이 대표책
/////////////////////////////////////////////////////////////////////////////
INT CExcelToMarcDlg::SetRepresentationBook(CESL_DataMgr *pDM_Book, 
								   CString strToAliasHeader, 
								   INT nIdx /*= -1*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));

	// nIdx 가 -1이고 대표책이 없을경우 첫번째 책을 대표책으로 한다.

	INT ids = -1;

	INT nRowCount = pDM_Book->GetRowCount();
	
	if (nIdx == -1)
	{
		if (GetRepresentationBook(pDM_Book, strToAliasHeader, nIdx) < 0)
		{
			// 대표책이 없을경우
			for (INT idx = 0; idx < nRowCount; idx++)
				if (IsBook(pDM_Book, idx, strToAliasHeader)) break;
			nIdx = idx;
		}		
	}
	else
	{
		// nIdx 의 데이터가 책인지 확인한다.
		if (!IsBook(pDM_Book, nIdx, strToAliasHeader)) return -1;
	}

	// 대표책 IDX만 빼고 나머지는 N으로 세팅해 준다.
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (idx == nIdx)
			pDM_Book->SetCellData( strToAliasHeader + _T("_대표책여부"), _T("Y"), idx);
		else
			pDM_Book->SetCellData( strToAliasHeader + _T("_대표책여부"), _T("N"), idx);
	}
	
	return nIdx;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  DM 복사  
// [Parameters] 소스DM/소스인덱스/타겟DM/타겟인덱스/알리아스헤더/거부리스트
// [Return Values] 성공 0 에러 -1
// [Mini Spec] 
// 거부리스트를 제외한 알리스에 맞춰 DM을 복사한다.
/////////////////////////////////////////////////////////////////////////////
INT CExcelToMarcDlg::CopyDM(CESL_DataMgr *pDM_From, INT nFromIdx, CESL_DataMgr *pDM_To, INT nToIdx, CString strToAliasHeader /*= _T("")*/, CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	EFS_BEGIN

	if (pDM_From == NULL || pDM_To == NULL) return -1;

	INT ids = -1;
	INT nFind = -1;

	INT nRowCount = pDM_From->GetRowCount();
	if (nRowCount - 1 < nFromIdx || nFromIdx < 0) return -1;

	nRowCount = pDM_To->GetRowCount();
	if (nRowCount - 1 < nToIdx ) return -1;

	if (nToIdx < 0)
	{
		ids = pDM_To->InsertRow(-1);
		if (ids < 0) return -1;
		nToIdx = pDM_To->GetRowCount() - 1;
	}	

	BOOL bSwitchHeader = FALSE;
	
	if (!strToAliasHeader.IsEmpty())
		bSwitchHeader = TRUE;
	
	CString strAlias;
	CString strItemData;

	CESL_DataMgr::reference *pRef = NULL;
	
	POSITION pos = pDM_From->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)pDM_From->RefList.GetNext(pos);
		if (pRef == NULL) continue;

		strAlias = pRef->FIELD_ALIAS;

		// 거부 리스트에 있는지 확인
		// Default Ignore List
		if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
		if ( strAlias == _T("IBS_배가위치부호") ) continue;
		// User Ignore List
		if (pArrIgnoreAlias != NULL)
			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
		
		ids = pDM_From->GetCellData(strAlias, nFromIdx, strItemData);
		if (ids < 0) continue;

		// 헤더 변경
		if (bSwitchHeader)
		{
			nFind = strAlias.Find(_T("_"));
			if (nFind >= 0)
				strAlias = strAlias.Mid(nFind + 1);
			strAlias = strToAliasHeader + _T("_") + strAlias;
		}
		/*
		if ( strAlias == _T("BB_권책기호") || strAlias == _T("BB_편권차") ) continue;
		*/
		if ( strAlias == _T("BB_등록구분_코드") ) continue;
		ids = pDM_To->SetCellData(strAlias, strItemData, nToIdx);
		if (ids < 0) continue;
	}

	return nToIdx;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  DB에 저장
// [Return Values] 에러면 음수
// [Mini Spec] 
// 1. 색인의 내용을 종에 적용시킨다.
// 2. 기본 정보를 적용시킨다.
// 3. 정리중 반입이면 제어번호 제거
// 4. 색인 쿼리 생성
// 5. 구입/기증정보 쿼리 생성
// 6. 책정보 쿼리 생성
// 7. ebook 관련 저장 하기 
// 8. 나머지쿼리 생성
// 9. 마지막 번호 Update
/////////////////////////////////////////////////////////////////////////////
INT CExcelToMarcDlg::InsertSpecies( CESL_DataMgr *pDM_Species, 
								  CESL_DataMgr *pDM_Index, 
								  CESL_DataMgr *pDM_Book, 
								  CESL_DataMgr *pDM_Volume, 
								  CESL_DataMgr *pDM_Appendix, 
								  CStringArray *pArrStrAddFrame, 
								  BOOL bCommit)
{
	EFS_BEGIN

	if (pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL
		) return -1;

	INT ids = -1;

	// 종키
	CString strSpeciesKey;

	// 쿼리 만들때 무시할 Alias Array
	CArray<CString, CString> arrRemoveAliasList;
	
	//========================================================================
	// 1. 색인의 내용을 종에 적용시킨다.
	//========================================================================
	CopyDM(pDM_Index, 0, pDM_Species, 0,  _T("BS"));

	//========================================================================
	// 2. 기본 정보를 적용시킨다.
	//========================================================================
	ids = m_pCatApi->ApplyDefaultValue(pDM_Species, pDM_Index, pDM_Book);
	if (ids < 0) return ids;

	// 쿼리 만들기 시작

	// 종 쿼리를 만든다.
	CMarc marc;
	CString strStreamMarc;
	
    ids = pDM_Species->GetCellData( _T("BS_MARC"), 0, strStreamMarc);
	ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

	//===================================================
	//2010.05.03 ADD BY PJW : 정리중 반입도 040tag 정보 추가하도록 함
	m_pCatApi->ApplyLastModifyData(&marc);
	//===================================================
	//===============================================
	//===============================================
	//2008.07.16 ADD BY PJW : 화폐구분을 마크에서 가져온다.
	CString strCurrencyCode;
	CString strTemp;
	ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("950$b"), strTemp);
	strCurrencyCode = GetPriceCode(strTemp, strCurrencyCode);
	//===============================================
	
	//========================================================================
	// 3. 정리중 반입이면 제어번호 제거
	//========================================================================	
	ids = m_pInfo->pMarcMgr->DeleteField( &marc, _T("001") );
	ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("BS_MARC"), strStreamMarc, 0 );

	ids = pDM_Species->MakeRecKey(strSpeciesKey);
	if (ids < 0) return ids;
	pDM_Species->SetCellData( _T("BS_종KEY"), strSpeciesKey, 0);
	
	pDM_Species->InitDBFieldData();

	ids = pDM_Species->MakeQueryByDM(arrRemoveAliasList, pDM_Species, pDM_Species->TBL_NAME, 0, pDM_Species);
	m_pCatApi->LogToFile(pDM_Species->m_lastmsg);

	//========================================================================
	// 4. 색인 쿼리 생성
	//========================================================================	
	// 색인에 Insert할 Insert문을 구성한다.
	pDM_Index->SetCellData( _T("IBS_REC_KEY"), strSpeciesKey, 0);
	pDM_Index->SetCellData( _T("IBS_종구분값"), _T("0"), 0);

	// 색인 데이터 적용
	arrRemoveAliasList.RemoveAll();
	ids = m_pCatApi->ApplyIndex(pDM_Species, pDM_Index, pDM_Book);

	// ST_CODE를 설정한다.
	m_pCatApi->ApplyIndex_ST_CODE(pDM_Index, pDM_Book);
	
//	pDM_Species->InitDBFieldData();

//	ids = pDM_Species->MakeQueryByDM(arrRemoveAliasList, pDM_Index, pDM_Index->TBL_NAME, 0, pDM_Species);
//	m_pCatApi->LogToFile(pDM_Species->m_lastmsg);

	//이용제한구분/관리구분/자료실/등록구분 구분값의 구분값 concatenation
	CStringArray Index_classfycodes;
	Index_classfycodes.Add( _T("IBS_이용제한구분_코드"));
	Index_classfycodes.Add( _T("IBS_관리구분_코드"));
	Index_classfycodes.Add( _T("IBS_자료있는곳INFO_코드"));
	Index_classfycodes.Add( _T("IBS_등록구분_코드"));

	CStringArray Book_classfycodes;
	Book_classfycodes.Add( _T("BB_이용제한구분_코드"));
	Book_classfycodes.Add( _T("BB_관리구분_코드"));
	Book_classfycodes.Add( _T("BB_배가위치부호"));
	Book_classfycodes.Add( _T("BB_등록구분_코드"));

	CString Index_Update_sql;
//	m_pCatApi->MakeIndexClassifyCodeConcatenationSQL(pDM_Index, pDM_Book, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strSpeciesKey, _T("BB"));

//	ids = pDM_Species->AddFrame(Index_Update_sql);
//	if (ids < 0) return ids;

	//========================================================================
	// 5. 구입/기증정보 쿼리 생성
	//========================================================================
	CString sAcqCode = pDM_Book->GetCellData( _T("BB_수입구분_코드"), 0 );
	
	CString sAcqKey = _T("");
//	ids = pDM_Species->MakeRecKey(sAcqKey);
	if ( 0 > ids ) return -1;

	CString sProvider = pDM_Book->GetCellData( _T("BB_구입처"), 0 );
	//===============================================
	//20080630 ADD BY PJW : 입력일자를 추가한다.수입년도를 추가한다.
	CTime t = CTime::GetCurrentTime();
	CString strInputDate;
	CString strAcqYear;
	strInputDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	GetDlgItem( IDC_eACQUISIT_YEAR )->GetWindowText(strAcqYear);
	//===============================================
	//===============================================
	//2008.07.08 ADD BY PJW : 화폐구분, 가격정보를 추가한다.
	CString strPrice; 
	strPrice = pDM_Book->GetCellData( _T("BB_가격"), 0 );
	//===============================================

	//===================================================
	//2010.06.01 ADD BY PJW : 구입정보 기증 정보는 제거한다.
//	if ( sAcqCode == _T("1") ) {
//		pDM_Species->InitDBFieldData();
//		pDM_Species->AddDBFieldData( _T("REC_KEY"), _T("STRING"), sAcqKey);
//		pDM_Species->AddDBFieldData( _T("PROVIDER_CODE"), _T("STRING"), sProvider);		
//		pDM_Species->AddDBFieldData( _T("SPECIES_KEY"), _T("STRING"), strSpeciesKey);
//		//===============================================
//		//20080630 ADD BY PJW : 입력일자를 추가한다.수입년도를 추가한다.
//		pDM_Species->AddDBFieldData( _T("INPUT_DATE"), _T("STRING"), strInputDate);
//		pDM_Species->AddDBFieldData( _T("ORDER_YEAR"), _T("STRING"), strAcqYear);
//		//===============================================
//		//2008.07.08 ADD BY PJW : 화폐구분, 가격정보를 추가한다.
//		pDM_Species->AddDBFieldData( _T("CURRENCY_CODE"), _T("STRING"), strCurrencyCode);
//		strPrice.Replace(_T(","),_T(""));
//		pDM_Species->AddDBFieldData( _T("PRICE"), _T("STRING"), strPrice);
//		//===============================================
//		pDM_Species->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), m_pInfo->MANAGE_CODE );
//
//		pDM_Species->MakeInsertFrame(_T("BO_PURCHASEINFO_TBL"));
//	}
//	else if ( sAcqCode == _T("2") ) {
//		pDM_Species->InitDBFieldData();
//		pDM_Species->AddDBFieldData( _T("REC_KEY"), _T("STRING"), sAcqKey);
//		pDM_Species->AddDBFieldData( _T("DONATOR_NAME"), _T("STRING"), sProvider);		
//		pDM_Species->AddDBFieldData( _T("SPECIES_KEY"), _T("STRING"), strSpeciesKey);
//		//===============================================
//		//20080630 ADD BY PJW : 입력일자를 추가한다.수입년도를 추가한다.
//		pDM_Species->AddDBFieldData( _T("INPUT_DATE"), _T("STRING"), strInputDate);
//		pDM_Species->AddDBFieldData( _T("ACQ_YEAR"), _T("STRING"), strAcqYear);
//		//===============================================
//		//===============================================
//		//2008.07.08 ADD BY PJW : 화폐구분, 가격정보를 추가한다.
//		pDM_Species->AddDBFieldData( _T("CURRENCY_CODE"), _T("STRING"), strCurrencyCode);
//		strPrice.Replace(_T(","),_T(""));
//		pDM_Species->AddDBFieldData( _T("PRICE"), _T("STRING"), strPrice);
//		//===============================================
//		pDM_Species->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), m_pInfo->MANAGE_CODE );
//
//		pDM_Species->MakeInsertFrame(_T("BO_DONINFO_TBL"));
//		
//	}
	//===================================================
	

	//========================================================================
	// 6. 책정보 쿼리 생성
	//========================================================================

	CString strVolSortNo;
	for ( INT i = 0 ; i < pDM_Book->GetRowCount() ; i++ ) {

		pDM_Book->SetCellData( _T("BB_원부KEY"), m_strAccessionRecKey, i );
		pDM_Book->SetCellData( _T("BB_수정정보KEY"), sAcqKey, i );

		strVolSortNo.Format(_T("%d"), (1)*10);
		pDM_Book->SetCellData( _T("BB_권일련번호"), strVolSortNo, i );
	}

	if ( sAcqKey != _T("") ) {
		ids = ((CCatApi_BO*)m_pCatApi)->MakeBookQueryFrame(pDM_Book, strSpeciesKey, sAcqKey, pDM_Species);
	}
	else {
		ids = ((CCatApi_BO*)m_pCatApi)->MakeBookQueryFrame(pDM_Book, strSpeciesKey, _T("-1"), pDM_Species);
	}

	if (ids < 0)
	{
		if ( m_strLastError == _T("") ) 
		{
			m_strLastError = _T("책정보 쿼리만들기 실패");
		}
		return ids;
	}

	//========================================================================
	// 7. ebook 관련 저장 하기
	//========================================================================
    CESL_DataMgr * pDM_EBook ; 
	pDM_EBook = new CESL_DataMgr;
	pDM_EBook->InitDMPKFromFile(_T("DMUP_단행_종_EBOOK"));
	pDM_EBook->SetCONNECTION_INFO(pDM_Species->CONNECTION_INFO);
	pDM_EBook->SetMarcMgr(m_pInfo->pMarcMgr);

	CBO_SPECIES *pBO_Species = new CBO_SPECIES ( this );

    ids = pBO_Species->MakeEBookInfoAndQueryFrame (pDM_Book,pDM_EBook,pDM_Species) ;
    if (ids < 0) 
	{
		m_strLastError = _T("ebook정보 쿼리만들기 실패");
		return ids;
	}

	//========================================================================
	// 8. 나머지쿼리 생성
	//======================================================================== 
	//=====================================================
	//2008.12.29 REM BY PJW : 홈체이지 IDX_BO_TBL에 추가할 쿼리를 SENDFRAME바로 앞으로 뺀다.
// 	m_pCatApi->ApplyExtraQuery(pDM_Species, pDM_Index, pDM_Book, strSpeciesKey, pArrStrAddFrame);
	//=====================================================
	
	// ECOSEARCH용 쿼리 생성
	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		CString strJJSQuery;
		strJJSQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		ids = pDM_Species->AddFrame(strJJSQuery);
		if (ids < 0) return ids;
	}

	//========================================================================
	// 9. 마지막 번호 Update
	//========================================================================
    CESL_DataMgr *pDmLastNum = FindDM( _T("DM_마지막번호") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("DM_마지막번호 확인") );
		return -1;
	}
	pDmLastNum->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	INT iBookCount = pDM_Book->GetRowCount();
	pDM_Book->SORT(_T("BB_등록번호"));
	CString sRegno = pDM_Book->GetCellData( _T("BB_등록번호"), iBookCount-1 );
	INT iRegno = 0;
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	if ( sRegno != _T("") ) {
// 		iRegno = _ttoi(sRegno.Mid(2));
// 	}
	if ( sRegno != _T("") ) {
		iRegno = _ttoi(sRegno.Mid(nRegCodeLength));
	}
	//=====================================================
//===================================================
	//2010.06.01 ADD BY PJW : 
//	pDM_Species
	//===================================================
	ids = pDM_Book->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	for ( i = 0 ; i < pDmLastNum->GetRowCount() ; i++ ) {
		CString sRegCode = pDmLastNum->GetCellData( _T("BB_등록구분_코드"), i );
		CString sLastNum = pDmLastNum->GetCellData( _T("마지막번호"), i );
		INT iLastNum = _ttoi ( sLastNum );
		if ( iRegno != 0 ) {
			iLastNum = iRegno;
		}
		CString sQuery;
		sQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%d', LAST_WORK='%s' WHERE division_value = '%s' AND MANAGE_CODE = '%s';") , iLastNum , _T("자동등록번호부여"), sRegCode, m_pInfo->MANAGE_CODE );
		ids = pDM_Book->AddFrame( sQuery );
		if ( 0 > ids ) {
			AfxMessageBox( _T("마지막번호 AddFrame 확인") );
			return -1;
		}
	}
	ids = pDM_Book->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return -100;
	}
	ids = pDM_Book->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 를 확인하세요"));
		return -100;
	}	

	return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  대표책 구한다.
// [Parameters] 책DM/알리아스헤더/참조인덱스
// [Return Values] 성공 0 에러 -1
// [Mini Spec] 
// 대표책을 구한다.
/////////////////////////////////////////////////////////////////////////////
INT CExcelToMarcDlg::GetRepresentationBook(CESL_DataMgr *pDM_Book, 
								   CString strToAliasHeader, 
								   INT &nIdx)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));
	nIdx = -1;
	
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, strToAliasHeader)) continue;

		if (pDM_Book->GetCellData( strToAliasHeader + _T("_대표책여부"), idx) == _T("Y")) 
		{
			nIdx = idx;
			return 0;
		}
	}

	return -1;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 반입 소장종 DB 저장
// [Return Values] 에러면 음수
// [Mini Spec] 
// 1. 등록번호 중복 조사
// 2. 배가위치 적용
// 3. 도서관 부호 가져오기
// 4. 최종수정정보 생성 040
// 5. LOC_FLAG 넣기
// 6. DB에 저장한다
// 7. 001 TAG 생성
// 8. 임의의 배가일자를 기록한다.
/////////////////////////////////////////////////////////////////////////////
INT CExcelToMarcDlg::ImportSpecies( CESL_DataMgr *pDM_Species, 
								  CESL_DataMgr *pDM_Index, 
								  CESL_DataMgr *pDM_Book, 
								  CESL_DataMgr *pDM_Volume, 
								  CESL_DataMgr *pDM_Appendix, 
								  CStringArray *pArrStrAddFrame, 
								  LPCTSTR lpszShelfDate)
{
	EFS_BEGIN

	if (!pDM_Species ||
		!pDM_Index ||
		!pDM_Book ) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;

//	ids = pDM_Species->StartFrame();
//	if ( ids < 0 ) 
//	{
//		AfxMessageBox(_T("StartFrame 를 확인하세요"));
//		return -1;
//	}

	CString strRegNo;
	CString strQuery;
	CString strResult;
	CString strTemp;
	
	//마지막번호 체크 하기 위해
	CESL_DataMgr *DM_LastNumber = NULL;

	//=======================================================================
	// 1. 등록번호 중복 조사
	//=======================================================================
	DM_LastNumber = FindDM( _T("DM_ExcelToMarc_마지막번호") );
	DM_LastNumber->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	
	CString sLastNumberQuery = _T("") ;
	sLastNumberQuery.Format(_T("SELECT DIVISION_VALUE, LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
	DM_LastNumber->RestructDataManager(sLastNumberQuery);

	nRowCount = pDM_Book->GetRowCount();


	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;
		
		ids = pDM_Book->GetCellData( _T("BB_등록번호"), idx, strRegNo);
		if (ids < 0 || strRegNo.IsEmpty()) continue;

		//등록번호 12자리 만들기
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// 			if ( strRegNo.GetLength() > 2 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , strRegNo.Mid(2) );
// 				strRegNo = strRegNo.Left(2) + sRight;
// 			}
// 		}

		if ( strRegNo.GetLength() > nRegCodeLength && strRegNo.GetLength() < 12 ) 
		{
			strRegNo = m_pInfo->MakeRegNo( strRegNo.Left(nRegCodeLength) , strRegNo.Mid(nRegCodeLength) );
		}
		//=====================================================
		

		//등록번호 넣기
		pDM_Book->SetCellData( _T("BB_등록번호"), strRegNo, idx );

		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO = '%s' AND MANAGE_CODE = '%s'"), pDM_Book->TBL_NAME, strRegNo, m_pInfo->MANAGE_CODE);
		ids = pDM_Species->GetOneValueQuery(strQuery, strResult);

		if (ids >= 0 && !strResult.IsEmpty())
		{
			//등록번호 중복조사
			m_strLastError.Format(_T("등록번호가 중복됩니다. [%s]"), strRegNo);
			return -1;
		}
	}
	
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("BS_MARC"), 0);
	ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return -1;

	//=======================================================================
	// 2. 배가위치 적용
	//=======================================================================
	ids = ((CCatApi_BO*)m_pCatApi)->ApplyShelfCodeData(pDM_Book, &marc, FALSE);
	if (ids < 0) return -1;


	
	//=======================================================================
	// 3. 도서관 부호 가져오기
	//=======================================================================
	CString strLibCode;
	m_pCatApi->GetLibInfo(&CString(_T("LIB_CODE")), &strLibCode, 1);
	if ( strLibCode.IsEmpty() )
	{
		m_strLastError.Format(_T("도서관 부호가 없습니다. 수정해주세요"));
		return -1;
	}

	//=======================================================================
	// 4. 최종수정정보 생성 040
	//=======================================================================
	m_pCatApi->ApplyLastModifyData(&marc);

	// 마크 수정된것을 적용한다.
	ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("BS_MARC"), strStreamMarc, 0);
	ids = pDM_Species->SetCellData( _T("BS_정리된자료"), _T("Y"), 0 );
	
	//=======================================================================
	// 5. LOC_FLAG 넣기
	//=======================================================================
	ids = pDM_Species->SetCellData( _T("BS_작업상태구분"), _T("1"), 0 );

	//=======================================================================
	// 6. DB에 저장한다. 
	//=======================================================================
	ids = InsertSpecies(pDM_Species, pDM_Index, pDM_Book, pDM_Volume, pDM_Appendix, pArrStrAddFrame, FALSE);

	if (ids < 0) return -1;

	//=======================================================================
	// 7. 001 TAG 생성
	//=======================================================================
//	ids = m_pCatApi->CompleteCataloging(pDM_Species, pDM_Species, 0, pDM_Book, FALSE);
//	if (ids < 0) return -1;

	CString strShelfDate;
	
	//=======================================================================
	// 8. 임의의 배가일자를 기록한다.
	//=======================================================================
	if (lpszShelfDate == NULL)
	{
		// 기존 홍수 수정
		// strShelfDate = _T("1200/01/01");
		strShelfDate = CTime::GetCurrentTime().Format(_T("%Y/%m/%d"));
	}
	else
		strShelfDate = lpszShelfDate;		

	CStringArray arrSpeciesKey;
	CStringArray arrBookKey;
	CStringArray arrAppendixKey;

	arrSpeciesKey.Add(pDM_Species->GetCellData( _T("BS_종KEY"), 0));
	
	nRowCount = pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;

		arrBookKey.Add(pDM_Book->GetCellData( _T("BB_책KEY"), idx));
	}
	if(pDM_Appendix)
	{
		nRowCount = pDM_Appendix->GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			arrAppendixKey.Add(pDM_Appendix->GetCellData( _T("BB_책KEY"), idx));
		}
	}
	//===================================================
	//2010.07.02 REM BY PJW : 배가기록에 대해서는 추후 추가 하기로함
////	ids = m_pCatApi->RecordLocDate(pDM_Species, &arrSpeciesKey, &arrBookKey, &arrAppendixKey, strShelfDate, FALSE);
//	ids = RecordLocDate(pDM_Species, &arrSpeciesKey, &arrBookKey, &arrAppendixKey, strShelfDate, FALSE);
//	if (ids < 0) return -1;
	//===================================================	

	//=====================================================
	//2008.12.29 ADD BY PJW : 홈체이지 IDX_BO_TBL에 추가할 쿼리를 SENDFRAME바로 앞으로 뺀다.
	CString strSpeciesKey;
	pDM_Species->GetCellData(_T("BS_종KEY"),0,strSpeciesKey);
 	m_pCatApi->ApplyExtraQuery(pDM_Species, pDM_Index, pDM_Book, strSpeciesKey, pArrStrAddFrame);
	//=====================================================

//	ids = pDM_Species->SendFrame();
//	if (ids < 0) 
//	{
//		m_strLastError = _T("DB에 데이터 저장 실패");
//		return ids;
//	}
//
//	ids = pDM_Species->EndFrame();
	
	return 0;

	EFS_END
	return -1;
}
//===================================================
//2010.06.01 UPDATE BY PJW : UPDATE 쿼리 대신 DM에 추가한다.
//INT CExcelToMarcDlg::RecordLocDate(CESL_DataMgr *pDM_Out, 
//							  CStringArray *pArrSpeciesKey, 
//							  CStringArray *pArrBookKey, 
//							  CStringArray *pArrAppendixKey, 
//							  CString &strShelfDate, 
//							  BOOL bCommit /*= TRUE*/)
//{
//	EFS_BEGIN
//
//	if (!pDM_Out || !pArrSpeciesKey || !pArrBookKey) return -1;
//
//	INT ids = -1;
//	INT idx = -1;
//	INT nRowCount = -1;
//
//	CString strRecKey;
//	CString strQuery;
//
//
//// bCommit은 입력일때 0이다.
//// 배가기록시 1이다.
//
//	if (bCommit)
//	{
//		ids = pDM_Out->StartFrame();
//		if ( ids < 0 ) 
//		{
//			AfxMessageBox(_T("StartFrame 를 확인하세요"));
//			return -1;
//		}
//	}
//
//	nRowCount = pArrSpeciesKey->GetSize();
//	for (idx = 0; idx < nRowCount; idx++)
//	{
//		strRecKey = pArrSpeciesKey->GetAt(idx);
//		if (strRecKey.IsEmpty()) continue;
//
//		// 색인의 SPECIES_CLASS, WORKING_STAUTS를 업데이트 한다.
//		strQuery.Format(_T("UPDATE IDX_BO_TBL SET HOLD_DATE = '%s', SPECIES_CLASS = '1', WORKING_STATUS = 'BOL112N' WHERE REC_KEY = %s;")
//							, strShelfDate, strRecKey);
//		ids = pDM_Out->AddFrame(strQuery);
//		if (ids < 0) return ids;
//
//		//=====================================================
//		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
//// 		strQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
//// 						_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', '%s');"),
//// 						strRecKey, strShelfDate);
//// 		ids = pDM_Out->AddFrame(strQuery);
//// 		if (ids < 0) return ids;
//		//=====================================================
//		
//		
//		// 종의 LOC_FLAG 를 업데이트 한다.
//		strQuery.Format(_T("UPDATE BO_SPECIES_TBL SET LOC_FLAG = '0' WHERE REC_KEY = %s;"), strRecKey);
//		ids = pDM_Out->AddFrame(strQuery);
//		if (ids < 0) return ids;
//		
//	}
//
//	// 저장해뒀던 책의 RecKey를 가진 책의 배가일자를 기록한다.
//	nRowCount = pArrBookKey->GetSize();
//	for (idx = 0; idx < nRowCount; idx++)
//	{
//		strRecKey = pArrBookKey->GetAt(idx);
//		if (strRecKey.IsEmpty()) continue;
//
//		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET SHELF_DATE = '%s', WORKING_STATUS = 'BOL112N', SHELF_WORKER = '%s' WHERE REC_KEY = %s;")
//			, strShelfDate, m_pInfo->USER_ID, strRecKey);
//		ids = pDM_Out->AddFrame(strQuery);
//		if (ids < 0) return ids;
//
//		// 비치희망 정보의 비치상태를 2 : 비치 로 바꾸어주자
//		CString strWhere;
//		CString strAcqCode;
//		CString strAcqKey;
//		strQuery.Format(_T("SELECT ACQ_CODE FROM BO_BOOK_TBL WHERE REC_KEY = %s"), strRecKey);
//		pDM_Out->GetOneValueQuery(strQuery, strAcqCode);
//		if (!strAcqCode.IsEmpty())
//			strWhere.Format(_T("ACQ_CODE = '%s'"), strAcqCode);
//		
//		//else
//		//	strWhere = _T("ACQ_CODE IS NULL");
//
//		strQuery.Format(_T("SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE REC_KEY = %s"), strRecKey);
//		pDM_Out->GetOneValueQuery(strQuery, strAcqKey);
//		if (!strAcqKey.IsEmpty()) {
//			if ( strWhere != _T("") ) {
//				strWhere += _T(" AND ACQ_KEY = ") + strAcqKey;
//			}
//			else {
//				strWhere += _T("ACQ_KEY = ") + strAcqKey;
//			}
//		}
//		//else
//		//	strWhere += _T(" AND ACQ_KEY IS NULL");
//
//		if ( strWhere != _T("") ) {
//			strQuery.Format(_T("UPDATE BO_FURNISH_TBL SET FURNISH_STATUS = '2' WHERE %s;"), strWhere);
//			ids = pDM_Out->AddFrame(strQuery);
//			if (ids < 0) return ids;
//		}
//	}
//
//	// 저장해뒀던 부록의 RecKey를 가진 책의 배가일자를 기록한다.	
//	if (pArrAppendixKey)
//	{
//		nRowCount = pArrAppendixKey->GetSize();
//		for (idx = 0; idx < nRowCount; idx++)
//		{
//			strRecKey = pArrAppendixKey->GetAt(idx);
//			if (strRecKey.IsEmpty()) continue;
//
//			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET SHELF_DATE = '%s', WORKING_STATUS = 'BOL112N', SHELF_WORKER = '%s' WHERE REC_KEY = %s;")
//				,strShelfDate, m_pInfo->USER_ID, strRecKey);
//			ids = pDM_Out->AddFrame(strQuery);
//			if (ids < 0) return ids;
//		}
//	}
//
//	if (bCommit)
//	{
//		ids = pDM_Out->SendFrame();
//		if (ids < 0) 
//		{
////			m_strLastErrMsg = _T("DB에 데이터 저장 실패");
//			return ids;
//		}
//
//		ids = pDM_Out->EndFrame();
//		if (ids < 0) return ids;
//	}
//
//	return 0;
//
//	EFS_END
//	return -1;
//}

INT CExcelToMarcDlg::RecordLocDate(CESL_DataMgr *pDM_Out, 
							  CStringArray *pArrSpeciesKey, 
							  CStringArray *pArrBookKey, 
							  CStringArray *pArrAppendixKey, 
							  CString &strShelfDate, 
							  BOOL bCommit /*= TRUE*/)
{
	EFS_BEGIN

	if (!pDM_Out || !pArrSpeciesKey || !pArrBookKey) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;

	CString strRecKey;
	CString strQuery;


// bCommit은 입력일때 0이다.
// 배가기록시 1이다.

	if (bCommit)
	{
		ids = pDM_Out->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return -1;
		}
	}

	nRowCount = pArrSpeciesKey->GetSize();
	for (idx = 0; idx < nRowCount; idx++)
	{
		strRecKey = pArrSpeciesKey->GetAt(idx);
		if (strRecKey.IsEmpty()) continue;

		// 색인의 SPECIES_CLASS, WORKING_STAUTS를 업데이트 한다.
		strQuery.Format(_T("UPDATE IDX_BO_TBL SET HOLD_DATE = '%s', SPECIES_CLASS = '1', WORKING_STATUS = 'BOL112N' WHERE REC_KEY = %s;")
							, strShelfDate, strRecKey);
		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;

		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		strQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 						_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', '%s');"),
// 						strRecKey, strShelfDate);
// 		ids = pDM_Out->AddFrame(strQuery);
// 		if (ids < 0) return ids;
		//=====================================================
		
		
		// 종의 LOC_FLAG 를 업데이트 한다.
		strQuery.Format(_T("UPDATE BO_SPECIES_TBL SET LOC_FLAG = '0' WHERE REC_KEY = %s;"), strRecKey);
		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;
		
	}

	// 저장해뒀던 책의 RecKey를 가진 책의 배가일자를 기록한다.
	nRowCount = pArrBookKey->GetSize();
	for (idx = 0; idx < nRowCount; idx++)
	{
		strRecKey = pArrBookKey->GetAt(idx);
		if (strRecKey.IsEmpty()) continue;

		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET SHELF_DATE = '%s', WORKING_STATUS = 'BOL112N', SHELF_WORKER = '%s' WHERE REC_KEY = %s;")
			, strShelfDate, m_pInfo->USER_ID, strRecKey);
		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;

		// 비치희망 정보의 비치상태를 2 : 비치 로 바꾸어주자
		CString strWhere;
		CString strAcqCode;
		CString strAcqKey;
		strQuery.Format(_T("SELECT ACQ_CODE FROM BO_BOOK_TBL WHERE REC_KEY = %s"), strRecKey);
		pDM_Out->GetOneValueQuery(strQuery, strAcqCode);
		if (!strAcqCode.IsEmpty())
			strWhere.Format(_T("ACQ_CODE = '%s'"), strAcqCode);
		
		//else
		//	strWhere = _T("ACQ_CODE IS NULL");

		strQuery.Format(_T("SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE REC_KEY = %s"), strRecKey);
		pDM_Out->GetOneValueQuery(strQuery, strAcqKey);
		if (!strAcqKey.IsEmpty()) {
			if ( strWhere != _T("") ) {
				strWhere += _T(" AND ACQ_KEY = ") + strAcqKey;
			}
			else {
				strWhere += _T("ACQ_KEY = ") + strAcqKey;
			}
		}
		//else
		//	strWhere += _T(" AND ACQ_KEY IS NULL");

		if ( strWhere != _T("") ) {
			strQuery.Format(_T("UPDATE BO_FURNISH_TBL SET FURNISH_STATUS = '2' WHERE %s;"), strWhere);
			ids = pDM_Out->AddFrame(strQuery);
			if (ids < 0) return ids;
		}
	}

	// 저장해뒀던 부록의 RecKey를 가진 책의 배가일자를 기록한다.	
	if (pArrAppendixKey)
	{
		nRowCount = pArrAppendixKey->GetSize();
		for (idx = 0; idx < nRowCount; idx++)
		{
			strRecKey = pArrAppendixKey->GetAt(idx);
			if (strRecKey.IsEmpty()) continue;

			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET SHELF_DATE = '%s', WORKING_STATUS = 'BOL112N', SHELF_WORKER = '%s' WHERE REC_KEY = %s;")
				,strShelfDate, m_pInfo->USER_ID, strRecKey);
			ids = pDM_Out->AddFrame(strQuery);
			if (ids < 0) return ids;
		}
	}

	if (bCommit)
	{
		ids = pDM_Out->SendFrame();
		if (ids < 0) 
		{
//			m_strLastErrMsg = _T("DB에 데이터 저장 실패");
			return ids;
		}

		ids = pDM_Out->EndFrame();
		if (ids < 0) return ids;
	}

	return 0;

	EFS_END
	return -1;
}
//===================================================

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 적용될 DM을 구성해준다. 
// [Return Values] 에러면 음수
// [Mini Spec] 
// 1. 색인/종 DM을 복사한다.
// 2. 마크로 부터 책정보를 구성한다. 
/////////////////////////////////////////////////////////////////////////////
INT CExcelToMarcDlg::MakeMarcAdjustDM(	CESL_DataMgr *pDM_From_Species, 
										INT nFromSpeciesRow, 
										CESL_DataMgr *pDM_From_Index, 
										INT nFromIndexRow, 
										CESL_DataMgr *pDM_To_Species, 
										CESL_DataMgr *pDM_To_Index, 
										CESL_DataMgr *pDM_To_Book, 
										CESL_DataMgr *pDM_To_Volume)
{
	EFS_BEGIN

	if (!pDM_From_Species ||
		!pDM_From_Index ||
		!pDM_To_Index ||
		!pDM_To_Species ||
		!pDM_To_Book ||
		!pDM_To_Volume ||
		nFromSpeciesRow < 0 ||
		nFromIndexRow < 0) return -1;

	// 데이터 초기화
	pDM_To_Species->FreeData();
	pDM_To_Index->FreeData();
	pDM_To_Book->FreeData();
	pDM_To_Volume->FreeData();

	pDM_To_Species->InsertRow(-1);
	pDM_To_Index->InsertRow(-1);
	pDM_To_Book->InsertRow(-1);
	pDM_To_Volume->InsertRow(-1);

	INT ids = -1;
	
	//=======================================================================
	// 1. 색인/종 DM을 복사한다.
	//=======================================================================
	ids = CopyDM(pDM_From_Species, nFromSpeciesRow, pDM_To_Species, 0);
	if( ids < 0 ) 
	{
		AfxMessageBox(_T("종DM구성에 실패"));
		return -1;
	}
	
	ids = CopyDM(pDM_From_Index, nFromIndexRow, pDM_To_Index, 0);
	if( ids < 0 ) 
	{
		AfxMessageBox(_T("색인DM구성에 실패"));
		return -1;
	}

	// 마크싱크를 맞추기 전에 049에 해당하는 정보를 책 DM에 넣는다...
	CMarc marc;
	CString strStreamMarc = pDM_To_Species->GetCellData( _T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

	CString strRegNo;
	CString strItemData;
	CArray<CString, CString&> arrItem;
	m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arrItem);
	INT nRowCount = arrItem.GetSize();


	//=======================================================================
	// 2. 마크로 부터 책정보를 구성한다. 
	//=======================================================================
	// 등록번호 자동부여 체크버튼 , 다권반입 체크버튼
	CButton * pChkRegNoAutoGive;
	CButton * pChkMultiBook;
	pChkRegNoAutoGive = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
	pChkMultiBook = (CButton *)GetDlgItem(IDC_ChkMULTIBOOK);

	// 049 Tag이 있으면 다시 만든다.
	if (nRowCount >= 0) 
	{	
		pDM_To_Book->FreeData();
		
		INT nBookCnt = 1;
		// 049 Tag없어도 자동부여이면 1책을 만든다.(소정 수정)
		if ( 0 == nRowCount && pChkRegNoAutoGive->GetCheck() == 1) 
		{
			//nRowCount = 1;
			if( pChkMultiBook->GetCheck() == 1 )
			{
				nBookCnt = _ttoi( pDM_To_Species->GetCellData(_T("UDF_책수"),0));
				if( nBookCnt > 0 )
					nRowCount = nBookCnt;
				else
					nRowCount = 1;
			}
			else
				nRowCount = 1;
		}
	}

	// 049 Tag의 개수만큼 책을 만든다.
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		pDM_To_Book->InsertRow(-1);

		// 등록번호 세팅(자동부여 아니면)
		if ( pChkRegNoAutoGive->GetCheck() == 0 )
		{
			strRegNo = arrItem.GetAt(idx);
			pDM_To_Book->SetCellData(_T("BB_등록번호"), strRegNo, idx);

			//등록구분 세팅
			CString sRegCode;
				//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			if ( strRegNo.GetLength() >= 2 ) {
// 				sRegCode = strRegNo.Left(2);
// 				pDM_To_Book->SetCellData(_T("BB_등록구분_코드"), sRegCode, idx);
// 			}

			if ( strRegNo.GetLength() >= nRegCodeLength ) {
				sRegCode = strRegNo.Left(nRegCodeLength);
				pDM_To_Book->SetCellData(_T("BB_등록구분_코드"), sRegCode, idx);
			}
			//=====================================================

			pDM_To_Book->SetCellData(_T("BB_등록구분_코드"), sRegCode, idx);
			
		}

		// 책부록 플래그
		pDM_To_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), idx);

		// DB FLAG 세팅
		pDM_To_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);

		// 종 -> 책
		ids = CopyDM(pDM_To_Species, 0, pDM_To_Book, idx, _T("BB"));
		if( ids < 0 )
		{
			AfxMessageBox(_T("책DM 구성 에러"));
			return -1;
		}
		// 색인 -> 책
		ids = CopyDM(pDM_To_Index, 0, pDM_To_Book, idx, _T("BB"));
		if( ids < 0 )
		{
			AfxMessageBox(_T("책DM 구성 에러"));
			return -1;
		}
	}

	// 대표책 설정
	INT nRepBookIdx = SetRepresentationBook(pDM_To_Book, _T("BB"));

	// 마크와 싱크맞춤
	m_pCatApi->ConvertMarcToCatDM(&marc, pDM_To_Species, pDM_To_Index, pDM_To_Book, pDM_To_Volume);

	return 0;

	EFS_END
	return -1;

}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 버튼 상태 세팅
// [Return Values] 
// [Mini Spec] 
// Mode : 0 작업진행중 -> 중지
// Mode : 1 작업완료 -> 닫기
/////////////////////////////////////////////////////////////////////////////
VOID CExcelToMarcDlg::ButtonStatus(INT nMode)
{
	EFS_BEGIN

	const INT nIDCount = 3;
	//===================================================
	//2010.06.01 ADD BY PJW : 
//	INT nArrID[nIDCount] = {
//		IDC_bMODIFY,
//		IDC_bIMPORT, IDC_bEXPORT
//	};
		INT nArrID[nIDCount] = {
		IDC_bMODIFY,
		IDC_bInit, IDC_btnSave
	};
	//===================================================

	for (INT i = 0; i < nIDCount; i++)
	{
		if (nMode == 0)
		{
			GetDlgItem(nArrID[i])->EnableWindow(FALSE);
		}
		else if (nMode == 1)
		{
			GetDlgItem(nArrID[i])->EnableWindow(TRUE);
		}
	}

	if (nMode == 0)
	{
		GetDlgItem(IDC_btnClose)->SetWindowText(_T("중지"));
	}
	else if (nMode == 1)
	{
		GetDlgItem(IDC_btnClose)->SetWindowText(_T("닫기"));
	}


	EFS_END
}

CString CExcelToMarcDlg::GetPriceCode(CString strPrice, 
									CString &strRealPrice)
{
	EFS_BEGIN

	CString strResult;

	INT nStartPrice = -1;

	TCHAR ch = 0;
	INT nLength = strPrice.GetLength();
	for (INT idx = 0; idx < nLength; idx++)
	{
		ch = strPrice.GetAt(idx);
		if (_istdigit(ch))
		{
			nStartPrice = idx;
			break;
		}
		
		strResult += ch;
	}

	if (nStartPrice >= 0)
		strRealPrice = strPrice.Mid(nStartPrice);

	return strResult;

	return _T("");

	EFS_END
	return _T("");
}

BOOL CExcelToMarcDlg::IsBook(CESL_DataMgr *pDM_Book, 
					 INT idx, 
					 CString strToAliasHeader /*= _T("")*/, 
					 LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	// 단행만 있음
	if (pDM_Book->GetCellData( strToAliasHeader + _T("_책부록플래그"), idx) == _T("A")) return FALSE;

	if (pDM_Book->GetCellData( strToAliasHeader + _T("_DB_FLAG"), idx) == _T("D")) return FALSE;

	// 편권차가 있으면 체크
	if (lpszVol != NULL)
		if (pDM_Book->GetCellData( strToAliasHeader + _T("_편권차"), idx) != lpszVol) return FALSE;

	return TRUE;
}

BOOL CExcelToMarcDlg::IsStringInArray(CStringArray *pArr, CString str)
{
	EFS_BEGIN

	if (pArr == NULL || str.IsEmpty()) return FALSE;

	INT nRowCount = pArr->GetSize();
	if (nRowCount == 0) return FALSE;

	for (INT i = 0; i < nRowCount; i++)
	{
		if (pArr->GetAt(i) == str) return TRUE;
	}

	return FALSE;

	EFS_END
	return FALSE;
}

void CExcelToMarcDlg::OnbMODIFY() 
{
EFS_BEGIN

	// 마크정리 화면을 띄운다.
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return;
	
	INT nCurSel = pTabCtrl->GetCurSel();

	CESL_Grid *pGrid = NULL;
	CESL_DataMgr *pDm = NULL;
	
	if (nCurSel == 0)
	{
		pGrid = m_pGrid_OK;
		pDm   = m_pDmGridOk;
	}
	else if (nCurSel == 1)
	{
		pGrid = m_pGrid_ERR;
		pDm   = m_pDmGridError;
	}

	if (pGrid == NULL || pDm == NULL)
	{
		return;
	}

	pGrid->SelectMakeList();
	INT nSelectCnt;
	nSelectCnt = pGrid->SelectGetCount();
	if( 1 > nSelectCnt )
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

//	CString strData = pGrid->GetTextMatrix(pGrid->GetRow(), 0) ;
//	if (strData.IsEmpty()) return;

	UpdateData();

	// 마크 편집 화면을 보여준다.
	CMarcEditDlg dlg(this);
	dlg.SetMarcImpExpMgr(&m_marcImpExpMgr);
	INT nFileType = 0;
	INT nChkMultiBook;
	INT nChkGiveAutoRegno;
	INT nChkWorkStatus;

	CButton * pChkGiveAutoRegno = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
	nChkGiveAutoRegno = pChkGiveAutoRegno->GetCheck();

	CButton * pChkMultiBook = (CButton *)GetDlgItem(IDC_ChkMULTIBOOK);
	nChkMultiBook = pChkMultiBook->GetCheck();

	CButton * pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkMULTIBOOK);
	nChkWorkStatus = pChkWorkStatus->GetCheck();

	 

	dlg.SetParentInfo(m_pDM_Species, pDm, pGrid, _T("BS_MARC"), _T("BS_파일반입오류")
		              ,0, nChkMultiBook, nChkGiveAutoRegno, nChkWorkStatus);

	pGrid->SetProtectSelectColumn(TRUE);
	dlg.DoModal();
	pGrid->Display();
	INT nIdx;
	CString strData;
	nIdx = pGrid->SelectGetHeadPosition();
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		strData = pDm->GetCellData(_T("순위"), nIdx);
		pGrid->SetTextMatrix(nIdx+1, 0,strData);
		nIdx = pGrid->SelectGetNext();		
	}
	pGrid->SetProtectSelectColumn(FALSE);

	return;
	EFS_END	
}

void CExcelToMarcDlg::OnbNORMALIZATION() 
{
EFS_BEGIN

	INT nDmColCount		= 0;
	INT nIdx			= 0;
	INT nSelectCnt		= 0;
	INT nOkDmRowCnt		= 0;
	INT nErrDmRowCnt	= 0;
	INT nRecovers		= 0;

	CString strMsg;
	CString strData;

 	BeginWaitCursor();

	nDmColCount = m_pDmGridError->GetRefColumnCount();
	nErrDmRowCnt = m_pDmGridError->GetRowCount();
	m_pGrid_ERR->SelectMakeList();
	nSelectCnt = m_pGrid_ERR->SelectGetCount();
	
	if( 0 < nSelectCnt )
	{
		nIdx = m_pGrid_ERR->SelectGetTailPosition();

		for( INT i = 0 ; i < nSelectCnt ; i++ )
		{
			m_pDmGridError->GetCellData(_T("BS_파일반입오류"), nIdx, strData);

			if( strData.IsEmpty() )
			{
				nOkDmRowCnt = m_pDmGridOk->GetRowCount();
				m_pDmGridOk->InsertRow(-1);
				
				for( INT j = 0 ; j < nDmColCount ; j++ )
				{
					strData = m_pDmGridError->GetCellData( nIdx, j );
					m_pDmGridOk->SetCellData( nOkDmRowCnt, j, strData );					
				}

				m_pDmGridError->DeleteRow( nIdx );

				// 성공 개수 설정
//				strData = m_wndStatusBarCtrl.GetText(1);
//				strData = strData.Mid(strData.ReverseFind(':') + 1);
//				strData.Format(_T("%d"), _ttoi(strData) + 1);
//				m_wndStatusBarCtrl.SetText(_T("정상 : ") + strData, 1, 0);
//				
//				strData = m_wndStatusBarCtrl.GetText(2);
//				strData = strData.Mid(strData.ReverseFind(':') + 1);
//				strData.Format(_T("%d"), _ttoi(strData) - 1);
//				m_wndStatusBarCtrl.SetText(_T("오류 : ") + strData, 2, 0);

				nRecovers++;
			}

			nIdx = m_pGrid_ERR->SelectGetPrev();
		}
	}
	else
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

 	EndWaitCursor();

	if( nRecovers > 0 )
	{
		INT nRowCnt;
		nRowCnt = m_pDmGridOk->GetRowCount();
		strData.Format( _T("%d"), nRowCnt );
		m_wndStatusBarCtrl.SetText(_T("정상 : ") + strData, 1, 0);
				
		nRowCnt = m_pDmGridError->GetRowCount();
		strData.Format( _T("%d"), nRowCnt );
		m_wndStatusBarCtrl.SetText(_T("오류 : ") + strData, 2, 0);

		strMsg.Format(_T("[ %d ]건 중 [ %d ] 건이 복귀했습니다."), nErrDmRowCnt, nRecovers);
		AfxMessageBox(strMsg);
// 		m_pDmGridOk->SORT(_T("순위"));
		SortDm(m_pDmGridOk);
		DisplayGrid();
	}
	else
	{
		strMsg.Format(_T("오류 수정된 자료가 없습니다."));
		AfxMessageBox(strMsg);	
	}

	return ;
	EFS_END	
}


//===================================================
//2009.03.10 ADD BY PJW : 그리드를 display한후 순위를 셋팅한다.
INT CExcelToMarcDlg::DisplayGrid()
{
	m_pGrid_OK->Display();
	m_pGrid_ERR->Display();	
	m_pGrid_SUCCESS->Display();	

	SetGridRank();
	
	return 0 ;
}
//===================================================

//===================================================
//2009.03.17 UPDATE BY PJW : 그리드를 순위에 따라 정렬한다.
INT CExcelToMarcDlg::SortDm( CESL_DataMgr * pSortDm )
{
	BOOL bChkSwap;

	INT nRowCnt;
	INT i;

	CString strData1;
	CString strData2;
	
	nRowCnt = pSortDm->GetRowCount();
	while(1)
	{
		bChkSwap = FALSE;
		for( i = 0 ; i < nRowCnt-1 ; i++ )
		{
			strData1 = pSortDm->GetCellData( _T("순위"), i );		
			strData2 = pSortDm->GetCellData( _T("순위"), i+1 );
			if( _ttoi(strData1) > _ttoi(strData2) )
			{
				pSortDm->SORT_SwapRow( i , i + 1 );
				bChkSwap = TRUE;
			}
		}

		if( bChkSwap == FALSE )
		{
			break;
		}
	}
	
	return 0 ;
}
//===================================================


//===================================================
//2009.03.10 ADD BY PJW : 그리드에 순위를 셋팅한다.
INT CExcelToMarcDlg::SetGridRank()
{
	INT nRowCnt;
	INT i;
	CString strData;

 	nRowCnt = m_pDmGridOk->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ )
	{
		strData = m_pDmGridOk->GetCellData(_T("순위"),i);
		m_pGrid_OK->SetTextMatrix( i+1, 0, strData);
	}

	nRowCnt = m_pDmGridError->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ )
	{
		strData = m_pDmGridError->GetCellData(_T("순위"),i);
		m_pGrid_ERR->SetTextMatrix( i+1, 0, strData);
	}

	nRowCnt = m_pDmGridSuccess->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ )
	{
		strData = m_pDmGridSuccess->GetCellData(_T("순위"),i);
		m_pGrid_SUCCESS->SetTextMatrix( i+1, 0, strData);
	}

	return 0 ;
}

//===================================================


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 마크정리가 끝났을 때 이벤트 메시지
// [Return Values] 에러면 음수
// [Mini Spec] 
// 성공한 자료들은 성공탭으로 자료를 넘겨준다. 
/////////////////////////////////////////////////////////////////////////////
UINT CExcelToMarcDlg::OnEndMarcAdjust(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	// HSS_040524 헤더세팅
	CString strHD_Species;
	strHD_Species = _T("BS");

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return -1;
	
	CESL_Grid *pGrid = NULL;

	INT nID_FROM = -1;
	CString strFROM;
	CString strData;

	INT nSel = pTabCtrl->GetCurSel();
	
	// OK GRID
	if (nSel == 0)
	{
		pGrid = m_pGrid_OK;
		nID_FROM = 1;
		strFROM = _T("정상 : ");
	}
	// ERROR GRID
	else if (nSel == 1)
	{
		pGrid = m_pGrid_ERR;
		nID_FROM = 2;
		strFROM = _T("오류 : ");
	}
	else
		return -1;
	
	INT nRowsCUR = -1;
	INT nRowsOK = -1;
	INT nRowOK = -1;
	
	INT nRowCount = pGrid->GetCount();
	INT nColCount = pGrid->GetCols(0);

	for (INT idx = nRowCount - 1; idx >= 0; idx--)
	{
		// 소정 수정
		if (m_pDM_Species->GetCellData( strHD_Species + _T("_파일반입성공"), _ttoi(pGrid->GetTextMatrix(idx + 1, 0))-1) == _T("Y"))
		{
			// 성공 Grid로 옮긴다.
			nRowsOK = m_pGrid_SUCCESS->GetRows();
			strData = m_pGrid_SUCCESS->GetTextMatrix(nRowsOK - 1, 0);
			if (strData.IsEmpty())
			{
				nRowOK = nRowsOK - 1;
			}
			else
			{
				m_pGrid_SUCCESS->SetRows(nRowsOK + 1);
				nRowOK = nRowsOK;
			}

			for (INT col = 0; col < nColCount; col++)
			{
				m_pGrid_SUCCESS->SetTextMatrix(nRowOK, col, pGrid->GetTextMatrix(idx + 1, col));
			}

			// RemoveItem
			INT nRowsCUR = pGrid->GetRows();
			if (nRowsCUR != 2)
			{
				pGrid->RemoveItem(idx + 1);
			}
			else if (nRowsCUR == 2)
			{
				pGrid->Clear();
				pGrid->SetColumnInfo();
			}

			// 개수를 설정한다.
			strData = m_wndStatusBarCtrl.GetText(nID_FROM);
			strData = strData.Mid(strData.ReverseFind(':') + 1);
			strData.Format(_T("%d"), _ttoi(strData) - 1);
			m_wndStatusBarCtrl.SetText(strFROM + strData, nID_FROM, 0);		

			strData = m_wndStatusBarCtrl.GetText(3);
			strData = strData.Mid(strData.ReverseFind(':') + 1);
			strData.Format(_T("%d"), _ttoi(strData) + 1);
			m_wndStatusBarCtrl.SetText(_T("성공 : ") + strData, 3, 0);
		}
	}

	return 0;

	EFS_END
	return -1;
}


INT CExcelToMarcDlg::MsgSortGrid()
{
	SortGrid();
	DisplayGrid();

	return 0 ;
}

INT CExcelToMarcDlg::MsgDisplayGrid()
{
	DisplayGrid();
	
	return 0 ;
}

//===================================================
//2009.03.17 UPDATE BY PJW : 그리드를 순위에 따라 정렬한다.
INT CExcelToMarcDlg::SortGrid()
{
//	INT nRowCnt;
//	CString strData;
//
// 	nRowCnt = m_pDmGridOk->GetRowCount();

	m_pDmGridOk->SORT(_T("순위"));
	m_pDmGridError->SORT(_T("순위"));
	m_pDmGridSuccess->SORT(_T("순위"));
//	SortDm( m_pDmGridOk );
//	SortDm( m_pDmGridError );
//	SortDm( m_pDmGridSuccess );

	return 0 ;
}
//===================================================

void CExcelToMarcDlg::OnSelchangeTabImport(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return;
	
	INT nCurSel = pTabCtrl->GetCurSel();

	CMSHFlexGrid *pGridNormal = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID_OK);
	CMSHFlexGrid *pGridError  = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID_ERR);
	CMSHFlexGrid *pGridSuccess  = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID_SUCCESS);

	if (pGridNormal == NULL || pGridError == NULL || pGridSuccess == NULL) return;
	
	if (nCurSel == 0)
	{
		pGridNormal->ShowWindow(SW_SHOW);
		pGridError->ShowWindow(SW_HIDE);
		pGridSuccess->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 1)
	{
		pGridError->ShowWindow(SW_SHOW);
		pGridNormal->ShowWindow(SW_HIDE);
		pGridSuccess->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 2)
	{
		pGridError->ShowWindow(SW_HIDE);
		pGridNormal->ShowWindow(SW_HIDE);
		pGridSuccess->ShowWindow(SW_SHOW);
	}
	
	
	*pResult = 0;


	EFS_END
}

void CExcelToMarcDlg::OnbtnCodeApply() 
{
	// TODO: Add your control notification handler code here
	EFS_BEGIN

	m_pGrid_OK->SelectMakeList();

	INT nCount = m_pGrid_OK->SelectGetCount();
	if (nCount < 1) 
	{
		AfxMessageBox(_T("반입할 자료를 선정해 주세요"));
		return;
	}

	//===================================================
	//2009.03.17 REM BY PJW : DM을 이용함으로 필요없어진 코드
//	if (m_pGrid_OK->GetTextMatrix(m_pGrid_OK->SelectGetHeadPosition() + 1, 0).IsEmpty())
//	{   
//		AfxMessageBox(_T("반입할 자료가 없습니다."));
//		return;
//	}
	//===================================================
	
	UpdateData();
	//=======================================================================
	// 1. 코드를 적용한다. 
	//=======================================================================
	CodeApply();

	//=======================================================================
	// 2. 원부 적용
	//=======================================================================
	//=====================================================
	//2009.01.05 UPDATE BY PJW : 체크되어있을경우만 원부를 적용한다.
	//SetAccessionRecNoApply();
	CButton* pBtn = (CButton*) GetDlgItem(IDC_ChkAccessionRec);
	if( pBtn->GetCheck() )
	{
		SetAccessionRecNoApply();
	}
	//=====================================================

	//=======================================================================
	// 3. 반입 쓰레드를 돌린다.
	//=======================================================================
	m_pThreadImport = AfxBeginThread(ThreadFunc_Import, this);

	EFS_END
	
}


INT CExcelToMarcDlg::SetAccessionRecNoApply()
{
	// 0. 원부 적용유무 체크
	//    원부 마지막 번호 체크. 마지막 번호 보다 크면 사용할 수 없다.
	// 1. 선정된 자료중 등록번호가 부여되지 않은 자료가 있는지 확인한다.
	// 2. 부여되지 않으 자료가 있으면 원부적용을 자동 해제 하면 메시지로 안내한다.
	//  => 등록번호가 부여되지 않은 자료가 있어 원부번호를 적용할 수 없습니다.
	// 3. 사용되고 있는 원부있지 알려준다.
	// 4. 원부가 있으면 키만 따오고
	// 5. 원부가 없으면 생성한다.	
	EFS_BEGIN
	CString strAccessionRecNo;
	m_strAccessionRecKey = _T("");

	CString strAcqCode;
	CString strAcqYear;
	CString sREGISTRANT;
	CString sREG_DATE;
	CString sQuery;
	
	GetControlData(_T("CM_ExcelToMarc_구분코드"), _T("IBS_수입구분_코드")	, strAcqCode);
	GetControlData(_T("CM_ExcelToMarc_구분코드"), _T("BS_수입년도")		, strAcqYear);
	GetControlData(_T("CM_ExcelToMarc_구분코드"), _T("BB_원부번호")		, strAccessionRecNo);

	//원부번호 체크		
	sQuery.Format(_T("SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR='%s' AND ACCESSION_REC_NO='%s' AND MANAGE_CODE = '%s'"), strAcqYear, strAccessionRecNo, m_pInfo->MANAGE_CODE );
	CString sResult;	//시작 등록번호
	m_pDM_Species->GetOneValueQuery( sQuery , sResult );
	if ( sResult != _T("") ) {
		m_strAccessionRecKey = sResult;		
		return 0;
	}


	sREGISTRANT = this->GetUserID();

	CTime t = CTime::GetCurrentTime();
	sREG_DATE.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	
	//rec_key 만들기
	CString sRecKey;	
	INT ids = m_pDM_Species->MakeRecKey( sRecKey );
	if ( 0 > ids ) return ids;
	ids = m_pDM_Species->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return ids;
	}

	m_strAccessionRecKey = sRecKey;
	m_pDM_Species->InitDBFieldData();
	m_pDM_Species->AddDBFieldData(_T("REC_KEY")					, _T("STRING"), sRecKey);
	m_pDM_Species->AddDBFieldData(_T("ACCESSION_REC_MAKE_YEAR")	, _T("STRING"), strAcqYear);
	m_pDM_Species->AddDBFieldData(_T("ACCESSION_REC_NO")		, _T("STRING"), strAccessionRecNo);
	m_pDM_Species->AddDBFieldData(_T("PUBLISH_FORM_CODE")		, _T("STRING"), _T("MO"));
	m_pDM_Species->AddDBFieldData(_T("ACQUISIT_CODE")			, _T("STRING"), strAcqCode);
	m_pDM_Species->AddDBFieldData(_T("ACCESSION_REC_MAKER")		, _T("STRING"), sREGISTRANT);
	m_pDM_Species->AddDBFieldData(_T("REG_DATE")				, _T("STRING"), sREG_DATE);
	m_pDM_Species->AddDBFieldData(_T("MANAGE_CODE")				, _T("STRING"), m_pInfo->MANAGE_CODE );

	ids = m_pDM_Species->MakeInsertFrame(_T("CO_ACCESSION_REC_TBL"));
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame 를 확인하세요"));
		return ids;
	}
	
	// 마지막번호 +1 을 그대로 사용하면 UPDATE
	// 마지막 번호+1보다 작을 경우 존재한다고 본다.
	if (m_nAccNum == _ttoi(strAccessionRecNo) )
	{		
		sQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = 'ACC_NO' AND MANAGE_CODE = '%s';") , strAccessionRecNo, strAcqYear, m_pInfo->MANAGE_CODE );
		INT iCh = m_pDM_Species->AddFrame( sQuery );
		if ( 0 > iCh ) return -100;
	}

	ids = m_pDM_Species->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return -100;
	}
	ids = m_pDM_Species->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 를 확인하세요"));
		return -100;
	}	

	return 0;
	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 코드적용
// [Return Values] 
// [Mini Spec] 
// 한개씩 로우별로 코드 적용
/////////////////////////////////////////////////////////////////////////////
VOID CExcelToMarcDlg::CodeApply()
{
	EFS_BEGIN

	BeginWaitCursor();
	
	INT nRowCount = m_pDM_Index->GetRowCount();
	for (INT row = 0; row < nRowCount; row++)
		CodeApply(row);
	
	EndWaitCursor();


	EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 코드적용
// [Return Values] 
// [Mini Spec] 
// 형식구분 매체구분에 알맞게 코드를 적용한다.
/////////////////////////////////////////////////////////////////////////////
VOID CExcelToMarcDlg::CodeApply(INT nIdxDM)
{
	EFS_BEGIN

	CStringArray arrIgnorAlias;
	CString strHD_Species, strHD_Index;
	CMarc marc;
	CString strStreamMarc;
	CString strFormCode;
	CString strMatCode;
	
	// HSS_040524 헤더세팅
	strHD_Species = _T("BS");
	strHD_Index = _T("IBS");

	CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias_Code);
	if (pCM == NULL) return;

	// TODO : 마크 구분별~ 코드 적용
	INT ids = -1;
	
	arrIgnorAlias.Add( strHD_Index + _T("_형식구분_코드"));
	arrIgnorAlias.Add( strHD_Index + _T("_매체구분_코드"));	

	// 형태 구분, 매체구분을 알맞게 넣어준다.
	ids = m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), nIdxDM, strStreamMarc);
	if (ids >= 0 && !strStreamMarc.IsEmpty())
	{
		// 형태 구분, 매체구분을 알맞게 넣어준다.
		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids >= 0)
		{
			ids = GetFormCodeMatCode(&marc, strFormCode, strMatCode);
			// 2007.06.13 ADD BY PDJ
			if ( strFormCode == _T("") ) strFormCode = _T("BK");
			if ( strMatCode == _T("") ) strMatCode = _T("PR");
			
			m_pDM_Index->SetCellData( strHD_Index + _T("_형식구분_코드"), strFormCode, nIdxDM);
			m_pDM_Index->SetCellData( strHD_Index + _T("_매체구분_코드"), strMatCode, nIdxDM);			
		}
	}
	
	m_pCatApi->MakeDMFromCM(pCM, m_pDM_Species, MNG_INSERT, nIdxDM, strHD_Species, &arrIgnorAlias);
	m_pCatApi->MakeDMFromCM(pCM, m_pDM_Index, MNG_INSERT, nIdxDM, strHD_Index, &arrIgnorAlias);

	EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] 마크로부터 형식구분, 매체구분 구함
// [Return Values] 에러면 음수
// [Mini Spec] 
// 1. 리더로부터 형식구분을 구한다.
/////////////////////////////////////////////////////////////////////////////
INT CExcelToMarcDlg::GetFormCodeMatCode(CMarc *pMarc, CString &strFormCode, CString &strMatCode)
{
	EFS_BEGIN

	if (!pMarc) return -1;
	strFormCode.Empty();
	strMatCode.Empty();

	CString strLeader;
	CString strFormCodeMatCode;
	CString strHD_Index;
	strHD_Index = _T("IBS");

	pMarc->GetLeader(strLeader);
	if (strLeader.GetLength() < 7)
	{
		AfxMessageBox(_T("리더의 길이가 7보다 작습니다."));
		return -1;
	}
	
	//=======================================================================
	// 1. 리더로부터 형식구분을 구한다.
	//=======================================================================
	CString strRecord = strLeader.Mid(6,1) + strLeader.Mid(7,1);
	// 2007.06.12 ADD BY PDJ
	// 엑셀파일 반입시 또는 레코드 형태가 NULL일 경우 Default 'a'
	if ( strRecord == _T("  ") ) strRecord = _T("am");

	BOOL bFind = m_mapRecord_FormCodeMatCode.Lookup(strRecord, strFormCodeMatCode);
	if (bFind < 0)
	{
		AfxMessageBox(_T(""));
		return -1;
	}

	// 형식구분, 매체구분 가져오기
	INT nFind = strFormCodeMatCode.Find(_T("="));
	if (nFind < 0) return -1;

	CString strGetFormCode = strFormCodeMatCode.Mid(0, nFind);
	CString strGetMatCode = strFormCodeMatCode.Mid(nFind + 1);

	strFormCode = strGetFormCode;
	
	// 결정할 수 없는 타입일 경우
	nFind = strGetMatCode.Find(_T("|"));
	if (nFind > 0)
	{
		strGetMatCode = strGetMatCode.Mid(0, nFind);

		// 화면의 설정된 값을 가져온다.
		CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias_Code);
		if (pCM == NULL) return -1;

		CString strTemp;
		pCM->GetControlMgrData( strHD_Index + _T("_형식구분_코드"), strTemp);
		if (strTemp == strGetFormCode)
		{
			pCM->GetControlMgrData( strHD_Index + _T("_매체구분_코드"), strTemp);
			strMatCode = strTemp;
			return 0;
		}
	}
	
	// 설정된 값도 아니고 결정할수 있는 타입인 경우
	strMatCode = strGetMatCode;
	
	return 0;

	EFS_END
	return -1;
}

BOOL CExcelToMarcDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd = CWnd::FromHandle(pMsg->hwnd);
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_ESCAPE )
		{
			OnbtnClose();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
