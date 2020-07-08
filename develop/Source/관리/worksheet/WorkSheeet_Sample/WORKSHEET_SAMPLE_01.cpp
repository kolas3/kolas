// WORKSHEET_SAMPLE_01.cpp : implementation file
//

#include "stdafx.h"
#include "WORKSHEET_SAMPLE_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWORKSHEET_SAMPLE_01 dialog


CWORKSHEET_SAMPLE_01::CWORKSHEET_SAMPLE_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CWORKSHEET_SAMPLE_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CWORKSHEET_SAMPLE_01::~CWORKSHEET_SAMPLE_01()
{
	
}
BOOL CWORKSHEET_SAMPLE_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CWORKSHEET_SAMPLE_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWORKSHEET_SAMPLE_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWORKSHEET_SAMPLE_01, CDialog)
	//{{AFX_MSG_MAP(CWORKSHEET_SAMPLE_01)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWORKSHEET_SAMPLE_01 message handlers

BOOL CWORKSHEET_SAMPLE_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if( InitESL_Mgr( _T("MAIN_SAMPLE") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MAIN_SAMPLE") );
		return false;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT * CWORKSHEET_SAMPLE_01::GetCheckedRowInfo(INT &rowCnt)
{
	INT *row;
	
	rowCnt = 0;
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	pGrid->SetCol( 1 );
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );

		if( _T("V") == pGrid->GetText() )
			rowCnt++;
	}

	row = ( INT * )calloc( rowCnt, sizeof( INT ) );
	INT index = 0;
	for( i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );

		if( _T("V") == pGrid->GetText() )
			row[ index++ ] = i - 1;
	}


	return row;
}

VOID CWORKSHEET_SAMPLE_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	if( NULL == pGrid->GetSafeHwnd() )
		return;

	//pGrid->MoveWindow( 15, 15, cx - 25, cy - 25 );
	pGrid->MoveWindow(0, 0, cx, cy);
}

VOID CWORKSHEET_SAMPLE_01::DeleteData()
{
	INT rowCnt;
	INT *row = GetCheckedRowInfo( rowCnt );
	
	if( !rowCnt )
	{
		AfxMessageBox( _T("선택항목이 없습니다.") );
		return;
	}

	CString notice;
	notice.Format( _T("%d건의 자료를 삭제하시겠습니까?"), rowCnt );
	if( IDYES != AfxMessageBox( notice, MB_YESNO ) )
	{
		free( row );
		return;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_MAIN_SAMPLE") );
	CString id, query;
	for( INT i = rowCnt - 1; i > -1 ; i-- )
	{
		pDM->GetCellData( _T("ID"), row[ i ], id );
		query.Format( _T("DELETE FROM ESL_TEST WHERE ID = '%s';"), id );
		pDM->AddFrame( query );
		pDM->DeleteRow( row[ i ] );
	}
	pDM->SendFrame();
	pDM->EndFrame();

	free( row );

	notice.Format( _T("%d 건의 자료를 삭제하였습니다."), rowCnt );
	AfxMessageBox( notice );
	ControlDisplay( _T("CM_MAIN_SAMPLE"), _T("MainGrid") );
}

HBRUSH CWORKSHEET_SAMPLE_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
