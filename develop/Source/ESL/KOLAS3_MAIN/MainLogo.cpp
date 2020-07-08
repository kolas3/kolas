// MainLogo.cpp : implementation file
//

#include "stdafx.h"
#include "MainLogo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainLogo dialog


CMainLogo::CMainLogo(CWnd* pParent /*=NULL*/)
	: CDialog(CMainLogo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainLogo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// KOL.UDF.022
	m_pParent = NULL;
}


void CMainLogo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainLogo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainLogo, CDialog)
	//{{AFX_MSG_MAP(CMainLogo)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainLogo message handlers
BOOL CMainLogo::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
	
	m_pParent = pParentWnd;
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CMainLogo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowPos(NULL, 0, 0, 500, 300, SWP_NOMOVE|SWP_NOZORDER);
	CenterWindow();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainLogo::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == 1 )
	{
		KillTimer(1);
		//ShowWindow(SW_HIDE);		
		CDialog::OnOK();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CMainLogo::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	SetTimer(1,2000,NULL);
	
}
