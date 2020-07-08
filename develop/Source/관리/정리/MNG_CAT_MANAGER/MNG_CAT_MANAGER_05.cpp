// MNG_CAT_MANAGER_05.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CAT_MANAGER_05.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CAT_MANAGER_05 dialog


CMNG_CAT_MANAGER_05::CMNG_CAT_MANAGER_05(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD,	pParent)
{
	//{{AFX_DATA_INIT(CMNG_CAT_MANAGER_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_CAT_MANAGER_05::~CMNG_CAT_MANAGER_05()
{
}

BOOL CMNG_CAT_MANAGER_05::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	

VOID CMNG_CAT_MANAGER_05::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CAT_MANAGER_05)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CAT_MANAGER_05, CDialog)
	//{{AFX_MSG_MAP(CMNG_CAT_MANAGER_05)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CAT_MANAGER_05 message handlers

VOID CMNG_CAT_MANAGER_05::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	switch( m_mode )
	{
		case 1 :
			SetWindowText( _T("TAG 입력") );
			break;
		case 2 :
			SetWindowText( _T("TAG 수정") );
			SetModifyData();
			break;
		default :
			break;
	}
}

VOID CMNG_CAT_MANAGER_05::OnOK() 
{
	// TODO: Add extra validation here
	
	CEdit *pEdt;

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->GetWindowText( m_tag );

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT2 );
	pEdt->GetWindowText( m_subFieldCode );
	
	m_tag.TrimLeft();
	m_tag.TrimRight();
	m_subFieldCode.TrimLeft();
	m_subFieldCode.TrimRight();

	if( !m_tag.GetLength() )
	{
		AfxMessageBox( _T("TAG를 입력하여 주십시오") );
		return;
	}

	if( !m_subFieldCode.GetLength() )
	{
		AfxMessageBox( _T("식별기호를 입력하여 주십시오") );
		return;
	}
	
	CDialog::OnOK();
}


VOID CMNG_CAT_MANAGER_05::SetModifyData()
{
	CEdit *pEdt;

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->SetWindowText( m_tag );

	pEdt = ( CEdit * )GetDlgItem( IDC_EDIT2 );
	pEdt->SetWindowText( m_subFieldCode );
}


BOOL CMNG_CAT_MANAGER_05::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CAT_MANAGER_05") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CAT_MANAGER_01") );
		return false;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CMNG_CAT_MANAGER_05::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
