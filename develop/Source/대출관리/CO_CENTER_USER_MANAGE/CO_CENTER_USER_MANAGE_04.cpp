// CO_CENTER_USER_MANAGE_04.cpp : implementation file
//

#include "stdafx.h"
#include "CO_CENTER_USER_MANAGE_04.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_MANAGE_04 dialog


CCO_CENTER_USER_MANAGE_04::CCO_CENTER_USER_MANAGE_04(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD,pParent)
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_MANAGE_04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCO_CENTER_USER_MANAGE_04::~CCO_CENTER_USER_MANAGE_04()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
	delete m_pRS_SQL;
}


VOID CCO_CENTER_USER_MANAGE_04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_MANAGE_04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_MANAGE_04, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_MANAGE_04)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_MANAGE_04 message handlers

BOOL CCO_CENTER_USER_MANAGE_04::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("CO_CENTER_USER_MANAGER_01")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	// 2010.09.08 ADD BY KSJ : 통합도서서비스 사용여부
	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService );
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");

	m_pRS_SQL = new CRS_SQL(this);
	InitDefaultDM();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




VOID CCO_CENTER_USER_MANAGE_04::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	

	// 그리드 리사이즈
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridCENTERUSER);
	
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	pGrid->MoveWindow( 0 , 0 , cx , cy );

	EFS_END
}



BOOL CCO_CENTER_USER_MANAGE_04::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


INT CCO_CENTER_USER_MANAGE_04::SearchedUser()
{
	EFS_BEGIN


	CESL_DataMgr* pDM = FindDM(_T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST"));

	return pDM->GetRowCount();

	EFS_END

	return -1;
}

INT CCO_CENTER_USER_MANAGE_04::DeleteUser()
{
	EFS_BEGIN

	INT ids;


	CString sODMAlias = _T("DM_CO_CENTER_USER_MANAGER_01");
	
	CESL_DataMgr* pDM = FindDM( sODMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DeleteUser") );
	
	CESL_Grid* pParentGrid = (CESL_Grid*)FindControl( _T("CM_CO_CENTER_USER_MANAGER_01"),_T("센터이용자정보그리드"));
	if( pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DeleteUser") );

	pParentGrid->SelectMakeList();

	INT nSelectedCnt = pParentGrid->SelectGetCount();

	if( nSelectedCnt < 1 )
	{
		AfxMessageBox( _T("삭제할 이용자를 먼저 선정해주십시오.") );
		return 1;
	}

	INT nSelectedIndex;

	pParentGrid->SelectGetTailPosition();

	CString sMasterRecKey , sLibUserRecKey;

	CLocProgressBarDlg* pProgress = CreateProgressBarDlg(nSelectedCnt);

	CString sCntSql;
	CString sDuplicateCnt;
	for( INT i = 0 ; i < nSelectedCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgress->ChangeProgressBar(i);
		
		nSelectedIndex = pParentGrid->SelectGetIdx();

		/// 하나인지 Select해온다.
		pDM->GetCellData( _T("유저통합관리KEY") , nSelectedIndex , sMasterRecKey );
		pDM->GetCellData( _T("도서관별대출자정보KEY") , nSelectedIndex  , sLibUserRecKey );


		sCntSql.Format(_T("SELECT COUNT(*) FROM CO_LIBRARY_USER_TBL WHERE USER_KEY = %s") , sMasterRecKey);
		
		ids = pDM->GetOneValueQuery( sCntSql , sDuplicateCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DeleteUser") );
		

		/// 하나라면 Master도 삭제
		pDM->StartFrame();
		pDM->InitDBFieldData();
		pDM->MakeDeleteFrame( _T("CO_LIBRARY_USER_TBL")	, _T("REC_KEY") , _T("NUMERIC") , sLibUserRecKey );

		if( _ttoi(sDuplicateCnt) == 1 )
		{
			pDM->InitDBFieldData();
			pDM->MakeDeleteFrame( _T("CO_MASTER_LOAN_USER_TBL")	, _T("REC_KEY") , _T("NUMERIC") , sMasterRecKey );			
		}

		ids = pDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DeleteUser") );
		pDM->EndFrame();

		pDM->DeleteRow( nSelectedIndex );

		pParentGrid->SelectGetPrev();

	}

	DeleteProgressBarDlg(pProgress);

	pParentGrid->Display();

	AfxMessageBox( _T("삭제하였습니다.") );

	return 0;

	EFS_END

	return -1;
}

// 프로그래스바 생성
CLocProgressBarDlg* CCO_CENTER_USER_MANAGE_04::CreateProgressBarDlg( INT nUpper )
{
	CLocProgressBarDlg* pProgressBar = new CLocProgressBarDlg(this);
	pProgressBar->Create( this );
	pProgressBar->ShowWindow( SW_SHOW );
	pProgressBar->InitRange(nUpper);
	
	return pProgressBar;
}

// 프로그래스바 종료
INT CCO_CENTER_USER_MANAGE_04::DeleteProgressBarDlg( CLocProgressBarDlg *pProgressBarDlg )
{
	if( pProgressBarDlg != NULL ) 
		delete pProgressBarDlg;
	pProgressBarDlg = NULL;
	
	return 0;
}



/// 기본 DM 설정
/// 검색 조건의 도서관명을 위해 도서관 정보를 Get
INT CCO_CENTER_USER_MANAGE_04::InitDefaultDM()
{
	EFS_BEGIN
		
		
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_LOCAL_LIB_INFO") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitDefaultDM") );

//	m_pRS_SQL->SetFetchCount( 0, 100 );
//	INT ids = m_pRS_SQL->LOAN_Select( pDM, _T(""), _T("DM_CO_LOCAL_LIB_INFO"), 1, 1 );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitDefaultDM") );
	
	return 0;
	
	EFS_END
		
	return -1;
}

HBRUSH CCO_CENTER_USER_MANAGE_04::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// 여기까지 추가

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
