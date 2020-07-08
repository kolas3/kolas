// ReceiptNumViwerDlg.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "resource.h"
#include "ReceiptNumViwerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReceiptNumViwerDlg dialog


CReceiptNumViwerDlg::CReceiptNumViwerDlg( CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CReceiptNumViwerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CReceiptNumViwerDlg::~CReceiptNumViwerDlg()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CReceiptNumViwerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReceiptNumViwerDlg)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReceiptNumViwerDlg, CDialog)
	//{{AFX_MSG_MAP(CReceiptNumViwerDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bFIND, OnbFIND)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReceiptNumViwerDlg message handlers

VOID CReceiptNumViwerDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	if( m_receiptNum.IsEmpty() )
	{
		MessageBox( _T("선택된 접수번호가 없습니다."), _T("KOLAS 2 Up"), MB_OK );
		return;
	}
	CDialog::OnOK();
}

VOID CReceiptNumViwerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CReceiptNumViwerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("SM_접수번호보기")) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error!") );
		return false;
	}	
	
	InitSetCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CReceiptNumViwerDlg::OnbFIND() 
{
	// TODO: Add your control notification handler code here
	
	SearchReceiptNum();
}

VOID CReceiptNumViwerDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	SearchReceiptNum();
	ViewGrid( 0 );	
}

VOID CReceiptNumViwerDlg::InitSetCtrls()
{
	// 그리드의 0번째 컬럼을 안보이게 숨기기
	const INT cnt = 2;
	UINT id[ cnt ] = {
		IDC_GRID_1, IDC_GRID_2 };

	CMSHFlexGrid *pGrid = NULL;
	for( INT i = 0; i < cnt; i++ )
	{
		pGrid = ( CMSHFlexGrid * )GetDlgItem( id[ i ] );
		pGrid->SetColWidth( 0, 0, 0 );
	}

	// Tab Control 넣기
	m_tab.InsertItem( 0, _T(" 구 입 ") );
	m_tab.InsertItem( 1, _T(" 기 증 ") );

	// DB에서 현재 년도 구해오기
	CESL_DataMgr *pDM = FindDM( _T("DM_접수번호_구입") );
	CTime today = pDM->GetDBTime();
	INT year = today.GetYear();

	// comboBox에 연도 목록 구성하기
	CString sYear;
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	for( i = 0; i < 21; i++ )
	{
		sYear.Format( _T("%d"), year - 10 + i );
		pCombo->InsertString( i, sYear );
	}
	pCombo->SetCurSel( 10 );
}

VOID CReceiptNumViwerDlg::SearchReceiptNum()
{
	CString query;
	query.Format( _T("ACQ_YEAR = '%s'"), GetSearchCondition() );

	CESL_DataMgr *pDM = NULL;
	pDM = FindDM( _T("DM_접수번호_구입") );
	pDM->RefreshDataManager( query );
	pDM = FindDM( _T("DM_접수번호_기증") );
	pDM->RefreshDataManager( query );

	AllControlDisplay( _T("CM_접수번호") );
	m_receiptNum.Empty();
}

CString CReceiptNumViwerDlg::GetSearchCondition()
{
	CString year;
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	pCombo->GetWindowText( year );
	return year;
}

VOID CReceiptNumViwerDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	ViewGrid( m_tab.GetCurSel() );

	*pResult = 0;
}

VOID CReceiptNumViwerDlg::ViewGrid( INT idx )
{
	const INT cnt = 2;
	UINT id[ cnt ] = {
		IDC_GRID_1, IDC_GRID_2 };
	bool show[ cnt ] = { false, false };
	show[ idx ] = true;

	CMSHFlexGrid *pGrid = NULL;
	for( INT i = 0; i < cnt; i++ )
	{
		pGrid = ( CMSHFlexGrid * )GetDlgItem( id[ i ] );
		pGrid->ShowWindow( show[ i ] );
	}	
}

BEGIN_EVENTSINK_MAP(CReceiptNumViwerDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CReceiptNumViwerDlg)
	ON_EVENT(CReceiptNumViwerDlg, IDC_GRID_1, -600 /* Click */, OnClickGrid1, VTS_NONE)
	ON_EVENT(CReceiptNumViwerDlg, IDC_GRID_2, -600 /* Click */, OnClickGrid2, VTS_NONE)
	ON_EVENT(CReceiptNumViwerDlg, IDC_GRID_1, -601 /* DblClick */, OnDblClickGrid1, VTS_NONE)
	ON_EVENT(CReceiptNumViwerDlg, IDC_GRID_2, -601 /* DblClick */, OnDblClickGrid2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CReceiptNumViwerDlg::OnClickGrid1() 
{
	// TODO: Add your control notification handler code here

	ShowGridReverse( 0 );
}

VOID CReceiptNumViwerDlg::OnClickGrid2() 
{
	// TODO: Add your control notification handler code here
	
	ShowGridReverse( 1 );
}

VOID CReceiptNumViwerDlg::OnDblClickGrid1() 
{
	// TODO: Add your control notification handler code here
	
	OnOK();	
}

VOID CReceiptNumViwerDlg::OnDblClickGrid2() 
{
	// TODO: Add your control notification handler code here

	OnOK();
}

VOID CReceiptNumViwerDlg::ShowGridReverse( INT idx )
{
	const INT cnt = 2;
	CString alias[ cnt ] = {
		_T("구입그리드"), _T("기증그리드") };

	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_접수번호"), alias[ idx ] );
	pGrid->SetReverse( pGrid->GetRow() - 1 );

	m_receiptNum = pGrid->GetTextMatrix( pGrid->GetRow(), 1 );
}

HBRUSH CReceiptNumViwerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}