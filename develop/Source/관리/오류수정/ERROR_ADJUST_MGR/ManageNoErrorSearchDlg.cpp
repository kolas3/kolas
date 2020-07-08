// ManageNoErrorSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ManageNoErrorSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManageNoErrorSearchDlg dialog


CManageNoErrorSearchDlg::CManageNoErrorSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CErrorAdjustSearchMgr(CManageNoErrorSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManageNoErrorSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CManageNoErrorSearchDlg::~CManageNoErrorSearchDlg()
{
	
} 

VOID CManageNoErrorSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManageNoErrorSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManageNoErrorSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CManageNoErrorSearchDlg)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManageNoErrorSearchDlg message handlers

BOOL CManageNoErrorSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(!Init())
	{
		ESLAfxMessageBox(_T("관리번호오류정보 검색 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CManageNoErrorSearchDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CManageNoErrorSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)GetDlgItem(IDC_cmbACQ_CODE);
	if(pCMB == NULL) return ;
	
	INT nCurSel = pCMB->GetCurSel();
	if(nCurSel == 0)
	{
		m_strQuery.Format(
			_T("I.REC_KEY=S.REC_KEY AND I.REC_KEY=B.SPECIES_KEY AND ")
			_T("b.reg_no IS NOT NULL and length(rtrim(ltrim(b.reg_no))) < 4 AND ")
			_T("B.ACQ_CODE='1'"));
	}
	else if(nCurSel == 1)
	{
		m_strQuery.Format(
			_T("I.REC_KEY=S.REC_KEY AND I.REC_KEY=B.SPECIES_KEY AND ")
			_T("b.reg_no IS NOT NULL and length(rtrim(ltrim(b.reg_no))) < 4 AND ")
			_T("B.ACQ_CODE='2'"));
	}		 
	else if(nCurSel == 2)
	{
		m_strQuery.Format(
			_T("I.REC_KEY=S.REC_KEY AND I.REC_KEY=B.SPECIES_KEY AND ")
			_T("b.reg_no IS NOT NULL and length(rtrim(ltrim(b.reg_no))) < 4 AND ")
			_T("(B.ACQ_CODE != '1' OR B.ACQ_CODE != '2' OR B.ACQ_CODE IS NULL)"));
	}
	SendQueryCreateMsg(nCurSel);
}

BOOL CManageNoErrorSearchDlg::Init()
{
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)GetDlgItem(IDC_cmbACQ_CODE);
	if(pCMB == NULL) return FALSE;
	pCMB->ResetContent();
	pCMB->AddString(_T("구입"));
	pCMB->AddString(_T("기증"));
	pCMB->AddString(_T("이외자료"));
	pCMB->SetCurSel(0);	
	return TRUE;
}

VOID CManageNoErrorSearchDlg::SendQueryCreateMsg(INT nData)
{
	::SendMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, nData, 0);
}


HBRUSH CManageNoErrorSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
