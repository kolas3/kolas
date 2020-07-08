// BO_LOC_3610.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3610.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3610 dialog


CBO_LOC_3610::CBO_LOC_3610(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3610)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3610::~CBO_LOC_3610()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3610::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3610)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3610, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3610)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3610 message handlers

BOOL CBO_LOC_3610::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_LOC_3610::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_LOC_3610::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	

//MACRODEL	CWnd* pTopBar = GetDlgItem( IDC_staTOPBAR );
//MACRODEL	if( pTopBar->GetSafeHwnd() == NULL ) return;
//MACRODEL
//MACRODEL	CRect TopBarRect;
//MACRODEL	pTopBar->GetWindowRect(TopBarRect);
//MACRODEL
//MACRODEL	pTopBar->MoveWindow( 0 , 0 , cx , TopBarRect.Height() );
//MACRODEL
//MACRODEL	CWnd* pGrid = GetDlgItem(IDC_gridMAIN);
//MACRODEL	if( pGrid->GetSafeHwnd() == NULL )
//MACRODEL		return;
//MACRODEL
//MACRODEL
//MACRODEL	pGrid->MoveWindow( 0, TopBarRect.Height() , cx , cy - TopBarRect.Height() );
	//	크기조절 표준화 위해 기존 소스 주석처리 재용 [2008-05-15 오후 3:22:05 ]  
	CWnd* pBar = GetDlgItem( IDC_staTOPBAR );
	if(NULL == pBar) return;	
	pBar->MoveWindow(0, 0, cx, 70);
	
	CWnd* pGrid = GetDlgItem(IDC_gridMAIN);
	if(NULL == pGrid) return;
	pGrid->MoveWindow(-2, 70 , cx+2 , cy);	
}

HBRUSH CBO_LOC_3610::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
