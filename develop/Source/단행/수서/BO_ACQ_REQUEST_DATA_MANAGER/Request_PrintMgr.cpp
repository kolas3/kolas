// Request_PrintMgr.cpp : implementation file
//

#include "stdafx.h"
#include "Request_PrintMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequest_PrintMgr dialog


CRequest_PrintMgr::CRequest_PrintMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequest_PrintMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequest_PrintMgr)
	m_PrintType = -1;
	//}}AFX_DATA_INIT
}
CRequest_PrintMgr::~CRequest_PrintMgr()
{
}

VOID CRequest_PrintMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequest_PrintMgr)
	DDX_Radio(pDX, IDC_radREQUESTDATA, m_PrintType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequest_PrintMgr, CDialog)
	//{{AFX_MSG_MAP(CRequest_PrintMgr)
	ON_BN_CLICKED(IDC_radREQUESTDATA, OnradREQUESTDATA)
	ON_BN_CLICKED(IDC_radREQMAILSENDDATA, OnradREQMAILSENDDATA)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequest_PrintMgr message handlers

BOOL CRequest_PrintMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_PrintType = 0;
	UpdateData(FALSE);	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRequest_PrintMgr::OnradREQUESTDATA() 
{
	// TODO: Add your control notification handler code here
	m_PrintType = 0;
	UpdateData(FALSE);	
}

VOID CRequest_PrintMgr::OnradREQMAILSENDDATA() 
{
	// TODO: Add your control notification handler code here
	m_PrintType = 1;
	UpdateData(FALSE);	
}

VOID CRequest_PrintMgr::OnOK() 
{
	// TODO: Add extra validation here
	if(m_PrintType != 0 && m_PrintType != 1)
	{
		ESLAfxMessageBox(_T("출력항목을 선정해 주십시요."));
		return;
	}	
	CDialog::OnOK();
}

INT CRequest_PrintMgr::GetPrintType()
{
	return m_PrintType;
}

HBRUSH CRequest_PrintMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}