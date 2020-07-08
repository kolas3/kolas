// SE_LOC_1598.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1598.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1598 dialog


CSE_LOC_1598::CSE_LOC_1598(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSE_LOC_1598::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1598)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1598::~CSE_LOC_1598()
{
}

VOID CSE_LOC_1598::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1598)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1598, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1598)
	ON_BN_CLICKED(IDC_btnCHANGE, OnbtnCHANGE)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_CBN_SELCHANGE(IDC_PRINT_LIST, OnSelchangeCombo1)
	ON_CBN_SETFOCUS(IDC_PRINT_LIST, OnSetfocusPrintList)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1598 message handlers

BOOL CSE_LOC_1598::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nCloseMode = -1;
	((CButton*)GetDlgItem(IDC_radLISTPRINT))->SetCheck(1);

	SetComboList();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_LOC_1598::OnbtnCHANGE() 
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_PRINT_LIST );
	CButton *pBtn = ( CButton * )GetDlgItem( IDC_radLABLEPRINT );

	if( pBtn->GetCheck() )
		m_nCloseMode = 1;
	else
	{
		m_nCloseMode = 0;
		pCombo->GetWindowText( m_printTitle );
	}
	/*
	if( ((CButton*)GetDlgItem(IDC_radLISTPRINT))->GetCheck() == 1 )
		m_nCloseMode = 0;	
	else
		m_nCloseMode = 1;	

	*/
	OnCancel();
}

VOID CSE_LOC_1598::OnbtnCLOSE() 
{
	OnCancel();	
}

VOID CSE_LOC_1598::SetComboList()
{
	const INT cnt = 7;
	CString list[ cnt ] = {
		_T("실시간 인계자료 목록"),
		_T("실시간자료 목록"),
		_T("복귀자료 목록"),
		_T("등록인계자료 목록"),
		_T("제본인계자료 목록"),
		_T("폐기자료 목록") ,
		_T("실시간 인계자료 목록(2)")

	};

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_PRINT_LIST );
	for( INT i = 0; i < cnt; i++ )
		pCombo->InsertString( i, list[ i ] );
	pCombo->SetCurSel( 0 );
}

VOID CSE_LOC_1598::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	
	CButton *pBtn = ( CButton * )GetDlgItem( IDC_radLABLEPRINT );
	pBtn->SetCheck( 0 );
}

VOID CSE_LOC_1598::OnSetfocusPrintList() 
{
	// TODO: Add your control notification handler code here
	CButton *pBtn = ( CButton * )GetDlgItem( IDC_radLABLEPRINT );
	pBtn->SetCheck( 0 );
}

HBRUSH CSE_LOC_1598::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
