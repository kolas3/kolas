// BO_LOC_33001.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_33001.h"
#include "..\\BL_LOC_USER\\BO_LOC_3320.h"
#include "..\\BL_LOC_USER\\BL_LOC_USER_PROC.h"
#include "BO_LOC_SECEDE.h"
#include "BO_LOC_3000Api.h"
#include "PersonInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_LOC_3300::CBO_LOC_3300(CESL_Mgr* pParent )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3300)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLoanShareManager = NULL;
	m_pLoanMangeValue = NULL;
}

CBO_LOC_3300::~CBO_LOC_3300()
{
	if( m_pLoanShareManager != NULL )	delete m_pLoanShareManager;
	if( m_pLoanMangeValue != NULL )	delete m_pLoanMangeValue;
}


VOID CBO_LOC_3300::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3300)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3300, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3300)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_COMMAND_RANGE( SORT_POSITION_CODE, SORT_CLASS_CODE_DESC, SortExDM )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3300 message handlers

BOOL CBO_LOC_3300::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BOOL CBO_LOC_3300::OnInitDialog() 
{
	EFS_BEGIN
	CDialog::OnInitDialog();
		
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	
	if(InitESL_Mgr(_T("BO_LOC_3300")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pLoanMangeValue = new CLonaManageValue(this);
	m_pLoanMangeValue->pMain = pMain;
	m_pLoanMangeValue->pParentMgr = (CESL_Mgr*)this;
	m_pLoanMangeValue->SM_STYLE = 'T';
	m_pLoanMangeValue->m_pInfo = this->m_pInfo;
	m_pLoanMangeValue->DoModal();

	m_pLoanShareManager = new CLoanShareManager(this);
	m_pLoanShareManager->Create(this);

	InitScreen();

	CESL_Grid *pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( _T("CM_BO_LOC_3300") , _T("MainGrid") );
	if( pGrid_MainGrid_ALL == NULL ) return -1;

	if(!m_pInfo->m_bUserInfoExportYN)
	{
		//pGrid_MainGrid_ALL->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid_MainGrid_ALL->m_bPrivacyPrintLogRecord = TRUE;
		pGrid_MainGrid_ALL->m_strPrintInfo = _T("이용자 관리 엑셀출력");
	}
	//-------------------------------------------------------------------------

	CArray <CString,CString> RemoveListArray;
	pGrid_MainGrid_ALL->InitPopupMenuItem();
	pGrid_MainGrid_ALL->AddPopupMenuItem( (CView*)pMain , this->VIEW_STYLE , &RemoveListArray );
	
	ChangeToolBarButton();

	CString strButtonAlias ;
	if(m_pInfo->MODE==10000)
	{
		strButtonAlias = _T("메일관리");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
	}

	if(_T("Y") != m_pLoanMangeValue->m_ArrearCheckYN)
	{
		strButtonAlias = _T("연체정보보기");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
	}
	
	if(m_pInfo->GetSMSUseMode() == FALSE)
	{
		strButtonAlias = _T("만료회원SMS안내");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3300::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	pGrid->MoveWindow(0, 0, cx, cy);

	EFS_END
}

VOID CBO_LOC_3300::InitScreen()
{
	EFS_BEGIN

	CString bCivilNoDisplayCheck;
	CString strColName;
	INT		i;
	INT		nColCnt;

	m_pLoanMangeValue->GetManageValue( _T("열람") , _T("공통") , _T("이용자관리") , _T("주민등록번호사용유무") , bCivilNoDisplayCheck );
	if ( bCivilNoDisplayCheck == _T("N") )
	{
		CESL_Grid *pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( _T("CM_BO_LOC_3300") , _T("MainGrid") );
		if( pGrid_MainGrid_ALL == NULL ) return;	
		nColCnt = pGrid_MainGrid_ALL->GetColCount();

		for ( i=0 ; i<nColCnt ; i++ )
		{
			strColName = pGrid_MainGrid_ALL->GetTextMatrix(0,i);
			if ( strColName==_T("주민등록번호") )
			{
				pGrid_MainGrid_ALL->SetColWidth(i,0,0);
			}
		}
	}

	EFS_END
}
BEGIN_EVENTSINK_MAP(CBO_LOC_3300, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_3300)
	ON_EVENT(CBO_LOC_3300, IDC_gridMAIN, -601 /* DblClick */, OnDblClickgridMAIN, VTS_NONE)
	ON_EVENT(CBO_LOC_3300, IDC_gridMAIN, -605 /* MouseDown */, OnMouseDowngridMAIN, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_3300::OnDblClickgridMAIN() 
{
	EFS_BEGIN

		
	CESL_Grid *pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( _T("CM_BO_LOC_3300") , _T("MainGrid") );
	if( pGrid_MainGrid_ALL == NULL ) return;
	
	if( pGrid_MainGrid_ALL->GetMouseRow() < 1 ) return;


	pGrid_MainGrid_ALL->SelectMakeList();
	if( pGrid_MainGrid_ALL->SelectGetCount() > 0 )
		pGrid_MainGrid_ALL->SetAt( pGrid_MainGrid_ALL->GetIdx() , 0 , _T("V") );

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3300_LIST") );
	if( pDM->GetRowCount() < 1 ) 
		return ;

	CBO_LOC_3320 Dlg( this, m_pLoanShareManager, m_pLoanMangeValue );
	Dlg.SetMode(1,TRUE);
	Dlg.m_nAction = ACTION_READ_F_USERINFO;
	if( Dlg.CheckParentDMCount() < 1 )
		return;
	
	Dlg.DoModal();

	EFS_END
}

INT CBO_LOC_3300::ChangeToolBarButton()
{
	EFS_BEGIN

	CString sValue;
	m_pLoanMangeValue->GetManageValue( _T("열람") , _T("공통") , _T("대출자접근권한") , GetUserID() , sValue );
	CString strButtonAlias;
	
	if( sValue.Compare(_T("1"))==0 )
	{
		strButtonAlias = _T("입력");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
		strButtonAlias = _T("수정");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
		strButtonAlias = _T("삭제");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);				
		strButtonAlias = _T("제적처리");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
		strButtonAlias = _T("일괄변경");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
		strButtonAlias = _T("센터업로드");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
		strButtonAlias = _T("센터반입");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
		strButtonAlias = _T("반입반출");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
	}
	else if(0 == sValue.Compare(_T("2")))
	{
		strButtonAlias = _T("삭제");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
	}

	if( m_pInfo->GetLoanUserShareMode() != 1 && (m_pInfo->GetLoanUserShareMode() != 2) )
	{
		strButtonAlias = _T("센터반입");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
	}
	strButtonAlias = _T("센터업로드");
	pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		

	return 0;
	
	EFS_END
		
	return -1;
}

INT CBO_LOC_3300::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	
	return 0;
	
	EFS_END
	return -1;	
}

INT CBO_LOC_3300::PrintList()
{
EFS_BEGIN
	
	INT ids;

	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BO_LOC_3300_LIST") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
		return 1;
	}
	
	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BO_LOC_3300_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	pPrintDM->FreeData();

	const INT nCopyCnt = 17;
	
	TCHAR *sField[nCopyCnt][2] =
	{
		{_T("주민등록번호"),_T("주민등록번호")},
		{_T("E_MAIL"),_T("E_MAIL")},
		{_T("불량회원구분"),_T("불량회원구분")},
		{_T("총대출책수"),_T("총대출책수")},
		{_T("대출직급정보"),_T("대출직급정보")},
		{_T("대출소속정보"),_T("대출소속정보")},
		{_T("이름"),_T("이름")},
		{_T("자택전화"),_T("자택전화")},
		{_T("총예약책수"),_T("총예약책수")},
		{_T("대출자번호_VIEW"),_T("대출자번호")},
		{_T("연체수"),_T("연체수")},
		{_T("학년"),_T("학년")},
		{_T("반"),_T("반")},
		{_T("교번") ,_T("교번")},
		{_T("등록일"),_T("등록일")},
		{_T("집주소"),_T("집주소")},
		{_T("REC_KEY"),_T("REC_KEY")}
	};

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3300") , _T("MainGrid") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	
	INT nMode = 0;
	INT nSelectCnt = pGrid->SelectGetCount();
	
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	
	CString sData;
	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();		

		pPrintDM->InsertRow(-1);
		
		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("번호") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		
		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}
		if( nMode == 1)
			pGrid->SelectGetNext();
	}
	
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("이용자관리"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}
	
	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	pSIReportManager->SetPrivacyPrintLogParm(TRUE, m_pInfo->USER_PK, m_pInfo->USER_ID, _T("이용자 관리 목록출력"), m_pInfo->MANAGE_CODE ,GetWorkLogMsg(_T("이용자 관리 목록출력"),__WFILE__,__LINE__), 0);
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_PRINT_LOG") );

	std::vector<CString> list;
	CString strKey;
	for(i=0; i<pPrintDM->GetRowCount(); i++)
	{
		ids = pPrintDM->GetCellData(_T("REC_KEY"), i, strKey );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3300::PrintList()") );
		list.push_back(strKey);
	}
	CPersonInfo::SetPrintLog(this, pDM, PERINFO_EXCEL, m_pInfo, _T("이용자목록출력"), pPrintDM->GetRowCount(), &list);
	list.clear();
	
	return 0;
	
EFS_END
	return -1;
}

BOOL CBO_LOC_3300::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gridMAIN );
	
	if( pMsg->hwnd == pGrid->m_hWnd )
	{
	}
	if (pMsg->message == WM_USER+1907)
	{
		CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3300"), _T("MainGrid"));
		ASSERT(pGrid);
		
		if (pGrid->GetSafeHwnd() == pMsg->hwnd && m_pInfo->m_bUserInfoExportYN == FALSE)
		{
			AfxMessageBox(_T("그리드 엑셀반출 기능을 사용할 권한이 없습니다.\r\n해당 권한은 관리자를 통해서만 부여받을 수 있습니다."), MB_ICONWARNING);
			return TRUE;
		}
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);

	EFS_END

	return FALSE;
}

VOID CBO_LOC_3300::OnMouseDowngridMAIN( short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gridMAIN );
	if( !pGrid->GetMouseRow() && pGrid->GetMouseCol() && 1 == Button )
		SortEx( pGrid->GetCol(), x, y );

	EFS_END
}

VOID CBO_LOC_3300::SortEx( INT col, long x, long y )
{
	EFS_BEGIN

	const INT menuCnt = 2;
	CString menuAlias[ menuCnt ];
	unsigned id[ menuCnt ];
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gridMAIN );
	CString strColAlias = pGrid->GetTextMatrix(0, col);
	if( _T("소속") == strColAlias )
	{
		menuAlias[ 0 ] = _T("소속 코드로 정렬");
		menuAlias[ 1 ] = _T("소속 코드설명값으로 정렬");
		id[ 0 ] = SORT_POSITION_CODE;
		id[ 1 ] = SORT_POSITION_CODE_DESC;
	}
	else if( _T("직급") == strColAlias )
	{
		menuAlias[ 0 ] = _T("직급 코드로 정렬");
		menuAlias[ 1 ] = _T("직급 코드설명값으로 정렬");
		id[ 0 ] = SORT_CLASS_CODE;
		id[ 1 ] = SORT_CLASS_CODE_DESC;
	}
	else
	{
		return;
	}
	
	CRect rect;
	GetWindowRect(&rect);
	
	CMenu menu;
	menu.CreatePopupMenu();
	for( INT i = 0; i < menuCnt; i++ )
		menu.AppendMenu( MF_STRING, id[ i ], menuAlias[ i ] );
	menu.TrackPopupMenu( TPM_LEFTALIGN, rect.left + x, rect.top + y, this );

	EFS_END

}

VOID CBO_LOC_3300::SortExDM( UINT nID )
{
	EFS_BEGIN

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3300_LIST") );
	if( !pDM->GetRowCount() )
		return;
	
	CString alias;
	switch( nID )
	{
		case SORT_POSITION_CODE :
			alias = _T("소속_CODE");
			break;
		case SORT_POSITION_CODE_DESC :
			alias = _T("대출소속정보");
			break;
		case SORT_CLASS_CODE :
			alias = _T("직급_CODE");
			break;
		case SORT_CLASS_CODE_DESC :
			alias = _T("대출직급정보");
			break;
		default :
			return;
	}

	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_BO_LOC_3300") , _T("MainGrid") );
	POSITION pos = pGrid->m_arrColumnInfo.FindIndex( pGrid->GetCol() - 1 );
	CESL_GridColumnInfo *pColumnInfo = ( CESL_GridColumnInfo * )pGrid->m_arrColumnInfo.GetAt( pos );
	
	pDM->SORT( alias, pColumnInfo->IsDesc );
	pGrid->Display();

	EFS_END
}

INT CBO_LOC_3300::SecedeLoanUser()
{
	CBO_LOC_SECEDE Dlg( this, m_pLoanShareManager );
	Dlg.m_sIsUnityLoanService = m_pLoanMangeValue->m_sIsUnityLoanService;
	Dlg.DoModal();

	return 0;
}
//--------------------------------------------------------------------------------}}

HBRUSH CBO_LOC_3300::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

LRESULT CBO_LOC_3300::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CDialog::WindowProc(message, wParam, lParam);
}
