// MNG_CODE_MANAGER_09.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CODE_MANAGER_09.h"
#include "MNG_CODE_MANAGER_10.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_09 dialog


CMNG_CODE_MANAGER_09::CMNG_CODE_MANAGER_09(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_MANAGER_09)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CMNG_CODE_MANAGER_09::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_CODE_MANAGER_09::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_MANAGER_09)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_MANAGER_09, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_MANAGER_09)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bINSERT, OnbINSERT)
	ON_BN_CLICKED(IDC_bUPDATE, OnbUPDATE)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_09 message handlers

BOOL CMNG_CODE_MANAGER_09::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CODE_MANAGER_09") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CODE_MANAGER_09") );
		return false;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_CODE_MANAGER_09::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	SearchData();
	
	// TODO: Add your message handler code here
	
}

// DM_MNG_CODE_MANAGER_09
// CM_MNG_CODE_MANAGER_09

VOID CMNG_CODE_MANAGER_09::SearchData()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_09") );
	pDM->RefreshDataManager( _T("") );
	ShowGrid();
}

VOID CMNG_CODE_MANAGER_09::ShowGrid()
{
	ControlDisplay( _T("CM_MNG_CODE_MANAGER_09"), _T("그리드") );	
}

// 입력
VOID CMNG_CODE_MANAGER_09::OnbINSERT() 
{
	// TODO: Add your control notification handler code here
	
	CMNG_CODE_MANAGER_10 dlg( this );
	dlg.m_type = 1;
	if( IDOK == dlg.DoModal() )
		ShowGrid();
}

// 수정
VOID CMNG_CODE_MANAGER_09::OnbUPDATE() 
{
	// TODO: Add your control notification handler code here

	CMNG_CODE_MANAGER_10 dlg( this );
	dlg.m_type = 2;
	if( IDOK == dlg.DoModal() )
		ShowGrid();
}

// 삭제
VOID CMNG_CODE_MANAGER_09::OnbDELETE() 
{
	// TODO: Add your control notification handler code here
	
	INT cnt = 0;
	INT *checkedRow = GetCheckedRowInfo( cnt );

	if( !cnt )
	{
		AfxMessageBox( _T("선택 목록이 없습니다.") );
		return;
	}

	CString notice;
	notice.Format( _T("%d 건의 자료를 삭제하시겠습니까?"), cnt );
	if( IDNO == AfxMessageBox( notice, MB_YESNO ) )
		return;

	if( !DeleteData( checkedRow, cnt ) )
	{
		AfxMessageBox( _T("삭제 완료~!") );
		ShowGrid();
	}
	
	free( checkedRow );
}

INT CMNG_CODE_MANAGER_09::GetNextClassNum()
{
	CString num, query;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_09") );
	query.Format( _T("SELECT MAX( CLASS_CODE ) FROM CD_CLASS_CODE_TBL") );
	pDM->GetOneValueQuery( query, num );

	return _ttoi( num ) + 1;
}

INT CMNG_CODE_MANAGER_09::SaveData( CString data[], INT type, INT row )
{
	const fieldCnt = 5;
	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"), _T("CLASS_CODE"), _T("CLASS_NAME"), _T("USE_YN"), _T("FIX_YN") };
	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_09") );
		
	pDM->StartFrame();
	pDM->InitDBFieldData();

	if( 1 == type )
		pDM->MakeRecKey( data[ 0 ] );
	else if( 2 == type )
		pDM->GetCellData( _T("REC_KEY"), row, data[ 0 ] );
	
	for( INT i = 0; i < fieldCnt; i++ )
		if( data[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], data[ i ] );

	switch( type )
	{
		case 1 :
			pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("코드관리"), __WFILE__, __LINE__ ), TRUE );
			pDM->MakeInsertFrame( _T("CD_CLASS_CODE_TBL") );
			pDM->SetCellData( fieldName, fieldCnt, data, -1 );
			break;
		case 2 :
			pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("코드관리"), __WFILE__, __LINE__ ), TRUE );
			pDM->MakeUpdateFrame( _T("CD_CLASS_CODE_TBL"), _T("REC_KEY"), _T("NUMERIC"), data[ 0 ], row );
			break;
		default :
			return -1;
			break;
	}

	pDM->SendFrame();
	pDM->EndFrame();
	
	return 0;
}

INT * CMNG_CODE_MANAGER_09::GetCheckedRowInfo( INT &cnt )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_09") );

	cnt = 0;

	INT dmRowCnt = pDM->GetRowCount();
	if( !dmRowCnt )
		return NULL;

	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	CArray< INT, INT > rows;
	pGrid->SetCol( 1 );
	for( INT i = 0; i < dmRowCnt; i++ )
	{
		pGrid->SetRow( i + 1 );

		if( _T("V") == pGrid->GetText() )
			rows.Add( i );
	}

	cnt = rows.GetSize();
	INT *checkedRow = ( INT * )calloc( cnt, sizeof( INT ) );
	for( i = 0; i < cnt; i++ )
		checkedRow[ i ] = rows.GetAt( i );

	return checkedRow;
}

VOID CMNG_CODE_MANAGER_09::GetCurrentData( CString data[], INT row )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_09") );

	CString alias[ 4 ] = {
		_T("CLASS_CODE"), _T("CLASS_NAME"), _T("USE_YN"), _T("FIX_YN") };

	pDM->GetCellData( alias, 4, data, row );
}

INT CMNG_CODE_MANAGER_09::DeleteData( INT *row, INT cnt )
{
	CString query, pk;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_09") );
	
	for( INT i = cnt - 1; i > -1; i-- )
	{
		pDM->GetCellData( _T("REC_KEY"), row[ i ], pk );
		
		query.Format( _T("DELETE FROM CD_CLASS_CODE_TBL WHERE REC_KEY = '%s';"), pk );
		pDM->ExecuteQuery( query, 1 );

		query.Format( _T("DELETE FROM CD_CODE_TBL WHERE CLASS = '%s';"), pk );
		pDM->ExecuteQuery( query, 1 );

		pDM->DeleteRow( row[ i ] );
	}

	return 0;
}
