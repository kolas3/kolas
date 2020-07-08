// RESEARCH_DLG_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "RESEARCH_DLG_MAIN.h"

#include "RESEARCH_NEWMAKE_DLG.h"
#include "RESEARCH_RESULT_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRESEARCH_DLG_MAIN dialog


CRESEARCH_DLG_MAIN::CRESEARCH_DLG_MAIN(CESL_Mgr* pParent )
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CRESEARCH_DLG_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMainDM = NULL ;

}
BOOL CRESEARCH_DLG_MAIN::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CRESEARCH_DLG_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRESEARCH_DLG_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRESEARCH_DLG_MAIN, CDialog)
	//{{AFX_MSG_MAP(CRESEARCH_DLG_MAIN)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRESEARCH_DLG_MAIN message handlers

BOOL CRESEARCH_DLG_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("SM_RESEARCH_MAIN"))<0)
	{	
		AfxMessageBox (_T("InitESL_Mgr() Failed"));
		return FALSE ;

	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRESEARCH_DLG_MAIN::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem (IDC_RESEARCH_MAIN_GRID) ;
	if(pGrid != NULL)
	{
		//pGrid->MoveWindow (5,5,cx-10,cy-10) ;
		pGrid->MoveWindow(0, 0, cx, cy);
	}
}

VOID CRESEARCH_DLG_MAIN::ShowResearchMakeDlg()
{

	CRESEARCH_NEWMAKE_DLG dlg(this) ;
	if(dlg.DoModal () == IDOK)
	{
		Search () ;
	}
	

}

VOID CRESEARCH_DLG_MAIN::Search()
{

	m_pMainDM = FindDM(_T("DM_RESEARCH_MAIN")) ;
	if(m_pMainDM == NULL)
	{
		AfxMessageBox(_T("FindDM(DM_RESEARCH_MAIN) Failed"));
		return ;
	}

	CString strOption ;
	strOption = _T("") ;
	m_pMainDM->RefreshDataManager (strOption) ;

	INT nCnt = m_pMainDM->GetRowCount();	// 050302 수정
	if ( nCnt<=0 )
	{
		AfxMessageBox(_T("검색결과가 없습니다."));
	}

	AllControlDisplay (_T("CM_RESEARCH_MAIN"));


}

VOID CRESEARCH_DLG_MAIN::Delete()
{

	if(AfxMessageBox (_T("선택항목을 삭제 하시겠습니까.?"),MB_YESNO)!=IDYES)
	{
		return ;
	}
	


	CESL_ControlMgr *pCM = FindCM(_T("CM_RESEARCH_MAIN"));
	if(pCM == NULL)
	{
		AfxMessageBox (_T("FindCM() Failed"));
		return ;
	}

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid"));
	if(pGrid == NULL)
	{
		AfxMessageBox (_T("FindControl() Failed"));
		return ;
	}

	pGrid->SelectMakeList () ;

	INT nSelectCnt =  pGrid->SelectGetCount () ;
	
	pGrid->SelectGetTailPosition () ;

	INT     ids ;
	INT		nIndex ;
	CString strResearchKey ;


	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_pMainDM->CONNECTION_INFO ) ;
	dm.StartFrame () ;

	for(INT i = 0 ;i <nSelectCnt ;i++)
	{
		nIndex = pGrid->SelectGetIdx () ;
		m_pMainDM->GetCellData (_T("REC_KEY"),nIndex,strResearchKey) ;

		dm.InitDBFieldData () ;	
		
		//RESEARCH_RESULT_TBL
		ids = dm.MakeDeleteFrame (_T("RESEARCH_RESULT_TBL"),_T("RESEARCH_KEY"),_T("NUMERIC"),strResearchKey ) ;
		if(ids <0)
		{
			dm.EndFrame () ;
			return ;
		}
		
		//QUESTION_TBL
		ids = dm.MakeDeleteFrame (_T("QUESTION_TBL"),_T("RESEARCH_KEY"),_T("NUMERIC"),strResearchKey ) ;
		if(ids <0)
		{
			dm.EndFrame () ;
			return ;
		}
		//RESEARCH_QUESTION_INFO_TBL
		ids = dm.MakeDeleteFrame (_T("RESEARCH_QUESTION_INFO_TBL"),_T("RESEARCH_KEY"),_T("NUMERIC"),strResearchKey ) ;
		if(ids <0)
		{
			dm.EndFrame () ;
			return ;
		}
		//RESEARCH_INFO_TBL
		ids = dm.MakeDeleteFrame (_T("RESEARCH_INFO_TBL"),_T("REC_KEY"),_T("NUMERIC"),strResearchKey ) ;
		if(ids <0)
		{
			dm.EndFrame () ;
			return ;
		}


		pGrid->SelectGetPrev () ;
	}

	dm.SendFrame () ;
	dm.EndFrame () ;


	Search() ;
	//AllControlDisplay (_T("CM_RESEARCH_MAIN"),-1) ;


}

VOID CRESEARCH_DLG_MAIN::ModifyResearch()
{
	
	CRESEARCH_NEWMAKE_DLG dlg(this) ;
	dlg.SetScreenMode (_T("DM_RESEARCH_MAIN"),_T("CM_RESEARCH_MAIN"),_T("MainGrid"),FALSE) ;
	if(dlg.DoModal () == IDOK)
	{
		Search () ;
	}

}

VOID CRESEARCH_DLG_MAIN::ShowResearchResult()
{
	CRESEARCH_RESULT_DLG dlg(this);
	dlg.SetParentManager (_T("DM_RESEARCH_MAIN"),_T("CM_RESEARCH_MAIN"),_T("MainGrid")) ;
	dlg.DoModal () ;
}
