// MNG_LIB_CENTER_MANAGER_02.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_LIB_CENTER_MANAGER_02.h"
#include "MNG_LIB_CENTER_MANAGER_03.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_02 dialog


CMNG_LIB_CENTER_MANAGER_02::CMNG_LIB_CENTER_MANAGER_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_CENTER_MANAGER_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_LIB_CENTER_MANAGER_02::~CMNG_LIB_CENTER_MANAGER_02()
{
	
}
BOOL CMNG_LIB_CENTER_MANAGER_02::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_LIB_CENTER_MANAGER_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_CENTER_MANAGER_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_CENTER_MANAGER_02, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_CENTER_MANAGER_02)
	ON_BN_CLICKED(IDC_bINSERT, OnbINSERT)
	ON_BN_CLICKED(IDC_bUPDATE, OnbUPDATE)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_02 message handlers

BOOL CMNG_LIB_CENTER_MANAGER_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_LIB_CENTER_MANAGER_02") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_LIB_CENTER_MANAGER_02") );
		return false;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

VOID CMNG_LIB_CENTER_MANAGER_02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	SearchData();
}

// 입력
VOID CMNG_LIB_CENTER_MANAGER_02::OnbINSERT() 
{
	// TODO: Add your control notification handler code here
	
	CMNG_LIB_CENTER_MANAGER_03 dlg( this );
	dlg.m_type = 1;
	dlg.DoModal();
}

// 수정
VOID CMNG_LIB_CENTER_MANAGER_02::OnbUPDATE() 
{
	// TODO: Add your control notification handler code here

	CMNG_LIB_CENTER_MANAGER_03 dlg( this );
	dlg.m_type = 2;
	dlg.DoModal();
}

INT CMNG_LIB_CENTER_MANAGER_02::SaveData( CString data[], INT type, INT row )
{
	const INT fieldCnt = 6;
	
	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),
		_T("CODE"),
		_T("NAME"),
		_T("IP"),
		_T("REMARK"),
		_T("FIRST_WORK") };

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_02") );

	switch( type )
	{
		case 1 :
			pDM->MakeRecKey( data[ 0 ] );
			break;
		case 2 :
			pDM->GetCellData( _T("REC_KEY"), row, data[ 0 ] );
			fieldName[ 5 ].Format( _T("LAST_WORK") );
			break;
		default :
			return -1;
			break;
	}

	data[ 5 ] = GetWorkLogMsg( _T("도서관센터관리"), __WFILE__, __LINE__ );

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( data[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], data[ i ] );

	switch( type )
	{
		case 1 :
			pDM->MakeInsertFrame( _T("CO_LOCAL_LIBRARY_TBL") );
			pDM->SetCellData( fieldName, fieldCnt, data, -1 );
			break;
		case 2 :
			pDM->MakeUpdateFrame( _T("CO_LOCAL_LIBRARY_TBL"), _T("REC_KEY"), _T("NUMERIC"), data[ 0 ], row );
			break;
	}

	pDM->SendFrame();
	pDM->EndFrame();
	
	return 0;
}

VOID CMNG_LIB_CENTER_MANAGER_02::ShowGrid()
{
	ControlDisplay( _T("CM_MNG_LIB_CENTER_MANAGER_02"), _T("그리드") );
}

VOID CMNG_LIB_CENTER_MANAGER_02::SearchData()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_02") );
	pDM->RefreshDataManager( _T("") );

	ShowGrid();
}

INT CMNG_LIB_CENTER_MANAGER_02::CheckLibCode(CString code)
{
	CString query, cnt;
	query.Format( _T("SELECT COUNT( * ) FROM CO_LOCAL_LIBRARY_TBL WHERE CODE = '%s'"), code );

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_02") );
	pDM->GetOneValueQuery( query, cnt );
	
	return _ttoi( cnt );
}

INT * CMNG_LIB_CENTER_MANAGER_02::GetCheckedRowInfo( INT &cnt )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_02") );
	INT rowCnt = pDM->GetRowCount();
	cnt = 0;

	if( !rowCnt )
		return NULL;

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	CArray< INT, INT > rows;

	pGrid->SetCol( 1 );
	for( INT i = 0; i < rowCnt; i++ )
	{
		pGrid->SetRow( i + 1 );

		if( _T("V") == pGrid->GetText() )
			rows.Add( i );
	}

	cnt = rows.GetSize();
	INT *checkedRow = ( INT * )calloc( cnt, sizeof( INT ) );
	for( i = 0; i < cnt; i++ )
		checkedRow[ i ] = rows.GetAt( i );

	pGrid->SetRow( row );
	pGrid->SetCol( col );

	return checkedRow;
}

INT CMNG_LIB_CENTER_MANAGER_02::GetCurrentData( CString data[], INT row )
{
	const INT cnt = 4;
	CString alias[ cnt ] = {
		_T("CODE"), _T("NAME"), _T("IP"), _T("REMARK") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_02") );
	pDM->GetCellData( alias, cnt, data, row );
		
	return 0;
}

// 삭제
VOID CMNG_LIB_CENTER_MANAGER_02::OnbDELETE() 
{
	// TODO: Add your control notification handler code here

	INT cnt = 0;
	INT *rows = GetCheckedRowInfo( cnt );

	if( !cnt )
	{
		AfxMessageBox( _T("선택 목록이 없습니다.") );
		return;
	}

	CString notice;
	notice.Format( _T("선택된 %d개의 목록을 삭제하시겠습니까?"), cnt );
	if( IDYES == AfxMessageBox( notice, MB_YESNO ) )
	{
		DeleteData( rows, cnt );
		ShowGrid();
		AfxMessageBox( _T("삭제되었습니다.") );
	}
		
	free( rows );	
}

INT CMNG_LIB_CENTER_MANAGER_02::DeleteData( INT *rows, INT cnt )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_02") );
	
	CString query, pk;
	for( INT i = cnt - 1; i > -1; i-- )
	{
		pDM->GetCellData( _T("REC_KEY"), rows[ i ], pk );
		query.Format( _T("DELETE FROM CO_LOCAL_LIBRARY_TBL WHERE REC_KEY = '%s';"), pk );
		pDM->ExecuteQuery( query, 1 );
		pDM->DeleteRow( rows[ i ] );
	}
	
	return 0;
}


HBRUSH CMNG_LIB_CENTER_MANAGER_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
