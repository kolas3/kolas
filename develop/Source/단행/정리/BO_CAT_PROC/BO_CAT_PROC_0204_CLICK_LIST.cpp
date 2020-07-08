// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 

// BO_CAT_PROC_0204_CLICK_LIST.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0204_CLICK_LIST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0204_CLICK_LIST dialog


CBO_CAT_PROC_0204_CLICK_LIST::CBO_CAT_PROC_0204_CLICK_LIST(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0204_CLICK_LIST)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBO_CAT_PROC_0204_CLICK_LIST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0204_CLICK_LIST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0204_CLICK_LIST, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0204_CLICK_LIST)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0204_CLICK_LIST message handlers

BOOL CBO_CAT_PROC_0204_CLICK_LIST::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(InitESL_Mgr(_T("SM_BO_STATICS_LIST")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	CESL_DataMgr*	m_pDM = FindDM(_T("DM_BO_STATICS_LIST"));
	if(NULL == m_pDM)
	{
		AfxMessageBox(_T("DM_BO_STATICS_LIST Error!"));
		return FALSE;
	}

	CESL_Grid*		m_pGrid = (CESL_Grid*)FindControl(_T("CM_BO_STATICS_LIST"), _T("MainGrid"));
	if(NULL == m_pGrid)
	{
		AfxMessageBox(_T("CM_BO_STATICS_LIST Error!"));
		return FALSE;
	}	 
	
	m_pGrid->SetTextMatrix(0, 1, _T("번호") );
	m_pGrid->SetTextMatrix(0, 2, _T("제목") );

	m_pDM->RestructDataManager( m_strQuery );
	m_pGrid->Display(-1);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
