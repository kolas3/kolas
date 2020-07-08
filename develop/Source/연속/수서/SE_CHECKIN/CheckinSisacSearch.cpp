// CheckinSisacSearch.cpp : implementation file
//

#include "stdafx.h"
//#include "se_checkin.h"
#include "CheckinSisacSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinSisacSearch dialog


CCheckinSisacSearch::CCheckinSisacSearch(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCheckinSisacSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinSisacSearch)
	m_strSisacData = _T("");
	//}}AFX_DATA_INIT
}

CCheckinSisacSearch::~CCheckinSisacSearch()
{
}

void CCheckinSisacSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinSisacSearch)
	DDX_Text(pDX, IDC_EDIT1, m_strSisacData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinSisacSearch, CDialog)
	//{{AFX_MSG_MAP(CCheckinSisacSearch)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinSisacSearch message handlers

BOOL CCheckinSisacSearch::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			Search(); 
			return TRUE;
		}
		else if( pMsg->wParam == VK_ESCAPE)
		{
			ESC(); 
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CCheckinSisacSearch::Search()
{
	UpdateData(TRUE);
	CDialog::OnOK();
}

void CCheckinSisacSearch::ESC()
{
	CDialog::OnCancel();
}
HBRUSH CCheckinSisacSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CCheckinSisacSearch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
