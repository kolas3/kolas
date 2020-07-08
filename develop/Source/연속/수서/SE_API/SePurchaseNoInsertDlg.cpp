// SePurchaseNoInsertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseNoInsertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseNoInsertDlg dialog


CSePurchaseNoInsertDlg::CSePurchaseNoInsertDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSePurchaseNoInsertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchaseNoInsertDlg)
	m_bUSE_WORK_NO = FALSE;
	//}}AFX_DATA_INIT

	m_strYEAR = _T("");
	m_pDM = NULL;
	m_pDM_WORK = NULL;
	m_pCM = NULL;

}

CSePurchaseNoInsertDlg::~CSePurchaseNoInsertDlg()
{
}

VOID CSePurchaseNoInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchaseNoInsertDlg)
	DDX_Check(pDX, IDC_chkPUR_NO_INSERT_USE_WORKNO, m_bUSE_WORK_NO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchaseNoInsertDlg, CDialog)
	//{{AFX_MSG_MAP(CSePurchaseNoInsertDlg)
	ON_BN_CLICKED(IDC_chkPUR_NO_INSERT_USE_WORKNO, OnchkPURNOINSERTUSEWORKNO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseNoInsertDlg message handlers

BOOL CSePurchaseNoInsertDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_차수번호_입력"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_차수번호_입력") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_차수번호_입력"));
	m_pDM = FindDM(_T("DM_연속_차수번호_입력"));
	m_pDM_WORK = FindDM(_T("DM_연속_차수번호_입력_작업번호"));

	if(!m_pDM || !m_pCM || !m_pDM_WORK) {
		AfxMessageBox( _T("DM,CM Error : 연속_수서_차수번호_입력") );
		return FALSE;
	}	
	
	if(m_strYEAR.IsEmpty()) {
		AfxMessageBox( _T("차수년도가 입력되지 않았습니다!") );
		return FALSE;
	}	


	m_bUSE_WORK_NO = TRUE;
	
	CString strPurchaseNo;
	strPurchaseNo = GetPurchaseNo(m_pDM, m_strYEAR);
	m_pCM->SetControlMgrData(_T("차수년도"),m_strYEAR);
	m_pCM->SetControlMgrData(_T("차수번호"),strPurchaseNo);
	m_pCM->SetControlMgrData(_T("작업상태"),_T("구입대상"));
	m_pCM->SetControlMgrData(_T("생성자"),m_pInfo->USER_ID);
	m_pCM->SetControlMgrData(_T("마지막작업번호"),_T("0"));


	UpdateData(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CSePurchaseNoInsertDlg::GetPurchaseNo(CESL_DataMgr* pDM, CString strYear)
{
	pDM->StartFrame();
	
	//해당년
	CString strLastNumber,strQuery,strPurchaseNo;
	
	strQuery.Format(_T("LOCK TABLE CO_LAST_NUMBER_TBL IN EXCLUSIVE MODE;"));
	pDM->ExecuteQuery(strQuery,0,FALSE);
	

	//해당년도의 해당 차수번호를 SELECT하고 없을 경우는 INSERT한다 
	//++2008.10.17 UPDATE BY PWR {{++
	// 통합시스템 적용
	strQuery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					 _T("WHERE KIND_CODE='PUR_NO' AND PUBLISH_FORM_CODE='SE' AND MAKE_YEAR='%s' ")
					 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strYear );
	// strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'PUR_NO' AND PUBLISH_FORM_CODE = 'SE' AND MAKE_YEAR = '%s'"),strYear);
	//--2008.10.17 UPDATE BY PWR --}}
	INT ids =  pDM->GetOneValueQuery(strQuery,strLastNumber,FALSE);

	INT nPurchaseNo = 0;
	if(!strLastNumber.IsEmpty())
	{
		nPurchaseNo = _ttoi(strLastNumber.GetBuffer(0));
	}

	nPurchaseNo++;

	if( -4007 == ids)	//검색결과가 없으면 INSERT 
	{
		strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR,DIVISION_VALUE,LAST_NUMBER,CREATE_DATE,CREATE_WORKER,FIRST_WORK,MANAGE_CODE)")
						_T("VALUES	(%s.NEXTVAL,'PUR_NO','SE','%s','연속_차수번호',1,'%s','%s','%s',UDF_MANAGE_CODE);"), DEFAULT_SEQ_NAME, strYear,GetTodayDate(),m_pInfo->USER_ID,GetWorkLogMsg(WORK_LOG_SEPURCHASENO));
		pDM->ExecuteQuery(strQuery,1);
		nPurchaseNo = 1;	
	
	}
	// 초기화시 마지막테이블에 값을 업데이트 하므로
	// 차수를 선택하지 않고 취소하였을 경우에도 차수가 점점 올라감
	// 해당 부분 주석처리
	// 마지막 번호 업데이트 쿼리는 OnOK()에서 실행.
	// 2005-09-30 강봉수
	//
	/*
	else	
	{
		//있으면 UPDATE
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %d WHERE PUBLISH_FORM_CODE = 'SE' AND MAKE_YEAR = '%s' AND KIND_CODE = 'PUR_NO';"),nPurchaseNo,strYear);
		pDM->ExecuteQuery(strQuery,1);
	}
	*/

	pDM->EndFrame();
	strPurchaseNo.Format(_T("%d"),nPurchaseNo);
	
	return strPurchaseNo;
}

VOID CSePurchaseNoInsertDlg::OnOK() 
{	
	//save
	SaveAllDataToDM();
	
	//차수번호
	CString strPurchaseNo;
	INT nPurchaseNo = 0;
	m_pCM->GetControlMgrData(_T("차수번호"),strPurchaseNo);
	
	if(!strPurchaseNo.IsEmpty())
		nPurchaseNo = _ttoi(strPurchaseNo.GetBuffer(0));
	
	
	//차수년도
	CString strYear;
	m_pCM->GetControlMgrData(_T("차수년도"),strYear);

	//frame
	CArray<CString,CString> RemoveAliasList;
	
	m_pDM->StartFrame();

	// 차수번호 
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_작업상태"));
	m_pDM->InitDBFieldData();
	m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("SE_PURCHASE_SEQ_NO_TBL"), 0 , m_pDM);
	
	//작업 번호 
	RemoveAliasList.RemoveAll();
	m_pDM->InitDBFieldData();
	m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM_WORK, _T("SE_PURCHASE_WORK_NO_TBL"), 0 , m_pDM);
	
	//마지막 차수 번호 
	if(nPurchaseNo>0)
	{
		CString strQuery;
		//++2008.10.17 UPDATE BY PWR {{++
		// 통합시스템 적용
		strQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d ")
						 _T("WHERE PUBLISH_FORM_CODE='SE' AND MAKE_YEAR='%s' AND KIND_CODE='PUR_NO' ")
						 _T("AND MANAGE_CODE=UDF_MANAGE_CODE;"), nPurchaseNo, strYear );
		// strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %d WHERE PUBLISH_FORM_CODE = 'SE' AND MAKE_YEAR = '%s' AND KIND_CODE = 'PUR_NO';"),nPurchaseNo,strYear);
		//--2008.10.17 UPDATE BY PWR --}}
		m_pDM->AddFrame(strQuery);
	}
	

	INT ids = 0;
	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	
	if(ids<0)
	{
		AfxMessageBox( _T("입력에 실패하였습니다!") );
		return;
	} 
	else 
	{
		AfxMessageBox( _T("입력하였습니다!") );
	}


	CDialog::OnOK();
}

VOID CSePurchaseNoInsertDlg::SaveAllDataToDM()
{
	m_pDM->FreeData();
	m_pDM->InsertRow(-1);

	
	const INT cnt = 6;
	CString strAlias[cnt] = {
		_T("차수년도"),
		_T("차수번호"),
		_T("생성자"),	
		_T("생성일"),
		_T("차수년도"),
		_T("마지막작업번호"),
	};
	
	CString strREC_KEY = _T("");
	m_pDM->MakeRecKey(strREC_KEY);
	m_pDM->SetCellData(_T("REC_KEY"),strREC_KEY,0);

	CString strData;
	for(INT i=0;i<cnt;i++)
	{
		strData = _T("");
		m_pCM->GetControlMgrData(strAlias[i],strData);
		m_pDM->SetCellData(strAlias[i],strData,0);
	}
	
	CString strWorkLogMsg = _T("");
	strWorkLogMsg = GetWorkLogMsg(WORK_LOG_SEPURCHASENO);
	m_pDM->SetCellData(_T("최초작업"),strWorkLogMsg,0);
	m_pDM->SetCellData(_T("마지막작업"),strWorkLogMsg,0);
	m_pDM->SetCellData(_T("작업상태"),_T("0"),0); //구입대상
	
	
	CString strWorkNo = _T("");
	m_pCM->GetControlMgrData(_T("마지막작업번호"),strWorkNo);
	m_pDM_WORK->FreeData();
	

	//작업번호 관리 테이블에 INSERT
	m_pDM_WORK->InsertRow(-1);
		
	CString strWorkKey;
	m_pDM_WORK->MakeRecKey(strWorkKey);
	m_pDM_WORK->SetCellData(_T("REC_KEY"),strWorkKey,0);
	m_pDM_WORK->SetCellData(_T("차수관리KEY"),strREC_KEY,0);

	CString tmp = _T("");
	m_pCM->GetControlMgrData(_T("생성자"),tmp);
	m_pDM_WORK->SetCellData(_T("생성자"),tmp,0);
		
	tmp = _T("");
	m_pCM->GetControlMgrData(_T("생성일"),tmp);
	m_pDM_WORK->SetCellData(_T("생성일"),tmp,0);
		
	m_pDM_WORK->SetCellData(_T("마지막일련번호"),_T("0"),0);
	m_pDM_WORK->SetCellData(_T("최초작업"),strWorkLogMsg,0);
	m_pDM_WORK->SetCellData(_T("마지막작업"),strWorkLogMsg,0);
	m_pDM_WORK->SetCellData(_T("작업번호"),strWorkNo,0);


}

VOID CSePurchaseNoInsertDlg::OnCancel() 
{
	CDialog::OnCancel();
}

VOID CSePurchaseNoInsertDlg::OnchkPURNOINSERTUSEWORKNO() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(m_bUSE_WORK_NO)
	{
		m_pCM->SetControlMgrData(_T("마지막작업번호"),_T("0"));
	}
	else
	{
		m_pCM->SetControlMgrData(_T("마지막작업번호"),_T("1"));
	}

}

CString CSePurchaseNoInsertDlg::GetTodayDate()
{
	CString result;
	COleDateTime t = COleDateTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}
HBRUSH CSePurchaseNoInsertDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
