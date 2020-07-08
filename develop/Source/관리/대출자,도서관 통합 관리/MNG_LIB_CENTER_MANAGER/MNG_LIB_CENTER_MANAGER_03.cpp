// MNG_LIB_CENTER_MANAGER_03.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_LIB_CENTER_MANAGER_03.h"
#include "MNG_LIB_CENTER_MANAGER_02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_03 dialog


CMNG_LIB_CENTER_MANAGER_03::CMNG_LIB_CENTER_MANAGER_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_CENTER_MANAGER_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_LIB_CENTER_MANAGER_03::~CMNG_LIB_CENTER_MANAGER_03()
{
	
}

BOOL CMNG_LIB_CENTER_MANAGER_03::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_LIB_CENTER_MANAGER_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_CENTER_MANAGER_03)
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_CENTER_MANAGER_03, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_CENTER_MANAGER_03)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_CTLCOLOR()   
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_03 message handlers

BOOL CMNG_LIB_CENTER_MANAGER_03::OnInitDialog() 
{
	CDialog::OnInitDialog();

	
	// TODO: Add extra initialization here
	
	m_checkedRowCnt = 0;
	m_checkedDMRow = NULL;
	m_index = 0;
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

VOID CMNG_LIB_CENTER_MANAGER_03::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	switch( m_type )
	{
		case 1 :
			SetInsertProperty();
			break;
		case 2 :
			SetUpdateProperty();
			break;
		default :
			break;
	}
}

VOID CMNG_LIB_CENTER_MANAGER_03::SetInsertProperty()
{
	SetWindowText( _T(" 입 력 ") );
	ShowButton( false );
}

VOID CMNG_LIB_CENTER_MANAGER_03::ShowButton( bool flag )
{
	CButton *pBtn;

	pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
	pBtn->ShowWindow( flag );

	pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
	pBtn->ShowWindow( flag );
}

VOID CMNG_LIB_CENTER_MANAGER_03::SetUpdateProperty()
{
	SetWindowText( _T(" 수 정 ") );
	SetCheckedRowInfo();
	ShowButton( true );

	if( !m_checkedRowCnt )
	{
		AfxMessageBox( _T("선택 목록이 없습니다.") );
		PostMessage( WM_CLOSE );
	}
/*
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->EnableWindow( false );
*/	
	ViewModifyData();
}

// 저장
VOID CMNG_LIB_CENTER_MANAGER_03::OnOK() 
{
	// TODO: Add extra validation here
	
	const INT fieldCnt = 6;
	CString data[ fieldCnt ];
	if( GetCtrlData( data ) )
	{
		AfxMessageBox( _T("모든 항목을 입력하여 주십시오") );
		return;
	}
	
	if( 1 == m_type && CheckLibCode( data[ 1 ] ) )
	{
		AfxMessageBox( _T("중복되는 도서관 CODE가 있습니다.") );
		return;
	}

	if( SaveData( data ) )
	{
		AfxMessageBox( _T("Error : INT CMNG_LIB_CENTER_MANAGER_03::SaveData( CString data[] )") );
		return;
	}

	ShowGrid();
	switch( m_type )
	{
		case 1 :
			AfxMessageBox( _T("저장되었습니다.") );
			CDialog::OnOK();
			break;
		case 2 :
			AfxMessageBox( _T("수정되었습니다.") );
			break;
		default :
			break;
	}	
}

INT CMNG_LIB_CENTER_MANAGER_03::GetCtrlData( CString data[] )
{
	CEdit *pEdt;

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->GetWindowText( data[ 1 ] );
	AllTrim( data[ 1 ] );
	if( !data[ 1 ].GetLength() )
		return -1;

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT2 );
	pEdt->GetWindowText( data[ 2 ] );
	AllTrim( data[ 2 ] );
	if( !data[ 2 ].GetLength() )
		return -2;

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT3 );
	pEdt->GetWindowText( data[ 4 ] );
	AllTrim( data[ 4 ] );

	m_ip.GetWindowText( data[ 3 ] );
	if( !data[ 3 ].GetLength() )
		return -3;

	return 0;
}

VOID CMNG_LIB_CENTER_MANAGER_03::AllTrim( CString &string )
{
	string.TrimLeft();
	string.TrimRight();
}

INT CMNG_LIB_CENTER_MANAGER_03::SaveData( CString data[] )
{
	CMNG_LIB_CENTER_MANAGER_02 *pParent = ( CMNG_LIB_CENTER_MANAGER_02 * )pParentMgr;
	
	switch( m_type )
	{
		case 1 :
			return pParent->SaveData( data, m_type );
			break;
		case 2 :
			return pParent->SaveData( data, m_type, m_checkedDMRow[ m_index ] );
			break;
		default :
			return -1;
			break;
	}		 
}

VOID CMNG_LIB_CENTER_MANAGER_03::ShowGrid()
{
	CMNG_LIB_CENTER_MANAGER_02 *pParent = ( CMNG_LIB_CENTER_MANAGER_02 * )pParentMgr;
	pParent->ShowGrid();
}

INT CMNG_LIB_CENTER_MANAGER_03::CheckLibCode( CString code )
{
	CMNG_LIB_CENTER_MANAGER_02 *pParent = ( CMNG_LIB_CENTER_MANAGER_02 * )pParentMgr;
	return pParent->CheckLibCode( code );
}

VOID CMNG_LIB_CENTER_MANAGER_03::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	FreeCheckedRowInfo();
	
	CDialog::OnClose();
}

VOID CMNG_LIB_CENTER_MANAGER_03::FreeCheckedRowInfo()
{
	if( m_checkedRowCnt )
		free( m_checkedDMRow );
}

VOID CMNG_LIB_CENTER_MANAGER_03::SetCheckedRowInfo()
{
	CMNG_LIB_CENTER_MANAGER_02 *pParent = ( CMNG_LIB_CENTER_MANAGER_02 * )pParentMgr;
	m_checkedDMRow = pParent->GetCheckedRowInfo( m_checkedRowCnt );
}

VOID CMNG_LIB_CENTER_MANAGER_03::ViewModifyData()
{
	const INT ctrlCnt = 4;
	CString data[ ctrlCnt ];
	GetCurrentData( data );

	ShowCurrentData( data );
}


// 이전
VOID CMNG_LIB_CENTER_MANAGER_03::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	if( !m_index )
	{
		AfxMessageBox( _T("맨 처음 목록입니다.") );
		return;
	}

	m_index--;
	ViewModifyData();
}

// 다음
VOID CMNG_LIB_CENTER_MANAGER_03::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	if( m_checkedRowCnt - 1 == m_index )
	{
		AfxMessageBox( _T("맨 마지막 목록입니다.") );
		return;
	}

	m_index++;
	ViewModifyData();
}

INT CMNG_LIB_CENTER_MANAGER_03::GetCurrentData( CString data[] )
{
	CMNG_LIB_CENTER_MANAGER_02 *pParent = ( CMNG_LIB_CENTER_MANAGER_02 * )pParentMgr;
	return pParent->GetCurrentData( data, m_checkedDMRow[ m_index ] );
}

VOID CMNG_LIB_CENTER_MANAGER_03::ShowCurrentData( CString data[] )
{
	CEdit *pEdt;

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->SetWindowText( data[ 0 ] );

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT2 );
	pEdt->SetWindowText( data[ 1 ] );

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT3 );
	pEdt->SetWindowText( data[ 3 ] );

	m_ip.SetWindowText( data[ 2] );	
}


HBRUSH CMNG_LIB_CENTER_MANAGER_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
