// RETURN_LOGVIEW_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "RETURN_LOGVIEW_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRETURN_LOGVIEW_DLG dialog


CRETURN_LOGVIEW_DLG::CRETURN_LOGVIEW_DLG(CESL_Mgr* pParent )
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CRETURN_LOGVIEW_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// KOL.UDF.022 시큐어코딩 보완
	m_pLogDM = NULL;
	m_pLogGrid = NULL;
}
CRETURN_LOGVIEW_DLG::~CRETURN_LOGVIEW_DLG()
{
}

BOOL CRETURN_LOGVIEW_DLG::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CRETURN_LOGVIEW_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRETURN_LOGVIEW_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRETURN_LOGVIEW_DLG, CDialog)
	//{{AFX_MSG_MAP(CRETURN_LOGVIEW_DLG)
	ON_BN_CLICKED(IDC_bDelete, OnbDelete)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRETURN_LOGVIEW_DLG message handlers

BOOL CRETURN_LOGVIEW_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//SM_CHECK_RETURN_LOG

	//CM_CHECK_RETURN_LOG
	//DM_CHECK_RETURN_LOG

	INT		ids;
	ids		=	InitESL_Mgr(_T("SM_CHECK_RETURN_LOG"));
	if( 0 > ids )
	{
		AfxMessageBox( _T("InitESL_Mgr() Failed") );
		//return FALSE;
		EndDialog(1);
	}

	m_pLogDM = FindDM(_T("DM_CHECK_RETURN_LOG"));
	if( NULL == m_pLogDM )
	{
		AfxMessageBox( _T("FindDM() Failed") );
		EndDialog(2);
	}

	ids		=	m_pLogDM->RefreshDataManager(_T(""));
	if( 0 > ids )
	{
		AfxMessageBox( _T("RefreshDataManager() Failed") );
		EndDialog(3);
	}
	
	CESL_ControlMgr *pCM = FindCM(_T("CM_CHECK_RETURN_LOG"));
	if( NULL == pCM )
	{
		AfxMessageBox( _T("FindCM() Failed") );
		EndDialog(4);
	}

	m_pLogGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid")) ;
	m_pLogGrid->Display (-1) ;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRETURN_LOGVIEW_DLG::OnbDelete() 
{
	// TODO: Add your control notification handler code here
	
	m_pLogGrid->SelectMakeList () ;

	INT nRowCnt = m_pLogGrid->SelectGetCount () ;
	
	if(nRowCnt == 0) return ;

	m_pLogGrid->SelectGetHeadPosition () ;
	INT nRowIndex ;
	
	CString strRecKey ;
	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_pLogDM->CONNECTION_INFO ) ;
	
	dm.StartFrame () ;
	for(INT i = 0 ; i < nRowCnt ; i++)
	{
		nRowIndex = m_pLogGrid->SelectGetIdx () ;
        
		strRecKey = m_pLogDM->GetCellData (_T("REC_KEY"),nRowIndex ) ;
		
        dm.InitDBFieldData () ;
		//ids = dm.MakeDeleteFrame (_T("QUESTION_TBL"),_T("RESEARCH_KEY"),_T("NUMERIC"),strResearchKey ) ;
		dm.MakeDeleteFrame (_T("RETURN_MAIL_LOG"),_T("REC_KEY"),_T("NUMERIC"),strRecKey);
		m_pLogGrid->SelectGetNext () ;
	}

	dm.SendFrame () ;
	dm.EndFrame () ;


	CString strQuery ;
	strQuery = _T("") ;
	m_pLogDM ->RefreshDataManager  (strQuery);
	m_pLogGrid -> Display (-1) ;



	return ;
}

HBRUSH CRETURN_LOGVIEW_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
