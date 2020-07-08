// MNG_MNGTBL_MANAGER_03.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_MNGTBL_MANAGER_03.h"
#include "MNG_MNGTBL_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_MNGTBL_MANAGER_03 dialog


CMNG_MNGTBL_MANAGER_03::CMNG_MNGTBL_MANAGER_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_MNGTBL_MANAGER_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


CMNG_MNGTBL_MANAGER_03::~CMNG_MNGTBL_MANAGER_03()
{
	
}
BOOL CMNG_MNGTBL_MANAGER_03::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CMNG_MNGTBL_MANAGER_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_MNGTBL_MANAGER_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_MNGTBL_MANAGER_03, CDialog)
	//{{AFX_MSG_MAP(CMNG_MNGTBL_MANAGER_03)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_MNGTBL_MANAGER_03 message handlers

BOOL CMNG_MNGTBL_MANAGER_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	InitSetControls();

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_MNGTBL_MANAGER_03::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

VOID CMNG_MNGTBL_MANAGER_03::OnOK() 
{
	// TODO: Add extra validation here
	
	const INT ctrlCnt = 5;
	CString ctrlData[ ctrlCnt ];
	GetCtrlData( ctrlData );
	
	CMNG_MNGTBL_MANAGER_01 *pParent = ( CMNG_MNGTBL_MANAGER_01 * )pParentMgr;
	INT cnt = pParent->SearchData( MakeSelectQuery( ctrlData ) );

	if( cnt )
		pParent->ShowGrid();
	else
	{
		pParent->ShowGrid();
		AfxMessageBox( _T("검색 결과가 없습니다.") );
	}

	//CDialog::OnOK();
}

VOID CMNG_MNGTBL_MANAGER_03::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	OnClose();
}

VOID CMNG_MNGTBL_MANAGER_03::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::DestroyWindow();
}

VOID CMNG_MNGTBL_MANAGER_03::InitSetControls()
{
	CComboBox *pCombo;

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->InsertString( 0, _T("☞ 선택") );
	pCombo->InsertString( 1, _T("LIGHT") );
	pCombo->InsertString( 2, _T("UPGRADE") );
	pCombo->SetCurSel( 1 );
	pCombo->EnableWindow( false );

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	pCombo->InsertString( 0, _T("☞ 선택") );
	pCombo->InsertString( 1, _T("수 서") );
	pCombo->InsertString( 2, _T("열 람") );
	pCombo->InsertString( 3, _T("정 리") );
	pCombo->InsertString( 4, _T("기 타") );
	pCombo->SetCurSel( 0 );

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	pCombo->InsertString( 0, _T("☞ 선택") );
	pCombo->InsertString( 1, _T("단 행") );
	pCombo->InsertString( 2, _T("연 속") );
	pCombo->InsertString( 3, _T("공 통") );
	pCombo->SetCurSel( 0 );
}

VOID CMNG_MNGTBL_MANAGER_03::GetCtrlData( CString data[] )
{
	CComboBox *pCombo;
	
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	switch( pCombo->GetCurSel() )
	{
		case 1 :
			data[ 0 ] = _T("L");
			break;
		case 2 :
			data[ 0 ] = _T("U");
			break;
		default :
			data[ 0 ].Empty();
			break;
	}

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	if( pCombo->GetCurSel() )
	{
		pCombo->GetWindowText( data[ 1 ] );
		data[ 1 ].Replace( _T(" "), _T("") );
	}
	else
		data[ 1 ].Empty();

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	if( pCombo->GetCurSel() )
	{
		pCombo->GetWindowText( data[ 2 ] );
		data[ 2 ].Replace( _T(" "), _T("") );
	}
	else
		data[ 2 ].Empty();

	CEdit * pEdt;
	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT3 );
	pEdt->GetWindowText( data[ 3 ] );
	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT4 );
	pEdt->GetWindowText( data[ 3 ] );	
}

CString CMNG_MNGTBL_MANAGER_03::MakeSelectQuery( CString data[] )
{
	CString query, tmp;

	const INT fieldCnt = 5;
	CString fieldName[ fieldCnt ] = {
		_T("GROUP_1"), _T("GROUP_2"), _T("GROUP_3"), _T("CLASS_ALIAS"), _T("CLASS_DESC") };

	for( INT i = 0; i < fieldCnt; i++ )
	{
		AllTrim( data[ i ] );
		if( data[ i ].GetLength() )
		{
			tmp.Format( _T(" %s LIKE '%%%s%%' AND"), fieldName[ i ], data[ i ] );
			query += tmp;
		}
	}

	if( _T("AND") == query.Right( 3 ) )
	{
		tmp = query.Left( query.GetLength() - 3 );
		query = tmp;
	}

	return query;
}

VOID CMNG_MNGTBL_MANAGER_03::AllTrim( CString &string )
{
	string.TrimLeft();
	string.TrimRight();
}



HBRUSH CMNG_MNGTBL_MANAGER_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
