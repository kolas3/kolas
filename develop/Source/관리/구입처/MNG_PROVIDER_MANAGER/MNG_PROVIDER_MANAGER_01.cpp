// MNG_PROVIDER_MANAGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_PROVIDER_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_PROVIDER_MANAGER_01 dialog


CMNG_PROVIDER_MANAGER_01::CMNG_PROVIDER_MANAGER_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_PROVIDER_MANAGER_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_PROVIDER_MANAGER_01::~CMNG_PROVIDER_MANAGER_01()
{
	
}

BOOL CMNG_PROVIDER_MANAGER_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	

VOID CMNG_PROVIDER_MANAGER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_PROVIDER_MANAGER_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMNG_PROVIDER_MANAGER_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_PROVIDER_MANAGER_01)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_PROVIDER_MANAGER_01 message handlers

BOOL CMNG_PROVIDER_MANAGER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_PROVIDER_MANAGER_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_PROVIDER_MANAGER_01") );
		return false;
	}
	
	CArray< CString, CString >RemoveListArray;
	RemoveListArray.RemoveAll();
	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_PROVIDER_MANAGER_01"), _T("그리드") );
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


// DM_MNG_PROVIDER_MANAGER_01

VOID CMNG_PROVIDER_MANAGER_01::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	SearchProviderCode();
}

VOID CMNG_PROVIDER_MANAGER_01::SearchProviderCode()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_PROVIDER_MANAGER_01") );
	pDM->RefreshDataManager( _T("") );

	ShowGrid();
}

INT CMNG_PROVIDER_MANAGER_01::ShowGrid()
{
	ControlDisplay( _T("CM_MNG_PROVIDER_MANAGER_01"), _T("그리드") );
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid *)GetDlgItem( IDC_mngGRID_1 );
	
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		if( _T("Y") == GetCellText( i, 2 ) )
			SetCellText( i, 2, _T("V") );
		else
			SetCellText( i, 2, _T("") );

		if( _T("Y") == GetCellText( i, 3 ) )
			SetCellText( i, 3, _T("V") );
		else
			SetCellText( i, 3, _T("") );
	}
	
	return 0;
}

CString CMNG_PROVIDER_MANAGER_01::GetCellText(INT row, INT col)
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid *)GetDlgItem( IDC_mngGRID_1 );
	pGrid->SetRow( row );
	pGrid->SetCol( col );
		
	return pGrid->GetText();
}

VOID CMNG_PROVIDER_MANAGER_01::SetCellText(INT row, INT col, CString text)
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid *)GetDlgItem( IDC_mngGRID_1 );
	pGrid->SetRow( row );
	pGrid->SetCol( col );

	pGrid->SetText( text );
}

BEGIN_EVENTSINK_MAP(CMNG_PROVIDER_MANAGER_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_PROVIDER_MANAGER_01)
	ON_EVENT(CMNG_PROVIDER_MANAGER_01, IDC_mngGRID_1, -600 /* Click */, OnClickmngGRID1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_PROVIDER_MANAGER_01::OnClickmngGRID1() 
{
	// TODO: Add your control notification handler code here
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid *)GetDlgItem( IDC_mngGRID_1 );
	
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();
	
	switch( col )
	{
		case 2 :
			SetActiveValue( row, col );
			break;
		case 3 :
			SetDefaultValue( row, col );
			break;
		default :
			return;
			break;
	}
}

VOID CMNG_PROVIDER_MANAGER_01::SetActiveValue( INT row, INT col )
{
	if( _T("V") == 	GetCellText( row, col ) )
		SetCellText( row, col, _T("") );
	else
		SetCellText( row, col, _T("V") );
}

VOID CMNG_PROVIDER_MANAGER_01::SetDefaultValue( INT row, INT col )
{
	if( _T("V") == GetCellText( row, col ) )
	{
		SetCellText( row, col, _T("") );
		return;
	}

	SetCellText( row, col, _T("V") );

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid *)GetDlgItem( IDC_mngGRID_1 );

	for( INT i = 1; i < pGrid->GetRows(); i++ )
		if( row == i )
			continue;
		else
			SetCellText( i, col, _T("") );
}


VOID CMNG_PROVIDER_MANAGER_01::SaveModifiedData()
{
	UpdateDataToDM();
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_PROVIDER_MANAGER_01") );
	
	const INT fieldCnt = 3;
	CString fieldAlias[ fieldCnt ] = {
		_T("ACTIVE"), _T("DEFAULT_VALUE"), _T("REC_KEY") };
	CString fieldType[ fieldCnt ] = {
		_T("STRING"), _T("STRING"), _T("NUMERIC") };
	CString data[ fieldCnt ];

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( fieldAlias, fieldCnt, data, i );

		for( INT j = 0; j < fieldCnt; j++ )
			pDM->AddDBFieldData( fieldAlias[ j ], fieldType[ j ], data[ j ] );
		
		pDM->MakeUpdateFrame( _T("CO_PROVIDER_TBL"), _T("REC_KEY"), _T("NUMERIC"), data[ 2 ] );
	}
	
	INT ids = pDM->SendFrame();
	CString error;
	if( ids < 0 )
	{
		error.Format( _T("Error %d : SendFrame()"), ids );
		AfxMessageBox( error );
		return ;
	}

	pDM->EndFrame();
	
	AfxMessageBox( _T("변경사항이 수정되었습니다.") );
	ShowGrid();
}

INT CMNG_PROVIDER_MANAGER_01::UpdateDataToDM()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_PROVIDER_MANAGER_01") );	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid *)GetDlgItem( IDC_mngGRID_1 );

	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		if( _T("V") == GetCellText( i, 2 ) )
			pDM->SetCellData( _T("ACTIVE"), _T("Y"), i - 1 );
		else
			pDM->SetCellData( _T("ACTIVE"), _T("N"), i - 1 );

		if( _T("V") == GetCellText( i, 3 ) )
			pDM->SetCellData( _T("DEFAULT_VALUE"), _T("Y"), i - 1 );
		else
			pDM->SetCellData( _T("DEFAULT_VALUE"), _T("N"), i - 1 );
	}
	
	return 0;
}

VOID CMNG_PROVIDER_MANAGER_01::DeleteData()
{
	INT *dmRow, rowCnt;
	dmRow = GetCheckedDMInfo( rowCnt );

	CString notice;
	notice.Format( _T("총 %d 건의 코드를 삭제하시겠습니까?"), rowCnt );
	if( IDNO == AfxMessageBox( notice, MB_YESNO ) )
	{
		free( dmRow );
		return;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_PROVIDER_MANAGER_01") );
	CString pk, query;;
	for( INT i = rowCnt - 1; i > -1; i-- )
	{
		pDM->GetCellData( _T("REC_KEY"), dmRow[ i ], pk );
		query.Format( _T("DELETE FROM CO_PROVIDER_TBL WHERE REC_KEY = %s;"), pk );
		pDM->ExecuteQuery( query, 1 );
		pDM->DeleteRow( dmRow[ i ] );
	}

	free( dmRow );
	AfxMessageBox( _T("삭제되었습니다.") );
	ShowGrid();
}

INT * CMNG_PROVIDER_MANAGER_01::GetCheckedDMInfo( INT &rowCnt )
{
	INT *dmRow, i;
	rowCnt = 0;

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid *)GetDlgItem( IDC_mngGRID_1 );
	INT rows = pGrid->GetRows();

	for( i = 1; i < rows; i++ )
		if( _T("V") == GetCellText( i, 1 ) )
			rowCnt++;

	dmRow = ( INT * )calloc( rowCnt, sizeof( INT ) );

	INT index = 0;
	for( i = 1; i < rows; i++ )
		if( _T("V") == GetCellText( i, 1 ) )
			dmRow[ index++ ] = i - 1;

	return dmRow;
}

VOID CMNG_PROVIDER_MANAGER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	if( NULL == pGrid->GetSafeHwnd() )
		return;

	pGrid->MoveWindow(0, 0, cx, cy);
	//pGrid->MoveWindow( 15, 15, cx - 25, cy - 25 );	
}

HBRUSH CMNG_PROVIDER_MANAGER_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
