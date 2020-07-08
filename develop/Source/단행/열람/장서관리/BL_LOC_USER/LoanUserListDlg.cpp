#include "stdafx.h"
#include "LoanUserListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CLoanUserListDlg::CLoanUserListDlg(CESL_Mgr* pParent /*=NULL*/,CString sWhere)
	: CESL_Mgr(IDD, pParent)
{
	m_nCloseState = -1;
	m_nInit = 0;
	m_sWhere = sWhere;
	m_bIsSearhed = FALSE;
	m_bInEnter = FALSE;
	m_strUserAccessAuth = _T("");
}

CLoanUserListDlg::~CLoanUserListDlg()
{
	
}

VOID CLoanUserListDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);	
	//{{AFX_DATA_MAP(CLoanUserListDlg) 
	//}}AFX_DATA_MAP	

	EFS_END
}


BEGIN_MESSAGE_MAP(CLoanUserListDlg, CDialog)
	//{{AFX_MSG_MAP(CLoanUserListDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnUSER_SELECT, OnbtnUSERSELECT)
	ON_BN_CLICKED(IDC_btnCLOSE_USER_SELECT, OnbtnCLOSEUSERSELECT)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()


BOOL CLoanUserListDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	

	if(InitESL_Mgr(_T("BL_LOC_USER_LIST")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return -1;
	}

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabUSERTITLE);
	pTab->InsertItem( 0 , _T("이용자 목록") );

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_MAINGRID_USER );
	pGrid->SetSort(TRUE);
	
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		((CESL_Grid*)pGrid)->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		((CESL_Grid*)pGrid)->m_bPrivacyPrintLogRecord = TRUE;
		((CESL_Grid*)pGrid)->m_strPrintInfo = _T("대출자 검색");
	}
	
	if( m_strUserAccessAuth.IsEmpty() || m_strUserAccessAuth == _T("1") )
	{
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("반"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("번호"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("휴대폰"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("학년"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("자택전화"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("근무지전화"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("학교"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("대출자ID"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("학과"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("학부"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("대출자주소"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("회원증비밀번호"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("생일"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("이메일"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("G-PINASCII인증코드"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("G-PINHASH인증코드"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("G-PIN성별"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("대리인명"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("대리인관계"),TRUE);
		((CESL_Grid*)pGrid)->SetColumnEncryption(_T("대출자이름"),TRUE);
	}

	RunInitProcess();	

	CRect pRect;		 

	 this->GetParent()->GetWindowRect(&pRect); 

	 INT y = pRect.top + (pRect.bottom - pRect.top)/2;
	 INT x = pRect.left + (pRect.right - pRect.left)/2 ;

	SetCursorPos(x, y);

	return TRUE;  
	              

	EFS_END
	return FALSE;

}



INT CLoanUserListDlg::SetWhereSql( CString sWhere )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BL_LOC_USER_LIST");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CLoanUserListDlg::SetWhereSql( CString &sWhere )") );

	ids = CLocCommonAPI::RefreshDataManager( this , pDM , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CLoanUserListDlg::SetWhereSql( CString &sWhere )") );

	if( pDM->GetRowCount() ==  1 )
		m_nCloseState = 0;

	
	ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("대출소속정보") , _T("대출소속정보") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("CLoanUserListDlg::SetWhereSql( CString &sWhere )") );
	
	return 0;

	EFS_END
	return -1;
}


INT CLoanUserListDlg::GetCount()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CLoanUserListDlg::GetCount()") );

	ids = pDM->GetRowCount();
	
	return ids;

	EFS_END
	return -1;
}

INT CLoanUserListDlg::GetSelectUserKey( CString &sUserRecKey )
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CLoanUserListDlg::GetSelectUserKey( CString &sUserRecKey )") );

	
	if( m_nCloseState > -1 )
	{

		CESL_DataMgr TmpDM;
		CLocCommonAPI::MakeCopyDM( this , pDM , &TmpDM );
		
		ids = CLocCommonAPI::AddDM( pDM ,&TmpDM , m_nCloseState , -1 , this);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CLoanUserListDlg::GetSelectUserKey( CString &sUserRecKey )") );

		pDM->FreeData();

		ids = CLocCommonAPI::AddDM( &TmpDM , pDM , 0 , -1 , this  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CLoanUserListDlg::GetSelectUserKey( CString &sUserRecKey )") );

		return 0;
	}
	
	else
	{
		pDM->FreeData();
		sUserRecKey.Empty();
		return 1;
	}

	return 0;

	EFS_END
	return -1;
}

BEGIN_EVENTSINK_MAP(CLoanUserListDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CLoanUserListDlg)
	ON_EVENT(CLoanUserListDlg, IDC_MAINGRID_USER, 69 /* SelChange */, OnSelChangeMaingridUser, VTS_NONE)
	ON_EVENT(CLoanUserListDlg, IDC_MAINGRID_USER, -601 /* DblClick */, OnDblClickMaingridUser, VTS_NONE)
	ON_EVENT(CLoanUserListDlg, IDC_MAINGRID_USER, -605 /* MouseDown */, OnMouseDownMaingridUser, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CLoanUserListDlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if ( VK_ESCAPE==pMsg->wParam )
		PostMessage( WM_CLOSE, NULL, NULL );
	else if ( (WM_KEYDOWN==pMsg->message) && (VK_RETURN==pMsg->wParam) )
		OnbtnUSERSELECT( );
	else if ( pMsg->message == WM_LBUTTONDBLCLK )
	{
	
	}
		
	return CESL_Mgr::PreTranslateMessage(pMsg);
	
	EFS_END
	return FALSE;
}

VOID CLoanUserListDlg::OnSelChangeMaingridUser() 
{
	EFS_BEGIN

	INT ids = 0;
	INT nCloseState = -1;

	CString sCMAlias = _T("CM_BL_LOC_USER_LIST");
	CString sGridAlias = _T("BL_LOC_USER_LIST_GRID");

	CESL_Grid *pGrid = (CESL_Grid*) FindControl( sCMAlias, sGridAlias );
	if( pGrid == NULL ) return ;

	nCloseState = pGrid->GetIdx( );
	if ( (nCloseState<0) || (m_nCloseState==nCloseState) )
		return;

	ids = SetReverse(pGrid, nCloseState);
	if ( ids < 0 ) return;

	EFS_END
}

VOID CLoanUserListDlg::OnDblClickMaingridUser() 
{
	EFS_BEGIN

	CESL_Grid *pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( _T("CM_BL_LOC_USER_LIST") , _T("BL_LOC_USER_LIST_GRID") );
	if( pGrid_MainGrid_ALL == NULL ) return;
	
	INT i = pGrid_MainGrid_ALL->GetRowSel();
	INT j = pGrid_MainGrid_ALL->GetMouseRow();
	CPoint k = pGrid_MainGrid_ALL->GetCaretPos();
	INT l = pGrid_MainGrid_ALL->GetRow();
	INT m = pGrid_MainGrid_ALL->GridGetRow();
	INT n = pGrid_MainGrid_ALL->GetMousePointer();

	if( j < 1 ) return;
		
	OnbtnUSERSELECT( );

	EFS_END
}

INT CLoanUserListDlg::SetReverse(CESL_Grid *pGrid, INT nIndex)
{
	EFS_BEGIN

	m_nCloseState = nIndex;

	return pGrid->SetReverse( m_nCloseState );

	EFS_END
	return -1;
}

INT CLoanUserListDlg::ViewGrid()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_BL_LOC_USER_LIST");
	CString sGridAlias = _T("BL_LOC_USER_LIST_GRID");

	CESL_Grid* pGrid = ( CESL_Grid* ) FindControl( sCMAlias, sGridAlias );
	if ( NULL==pGrid ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CLoanUserListDlg::ViewGrid()") );

	
	
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
	if( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("CLoanUserListDlg::ViewGrid()") );

	
	
	CString strValue1;
	
	ids = this->GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1 );
	if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2(  -6 , _T("CLoanUserListDlg::ViewGrid()") );

	strValue1.TrimLeft();strValue1.TrimRight();
	strValue1.MakeUpper();

	CString strUserNo ,  strCivilNo , strBuf;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		if( _T("Y") != strValue1 )
		{
			ids = pDM->GetCellData(_T("대출자번호"), i, strUserNo);
			if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("CLoanUserListDlg::ViewGrid()") );

			CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
			ids = pDM->SetCellData( _T("대출자번호표시"), strUserNo, i );
			if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2(  -10 , _T("CLoanUserListDlg::ViewGrid()") );
		}
		
		else
		{
			ids = pDM->GetCellData(_T("대출자번호"), i, strUserNo);
			if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("CLoanUserListDlg::ViewGrid()") );
			ids = pDM->SetCellData( _T("대출자번호표시"), strUserNo, i );
			if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2(  -10 , _T("CLoanUserListDlg::ViewGrid()") );
		}

		ids = pDM->GetCellData( _T("주민등록번호") , i , strCivilNo );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(  -11 , _T("ViewGrid") );
		}

		CLocCommonAPI::ChangeSecuriyCivilNo( this , strCivilNo , m_sIsUnityLoanService );

		ids = pDM->SetCellData( _T("주민등록번호_VIEW"), strCivilNo, i );
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2(  -10 , _T("ViewGrid") );
	}

	ids = AllControlDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CLoanUserListDlg::ViewGrid()") );
	
	ids = SetReverse(pGrid, 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CLoanUserListDlg::ViewGrid()") );
	return 0;

	EFS_END
	return -1;
}

VOID CLoanUserListDlg::OnClose() 
{	
	EFS_BEGIN

	OnbtnCLOSEUSERSELECT();

	EFS_END
}

VOID CLoanUserListDlg::OnbtnUSERSELECT() 
{
	EFS_BEGIN

	if ( m_nCloseState < 0 )
	{
		AfxMessageBox( _T("대출자를 먼저 선택해주십시오-OnbtnUSERSELECT") );
		return ;
	}

	OnCancel();

	EFS_END
}

VOID CLoanUserListDlg::OnbtnCLOSEUSERSELECT() 
{
	EFS_BEGIN

	m_nCloseState = -1;
	OnCancel();

	EFS_END
}

VOID CLoanUserListDlg::OnTimer(UINT nIDEvent) 
{
	EFS_BEGIN

	if( nIDEvent == 0 )
	{
		KillTimer(0);
		if( GetSafeHwnd() == NULL ) 
		{
			SetTimer(0,1,NULL);
			return;
		}
		RunInitProcess();
		return;
	}
	
	CDialog::OnTimer(nIDEvent);

	EFS_END
}


INT CLoanUserListDlg::RunInitProcess()
{
	EFS_BEGIN

	INT ids = ViewGrid();
	if( ids ){
		AfxMessageBox( _T("RunInitProcess ") );
		return -1;
	}

	m_bIsSearhed = TRUE;

	return 0;

	EFS_END
	return -1;
}


INT CLoanUserListDlg::SortGridData(INT nColIndex)
{
	EFS_BEGIN


	return 0;

	EFS_END
	return -1;
}

VOID CLoanUserListDlg::OnMouseDownMaingridUser(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN



	EFS_END	
}

HBRUSH CLoanUserListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
