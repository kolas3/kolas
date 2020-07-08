// UserMainWait.cpp : implementation file
//

#include "stdafx.h"
#include "UserMainWait.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserMainWait dialog


CUserMainWait::CUserMainWait(CWnd* pParent /*=NULL*/)
	: CDialog(CUserMainWait::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserMainWait)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIsEnd = FALSE;
	m_pParent = pParent;
}


void CUserMainWait::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserMainWait)
	DDX_Control(pDX, IDC_staUSERMAIN_PIC, m_Picture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserMainWait, CDialog)
	//{{AFX_MSG_MAP(CUserMainWait)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserMainWait message handlers

int CUserMainWait::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
		
	return CDialog::DoModal();
}

BOOL CUserMainWait::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_Picture.Load(_T("..\\bmp\\UserMain\\Client\\loading.gif")))
	{
		m_Picture.Draw();
	}

	SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 583, 158, SWP_NOMOVE|SWP_NOACTIVATE);
	CenterWindow();

	this->ShowWindow(SW_HIDE);

// 	if(TRUE == m_bIsEnd)
// 	{
// 		PostMessage(WM_CLOSE, NULL, NULL);		
// 	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CUserMainWait::OnEraseBkgnd(CDC* pDC) 
{
 	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CUserMainWait::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
 	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}


BOOL CUserMainWait::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

INT CUserMainWait::Run(BOOL bRun)
{
// 	if(TRUE == bRun)
// 	{
// 		if(FALSE == m_bIsEnd)
// 		{
// 			DoModal();
// 			TRACE(_T("TRUE\r\n"));
// 		}
// 		else
// 		{
// 			m_bIsEnd = FALSE;
// 		}
// 	}
// 	else
// 	{		
// 		m_bIsEnd = TRUE;
// 		if(GetSafeHwnd())
// 		{
// 			PostMessage(WM_CLOSE, NULL, NULL);
// 			TRACE(_T("FALSE\r\n"));
// 		}
// 	}	

	return 0;
}

void CUserMainWait::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(TRUE == bShow)
	{
		m_pParent->SetCapture();
	}
	else
	{
		ReleaseCapture();
	}
	// TODO: Add your message handler code here
// 	if(TRUE == bShow)
// 	{
// 		if(TRUE == m_bIsEnd)
// 		{
// 			PostMessage(WM_CLOSE, NULL, NULL);
// 			TRACE(_T("EXIT\r\n"));
// 		}
// 	}
}
