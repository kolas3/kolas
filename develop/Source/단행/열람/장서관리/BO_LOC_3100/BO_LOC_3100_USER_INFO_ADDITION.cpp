// BO_LOC_3100_USER_INFO_ADDITION.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_31001.h"
#include "BO_LOC_3100_USER_INFO_ADDITION.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBO_LOC_3100_USER_INFO_ADDITION::CBO_LOC_3100_USER_INFO_ADDITION(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_LOC_3100_USER_INFO_ADDITION::IDD, pParent)
{
	m_strUserAccessAuth = _T("");
}


void CBO_LOC_3100_USER_INFO_ADDITION::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3100_USER_INFO_ADDITION)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3100_USER_INFO_ADDITION, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3100_USER_INFO_ADDITION)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3100_USER_INFO_ADDITION message handlers

BOOL CBO_LOC_3100_USER_INFO_ADDITION::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT	ids	=	InitESL_Mgr(_T("BO_LOC_3100_USER_INFO_ADDITION"));
	if( ids < 0 )
	{
		AfxMessageBox( _T("Init Refrence Error") );
	}

	if( TRUE == m_strUserAccessAuth.IsEmpty() ) 
	{
		m_strUserAccessAuth = _T("1");
	}
	if (m_strUserAccessAuth == _T("1") ){
		((CEdit*)GetDlgItem(IDC_EDIT_MOBILE_PHONE))->SetPasswordChar(0x2022);
		((CEdit*)GetDlgItem(IDC_EDIT_HOME_PHONE))->SetPasswordChar(0x2022);
		((CEdit*)GetDlgItem(IDC_EDIT_EMAIL))->SetPasswordChar(0x2022);
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_LOC_3100_USER_INFO_ADDITION::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_ESCAPE )
		{
			GetParent()->PostMessage(WM_KEYDOWN, VK_ESCAPE);
			return TRUE;
		}	
		else if ( pMsg->wParam == VK_RETURN )
		{
			return TRUE;
		}
	}	

	return CDialog::PreTranslateMessage(pMsg);
}
