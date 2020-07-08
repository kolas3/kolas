// TmpDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TmpDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTmpDialog dialog


CTmpDialog::CTmpDialog(CESL_Mgr* pParent,CWnd* pCondWnd,CWnd* pXYSettingWnd)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTmpDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	nShowCndWnd = SW_SHOW;
	nShowXYWnd = SW_SHOW;

	m_pCondWnd = pCondWnd;
	m_pXYSettingWnd = pXYSettingWnd;
}

CTmpDialog::~CTmpDialog()
{
}

VOID CTmpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTmpDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTmpDialog, CDialog)
	//{{AFX_MSG_MAP(CTmpDialog)
	ON_BN_CLICKED(IDC_btnTITLE_COND2, OnbtnTITLECOND2)
	ON_BN_CLICKED(IDC_btnTITLE_COND, OnbtnTITLECOND)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnRunS, OnbtnRunS)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTmpDialog message handlers

BOOL CTmpDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );
	
	CreateChildWnd();
	
	SetPosWnd();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}


BOOL CTmpDialog::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

INT CTmpDialog::SetPosWnd()
{
	
	CRect WndPos;
	GetWindowRect(WndPos);

	// 1. 타이틀 위치 잡기
	const INT nCnt = 2;
	CRect TitleWndPos[nCnt];
	CWnd* pTitleWnd[nCnt];
	INT nTitleIDC[nCnt] = 
	{
		IDC_btnTITLE_COND , IDC_btnTITLE_COND2
	};
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		pTitleWnd[i] = GetDlgItem(nTitleIDC[i]);
		if( pTitleWnd[i]->GetSafeHwnd() == NULL  ) return 0;
		pTitleWnd[i]->GetWindowRect(TitleWndPos[i]);
		ScreenToClient(TitleWndPos[i]);
	}


	pTitleWnd[0]->MoveWindow( 10 , TitleWndPos[0].top , WndPos.Width()-30 , TitleWndPos[0].Height() );
	
	// 조건 구성 윈도우 위치 잡기
	CRect CondWndPos;
	if( m_pCondWnd->GetSafeHwnd() == NULL ) return 0;
	m_pCondWnd->GetWindowRect( CondWndPos );
	m_pCondWnd->MoveWindow( 0 , TitleWndPos[0].bottom , CondWndPos.Width() , CondWndPos.Height() );
	m_pCondWnd->GetWindowRect( CondWndPos );
	m_pCondWnd->ShowWindow(nShowCndWnd);
	ScreenToClient(CondWndPos);

	INT nTitleWnd1top = CondWndPos.bottom;
	if( nShowCndWnd == SW_HIDE )
	{
		nTitleWnd1top = TitleWndPos[0].bottom;
	}

	pTitleWnd[1]->MoveWindow( 10 , nTitleWnd1top , WndPos.Width()-30 , TitleWndPos[1].Height() );

	INT nXYtop = TitleWndPos[0].bottom + TitleWndPos[1].Height();
	if( nShowCndWnd == SW_SHOW )
	{
		nXYtop += CondWndPos.Height();
	}
	CRect XYWndPos;
	if( m_pXYSettingWnd->GetSafeHwnd() == NULL ) return 0;
	m_pXYSettingWnd->GetWindowRect( XYWndPos );
	m_pXYSettingWnd->MoveWindow( 0 , nXYtop , WndPos.Width() - 10 , WndPos.Height() - nXYtop );
	m_pXYSettingWnd->ShowWindow(nShowXYWnd);
	
	return 0;
	

}


INT CTmpDialog::CreateChildWnd()
{
	if(1 == m_nTYPE)
	{
		((CLocStatisticsCond1*)m_pCondWnd)->Create(this);
	}
	else if(2 == m_nTYPE)
	{
		((CLocStatisticsCond2*)m_pCondWnd)->Create(this);
	}
	else if(3 == m_nTYPE)
	{

		((CLocStatisticsCond3*)m_pCondWnd)->Create(this);
	}

	m_pCondWnd->ShowWindow(nShowCndWnd);

	((CLocStatisticsAxis*)m_pXYSettingWnd)->Create(this);
	m_pXYSettingWnd->ShowWindow( nShowXYWnd );


	return 0;

}


VOID CTmpDialog::OnbtnTITLECOND2() 
{
	if( nShowXYWnd == SW_HIDE )
		nShowXYWnd = SW_SHOW;
	else
		nShowXYWnd = SW_HIDE;
	
	SetPosWnd();
}

VOID CTmpDialog::OnbtnTITLECOND() 
{
	if( nShowCndWnd == SW_HIDE )
		nShowCndWnd = SW_SHOW;
	else
		nShowCndWnd = SW_HIDE;
	
	SetPosWnd();
}

VOID CTmpDialog::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL ) return;

	SetPosWnd();	
}

VOID CTmpDialog::OnbtnCLOSE() 
{
	OnCancel();	
}


VOID CTmpDialog::OnbtnRunS() 
{
	
	// 조건 검사
	INT ids;

	if(1 == m_nTYPE)
	{
		ids = ((CLocStatisticsCond1*)m_pCondWnd)->SaveCondition();
	}
	else if(2 == m_nTYPE)
	{
		ids = ((CLocStatisticsCond2*)m_pCondWnd)->SaveCondition();
	}
	else if(3 == m_nTYPE)
	{
		ids = ((CLocStatisticsCond3*)m_pCondWnd)->SaveCondition();
	}
	
	if(ids<0) return;
	
	pParentMgr->SendMessage( WM_RUN_S );
	
}

HBRUSH CTmpDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
