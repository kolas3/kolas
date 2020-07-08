// MNG_CODE_SHELF_02.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_CODE_SHELF_02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_SHELF_02 dialog


CMNG_CODE_SHELF_02::CMNG_CODE_SHELF_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_SHELF_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
		
}
CMNG_CODE_SHELF_02::~CMNG_CODE_SHELF_02()
{

	
}



BOOL CMNG_CODE_SHELF_02::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_CODE_SHELF_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_SHELF_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_SHELF_02, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_SHELF_02)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_INIT, OnInit)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_SHELF_02 message handlers

BOOL CMNG_CODE_SHELF_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if( InitESL_Mgr( _T("MNG_CODE_SHELF_02") ) < 0 )
	{
		AfxMessageBox( _T("Error : InitESL_Mgr - MNG_CODE_SHELF_02") );
		return false;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_CODE_SHELF_02::OnSize( UINT nType, INT cx, INT cy ) 
{
	CDialog::OnSize( nType, cx, cy );
	
	// TODO: Add your message handler code here
	
}

// 초기화
VOID CMNG_CODE_SHELF_02::OnInit() 
{
	// TODO: Add your control notification handler code here

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID );

	for( INT i = 0; i < m_rowCnt; i++ )
		SetCellText( _T(""), i + 1, 1 );

	m_rankCnt = 0;
	m_save = true;
}

// 저장
VOID CMNG_CODE_SHELF_02::OnSave() 
{
	// TODO: Add your control notification handler code here
	
	if( !m_save )
	{
		AfxMessageBox( _T("변경 사항이 없습니다.") );
		return;
	}
	
	if( m_rankCnt != m_rowCnt )
	{
		AfxMessageBox( _T("우선 순위를 모두 지정하여 주십시오") );
		return;
	}

	if( !SaveData() )
	{
		AfxMessageBox( _T("저장되었습니다.") );
		OnClose();;
	}
	else
	{
		AfxMessageBox( _T("저장 실패") );
		return;
	}
}

VOID CMNG_CODE_SHELF_02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID );

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_02") );
	pDM->RefreshDataManager( _T("") );
	
	ShowGrid();

	m_rowCnt = pDM->GetRowCount();
	m_rankCnt = 0;
	m_save = false;	
}

BEGIN_EVENTSINK_MAP(CMNG_CODE_SHELF_02, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_CODE_SHELF_02)
	ON_EVENT(CMNG_CODE_SHELF_02, IDC_mngGRID, -600 /* Click */, OnClickmngGRID, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_CODE_SHELF_02::OnClickmngGRID() 
{
	// TODO: Add your control notification handler code here

	if( !m_save )
		return;
	
	INT row, col;
	GetCurrentCell( row, col );

	if( 1 != col )
		return;

	CString rnk = GetCellText();

	if( 0 != _ttoi( rnk ) )
		return;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_02") );
	CString tmp;
	tmp.Format( _T("%d"), ++m_rankCnt );
	pDM->SetCellData( _T("LOAN_PERIOD"), tmp, row - 1 );

	rnk.Format( _T("%d"), m_rankCnt );
	SetCellText( rnk );
}

VOID CMNG_CODE_SHELF_02::GetCurrentCell( INT &row, INT &col )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID );

	row = pGrid->GetRow();
	col = pGrid->GetCol();
}

VOID CMNG_CODE_SHELF_02::SetCellText( CString text, INT row, INT col )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID );
	
	if( -1 != row )
		pGrid->SetRow( row );
	if( -1 != col )
		pGrid->SetCol( col );
	
	pGrid->SetText( text );
}

CString CMNG_CODE_SHELF_02::GetCellText( INT row, INT col )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID );
	
	if( -1 != row )
		pGrid->SetRow( row );
	if( -1 != col )
		pGrid->SetCol( col );

	return pGrid->GetText();
}

VOID CMNG_CODE_SHELF_02::ShowGrid()
{
	ControlDisplay( _T("CM_MNG_CODE_SHELF_02_01"), _T("그리드") );
}

INT CMNG_CODE_SHELF_02::SaveData()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_02") );
	
	const INT fieldCnt = 3;
	CString fieldName[ fieldCnt ] = { _T("CODE"), _T("DESCRIPTION"), _T("LOAN_PERIOD") };
	CString fieldType[ fieldCnt ] = { _T("STRING"), _T("STRING"), _T("NUMERIC") };

	CString data[ fieldCnt ], pk;

	pDM->StartFrame();
	pDM->InitDBFieldData();
	
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( fieldName, fieldCnt, data, i );
		pDM->GetCellData( _T("REC_KEY"), i, pk );

		for( INT j = 0; j < fieldCnt; j++ )
			if( data[ j ].GetLength() )
				pDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], data[ j ] );

		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("배가기준관리"), __WFILE__, __LINE__ ), TRUE );
		pDM->MakeUpdateFrame( _T("CD_CODE_TBL"), _T("REC_KEY"), _T("NUMERIC"), pk );
	}

	pDM->SendFrame();
	pDM->EndFrame();

	return 0;
}

VOID CMNG_CODE_SHELF_02::OnClose() 
{		
	if( 1 == m_openMode )
		CDialog::DestroyWindow();
	else if( 2 == m_openMode )
		CDialog::EndDialog( 1 );
}
HBRUSH CMNG_CODE_SHELF_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

