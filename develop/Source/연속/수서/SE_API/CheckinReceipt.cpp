// CheckinReceipt.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinReceipt.h"
#include "CheckinNewReceipt.h"
#include "CheckinReceiptBookInfo.h"
#include "SeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinReceipt dialog


CCheckinReceipt::CCheckinReceipt(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCheckinReceipt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinReceipt)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nACQ_TYPE = 1; //default
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_strACQ_YEAR = _T("");
	m_strRECEIPT_NO = _T("");
	m_strACQ_TYPE = _T("");
	m_nCurrentIdx = -1;
}
CCheckinReceipt::~CCheckinReceipt()
{
}
VOID CCheckinReceipt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinReceipt)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinReceipt, CDialog)
	//{{AFX_MSG_MAP(CCheckinReceipt)
	ON_BN_CLICKED(IDC_btnRECEIPT_INSERT, OnbtnRECEIPTINSERT)
	ON_BN_CLICKED(IDC_btnRECEIPT_SEARCH, OnbtnRECEIPTSEARCH)
	ON_BN_CLICKED(IDC_btnRECEIPT_DELETE, OnbtnRECEIPTDELETE)
	ON_BN_CLICKED(IDC_btnRECEIPT_BOOK_INFO, OnbtnRECEIPTBOOKINFO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinReceipt message handlers

BOOL CCheckinReceipt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_체크인_접수번호"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_체크인_접수번호") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_체크인_접수번호"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_체크인_접수번호"));
	

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : CM_체크인_접수번호, DM_체크인_접수번호") );
		return false;
	}

	CString strACQ_TYPE;
	if(1 == m_nACQ_TYPE) m_strACQ_TYPE = _T("구입");
	else m_strACQ_TYPE = _T("기증");
	m_pCM->SetControlMgrData(_T("수입구분"),m_strACQ_TYPE);   
	m_pCM->SetControlMgrData(_T("수입년도"),m_strACQ_YEAR);  
	
	RefreshAll();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CCheckinReceipt::RefreshAll()
{
	
	if(m_strACQ_YEAR.IsEmpty())
	{
		AfxMessageBox(_T("수입년도가 입력되지 않았습니다!"));
	}

	if(!m_strACQ_YEAR.IsEmpty())
	{
		CString strQuery;
		strQuery.Format(_T("ACQ_CODE = '%d' AND ACQ_YEAR = '%s'"),m_nACQ_TYPE,m_strACQ_YEAR);
		m_pDM->RefreshDataManager(strQuery);
		m_pGrid->Display();
	}
}


VOID CCheckinReceipt::OnbtnRECEIPTINSERT() 
{
	// TODO: Add your control notification handler code here
	// 접수번호추가 
	m_pCM->GetControlMgrData(_T("수입년도"),m_strACQ_YEAR);  
	
	if(m_strACQ_YEAR.IsEmpty())
	{
		AfxMessageBox(_T("수입년도가 입력되지 않았습니다!"));
		return;
	}
	
	CSeApi api(this);
	CString strRECEIPT_NO;
	strRECEIPT_NO = api.GetLastReceiptNo(m_pDM,m_nACQ_TYPE,m_strACQ_YEAR);

	CString strSTATIC;
	strSTATIC.Format(_T("수입구분:%s  수입년도:%s"),m_strACQ_TYPE,m_strACQ_YEAR);

	CCheckinNewReceipt dlg;
	dlg.m_strNEW_RECEIPT_NO = strRECEIPT_NO;
	dlg.m_strSTATIC_NEW_RECEIPT = strSTATIC;
	dlg.m_strACQ_YEAR = m_strACQ_YEAR;
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_pParentDM = m_pDM;
	UpdateData(FALSE);
	
	if(dlg.DoModal() != IDOK) return;
	

	//추가
	m_pDM->InsertRow(-1);
	INT nRowIdx = m_pDM->GetRowCount()-1;
	CString strACQ_TYPE;
	CString strREC_KEY;
	m_pDM->MakeRecKey(strREC_KEY);
	strACQ_TYPE.Format(_T("%d"),m_nACQ_TYPE);

	
	m_pDM->SetCellData(_T("REC_KEY"),strREC_KEY,nRowIdx);
	m_pDM->SetCellData(_T("수입년도"),m_strACQ_YEAR,nRowIdx);
	m_pDM->SetCellData(_T("접수번호"),dlg.m_strNEW_RECEIPT_NO,nRowIdx);
	m_pDM->SetCellData(_T("최종일련번호"),_T("0"),nRowIdx);
	m_pDM->SetCellData(_T("생성일"),GetTodayDate(),nRowIdx);
	m_pDM->SetCellData(_T("수입구분"),strACQ_TYPE,nRowIdx);
	m_pDM->SetCellData(_T("작업자"),m_pInfo->USER_ID,nRowIdx);
	m_pDM->SetCellData(_T("최초작업"),GetWorkLogMsg(WORK_LOG_SERECEIPT),nRowIdx);
	m_pDM->SetCellData(_T("마지막작업"),GetWorkLogMsg(WORK_LOG_SERECEIPT),nRowIdx);
	m_pDM->SetCellData(_T("UDF_책수"),_T("0"),nRowIdx);
	

	CArray<CString,CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_책수"));
	

	m_pDM->StartFrame();

	// 2005-04-11 By SM5ong
	// 마지막 차수번호+1로 Update
	// BEGIN:
	CString strKindCode, strQuery;
  	if(1 == m_nACQ_TYPE)
		strKindCode = _T("CK1_NO"); //구입 접수번호 
	else if(2 == m_nACQ_TYPE)
		strKindCode = _T("CK2_NO"); //기증 접수번호 
  

	//=====================================================
	//2009.01.29 UPDATE BY PJW : CO_LAST_NUMBER_TBL은 하나씩만 존재해야 함으로 없을때만 INSERT하면 됨

// 	// 2008.06.13 REM BY PDJ : Insert에서 업데이트 할 필요 없음
// 	// 비교 문장 잘못되었음.
// 	//if( _T("1") == strRECEIPT_NO )	//검색결과가 없으면 INSERT 
// 	//{
// 		strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR,LAST_NUMBER,CREATE_DATE,CREATE_WORKER,FIRST_WORK, MANAGE_CODE)")
// 						_T("VALUES	(%s.NEXTVAL,'%s','SE','%s',1,'%s','%s','%s', UDF_MANAGE_CODE);"), 
// 						DEFAULT_SEQ_NAME, strKindCode, m_strACQ_YEAR,GetTodayDate(),api.m_pMgr->m_pInfo->USER_ID,api.m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI));
// 		m_pDM->AddFrame(strQuery);
// 	//}
// 	//else	//있으면 UPDATE
// 	//{
// //		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = '%s';"),
// //			            strRECEIPT_NO,api.m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),m_strACQ_YEAR,strKindCode);
// //		m_pDM->AddFrame(strQuery);
// //	}

	if( _T("1") == strRECEIPT_NO )	//검색결과가 없으면 INSERT 
	{
		strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR,LAST_NUMBER,CREATE_DATE,CREATE_WORKER,FIRST_WORK, MANAGE_CODE)")
						_T("VALUES	(%s.NEXTVAL,'%s','SE','%s',1,'%s','%s','%s', UDF_MANAGE_CODE);"), 
						DEFAULT_SEQ_NAME, strKindCode, m_strACQ_YEAR,GetTodayDate(),api.m_pMgr->m_pInfo->USER_ID,api.m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI));
		m_pDM->AddFrame(strQuery);
	}
	else	//있으면 UPDATE
	{
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;"),
		// KOL.UDF.022 시큐어코딩 보완
			            strRECEIPT_NO, api.m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI), m_strACQ_YEAR, strKindCode);
		//	            strRECEIPT_NO,api.m_pMgr->GetWorkLogMsg(WORK_LOG_SEAPI),m_strACQ_YEAR,strKindCode,api.m_pMgr->m_pInfo->USER_ID);
		m_pDM->AddFrame(strQuery);
	}
	//=====================================================
	
	// END:

	m_pDM->InitDBFieldData();
	INT nColumn = m_pDM->GetRefColumnCount();
	m_pDM->MakeQueryByJoinedDM(RemoveAliasList, m_pDM, _T("SE_RECEIPT_TBL") , nRowIdx, m_pDM);
	
	INT ids = m_pDM->SendFrame();	
	m_pDM->EndFrame();

	if(ids<0)
	{
		AfxMessageBox(_T("입력에 실패 하였습니다!"));
		m_pDM->DeleteRow(nRowIdx);
		return;
	}
	else
	{
		AfxMessageBox(_T("입력하였습니다!"));
		RefreshAll();
		return;

	}

}

CString CCheckinReceipt::GetTodayDate()
{
	CString result;
	COleDateTime t = COleDateTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

VOID CCheckinReceipt::OnOK() 
{
	// TODO: Add extra validation here
	
	// 선택 
	if(m_nCurrentIdx<0) 
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}

	m_strRECEIPT_NO = m_pDM->GetCellData(_T("접수번호"),m_nCurrentIdx);	
	
	if(m_strRECEIPT_NO.IsEmpty())
	{
		AfxMessageBox(_T("접수번호 부여에 실패하였습니다!"));
		return;
	}
	
	m_strACQ_YEAR = m_pDM->GetCellData(_T("수입년도"),m_nCurrentIdx);	
	
	if(m_strACQ_YEAR.IsEmpty())
	{
		AfxMessageBox(_T("수입년도를 찾을 수 없습니다!"));
		return;
	}


	CDialog::OnOK();
}

VOID CCheckinReceipt::OnbtnRECEIPTSEARCH() 
{
	// TODO: Add your control notification handler code here
	m_pCM->GetControlMgrData(_T("수입년도"),m_strACQ_YEAR);  
	
	RefreshAll();
}

BEGIN_EVENTSINK_MAP(CCheckinReceipt, CDialog)
    //{{AFX_EVENTSINK_MAP(CCheckinReceipt)
	ON_EVENT(CCheckinReceipt, IDC_gridCHECKIN_RECEIPT, -600 /* Click */, OnClickgridCHECKINRECEIPT, VTS_NONE)
	ON_EVENT(CCheckinReceipt, IDC_gridCHECKIN_RECEIPT, -601 /* DblClick */, OnDblClickgridCHECKINRECEIPT, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CCheckinReceipt::OnClickgridCHECKINRECEIPT() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			  m_nCurrentIdx = row-1;
		}
	}
}

VOID CCheckinReceipt::OnbtnRECEIPTDELETE() 
{
	// TODO: Add your control notification handler code here
	if(m_nCurrentIdx<0) 
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}
	
	CString strBookCnt;
	strBookCnt = m_pDM->GetCellData(_T("UDF_책수"),m_nCurrentIdx);

	INT nBook;
	nBook = _ttoi(strBookCnt.GetBuffer(0));

	if(0 != nBook)
	{
		AfxMessageBox(_T("책이 있는 작업번호는 삭제할 수 없습니다!"));
		return;
	}


	CString msg;
	CString strNo;
	strNo = m_pDM->GetCellData(_T("접수번호"),m_nCurrentIdx);
	msg.Format(_T(" [ %s ] 접수번호를 삭제하시겠습니까?"),strNo);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;
	

	CString strREC_KEY;
	strREC_KEY = m_pDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
	

	if(!strREC_KEY.IsEmpty())
	{
		CString strQuery;
		strQuery.Format(_T("DELETE FROM SE_RECEIPT_TBL WHERE REC_KEY = %s;"),strREC_KEY);
		
		m_pDM->StartFrame();
		m_pDM->InitDBFieldData();
		m_pDM->AddFrame(strQuery);
		INT ids = m_pDM->SendFrame();	
		m_pDM->EndFrame();

		if(ids<0)
		{
			AfxMessageBox(_T("삭제에 실패 하였습니다!"));
			return;
		}
		else
		{
			AfxMessageBox(_T("삭제하였습니다!"));
			m_pDM->DeleteRow(m_nCurrentIdx);
			m_pGrid->Display();      
			return;
		}
	}
}

VOID CCheckinReceipt::OnbtnRECEIPTBOOKINFO() 
{
	// TODO: Add your control notification handler code here
	//책정보보기
	
	// 선택 
	if(m_nCurrentIdx<0) 
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}

	INT nRow = m_pDM->GetRowCount();
	if(nRow<1) 
	{
		AfxMessageBox(_T("검색된 항목이 없습니다!"));
		return;
	}

	CString strAcqYear = m_pDM->GetCellData(_T("수입년도"),m_nCurrentIdx);	
	CString strReceiptNo = m_pDM->GetCellData(_T("접수번호"),m_nCurrentIdx);	

	if(strAcqYear.IsEmpty())
	{
		AfxMessageBox(_T("수입년도를 찾을 수 없습니다!"));
		return;
	}
	
	if(strReceiptNo.IsEmpty())
	{
		AfxMessageBox(_T("접수번호를 찾을 수 없습니다!"));
		return;
	}	
	
	CCheckinReceiptBookInfo dlg(this);
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_strAcqYear = 	strAcqYear;
	dlg.m_strReceiptNo = strReceiptNo;
	dlg.DoModal();
}

VOID CCheckinReceipt::OnDblClickgridCHECKINRECEIPT() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			  m_nCurrentIdx = row-1;
			  OnOK();
		}
	}
	
}

HBRUSH CCheckinReceipt::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
