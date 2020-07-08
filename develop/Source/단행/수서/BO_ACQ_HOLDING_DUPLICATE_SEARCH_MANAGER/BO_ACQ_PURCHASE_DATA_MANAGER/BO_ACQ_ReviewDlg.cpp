// BO_ACQ_ReviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_ReviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_ReviewDlg dialog


CBO_ACQ_ReviewDlg::CBO_ACQ_ReviewDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_ReviewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_ReviewDlg)
	m_bReviewResultPrint = FALSE;
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pGroupCollection = NULL;
}


VOID CBO_ACQ_ReviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_ReviewDlg)
	DDX_Check(pDX, IDC_chkREVIEW_RESULT_PRINT, m_bReviewResultPrint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_ReviewDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_ReviewDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_ReviewDlg message handlers

BOOL CBO_ACQ_ReviewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_구입자료심의")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_구입자료심의"));
	
	if(!Init())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_ACQ_ReviewDlg::Init()
{
	if(m_pGroupCollection == NULL)	
	{
		ESLAfxMessageBox(_T("차수정보가 올바르지 않습니다."));
		return FALSE;
	}
	CString strGroupInfo, strAcqYear, strWorkCode;
	strAcqYear = m_pGroupCollection->GetAcqYear();
	strWorkCode = m_pGroupCollection->GetWorkCode();
	m_pGroupCollection->GetGroupInfo(strGroupInfo);
	m_pCM->SetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->SetControlMgrData(_T("차수번호"), strGroupInfo);
	m_pCM->SetControlMgrData(_T("업무구분"), strWorkCode);
	
	SetTotalInfo();
	return TRUE;
}

VOID CBO_ACQ_ReviewDlg::SetTotalInfo()
{
	CString strTotalPrice, strTotalBookCnt;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(this->CONNECTION_INFO);
	m_pGroupCollection->GetBookTotalInfo(&tmpDM, strTotalPrice, strTotalBookCnt);

	m_pCM->SetControlMgrData(_T("총책수"), strTotalBookCnt);
	m_pCM->SetControlMgrData(_T("총가격"), strTotalPrice);
}

VOID CBO_ACQ_ReviewDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	CDialog::OnOK();
}
