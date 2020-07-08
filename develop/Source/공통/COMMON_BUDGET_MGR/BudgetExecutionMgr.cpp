// BudgetExecutionMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetExecutionMgr.h"
#include "BudgetExecutionInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetExecutionMgr dialog


CBudgetExecutionMgr::CBudgetExecutionMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetExecutionMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;

	m_nOpenMode = 1; //default
	m_strBUDGET_COMPILATION_KEY = _T("");
	m_pArrayDeleteKey = NULL;
	m_nCurrentIdx = -1;
}
CBudgetExecutionMgr::~CBudgetExecutionMgr()
{
}

VOID CBudgetExecutionMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetExecutionMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BOOL CBudgetExecutionMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


BEGIN_MESSAGE_MAP(CBudgetExecutionMgr, CDialog)
	//{{AFX_MSG_MAP(CBudgetExecutionMgr)
	ON_BN_CLICKED(IDC_btnBUDGET_EXECUTION_MGE_MODIFY, OnbtnBUDGETEXECUTIONMGEMODIFY)
	ON_BN_CLICKED(IDC_btnBUDGET_EXECUTION_MGE_DELETE, OnbtnBUDGETEXECUTIONMGEDELETE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetExecutionMgr message handlers

BOOL CBudgetExecutionMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("예산관리_예산편성_예산집행") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 예산관리_예산편성_예산집행") );
		return false;
	}
	
	
	m_pCM = FindCM(_T("CM_예산집행"));
	m_pDM = FindDM(_T("DM_예산집행"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));

	if(!m_pCM || !m_pDM || !m_pGrid) {
		AfxMessageBox( _T("CM,DM,Grid Error : CM_예산집행,DM_예산집행,그리드") );
		return FALSE;
	}

	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBudgetExecutionMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData1, strTmpData2;
	
	if(strDMFieldAlias == _T("UDF_집행예산"))
	{	
		strTmpData1 = m_pDM->GetCellData(_T("집행예산"),nRow);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData1, strTmpData2);
		m_pDM->SetCellData(_T("UDF_집행예산"), strTmpData2, nRow);	
	}
	return 0;
}

INT CBudgetExecutionMgr::RefreshAll()
{
	if(m_strBUDGET_COMPILATION_KEY.IsEmpty())
	{
		AfxMessageBox(_T("예산편성KEY를 찾을 수 없습니다!"));
		return -1;
	}

	CString strWhere;
	strWhere.Format(_T("BUDGET_COMPILATION_KEY = %s"),m_strBUDGET_COMPILATION_KEY);
	m_pDM->RefreshDataManager(strWhere);

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("예산집행유형"),_T("UDF_집행타입"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("예산집행상태"),_T("UDF_집행상태"));
	ShowAllData();
	
	return 0;

}

VOID CBudgetExecutionMgr::OnOK() 
{
	// TODO: Add extra validation here
	
	//insert
	CBudgetExecutionInfo dlg(this);
	dlg.m_pParentDM = m_pDM;
	dlg.m_pParentGrid = m_pGrid;
	dlg.m_nOpenMode = 1;

	if(dlg.DoModal() == IDOK) ShowAllData();

	return;
	CDialog::OnOK();
}

VOID CBudgetExecutionMgr::OnbtnBUDGETEXECUTIONMGEMODIFY() 
{
	// TODO: Add your control notification handler code here
	
	//modify
	CBudgetExecutionInfo dlg(this);
	dlg.m_nCurrentIdx = m_nCurrentIdx;
	dlg.m_pParentDM = m_pDM;
	dlg.m_pParentGrid = m_pGrid;
	dlg.m_nOpenMode = -1;
	
	if(dlg.DoModal() == IDOK) ShowAllData();
}


VOID CBudgetExecutionMgr::ShowAllData()
{
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("예산집행유형"),_T("UDF_집행타입"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("예산집행상태"),_T("UDF_집행상태"));
	
	m_pCM->AllControlDisplay();
	m_pGrid->Display();
}


VOID CBudgetExecutionMgr::OnbtnBUDGETEXECUTIONMGEDELETE() 
{
	// TODO: Add your control notification handler code here
	
	//delete 
	CString strType,strBudget;
	


	strType = m_pDM->GetCellData(_T("UDF_집행타입"),m_nCurrentIdx);
	strBudget = m_pDM->GetCellData(_T("집행예산"),m_nCurrentIdx);


	CString msg;
	msg.Format(_T("[%s]으로  집행한 [%s]의  예산집행내역을 삭제하겠습니까?"), strType,strBudget);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;


	CString strUDF;
	strUDF = m_pDM->GetCellData(_T("UDF_상태"),m_nCurrentIdx);

	if(_T("I") == strUDF)
	{
		
	}
	else 
	{
		CString strREC_KEY;
		strREC_KEY = m_pDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
		m_pArrayDeleteKey->Add(strREC_KEY); //update
		//2005-09-27 황정영: 사용내역 DB에서 삭제
		if(!strREC_KEY.IsEmpty())
		{
			m_pDM->StartFrame();
			
			CString strQuery;
			strQuery.Format(_T("DELETE FROM CO_BUDGET_EXECUTION_TBL WHERE REC_KEY = %s;"),strREC_KEY);
			m_pDM->AddFrame(strQuery);
			m_pDM->SendFrame();
			m_pDM->EndFrame();
		}
	}

	m_pDM->DeleteRow(m_nCurrentIdx);

	ShowAllData();
	
}

BEGIN_EVENTSINK_MAP(CBudgetExecutionMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBudgetExecutionMgr)
	ON_EVENT(CBudgetExecutionMgr, IDC_gridBUDGET_EXECUTION_MGR, -600 /* Click */, OnClickgridBUDGETEXECUTIONMGR, VTS_NONE)
	ON_EVENT(CBudgetExecutionMgr, IDC_gridBUDGET_EXECUTION_MGR, -601 /* DblClick */, OnDblClickgridBUDGETEXECUTIONMGR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBudgetExecutionMgr::OnClickgridBUDGETEXECUTIONMGR() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pDM->GetRowCount()) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_nCurrentIdx = row-1;
	}
}

VOID CBudgetExecutionMgr::OnDblClickgridBUDGETEXECUTIONMGR() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pGrid->GetMouseRow()) return;
	OnbtnBUDGETEXECUTIONMGEMODIFY();
}

VOID CBudgetExecutionMgr::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(!m_strBUDGET_COMPILATION_KEY.IsEmpty())
		SendMessage(IDS_REFRESHALL,0,0);
	CDialog::OnCancel();
}

HBRUSH CBudgetExecutionMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
