// AddNoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AddNoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddNoDlg dialog


CAddNoDlg::CAddNoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CAddNoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddNoDlg)
	m_sNewSeqNo = _T("");
	m_stcNewSeqNo = _T("");
	//}}AFX_DATA_INIT

	m_pParentDM	= NULL;
	m_sAcqYear	= _T("");
}

CAddNoDlg::~CAddNoDlg()
{
}


VOID CAddNoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddNoDlg)
	DDX_Text(pDX, IDC_eNEW_SEQNO, m_sNewSeqNo);
	DDX_Text(pDX, IDC_stcNEW_SEQNO, m_stcNewSeqNo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddNoDlg, CDialog)
	//{{AFX_MSG_MAP(CAddNoDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddNoDlg message handlers

BOOL CAddNoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAddNoDlg::OnOK() 
{
	// 기존에 있는 접수번호인가를 확인한다.
	UpdateData(TRUE);

	CString strCOUNT;
	CString strQuery;
	INT nCnt = 0;	

	strQuery.Format(_T("SELECT COUNT(*) FROM SE_BINDING_SEQ_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s"), m_sAcqYear, m_sNewSeqNo );
	m_pParentDM->GetOneValueQuery(strQuery,strCOUNT);
	
	if(!strCOUNT.IsEmpty())	{
		nCnt = _ttoi(strCOUNT.GetBuffer(0));
	}

	if(0 != nCnt) {
		AfxMessageBox(_T("기존에 존재하는 접수번호입니다!"));
		return ;
	}
	
	CDialog::OnOK();
}

HBRUSH CAddNoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
