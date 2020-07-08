// CO_CENTER_USER_CHANGE_HISTORY.cpp : implementation file
//

#include "stdafx.h"
#include "CO_CENTER_USER_CHANGE_HISTORY.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_CHANGE_HISTORY dialog


CCO_CENTER_USER_CHANGE_HISTORY::CCO_CENTER_USER_CHANGE_HISTORY(CESL_Mgr* pParent , CString sParentCMAlias , CString sParentGridAlias )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_CHANGE_HISTORY)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sParentCMAlias = sParentCMAlias;
	m_sParentGridAlias = sParentGridAlias;

	m_pRS_SQL = NULL;
	
}
CCO_CENTER_USER_CHANGE_HISTORY::~CCO_CENTER_USER_CHANGE_HISTORY()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
	delete m_pRS_SQL;
}

VOID CCO_CENTER_USER_CHANGE_HISTORY::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_CHANGE_HISTORY)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_CHANGE_HISTORY, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_CHANGE_HISTORY)
	ON_BN_CLICKED(IDC_btnDPREV, OnbtnDPREV)
	ON_BN_CLICKED(IDC_btnDNEXT, OnbtnDNEXT)
	ON_BN_CLICKED(IDC_btnDCLOSE, OnbtnDCLOSE)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_CHANGE_HISTORY message handlers

BOOL CCO_CENTER_USER_CHANGE_HISTORY::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );	

	INT ids = InitESL_Mgr(_T("CO_CENTER_USER_CHANGE_HISTORY"));
	if( ids < 0 )
	{
		AfxMessageBox(_T("Init ESL Error[]"));
		return FALSE;
	}

/*
ADDRESS_HOME			H_ADDR1	
ADDRESS_WORK			W_ADDR1	
FAMILY_ID				FID_KEY
IDX_NAME				IDX_USER_NAME
LOAN_USER_CLASS_CODE	USER_CLASS_CODE)	
LOAN_USER_POSITION_CODE	USER_POSITION_CODE)	
PHONE_HOME				H_PHONE
PHONE_WORK				W_PHONE
ZIP_CODE_HOME			H_ZIPCODE
ZIP_CODE_WORK			W_ZIPCODE
INFERIOR_CLASS			USER_CLASS
*/
	
	m_pRS_SQL = new CRS_SQL(this);

	SetScreen();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




/// 부모 그리드 포인터
CESL_Grid* CCO_CENTER_USER_CHANGE_HISTORY::GetMainGridPtr()
{
	CESL_ControlMgr *pCM = FindCM( m_sParentCMAlias );
	if( pCM == NULL ) return NULL;
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(m_sParentGridAlias));
	if( pGrid == NULL ) return NULL;
	
	return pGrid;
}


/// 이전 다음 이동
INT CCO_CENTER_USER_CHANGE_HISTORY::MoveIndex( INT nMode )
{
		
	if( m_nViewMode == 0 )
	{
		if( nMode == 0 && ( m_nCurrentIndex != m_nFirstIndex )) m_nCurrentIndex--;
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) ) m_nCurrentIndex++;
	}
	else
	{
		CESL_Grid *pGrid = GetMainGridPtr();
		if( nMode == 0 )
		{
			m_nCurrentIndex = pGrid->SelectGetPrev();
		}
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) )
		{
			m_nCurrentIndex = pGrid->SelectGetNext();
		}
	}
	
	SetPreNextButton();
	
	return 0;
	
}

/// 현재 인덱스와 처음 인덱스 , 마지막 인덱스 설정
INT CCO_CENTER_USER_CHANGE_HISTORY::SetCurrentIndex()
{
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) return -1;
	
	// 이전 다음이
	// 'V'체크가 된 것이 있다면 'V'체크 된것만 보여주며
	// 'V'체크가 된 것이 없다면 모두 보여준다.
	INT nTmpIndex;
	if( bFirstView ) nTmpIndex = pGrid->GetIdx();
	else nTmpIndex = pGrid->GetIdx();
	
	pGrid->SelectMakeList();
	INT nSelectCount = pGrid->SelectGetCount();
	if( nSelectCount > 0 ) m_nViewMode = 1;
	else m_nViewMode = 0;
	
	if( m_nViewMode == 0 )
	{
		m_nCurrentIndex = pGrid->GetIdx();
		m_nLastIndex = pGrid->GetCount() - 1;
		m_nFirstIndex = 0;
	}
	else
	{
		m_nLastIndex = pGrid->SelectGetTailPosition();
		m_nFirstIndex = pGrid->SelectGetHeadPosition();
		for( INT i = 0 ; i < nSelectCount-1 ; i++ )
		{
			if( nTmpIndex == pGrid->SelectGetIdx() )
				break;
			pGrid->SelectGetNext();
		}
		if( i==nSelectCount ) m_nCurrentIndex = pGrid->SelectGetHeadPosition();
		else m_nCurrentIndex = pGrid->SelectGetIdx();
		
	}

	SetPreNextButton();
	
	return 0;
}


/// 이전,다음 버튼 활성화 비활성화
VOID CCO_CENTER_USER_CHANGE_HISTORY::SetPreNextButton()
{
	
	GetDlgItem(IDC_btnDPREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnDNEXT)->EnableWindow(TRUE);
	
	if( m_nCurrentIndex == m_nFirstIndex )
	{
		GetDlgItem(IDC_btnDPREV)->EnableWindow(FALSE);
	}
	if( m_nCurrentIndex == m_nLastIndex )
	{
		GetDlgItem(IDC_btnDNEXT)->EnableWindow(FALSE);
	}
}


/// 정보를 화면에 보여줌
INT CCO_CENTER_USER_CHANGE_HISTORY::DisplayWindow()
{
	INT ids;

	ids = SearchHistory();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayWindow") );

	ids = ShowHistotyInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DisplayWindow") );

	/// 3. 아래 그리드에 리버스하기
	CESL_Grid* pParentGrid = GetMainGridPtr();
	if( pParentGrid == NULL ) return 0;
	
	pParentGrid->SetReverse(m_nCurrentIndex);
	

	return 0;
}


INT CCO_CENTER_USER_CHANGE_HISTORY::SetScreen()
{
	
	AfxGetApp()->DoWaitCursor(1);
	
	SetCurrentIndex();
	DisplayWindow();	
	
	AfxGetApp()->DoWaitCursor(-1);
	
	return 0;
}

INT CCO_CENTER_USER_CHANGE_HISTORY::SearchHistory()
{
	CString sWhere;

	CESL_DataMgr* pMasterUserDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );	
	if( pMasterUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchHistory") );
	
	CString sMasterUserKey;
	INT ids = pMasterUserDM->GetCellData( _T("레코드KEY") , m_nCurrentIndex , sMasterUserKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchHistory") );
	
	sWhere.Format(_T("MASTER_USER_KEY = %s") , sMasterUserKey );
	
	CESL_DataMgr* pLibUserInfo = FindDM(_T("DM_CO_CENTER_USER_CHANGE_HISTORY"));
	if( pLibUserInfo == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchHistory") );
	
//	m_pRS_SQL->SetFetchCount( 0, 100 );
//	ids = m_pRS_SQL->LOAN_Select( pLibUserInfo , sWhere , _T("") , 1 , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SearchHistory") );	
	
	/// 값변경
	/// 1. 주민등록번호 , 우편발송지역
	CString sCivilNo , sSendMail;
	for( INT i = 0 ; i < pLibUserInfo->GetRowCount() ; i++ )
	{
		ids = pLibUserInfo->GetCellData( _T("CIVIL_NO") , i , sCivilNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SearchHistory") );	

		ids = pLibUserInfo->GetCellData( _T("MAIL_SEND_AREA") , i , sSendMail );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5, _T("SearchHistory") );	
		
		CLocCommonAPI::ChangeSecuriyCivilNo( this , sCivilNo , m_sIsUnityLoanService );

		if( sSendMail.Compare(_T("H")) == 0 ) sSendMail = _T("집");
		else sSendMail = _T("직장");

		ids = pLibUserInfo->SetCellData( _T("CIVIL_NO") , sCivilNo , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("SearchHistory") );	
		
		ids = pLibUserInfo->SetCellData( _T("MAIL_SEND_AREA") , sSendMail , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("SearchHistory") );	
		
	}

	return 0;
}

INT CCO_CENTER_USER_CHANGE_HISTORY::ShowHistotyInfo()
{
	INT ids;

	ids = AllControlDisplay(_T("CM_CO_CENTER_USER_CHANGE_HISTORY"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ChangeView") );
	

	return 0;
}

VOID CCO_CENTER_USER_CHANGE_HISTORY::OnbtnDPREV() 
{
	AfxGetApp()->DoWaitCursor(1);
	
	MoveIndex(0);	
	DisplayWindow();
	
	AfxGetApp()->DoWaitCursor(-1);
}

VOID CCO_CENTER_USER_CHANGE_HISTORY::OnbtnDNEXT() 
{
	AfxGetApp()->DoWaitCursor(1);
	
	MoveIndex(1);	
	DisplayWindow();	
	
	AfxGetApp()->DoWaitCursor(-1);
}

VOID CCO_CENTER_USER_CHANGE_HISTORY::OnbtnDCLOSE() 
{
	OnCancel();	
}

VOID CCO_CENTER_USER_CHANGE_HISTORY::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if( GetSafeHwnd() == NULL ) return;

	if( GetDlgItem(IDC_gridDDCENTERUSER2)->GetSafeHwnd() == NULL ) return;

	CRect WndPos;
	GetDlgItem(IDC_gridDDCENTERUSER2)->GetWindowRect(WndPos);

	ScreenToClient(WndPos);

	GetDlgItem(IDC_gridDDCENTERUSER2)->MoveWindow( 0 , WndPos.top , cx , cy - WndPos.top - 10 );

	
}

HBRUSH CCO_CENTER_USER_CHANGE_HISTORY::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
