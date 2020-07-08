// MNG_MNGTBL_MANAGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_MNGTBL_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_MNGTBL_MANAGER_01 dialog


CMNG_MNGTBL_MANAGER_01::CMNG_MNGTBL_MANAGER_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_MNGTBL_MANAGER_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_MNGTBL_MANAGER_01::~CMNG_MNGTBL_MANAGER_01()
{
	
}
BOOL CMNG_MNGTBL_MANAGER_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	

VOID CMNG_MNGTBL_MANAGER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_MNGTBL_MANAGER_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_MNGTBL_MANAGER_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_MNGTBL_MANAGER_01)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_MNGTBL_MANAGER_01 message handlers

BOOL CMNG_MNGTBL_MANAGER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	if( InitESL_Mgr( _T("MNG_MNGTBL_MANAGER_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_MNGTBL_MANAGER_01") );
		return false;
	}

	CArray< CString, CString >RemoveListArray;
	RemoveListArray.RemoveAll();
	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_MNGTBL_MANAGER_01"), _T("그리드") );
	if( pGrid )
	{
		//RemoveListArray.Add( _T("") );

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &RemoveListArray );
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_MNGTBL_MANAGER_01::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

VOID CMNG_MNGTBL_MANAGER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	if( NULL == pGrid->GetSafeHwnd() )
		return;

	//pGrid->MoveWindow( 15, 15, cx - 25, cy - 25 );
	pGrid->MoveWindow(0, 0, cx, cy);
}

// CM_MNG_MNGTBL_MANAGER_01
// DM_MNG_MNGTBL_MANAGER_01


VOID CMNG_MNGTBL_MANAGER_01::ShowGrid()
{
	ControlDisplay( _T("CM_MNG_MNGTBL_MANAGER_01"), _T("그리드") );
}

VOID CMNG_MNGTBL_MANAGER_01::DeleteData()
{
	INT cnt = 0;
	INT *checkedDMRow = GetCheckedRowInfo( cnt );

	if( !cnt )
	{
		AfxMessageBox( _T("선택된 목록이 없습니다") );
		return;
	}
	
	CString notice;
	notice.Format( _T("%d 건의 자료를 삭제하시겠습니까?"), cnt );
	if( IDNO == AfxMessageBox( notice, MB_YESNO ) )
		return;

	if( !DeleteData( checkedDMRow, cnt ) )
	{
		AfxMessageBox( _T("삭제 완료~!") );
		ShowGrid();
	}

	free( checkedDMRow );
}

INT CMNG_MNGTBL_MANAGER_01::SearchData( CString where )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGTBL_MANAGER_01") );
	pDM->RefreshDataManager( where );

	return pDM->GetRowCount();
}

INT * CMNG_MNGTBL_MANAGER_01::GetCheckedRowInfo( INT &cnt )
{
	cnt = 0;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGTBL_MANAGER_01") );
	INT rowCnt = pDM->GetRowCount();
	if( !rowCnt )
		return NULL;

	CArray< INT, INT > rows;
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

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

INT CMNG_MNGTBL_MANAGER_01::DeleteData( INT *row, INT cnt )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MNGTBL_MANAGER_01") );
	CString pk, query;;
	for( INT i = cnt - 1; i > -1; i-- )
	{
		pDM->GetCellData( _T("REC_KEY"), row[ i ], pk );
		query.Format( _T("DELETE FROM ESL_MANAGE_TBL WHERE REC_KEY = '%s';"),	pk );
		pDM->ExecuteQuery( query, 1 );
		pDM->DeleteRow( row[ i ] );
	}

	return 0;
}


HBRUSH CMNG_MNGTBL_MANAGER_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}