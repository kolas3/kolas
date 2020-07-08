// MNG_USER_MANAGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_USER_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_01 dialog


CMNG_USER_MANAGER_01::CMNG_USER_MANAGER_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_USER_MANAGER_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CMNG_USER_MANAGER_01::Create(CWnd* pParentWnd)
{	
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd); 

	EFS_END
	return FALSE;

}	


VOID CMNG_USER_MANAGER_01::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_USER_MANAGER_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMNG_USER_MANAGER_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_USER_MANAGER_01)
	ON_WM_SIZE()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_01 message handlers

BOOL CMNG_USER_MANAGER_01::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_USER_MANAGER_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_USER_MANAGER_01") );
		return false;
	}

	m_pDM_MenuGroup = FindDM( _T("DM_MNG_USER_METHOD_GROUP"));
	m_pDM_Menu		= FindDM( _T("DM_MNG_USER_P_MENU"));
	m_pDM_ToolBar	= FindDM( _T("DM_MNG_USER_P_TOOLBAR"));

	CESL_ControlMgr *pCM = FindCM( _T("CM_MNG_USER_MANAGER_01") );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("그리드") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );


	const INT gridCnt = 1;
	CString gridAlias[ gridCnt ] = { _T("그리드") };
	CArray< CString, CString >removeListArray;
	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_USER_MANAGER_01"), gridAlias[ i ] );
		if( !pGrid )
			return -1;

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &removeListArray );

		if( !m_pInfo->m_bUserInfoExportYN )
		{
			pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
		}
		else
		{
			pGrid->m_bPrivacyPrintLogRecord = TRUE;
			pGrid->m_strPrintInfo = _T("사용자 관리");
		}
	}


	InitToolBar();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;
}

VOID CMNG_USER_MANAGER_01::DeleteData()
{
	EFS_BEGIN

	INT *dmRow, checkedRowCnt, tmp;
	dmRow = GetCheckedDMRow( checkedRowCnt, tmp );

	TCHAR userPermition = GetUserPermition();
	CString error;

	if( 'A' != userPermition && 'M' != userPermition && 'S' != userPermition)
	{
		error.Format( _T("%s 님은 사용자 입력 및 삭제에 대한 권한이 없습니다."), GetUserID() );
		AfxMessageBox( error );
		return ;
	}	
	

	if( !checkedRowCnt )
	{
		AfxMessageBox( _T("선택된 사용자가 없습니다.") );
		return;
	}
	
	CString message;
	message.Format( _T(" 선택하신 %d 명의 사용자를 삭제하시겠습니까?"), checkedRowCnt );

	INT ids = AfxMessageBox( message, MB_YESNO );
	if( IDYES == ids )
		if( DeleteUser( dmRow, checkedRowCnt ) )
			AfxMessageBox( _T("Error : DeleteUser( INT * dmRow, INT checkedRowCnt )") );
		
	free( dmRow );

	if( IDYES == ids )
	{
		AfxMessageBox( _T("삭제되었습니다.") );
	}
	else
		return;
	
	ShowGrid();	

	EFS_END
}

CString CMNG_USER_MANAGER_01::MakeSelectQuery(CString condition)
{
	EFS_BEGIN

	CString where;
	where.Format( _T(" DIVISION_NAME = '%s' AND STATUS = 'Y'"), condition );

	return where;

	EFS_END
	return _T("");

}

BEGIN_EVENTSINK_MAP(CMNG_USER_MANAGER_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_USER_MANAGER_01)
	ON_EVENT(CMNG_USER_MANAGER_01, IDC_mngGRID_1, 71 /* EnterCell */, OnEnterCellmngGRID1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_USER_MANAGER_01::OnEnterCellmngGRID1() 
{
	EFS_BEGIN

	// TODO: Add your control notification handler code here

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );
	if( !pDM->GetRowCount() )
		return;
	
	INT row, col;
	
	GetCurrentGridCell( row, col );
	SetGridReverse( row - 1 );

	EFS_END
}

VOID CMNG_USER_MANAGER_01::GetCurrentGridCell( INT &row, INT &col )
{
	EFS_BEGIN

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	row = pGrid->GetRow();
	col = pGrid->GetCol();

	EFS_END
}

INT CMNG_USER_MANAGER_01::SendControlData( CString alias, CString data )
{
	EFS_BEGIN

	SetControlData( _T("CM_MNG_USER_MANAGER_01"), alias, data, -1, 0 );

	return 0;

	EFS_END
	return -1;

}

INT * CMNG_USER_MANAGER_01::GetCheckedDMRow( INT &rowCnt, INT &idx, INT type )
{
	EFS_BEGIN

	EFS_BEGIN

	INT *dmRow;
	rowCnt = 0;
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	INT dmRowCnt = GetDMRowCount();
	for( INT i = 0; i < dmRowCnt; i ++ )
		if( _T("V") == pGrid->GetTextMatrix( i + 1, 1 ) )
			rowCnt++;

	dmRow = ( INT * )calloc( rowCnt, sizeof( INT ) );

	INT index = 0;
	for( i = 0; i < dmRowCnt; i++ )
		if( _T("V") == pGrid->GetTextMatrix( i + 1, 1 ) )
			dmRow[ index++ ] = i;

	if( !rowCnt && type )
	{
		rowCnt = dmRowCnt;
		dmRow = ( INT * )calloc( dmRowCnt, sizeof( INT ) );

		for( i = 0; i < dmRowCnt; i++ )
			dmRow[ i ] = i;
		idx = pGrid->GetRow() - 1;
	}
	
	
	return dmRow;

	EFS_END
	return NULL;


	EFS_END
	return NULL;

}

INT CMNG_USER_MANAGER_01::GetDMRowCount()
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );
	
	return pDM->GetRowCount();

	EFS_END
	return -1;

}

INT CMNG_USER_MANAGER_01::DeleteUser( INT *dmRow, INT checkedRowCnt )
{
	EFS_BEGIN	

	CString today = GetTodaybyString();
	CString pk;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );

	pDM->StartFrame();
	pDM->InitDBFieldData();

	CString sUserID;

	CString* pStrTmpData;

	if(checkedRowCnt > 0)
		pStrTmpData = new CString[checkedRowCnt*3];
	
	for( INT i = 0; i < checkedRowCnt; i++ )
	{
		pDM->GetCellData( _T("REC_KEY"), dmRow[ i ], pk );
		pDM->GetCellData( _T("ID"), dmRow[ i ], sUserID );

		pStrTmpData[(i*3)] = pk;
		pStrTmpData[(i*3)+1] = sUserID;
		pDM->GetCellData( _T("NAME"), dmRow[ i ], pStrTmpData[(i*3)+2] );
		
		pDM->MakeDeleteFrame( _T("MN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , pk );

		pDM->MakeDeleteFrame( _T("ESL_MENU_METHOD_TBL") , _T("USER_ID") , _T("STRING") , sUserID );
		
		CString query;
		query.Format(_T("DELETE FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS ='대출자접근권한' AND VALUE_NAME IN ( SELECT ID FROM MN_USER_TBL WHERE REC_KEY=%s );"),pk);
		pDM->AddFrame(query);
	}

	pDM->SendFrame();
	
	pDM->EndFrame();


	for( i = checkedRowCnt; i > 0; i-- )
		pDM->DeleteRow( dmRow[ i - 1 ] );

	pDM = FindDM( _T("DM_MNG_USER_MANAGER_AUTH_LOG") );
	pDM->StartFrame();

	for(i = 0; i < checkedRowCnt; i++ )
	{
		CString strQuery;
		strQuery.Format(
			_T("INSERT INTO MN_USER_AUTH_LOG_TBL(REC_KEY, TYPE, WORKER_IP, WORK_DATE, WORKER_NAME, WORKER_ID, USER_KEY, FIRST_WORK, WORKER_SYS, WORKER_DO ) ")
			_T("VALUES (ESL_SEQ.NextVal, 'D', '%s', SYSDATE, '%s', '%s', '%s', '%s', '%s', '%s' ) ;"), 
			m_pInfo->LOCAL_IP, m_pInfo->USER_NAME,m_pInfo->USER_ID, pStrTmpData[(i*3)],GetWorkLogMsg( _T("유저관리"), __WFILE__, __LINE__ ) ,_T("KOLASIII") , _T("사용자 삭제 : ")+pStrTmpData[(i*3)+1]);
		
		pDM->AddFrame(strQuery);
	}
	pDM->SendFrame();
	pDM->EndFrame();


	if(pStrTmpData)
	{
		delete [] pStrTmpData;
		pStrTmpData = NULL;
	}

	return 0;

	EFS_END
	return -1;

}

CTime CMNG_USER_MANAGER_01::GetToDay()
{
	CTime day;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );
	day = pDM->GetDBTime();

	return day;
}

CString CMNG_USER_MANAGER_01::GetTodaybyString()
{
	EFS_BEGIN

	CTime day = GetToDay();
	CString today;
	today.Format( _T("%d/%d/%d"), day.GetYear(), day.GetMonth(), day.GetDay() );

	return today;

	EFS_END
	return _T("");

}

VOID CMNG_USER_MANAGER_01::ShowGrid( bool bIsCombobChange )
{
	EFS_BEGIN

	INT ids = CodeToDesc();
	if( ids < 0 )
		return ;

	ids = ControlDisplay( _T("CM_MNG_USER_MANAGER_01"), _T("그리드") );

	CString error;
	if( ids )
	{
		error.Format( _T(" Error %d : ControlDisplay( CString cmAlias, CString ctrlAlias )") );
		AfxMessageBox( error );
		return;
	}

	EFS_END
}

CString CMNG_USER_MANAGER_01::GetCurrentUserID( INT DMRow )
{
	EFS_BEGIN

	CString id;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );
	pDM->GetCellData( _T("ID"), DMRow, id );

	return id;

	EFS_END
	return _T("");

}


VOID CMNG_USER_MANAGER_01::RefreshMenuData( CString id )
{
	EFS_BEGIN

	UpdateData();

	CString strOption;
	strOption.Format( _T("FLAG = 'U' AND USER_ID = '%s'"), id );
	CLocCommonAPI::RefreshDataManager( this , m_pDM_Menu , strOption );

	EFS_END
}



VOID CMNG_USER_MANAGER_01::SetGridReverse( INT row )
{
	EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM( _T("CM_MNG_USER_MANAGER_01") );
	CESL_Grid *pGrid = ( CESL_Grid * )pCM->FindControl( _T("그리드") );
	pGrid->SetReverse( row );	

	EFS_END
}


VOID CMNG_USER_MANAGER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	

	if( GetSafeHwnd() == NULL ) return;

	CWnd *pWnd2;
	CRect WndRect1,WndRect2;

	pWnd2 = GetDlgItem(IDC_mngGRID_1);
	if( pWnd2->GetSafeHwnd() == NULL ) return;

	pWnd2->MoveWindow( 0 , 0 , cx , cy );



	EFS_END
}

INT CMNG_USER_MANAGER_01::SearchUser()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_MNG_USER_MANAGER_01");

	CString sWhere;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );

	if( CLocCommonAPI::RefreshDataManager( this , pDM , sWhere ) < 0 )
		return -1;

	ShowGrid();

	return 0;

	EFS_END
	return -1;

}

VOID CMNG_USER_MANAGER_01::OnMove(INT x, INT y) 
{
	EFS_BEGIN

	CDialog::OnMove(x, y);
	
	

	EFS_END
}

INT CMNG_USER_MANAGER_01::CodeToDesc()
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );

	CString sPermition;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("PERMITION") , i , sPermition );

		if( sPermition.Compare(_T("A")) == 0 )
			sPermition = _T("관리자");
		else if( sPermition.Compare(_T("M")) == 0 )
			sPermition = _T("관리자");
		else if( sPermition.Compare(_T("U")) == 0 )
			sPermition = _T("일반사용자");
		else if( sPermition.Compare(_T("S")) == 0 )
			sPermition = _T("통합관리자");

		pDM->SetCellData( _T("PERMITION") , sPermition , i );
	}

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("직원부서") , _T("DIVISION_NAME") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("직원직급") , _T("CLASS_NAME") );
	
	return 0;
	

	EFS_END
	return -1;

}


TCHAR CMNG_USER_MANAGER_01::GetUserPermition()
{
	EFS_BEGIN

	CString query, result;
	query.Format( _T("SELECT PERMITION FROM MN_USER_TBL WHERE ID = '%s'"), GetUserID() );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );
	CLocCommonAPI::GetOneValueQuery( this , pDM , query , result );
	
	TCHAR permition;
	result.MakeUpper();
	wmemcpy( &permition, &result.GetBuffer( 0 )[ 0 ], 1 );
	
	return permition;

	EFS_END

	return ' ';
}

INT CMNG_USER_MANAGER_01::InitToolBar()
{
	EFS_BEGIN

	CString strAlias;
	if		(this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("S")) == 0)
	{

	}
	else if (this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("A")) == 0)
	{
		
	}
	else if (this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("U")) == 0)	
	{				
		strAlias = _T("API인증키");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strAlias, TRUE);
		strAlias = _T("삭제");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strAlias, TRUE);
		strAlias = _T("등록");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strAlias, TRUE);
		strAlias = _T("개인정보 처리로그");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strAlias, TRUE);
	}

	EFS_END
	return -1;

}

INT CMNG_USER_MANAGER_01::SearchCurrentUser()
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );

	CString sWhere;
	sWhere.Format( _T("ID = '%s'") , GetUserID() );

	if( pDM->RefreshDataManager( sWhere ) < 0 )
		return -1;
	
	ShowGrid(TRUE);

	return 0;

	EFS_END

	return -1;
}