// WORKSHEET_SAMPLE_04.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "WORKSHEET_SAMPLE_04.h"
#include "WORKSHEET_SAMPLE_02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWORKSHEET_SAMPLE_04 dialog


CWORKSHEET_SAMPLE_04::CWORKSHEET_SAMPLE_04(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CWORKSHEET_SAMPLE_04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CWORKSHEET_SAMPLE_04::~CWORKSHEET_SAMPLE_04()
{
	
}

BOOL CWORKSHEET_SAMPLE_04::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CWORKSHEET_SAMPLE_04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWORKSHEET_SAMPLE_04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWORKSHEET_SAMPLE_04, CDialog)
	//{{AFX_MSG_MAP(CWORKSHEET_SAMPLE_04)
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWORKSHEET_SAMPLE_04 message handlers

BOOL CWORKSHEET_SAMPLE_04::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	INT ids;
	switch( m_mode )
	{
	case 1 :	// 입력 
			
		SetWorkSheetRect( CRect( 0, 0, 400, 500 ) );
			
		ids = InitWorkSheet_Mgr( _T("CM_MAIN_SAMPLE_INSERT"), m_workSheetInitFlag );
		if( -1 == ids )
			return  -1;
			
		CButton *button;
		button = ( CButton * )GetDlgItem( IDC_bBEFORE );
		button->ShowWindow( false );
		button = ( CButton * )GetDlgItem( IDC_bNEXT   );
		button->ShowWindow( false );
			
		break;
	case 2 :	// 수정
		SetWorkSheetRect( CRect( 0, 0, 400, 500 ) );
		InitWorkSheet_Mgr( _T("CM_MAIN_SAMPLE_INSERT"), m_workSheetInitFlag );
		break;
	default :
		break;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CWORKSHEET_SAMPLE_04::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here

	CWORKSHEET_SAMPLE_02 *pParent = ( CWORKSHEET_SAMPLE_02 * )pParentMgr;
	if( !pParent ) 
		return;
	pParent->OnbBEFORE();
	
	RefreshWorkSheetData();
}

VOID CWORKSHEET_SAMPLE_04::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	CWORKSHEET_SAMPLE_02 *pParent = ( CWORKSHEET_SAMPLE_02 * )pParentMgr;
	if( !pParent )
		return;

	pParent->OnbNEXT();
	
	RefreshWorkSheetData();
}

VOID CWORKSHEET_SAMPLE_04::OnOK() 
{
	// TODO: Add extra validation here
	
	SaveWorkSheetData();

	if( 1 == m_workSheetType || 4 == m_workSheetType )
	{
		CWORKSHEET_SAMPLE_02 *pDlg = ( CWORKSHEET_SAMPLE_02 * )pParentMgr;
		pDlg->OnOK();

		pDlg->PostMessage( WM_CLOSE );
	}
	
	CDialog::OnOK();
}

VOID CWORKSHEET_SAMPLE_04::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	if( 1 == m_workSheetType || 4 == m_workSheetType )
	{
		CWORKSHEET_SAMPLE_02 *pDlg = ( CWORKSHEET_SAMPLE_02 * )pParentMgr;
		pDlg->PostMessage( WM_CLOSE );
	}
	
	CDialog::OnCancel();	
}

HBRUSH CWORKSHEET_SAMPLE_04::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
