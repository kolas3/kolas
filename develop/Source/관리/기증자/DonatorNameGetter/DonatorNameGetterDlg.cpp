// DonatorNameGetterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "DonatorNameGetterDlg.h"
#include "DonatorNameGetter_Search.h"
#include "DonatorNameGetter_Input.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetterDlg dialog


CDonatorNameGetterDlg::CDonatorNameGetterDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonatorNameGetterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDonatorNameGetterDlg::~CDonatorNameGetterDlg()
{	
	
}

BOOL CDonatorNameGetterDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CDonatorNameGetterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonatorNameGetterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonatorNameGetterDlg, CDialog)
	//{{AFX_MSG_MAP(CDonatorNameGetterDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bFIND, OnbFIND)
	ON_BN_CLICKED(IDC_bINPUT, OnbINPUT)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetterDlg message handlers

BOOL CDonatorNameGetterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("DonatorNameGetterDlg") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : DonatorNameGetterDlg") );
		return false;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	
}

VOID CDonatorNameGetterDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

// 검색
VOID CDonatorNameGetterDlg::OnbFIND() 
{
	// TODO: Add your control notification handler code here
	
	CDonatorNameGetter_Search dlg( this );
	dlg.DoModal();
}

// 입력
VOID CDonatorNameGetterDlg::OnbINPUT() 
{
	// TODO: Add your control notification handler code here
	
	CDonatorNameGetter_Input dlg( this );
	dlg.m_mode = 1;
	dlg.DoModal();
}

// 수정
VOID CDonatorNameGetterDlg::OnbMODIFY() 
{
	// TODO: Add your control notification handler code here

	CDonatorNameGetter_Input dlg( this );
	dlg.m_mode = 2;
	dlg.DoModal();	
}

// 삭제
VOID CDonatorNameGetterDlg::OnbDELETE() 
{
	// TODO: Add your control notification handler code here
	
	DeleteData();
}

// 확인
VOID CDonatorNameGetterDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	pGrid->SetRow( m_curRow );
	pGrid->SetCol( 3 );
	m_donatorName = pGrid->GetText();
	
	CDialog::OnOK();
}

// 닫기
VOID CDonatorNameGetterDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CDonatorNameGetterDlg::ShowGrid()
{
	ControlDisplay( _T("CM_DonatorNameGetterDlg"), _T("그리드") );
}

BEGIN_EVENTSINK_MAP(CDonatorNameGetterDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDonatorNameGetterDlg)
	ON_EVENT(CDonatorNameGetterDlg, IDC_mngGRID_1, -600 /* Click */, OnClickmngGRID1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDonatorNameGetterDlg::OnClickmngGRID1() 
{
	// TODO: Add your control notification handler code here
	
	CESL_ControlMgr *pCM = FindCM( _T("CM_DonatorNameGetterDlg") );
	CESL_Grid *pGrid = ( CESL_Grid * )pCM->FindControl( _T("그리드") );

	m_curRow = pGrid->GetRow();
	pGrid->SetReverse( m_curRow - 1 );
}

INT * CDonatorNameGetterDlg::GetCheckedDMRow(INT &cnt)
{
	CArray< INT, INT > row;

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	INT curRow = pGrid->GetRow();
	INT curCol = pGrid->GetCol();
		
	pGrid->SetCol( 1 );
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );

		if( _T("V") == pGrid->GetText() )
			row.Add( i - 1 );
	}

	
	cnt = row.GetSize();
	INT *checkedDMRow = ( INT * )calloc( cnt, sizeof( INT ) );
	for( i = 0; i < cnt; i++ )
		checkedDMRow[ i ] = row.GetAt( i );
	
	return checkedDMRow;
}

VOID CDonatorNameGetterDlg::DeleteData()
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

	CESL_DataMgr *pDM = FindDM( _T("DM_DonatorNameGetterDlg") );
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


HBRUSH CDonatorNameGetterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

