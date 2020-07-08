// UserInfo.cpp : implementation file
//

#include "stdafx.h"
#include "kolas2up_setup.h"
#include "UserInfo.h"
#include "ESL_Mgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserInfo dialog
IMPLEMENT_DYNCREATE(CUserInfo, CPropertyPage)

CUserInfo::CUserInfo() : CPropertyPage(CUserInfo::IDD)	
{
	//{{AFX_DATA_INIT(CUserInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CUserInfo::~CUserInfo()
{
}


VOID CUserInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserInfo, CDialog)
	//{{AFX_MSG_MAP(CUserInfo)
	ON_BN_CLICKED(IDC_bSign, OnbSign)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserInfo message handlers

VOID CUserInfo::OnbSign() 
{	

	CString strID;
	CString strPass;
	CString strExec;

	( (CEdit*) GetDlgItem (IDC_EDIT1) )->GetWindowText(strID);
	( (CEdit*) GetDlgItem (IDC_EDIT2) )->GetWindowText(strPass);

	if ( strID==_T("") || strPass==_T("") ) 
	{
		AfxMessageBox(_T("ID 또는 암호가 잘못되었습니다. 다시 입력하여 주십시오."));
		return;
	}	

	( (CEdit*) GetDlgItem (IDC_EDIT2) )->SetWindowText(_T(""));


	strExec.Format(_T("UserInfoManager.exe %s %s"), strID, strPass);	
	CHAR szExec[256];
	
	WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)strExec, -1, szExec, 256, NULL,  NULL);
	::WinExec(szExec ,SW_SHOW);	
}

BOOL CUserInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	( (CEdit*) GetDlgItem (IDC_EDIT1) )->SetLimitText(12);
	( (CEdit*) GetDlgItem (IDC_EDIT2) )->SetLimitText(8);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CUserInfo::PreTranslateMessage(MSG* pMsg) 
{
	/*
	if ( pMsg->lParam == VK_RETURN && pMsg->message == WM_KEYDOWN )
	{
		OnbSign();
		return TRUE;
	}
	*/
	
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CUserInfo::OnApply() 
{
//DEL  	OnbSign();
	
	return CPropertyPage::OnApply();
}