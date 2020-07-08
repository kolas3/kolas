// BO_LOC_2140.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_2140.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2140 dialog


CBO_LOC_2140::CBO_LOC_2140(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_2140)
	m_pSpfWork = NULL;
	//}}AFX_DATA_INIT
}

CBO_LOC_2140::~CBO_LOC_2140()
{
	if( m_pSpfWork != NULL)
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}
}

VOID CBO_LOC_2140::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_2140)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_2140, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_2140)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2140 message handlers

BOOL CBO_LOC_2140::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("BO_LOC_2140")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}


	m_pSpfWork = new CBLBookCheckProc(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2140::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);

	INT ids;
	ids = m_pSpfWork->SPFExcutePath( BOOK_CHECK );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnShowWindow(BOOL bShow, UINT nStatus)") );
	
	EFS_END
}

BOOL CBO_LOC_2140::Create(CWnd* pParentWnd) 
{	
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2140::OnClose() 
{
	EFS_BEGIN

	if( m_pSpfWork != NULL ) delete m_pSpfWork;
	m_pSpfWork = NULL;

	CDialog::OnClose();


	EFS_END
}

VOID CBO_LOC_2140::OnbtnCLOSE() 
{
	EFS_BEGIN

	bool bThreadState = m_pSpfWork->GetThreadState();

	if( !bThreadState )
	{
		INT ids = MessageBox( _T("장서체크 중입니다. 중단하시겠습니까?") , _T("장서 체크") , MB_YESNO | MB_ICONQUESTION );
		if( ids == IDYES ) m_pSpfWork->SetStopThread();
	}
	else
		OnCancel();

	EFS_END
}



VOID CBO_LOC_2140::OnTimer(UINT nIDEvent) 
{
	EFS_BEGIN

	CDialog::OnTimer(nIDEvent);

	EFS_END
}


HBRUSH CBO_LOC_2140::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

BOOL CBO_LOC_2140::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CBO_LOC_2140::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( message == (WM_USER + 2011) )
	{
		m_pSpfWork->Display_BookCheckProc();
	}	
	return CDialog::DefWindowProc(message, wParam, lParam);
}
