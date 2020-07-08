// BO_CAT_REG_SEARCH_POPUP.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_REG_SEARCH_POPUP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_SEARCH_POPUP dialog


CBO_CAT_REG_SEARCH_POPUP::CBO_CAT_REG_SEARCH_POPUP(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_SEARCH_POPUP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_REG_SEARCH_POPUP::~CBO_CAT_REG_SEARCH_POPUP()
{
}

VOID CBO_CAT_REG_SEARCH_POPUP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_SEARCH_POPUP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_SEARCH_POPUP, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_SEARCH_POPUP)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnREG_SEARCH2, OnbtnREGSEARCH2)
	ON_BN_CLICKED(IDC_btnREG_SEARCH, OnbtnREGSEARCH)
	ON_BN_CLICKED(IDC_btnREG_SEARCH3, OnbtnREGSEARCH3)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_SEARCH_POPUP message handlers

BOOL CBO_CAT_REG_SEARCH_POPUP::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_0320")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pSearchDlg = new CBO_CAT_REG_SEARCH_0100(this);
	m_pSearchDlg->Create(this);
	m_pSearchDlg->ShowWindow(SW_SHOW);
	m_pSearchDlg->CenterWindow();

	m_pTmpSearchDlg = m_pSearchDlg;
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	
	return -1;
}


BOOL CBO_CAT_REG_SEARCH_POPUP::Create(CWnd* pParentWnd)
{
	EFS_BEGIN
		
		return CDialog::Create(IDD, pParentWnd);
	
	EFS_END
		return FALSE;
	
}

VOID CBO_CAT_REG_SEARCH_POPUP::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if( GetSafeHwnd() == NULL ) return;
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRD_REG_0320);
	if (pGrid->GetSafeHwnd() == NULL) return;
	
	pGrid->MoveWindow(0 ,  50 , cx, cy-80);


	CRect WndPos[2];
	INT nIDC[2] = { IDC_staTOTALBOOK , IDC_edtBOOK_CNT };
	CWnd* pWnd[2];

	for( INT i = 0 ; i < 2 ; i++ )
	{
		pWnd[i] = GetDlgItem( nIDC[i] );
		pWnd[i]->GetWindowRect(WndPos[i]);
	}

	pWnd[0]->MoveWindow( 10 , cy - 22 , WndPos[0].Width() , WndPos[0].Height() );
	pWnd[1]->MoveWindow( 10 + WndPos[0].Width() , cy - 24 , WndPos[1].Width() , WndPos[1].Height() );
	
}

VOID CBO_CAT_REG_SEARCH_POPUP::OnbtnREGSEARCH2() 
{
	/*
	CBO_CAT_REG_SEARCH_0100 Dlg(this);
	Dlg.DoModal();
	*/
	m_pSearchDlg = m_pTmpSearchDlg;
	m_pSearchDlg->ShowWindow(SW_SHOW);	
	
}


VOID CBO_CAT_REG_SEARCH_POPUP::OnbtnREGSEARCH() 
{
	EFS_BEGIN

/*
	CBO_CAT_REG_SEARCH_0100 Dlg(this);
	
	CString sRegSw = m_sRegSh;
	Dlg.Onb0320TOFILE(sRegSw);
*/
	m_pSearchDlg = m_pTmpSearchDlg;
	m_pSearchDlg->Onb0320TOFILE(_T(""));
	


	EFS_END
}



VOID CBO_CAT_REG_SEARCH_POPUP::SetRegSwitch()
{
	EFS_BEGIN
		
		m_sRegSh = _T("µî·Ï");
	
	EFS_END
}

VOID CBO_CAT_REG_SEARCH_POPUP::OnbtnREGSEARCH3() 
{
	OnCancel();	
}

VOID CBO_CAT_REG_SEARCH_POPUP::OnClose() 
{
	m_pSearchDlg = m_pTmpSearchDlg;
	if( m_pSearchDlg != NULL )
		delete m_pSearchDlg;
	
	CDialog::OnClose();
}

HBRUSH CBO_CAT_REG_SEARCH_POPUP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
