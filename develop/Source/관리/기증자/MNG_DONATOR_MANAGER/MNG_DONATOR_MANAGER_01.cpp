// MNG_DONATOR_MANAGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_DONATOR_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_DONATOR_MANAGER_01 dialog


CMNG_DONATOR_MANAGER_01::CMNG_DONATOR_MANAGER_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_DONATOR_MANAGER_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_DONATOR_MANAGER_01::~CMNG_DONATOR_MANAGER_01()
{	
	
}

BOOL CMNG_DONATOR_MANAGER_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CMNG_DONATOR_MANAGER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_DONATOR_MANAGER_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMNG_DONATOR_MANAGER_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_DONATOR_MANAGER_01)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_DONATOR_MANAGER_01 message handlers

BOOL CMNG_DONATOR_MANAGER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_DONATOR_MANAGER_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_USER_MANAGER_01") );
		return false;
	}

	CArray< CString, CString >RemoveListArray;
	RemoveListArray.RemoveAll();
	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_DONATOR_MANAGER_01"), _T("그리드") );
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

// DM_MNG_DONATOR_MANAGER_01

VOID CMNG_DONATOR_MANAGER_01::ShowGrid()
{
	ControlDisplay( _T("CM_MNG_DONATOR_MANAGER_01"), _T("그리드") );	
}

INT * CMNG_DONATOR_MANAGER_01::GetCheckedDMRow( INT &checkedRowCnt )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	INT *checkedDMRow, i;
	checkedRowCnt = 0;

	INT rows = pGrid->GetRows();

	pGrid->SetCol( 1 );
	for( i = 1; i < rows; i++ )
	{
		pGrid->SetRow( i );

		if( _T("V") == pGrid->GetText() )
			checkedRowCnt++;
	}

	checkedDMRow = ( INT * )calloc( checkedRowCnt, sizeof( INT ) );

	INT index = 0;
	for( i = 1; i < rows; i++ )
	{
		pGrid->SetRow( i );

		if( _T("V") == pGrid->GetText() )
			checkedDMRow[ index++ ] = i - 1;
	}

	return checkedDMRow;
}

VOID CMNG_DONATOR_MANAGER_01::DeleteData()
{
	INT *checkedDMRow;
	INT checkedRowCnt = 0;

	checkedDMRow = GetCheckedDMRow( checkedRowCnt );

	if( !checkedRowCnt )
	{
		AfxMessageBox( _T("선택된 목록이 없습니다.") );
		return;
	}

	CString notice;
	notice.Format( _T("선택된 %d개의 목록을 삭제하시겠습니까?"), checkedRowCnt );
	if( IDNO == AfxMessageBox( notice, MB_YESNO ) )
	{
		free( checkedDMRow );
		return;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_DONATOR_MANAGER_01") );
	CString pk, query;

	for( INT i = checkedRowCnt - 1; i > -1; i-- )
	{
		pDM->GetCellData( _T("REC_KEY"), checkedDMRow[ i ], pk );
		query.Format( _T("DELETE FROM CO_DONATOR_TBL WHERE REC_KEY = %s;"), pk );
		
		pDM->ExecuteQuery( query, 1 );
		pDM->DeleteRow( checkedDMRow[ i ] );
	}

	AfxMessageBox( _T("삭제되었습니다.") );
	
	ShowGrid();

	free( checkedDMRow );
}


VOID CMNG_DONATOR_MANAGER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	if( NULL == pGrid->GetSafeHwnd() )
		return;

	//pGrid->MoveWindow( 15, 15, cx - 25, cy - 25 );	
	pGrid->MoveWindow(0, 0, cx, cy);
}


HBRUSH CMNG_DONATOR_MANAGER_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}