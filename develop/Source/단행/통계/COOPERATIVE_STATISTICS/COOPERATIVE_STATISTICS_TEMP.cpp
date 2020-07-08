// COOPERATIVE_STATISTICS_TEMP.cpp : implementation file
//

#include "stdafx.h"
#include "COOPERATIVE_STATISTICS_TEMP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_TEMP dialog


CCOOPERATIVE_STATISTICS_TEMP::CCOOPERATIVE_STATISTICS_TEMP(CESL_Mgr* pParent, CWnd* pSearch, CWnd* pPreview)
	: CESL_Mgr(CCOOPERATIVE_STATISTICS_TEMP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCOOPERATIVE_STATISTICS_TEMP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// 검색조건, 미리보기화면 SHOW기본
	m_nSerchDlgSHOW = SW_SHOW;
	m_nPreviewDlgSHOW = SW_SHOW;
	
	m_pSearchDlg = pSearch;		// 검색조건 화면
	m_pPreviewDlg = pPreview;	// 미리보기 화면

}
CCOOPERATIVE_STATISTICS_TEMP::~CCOOPERATIVE_STATISTICS_TEMP()
{
}

void CCOOPERATIVE_STATISTICS_TEMP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCOOPERATIVE_STATISTICS_TEMP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCOOPERATIVE_STATISTICS_TEMP, CDialog)
	//{{AFX_MSG_MAP(CCOOPERATIVE_STATISTICS_TEMP)
	ON_BN_CLICKED(IDC_btnTITLE_COND2, OnbtnTITLECOND2)
	ON_BN_CLICKED(IDC_btnTITLE_COND, OnbtnTITLECOND)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnRunS, OnbtnRunS)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_TEMP message handlers

BOOL CCOOPERATIVE_STATISTICS_TEMP::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CCOOPERATIVE_STATISTICS_TEMP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );

	// 검색조건, 미리보기 화면생성
	CreateChildWnd();
	// 위치조정
	SetPosWnd();

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;
}

// ** 검색조건, 미리보기 화면생성
INT CCOOPERATIVE_STATISTICS_TEMP::CreateChildWnd()
{
EFS_BEGIN

	((CCOOPERATIVE_STATISTICS_WHERE*)m_pSearchDlg)->Create(this);
	m_pSearchDlg->ShowWindow(m_nSerchDlgSHOW);
	
	((CCOOPERATIVE_STATISTICS_PREVIEW*)m_pPreviewDlg)->Create(this);
	m_pPreviewDlg->ShowWindow(m_nPreviewDlgSHOW);

	return 0;

EFS_END
return -1;
}

// ** 위치조정
INT CCOOPERATIVE_STATISTICS_TEMP::SetPosWnd()
{
EFS_BEGIN

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
	if( m_pSearchDlg->GetSafeHwnd() == NULL ) return 0;
	m_pSearchDlg->GetWindowRect( CondWndPos );
	m_pSearchDlg->MoveWindow( 0 , TitleWndPos[0].bottom , CondWndPos.Width() , CondWndPos.Height() );
	m_pSearchDlg->GetWindowRect( CondWndPos );
	m_pSearchDlg->ShowWindow(m_nSerchDlgSHOW);
	ScreenToClient(CondWndPos);

	INT nTitleWnd1top = CondWndPos.bottom;
	if( m_nSerchDlgSHOW == SW_HIDE )
	{
		nTitleWnd1top = TitleWndPos[0].bottom;
	}

	pTitleWnd[1]->MoveWindow( 10 , nTitleWnd1top , WndPos.Width()-30 , TitleWndPos[1].Height() );

	INT nXYtop = TitleWndPos[0].bottom + TitleWndPos[1].Height();
	if( m_nSerchDlgSHOW == SW_SHOW )
	{
		nXYtop += CondWndPos.Height();
	}
	CRect XYWndPos;
	if( m_pPreviewDlg->GetSafeHwnd() == NULL ) return 0;
	m_pPreviewDlg->GetWindowRect( XYWndPos );
	m_pPreviewDlg->MoveWindow( 0 , nXYtop , WndPos.Width() - 10 , WndPos.Height() - nXYtop );
	m_pPreviewDlg->ShowWindow(m_nPreviewDlgSHOW);

	return 0;

EFS_END
return -1;
}

// ** 검색조건 화면 SHOW/HIDE
VOID CCOOPERATIVE_STATISTICS_TEMP::OnbtnTITLECOND() 
{
	if( m_nSerchDlgSHOW == SW_HIDE )
		m_nSerchDlgSHOW = SW_SHOW;
	else
		m_nSerchDlgSHOW = SW_HIDE;
	
	SetPosWnd();
}

// ** 미리보기 화면 SHOW/HIDE
VOID CCOOPERATIVE_STATISTICS_TEMP::OnbtnTITLECOND2() 
{
	if( m_nPreviewDlgSHOW == SW_HIDE )
		m_nPreviewDlgSHOW = SW_SHOW;
	else
		m_nPreviewDlgSHOW = SW_HIDE;
	
	SetPosWnd();
}

// ** 닫기
VOID CCOOPERATIVE_STATISTICS_TEMP::OnbtnCLOSE() 
{
	OnCancel();		
}

VOID CCOOPERATIVE_STATISTICS_TEMP::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL ) return;

	SetPosWnd();	
}

// ** 통계실행
VOID CCOOPERATIVE_STATISTICS_TEMP::OnbtnRunS() 
{
EFS_BEGIN

	INT ids;

	// 검색조건 검사
	ids = ((CCOOPERATIVE_STATISTICS_WHERE*)m_pSearchDlg)->SaveCondition();	
	if( ids < 0 )	return;

	// 행,열 검사
	ids = ((CCOOPERATIVE_STATISTICS_PREVIEW*)m_pPreviewDlg)->SaveXY();
	if( ids < 0 )	return ;	

	pParentMgr->SendMessage( WM_RUN_S );

EFS_END
}

void CCOOPERATIVE_STATISTICS_TEMP::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(m_pSearchDlg)
		((CCOOPERATIVE_STATISTICS_WHERE*)m_pSearchDlg)->SetControlSetting();
}
