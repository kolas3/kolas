// MNG_CLASS_SIGN_MINI_POPUP_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CLASS_SIGN_MINI_POPUP_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CLASS_SIGN_MINI_POPUP_01 dialog


CMNG_CLASS_SIGN_MINI_POPUP_01::CMNG_CLASS_SIGN_MINI_POPUP_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CLASS_SIGN_MINI_POPUP_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_CLASS_SIGN_MINI_POPUP_01::~CMNG_CLASS_SIGN_MINI_POPUP_01()
{	
	
}  

BOOL CMNG_CLASS_SIGN_MINI_POPUP_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CMNG_CLASS_SIGN_MINI_POPUP_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CLASS_SIGN_MINI_POPUP_01)
	DDX_Control(pDX, IDC_sTAB, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CLASS_SIGN_MINI_POPUP_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_CLASS_SIGN_MINI_POPUP_01)
	ON_BN_CLICKED(IDC_bFIND, OnbFIND)
	ON_NOTIFY(TCN_SELCHANGING, IDC_sTAB, OnSelchangingsTAB)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CLASS_SIGN_MINI_POPUP_01 message handlers

BOOL CMNG_CLASS_SIGN_MINI_POPUP_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CLASS_SIGN_MINI_POPUP_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CLASS_SIGN_MINI_POPUP_01") );
		return false;
	}

	InitCtrlSetting();
	m_kdc = -1;
	m_ddc = -1;
	m_curTab = 0;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 확인
VOID CMNG_CLASS_SIGN_MINI_POPUP_01::OnOK() 
{
	// TODO: Add extra validation here
	
	CString sign;
	if( GetSign( sign ) )
	{
		AfxMessageBox( _T("선택항목이 없습니다.") );
		return;
	}

	m_sign = sign;
	
	CDialog::OnOK();
}

// 닫기
VOID CMNG_CLASS_SIGN_MINI_POPUP_01::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

// 검색
VOID CMNG_CLASS_SIGN_MINI_POPUP_01::OnbFIND() 
{
	// TODO: Add your control notification handler code here

	CString keyword;
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eCONDITION );
	pEdt->GetWindowText( keyword );
	keyword.TrimLeft(); keyword.TrimRight();
	if( !keyword.GetLength() )
	{
		AfxMessageBox( _T("검색어를 입력하여 주십시오") );
		return;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CLASS_SIGN_MINI_POPUP_01") );
	pDM->RefreshDataManager( MakeSelectQuery( keyword ) );

	if( !pDM->GetRowCount() )
		AfxMessageBox( _T("검색 결과가 없습니다.") );

	ShowGrid();
}

VOID CMNG_CLASS_SIGN_MINI_POPUP_01::InitCtrlSetting()
{
	m_tab.InsertItem( 0, _T("KDC") );
	m_tab.InsertItem( 1, _T("DDC") );
	ShowTabGrid( 0 );
}

VOID CMNG_CLASS_SIGN_MINI_POPUP_01::OnSelchangingsTAB(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	ShowTabGrid();

	*pResult = 0;
}

VOID CMNG_CLASS_SIGN_MINI_POPUP_01::ShowTabGrid( INT option )
{
	if( -1 == option )
	{
		option = m_tab.GetCurSel();

		if( option )
			option = 0;
		else
			option = 1;

		m_curTab = option;
	}
	
	CMSHFlexGrid *pGrid_1 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_S1 );
	CMSHFlexGrid *pGrid_2 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_S2 );

	switch( option )
	{
		case 0 :
			pGrid_1->ShowWindow( true );
			pGrid_2->ShowWindow( false );
			break;
		case 1 :
			pGrid_1->ShowWindow( false );
			pGrid_2->ShowWindow( true );
			break;
	}
}

CString CMNG_CLASS_SIGN_MINI_POPUP_01::MakeSelectQuery( CString keyword )
{
	CString where;

	where.Format( _T("INDEX_WORD LIKE '%%%s%%'"), keyword );
	return where;
}

VOID CMNG_CLASS_SIGN_MINI_POPUP_01::ShowGrid()
{
	ControlDisplay( _T("CM_MNG_CLASS_SIGN_MINI_POPUP_01"), _T("그리드_1") );
	ControlDisplay( _T("CM_MNG_CLASS_SIGN_MINI_POPUP_01"), _T("그리드_2") );
}

BEGIN_EVENTSINK_MAP(CMNG_CLASS_SIGN_MINI_POPUP_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_CLASS_SIGN_MINI_POPUP_01)
	ON_EVENT(CMNG_CLASS_SIGN_MINI_POPUP_01, IDC_mngGRID_S1, -600 /* Click */, OnClickmngGRIDS1, VTS_NONE)
	ON_EVENT(CMNG_CLASS_SIGN_MINI_POPUP_01, IDC_mngGRID_S2, -600 /* Click */, OnClickmngGRIDS2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_CLASS_SIGN_MINI_POPUP_01::OnClickmngGRIDS1() 
{
	// TODO: Add your control notification handler code here

	GridCheckSet( 0 );
}

VOID CMNG_CLASS_SIGN_MINI_POPUP_01::OnClickmngGRIDS2() 
{
	// TODO: Add your control notification handler code here
	
	GridCheckSet( 1 );
}

VOID CMNG_CLASS_SIGN_MINI_POPUP_01::GridCheckSet( INT option )
{
	CMSHFlexGrid *pGrid;
	switch( option )
	{
		case 0 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_S1 );
			break;
		case 1 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_S2 );
			break;
	}

	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	if( 1 != col )
		return;

	INT selectedRow;
	if( _T("V") == pGrid->GetText() )
	{
		pGrid->SetText( _T("") );
		selectedRow = -1;
		return;
	}

	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );

		if( row == i )
		{
			pGrid->SetText( _T("V") );
			selectedRow = row;
		}
		else
			pGrid->SetText( _T("") );
	}

	pGrid->SetRow( row );

	switch( option )
	{
		case 0 :
			m_kdc = selectedRow;
			break;
		case 1 :
			m_ddc = selectedRow;
			break;
	}
}

INT CMNG_CLASS_SIGN_MINI_POPUP_01::GetSign( CString &sign )
{
	switch( m_curTab )
	{
		case 0 :
			if( -1 == m_kdc )
				return -1;
			else
				sign = GetGridText( 0, m_kdc );
			break;
		case 1 :
			if( -1 == m_ddc )
				return -1;
			else
				sign = GetGridText( 1, m_ddc );
			break;
	}

	return 0;
}

CString CMNG_CLASS_SIGN_MINI_POPUP_01::GetGridText( INT option, INT row )
{
	CString sign;

	CMSHFlexGrid *pGrid;
	switch( option )
	{
		case 0 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_S1 );
			break;
		case 1 :
			pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_S2 );
			break;
	}

	pGrid->SetRow( row );
	pGrid->SetCol( 2 );

	return pGrid->GetText();
}

HBRUSH CMNG_CLASS_SIGN_MINI_POPUP_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
