// RETURN_CODE_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "RETURN_CODE_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRETURN_CODE_DLG dialog


CRETURN_CODE_DLG::CRETURN_CODE_DLG(CESL_Mgr* pParent )
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CRETURN_CODE_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//JOB.2019.0073 : 面倒 License 力芭
//	m_pReturnCodeMgr = NULL ;
}
CRETURN_CODE_DLG::~CRETURN_CODE_DLG()
{
}

BOOL CRETURN_CODE_DLG::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CRETURN_CODE_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRETURN_CODE_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRETURN_CODE_DLG, CDialog)
	//{{AFX_MSG_MAP(CRETURN_CODE_DLG)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRETURN_CODE_DLG message handlers

BOOL CRETURN_CODE_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	if(InitESL_Mgr(_T("SM_CHECK_RETURN_CODE")) <0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Failed")) ;
		return FALSE ;
	}

	DisplayCode() ;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//JOB.2019.0073 : 面倒 License 力芭
// VOID CRETURN_CODE_DLG::SetReturnCodeMgr(CReturnCodeMgr *pCodeMgr)
// {
// 	m_pReturnCodeMgr = pCodeMgr ;
// }

VOID CRETURN_CODE_DLG::DisplayCode()
{
	
	CESL_ControlMgr *pCM = FindCM(_T("CM_CHECK_RETURN_CODE")) ;
	if(pCM == NULL) 
	{
		AfxMessageBox (_T("FindCM() Failed")) ;
		return ;
	}

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("CodeGrid")) ;
	if(pGrid == NULL)
	{
		AfxMessageBox (_T("FindControl() Failed")) ;
		return ;
	}

	pGrid->Display (-1) ;

	for(INT i = 0 ; i < pGrid->GetRows ()-1 ;i++)
	{
		pGrid->SetRowHeight (i+1,850) ;
	}

	return  ;
}

HBRUSH CRETURN_CODE_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
