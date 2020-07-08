// KOMarcGetterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "KOMarcGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKOMarcGetterDlg dialog


CKOMarcGetterDlg::CKOMarcGetterDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CKOMarcGetterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKOMarcGetterDlg::~CKOMarcGetterDlg()
{
	
}

BOOL CKOMarcGetterDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CKOMarcGetterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKOMarcGetterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKOMarcGetterDlg, CDialog)
	//{{AFX_MSG_MAP(CKOMarcGetterDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bFIND, OnbFIND)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKOMarcGetterDlg message handlers

BOOL CKOMarcGetterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("KOMarcGetterDlg") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CODE_MANAGER_01") );
		return false;
	}
	
	m_code = _T("");
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CKOMarcGetterDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( -1 == m_type )
	{
		AfxMessageBox( _T("초기화가 되지 않았습니다.") );
		PostMessage( WM_CLOSE );
	}
	else
	{
		SetControls();
		OnbFIND();
	}
}


INT CKOMarcGetterDlg::InitSetProperty( INT type, CString condition )
{
	if( type < 0 || type > 7 )
	{
		PostMessage( WM_CLOSE );
		m_type = -1;
		return -1;
	}

	m_type = type;
	m_condition = condition;
	
	return 0;
}

VOID CKOMarcGetterDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

VOID CKOMarcGetterDlg::SetControls()
{
	const INT gridCnt = 8;
	UINT gridID[ gridCnt ] = {
		IDC_GRID_1, IDC_GRID_2, IDC_GRID_3, IDC_GRID_4, IDC_GRID_5, IDC_GRID_6, IDC_GRID_7, IDC_GRID_8 };
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( gridID[ m_type ] );
	pGrid->ShowWindow( true );

	// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
	// 통합마크 개정 적용
	// 부호 선택 화면 그리드 ScrollTrack 적용
/*//NEW------------------------------------------------------------------------*/
	pGrid->SetScrollTrack(TRUE);
/*//END------------------------------------------------------------------------*/

	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( IDC_eCODE );
	pEdt->SetWindowText( m_condition );

	CButton *pBtn = ( CButton * )GetDlgItem( IDC_MANAGE_TYPE );
	pEdt = ( CEdit * )GetDlgItem( IDC_eETC );
	
	if( 1 == m_type || 3 == m_type || 7 == m_type || 4 == m_type )
	{
		pBtn->ShowWindow( true );
		pEdt->ShowWindow( true );
	}

	if( 4 == m_type )
		pBtn->SetWindowText( _T("부호(num)") );

	const titleCnt = 8;
	CString title[ titleCnt ] = {
		_T("발행국부호"), _T("한국대학부호"), _T("언어구분부호"), _T("한국정부기관부호"), _T("한국지역구분분호"), _T("외국지역구분부호"), _T("국가부호"), _T("한국도서관부호") };
	SetWindowText( title[ m_type ] );
}

VOID CKOMarcGetterDlg::GetCondition(CString condition[])
{
	const INT edtCnt = 3;
	UINT edtID[ edtCnt ] = { IDC_eCODE, IDC_eCODENAME, IDC_eETC };
	
	CEdit *pEdt;
	for( INT i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->GetWindowText( condition[ i ] );
	}
}

VOID CKOMarcGetterDlg::OnbFIND() 
{
	// TODO: Add your control notification handler code here
	
	const INT ctrlCnt = 3;
	CString condition[ ctrlCnt ];
	GetCondition( condition );

	const INT dmCnt = 8;
	CString dmAlias[ dmCnt ] = {
		_T("DM_KOMarcGetterDlg_01"), _T("DM_KOMarcGetterDlg_02"), _T("DM_KOMarcGetterDlg_03"), 
		_T("DM_KOMarcGetterDlg_04"), _T("DM_KOMarcGetterDlg_05"), _T("DM_KOMarcGetterDlg_06"), 
		_T("DM_KOMarcGetterDlg_07"), _T("DM_KOMarcGetterDlg_08") };
	CESL_DataMgr *pDM = FindDM( dmAlias[ m_type ] );

	condition[0].MakeLower();
	condition[1].MakeUpper();
	if ( dmAlias[ m_type ] == _T("DM_KOMarcGetterDlg_04") || dmAlias[ m_type ] == _T("DM_KOMarcGetterDlg_02") ) {
		condition[0].MakeUpper();
	}
	pDM->RefreshDataManager( MakeSelectQuery( condition ) );
	
	if( !pDM->GetRowCount() )
		AfxMessageBox( _T("검색결과가 없습니다.") );
	
	CString gridAlias;
	gridAlias.Format( _T("그리드_%d"), m_type );
	ControlDisplay( _T("CM_KOMarcGetterDlg"), gridAlias );
}


CString CKOMarcGetterDlg::MakeSelectQuery( CString condition[] )
{
	CString query, tmp;
	
	AllTrim( condition[ 0 ] );
	if( condition[ 0 ].GetLength() )
	{
		tmp.Format( _T(" CLASS_CODE LIKE '%s%%' AND"), condition[ 0 ] );
		query += tmp;
	}

	AllTrim( condition[ 1 ] );
	if( condition[ 1 ].GetLength() )
	{
		//tmp.Format( _T(" REFERENCE LIKE '%%%s%%' AND"), condition[ 1 ] );
		//tmp.Format( _T(" description LIKE '%%%s%%' AND"), condition[ 1 ] );
		tmp.Format( _T(" upper(description) LIKE '%s%%' AND"), condition[ 1 ] );
		query += tmp;
	}

	AllTrim( condition[ 2 ] );
	if( condition[ 2 ].GetLength() )
		if( 1 == m_type || 3 == m_type || 7 == m_type )
		{
			tmp.Format( _T(" MANAGE_CLASS = '%s'"), condition[ 2 ] );
			query += tmp;
		}
		else if( 4 == m_type )
		{
			tmp.Format( _T(" NUMBER_CODE = '%s'"), condition[ 2 ] );
			query += tmp;
		}
	
	if( _T("AND") == query.Right( 3 ) )
	{
		tmp = query.Left( query.GetLength() - 3 );
		query = tmp;
	}
	AllTrim( query );

	return query;
}

VOID CKOMarcGetterDlg::AllTrim( CString &data )
{
	data.TrimLeft();
	data.TrimRight();
}

BEGIN_EVENTSINK_MAP(CKOMarcGetterDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CKOMarcGetterDlg)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_1, -600 /* Click */, OnClickGrid1, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_2, -600 /* Click */, OnClickGrid2, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_3, -600 /* Click */, OnClickGrid3, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_4, -600 /* Click */, OnClickGrid4, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_5, -600 /* Click */, OnClickGrid5, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_6, -600 /* Click */, OnClickGrid6, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_7, -600 /* Click */, OnClickGrid7, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_8, -600 /* Click */, OnClickGrid8, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_1, -601 /* DblClick */, OnDblClickGrid1, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_2, -601 /* DblClick */, OnDblClickGrid2, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_3, -601 /* DblClick */, OnDblClickGrid3, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_4, -601 /* DblClick */, OnDblClickGrid4, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_5, -601 /* DblClick */, OnDblClickGrid5, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_6, -601 /* DblClick */, OnDblClickGrid6, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_7, -601 /* DblClick */, OnDblClickGrid7, VTS_NONE)
	ON_EVENT(CKOMarcGetterDlg, IDC_GRID_8, -601 /* DblClick */, OnDblClickGrid8, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CKOMarcGetterDlg::OnClickGrid1() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
}

VOID CKOMarcGetterDlg::OnClickGrid2() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
}

VOID CKOMarcGetterDlg::OnClickGrid3() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
}

VOID CKOMarcGetterDlg::OnClickGrid4() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
}

VOID CKOMarcGetterDlg::OnClickGrid5() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
}

VOID CKOMarcGetterDlg::OnClickGrid6() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
}

VOID CKOMarcGetterDlg::OnClickGrid7() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
}

VOID CKOMarcGetterDlg::OnClickGrid8() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
}

VOID CKOMarcGetterDlg::SetGridReverse()
{
	CString gridAlias;
	gridAlias.Format( _T("그리드_%d"), m_type );
	CESL_ControlMgr *pCM = FindCM( _T("CM_KOMarcGetterDlg") );
	CESL_Grid *pGrid = ( CESL_Grid * )pCM->FindControl( gridAlias );

	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	pGrid->SetReverse( row - 1 );

	pGrid->SetCol( 1 );
	m_code = pGrid->GetText();

	pGrid->SetRow( row );
	pGrid->SetCol( col );
}

VOID CKOMarcGetterDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

VOID CKOMarcGetterDlg::OnDblClickGrid1() 
{
	// TODO: Add your control notification handler code here

	SetGridReverse();
	CDialog::OnOK();
}

VOID CKOMarcGetterDlg::OnDblClickGrid2() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
	CDialog::OnOK();
}

VOID CKOMarcGetterDlg::OnDblClickGrid3() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
	CDialog::OnOK();
}

VOID CKOMarcGetterDlg::OnDblClickGrid4() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
	CDialog::OnOK();
}

VOID CKOMarcGetterDlg::OnDblClickGrid5() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
	CDialog::OnOK();
}

VOID CKOMarcGetterDlg::OnDblClickGrid6() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
	CDialog::OnOK();
}

VOID CKOMarcGetterDlg::OnDblClickGrid7() 
{
	// TODO: Add your control notification handler code here
	
	SetGridReverse();
	CDialog::OnOK();
}

VOID CKOMarcGetterDlg::OnDblClickGrid8() 
{
	// TODO: Add your control notification handler code here

	SetGridReverse();
	CDialog::OnOK();
}
HBRUSH CKOMarcGetterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
