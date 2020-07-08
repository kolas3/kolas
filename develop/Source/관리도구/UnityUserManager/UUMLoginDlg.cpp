// UUMLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnityUserManager.h"
#include "UUMLoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUUMLoginDlg dialog


CUUMLoginDlg::CUUMLoginDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CUUMLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUUMLoginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUUMLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUUMLoginDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUUMLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CUUMLoginDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUUMLoginDlg message handlers

void CUUMLoginDlg::OnOK() 
{
	CString strID, strPassword;

	GetDlgItemText(IDC_edtLOGIN_ID, strID);
	GetDlgItemText(IDC_edtLOGIN_Password, strPassword);

	if(TRUE == strID.IsEmpty())
	{
		SetMessage(_T("ID를 입력하여 주십시요."));
		GetDlgItem(IDC_edtLOGIN_ID)->SetFocus();
		return;
	}
	if(TRUE == strPassword.IsEmpty())
	{
		SetMessage(_T("비밀번호를 입력하여 주십시요."));
		GetDlgItem(IDC_edtLOGIN_Password)->SetFocus();
		return;
	}

	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO(_T(""));

	CString strQuery;
	strQuery.Format(_T("SELECT REC_KEY, ID, PASSWD, NAME FROM MN_USER_TBL WHERE ID='%s'"), strID);
	DM.RestructDataManager(strQuery);

	if(TRUE == DM.GetCellData(0, 0).IsEmpty())
	{
		SetMessage(_T("ID를 확인하여 주십시요."));
		GetDlgItem(IDC_edtLOGIN_ID)->SetFocus();
		return;
	}
	if(strPassword != DM.GetCellData(0, 2))
	{
		SetMessage(_T("비밀번호가 일치하지 않습니다."));
		GetDlgItem(IDC_edtLOGIN_Password)->SetFocus();
		return;
	}

	m_strRecKey = DM.GetCellData(0, 0);
	m_strID = DM.GetCellData(0, 1);
	m_strName = DM.GetCellData(0, 3);

	CDialog::OnOK();
}

void CUUMLoginDlg::SetMessage(CString strMsg, UINT nTime)
{
	SetDlgItemText(IDC_staLOGIN_MSG, strMsg);
	SetTimer(1, nTime, NULL);
}

BOOL CUUMLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CUUMLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			OnOK();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CUUMLoginDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(1 == nIDEvent)
	{
		KillTimer(1);
		SetDlgItemText(IDC_staLOGIN_MSG, _T("※ 프로그램의 사용을 위해선 먼저 로그인하셔야 합니다."));
	}

	CDialog::OnTimer(nIDEvent);
}
