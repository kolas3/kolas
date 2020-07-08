// Major_Search_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Major_Search_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMajor_Search_Dlg dialog




CMajor_Search_Dlg::CMajor_Search_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmd_Search_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CMajor_Search_Dlg::~CMajor_Search_Dlg()
{
	
}

BOOL CMajor_Search_Dlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMajor_Search_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMajor_Search_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CMajor_Search_Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
	    OnOK() ;
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(CMajor_Search_Dlg, CDialog)
	//{{AFX_MSG_MAP(CMajor_Search_Dlg)
	ON_EN_SETFOCUS(IDC_eTITLE, OnSetfocuseTITLE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMajor_Search_Dlg message handlers

VOID CMajor_Search_Dlg::OnSetfocuseTITLE() 
{
	EFS_BEGIN	
	
	CEdit *pEdit1 = (CEdit *) GetDlgItem (IDC_eTITLE);
	HIMC hIME;
	DWORD dwConversion, dwSentence;
	
	hIME = ::ImmGetContext(pEdit1->m_hWnd);
	if(hIME) 
	{
		ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;
		
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
		
		ImmReleaseContext(pEdit1->m_hWnd,hIME); 
	} 	
	
	EFS_END	
}

HBRUSH CMajor_Search_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CMajor_Search_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
