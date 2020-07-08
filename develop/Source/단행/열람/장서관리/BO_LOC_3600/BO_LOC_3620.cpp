// BO_LOC_3620.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3620.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3620 dialog


CBO_LOC_3620::CBO_LOC_3620(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3620)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3620::~CBO_LOC_3620()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3620::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3620)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3620, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3620)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3620 message handlers

BOOL CBO_LOC_3620::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBO_LOC_3620::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CBO_LOC_3620::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
