// BO_ACQ_OrderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_OrderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_OrderDlg dialog


CBO_ACQ_OrderDlg::CBO_ACQ_OrderDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_OrderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_OrderDlg)
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pGroupCollection = NULL;
}


VOID CBO_ACQ_OrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_OrderDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_OrderDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_OrderDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_OrderDlg message handlers

BOOL CBO_ACQ_OrderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_구입자료주문")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_구입자료주문"));
	
	if(!Init())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_ACQ_OrderDlg::Init()
{
	CString strGroupInfo, strAcqYear, strWorkCode;
	strAcqYear = m_pGroupCollection->GetAcqYear();
	m_pGroupCollection->GetGroupInfo(strGroupInfo);
	strWorkCode = m_pGroupCollection->GetWorkCode();
	m_pCM->SetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->SetControlMgrData(_T("차수번호"), strGroupInfo);
	m_pCM->SetControlMgrData(_T("업무구분"), strWorkCode);

	SetTotalInfo();
	return TRUE;
}

VOID CBO_ACQ_OrderDlg::SetTotalInfo()
{
	CString strTotalPrice, strTotalBookCnt;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(this->CONNECTION_INFO);
	m_pGroupCollection->GetBookTotalInfo(&tmpDM, strTotalPrice, strTotalBookCnt);

	m_pCM->SetControlMgrData(_T("총책수"), strTotalBookCnt);
	m_pCM->SetControlMgrData(_T("총가격"), strTotalPrice);
}

VOID CBO_ACQ_OrderDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}
