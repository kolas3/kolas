// MNG_LIB_CENTER_MANAGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_LIB_CENTER_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_01 dialog


CMNG_LIB_CENTER_MANAGER_01::CMNG_LIB_CENTER_MANAGER_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_CENTER_MANAGER_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CMNG_LIB_CENTER_MANAGER_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_LIB_CENTER_MANAGER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_CENTER_MANAGER_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_CENTER_MANAGER_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_CENTER_MANAGER_01)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_01 message handlers

BOOL CMNG_LIB_CENTER_MANAGER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_LIB_CENTER_MANAGER_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_LIB_CENTER_MANAGER_01") );
		return false;
	}
	
	CArray< CString, CString >RemoveListArray;
	RemoveListArray.RemoveAll();
	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_LIB_CENTER_MANAGER_01"), _T("그리드") );
	if( pGrid )
	{
		//RemoveListArray.Add( _T("") );

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &RemoveListArray );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_LIB_CENTER_MANAGER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	if( NULL == pGrid->GetSafeHwnd() )
		return;

	pGrid->MoveWindow( 0, 0, cx , cy  );	
}

VOID CMNG_LIB_CENTER_MANAGER_01::DeleteData()
{
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
		AfxMessageBox( _T("삭제되었습니다.") );
	}

	free( rows );
	ShowGrid();
}

VOID CMNG_LIB_CENTER_MANAGER_01::ShowGrid()
{
	ControlDisplay( _T("CM_MNG_LIB_CENTER_MANAGER_01"), _T("그리드") );
}

INT * CMNG_LIB_CENTER_MANAGER_01::GetCheckedRowInfo( INT &cnt )
{
	cnt = 0;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_01") );
	INT rowCnt = pDM->GetRowCount();

	if( !rowCnt )
		return NULL;

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	pGrid->SetCol( 1 );
	CArray< INT, INT > rows;
	for( INT i = 0; i < rowCnt; i++ )
	{
		pGrid->SetRow( i + 1 );
		if( _T("V") == pGrid->GetText() )
			rows.Add( i );	
	}

	cnt = rows.GetSize();
	INT *DMRow = ( INT * )calloc( cnt, sizeof( INT ) );
	for( i = 0; i < cnt; i++ )
		DMRow[ i ] = rows.GetAt( i );

	return DMRow;
}

INT CMNG_LIB_CENTER_MANAGER_01::DeleteData( INT *rows, INT cnt )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_01") );
	CString query, pk;

	for( INT i = cnt - 1; i > -1; i-- )
	{
		pDM->GetCellData( _T("REC_KEY"), rows[ i ], pk );
		
		query.Format( _T("DELETE FROM CO_MASTER_LOAN_USER_TBL WHERE REC_KEY = '%s';"), pk );
		pDM->ExecuteQuery( query, 1 );
		pDM->DeleteRow( rows[ i ] );

		query.Format( _T("DELETE FROM CO_LIBRARY_USER_TBL WHERE USER_KEY = '%s';"), pk );
		pDM->ExecuteQuery( query, 1 );
	}
	
	return 0;
}
