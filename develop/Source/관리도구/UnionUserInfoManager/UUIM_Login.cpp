// UUIM_Login.cpp : implementation file
//

#include "stdafx.h"
#include "unionuserinfomanager.h"
#include "UUIM_Login.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUUIM_Login dialog


CUUIM_Login::CUUIM_Login(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUUIM_Login)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUUIM_Login::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUUIM_Login)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUUIM_Login, CDialog)
	//{{AFX_MSG_MAP(CUUIM_Login)
	ON_EN_SETFOCUS(IDC_edtLOGIN_ID, OnSetfocusedtLOGINID)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUUIM_Login message handlers

BOOL CUUIM_Login::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 

	CFileFind FF;
	if(TRUE == FF.FindFile(_T("..\\cfg\\_debug")))
	{

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUUIM_Login::OnOK() 
{
	// TODO: Add extra validation here
	CString strID, strPassword;

	GetDlgItemText(IDC_edtLOGIN_ID, strID);
	GetDlgItemText(IDC_edtLOGIN_Password, strPassword);

	if(TRUE == strID.IsEmpty())
	{
		SetMessage(_T("ID를 입력하여 주십시요."));
// 		MessageBox(_T("ID를 입력하여 주십시요."), _T("로그인"), MB_OK);
		GetDlgItem(IDC_edtLOGIN_ID)->SetFocus();
		return;
	}
	if(TRUE == strPassword.IsEmpty())
	{
		SetMessage(_T("비밀번호를 입력하여 주십시요."));
// 		MessageBox(_T("비밀번호를 입력하여 주십시요."), _T("로그인"), MB_OK);
		GetDlgItem(IDC_edtLOGIN_Password)->SetFocus();
		return;
	}

	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO(_T(""));

	CString strQuery, strResult[2];
	strQuery.Format(_T("SELECT PASSWD, REC_KEY FROM MN_USER_TBL WHERE ID = '%s'"), strID);

	DM.GetValuesQuery(strQuery, strResult, 2);

	if(TRUE == strResult[1].IsEmpty())
	{
		SetMessage(_T("ID를 확인하여 주십시요."));
// 		MessageBox(_T("ID를 확인하여 주십시요."), _T("로그인"), MB_OK);
		GetDlgItem(IDC_edtLOGIN_ID)->SetFocus();
		return;
	}
	if(strPassword != strResult[0])
	{
		SetMessage(_T("비밀번호가 일치하지 않습니다."));
// 		MessageBox(_T("비밀번호가 일치하지 않습니다."), _T("로그인"), MB_OK);
		GetDlgItem(IDC_edtLOGIN_Password)->SetFocus();
		return;
	}

	m_strRecKey = strResult[1];
	CDialog::OnOK();
}

void CUUIM_Login::OnSetfocusedtLOGINID() 
{
	// TODO: Add your control notification handler code here
// 	SetIMEStatus(this, _T("한글"), IDC_edtLOGIN_ID);
}

void CUUIM_Login::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(1 == nIDEvent)
	{
		KillTimer(1);
		SetDlgItemText(IDC_staLOGIN_MSG, _T("※ 프로그램의 사용을 위해선 먼저 로그인하셔야 합니다."));
	}

	CDialog::OnTimer(nIDEvent);
}

void CUUIM_Login::SetMessage(CString strMsg, UINT nTime)
{
	SetDlgItemText(IDC_staLOGIN_MSG, strMsg);
	SetTimer(1, nTime, NULL);
}

BOOL CUUIM_Login::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			OnOK();
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
