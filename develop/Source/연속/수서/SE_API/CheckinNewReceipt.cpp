// CheckinNewReceipt.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinNewReceipt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinNewReceipt dialog


CCheckinNewReceipt::CCheckinNewReceipt(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCheckinNewReceipt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinNewReceipt)
	m_strNEW_RECEIPT_NO = _T("");
	m_strSTATIC_NEW_RECEIPT = _T("");
	//}}AFX_DATA_INIT
	m_pParentDM = NULL;
	m_nACQ_TYPE = 1; //default
	m_strACQ_YEAR = _T("");
}

CCheckinNewReceipt::~CCheckinNewReceipt()
{
}


VOID CCheckinNewReceipt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinNewReceipt)
	DDX_Text(pDX, IDC_edtCHECKIN_NEW_RECEIPT, m_strNEW_RECEIPT_NO);
	DDX_Text(pDX, IDC_stcCHECKIN_NEW_RECEIPT, m_strSTATIC_NEW_RECEIPT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinNewReceipt, CDialog)
	//{{AFX_MSG_MAP(CCheckinNewReceipt)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinNewReceipt message handlers

VOID CCheckinNewReceipt::OnOK() 
{
	// TODO: Add extra validation here
	// 기존에 있는 접수번호인가를 확인한다.
	UpdateData(TRUE);

	CString strCOUNT;
	CString strQuery;
	INT nCnt = 0;	

	//++2008.10.17 UPDATE BY PWR {{++
	// 통합시스템 적용
	strQuery.Format( _T("SELECT COUNT(*) FROM SE_RECEIPT_TBL ")
					 _T("WHERE ACQ_CODE='%d' AND ACQ_YEAR='%s' AND RECEIPT_NO=%s AND MANAGE_CODE=UDF_MANAGE_CODE")
					 , m_nACQ_TYPE, m_strACQ_YEAR, m_strNEW_RECEIPT_NO );
	// strQuery.Format(_T("SELECT COUNT(*) FROM SE_RECEIPT_TBL WHERE ACQ_CODE = '%d' AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s"),m_nACQ_TYPE,m_strACQ_YEAR,m_strNEW_RECEIPT_NO);
	//--2008.10.17 UPDATE BY PWR --}}
	m_pParentDM->GetOneValueQuery(strQuery,strCOUNT);
	
	if(!strCOUNT.IsEmpty())
	{
		nCnt = _ttoi(strCOUNT.GetBuffer(0));
	}

	if(0 != nCnt)
	{
		AfxMessageBox(_T("기존에 존재하는 접수번호입니다!"));
		return;
	}
	

	CDialog::OnOK();
}

HBRUSH CCheckinNewReceipt::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CCheckinNewReceipt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
