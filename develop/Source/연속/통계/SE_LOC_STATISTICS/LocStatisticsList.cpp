// LocStatisticsList.cpp : implementation file
//

#include "stdafx.h"
#include "LocStatisticsList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsList dialog


CLocStatisticsList::CLocStatisticsList(CESL_Mgr* pParent /*=NULL*/, CESL_DataMgr* pSourceDM /*=NULL*/)
	: CESL_Mgr(CLocStatisticsList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocStatisticsList)
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pSourceDM = pSourceDM;
	//}}AFX_DATA_INIT
}


void CLocStatisticsList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocStatisticsList)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocStatisticsList, CDialog)
	//{{AFX_MSG_MAP(CLocStatisticsList)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsList message handlers
BOOL CLocStatisticsList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	if(InitESL_Mgr(_T("연속_열람_통계_대상목록")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	m_pDM = FindDM(_T("DM_연속_열람_통계_대상목록"));
	if(NULL == m_pDM)
	{
		AfxMessageBox(_T("DM_BO_STATICS_LIST Error!"));
		return FALSE;
	}

	m_pGrid = (CESL_Grid*)FindControl(_T("CM_연속_열람_통계_대상목록"), _T("MainGrid"));
	if(NULL == m_pGrid)
	{
		AfxMessageBox(_T("CM_연속_열람_통계_대상목록 Error!"));
		return FALSE;
	}	 
	
	m_pDM->CopyDM(m_pSourceDM);
	m_pGrid->Display(-1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}