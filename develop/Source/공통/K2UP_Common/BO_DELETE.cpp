// BO_DELETE.cpp : implementation file
//

#include "stdafx.h"
#include "BO_DELETE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_DELETE dialog


CBO_DELETE::CBO_DELETE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_DELETE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_DELETE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// KOL.UDF.022 시큐어코딩 보완
	m_IsDelete = FALSE;
	
}


VOID CBO_DELETE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_DELETE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_DELETE, CDialog)
	//{{AFX_MSG_MAP(CBO_DELETE)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_rRECALL, OnrRECALL)
	ON_BN_CLICKED(IDC_rDELETE, OnrDELETE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_DELETE message handlers

VOID CBO_DELETE::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	EndDialog(IDCANCEL);
}

BOOL CBO_DELETE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CButton * pRad = (CButton*)GetDlgItem(IDC_rRECALL);
	pRad->SetCheck(1);
	m_IsDelete = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_DELETE::OnrRECALL() 
{
	// TODO: Add your control notification handler code here
	m_IsDelete = FALSE;
}

VOID CBO_DELETE::OnrDELETE() 
{
	// TODO: Add your control notification handler code here
	m_IsDelete = TRUE;
}

HBRUSH CBO_DELETE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
