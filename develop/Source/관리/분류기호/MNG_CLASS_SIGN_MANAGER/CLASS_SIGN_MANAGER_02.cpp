// CLASS_SIGN_MANAGER_02.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "CLASS_SIGN_MANAGER_02.h"

#include "CLASS_SIGN_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCLASS_SIGN_MANAGER_02 dialog


CCLASS_SIGN_MANAGER_02::CCLASS_SIGN_MANAGER_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCLASS_SIGN_MANAGER_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCLASS_SIGN_MANAGER_02::~CCLASS_SIGN_MANAGER_02()
{
	
}

BOOL CCLASS_SIGN_MANAGER_02::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CCLASS_SIGN_MANAGER_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCLASS_SIGN_MANAGER_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCLASS_SIGN_MANAGER_02, CDialog)
	//{{AFX_MSG_MAP(CCLASS_SIGN_MANAGER_02)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLASS_SIGN_MANAGER_02 message handlers

BOOL CCLASS_SIGN_MANAGER_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	InitComboSet();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCLASS_SIGN_MANAGER_02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	CCLASS_SIGN_MANAGER_01 *pParent = ( CCLASS_SIGN_MANAGER_01 * )pParentMgr;
	pCombo->SetCurSel( pParent->m_type );
}

VOID CCLASS_SIGN_MANAGER_02::InitComboSet()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->InsertString( 0, _T("KDC") );
	pCombo->InsertString( 1, _T("DDC") );
	pCombo->InsertString( 2, _T("색인어") );
}

VOID CCLASS_SIGN_MANAGER_02::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	CCLASS_SIGN_MANAGER_01 *pParent = ( CCLASS_SIGN_MANAGER_01 * )pParentMgr;
	pParent->SetScreenType( pCombo->GetCurSel() );

	if ( pCombo->GetCurSel() == 2 )
	{
		( (CStatic*) GetDlgItem (IDC_STATIC_RANGE) )->ShowWindow(FALSE);
		( (CEdit*) GetDlgItem (IDC_EDIT5) )->ShowWindow(FALSE);
	}
	else
	{
		( (CStatic*) GetDlgItem (IDC_STATIC_RANGE) )->ShowWindow(TRUE);
		( (CEdit*) GetDlgItem (IDC_EDIT5) )->ShowWindow(TRUE);
	}
}

// 검색
VOID CCLASS_SIGN_MANAGER_02::OnOK() 
{
	// TODO: Add extra validation here
	
	CString keyword, Tokeyword;
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	CEdit *pEdt2 = ( CEdit * )GetDlgItem( IDC_EDIT5 );
	pEdt->GetWindowText( keyword );
	pEdt2->GetWindowText( Tokeyword );

	keyword.TrimLeft(); keyword.TrimRight();
	Tokeyword.TrimLeft(); Tokeyword.TrimRight();
	if( !keyword.GetLength() )
	{
		AfxMessageBox( _T("검색어를 입력하여 주십시오") );
		return;
	}
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	CCLASS_SIGN_MANAGER_01 *pParent = ( CCLASS_SIGN_MANAGER_01 * )pParentMgr;
	pParent->SearchData( pCombo->GetCurSel(), keyword, Tokeyword );
	
	//CDialog::OnOK();
}

VOID CCLASS_SIGN_MANAGER_02::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CCLASS_SIGN_MANAGER_02::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN ) {
		
		if ( pMsg->wParam == VK_RETURN ) {
			OnOK();
			return TRUE;
		} 
		
	}
	
	return CDialog::PreTranslateMessage(pMsg);	
}


HBRUSH CCLASS_SIGN_MANAGER_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
