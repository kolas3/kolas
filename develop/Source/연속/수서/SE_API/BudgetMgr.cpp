// BudgetMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetMgr.h"
#include "BudgetListDlg.h"
#include "BudgetApi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetMgr dialog


CBudgetMgr::CBudgetMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pGrid = NULL;
	m_pDlg = NULL;
	m_nCurrentIdx = -1;


}

CBudgetMgr::~CBudgetMgr()
{
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
	
}

VOID CBudgetMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BOOL CBudgetMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CBudgetMgr, CDialog)
	//{{AFX_MSG_MAP(CBudgetMgr)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetMgr message handlers

BOOL CBudgetMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	

	if(InitESL_Mgr( _T("예산관리") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 예산관리") );
		return FALSE;
	}

	CESL_ControlMgr* pCM = FindCM(_T("CM_예산관리"));
	m_pGrid = (CESL_Grid*)pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_예산관리"));
	
	if(!m_pDM || !m_pGrid) {
		AfxMessageBox( _T("DM,Grid Error : DM_예산관리,그리드") );
		return FALSE;
	}

	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CBudgetSearch(this);
		m_pDlg->pMain = this->pMain;
		m_pDlg->pParentMgr = (CESL_Mgr*)this;
		m_pDlg->m_pInfo = this->m_pInfo;
		m_pDlg->Create(this);
		m_pDlg->ShowWindow(FALSE);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBudgetMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridBUDGET_MGR);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);	
}


VOID CBudgetMgr::InsertProcedure()
{
	CBudgetListDlg dlg(this);
	dlg.m_nOpenMode = 1;
	dlg.m_pParentDM = m_pDM;
	dlg.m_pParentGrid = m_pGrid;
	dlg.DoModal();
	
}

VOID CBudgetMgr::ModifyProcedure()
{
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선정 된 정보가 없습니다!"));
		return;		
	}
	else
	{

		CBudgetListDlg dlg(this);
		dlg.m_nOpenMode = -1;


		m_pGrid->SelectMakeList();
		INT cnt = m_pGrid->SelectGetCount();

		if(cnt>0) dlg.m_nMoveOption = 2;
		else dlg.m_nMoveOption = 1;

		dlg.m_nCurrentIdx = m_nCurrentIdx;
		dlg.m_pParentDM = m_pDM;
		dlg.m_pParentGrid = m_pGrid;

		dlg.DoModal();
	}
}


VOID CBudgetMgr::DeleteProcedure()
{
	m_pGrid->SelectMakeList();

	INT idx = m_pGrid->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("선정된 항목이 없습니다!"));
	    return;
	}
	
	CString msg;
	INT nCount = m_pGrid->SelectGetCount();
	msg.Format(_T("[ %d ] 건의 예산을 삭제 하시겠습니까?"),nCount);


	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return; 
	
	BeginWaitCursor();

	CBudgetApi api(this);
	CString strBUDGET_COMPILATION_KEY;


	CString strTITLE;
	INT ids = 0;
	CArray<INT,INT> arrayDeletedRow;

	while(idx>=0)
	{
		strBUDGET_COMPILATION_KEY = m_pDM->GetCellData(_T("REC_KEY"),idx);
		

		ids = api.DeleteBudgetList(m_pDM,
								   strBUDGET_COMPILATION_KEY);
							 
	
		if(ids<0)
		{
			EndWaitCursor();
			strTITLE = m_pDM->GetCellData(_T("예산편성명"),idx);

			msg.Format(_T("[ %s ] 예산을 삭제하던 중 문제가 발생해 작업을 중단합니다!"),strTITLE);
			AfxMessageBox(msg);
			break;
		}
			
		arrayDeletedRow.Add(idx);

		idx = m_pGrid->SelectGetNext();
	}
	
	
	INT nSize = arrayDeletedRow.GetSize();
	INT nDeletedRow;

	for(INT i=0;i<nSize;i++)
	{
		nDeletedRow = arrayDeletedRow.GetAt(nSize-i-1);
		m_pDM->DeleteRow(nDeletedRow);
	}
	
	m_pGrid->Display();
	EndWaitCursor();

	msg.Format(_T("[ %d ] 건의 예산을 삭제하였습니다!"),nSize);
	AfxMessageBox(msg);
	return; 	

}


VOID CBudgetMgr::SearchProcedure()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
	}
}

VOID CBudgetMgr::OnSendQuery(WPARAM w,LPARAM l)
{

	m_pDM->RefreshDataManager(m_pDlg->m_strQuery);

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("예산편성구분"),_T("UDF_예산편성구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("예산편성상태"),_T("UDF_예산편성상태"));


	m_pGrid->Display();
	m_nCurrentIdx = -1;
	
	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();

	if(0 == nRowCnt)
	{
		CString str;
		// KOL.UDF.022 시큐어코딩 보완
		str.Format(_T("검색 결과가 없습니다."));
		//str.Format(_T("검색 결과가 없습니다."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
		AfxMessageBox(str);
	}
	else
	{
		CString str;
		// KOL.UDF.022 시큐어코딩 보완
		str.Format(_T(" %d 건의 자료가 검색되었습니다."));
		//str.Format(_T(" %d 건의 자료가 검색되었습니다."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
	}


}

BEGIN_EVENTSINK_MAP(CBudgetMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBudgetMgr)
	ON_EVENT(CBudgetMgr, IDC_gridBUDGET_MGR, -600 /* Click */, OnClickgridBUDGETMGR, VTS_NONE)
	ON_EVENT(CBudgetMgr, IDC_gridBUDGET_MGR, -601 /* DblClick */, OnDblClickgridBUDGETMGR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBudgetMgr::OnClickgridBUDGETMGR() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pDM->GetRowCount()) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_nCurrentIdx = row-1;
	}
}

VOID CBudgetMgr::OnDblClickgridBUDGETMGR() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pGrid->GetMouseRow()) return;
	ModifyProcedure();
}

HBRUSH CBudgetMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}