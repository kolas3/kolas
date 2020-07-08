// CheckinVolProgressive.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinVolProgressive.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolProgressive dialog


CCheckinVolProgressive::CCheckinVolProgressive(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCheckinVolProgressive::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinVolProgressive)
	m_strCheckinMessage = _T("");
	m_strCheckinMessageBottom = _T("");
	//}}AFX_DATA_INIT
}

CCheckinVolProgressive::~CCheckinVolProgressive()
{
}

VOID CCheckinVolProgressive::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinVolProgressive)
	DDX_Control(pDX, IDC_prgCHEKIN, m_prgCHECKIN);
	DDX_Text(pDX, IDC_stcCHE_PROGRESSIVE, m_strCheckinMessage);
	DDX_Text(pDX, IDC_stcCHK_PROGRESSIVE_BOTTOM, m_strCheckinMessageBottom);
	//}}AFX_DATA_MAP
}

BOOL CCheckinVolProgressive::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CCheckinVolProgressive, CDialog)
	//{{AFX_MSG_MAP(CCheckinVolProgressive)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolProgressive message handlers
VOID CCheckinVolProgressive::SetMessage(CString strMessage)
{
	m_strCheckinMessage = strMessage;
	UpdateData(FALSE);
}

VOID CCheckinVolProgressive::SetMessageBottom(CString strMessage)
{
	m_strCheckinMessageBottom = strMessage;
	UpdateData(FALSE);
}

VOID CCheckinVolProgressive::SetProGressRange(INT maxCnt)
{
	m_prgCHECKIN.SetRange(0, maxCnt);
}

VOID CCheckinVolProgressive::SetProGressPos(INT pos)
{
	m_prgCHECKIN.SetPos(pos);
	UpdateData(FALSE);
}

CButton* CCheckinVolProgressive::GetMessageCtrl()
{
	return (CButton*)GetDlgItem(IDC_stcCHE_PROGRESSIVE);
}

CProgressCtrl * CCheckinVolProgressive::GetProgressCtrl()
{
	return &m_prgCHECKIN;

}

HBRUSH CCheckinVolProgressive::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CCheckinVolProgressive::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
