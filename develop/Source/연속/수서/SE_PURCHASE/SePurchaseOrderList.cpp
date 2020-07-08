// SePurchaseOrderList.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseOrderList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseOrderList dialog


CSePurchaseOrderList::CSePurchaseOrderList(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchaseOrderList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pCM = NULL;
	m_pDM_SEQ_NO = NULL;

}

CSePurchaseOrderList::~CSePurchaseOrderList()
{
}

VOID CSePurchaseOrderList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchaseOrderList)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchaseOrderList, CDialog)
	//{{AFX_MSG_MAP(CSePurchaseOrderList)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseOrderList message handlers

BOOL CSePurchaseOrderList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(InitESL_Mgr( _T("연속_수서_구입자료관리_주문목록") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_구입자료관리_주문목록") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_연속_구입자료관리_주문목록"));
	m_pDM = FindDM(_T("DM_연속_구입자료관리_주문목록"));
	m_pDM_SEQ_NO = FindDM(_T("DM_연속_구입자료관리_주문목록_차수"));
	
	if(!m_pDM || !m_pCM || !m_pDM_SEQ_NO ) {
		AfxMessageBox( _T("DM,CM Error : 연속_수서_구입자료관리_주문목록") );
		return false;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSePurchaseOrderList::OnOK() 
{
	// TODO: Add extra validation here
	
	CString ORDER_YEAR;
	CString SEQ_NO1;
	CString SEQ_NO2;
	INT nSeqNo1 = 0;
	
	m_pCM->GetControlMgrData(_T("차수년도"),ORDER_YEAR);
	m_pCM->GetControlMgrData(_T("차수번호1"),SEQ_NO1);
	m_pCM->GetControlMgrData(_T("차수번호2"),SEQ_NO2);
	
	if(ORDER_YEAR.IsEmpty())
	{
		AfxMessageBox(_T("차수년도가 입력되지 않았습니다!"));
		return;
	}
	
	if(SEQ_NO1.IsEmpty())
	{
		AfxMessageBox(_T("차수번호의 앞부분이 입력되지 않았습니다!"));
		return;
	}

	if(SEQ_NO1 == SEQ_NO2)
	{
		SEQ_NO2.Empty(); //단일 검색으로 전환
	}
	

	nSeqNo1 = _ttoi(SEQ_NO1.GetBuffer(0));
	if(0 == nSeqNo1) 
	{
		AfxMessageBox(_T("차수번호 앞부분은 0 이 될 수 없습니다!"));
		return;
	}

	if(!SEQ_NO2.IsEmpty())
	{
		INT nSeqNo2 = 0;
		nSeqNo2 = _ttoi(SEQ_NO2.GetBuffer(0));
		
		if(nSeqNo1>nSeqNo2) {
			AfxMessageBox(_T("차수번호 앞부분이 뒷부분보다 클수 없습니다!"));
			return;
		}
	}

	//주문 처리중이 아닌 차수가 있는 지 검증한다. 
	if(CheckPurchaseNo(ORDER_YEAR,SEQ_NO1,SEQ_NO2)<0) return;

	
	CString strQuery;
	CString strtmpquery;
	
	strtmpquery.Format(_T("ORDER_YEAR = '%s'"),ORDER_YEAR);
	strQuery += strtmpquery;

	strtmpquery.Empty();
	strtmpquery = MakeIntRangeQuery(_T("ORDER_SEQ_NO"),SEQ_NO1,SEQ_NO2);
	strQuery += _T(" AND ");
	strQuery += strtmpquery;

	m_pDM->RefreshDataManager(strQuery);
	
	INT nRowCnt = m_pDM->GetRowCount();
	
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("해당 차수 내에 자료가 없습니다!"));
		return;
	}
	
	PrintOrderList();

	CDialog::OnOK();
}

CString CSePurchaseOrderList::MakeIntRangeQuery(CString strFieldName, CString strData1, CString strData2)
{

	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s = %s"),strFieldName,strData1);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN %s AND %s"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

INT	CSePurchaseOrderList::CheckPurchaseNo(CString strYear,CString strNo1, CString strNo2) 
{
	
	//차수 범위의 차수 작업 상태를 검사해 주문이 아닌 자료가 있는지를 검증한다
	m_pDM_SEQ_NO->FreeData();

	CString strQuery;
	CString strtmpquery;
	
	strtmpquery.Format(_T("ACQ_YEAR = '%s'"),strYear);
	strQuery += strtmpquery;

	strtmpquery.Empty();
	strtmpquery = MakeIntRangeQuery(_T("SEQ_NO"),strNo1,strNo2);
	strQuery += _T(" AND ");
	strQuery += strtmpquery;

	m_pDM_SEQ_NO->RefreshDataManager(strQuery);
	
	INT nRowCnt;
	nRowCnt = m_pDM_SEQ_NO->GetRowCount();

	if(0 == nRowCnt) {
		AfxMessageBox(_T("검색 조건에 해당하는 차수가 없습니다!"));
		return -1;
	}
	
	CString strWORKING_STATUS;

	for(INT i=0;i<nRowCnt;i++)
	{
		strWORKING_STATUS = _T("");
		strWORKING_STATUS = m_pDM_SEQ_NO->GetCellData(_T("작업상태"),i);
		if(_T("1") != strWORKING_STATUS)
		{
			CString strSEQ_NO;
			CString msg;
			strSEQ_NO = m_pDM_SEQ_NO->GetCellData(_T("차수번호"),i);
			msg.Format(_T("[ %s ] 차수는 주문중이 아닙니다! \n 확인하여 주십시요."),strSEQ_NO);
			AfxMessageBox(msg);
			return -2;
		}
	}

	return 0;
}

VOID CSePurchaseOrderList::PrintOrderList()
{
	//구입처 설명 
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("구입처관리"),_T("UDF_구입처"));
	
	//본서명,발행자를 마크에서 가지고 온다
	CMarc Marc;
	CString strStreamMarc = _T("");
	CString strTitle,strPublisher;
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	
	INT nRowCnt = m_pDM->GetRowCount();

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
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속주문자료목록"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return ;
	}
	
	//=====================================================
	//2009.01.29 REM BY PJW : 엑셀을 지원함으로 필요 없어진 코드임
// 	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	//=====================================================	
	pSIReportManager->SetDataMgr(0, m_pDM, 0);
	
	/*
	pSIReportManager->SetTempVariable(_T("형식"),m_strCondition);
	*/

	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}



}
HBRUSH CSePurchaseOrderList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
