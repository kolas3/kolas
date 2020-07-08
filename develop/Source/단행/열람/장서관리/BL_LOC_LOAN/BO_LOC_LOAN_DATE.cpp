#include "stdafx.h"
#include "BO_LOC_LOAN_DATE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBO_LOC_LOAN_DATE::CBO_LOC_LOAN_DATE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_LOC_LOAN_DATE::IDD, pParent)
{	
	m_nLoanDay = 0;	
}

CBO_LOC_LOAN_DATE::~CBO_LOC_LOAN_DATE()
{
}
VOID CBO_LOC_LOAN_DATE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_LOAN_DATE)	
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CBO_LOC_LOAN_DATE, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_LOAN_DATE)
	ON_BN_CLICKED(IDC_btnOK, OnbtnOK)
	ON_BN_CLICKED(IDC_btnLOAN_CANCEL, OnbtnCANCEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CBO_LOC_LOAN_DATE::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_LOAN_DATE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	strLoanDay;

	strLoanDay.Format(_T("%d"), m_nLoanDay );
	( (CEdit*) GetDlgItem (IDC_edtINPUT_LOAN_DAY) )->SetWindowText(strLoanDay);	
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  
	              
}

VOID CBO_LOC_LOAN_DATE::OnbtnOK() 
{
	CString strDay;
	( (CEdit*) GetDlgItem (IDC_edtINPUT_LOAN_DAY) )->GetWindowText(strDay);
	m_nLoanDay = _ttoi(strDay);
	
	CDialog::OnOK();		
}

VOID CBO_LOC_LOAN_DATE::OnbtnCANCEL() 
{
	CDialog::OnCancel();		
}

VOID CBO_LOC_LOAN_DATE::SetLoanExpDate(CString strLoanDay)
{		
	m_nLoanDay = _ttoi(strLoanDay);
}

VOID CBO_LOC_LOAN_DATE::SetLoanExpDate(INT	nLoanDay)
{		
	m_nLoanDay = nLoanDay;
}

INT CBO_LOC_LOAN_DATE::GetLoanExpDate()
{
	return m_nLoanDay;
}

BOOL CBO_LOC_LOAN_DATE::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnbtnOK();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CBO_LOC_LOAN_DATE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}
