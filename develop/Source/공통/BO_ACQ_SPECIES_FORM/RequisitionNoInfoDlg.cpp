// RequisitionNoInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RequisitionNoInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequisitionNoInfoDlg dialog


CRequisitionNoInfoDlg::CRequisitionNoInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequisitionNoInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequisitionNoInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// KOL.UDF.022 시큐어코딩 보완
	m_X = 7;
	m_Y = 0;
	/*INT m_X = 7;
	INT m_Y = 0;*/
}


VOID CRequisitionNoInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequisitionNoInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequisitionNoInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CRequisitionNoInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequisitionNoInfoDlg message handlers

BOOL CRequisitionNoInfoDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CRequisitionNoInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
