// PURCHASE.cpp : implementation file
//

#include "stdafx.h"
#include "PURCHASE.h"
#include "..\SE_API\SeApi.h"
#include "SePurchaseOrder.h"
#include "..\SE_API\SeFileImport.h"
#include "..\SE_API\SePurchaseNoDlg.h"
#include "SePurchaseOrderList.h"
#include "SePurchaseMail.h"
#include "SePurchase.h"
#include "Purchase_PriceChange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPURCHASE dialog


CPURCHASE::CPURCHASE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CPURCHASE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPURCHASE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pCM = NULL;
	m_nCurrentIdx = -1;

	m_bIsLightVersion = FALSE;
	m_bIsDepartMngUse = FALSE;

	m_strSEQ_NO = _T("");
	m_strSEQ_YEAR = _T("");
	AfxInitRichEdit();
}

CPURCHASE::~CPURCHASE()
{
	//search dialog delete
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
}

VOID CPURCHASE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPURCHASE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPURCHASE, CDialog)
	//{{AFX_MSG_MAP(CPURCHASE)
	ON_WM_SIZE()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPURCHASE message handlers
BOOL CPURCHASE::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CPURCHASE::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridPURCHASE);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);
	
}

BOOL CPURCHASE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		
	if(InitESL_Mgr( _T("연속_수서_구입자료관리") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_구입자료관리") );
		return false;
	}

	CESL_ControlMgr* pCM = FindCM(_T("CM_연속_구입자료관리"));
	m_pGrid = (CESL_Grid*)pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_연속_구입자료관리"));
	
	if(!m_pDM) {
		AfxMessageBox( _T("DM Error : DM_연속_구입자료관리") );
		return false;
	}
	
	//라이트 버젼여부를 가져온다
	CSeApi api(this);
	m_bIsLightVersion = api.IsLightVersion(m_pDM);
	

	//부서지정 사용 여부를 가져온다
	m_bIsDepartMngUse = api.IsDepartMngUse(m_pDM);


	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CSePurchaseSearch(this);
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


VOID CPURCHASE::DoSearch()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
		//=====================================================
		//2009.02.09 ADD  BY PJW : 재검색시 그리드를 다시 뿌려준다.
		m_pGrid->Display();
		//=====================================================
		
	}
}


VOID CPURCHASE::OnSendQuery(WPARAM w,LPARAM l)
{
	
	CString strQuery;
	strQuery = m_pDlg->m_strQuery;
	m_strSEQ_NO = m_pDlg->m_strSEQ_NO;
	m_strSEQ_YEAR = m_pDlg->m_strSEQ_YEAR;

	m_pDM->RefreshDataManager(strQuery);
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("구입처관리"),_T("UDF_구입처"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료구분"),_T("UDF_자료구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("관리구분"),_T("UDF_관리구분"));


	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();


	//차수작업상태 
	CString strWorking_status;
	CString strUDF;
	for(INT i=0;i<nRowCnt;i++)
	{
		strUDF = _T("");
		strWorking_status = m_pDM->GetCellData(_T("차수작업상태"),i);
	
		if(_T("0") == strWorking_status) strUDF = _T("구입대상");
		else if(_T("1") == strWorking_status) strUDF = _T("주문중");
		
		m_pDM->SetCellData(_T("UDF_차수작업상태"),strUDF,i);
	}


	m_pGrid->Display();
	m_nCurrentIdx = -1;


	if(0 == nRowCnt)
	{
		CString str;
		str.Format(_T("검색 결과가 없습니다."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
		AfxMessageBox(str);
	}
	else
	{
		CString str;
		str.Format(_T(" %d 건의 자료가 검색되었습니다."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
	}
	if( nRowCnt > 0 )
		m_pDlg->ShowWindow(SW_HIDE);
}


VOID CPURCHASE::InsertPurchaseSpecies()
{
	CSePurchase dlg(this);
	dlg.m_nOpenMode = 1;
	dlg.m_pParentDM = m_pDM;
	dlg.m_pParentGrid = m_pGrid;
	dlg.m_bIsLightVersion = m_bIsLightVersion;
	dlg.m_bIsDepartMngUse = m_bIsDepartMngUse;
	dlg.DoModal();
}


VOID CPURCHASE::ModifyPurchaseSpecies()
{
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선정 된 정보가 없습니다!"));
		return;
	}
	else
	{
		CSePurchase dlg(this);
		m_pGrid->SelectMakeList();
		INT cnt = m_pGrid->SelectGetCount();

		if(cnt>0) dlg.m_nMoveOption = 2;
		else dlg.m_nMoveOption = 1;
		
		dlg.m_nCurrentIdx = m_nCurrentIdx;
		dlg.m_nOpenMode = -1;
		dlg.m_pParentDM = m_pDM;
		dlg.m_pParentGrid = m_pGrid;
		dlg.m_bIsLightVersion = m_bIsLightVersion;
		dlg.m_bIsDepartMngUse = m_bIsDepartMngUse;
		dlg.DoModal();
	}


}

VOID CPURCHASE::DeletePurchaseSpecies()
{
	m_pGrid->SelectMakeList();

	INT idx = m_pGrid->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
	    return;
	}
	
	CString msg;
	INT nCount = m_pGrid->SelectGetCount();
	msg.Format(_T("[ %d ] 건의 자료를 삭제 하시겠습니까?"),nCount);

	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return; 
	
	CSeApi api(this);
	INT ids = 0;
	CArray<INT,INT> arrayDeletedRow;

	while(idx>=0)
	{
		
		ids = api.SPECIES_PUR_OBJ_DELETE(m_pDM,idx);

		if(ids<0)
		{
			
			msg.Format(_T("[ %d ]번째 자료를 삭제하던 중 문제가 발생해 작업을 중단합니다!"),idx+1);
			AfxMessageBox(msg);
			return; 	
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

	msg.Format(_T("[ %d ] 건의 자료를 삭제하였습니다!"),nCount);
	AfxMessageBox(msg);
	return; 	
}

VOID CPURCHASE::OrderProcess()
{
	//현재 검색이 되어 있고, 검색된 차수년도와 차수번호가 구입대상일 경우는 주문 클래스에 
	//기본 값으로 설정해 준다
	CSePurchaseOrder dlg(this);
	
	if(!m_strSEQ_NO.IsEmpty() && !m_strSEQ_YEAR.IsEmpty())
	{
		if(m_pDM->GetRowCount()>0)
		{

			CString strQuery;
			CString strResult;
			strQuery.Format(_T("SELECT WORKING_STATUS FROM SE_PURCHASE_SEQ_NO_TBL ")
				            _T("WHERE ACQ_YEAR = %s AND SEQ_NO = %s AND MANAGE_CODE=UDF_MANAGE_CODE"),m_strSEQ_YEAR,m_strSEQ_NO);
		
			m_pDM->GetOneValueQuery(strQuery,strResult);
			if(_T("0") == strResult)
			{
				dlg.m_strParentYear = m_strSEQ_YEAR;
				dlg.m_strParentNo = m_strSEQ_NO;
				
			}

		}


	}
	
	

	dlg.DoModal();
}

VOID CPURCHASE::OrderCancel()
{


}

VOID CPURCHASE::OrderMail()
{
	//JOB.2019.0073 : 충돌 License
// 	CSePurchaseMail dlg(this);
// 	dlg.DoModal();

}

VOID CPURCHASE::FileImport()
{

	CSeFileImport dlg(this);
	dlg.DoModal();

}

VOID CPURCHASE::OrderList()
{
	CSePurchaseOrderList dlg(this);
	dlg.DoModal();

}

VOID CPURCHASE::OrderSeqNo()
{

	CSePurchaseNoDlg dlg(this);
	dlg.m_nOpenMode = -1;
	dlg.DoModal();

}

VOID CPURCHASE::PrintGridData()
{
	INT nRowCnt = m_pDM->GetRowCount();

	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("출력할 자료가 없습니다!"));
		return;
	}

	//본서명,발행자를 마크에서 가지고 온다
	CMarc Marc;
	CString strStreamMarc = _T("");
	CString strTitle,strPublisher;
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));


	for(INT i=0;i<nRowCnt;i++)
	{
		strStreamMarc = m_pDM->GetCellData(_T("MARC"),i);
		
		m_pInfo->pMarcMgr->Decoding(strStreamMarc,&Marc);
		
		strTitle = _T("");
		strPublisher = _T("");
		
		m_pInfo->pMarcMgr->GetItem(&Marc,_T("245$a"),strTitle);
		m_pInfo->pMarcMgr->GetItem(&Marc,_T("260$b"),strPublisher);
		
		m_pDM->SetCellData(_T("UDF_본서명"),strTitle,i);
		m_pDM->SetCellData(_T("UDF_발행자"),strPublisher,i);
	}
	
	
	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속구입자료목록"));
	
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return ;
	}
	//=====================================================
	//2009.01.29 REM BY PJW : 엑셀을 지원함으로 필요 없어진 코드임
// 	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	//=====================================================
 	
	pSIReportManager->SetDataMgr(0, m_pDM, 0);
	
	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

}

BEGIN_EVENTSINK_MAP(CPURCHASE, CDialog)
    //{{AFX_EVENTSINK_MAP(CPURCHASE)
	ON_EVENT(CPURCHASE, IDC_gridPURCHASE, -600 /* Click */, OnClickgridPURCHASE, VTS_NONE)
	ON_EVENT(CPURCHASE, IDC_gridPURCHASE, -601 /* DblClick */, OnDblClickgridPURCHASE, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CPURCHASE::OnClickgridPURCHASE() 
{
	if(0 == m_pDM->GetRowCount()) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
			m_nCurrentIdx = row-1;
	}
}

VOID CPURCHASE::OnDblClickgridPURCHASE() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pGrid->GetMouseRow()) return;
	ModifyPurchaseSpecies();
}

VOID CPURCHASE::PriceChange()
{
	//구입가격 일괄변경.. 화면을 띄운다.
	CPurchase_PriceChange dlg;
	dlg.SetInfo (m_pDM, m_pGrid);
	dlg.DoModal();
}

HBRUSH CPURCHASE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
