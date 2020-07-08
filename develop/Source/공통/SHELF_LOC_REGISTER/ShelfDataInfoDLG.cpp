// ShelfDataInfoDLG.cpp : implementation file
//

#include "stdafx.h"
#include "shelf_loc_register.h"
#include "ShelfDataInfoDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShelfDataInfoDLG dialog


CShelfDataInfoDLG::CShelfDataInfoDLG(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CShelfDataInfoDLG)
	//}}AFX_DATA_INIT
	bModify = FALSE;
}
CShelfDataInfoDLG::~CShelfDataInfoDLG()
{
}


void CShelfDataInfoDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShelfDataInfoDLG)
	DDX_Control(pDX, IDC_EDT_SHELFROW_MOD, m_SHELFROW_MOD);
	DDX_Control(pDX, IDC_EDT_SHELFNUM_MOD, m_SHELFNUM_MOD);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShelfDataInfoDLG, CDialog)
	//{{AFX_MSG_MAP(CShelfDataInfoDLG)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_MODIFY, OnBtnModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShelfDataInfoDLG message handlers

HBRUSH CShelfDataInfoDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CShelfDataInfoDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SHELFNUM_MOD.SetWindowText(m_strShelfNum_MOD);
	m_SHELFROW_MOD.SetWindowText(m_strShelfRow_MOD);
	
	if(nBO_SE_FLAG == 0)
	{
		if (InitESL_Mgr(_T("SM_SHELF_LOC_REG_01")) < 0)
		{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}
	}
	else
	{
		if (InitESL_Mgr(_T("SM_SHELF_LOC_REG_02")) < 0)
		{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}
	}
	
	if(nBO_SE_FLAG == 0)
	{
		controlMgr = FindCM(_T("CM_SHELF_LOC_REG_BO"));
	}
	else
	{
		controlMgr = FindCM(_T("CM_SHELF_LOC_REG_SE"));
	}
	
	controlMgr->AllControlDisplay(0);

	GetDlgItemText(IDC_EDT_SHELFNUM_PRE,m_strShelfNum_PRE);
	GetDlgItemText(IDC_EDT_SHELFROW_PRE,m_strShelfRow_PRE);
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShelfDataInfoDLG::OnBtnModify() 
{
	// TODO: Add your control notification handler code here
	bModify = TRUE;
	GetDlgItemText(IDC_EDT_SHELFNUM_MOD,m_strShelfNum_MOD);
	GetDlgItemText(IDC_EDT_SHELFROW_MOD,m_strShelfRow_MOD);
	OnOK();
}

void CShelfDataInfoDLG::GetPreShelfInfo(CString &strShelfNum_PRE,CString &strShelfRow_PRE)
{ 	
	strShelfNum_PRE=m_strShelfNum_PRE;
	strShelfRow_PRE=m_strShelfRow_PRE;
}

BOOL CShelfDataInfoDLG::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->lParam == VK_RETURN )
		{
			OnBtnModify();
			return TRUE;
		}
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}
