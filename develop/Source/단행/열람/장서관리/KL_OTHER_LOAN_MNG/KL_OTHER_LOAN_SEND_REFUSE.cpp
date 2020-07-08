// KL_OTHER_LOAN_SEND_REFUSE.cpp : implementation file
//

#include "stdafx.h"
#include "KL_OTHER_LOAN_SEND_REFUSE.h"
#include "KL_OTHER_LOAN_DEFAULT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_SEND_REFUSE dialog


CKL_OTHER_LOAN_SEND_REFUSE::CKL_OTHER_LOAN_SEND_REFUSE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD,pParent)
{
	//{{AFX_DATA_INIT(CKL_OTHER_LOAN_SEND_REFUSE)
	// KOL.UDF.022
	m_strMessage = _T("");
	m_nSuccess = 0;
	m_bMessageSendYN = FALSE;
	//}}AFX_DATA_INIT
}

CKL_OTHER_LOAN_SEND_REFUSE::~CKL_OTHER_LOAN_SEND_REFUSE()
{

}

void CKL_OTHER_LOAN_SEND_REFUSE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_LOAN_SEND_REFUSE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_LOAN_SEND_REFUSE, CDialog)
	//{{AFX_MSG_MAP(CKL_OTHER_LOAN_SEND_REFUSE)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnSEND_REFUSE, Refuse)
	ON_BN_CLICKED(IDC_btnOnCancel, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_SEND_REFUSE message handlers


BOOL CKL_OTHER_LOAN_SEND_REFUSE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 16.10.20 JMJ KOL.RED.008
	m_bMessageSendYN = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKL_OTHER_LOAN_SEND_REFUSE::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

BOOL CKL_OTHER_LOAN_SEND_REFUSE::Create(CESL_Mgr *pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}


INT CKL_OTHER_LOAN_SEND_REFUSE::Refuse()
{
	GetDlgItemText(IDC_REFUSE_REASON_EDT,m_strMessage);
	
	// 16.10.20 JMJ KOL.RED.008
	m_bMessageSendYN = ((CButton*)GetDlgItem(IDC_CHK_SendRefuseSMS))->GetCheck();

	if(60 < m_strMessage.GetLength())
	{
		AfxMessageBox(_T("발송 메시지는 60자까지 입력 가능합니다."));
	}	
	else if(1 > m_strMessage.GetLength())
	{
		AfxMessageBox(_T("발송 메시지를 입력하세요"));
	}
	else
	{
		m_nSuccess = 1;
		OnCancel();
	}
	return 0;
}
