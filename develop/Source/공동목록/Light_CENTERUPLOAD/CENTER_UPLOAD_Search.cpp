// CENTER_UPLOAD_Search.cpp : implementation file
//

#include "stdafx.h"

#include "CENTER_UPLOAD_Search.h"
#include "CENTER_UPLOAD_01.h"

#include "UC_KOLISNetUseChecker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCENTER_UPLOAD_Search dialog


CCENTER_UPLOAD_Search::CCENTER_UPLOAD_Search(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCENTER_UPLOAD_Search)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// KOL.UDF.022 시큐어코딩 보완
	m_pBaseDataMgr = NULL;
}

CCENTER_UPLOAD_Search::~CCENTER_UPLOAD_Search()
{
}


VOID CCENTER_UPLOAD_Search::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCENTER_UPLOAD_Search)
//	DDX_Control(pDX, IDC_PROGRESS_search, m_progress_search);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCENTER_UPLOAD_Search, CDialog)
	//{{AFX_MSG_MAP(CCENTER_UPLOAD_Search)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCENTER_UPLOAD_Search message handlers


BOOL CCENTER_UPLOAD_Search::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CCENTER_UPLOAD_Search::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_CENTER_UPLOAD_SEARCH")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
   

    m_pBaseDataMgr = FindDM(_T("DM_CENTERUPLOAD_01")) ;
	if(m_pBaseDataMgr == NULL) 
	{
       AfxMessageBox (_T("FindDM() Error")) ;
	   return FALSE ;
	}
    
    

	CESL_ControlMgr *pCM = FindCM(_T("CM_CENTERUPLOAD_SEARCH"));
	AllControlDisplay (_T("CM_CENTERUPLOAD_SEARCH"),-1) ;
	
	UC_KOLISNetUseChecker KolisnetUseChk(this);
	if ( KolisnetUseChk.GetUseFlag() == FALSE )
	{
		CDialog::OnCancel();
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_CENTERUPLOAD_SEARCH"), _T("번호부터"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);		

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_CENTERUPLOAD_SEARCH"), _T("번호까지"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		
		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCENTER_UPLOAD_Search::OnOK() 
{
	CCENTER_UPLOAD_01 *pParentDlg = (CCENTER_UPLOAD_01*)pParentMgr;
	pParentDlg->OnSearch(this);
}

VOID CCENTER_UPLOAD_Search::OnCancel() 
{	
	CDialog::OnCancel();
}


HBRUSH CCENTER_UPLOAD_Search::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
