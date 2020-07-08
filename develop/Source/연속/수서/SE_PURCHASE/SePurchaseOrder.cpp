// SePurchaseOrder.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseOrder.h"
#include "..\SE_API\SeApi.h"
#include "SePurchaseMail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseOrder dialog


CSePurchaseOrder::CSePurchaseOrder(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSePurchaseOrder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchaseOrder)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pDM_PUR_SPECIES = NULL;
	m_pDM_SPECIES = NULL;
	m_pDM_IDX = NULL;
	m_pDM_PURCASE_MANAGE = NULL;
	m_pDM_SUB_HISTORY = NULL;
	m_pDM_SEQ_NO = NULL;

	//Thread
	m_pThreadOrder = NULL;
	m_bIsThreadRunning = FALSE;
	m_pProgressDlg = NULL;
	m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));

	m_nRowCnt = 0;
	
	//mail
	m_strOrderYear = _T("");
	m_strSEQ_NO1 = _T("");
	m_strSEQ_NO2 = _T("");

	//parent
	m_strParentYear = _T("");
	m_strParentNo = _T("");

}

CSePurchaseOrder::~CSePurchaseOrder()
{
	
	// Thread Check
	if (m_bIsThreadRunning)
	{
		m_bIsThreadRunning = FALSE;

		MSG msg;
		while (TRUE)
		{
			if (::PeekMessage(&msg,GetSafeHwnd(), 1, 1, PM_NOREMOVE))
				AfxGetApp()->PumpMessage();

			if (WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0)
			break;
		}
	}
	

	/*
	if(m_pThreadOrder != NULL)
	{
		DWORD dwRetCode;
		dwRetCode = ::WaitForSingleObject(m_pThreadOrder->m_hThread , 2000);
		if (dwRetCode == WAIT_TIMEOUT)
		{
			TerminateThread(m_pThreadOrder->m_hThread , 0);
		}
	}
	*/
	
	if(m_pProgressDlg) 
	{
		delete m_pProgressDlg;
		m_pProgressDlg = NULL;
	}
}

VOID CSePurchaseOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchaseOrder)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchaseOrder, CDialog)
	//{{AFX_MSG_MAP(CSePurchaseOrder)
	ON_MESSAGE(ORDER_MAIL_SEND, OnMailSend)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseOrder message handlers

BOOL CSePurchaseOrder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("연속_수서_구입자료관리_주문") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_구입자료관리_주문") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_연속_구입자료관리_주문"));
	

	m_pDM_PUR_SPECIES = FindDM(_T("DM_연속_주문_구입종"));
	m_pDM_SPECIES = FindDM(_T("DM_연속_주문_종"));
	m_pDM_IDX = FindDM(_T("DM_연속_주문_색인"));
	m_pDM_PURCASE_MANAGE = FindDM(_T("DM_연속_주문_구입정보"));
	m_pDM_SUB_HISTORY = FindDM(_T("DM_연속_주문_구독이력정보"));
	m_pDM_SEQ_NO = FindDM(_T("DM_연속_구입자료관리_주문_차수"));

	if(!m_pCM || !m_pDM_PUR_SPECIES || !m_pDM_SPECIES || !m_pDM_IDX || !m_pDM_PURCASE_MANAGE || !m_pDM_SUB_HISTORY || !m_pDM_SEQ_NO) {
		AfxMessageBox( _T("DM,CM Error : 연속_수서_구입자료관리_주문") );
		return false;
	}
	

	if(!m_strParentYear.IsEmpty() && !m_strParentNo.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("차수년도"),m_strParentYear);
		m_pCM->SetControlMgrData(_T("차수번호1"),m_strParentNo);

	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSePurchaseOrder::OnOK() 
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
	
	
	//주문 처리할 차수중에 이미 주문 처리된 차수가 있는 지 검증한다. 
	if(CheckPurchaseNo(ORDER_YEAR,SEQ_NO1,SEQ_NO2)<0) return;


	CString strQuery;
	CString strtmpquery;
	
	strtmpquery.Format(_T("ORDER_YEAR = '%s'"),ORDER_YEAR);
	strQuery += strtmpquery;

	strtmpquery.Empty();
	strtmpquery = MakeIntRangeQuery(_T("ORDER_SEQ_NO"),SEQ_NO1,SEQ_NO2);
	strQuery += _T(" AND ");
	strQuery += strtmpquery;

	m_pDM_PUR_SPECIES->RefreshDataManager(strQuery);
	
	INT nRowCnt = m_pDM_PUR_SPECIES->GetRowCount();
	
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("해당 차수 내에 자료가 없습니다!"));
		return;
	}

	CString msg;
	msg.Format(_T("[ %d ] 건의 자료가 해당 차수에 있습니다. \n 주문 처리 하시겠습니까?"), nRowCnt);

	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;
	
	
	m_nRowCnt = nRowCnt;
	
	m_strOrderYear = ORDER_YEAR;
	m_strSEQ_NO1 = SEQ_NO1;
	m_strSEQ_NO2 = SEQ_NO2;
	

	//프로그레시브바 생성
	if(NULL == m_pProgressDlg)
	{
		m_pProgressDlg = new CSePurchaseProgress(this);
		m_pProgressDlg->Create(this);
	}

	//주문 처리 Thread
	m_pThreadOrder = AfxBeginThread(ThreadFunc_Order, this);
	
	
	return;

	//CDialog::OnOK();
}

VOID CSePurchaseOrder::OnMailSend(WPARAM w,LPARAM l)
{
	//JOB.2019.0073
	/*
	//주문 메일
	CSePurchaseMail dlg(this);
	dlg.m_bAutoMode = TRUE;
	dlg.m_strOrderSeqYear = m_strOrderYear;
	dlg.m_strSEQ_NO1 = m_strSEQ_NO1;
	dlg.m_strSEQ_NO2 = m_strSEQ_NO2;
	dlg.DoModal();
	
	m_strOrderYear = _T("");
	m_strSEQ_NO1 = _T("");
	m_strSEQ_NO2 = _T("");
	*/
	return;
}

/*
INT CSePurchaseOrder::SendAutoOderMail()
{

	return 0;
}
*/

INT	CSePurchaseOrder::CheckPurchaseNo(CString strYear,CString strNo1, CString strNo2) 
{
	
	//차수 범위의 차수 작업 상태를 검사해 이미 주문중인 자료가 있는지를 검증한다
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
		if(_T("1") == strWORKING_STATUS)
		{
			CString strSEQ_NO;
			CString msg;
			strSEQ_NO = m_pDM_SEQ_NO->GetCellData(_T("차수번호"),i);
			msg.Format(_T("[ %s ] 차수는 이미 주문중입니다! \n 확인하여 주십시요."),strSEQ_NO);
			AfxMessageBox(msg);
			return -2;
		}
	}

	return 0;
}

CString CSePurchaseOrder::MakeIntRangeQuery(CString strFieldName, CString strData1, CString strData2)
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

UINT ThreadFunc_Order(LPVOID pParam)
{
	CSePurchaseOrder *pDlg = (CSePurchaseOrder*)pParam;
	// KOL.UDF.022
	if (&pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}
	
	pDlg->m_bIsThreadRunning = TRUE;
	pDlg->m_pProgressDlg->m_ctrlProgress.SetRange(0,pDlg->m_nRowCnt);
	pDlg->m_pProgressDlg->m_ctrlProgress.SetPos(0);
	pDlg->m_pProgressDlg->ShowWindow(SW_SHOW);
	pDlg->m_pProgressDlg->BringWindowToTop();
	pDlg->m_pProgressDlg->CenterWindow();

	CSeApi api(pDlg);
	INT ids = 0;
	CMarc Marc;
	CString strStreamMarc;
	CString strTitle;
	CString msg;

	//연속설정
	pDlg->m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	for(INT i=0;i<pDlg->m_nRowCnt;i++)
	{
		
		Marc.Init();
		strStreamMarc = _T("");
		strStreamMarc = pDlg->m_pDM_PUR_SPECIES->GetCellData(_T("MARC"),i);
		pDlg->m_pInfo->pMarcMgr->Decoding(strStreamMarc,&Marc);
		
		pDlg->m_pInfo->pMarcMgr->GetItem(&Marc,_T("245$a"),strTitle);
		msg.Format(_T("[ %s ] \n 자료를 처리중입니다..."),strTitle);
		pDlg->m_pProgressDlg->GetDlgItem(IDC_stcPROGRESS)->SetWindowText(msg);

		ids = api.SPECIES_ORDER(pDlg->m_pDM_PUR_SPECIES,
								pDlg->m_pDM_SPECIES,
								pDlg->m_pDM_IDX,
								pDlg->m_pDM_PURCASE_MANAGE,
								pDlg->m_pDM_SUB_HISTORY,
								&Marc,
								i);

		
		if(ids<0) {

			msg.Format(_T("[ %s ] \n 자료를 처리중	문제가 발생하였습니다! \n 계속 주문처리를 진행하시겠습니까?"),strTitle);
			if(AfxMessageBox(msg,MB_YESNO) != IDYES)
			{
				pDlg->m_bIsThreadRunning = FALSE;
				return -1;
			}
		}

		pDlg->m_pProgressDlg->m_ctrlProgress.SetPos(i+1);

	}

	pDlg->m_pProgressDlg->ShowWindow(SW_HIDE);
	
	msg.Format(_T("[ %d ] 건의 주문처리를 완료했습니다!"), pDlg->m_nRowCnt);
	AfxMessageBox(msg);
		
	pDlg->m_bIsThreadRunning = FALSE;
	SetEvent(pDlg->m_hThread);
	
	//OrderMail Send
	::PostMessage(pDlg->GetSafeHwnd(),ORDER_MAIL_SEND,0,0);
	
	return 0;
}



HBRUSH CSePurchaseOrder::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
