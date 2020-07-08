// SeBookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeBookDlg.h"
#include "..\SE_API\CheckinReceipt.h"
#include "..\SE_API\SeApi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeBookDlg dialog


CSeBookDlg::CSeBookDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeBookDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeBookDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_nCurrentIdx = -1;
	m_nACQ_TYPE = 1; //default
	m_bIsModifyStatus = FALSE;
	

	m_strACQ_VOL_KEY = _T("");
	m_strVOL_TITLE = _T(""); 
	m_strTITLE_INFO = _T(""); 
	m_strAUTHOR_INFO = _T("");
	m_strPUB_INFO = _T(""); 

	m_bIsLightVersion = FALSE;
	m_bIsDeleteProc = FALSE;
}

CSeBookDlg::~CSeBookDlg()
{
}

VOID CSeBookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeBookDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeBookDlg, CDialog)
	//{{AFX_MSG_MAP(CSeBookDlg)
	ON_BN_CLICKED(IDC_btnBOOK_RECEIPT, OnbtnBOOKRECEIPT)
	ON_BN_CLICKED(IDC_btnLINE_MODFY, OnbtnLINEMODFY)
	ON_BN_CLICKED(IDC_btnBOOK_DELETE, OnbtnBOOKDELETE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeBookDlg message handlers

BOOL CSeBookDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( InitESL_Mgr(_T("연속_수서_책")) < 0 ) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_책") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_연속_책"));
	m_pDM = FindDM(_T("DM_연속_책"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드")); 

	if(!m_pCM || !m_pDM || !m_pGrid) {
		AfxMessageBox( _T("CM,DM,Gird Error : 연속_수서_책") );
		return FALSE;
	}

	m_pCM->SetControlMgrData(_T("권호명"),m_strVOL_TITLE);
	
	if(m_strACQ_VOL_KEY.IsEmpty())
	{
		AfxMessageBox(_T("수입구분별권KEY 를 찾을 수 없습니다!"));
		return FALSE;
	}
	

	//자료 상태를 설정 해준다.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbBOOK_WORKING_STATUS);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("실시간인계자료") );
		pCombo->InsertString( 1, _T("제본인계자료") );
		pCombo->InsertString( 2, _T("등록인계자료") );
		pCombo->InsertString( 3, _T("기타자료상태") );
		pCombo->SetCurSel( 0 );
	}

	//Data Selection
	RefreshDM();
			
	//라이트 버젼 setting 
	if(m_bIsLightVersion)
	{
		SetLightVersion();
	}

	m_pGrid->Display();
	ShowAllData();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSeBookDlg::SetLightVersion()
{
	//라이트 버젼의 자료상태의 수정 콘트롤을 없애고 입수일 콘트롤의 위치를 재배치 한다 
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_STATIC_BOOK_WORKING_STATUS);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_cmbBOOK_WORKING_STATUS);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	CRect rect;
	
	pWnd = GetDlgItem(IDC_STATIC_BOOK_ACQ_DATE);
	if(pWnd)
	{
		rect.left = 280;
		rect.right = 330;
		rect.top = 230;
		rect.bottom = 250;

		pWnd->MoveWindow(rect);
	}
	
	pWnd = GetDlgItem(IDC_edtBOOK_ACQ_DATE);
	if(pWnd)
	{
		rect.left = 335;
		rect.right = 520;
		rect.top = 225;
		rect.bottom = 247;		
		
		pWnd->MoveWindow(rect);
	}
	
}

BEGIN_EVENTSINK_MAP(CSeBookDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeBookDlg)
	ON_EVENT(CSeBookDlg, IDC_gridBOOK, -600 /* Click */, OnClickgridBOOK, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSeBookDlg::OnClickgridBOOK() 
{
	// TODO: Add your control notification handler code here

	
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		m_nCurrentIdx = row-1;
		m_pGrid->SetReverse(row-1);
		ShowAllData();
		
		// color refresh
		RefreshGridColor();
	}
	
}

INT CSeBookDlg::RefreshDM()
{
	CString strWhere;
	strWhere.Format(_T("BOOK_TYPE = 'C' AND ACQ_TYPE_VOL_KEY = %s"),m_strACQ_VOL_KEY);
	m_pDM->RefreshDataManager(strWhere);
	m_nCurrentIdx = 0;
	m_pGrid->SetReverse(m_nCurrentIdx);
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료실구분"),_T("UDF_자료실"));
	
	//자료 상태 처리, RFID 여부
	CString strCode,strDesc,strRFIDSerial;
	CSeApi api(this);

	for(INT i=0;i<m_pDM->GetRowCount();i++)
	{
		strCode = _T("");
		strDesc = _T("");
		strRFIDSerial = _T("");

		strCode = m_pDM->GetCellData(_T("SB_자료상태"),i);
		strDesc = api.GetWorkingStatusDesc(strCode);
		m_pDM->SetCellData(_T("UDF_자료상태"),strDesc,i);


		strRFIDSerial = m_pDM->GetCellData(_T("SB_RFID"),i);
		strRFIDSerial.TrimLeft();
		strRFIDSerial.TrimRight();
		
		if(strRFIDSerial.IsEmpty())
		{
			m_pDM->SetCellData(_T("UDF_RFID"),_T("N"),i);
		}
		else
		{
			m_pDM->SetCellData(_T("UDF_RFID"),_T("Y"),i);
		}
	}

	return 0;
}

INT CSeBookDlg::ShowAllData()
{
	//그리드 빼고 
	m_pCM->ControlDisplay(_T("입수일"),m_nCurrentIdx);
	m_pCM->ControlDisplay(_T("비고"),m_nCurrentIdx);
	m_pCM->ControlDisplay(_T("자료실"),m_nCurrentIdx);


	//자료상태가 실시간인계-제본인계-등록인계중 하나면 자료상태 콤보를 데이터에 맞추어 준다
	//그 외에는 비활성화 
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbBOOK_WORKING_STATUS);
	if(!pCombo) return -1;

	CString strWorkingStatus;
	strWorkingStatus = m_pDM->GetCellData(_T("SB_자료상태"),m_nCurrentIdx);
	
	if(_T("SEL111O") == strWorkingStatus) //실시간
	{
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);
		m_bIsModifyStatus = TRUE;

	}
	else if(_T("SEB111N") == strWorkingStatus) //제본 
	{
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(1);
		m_bIsModifyStatus = TRUE;
	}
	else if(_T("SER111N") == strWorkingStatus) //등록
	{
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(2);
		m_bIsModifyStatus = TRUE;
	}
	else  
	{
		pCombo->EnableWindow(FALSE);
		pCombo->SetCurSel(3);
		m_bIsModifyStatus = FALSE;
	}

	return 0;
}

VOID CSeBookDlg::OnbtnBOOKRECEIPT() 
{
	// TODO: Add your control notification handler code here
	
	m_pGrid->SelectMakeList();
	INT idx = m_pGrid->SelectGetHeadPosition();

	if(idx<0) {
		AfxMessageBox(_T("선정된 정보가 없습니다!"));
			return;
	}
	
	CString strACQ_YEAR;
	strACQ_YEAR = m_pDM->GetCellData(_T("SB_수입년도"),idx);
	CCheckinReceipt dlg(this);
	dlg.m_strACQ_YEAR = strACQ_YEAR;
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	UpdateData(FALSE);
	

	if(dlg.DoModal() != IDOK) return;

	CString strRECEIPT_NO;
	CString strLAST_SERIAL_NO;
	CString strNEW_ACQ_YEAR = dlg.m_strACQ_YEAR;
	CString strNEW_RECEIPT_NO = dlg.m_strRECEIPT_NO;
	CSeApi api(this);

	while(idx >= 0)
	{
			
		//기존 수입년도 혹은 접수번호와 비교해서 다를 경우 새로 부여한다. 
		strACQ_YEAR = _T("");
		strRECEIPT_NO = _T("");
		strLAST_SERIAL_NO = _T("");

		strACQ_YEAR = m_pDM->GetCellData(_T("SB_수입년도"),idx);
		strRECEIPT_NO = m_pDM->GetCellData(_T("SB_접수번호"),idx);
			

		if(strACQ_YEAR.Compare(strNEW_ACQ_YEAR) || strRECEIPT_NO.Compare(strNEW_RECEIPT_NO))
		{
			//접수 입련 번호를 부여한다 
			strLAST_SERIAL_NO = api.GetReceiptSerialNo(m_pDM,m_nACQ_TYPE,strNEW_ACQ_YEAR,strNEW_RECEIPT_NO);
			if(!strLAST_SERIAL_NO.IsEmpty())
			{
				m_pDM->SetCellData(_T("SB_수입년도"),dlg.m_strACQ_YEAR,idx);
				m_pDM->SetCellData(_T("SB_접수번호"),dlg.m_strRECEIPT_NO,idx);
				m_pDM->SetCellData(_T("SB_접수일련번호"),strLAST_SERIAL_NO,idx);
			}
		}		

		idx = m_pGrid->SelectGetNext(); 
	}

	m_pGrid->Display();
	m_pGrid->SetReverse(m_nCurrentIdx);
}

VOID CSeBookDlg::OnbtnLINEMODFY() 
{
	// TODO: Add your control notification handler code here
	
	CString strDeleteYn;
	strDeleteYn = m_pDM->GetCellData(_T("UDF_삭제여부"),m_nCurrentIdx);
	if(_T("Y") == strDeleteYn)
	{
		AfxMessageBox(_T("삭제 처리될 책은 수정할 수 없습니다!"));
		return;
	}


	if(m_bIsModifyStatus)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbBOOK_WORKING_STATUS);
		if(!pCombo) return;
		INT nCurSel = pCombo->GetCurSel();
		
		if(3 == nCurSel)
		{
			AfxMessageBox(_T("기타자료상태는 적용이 불가능한 자료상태 입니다!"));
			return;
		}
		else if(1 == nCurSel)
		{
			//제본 인계자료는 이미 제본이 되어 있는 지를 검증한다 
			if(VerifyBindingBook()<0) return;
		}
	}
	
	

	//입수일 
	CString strData = _T("");
	m_pCM->GetControlMgrData(_T("입수일"),strData);
	m_pDM->SetCellData(_T("SB_입수일"),strData,m_nCurrentIdx);
	strData = _T("");
	
	//비고 
	m_pCM->GetControlMgrData(_T("비고"),strData);
	m_pDM->SetCellData(_T("SB_비고"),strData,m_nCurrentIdx);
	strData = _T("");
	
	//자료실
	m_pCM->GetControlMgrData(_T("자료실"),strData);
	m_pDM->SetCellData(_T("SB_배가위치_코드"),strData,m_nCurrentIdx);
	m_pDM->SetCellData(_T("UDF_자료실"),strData,m_nCurrentIdx);
	strData = _T("");
	

	//마지막작업
	m_pDM->SetCellData(_T("SB_마지막작업"),GetWorkLogMsg(WORK_LOG_SEBOOK),m_nCurrentIdx);


	CSeApi api(this);
	CString strToday;
	strToday = api.GetTodayDate();
	CString strDB_Working_Status;
	strDB_Working_Status = m_pDM->GetCellData(_T("DB_자료상태"),m_nCurrentIdx);


	//자료상태가 수정이 가능한 상태의 경우
	if(m_bIsModifyStatus)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbBOOK_WORKING_STATUS);
		if(!pCombo) return;
		
		INT nCurSel = pCombo->GetCurSel();
		CString strUDF = _T("");

		if(0 == nCurSel)
		{
			strData = _T("SEL111O");
			strUDF = _T("실시간인계자료");

			if(strDB_Working_Status != strData) //다른 곳으로 인계 되었을때 인계일을 설정해준다
			{
				m_pDM->SetCellData(_T("SB_실시간인계일"), strToday,m_nCurrentIdx);
			}
		}
		else if(1 == nCurSel)
		{
			strData = _T("SEB111N");
			strUDF = _T("제본인계자료");

			if(strDB_Working_Status != strData)
			{
				m_pDM->SetCellData(_T("SB_제본인계일"), strToday,m_nCurrentIdx);
			}

		}
		else if(2 == nCurSel)
		{
			strData = _T("SER111N");
			strUDF = _T("등록인계자료");

			if(strDB_Working_Status != strData)
			{
				m_pDM->SetCellData(_T("SB_등록인계일"), strToday,m_nCurrentIdx);
			}
		}

			
		m_pDM->SetCellData(_T("SB_자료상태"), strData,m_nCurrentIdx);
		m_pDM->SetCellData(_T("UDF_자료상태"), strUDF,m_nCurrentIdx);

	}
	

	//마지막 작업
	CString strWorkLogMsg;
	strWorkLogMsg = this->GetWorkLogMsg(WORK_LOG_SEBOOK);
	m_pDM->SetCellData(_T("SB_마지막작업"), strWorkLogMsg,m_nCurrentIdx);
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료실구분"),_T("UDF_자료실"));
	m_pGrid->Display();
	m_pGrid->SetReverse(m_nCurrentIdx);
	
}

INT CSeBookDlg::VerifyBindingBook()
{
	CString strBookKey;
	strBookKey = m_pDM->GetCellData(_T("SB_책KEY"),m_nCurrentIdx);

	CString strQuery;
	CString strResult;
	strQuery.Format(_T("SELECT BINDED_BOOK_YN FROM SE_BOOK_TBL WHERE REC_KEY = %s"),strBookKey);
	
	m_pDM->GetOneValueQuery(strQuery,strResult);

	strResult.TrimLeft();
	strResult.TrimRight();

	if(_T("Y") == strResult)
	{

		AfxMessageBox(_T("해당 책은 이미 제본되어 있으므로\n")
						_T("제본 인계처리 할 수 없습니다!"));
		
		return -1;
	}

	return 0;
}

VOID CSeBookDlg::OnOK() 
{
	// TODO: Add extra validation here
	// 일괄저장 함수 
	// 한 row 씩 저장하는 방식으로 전환
	
	INT nRowCnt = m_pDM->GetRowCount();
	CString strBOOK_KEY;
	CString strDeleteYn;
	CString msg;
	CArray<CString,CString> RemoveAliasList;
	CSeApi api(this);
	CESL_DataMgr* pDM_VOL = NULL;
	CESL_DataMgr* pDM_VOL_DELETE = NULL;
	INT ids = 0;

	pDM_VOL	= FindDM(_T("DM_연속_책삭제_권"));
	pDM_VOL_DELETE = FindDM(_T("DM_연속_책삭제_삭제권"));
	

	CString strACQ_TYPE_VOL_KEY,strVOL_KEY,strSPECIES_KEY;
	CString strtitle;
	strtitle = m_strTITLE_INFO + _T(" ") + m_strVOL_TITLE;


	if(!pDM_VOL || !pDM_VOL_DELETE)
	{
		AfxMessageBox(_T("저장관련 데이터 객체를 찾을 수 없습니다!"));
	}


	for(INT i=0;i<nRowCnt;i++)
	{
		strDeleteYn = _T("");
		strDeleteYn = m_pDM->GetCellData(_T("UDF_삭제여부"),nRowCnt-1-i);
		
		if(_T("Y") == strDeleteYn)
		{
			
			strBOOK_KEY = m_pDM->GetCellData(_T("SB_책KEY"),nRowCnt-1-i);
			strACQ_TYPE_VOL_KEY = m_pDM->GetCellData(_T("SB_수입구분별권KEY"),nRowCnt-1-i);
			strVOL_KEY = m_pDM->GetCellData(_T("SB_권KEY"),nRowCnt-1-i);
			strSPECIES_KEY = m_pDM->GetCellData(_T("SB_종KEY"),nRowCnt-1-i);

			CSeApi api(this);
			ids = api.BOOK_CAT_DELETE(m_pDM,
									  strBOOK_KEY,
									  strACQ_TYPE_VOL_KEY,
									  strVOL_KEY,
									  strSPECIES_KEY,
									  _T("C"),
									  strtitle,
									  FALSE,
									  FALSE);
			
	
			if(ids<0)
			{
				msg.Format(_T(" [ %d ] 번째 작업을 진행하던 중 문제가 생겨 작업을 중단합니다!"),nRowCnt-1-i);
				AfxMessageBox(msg);
				return;
			}
			else
			{
				m_bIsDeleteProc = TRUE;
				m_pDM->DeleteRow(nRowCnt-1-i);

			}

		}
		else 
		{

		
			m_pDM->StartFrame();

			RemoveAliasList.RemoveAll();
			RemoveAliasList.Add(_T("SB_책KEY"));
			RemoveAliasList.Add(_T("UDF_자료실"));
			RemoveAliasList.Add(_T("DB_자료상태"));
			
			strBOOK_KEY = _T("");
			strBOOK_KEY = m_pDM->GetCellData(_T("SB_책KEY"),nRowCnt-1-i);
			m_pDM->InitDBFieldData();
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("SE_BOOK_TBL") , nRowCnt-1-i , m_pDM, _T("REC_KEY") , _T("NUMERIC") , strBOOK_KEY);
				

			ids = m_pDM->SendFrame();
			m_pDM->EndFrame();
			
			if(ids<0)
			{
				msg.Format(_T(" [ %d ] 번째 작업을 진행하던 중 문제가 생겨 작업을 중단합니다!"),nRowCnt-1-i);
				AfxMessageBox(msg);
				return;
			}
		}

	}

	
	AfxMessageBox(_T("저장하였습니다!"));
	
	//삭제 오퍼레이션 수행시 하위 화면 refresh 
	if(m_bIsDeleteProc)
	{
		EndDialog(11);
	}

	//창을 닫지 않는다 
	return;
	
	CDialog::OnOK();
}

VOID CSeBookDlg::OnbtnBOOKDELETE() 
{
	// TODO: Add your control notification handler code here
	CString strWorkingStatus;
	CString strMsg;

	strWorkingStatus = m_pDM->GetCellData(_T("UDF_자료상태"),m_nCurrentIdx);
	strMsg.Format(_T("[ %s ] 상태인 책을 삭제 하시겠습니까?"), strWorkingStatus);

	if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) return; 

	m_pDM->SetCellData(_T("UDF_삭제여부"),_T("Y"),m_nCurrentIdx);
	


	if(VerifyDeleteProc()<0)
	{
		m_pDM->SetCellData(_T("UDF_삭제여부"),_T(""),m_nCurrentIdx);
		return;
	}
	

	
	//정리와 동일한 방식으로 한번더 삭제 검증
	CString strBOOK_KEY,strACQ_TYPE_VOL_KEY,strVOL_KEY,strSPECIES_KEY;
	strBOOK_KEY = m_pDM->GetCellData(_T("SB_책KEY"),m_nCurrentIdx);
	strACQ_TYPE_VOL_KEY = m_pDM->GetCellData(_T("SB_수입구분별권KEY"),m_nCurrentIdx);
	strVOL_KEY = m_pDM->GetCellData(_T("SB_권KEY"),m_nCurrentIdx);
	strSPECIES_KEY = m_pDM->GetCellData(_T("SB_종KEY"),m_nCurrentIdx);

	INT ids = -1;

  	CString strtitle;
	strtitle = m_strTITLE_INFO + _T(" ") + m_strVOL_TITLE;

	CSeApi api(this);
	ids = api.BOOK_CAT_DELETE(m_pDM,
		                      strBOOK_KEY,
						      strACQ_TYPE_VOL_KEY,
						      strVOL_KEY,
						      strSPECIES_KEY,
						      _T("C"),
						      strtitle,
						      FALSE,
						      TRUE);
   
	if(ids<0)
	{
		m_pDM->SetCellData(_T("UDF_삭제여부"),_T(""),m_nCurrentIdx);
		return;
	}


	// color refresh
	RefreshGridColor();

}

INT CSeBookDlg::VerifyDeleteProc()
{
	
	//1) 모든 입수 책이 삭제 되었을 경우 -- 기사, 부록 유무 판별
	
	INT nRowCnt = m_pDM->GetRowCount();
	CString strDeleteYn;
	INT nDeleteCnt = 0;
	CString strQuery;
	CString msg;

	for(INT i=0;i<nRowCnt;i++)
	{
		strDeleteYn = _T("");	
		strDeleteYn = m_pDM->GetCellData(_T("UDF_삭제여부"),i);
		if(_T("Y") == strDeleteYn) nDeleteCnt++;
	}

	if(nDeleteCnt == nRowCnt)
	{
		CString strAcqVolKey;
		CString strVolKey;
		strAcqVolKey = m_pDM->GetCellData(_T("SB_수입구분별권KEY"),m_nCurrentIdx);
		strVolKey = m_pDM->GetCellData(_T("SB_권KEY"),m_nCurrentIdx);
		
		//결호 책수 조사
		INT nMissBookCnt = 0;
		CString strMissBookCnt;
		strQuery.Format(_T("SELECT MISS_BOOK_CNT FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s"),strAcqVolKey);
		m_pDM->GetOneValueQuery(strQuery,strMissBookCnt);
		if(!strMissBookCnt.IsEmpty()) nMissBookCnt = _ttoi(strMissBookCnt.GetBuffer(0));
		
		
		if(0 == nMissBookCnt) //해당 권호 삭제 
		{
			//다른 수입 구분이 있는지를 조사
			CString strResult;
			INT nAcqVolCnt = 0;
			strQuery.Format(_T("SELECT COUNT(*) FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s"),strVolKey);
			m_pDM->GetOneValueQuery(strQuery,strResult);			
			if(!strResult.IsEmpty()) nAcqVolCnt = _ttoi(strResult.GetBuffer(0));
			
			if(nAcqVolCnt>1) //다른 수입구분이 있는 경우 (해당 권호 삭제 메시지)
			{
				msg.Format(_T("해당책 삭제시 남은 책이 없게 되어\n") 
							_T("권호 정보가 사라집니다\n")
							_T("그래도 삭제 하시겠습니까?"));
				
				if(AfxMessageBox(msg,MB_YESNO) != IDYES) return -1001;

			}
			else  //다른 수입구분이 없는 경우 (해당 권호의 기사와 부록을 확인 후 삭제 메시지)
			{
				
				CString strArticleCnt;
				INT nArticleCnt;
				strQuery.Format(_T("SELECT COUNT(*) FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s"),strVolKey);
				m_pDM->GetOneValueQuery(strQuery,strArticleCnt);
				nArticleCnt = _ttoi(strArticleCnt.GetBuffer(0));
					
				if(nArticleCnt>0)
				{
					AfxMessageBox(_T("해당 권호에  기사정보가 있어 해당 책을 삭제 할수 없습니다!\n")
							   _T("먼저 기사정보를 삭제하신 후 삭제 하실 수 있습니다."));					  
					return -1002;
				}
				
				CString strAppendixCnt;
				INT nAppendixCnt;
				strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND BOOK_TYPE = 'A'"),strVolKey);
				m_pDM->GetOneValueQuery(strQuery,strAppendixCnt);
				nAppendixCnt = _ttoi(strAppendixCnt.GetBuffer(0));
	
				if(nAppendixCnt>0)
				{
					AfxMessageBox(_T("해당 권호에 부록 정보가 있어 해당 책을 삭제 할수 없습니다!\n")
									_T("먼저 부록 정보를 삭제하신 후 삭제 하실 수 있습니다."));	
					return -1003;
				}

				msg.Format(_T("해당책 삭제시 남은 책이 없게 되어\n") 
				_T("권호 정보가 사라집니다\n")
				_T("그래도 삭제 하시겠습니까?"));
				
				if(AfxMessageBox(msg,MB_YESNO) != IDYES) return -1001;
			}

		}
	}

	
	return 0;
}

VOID CSeBookDlg::RefreshGridColor()
{
	INT nRowCnt = m_pDM->GetRowCount();
	
	CString strDeleteYn;
	
	for(INT i=0;i<nRowCnt;i++)
	{
		strDeleteYn = _T("");
		strDeleteYn = m_pDM->GetCellData(_T("UDF_삭제여부"),i);
		
		if(_T("Y") == strDeleteYn)
		{
			SetGridRowUniqueColor(i);
		}

	}

}

VOID CSeBookDlg::SetGridRowUniqueColor(INT nRow)
{
	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT nCol = m_pGrid->GetCol();
	INT nCols = m_pGrid->GetCols(0);
	
	if (nRow >= 0)
	{
		m_pGrid->SetRow(nRow + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			m_pGrid->SetCol(i);
			m_pGrid->SetCellBackColor(red);
			m_pGrid->SetCellForeColor(black);
		}
	}
	
	m_pGrid->SetCol(nCol);
}

VOID CSeBookDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	if(m_bIsDeleteProc)
	{
		EndDialog(11);
	}

	CDialog::OnCancel();
}

HBRUSH CSeBookDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
