// RESEARCH_RESULT_DESCTYPE.cpp : implementation file
//

#include "stdafx.h"
#include "RESEARCH_RESULT_DESCTYPE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRESEARCH_RESULT_DESCTYPE dialog


CRESEARCH_RESULT_DESCTYPE::CRESEARCH_RESULT_DESCTYPE(CESL_Mgr* pParent )
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CRESEARCH_RESULT_DESCTYPE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strResearchKey    = _T("") ;
	m_strQuestionInfoKey = _T("") ;
}

CRESEARCH_RESULT_DESCTYPE::~CRESEARCH_RESULT_DESCTYPE()
{
	
}

BOOL CRESEARCH_RESULT_DESCTYPE::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CRESEARCH_RESULT_DESCTYPE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRESEARCH_RESULT_DESCTYPE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRESEARCH_RESULT_DESCTYPE, CDialog)
	//{{AFX_MSG_MAP(CRESEARCH_RESULT_DESCTYPE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRESEARCH_RESULT_DESCTYPE message handlers

BOOL CRESEARCH_RESULT_DESCTYPE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	//SM_RESEARCH_DESCTYPE
	//CM_RESEARCH_RESULT_DESCTYPE
	//DM_RESEARCH_RESULT_DESCTYPE


	if(InitESL_Mgr(_T("SM_RESEARCH_DESCTYPE")) <0)
	{
		AfxMessageBox(_T("InitESL_Mgr () Failed")) ;
		return FALSE ;
	}


	if(m_strResearchKey == _T("")  || m_strQuestionInfoKey == _T("") )
	{
		AfxMessageBox (_T("RESEARKEY OR QUESTION_INFO_KEY Failed")) ;
		return FALSE ;
	}

	CString strQuery ;
	strQuery.Format (_T(" research_key = %s and QUESTION_INFO_KEY = %s and TYPE = '¼­¼úÇü' "),m_strResearchKey,m_strQuestionInfoKey) ;

    CESL_DataMgr *pDM = FindDM(_T("DM_RESEARCH_RESULT_DESCTYPE"));
	pDM->RefreshDataManager (strQuery) ;


	CESL_ControlMgr *pCM = FindCM(_T("CM_RESEARCH_RESULT_DESCTYPE")) ;
	if(pCM == NULL)
	{
		AfxMessageBox (_T("FindCM() Failed")) ;
		return FALSE ;
	}
	
	CESL_Grid *pGrid = (CESL_Grid *)pCM->FindControl (_T("MainGrid")) ;
	if(pGrid == NULL) 
	{
		AfxMessageBox (_T("FindControl() Failed")) ;
		return FALSE ;
	}


	pGrid->Display (-1) ;

	for(INT i = 0 ; i < pGrid->GetRows ()-1 ;i++)
	{
		pGrid->SetRowHeight (i+1,850) ;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRESEARCH_RESULT_DESCTYPE::SetQueryWhere(CString strResearchKey, CString strQuestionInfoKey)
{

	m_strResearchKey    = strResearchKey ;
	m_strQuestionInfoKey = strQuestionInfoKey ;


}

HBRUSH CRESEARCH_RESULT_DESCTYPE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
