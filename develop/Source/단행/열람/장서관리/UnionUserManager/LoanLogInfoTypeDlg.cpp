// LoanLogInfoTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoanLogInfoTypeDlg.h"
#include "UnionUserInfoViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanLogInfoTypeDlg dialog


CLoanLogInfoTypeDlg::CLoanLogInfoTypeDlg( CESL_Mgr *pParent /*=NULL*/ )
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CLoanLogInfoTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;

	EnableThemeDialogTexture(GetSafeHwnd()); 
}

CLoanLogInfoTypeDlg::~CLoanLogInfoTypeDlg()
{
}

BOOL CLoanLogInfoTypeDlg::Create( CWnd *pParentWnd )
{	
	return CDialog::Create( IDD, pParentWnd ); 
}	


void CLoanLogInfoTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanLogInfoTypeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoanLogInfoTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CLoanLogInfoTypeDlg)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanLogInfoTypeDlg message handlers



BOOL CLoanLogInfoTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("LoanLogInfoTypeDlg") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : LoanLogInfoTypeDlg") );
		return false;
	}
	
	SetInitCheck();
	MakeLibListCombo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CLoanLogInfoTypeDlg::SetLoanLogInfoType( int type[] )
{
	const int cnt = 6;
	unsigned int id[ cnt ];
	GetCheckBtnID( id );
	
	CButton *pBtn = NULL;
	for( int i = 0; i < cnt; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( id[ i ] );
		pBtn->SetCheck( type[ i ] );		
	}
}

void CLoanLogInfoTypeDlg::GetCheckBtnID( unsigned int id[] )
{
	id[ 0 ] = IDC_CHECK_UUM1;
	id[ 1 ] = IDC_CHECK_UUM2;
	id[ 2 ] = IDC_CHECK_UUM3;
	id[ 3 ] = IDC_CHECK_UUM4;
	id[ 4 ] = IDC_CHECK_UUM5;
	id[ 5 ] = IDC_CHECK_UUM6;
}

void CLoanLogInfoTypeDlg::GetLoanLogInfoType( int type[] )
{
	const int cnt = 6;
	unsigned int id[ cnt ];
	GetCheckBtnID( id );
	
	CButton *pBtn = NULL;
	for( int i = 0; i < cnt; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( id[ i ] );
		type[ i ] = pBtn->GetCheck();
	}
}

void CLoanLogInfoTypeDlg::SetInitCheck()
{
	const int cnt = 6;
	int type[ cnt ] = {
		true, false, true, false, false, false };

	SetLoanLogInfoType( type );
}

int CLoanLogInfoTypeDlg::GetLibDMIndex()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cLIB_LIST );
	int curSel = pCombo->GetCurSel();
	
	if( curSel )
		return pCombo->GetCurSel() - 1;
	else
		return -1;
}

void CLoanLogInfoTypeDlg::MakeLibListCombo()
{   
	CUnionUserInfoViewer *pParent = ( CUnionUserInfoViewer * )pParentMgr;

	CESL_DataMgr *pDM = FindDM( _T("DM_UnionUserInfoViewer_LIB_LIST") );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cLIB_LIST );
	pCombo->InsertString( 0, _T(" - Àü Ã¼ -") );
	for( int i = 0; i < pDM->GetRowCount(); i++ )
		pCombo->InsertString( i + 1, pDM->GetCellData( _T("NAME"), i ) );
	pCombo->SetCurSel( 0 );
}

BOOL CLoanLogInfoTypeDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN )
		switch( pMsg->wParam )
		{
			case VK_ESCAPE :
				return NULL;
				break;
			default :
				break;
		}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CLoanLogInfoTypeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}


