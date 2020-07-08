// TransferRegNo.cpp : implementation file
//

#include "stdafx.h"
#include "TransferRegNo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransferRegNo dialog


CTransferRegNo::CTransferRegNo(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CTransferRegNo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransferRegNo)
	m_strBookCnt = _T("");
	m_strSpeciesCnt = _T("");
	//}}AFX_DATA_INIT
}

CTransferRegNo::~CTransferRegNo()
{
}

VOID CTransferRegNo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransferRegNo)
	DDX_Text(pDX, IDC_edtBOOK_CNT, m_strBookCnt);
	DDX_Text(pDX, IDC_edtSPECIES_CNT, m_strSpeciesCnt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransferRegNo, CDialog)
	//{{AFX_MSG_MAP(CTransferRegNo)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferRegNo message handlers

BOOL CTransferRegNo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	UpdateData(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CTransferRegNo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
