// RECEIPT_SERARCH_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "RECEIPT_SERARCH_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRECEIPT_SERARCH_DLG dialog


CRECEIPT_SERARCH_DLG::CRECEIPT_SERARCH_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRECEIPT_SERARCH_DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRECEIPT_SERARCH_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CRECEIPT_SERARCH_DLG::~CRECEIPT_SERARCH_DLG()
{
}
VOID CRECEIPT_SERARCH_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRECEIPT_SERARCH_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRECEIPT_SERARCH_DLG, CDialog)
	//{{AFX_MSG_MAP(CRECEIPT_SERARCH_DLG)
	ON_BN_CLICKED(IDC_btnOK, OnbtnOK)
	ON_BN_CLICKED(IDC_btnLocloanCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRECEIPT_SERARCH_DLG message handlers

BOOL CRECEIPT_SERARCH_DLG::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CRECEIPT_SERARCH_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRECEIPT_SERARCH_DLG::OnbtnOK() 
{
	CString	strSerialNo;

	(( CEdit*) GetDlgItem (IDC_edtSERIALNO))->GetWindowText(strSerialNo);	

	if ( strSerialNo.IsEmpty() )
	{
		AfxMessageBox(_T("영수증일련번호를 입력하십시오."));
		return;
	}

	m_strSerialNo = strSerialNo;

	CDialog::OnOK();	
}

VOID CRECEIPT_SERARCH_DLG::OnbtnCLOSE() 
{
	CDialog::OnCancel();		
}

VOID CRECEIPT_SERARCH_DLG::GetSearchSerialNo(CString *f_strSerialNo)
{
	*f_strSerialNo = m_strSerialNo;
}

BOOL CRECEIPT_SERARCH_DLG::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			OnbtnOK();
			return TRUE;
		}

	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CRECEIPT_SERARCH_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
