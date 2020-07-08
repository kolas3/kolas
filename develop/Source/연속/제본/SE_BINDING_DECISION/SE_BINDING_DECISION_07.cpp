// SE_BINDING_DECISION_07.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_DECISION_07.h"

#include "AddNoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_DECISION_07 dialog

CSE_BINDING_DECISION_07::CSE_BINDING_DECISION_07(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pDM	= NULL;
	m_pCM	= NULL;
	m_pGrid = NULL;
	m_strACQ_YEAR	= _T("");
	m_strRECEIPT_NO = _T("");
	m_nCurrentIdx	= -1;
}

CSE_BINDING_DECISION_07::~CSE_BINDING_DECISION_07()
{
}

VOID CSE_BINDING_DECISION_07::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_DECISION_07)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_BINDING_DECISION_07, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_DECISION_07)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bADD, OnbADD)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_DECISION_07 message handlers

BOOL CSE_BINDING_DECISION_07::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_DECISION_07")) < 0 ) {

		AfxMessageBox( _T("InitESL_Mgr() Error!") );
		return FALSE ;
	}

	if( Init() < 0 ) {
		AfxMessageBox( _T("Init() Error!") );
		return FALSE ;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CSE_BINDING_DECISION_07::Init()
{
	m_pCM	= FindCM( _T("CM_SE_BINDING_DECISION_07") );
	m_pGrid = (CESL_Grid*)m_pCM->FindControl( _T("NoGrid") );
	m_pDM	= FindDM( _T("DM_SE_BINDING_DECISION_07") );

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("Mgr Error!") );
		return -1;
	}

	m_pCM->SetControlMgrData( _T("AcquisitYear"), m_strACQ_YEAR );  

	if( !m_strACQ_YEAR.IsEmpty() ) {

		CString strQuery;
		strQuery.Format( _T("ACQ_YEAR = '%s'"), m_strACQ_YEAR );
		m_pDM->RefreshDataManager(strQuery);
		m_pGrid->Display();
	}
	return 0;
}

VOID CSE_BINDING_DECISION_07::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here
	m_pCM->GetControlMgrData( _T("AcquisitYear"), m_strACQ_YEAR );
	
	if( m_strACQ_YEAR.IsEmpty() ) {
		AfxMessageBox( _T("수입년도가 입력되지 않았습니다!") );
		return;
	}

	CString strQuery;
	strQuery.Format( _T("ACQ_YEAR = '%s'"), m_strACQ_YEAR );

	m_pDM->RefreshDataManager(strQuery);
	m_pGrid->Display();
}

VOID CSE_BINDING_DECISION_07::OnbADD() 
{
	// 접수번호추가 
	m_pCM->GetControlMgrData( _T("AcquisitYear"), m_strACQ_YEAR );  
	
	if(m_strACQ_YEAR.IsEmpty())
	{
		AfxMessageBox( _T("수입년도가 입력되지 않았습니다!") );
		return;
	}
	
	CString strRECEIPT_NO;
	CString strQuery;
	CString strACQ_YEAR;
	INT nRECEIPT_NO = 1;
	INT nBeforeNo = 0;

	strQuery.Format( _T("SELECT MAX(SEQ_NO) FROM SE_BINDING_SEQ_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s'"), m_strACQ_YEAR );
	m_pDM->GetOneValueQuery(strQuery,strRECEIPT_NO);

	if(!strRECEIPT_NO.IsEmpty()) {
		nBeforeNo = _ttoi(strRECEIPT_NO.GetBuffer(0));
		nRECEIPT_NO = nBeforeNo +1;
	}

	strRECEIPT_NO.Format( _T("%d"), nRECEIPT_NO );
	CString strSTATIC;
	strSTATIC.Format( _T("수입년도:%s"), m_strACQ_YEAR );

	CAddNoDlg dlg;
	dlg.m_sNewSeqNo		= strRECEIPT_NO;
	dlg.m_stcNewSeqNo	= strSTATIC;
	dlg.m_sAcqYear		= m_strACQ_YEAR;
	dlg.m_pParentDM		= m_pDM;
	UpdateData( FALSE );
	
	if(dlg.DoModal() != IDOK) return;
	
	//
	CString sData;

	//추가
	m_pDM->InsertRow(-1);
	INT nRowIdx = m_pDM->GetRowCount()-1;

	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();

	//REC_KEY
	m_pDM->MakeRecKey(sData);
	m_pDM->AddDBFieldData( _T("REC_KEY")				, _T("NUMERIC"), sData );
	m_pDM->AddDBFieldData( _T("ACQ_YEAR")			, _T("STRING"), m_strACQ_YEAR );
	m_pDM->AddDBFieldData( _T("SEQ_NO")					, _T("NUMERIC"), dlg.m_sNewSeqNo );
	m_pDM->AddDBFieldData( _T("LAST_SERIAL_NO")			, _T("NUMERIC"), _T("0") );
	m_pDM->AddDBFieldData( _T("SEQ_NO_CREATE_DATE")		, _T("STRING"), GetTodayDate() );
	m_pDM->AddDBFieldData( _T("SEQ_NO_CREATE_WORKER")	, _T("STRING"), m_pInfo->USER_ID );
	m_pDM->AddDBFieldData( _T("FIRST_WORK")				, _T("STRING"), GetWorkLogMsg(WORK_LOG_SEBIND_DECISION) );
	m_pDM->AddDBFieldData( _T("LAST_WORK")				, _T("STRING"), GetWorkLogMsg(WORK_LOG_SEBIND_DECISION) );

	//
	INT ids = m_pDM->MakeInsertFrame( _T("SE_BINDING_SEQ_TBL") );
	if( ids < 0 ) return;

	ids = m_pDM->SendFrame();	
	if(ids<0)
	{
		AfxMessageBox( _T("입력에 실패 하였습니다!") );
		m_pDM->DeleteRow(nRowIdx);
		return;
	}
	else
	{
		AfxMessageBox( _T("입력하였습니다!") );
		OnbSEARCH();      
		return;

	}
}

VOID CSE_BINDING_DECISION_07::OnbDELETE() 
{
	if(m_nCurrentIdx<0) 
	{
		AfxMessageBox( _T("선택된 항목이 없습니다!") );
		return;
	}
	
	CString strBookCnt;
	strBookCnt = m_pDM->GetCellData( _T("UDF_책수"), m_nCurrentIdx );

	INT nBook;
	nBook = _ttoi(strBookCnt.GetBuffer(0));

	if(0 != nBook)
	{
		AfxMessageBox( _T("작업중인 작업번호는 삭제할 수 없습니다!") );
		return;
	}


	CString msg;
	CString strNo;
	strNo = m_pDM->GetCellData( _T("SEQ_NO"), m_nCurrentIdx );
	
	msg.Format( _T(" [ %s ] 발주번호를 삭제하시겠습니까?"), strNo );
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;
	

	CString strREC_KEY;
	strREC_KEY = m_pDM->GetCellData( _T("REC_KEY"), m_nCurrentIdx );
	

	if(!strREC_KEY.IsEmpty())
	{
		CString strQuery;
		strQuery.Format( _T("DELETE FROM SE_BINDING_SEQ_TBL WHERE REC_KEY = %s;"), strREC_KEY );
		
		m_pDM->StartFrame();
		m_pDM->InitDBFieldData();
		m_pDM->AddFrame(strQuery);
		INT ids = m_pDM->SendFrame();	
	
		if(ids<0)
		{
			AfxMessageBox( _T("삭제에 실패 하였습니다!") );
			return;
		}
		else
		{
			AfxMessageBox( _T("삭제하였습니다!") );
			m_pDM->DeleteRow(m_nCurrentIdx);
			m_pGrid->Display();      
			return;
		}
	}
}

VOID CSE_BINDING_DECISION_07::OnOK() 
{
	// TODO: Add extra validation here
	if(m_nCurrentIdx<0) 
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}

	m_strRECEIPT_NO = m_pDM->GetCellData( _T("SEQ_NO"),m_nCurrentIdx );	
	
	if(m_strRECEIPT_NO.IsEmpty())
	{
		AfxMessageBox(_T("발주번호 부여에 실패하였습니다!"));
		return;
	}
	
	m_strACQ_YEAR = m_pDM->GetCellData(_T("ACQ_YEAR"),m_nCurrentIdx);	
	
	if(m_strACQ_YEAR.IsEmpty())
	{
		AfxMessageBox(_T("수입년도를 찾을 수 없습니다!"));
		return;
	}

	CDialog::OnOK();
}

BEGIN_EVENTSINK_MAP(CSE_BINDING_DECISION_07, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_BINDING_DECISION_07)
	ON_EVENT(CSE_BINDING_DECISION_07, IDC_LIST_NO, -600 /* Click */, OnClickListNo, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_BINDING_DECISION_07::OnClickListNo() 
{
	INT row = m_pGrid->GetRow();

	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			  m_nCurrentIdx = row-1;
		}
	}	
}

CString CSE_BINDING_DECISION_07::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format( _T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

HBRUSH CSE_BINDING_DECISION_07::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
