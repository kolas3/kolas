// LibInfoCollectorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LibInfoCollector.h"
#include "LibInfoCollectorDlg.h"
#include "StdioFileEx.h"
#include "charconvert.h"
#include "OCI_API.h"
#include "..\..\단행\열람\장서관리\LOC_CommonAPI\LocCommonAPI.h"


// DES 암복호화
#include "FWDES.h"

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
// CLibInfoCollectorDlg dialog

CLibInfoCollectorDlg::CLibInfoCollectorDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CLibInfoCollectorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strTNSPath		=	_T("..\\ORACLE\\network\\admin\\tnsnames.ora");
	m_strLibFilePath	=	_T("..\\Cfg\\LibList.txt");
	m_strRunTime		=	_T("");
//DEL 	m_pTableInfoDlg		=	NULL;
	m_pGrid				=	NULL;
	m_bIsRunning		=	FALSE;

	m_strLibInfo[0] = _T("도서관명");
	m_strLibInfo[1] = _T("IP");
	m_strLibInfo[2] = _T("PORT");
	m_strLibInfo[3] = _T("K3");
	m_strLibInfo[4] = _T("ID");
	m_strLibInfo[5] = _T("PWD");

	// KOL.UDF.022
	m_pTableInfoDlg = NULL;
	m_pLibListDM = NULL;
	m_asFailedLibName.RemoveAll();
	m_strDMInfo = _T("");
	m_nGridIndex = 0;
	m_nSearchSucc = 0;
	m_bIsLoad = FALSE;
	m_bIsFirstRun = TRUE;
}

CLibInfoCollectorDlg::~CLibInfoCollectorDlg()
{
//DEL 	if( NULL != m_pTableInfoDlg )
//DEL 	{
//DEL 		delete m_pTableInfoDlg;
//DEL 		m_pTableInfoDlg = NULL;
//DEL 	}
	FreeTnsList();
}

void CLibInfoCollectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLibInfoCollectorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLibInfoCollectorDlg, CDialog)
	//{{AFX_MSG_MAP(CLibInfoCollectorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnRunSql, OnbtnRunSql)
	ON_BN_CLICKED(IDC_btnQueryClear, OnbtnQueryClear)
	ON_BN_CLICKED(IDC_btnShowTableInfo, OnbtnShowTableInfo)
	ON_BN_CLICKED(IDC_btnStop, OnbtnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLibInfoCollectorDlg message handlers

BOOL CLibInfoCollectorDlg::OnInitDialog()
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

	if(InitESL_Mgr(_T("SM_LibInfoCollector")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	// 1. Get Library List
	INT ids = GetLibList();

	// 2. Grid 초기화
	m_pGrid = ((CESL_Grid*)GetDlgItem(IDC_gridResult));
	InitGrid();

	// 2011.01.26 ADD BY KSJ : 
	((CButton*)GetDlgItem(IDC_radSelect))->SetCheck(TRUE);

	// CTableInfoDlg 생성
	//DEL CreateTableInfoDlg();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLibInfoCollectorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLibInfoCollectorDlg::OnPaint() 
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
HCURSOR CLibInfoCollectorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CLibInfoCollectorDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(		WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam )
	{
		//DEL OnbtnRunSql();
		return TRUE;
	}
	else if(WM_KEYDOWN == pMsg->message && VK_ESCAPE == pMsg->wParam )
	{
		return TRUE;
	}
	
	//DEL return CESL_Mgr::PreTranslateMessage(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

void CLibInfoCollectorDlg::OnbtnQueryClear() 
{
//DEL 	const INT nControlCnt = 6;
//DEL 	INT nID[nControlCnt] = {IDC_edtQueryColumn, IDC_edtQueryTable, IDC_edtQueryCondition, IDC_edtQuerySort, IDC_edtQueryGroup, IDC_staRunResult};	
//DEL 	for( INT i=0; i<nControlCnt ; i++ )
//DEL 	{
//DEL 		GetDlgItem(nID[i])->SetWindowText( _T("") );
//DEL 	}
	GetDlgItem(IDC_edtQuery)->SetWindowText(_T(""));
}

INT CLibInfoCollectorDlg::Run()
{
	CString strStatus;

	// 1. 초기화
	Init();

	// 2. 검색 Query 구성
	INT		ids;
	CString strQuery;
	ids = MakeQuery(strQuery);
	if( 0 != ids )
	{
		m_bIsRunning = FALSE;
		strStatus.Format(_T("Query 구성에 실패 하였습니다."));
		this->SendMessage(WM_VIEW_STATUS, NULL, (LPARAM)(LPCTSTR)strStatus);
		return -1010;
	}
	if(strQuery.IsEmpty())
	{
		m_bIsRunning = FALSE;
		strStatus.Format(_T("Query를 입력하여 주십시요."));
		this->SendMessage(WM_VIEW_STATUS, NULL, (LPARAM)(LPCTSTR)strStatus);
		return -1020;
	}

	// 3. 도서관 정보 가져오기
	/// 3-1. File 열기
	CString strAlias[LIB_INFO_CNT] = { _T("도서관명"), _T("IP"), _T("PORT"), _T("SID"), _T("ID"), _T("PWD")};
	DWORD	dwStart , dwEnd;
	dwStart = GetTickCount();
	INT i,j;
	INT nRowCnt = m_pLibListDM->GetRowCount();
	for( i=0; i<nRowCnt; i++)
	{
		Sleep(1000);
		if( FALSE == m_bIsRunning)
		{
			m_nSearchSucc = 1;
			break;
		}

		for(j=0; j<LIB_INFO_CNT; j++)
		{
			ids = m_pLibListDM->GetCellData(strAlias[j], i, m_strLibInfo[j]);
		}

		/// 4. Config File 수정
		DisconnectDB();
		ids = SaveConfig();
		if( 0 > ids )
		{
			m_bIsRunning = FALSE;
			return -1;
		}

		if(((CButton*)GetDlgItem(IDC_radSelect))->GetCheck())
		{
			// 5. Query로 DB 조회
			/// Status 변경
			strStatus.Format(_T("%s Select 중 입니다."), m_strLibInfo[0]);
			this->SendMessage(WM_VIEW_STATUS, NULL, (LPARAM)(LPCTSTR)strStatus);

			/// Query 조회
			this->SendMessage(WM_SEARCH_QUERY, NULL, (LPARAM)(LPCTSTR)strQuery);
			m_bIsFirstRun = FALSE;		
		}
		else
		{
			// 5. Query로 DB 조회
			/// Status 변경
			strStatus.Format(_T("%s Execute 중 입니다."), m_strLibInfo[0]);
			this->SendMessage(WM_VIEW_STATUS, NULL, (LPARAM)(LPCTSTR)strStatus);

			/// Query 조회
			this->SendMessage(WM_EXECUTE_QUERY, NULL, (LPARAM)(LPCTSTR)strQuery);
			m_bIsFirstRun = FALSE;		
		}
	}

	dwEnd = GetTickCount();

	if( 0 == m_nSearchSucc )
	{
		strStatus.Format(_T("조회가 완료 되었습니다 (소요시간 : %d.%d)") , (dwEnd-dwStart)/1000 , (dwEnd-dwStart)%1000 );
	}
	else if( 1  == m_nSearchSucc)
	{
		strStatus.Format(_T("조회를 중지 하였습니다 : %s") , m_strLibInfo[0] );
	}
	else if( -1 == m_nSearchSucc )
	{
		strStatus.Format(_T("조회에 실패 하였습니다 : %s") , m_strLibInfo[0] );
	}
	else if( -2 == m_nSearchSucc )
	{
		strStatus.Format(_T("Grid 출력 실패 하였습니다"));
	}
	else if( -3 == m_nSearchSucc )
	{
		strStatus.Format(_T("TNSNAMES.ORA File Load 실패 하였습니다"));
	}
	else if( -4 == m_nSearchSucc )
	{
		strStatus.Format(_T("TNSNAMES.ORA File Save 실패 하였습니다"));
	}
	else if( -5 == m_nSearchSucc )
	{
		strStatus.Format(_T("UseDMInfoChange.Cfg File Save 실패 하였습니다"));
	}

	this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strStatus);
	this->SendMessage(WM_VIEW_FAILED_LIB,NULL,NULL);

	// 7. TNS 되돌리기
	m_strLibInfo[1]	= m_BackupTNS.m_strHOST;
	m_strLibInfo[2]	= m_BackupTNS.m_strPORT;
	m_strLibInfo[3]	= m_BackupTNS.m_strSERVICE_NAME;
	ids = SaveTNS();
	if( 0 > ids )
	{
		m_bIsRunning = FALSE;
		return -1;
	}

	m_bIsRunning = FALSE;

	return 0;
}

/*
@Create
 - 2009.12.04 BY KSJ
@Description
 - 
@Parameter
 - 
@Return Value
 - 
@Mini Spec
 1. 초기화
 2. 검색 Query 구성
 3. 도서관 정보 가져오기
 4. Config File 수정
 5. Query로 DB 조회
 6. 결과 출력
*/
void CLibInfoCollectorDlg::OnbtnRunSql() 
{
	// Run();
	// 스레드 시작
	CWinThread * Thread;
	Thread = AfxBeginThread( SearchThread , this );
}

/*
@Create
 - 2009.12.04 By KSJ
@Description
 - 
@Parameter
 - 
@Return Value
 - 
@Mini Spec
 1. 입력 된 Data 가져오기
 2. Field 조건 구성
 3. Table 조건 구성
 4. Where 조건 구성
 5. Order By 조건 구성
 6. Group By 조건 구성
*/
INT CLibInfoCollectorDlg::MakeQuery(CString &strQuery)
{
	// 2011.01.26 ADD BY KSJ : 
//DEL 	CString strField, strTable, strCondition, strSort, strGroup, strTempQuery, strTemp;
//DEL 	// 1. 입력 된 Data 가져오기
//DEL 	((CEdit*)GetDlgItem(IDC_edtQueryColumn))->GetWindowText( strField );
//DEL 	((CEdit*)GetDlgItem(IDC_edtQueryTable))->GetWindowText( strTable );
//DEL 	((CEdit*)GetDlgItem(IDC_edtQueryCondition))->GetWindowText( strCondition );
//DEL 	((CEdit*)GetDlgItem(IDC_edtQuerySort))->GetWindowText( strSort );
//DEL 	((CEdit*)GetDlgItem(IDC_edtQueryGroup))->GetWindowText( strGroup );
//DEL 
//DEL 	/// 2. Field 조건 구성
//DEL 	strTemp = strField;
//DEL 	strTemp.TrimLeft(); strTemp.TrimRight(); strTemp.Replace( _T(" "), _T("") );
//DEL 	if( TRUE != strTemp.IsEmpty() )
//DEL 	{
//DEL 		strTempQuery.Format( _T("SELECT %s") , strField );
//DEL 		strQuery = strTempQuery;
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		SetStatus(_T("조회 할 Field가 입력되지 않았습니다."));
//DEL 		return 1010;
//DEL 	}
//DEL 
//DEL 	/// 3. Table 조건 구성
//DEL 	strTemp = strTable;
//DEL 	strTemp.TrimLeft(); strTemp.TrimRight(); strTemp.Replace( _T(" "), _T("") );
//DEL 	if( TRUE != strTemp.IsEmpty() )
//DEL 	{
//DEL 		strTempQuery.Format( _T("%s FROM %s") , strQuery , strTable );
//DEL 		strQuery = strTempQuery;
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		SetStatus(_T("조회 할 Table이 입력되지 않았습니다."));
//DEL 		return 1020;
//DEL 	}
//DEL 
//DEL 	/// 4. Where 조건 구성
//DEL 	strTemp = strCondition;
//DEL 	strTemp.TrimLeft(); strTemp.TrimRight(); strTemp.Replace( _T(" "), _T("") );
//DEL 	if( TRUE != strTemp.IsEmpty() )
//DEL 	{
//DEL 		strTempQuery.Format( _T("%s WHERE %s") , strQuery , strCondition );
//DEL 		strQuery = strTempQuery;
//DEL 	}
//DEL 
//DEL 	/// 5. Order By 조건 구성
//DEL 	strTemp = strSort;
//DEL 	strTemp.TrimLeft(); strTemp.TrimRight(); strTemp.Replace( _T(" "), _T("") );
//DEL 	if( TRUE != strTemp.IsEmpty() )
//DEL 	{
//DEL 		strTempQuery.Format( _T("%s ORDER BY %s") , strQuery , strSort );
//DEL 		strQuery = strTempQuery;
//DEL 	}
//DEL 
//DEL 	/// 6. Group By 조건 구성
//DEL 	strTemp = strGroup;
//DEL 	strTemp.TrimLeft(); strTemp.TrimRight(); strTemp.Replace( _T(" "), _T("") );
//DEL 	if( TRUE != strTemp.IsEmpty() )
//DEL 	{
//DEL 		strTempQuery.Format( _T("%s GROUP BY %s") , strQuery , strSort );
//DEL 		strQuery = strTempQuery;
//DEL 	}
//DEL 
//DEL 	strQuery.Replace( _T(";"), _T("") );
	if(((CButton*)GetDlgItem(IDC_radSelect))->GetCheck())
	{
		((CEdit*)GetDlgItem(IDC_edtQuery))->GetWindowText(strQuery);
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_edtQuery))->GetWindowText(strQuery);
	}
	

	return 0;
}

/*
@Create
 - 2009.12.04 By KSJ
@Description
 - 
@Parameter
 - 
@Return Value
 - 
@Mini Spec
 1. DB 접속 Test
 2. DB 조회
 3. 검색결과 출력
*/
INT CLibInfoCollectorDlg::SearchQuery(CString strQuery)
{
EFS_BEGIN

	DWORD	dwStart , dwEnd;
	dwStart = GetTickCount();

	INT		ids;
	CString strStatus , strTemp;
	
	// 1. DB 접속 Test
	strTemp.Format( _T("%s@K3") , m_strLibInfo[4] );
	
	INT nRequestID;
	nRequestID = GetSessionRequestID();
	if (ConnectDB(nRequestID, strTemp.GetBuffer(0), m_strLibInfo[5].GetBuffer(0)) < 0)
	{/// DB 접속 실패
		dwEnd = GetTickCount();
		strTemp.Format( _T("|%d.%d초") , (dwEnd-dwStart)/1000 , (dwEnd-dwStart)%1000 );
		m_strLibInfo[0] += strTemp;
		m_asFailedLibName.Add(m_strLibInfo[0]);
		return 1010;
	}
	else
	{/// DB 접속 성공
		DisconnectDB();

		m_DM.FreeData();
		m_DM.SetCONNECTION_INFO(_T(""));
		strQuery.TrimLeft(); strQuery.TrimRight();
		ids = m_DM.RestructDataManager( strQuery );
		if(0 > ids)
		{
			//SetStatus(_T(""));
			this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strStatus);
			m_nSearchSucc = -1;
			return -1010;
		}

		if(TRUE == m_bIsFirstRun)
		{
			CString strField;
			((CEdit*)GetDlgItem(IDC_edtQuery))->GetWindowText( strField );
			INT nColCnt = m_DM.GetColCount();
			SetGridColName(strField);
		}
	}

	dwEnd = GetTickCount();
	m_strRunTime.Format( _T("%d.%d초") , (dwEnd-dwStart)/1000 , (dwEnd-dwStart)%1000 );
	
	// 2. 검색결과 출력
	/// 2-1. 검색결과 확인
	INT		nDMRowCnt , nDMColCnt;
	nDMRowCnt = m_DM.GetRowCount();
	nDMColCnt = m_DM.GetColCount();
	if( 0 >= nDMRowCnt || 0 >= nDMColCnt )
	{//// 검색 결과가 없을 경우 
		return 1010;
	}

	/// 2-2. 검색결과 Grid 출력
	this->SendMessage(WM_VIEW_RESULT_SEARCH,NULL,NULL);

	return 0;

EFS_END
return -1;
}

INT CLibInfoCollectorDlg::ExecuteQuery(CString strQuery)
{
EFS_BEGIN

	DWORD	dwStart , dwEnd;
	dwStart = GetTickCount();

	INT		ids;
	CString strStatus, strTemp, strResult;
	
	// 1. DB 접속 Test
	strTemp.Format( _T("%s@K3") , m_strLibInfo[4] );
	
	INT nRequestID;
	nRequestID = GetSessionRequestID();
	if (ConnectDB(nRequestID, strTemp.GetBuffer(0), m_strLibInfo[5].GetBuffer(0)) < 0)
	{/// DB 접속 실패
		strResult.Format(_T("DB 접속실패"));
		this->SendMessage(WM_VIEW_RESULT_EXECUTE,NULL,(LPARAM)(LPCTSTR)strResult);
		return 1010;
	}
	else
	{/// DB 접속 성공
		DisconnectDB();

		m_DM.FreeData();
		m_DM.SetCONNECTION_INFO(_T(""));
		strQuery.TrimLeft(); strQuery.TrimRight();
		ids = m_DM.ExecuteQuery( strQuery,TRUE );
	}

	dwEnd = GetTickCount();
	m_strRunTime.Format( _T("%d.%d초") , (dwEnd-dwStart)/1000 , (dwEnd-dwStart)%1000 );
	
	// 2. 검색결과 출력
	if(0==ids)			strResult.Format(_T("성공"));
	else if(-4004==ids) strResult.Format(_T("접속되지않음"));
	else if(-4005==ids) strResult.Format(_T("쿼리실행시 에러발생"));
	else if(-4999==ids) strResult.Format(_T("아직 Oracle를 지원하지 않음"));
	else				strResult.Format(_T("실패 Error Code : %d"), ids);

	this->SendMessage(WM_VIEW_RESULT_EXECUTE,NULL,(LPARAM)(LPCTSTR)strResult);
	return 0;

EFS_END
return -1;
}


/*
@Create
 - 2009.12.04 By KSJ
@Description
 - 
@Parameter
 - 
@Return Value
 - 
@Mini Spec
 1. 검색결과 확인
 2. Grid에 출력
*/
INT CLibInfoCollectorDlg::ViewResultSearch()
{
	INT		ids , nDMRowCnt , nDMColCnt , nGridRowCnt , nGridColCnt , i , j;
	CString strData , strTemp;

	// 1. 검색결과 확인
	nDMRowCnt = m_DM.GetRowCount();
	nDMColCnt = m_DM.GetColCount();
	if( 0 == nDMRowCnt || 0 == nDMColCnt )
	{
		return 1010;
	}

	// 2. Grid 출력
	nGridRowCnt = m_pGrid->GetRows();
	nGridColCnt = m_pGrid->GetColCount();
	if( TRUE == m_bIsFirstRun)
	{
		nGridRowCnt = 1;
	}

	m_pGrid->SetRows(nGridRowCnt+nDMRowCnt);
	for( i=0 ; i<nDMRowCnt ; i++ )
	{
		strTemp.Format( _T("%d") , ++m_nGridIndex );
		//// Index
		m_pGrid->SetTextMatrix( nGridRowCnt+i , 0 , strTemp );
		//// 도서관이름
		m_pGrid->SetTextMatrix( nGridRowCnt+i , 1 , m_strLibInfo[0] );
		//// 소요시간
		m_pGrid->SetTextMatrix( nGridRowCnt+i , 2 , m_strRunTime );
		for( j=0 ; j<nDMColCnt ; j++ )
		{//// 결과 Grid 출력
			ids = m_DM.GetCellData( i , j , strData );
			if( 0 > ids )
			{
				m_nSearchSucc = -2;
				return -1010;
			}

			m_pGrid->SetTextMatrix( nGridRowCnt+i , j+3 , strData );
		}
	}
	m_pGrid->SendMessage(WM_VSCROLL, SB_BOTTOM, 0 );

	UpdateWindow();

	return 0;
}

INT CLibInfoCollectorDlg::ViewResultExecute(CString strResult)
{
	INT		nDMRowCnt , nGridRowCnt , i;
	CString strData , strTemp;

	// 1. Grid 출력
	nGridRowCnt = m_pGrid->GetRows();
	if( TRUE == m_bIsFirstRun)
	{
		INT nGridColCnt = m_pGrid->GetColCount();
		m_pGrid->SetCols(0,nGridColCnt+2);
		m_pGrid->SetTextMatrix( 0 , 3 , _T("결과") );
		nGridRowCnt = 1;
	}

	m_pGrid->SetRows(nGridRowCnt+1);
	strTemp.Format( _T("%d") , ++m_nGridIndex );
	//// Index
	m_pGrid->SetTextMatrix( nGridRowCnt , 0 , strTemp );
	//// 도서관이름
	m_pGrid->SetTextMatrix( nGridRowCnt , 1 , m_strLibInfo[0] );
	//// 소요시간
	m_pGrid->SetTextMatrix( nGridRowCnt , 2 , m_strRunTime );
	//// 수행결과
	m_pGrid->SetTextMatrix( nGridRowCnt , 3 , strResult );
	m_pGrid->SendMessage(WM_VSCROLL, SB_BOTTOM, 0 );

	UpdateWindow();

	return 0;
}


void CLibInfoCollectorDlg::OnbtnShowTableInfo() 
{
	m_pTableInfoDlg->UpdateWindow();
	m_pTableInfoDlg->ShowWindow(SW_SHOW);
}

INT CLibInfoCollectorDlg::CreateTableInfoDlg()
{
	if( NULL != m_pTableInfoDlg )
	{
		m_pTableInfoDlg->SetFocus();
	}
	else
	{
		m_pTableInfoDlg	=	new	CTableInfoDlg(this);
		if( NULL	==		m_pTableInfoDlg )
		{
			delete		m_pTableInfoDlg;
			m_pTableInfoDlg	=	NULL;
			return 0;
		}

		m_pTableInfoDlg->Create(IDD_TABLE_INFO_DLG,this);
	}

	return 0;
}

/*
@Create
 - 
@Description
 - 
@Parameter
 - 
@Return Value
 - 
@Mini Spec
 1. 
*/
INT CLibInfoCollectorDlg::SaveConfig()
{
	INT ids;

	// 1. TNSNAMES.ORA File 설정
	ids = LoadTNS();
	if( 0 > ids )
	{
		m_nSearchSucc = -3;
		return -1010;
	}

	ids = SaveTNS();
	if( 0 > ids )
	{
		m_nSearchSucc = -4;
		return -1020;
	}

	// 2. UseDMInfoChange.Cfg File 설정
	ids = SaveUseDMInfo();
	if( 0 > ids )
	{
		m_nSearchSucc = -5;
		return -1030;
	}

	return 0;
}

INT CLibInfoCollectorDlg::MoveRow(CESL_DataMgr* pSrcDM, INT nSrcRow, CESL_DataMgr* pDstDM, INT nDstRow)
{
	if(NULL == pSrcDM || NULL == pDstDM)
	{
		return -1000;
	}
	if(0 > nSrcRow || pSrcDM->GetRowCount() <= nSrcRow)
	{
		return -1010;
	}
	if(pDstDM->GetRowCount() <= nDstRow)
	{
		return -1020;
	}

	DWORD* row = (DWORD*)pSrcDM->data.GetAt(pSrcDM->m_pDataPosition[nSrcRow]);
	if(0 > nDstRow)
	{
		pDstDM->data.AddTail((CObject*)row);
	}
	else
	{
		POSITION pos = pDstDM->data.FindIndex(nDstRow);
		pDstDM->data.InsertBefore(pos, (CObject*)row);
	}

	pSrcDM->data.RemoveAt(pSrcDM->m_pDataPosition[nSrcRow]);
	pSrcDM->RefreshPositionInfo();
	pDstDM->RefreshPositionInfo();

	return 0;
}

INT CLibInfoCollectorDlg::SaveUseDMInfo()
{
	CString strMode = _T("");

	CString strAlias = _T("");
	CString strLine = _T("");
	CString strModeLine = _T("");
	CString strBeforeLine = _T("");
	CString strAfterLine = _T("");
	
	CString strDMInfo = m_strDMInfo;
	if(FALSE == m_bIsLoad ||
		-1 == m_strDMInfo.Find(_T("MODE")) ||
		-1 == m_strDMInfo.Find(_T("BEFORE")) ||
		-1 == m_strDMInfo.Find(_T("AFTER"))
		)
	{
		strDMInfo = 
			_T("MODE = TRUE\r\n")
			_T("BEFORE = kolas2@k2up/kolas2#[]\r\n")
			_T("AFTER = kolas@kolas3/eco_kolas3#[]\r\n");
	}

	for(int nCnt=0; nCnt < 3; nCnt++)
	{
		AfxExtractSubString(strLine, strDMInfo, nCnt, '\n');
		AfxExtractSubString(strAlias, strLine, 0, '=');
		
		strAlias.TrimLeft();
		strAlias.TrimRight();
		if(_T("MODE") == strAlias)
		{
			AfxExtractSubString(strMode, strLine, 1, '=');
			strMode.TrimLeft();
			strMode.TrimRight();

			strModeLine = strLine;
		}
		else if(_T("BEFORE") == strAlias)
		{
			strBeforeLine = strLine;
		}
		else if(_T("AFTER") == strAlias)
		{
			strAfterLine = strLine;
		}			
	}

	CString info = _T("");
	info.Format( _T("%s@%s/%s[]\r\n"), m_strLibInfo[4],	_T("K3"), m_strLibInfo[5] );
	if(_T("TRUE") == strMode)
	{
		info = _T("AFTER = ") + info;
		strDMInfo.Replace(strAfterLine, info);
	}
	else if(_T("FALSE") == strMode)
	{
		info = _T("BEFORE = ") + info;
		strDMInfo.Replace(strBeforeLine, info);
	}
		
	strDMInfo.TrimLeft();
	strDMInfo.TrimRight();

	// UseDMInfoChange.cfg 파일을 암호화하여 저장
	// DES클래스의 Simple_Cipher 함수에서 결과값 리턴시 힙 오류가 발생하여
	// 이 곳에 해당 함수내용을 풀어서 작성
	CString strFile = _T("..\\cfg\\UseDMInfoChange.cfg");
	CString strKey = _T("k^Ty");

	CString strSrc = strDMInfo;
	CString strDst = _T("");
	/////////////////////////////////////////////////////////////////////////////////
	// Simple_Cipher
	UCHAR* szSrc = NULL;
	UINT nSrcLen = 0;

	UCHAR* szDst = NULL;
	UINT nDstLen = 0;

	UCHAR* szKey = NULL;

	{
 		// 평문을 8바이트의 배수로 맞춰준다.
 		while(0 != strSrc.GetLength()% (8/sizeof(TCHAR)) )
 		{
 			strSrc += _T(" ");
 		}
	}
	{
		// 소스 변환
		UINT LEN = strSrc.GetLength();

		TCHAR* sz_Tmp = new TCHAR[LEN+1];
		ZeroMemory(sz_Tmp, LEN);
		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strSrc.GetBuffer(0));
		strSrc.ReleaseBuffer();
		sz_Tmp[LEN] = '\0';

		szSrc = new UCHAR[LEN*sizeof(TCHAR)+1];
		ZeroMemory(szSrc, LEN*sizeof(TCHAR));
		szSrc[LEN*sizeof(TCHAR)] = '\0';

		// TCHAR를 바이트단위로 나누어 UCHAR에 설정한다.
		for(UINT uCnt = 0; uCnt < LEN; uCnt++)
		{
			TCHAR Tmp = sz_Tmp[uCnt];
			szSrc[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szSrc[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}		
		nSrcLen = (LEN) * sizeof(TCHAR);

		delete []sz_Tmp;
	}
	{
		// 결과물 메모리 할당
		UINT LEN = strSrc.GetLength();
		szDst = new UCHAR[LEN*sizeof(TCHAR)+1];
		ZeroMemory(szDst, LEN*sizeof(TCHAR));
		szDst[LEN*sizeof(TCHAR)] = '\0';		
	}	
	{
		// 키 메모리 할당
		const INT KEY_LENGTH = 8/sizeof(TCHAR); // 키는 8바이트
		while(KEY_LENGTH > strKey.GetLength())
		{
			strKey += _T(" ");
		}
		strKey = strKey.Left(KEY_LENGTH);

		TCHAR* sz_Tmp = new TCHAR[KEY_LENGTH+1];
		ZeroMemory(sz_Tmp, KEY_LENGTH);
		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strKey.GetBuffer(0));
		strKey.ReleaseBuffer();
		sz_Tmp[KEY_LENGTH] = '\0';

		szKey = new UCHAR[KEY_LENGTH*sizeof(TCHAR)+1];
		ZeroMemory(szKey, KEY_LENGTH*sizeof(TCHAR));
		szKey[KEY_LENGTH] = '\0';
		for(int uCnt = 0; uCnt < KEY_LENGTH; uCnt++)
		{
			TCHAR Tmp = sz_Tmp[uCnt];
			szKey[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szKey[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}

		delete []sz_Tmp;
	}
	
	// 2. 암호화
	CFWDES DES;
	DES.Cipher(szKey, nSrcLen, szSrc, &nDstLen, szDst);
	
	// 3. CString 변환
	{
		UCHAR* sz_Tmp = new UCHAR[nDstLen+2];
		ZeroMemory(sz_Tmp, nDstLen+1);
		sz_Tmp[nDstLen+1] = '\0';

		
		for(UINT nCnt = 0; nCnt < nDstLen; nCnt++)
		{			
			sz_Tmp[nCnt] = szDst[nCnt];			
		}		
	 	strDst.Format(_T("%s"), (TCHAR*)sz_Tmp);
		delete []sz_Tmp;
	}

	// 4. 메모리 해제
	delete []szKey;
	delete []szSrc;
	delete []szDst;

	UINT nOpenFlags = CFile::modeWrite|CFile::modeCreate|CFile::typeBinary;
	CFile file;
	CFileException ex;
 	BOOL bIsOpen = file.Open(strFile, nOpenFlags, &ex);
	if(FALSE == bIsOpen)
	{
		CString strErrorCode = ex.m_cause;
		this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strErrorCode);
		return -1001;
	}

	file.SeekToEnd();
	file.Write((TCHAR*)strDst.GetBuffer(0), (strDst.GetLength())*sizeof(TCHAR));
	strDst.ReleaseBuffer();
	file.Close();

	return 0;
}

INT CLibInfoCollectorDlg::SaveTNS()
{
	INT ids;
	CString	strTemp;
	ids = LoadTNSFilePath();

	CStdioFile file;
	CFileException fileException;
	if (!file.Open(m_strTNSPath, CFile::modeWrite | CFile::modeCreate , &fileException ))
	{
		CString strErrorCode = fileException.m_cause;
		//SetStatus(strErrorCode);
		this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strErrorCode);
		return -1010;
	}
	
	CTns *pTns = NULL;

 	CString strKey;
	
	INT			nConvertLen;
	CHAR**		szpTns = NULL;
	CharConvert cvt;
	
	POSITION pos = m_mapTns.GetStartPosition();	
	while (pos)
	{
		m_mapTns.GetNextAssoc(pos, strKey, (VOID*&)pTns);
		if( NULL == pTns )
		{
			continue;
		}
		
		if( _T("K3") == strKey )
		{
			pTns->m_strHOST = m_strLibInfo[1];
			pTns->m_strPORT = m_strLibInfo[2];
			pTns->m_strSERVICE_NAME = m_strLibInfo[3];
			//DEL pTns->m_strSERVICE_NAME.Format( _T("SERVICE_NAME = %s") , m_strDBSID );
		}

		strTemp.Format(_T("%s = \n")
					   _T("  (DESCRIPTION = \n")
					   _T("    (ADDRESS_LIST = \n")
					   _T("      (ADDRESS = (PROTOCOL = TCP)(HOST = %s)(PORT = %s)) \n")
					   _T("    ) \n")
					   _T("    (CONNECT_DATA = \n")
					   _T("      (SERVICE_NAME = %s)\n")
//DEL 					   _T("      (%s)\n")
					   _T("    )\n")
					   _T("  )\n")
					   , strKey, pTns->m_strHOST, pTns->m_strPORT, pTns->m_strSERVICE_NAME);

		szpTns = (CHAR**) malloc (sizeof(CHAR*));
		
		cvt.UnicodeToMultiByte(strTemp.GetBuffer(0), szpTns, &nConvertLen );
		strTemp.ReleaseBuffer();
		file.Write(*szpTns, nConvertLen);
		free(szpTns);		
	}

	file.Close();

	return 0;
}

INT CLibInfoCollectorDlg::LoadTNS()
{
	INT ids;
	FreeTnsList();

	ids = LoadTNSFilePath();
	if( 0 > ids)
	{
		return -1010;
	}

	CStdioFile file;
	CFileException	Exception;
	// 오라클
	if (!file.Open(m_strTNSPath, CFile::modeRead))
	{
		if (!file.Open(m_strTNSPath, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary , &Exception))
		{
			CString strErrorCode = Exception.m_cause;
			this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strErrorCode);
			return -1020;
		}	
	}	

	DWORD dwLen = file.GetLength()+1;
	if (dwLen == 0)
	{
		file.Close();
		return 0;
	}

	CHAR *pszBuffer = new CHAR[dwLen];	
	ZeroMemory(pszBuffer, dwLen);
	TCHAR **szUniBuffer= NULL;		
	INT			nConvertLen;

	file.ReadHuge(pszBuffer, dwLen);
	file.Close();
	CharConvert cvt;
	

	szUniBuffer = (TCHAR**) malloc (sizeof(TCHAR*));
	cvt.MultiByteToUnicode(pszBuffer, szUniBuffer, &nConvertLen);		
	if ( pszBuffer )		
	{
		// KOL.UDF.022
		//delete pszBuffer;
		delete[] pszBuffer;
		pszBuffer = NULL;
	}

	CString strFileData;
	strFileData.Format(_T("%s"), *szUniBuffer);
	// KOL.UDF.022
	//if (szUniBuffer) 
	if (&szUniBuffer != NULL) 
	{
		free(szUniBuffer);
		szUniBuffer = NULL;
	}
	
	// # 제거
	INT nFind = strFileData.Find(_T("#"));
	while (nFind >= 0)
	{
		strFileData.Delete(nFind, strFileData.Find(_T("\n"), nFind) - nFind + 1);
		nFind = strFileData.Find(_T("#"));
	}

	TCHAR ch;
	INT n = strFileData.Find(_T("("));
	INT nCount = 1;
	INT nStart = 0;

	INT nFileLen = strFileData.GetLength();

	while (TRUE)
	{
		if (nFileLen - 1 < ++n) break;
		ch = strFileData.GetAt(n);
		if (ch == '(')
			nCount++;
		else if (ch == ')')
			nCount--;

		if (nCount == 0)
		{
			CString strOneData = strFileData.Mid(nStart, n - nStart + 1);
			strOneData.TrimLeft	();
			strOneData.TrimRight();
			if (!strOneData.IsEmpty())
			{
				CTns *pTns = new CTns;
				// KEY VALUE
				CString strKeyValue = strOneData.Mid(0, strOneData.Find(_T("=")));
				strKeyValue.TrimLeft();	strKeyValue.TrimRight();

				// HOST
				GetTnsValue(strOneData, _T("HOST"), pTns->m_strHOST);
				// PORT
				GetTnsValue(strOneData, _T("PORT"), pTns->m_strPORT);
				// SERVICE_NAME
				GetTnsValue(strOneData, _T("SERVICE_NAME"), pTns->m_strSERVICE_NAME);
				m_mapTns.SetAt(strKeyValue, (VOID*)pTns);

				if( _T("K3") == strKeyValue && TRUE == m_bIsFirstRun )
				{
					m_BackupTNS.m_strHOST = pTns->m_strHOST;
					m_BackupTNS.m_strPORT = pTns->m_strPORT;
					m_BackupTNS.m_strSERVICE_NAME = pTns->m_strSERVICE_NAME;
				}
			}

			nStart = ++n;
			n = strFileData.Find(_T("("), n);
			nCount++;
			if (n < 0) break;
		}
	}

	return 0;
}

INT CLibInfoCollectorDlg::GetTnsValue(CString strTns, CString strKey, CString &strValue)
{
	if (strTns.IsEmpty() || strKey.IsEmpty()) return -1;

	INT nFind = strTns.Find(strKey);
	if (nFind > 0)
	{
		INT nStart = strTns.Find(_T("="), nFind);
		INT nEnd = strTns.Find(_T(")"), nStart);
//DEL 		if( _T("SERVICE_NAME") == strKey )
//DEL 		{
//DEL 			nStart = nFind-1;
//DEL 		}
		
		if (nStart >= 0 && nEnd >= 0)
		{
			strValue = strTns.Mid(nStart + 1, nEnd - nStart - 1);
			strValue.TrimLeft();
			strValue.TrimRight();
		}
	}

	return 0;
}

INT CLibInfoCollectorDlg::LoadTNSFilePath()
{
	CString strErrorCode , strTemp , strDefaultHome;

	TCHAR szBuffer[1024];
	DWORD dwBufLen = sizeof(TCHAR) * 1024;
	ZeroMemory(szBuffer, dwBufLen);

	INT i = -1;
	INT nLastHome = -1;
	INT nHomeCounter = -1;
	BOOL bFindOracleHome = FALSE;

	HKEY hKey;
	LPCTSTR lpSubKey = NULL;
	LPCTSTR lpValueName = NULL;
	
	lpSubKey  = _T("SOFTWARE\\ORACLE\\ALL_HOMES");
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
	{
		strErrorCode.Format(_T("HKEY_LOCAL_MACHINE\\%s 키가 존재하지 않습니다."), lpSubKey);
		//SetStatus(strLastError);
		this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strErrorCode);
		return -1010;
	}

	// DEFAULT_HOME을 가져온다.
	lpValueName = _T("DEFAULT_HOME");
	dwBufLen = sizeof(TCHAR) * 1024;
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		strErrorCode.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), lpSubKey, lpValueName);
		//SetStatus(strLastError);
		this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strErrorCode);
		return -1020;
	}
	strDefaultHome.Format(_T("%s"), szBuffer);

	// HOME_COUNTER를 가져온다.
	lpValueName = _T("HOME_COUNTER");
	dwBufLen = sizeof(TCHAR) * 1024;
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		strErrorCode.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), lpSubKey, lpValueName);
		//SetStatus(strLastError);
		this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strErrorCode);
		return -1030;
	}
	nHomeCounter = _ttoi(szBuffer);

	// LAST_HOME을 가져온다.
	lpValueName = _T("LAST_HOME");
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		strErrorCode.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), lpSubKey, lpValueName);
		//SetStatus(strLastError);
		this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strErrorCode);
		return -1040;
	}

	// LAST_HOME이 가리키는 KEY 확인
	strTemp.Format(_T("SOFTWARE\\ORACLE\\HOME%s"), szBuffer);
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, strTemp, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &hKey) != ERROR_SUCCESS)
	{
		strErrorCode.Format(_T("HKEY_LOCAL_MACHINE\\%s 키가 존재하지 않습니다."), strTemp);
		//SetStatus(strLastError);
		this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strErrorCode);
		return -1050;
	}
	nLastHome = _ttoi(szBuffer);

	RegCloseKey( hKey );

	// DEFAULT_HOME이 가리키는 KEY 확인
	for (i = 0; i <= nLastHome; i++)
	{
		strTemp.Format(_T("SOFTWARE\\ORACLE\\HOME%d"), i);
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, strTemp, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &hKey) != ERROR_SUCCESS)
		{
			strErrorCode.Format(_T("HKEY_LOCAL_MACHINE\\%s 키가 존재하지 않습니다."), strTemp);
			//SetStatus(strLastError);
			this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strErrorCode);
			return -1060;
		}

		// ORACLE_HOME_NAME 이 맞는지 확인
		lpValueName = _T("ORACLE_HOME_NAME");
		dwBufLen = sizeof(TCHAR) * 1024;
		if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
		{
			strErrorCode.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), strTemp, lpValueName);
			//SetStatus(strLastError);
			this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strErrorCode);
			return -1070;
		}

		if (_tcscmp(szBuffer, strDefaultHome) == 0) 
		{
			bFindOracleHome = TRUE;
			break;		
		}
	}

	if (!bFindOracleHome) 
	{
		strErrorCode = _T("DEFAULT HOME을 찾을수 없습니다.");
		//SetStatus(strLastError);
		this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strErrorCode);
		return -1080;
	}

	// ORACLE_HOME을 확인한다. tnsname.ora로
	lpValueName = _T("ORACLE_HOME");
	dwBufLen = sizeof(TCHAR) * 1024;
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		strErrorCode.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), strTemp, lpValueName);
		//SetStatus(strLastError);
		this->SendMessage(WM_VIEW_STATUS,NULL,(LPARAM)(LPCTSTR)strErrorCode);
		return -1090;
	}

	m_strTNSPath.Format(_T("%s\\network\\admin\\tnsnames.ora"), szBuffer);
	return 0;
}

VOID CLibInfoCollectorDlg::Init()
{
	InitGrid();
	FreeTnsList();

	m_nSearchSucc = 0;
	m_bIsRunning = TRUE;
	m_bIsFirstRun = TRUE;

	m_BackupTNS.m_strHOST = _T("");
	m_BackupTNS.m_strPORT = _T("");
	m_BackupTNS.m_strSERVICE_NAME = _T("");

	m_asFailedLibName.RemoveAll();

	GetDlgItem(IDC_staRunResult)->SetWindowText(_T(""));
}

VOID CLibInfoCollectorDlg::InitGrid()
{
	m_nGridIndex = 0;

	m_pGrid->Clear();
	m_pGrid->SetRows(2);
	m_pGrid->SetColWidth(0,0,500);
	m_pGrid->SetCols(0,3);
	m_pGrid->SetTextMatrix( 0 , 1 , _T("도서관이름") );
	m_pGrid->SetTextMatrix( 0 , 2 , _T("소요시간") );
}

INT CLibInfoCollectorDlg::FreeTnsList()
{
	CString strKey;
	CTns *pTns = NULL;
	POSITION pos = m_mapTns.GetStartPosition();
	while (pos)
	{
		m_mapTns.GetNextAssoc(pos, strKey, (VOID*&)pTns);
		if( NULL == pTns ) 
		{
			continue;
		}

		delete pTns;
		pTns = NULL;
	}

	m_mapTns.RemoveAll();
	
	return 0;
}

//Thread
UINT CLibInfoCollectorDlg::SearchThread(LPVOID pParam)
{
	CLibInfoCollectorDlg* pDlg = (CLibInfoCollectorDlg *)pParam;
	pDlg->Run();
	
	return 0;
}

LRESULT CLibInfoCollectorDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(			WM_VIEW_RESULT_SEARCH == message )
	{
		ViewResultSearch();
	}
	else if(	WM_VIEW_RESULT_EXECUTE == message )
	{
		CString strResult;
		strResult = (LPCTSTR)lParam;
		ViewResultExecute(strResult);
	}
	else if(	WM_VIEW_STATUS == message  )
	{
		CString strMsg;
		strMsg = (LPCTSTR)lParam;

		SetStatus(strMsg);
	}
	else if(	WM_SEARCH_QUERY == message  )
	{
		CString strMsg;
		strMsg = (LPCTSTR)lParam;

		INT ids = SearchQuery(strMsg);
		if( 0 > ids )
		{
			m_bIsRunning = FALSE;
			return TRUE;
		}
	}
	else if(	WM_EXECUTE_QUERY == message  )
	{
		CString strMsg;
		strMsg = (LPCTSTR)lParam;

		INT ids = ExecuteQuery(strMsg);
		if( 0 > ids )
		{
			m_bIsRunning = FALSE;
			return TRUE;
		}
	}
	else if(	WM_VIEW_FAILED_LIB == message)
	{
		ViewGridConnectionFailedLib();
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CLibInfoCollectorDlg::OnbtnStop() 
{
	m_bIsRunning = FALSE;
}

VOID CLibInfoCollectorDlg::SetStatus(CString strMsg) 
{
	GetDlgItem(IDC_staRunResult)->SetWindowText(strMsg);
}

INT CLibInfoCollectorDlg::SetGridColName(CString strCol)
{
	CStringArray asCol;
	//CLocCommonAPI::MakeArrayList( strCol , _T(",") , asCol );
	MakeColList(strCol, asCol);

	CString strData;
	INT i,nCnt,nGridColCnt,nFind;
	nCnt = asCol.GetSize();
	nGridColCnt = m_pGrid->GetColCount();

	m_pGrid->SetCols(0,nCnt+nGridColCnt+1);
	m_pGrid->SetColWidth(0,0,500);
	for( i=0 ; i<nCnt ; i++ )
	{
		strData = asCol.GetAt(i);
		strData.MakeUpper();
		nFind = strData.Find( _T(" AS ") );
		if( 0 <= nFind )
		{
			strData = strData.Mid( nFind+4 );
		}

		strData.TrimLeft();	strData.TrimRight();
		m_pGrid->SetTextMatrix( 0 , i+nGridColCnt+1 , strData );
	}
//DEL 	INT i, nColCnt, nGridColCnt, nFind;
//DEL 	nGridColCnt = m_pGrid->GetColCount();
//DEL 	nColCnt = m_DM.GetColCount();
//DEL 	m_pGrid->SetCols(0,nColCnt+nGridColCnt+1);
//DEL 	m_pGrid->SetColWidth(0,0,500);
//DEL 
//DEL 	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
//DEL 	for(i=0; i<nColCnt ; i++)
//DEL 	{
//DEL 		m_DM.GetColumnRefInfo(i+1, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
//DEL 		m_pGrid->SetTextMatrix(0 , nGridColCnt+i+1, FIELD_NAME);
//DEL 	}
	
	return 0;
}

INT CLibInfoCollectorDlg::ViewGridConnectionFailedLib()
{
	CString strLibName , strRunTime;
	INT nFailedLibCnt , nGridRowCnt , nGridColCnt , i;
	nFailedLibCnt = m_asFailedLibName.GetSize();
	if( 0 < nFailedLibCnt )
	{
		CString strData , strTemp;
		nGridRowCnt = m_pGrid->GetRows();
		m_pGrid->SetRows(nGridRowCnt+nFailedLibCnt);
		for( i=0; i<nFailedLibCnt ; i++ )
		{
			strTemp.Format( _T("%d") , ++m_nGridIndex );
			strData = m_asFailedLibName.GetAt(i);
			AfxExtractSubString(strLibName, strData, 0, '|');
			AfxExtractSubString(strRunTime, strData, 1, '|');

			m_pGrid->SetTextMatrix(m_nGridIndex, 0, strTemp);
			m_pGrid->SetTextMatrix(m_nGridIndex, 1, strLibName);
			m_pGrid->SetTextMatrix(m_nGridIndex, 2, strRunTime);
			m_pGrid->SetTextMatrix(m_nGridIndex, 3, _T("DB 접속 실패"));
		}
	}
	UpdateWindow();

	return 0;
}

/*
@Create
 - 2010.04.13 ADD KSJ
@Description
 - Get Library List
@Parameter
 - 
@Return Value
 - 
@Mini Spec
 1. 
*/
INT CLibInfoCollectorDlg::GetLibList()
{
	m_pLibListDM = FindDM(_T("DM_LibInfoCollector"));
	if( NULL == m_pLibListDM )
	{
		AfxMessageBox( _T("DM_LibInfoCollector is null"), MB_ICONINFORMATION );
		return -1010;
	}

	// 3. 도서관 정보 가져오기
	/// 3-1. File 열기
	CString			strLine;
	CStdioFileEx	fFile;
	CFileException	Exception;
	if( !fFile.Open( m_strLibFilePath , CFile::modeRead | CFile::typeText , &Exception) )
	{
		CString strErrorCode = Exception.m_cause;
		AfxMessageBox( strErrorCode, MB_ICONINFORMATION );
		return -1020;
	}

	TCHAR cUni;
	fFile.Read( &cUni , sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{/// Unicode 아니라면
		fFile.SeekToBegin();
		fFile.Read( &cUni, 3 );
		if( 0xBBEF == cUni )
		{/// UTF일 경우
			fFile.SetCodePage(CP_UTF8);
		}
		else
		{/// ANSI일 경우
			fFile.SetCodePage(CP_ACP);
			fFile.SeekToBegin();
		}
	}

	INT i, ids, nRowCnt=0;
	const INT nInfoCnt = 6;
	CString strInfo;
	CString strAlias[nInfoCnt] = { _T("도서관명"), _T("IP"), _T("PORT")
								  ,_T("SID"), _T("ID"), _T("PWD")};
	while( fFile.ReadString( strLine ) )
	{
		strLine.TrimLeft();		strLine.TrimRight();
		strLine.Replace( _T(" ") , _T("") );
		strLine.Replace( _T("	") , _T("") );
		if( TRUE == strLine.IsEmpty() )
		{
			continue;
		}

		m_pLibListDM->InsertRow(-1);
		nRowCnt = m_pLibListDM->GetRowCount() -1;
		for( i=0; i<nInfoCnt ; i++ )
		{
			AfxExtractSubString(strInfo, strLine, i, '|');
			ids = m_pLibListDM->SetCellData( strAlias[i], strInfo, nRowCnt);			
		}
	}

	return 0;
}

INT CLibInfoCollectorDlg::MakeColList(CString strField, CStringArray &asColList)
{
	strField.TrimLeft();	strField.TrimRight();
	asColList.RemoveAll();

	INT nIndex,nTemp[2];
	CString strFieldName , strTemp;
	BOOL bIsStop = FALSE;
	while(TRUE)
	{
		nIndex = strField.Find(_T(","));
		if( 0 > nIndex )
		{
			asColList.Add(strField);
			bIsStop = TRUE;
		}
		else
		{
			strFieldName = strField.Mid( 0 , nIndex );
			nTemp[0] = GetCharacterCount(strFieldName, _T("("));
			nTemp[1] = GetCharacterCount(strFieldName, _T(")"));
			if(nTemp[0] == nTemp[1])
			{
				strField = strField.Mid( nIndex+1 , strField.GetLength()-nIndex );
				strFieldName.TrimLeft(); strFieldName.TrimRight();
				asColList.Add( strFieldName );
			}
			else
			{
				while(TRUE)
				{
					nIndex = strField.Find(_T(","),nIndex+1);
					if(0 > nIndex)
					{
						asColList.Add(strField);
						bIsStop = TRUE;
						break;
					}
					else
					{
						strFieldName = strField.Mid( 0 , nIndex );
						nTemp[0] = GetCharacterCount(strFieldName, _T("("));
						nTemp[1] = GetCharacterCount(strFieldName, _T(")"));
						if(nTemp[0] == nTemp[1])
						{
							strField = strField.Mid( nIndex+1 , strField.GetLength()-nIndex );
							strFieldName.TrimLeft(); strFieldName.TrimRight();
							asColList.Add( strFieldName );
							break;
						}
					}
				}
			}
		}

		if(TRUE==bIsStop) break;
	}

	return 0;
}

INT CLibInfoCollectorDlg::GetCharacterCount(CString strLine, CString strFind)
{
	INT nCnt=0;
	INT nIdx=0;

	while(TRUE)
	{
		nIdx = strLine.Find(strFind, nIdx);
		if(0 <= nIdx)
		{
			nIdx++;
			nCnt++;
		}
		else break;
	}

	return nCnt;
}
