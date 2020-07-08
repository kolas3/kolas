// SeAppendixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeAppendixDlg.h"
#include "..\SE_API\SeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeAppendixDlg dialog


CSeAppendixDlg::CSeAppendixDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeAppendixDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeAppendixDlg)
	m_bDISCARD_YN = FALSE;
	m_bREG_FLAG = FALSE;
	//}}AFX_DATA_INIT
	
	m_pDM = NULL;
	m_pDM_BOOK = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	strVOL_TITLE = _T("");
	strACQ_VOL_KEY = _T("");
	m_nCurrentIdx = -1;
	m_nACQ_TYPE = 1; //default
	m_arrayDeleteAppendixKey.RemoveAll();
	m_arrayDeleteBookKey.RemoveAll();
	m_nCurrentParentIdx = 0;
	m_strNewArticleCnt = _T("");
	m_bIsModifyStatus = FALSE;
}

CSeAppendixDlg::~CSeAppendixDlg()
{
}

VOID CSeAppendixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeAppendixDlg)
	DDX_Check(pDX, IDC_chkAPPENDIX_DISCARD_YN, m_bDISCARD_YN);
	DDX_Check(pDX, IDC_chkAPPENDIX_REG_FLAG, m_bREG_FLAG);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeAppendixDlg, CDialog)
	//{{AFX_MSG_MAP(CSeAppendixDlg)
	ON_BN_CLICKED(IDC_btnAPPENDIX_INSERT, OnbtnAPPENDIXINSERT)
	ON_BN_CLICKED(IDC_btnAPPENDIX_MODIFY, OnbtnAPPENDIXMODIFY)
	ON_BN_CLICKED(IDC_btnAPPENDIX_DELETE, OnbtnAPPENDIXDELETE)
	ON_BN_CLICKED(IDC_btnAPPENDIX_CLEAR, OnbtnAPPENDIXCLEAR)
	ON_BN_CLICKED(IDC_chkAPPENDIX_DISCARD_YN, OnchkAPPENDIXDISCARDYN)
	ON_BN_CLICKED(IDC_chkAPPENDIX_REG_FLAG, OnchkAPPENDIXREGFLAG)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeAppendixDlg message handlers

BOOL CSeAppendixDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr(_T("연속_수서_부록")) < 0 ) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_부록") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_연속_부록"));
	m_pDM = FindDM(_T("DM_연속_부록"));
	m_pDM_BOOK = FindDM(_T("DM_연속_부록_책"));
	m_pDM_WORK = FindDM(_T("DM_체크인_작업"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드")); 

	if(!m_pCM || !m_pDM || !m_pGrid || !m_pDM_BOOK || !m_pDM_WORK) {
		AfxMessageBox( _T("CM,DM,Gird Error : 연속_수서_부록") );
		return FALSE;
	}

	m_pCM->SetControlMgrData(_T("권호명"),strVOL_TITLE);
	
	if(strACQ_VOL_KEY.IsEmpty())
	{
		AfxMessageBox(_T("수입구분별권KEY 를 찾을 수 없습니다!"));
		return FALSE;
	}
	

	
	//자료 상태를 설정 해준다.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("적용안함") );
		pCombo->InsertString( 1, _T("실시간인계자료") );
		pCombo->InsertString( 2, _T("등록인계자료") );
		pCombo->InsertString( 3, _T("기타자료상태") );
		pCombo->SetCurSel( 0 );
	}

	
	CString strWhere;
	strWhere.Format(_T("A.BOOK_KEY = B.REC_KEY AND B.BOOK_TYPE = 'A' AND B.ACQ_TYPE_VOL_KEY = %s"),strACQ_VOL_KEY);
	m_pDM->RefreshDataManager(strWhere);
	
	if(m_pDM->GetRowCount()>0) {
		m_nCurrentIdx = 0;
		m_pGrid->SetReverse(m_nCurrentIdx);
		ShowAllData();
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CSeAppendixDlg::ShowAllData()
{
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("형식구분"),_T("UDF_형식구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("매체구분"),_T("UDF_매체구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("등록구분"),_T("UDF_등록구분"));

	CString strREG_FLAG;
	CString strWorkingStatus;
	CString strUDF_Working_Status;

	INT nRow = m_pDM->GetRowCount();
	CSeApi api(this);

	//전체 자료상태처리
	for(INT i=0;i<nRow;i++)
	{
		strREG_FLAG = m_pDM->GetCellData(_T("단행본등록여부"),i);
		strWorkingStatus = m_pDM->GetCellData(_T("자료상태"),i);
		strWorkingStatus.TrimLeft();
		strWorkingStatus.TrimRight();

		
		if(_T("Y") == strREG_FLAG) 
		{
			strUDF_Working_Status = _T("단행본등록자료");
		}
		else
		{
			if(strWorkingStatus.IsEmpty())
			{
				strUDF_Working_Status = _T("적용안함");
			}
			else
			{
				strUDF_Working_Status = api.GetWorkingStatusDesc(strWorkingStatus);
			}
		}

		m_pDM->SetCellData(_T("UDF_자료상태"),strUDF_Working_Status,i);
	}
	
	
	
	m_pCM->AllControlDisplay(m_nCurrentIdx);
	

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS);
	if(!pCombo) return -1;


	//단행본등록여부 (단행본 등록 상태로 되어 있으면 자료상태는 기타로 해서 disable한다
	strREG_FLAG = m_pDM->GetCellData(_T("단행본등록여부"),m_nCurrentIdx);
	if(_T("Y") == strREG_FLAG) {

		m_bREG_FLAG = TRUE;

	}
	else 
	{
		m_bREG_FLAG = FALSE;
	}

	//자료상태가 적용안함(빈칸)-실시간인계-등록인계중 하나면 자료상태 콤보를 데이터에 맞추어 준다
	//그 외에는 비활성화 

	strWorkingStatus = m_pDM->GetCellData(_T("자료상태"),m_nCurrentIdx);
	strWorkingStatus.TrimLeft();
	strWorkingStatus.TrimRight();

	if(m_bREG_FLAG) //단행본 등록 상태 
	{
		pCombo->EnableWindow(FALSE);
		pCombo->SetCurSel(3);
		m_bIsModifyStatus = FALSE;
		GetDlgItem(IDC_chkAPPENDIX_REG_FLAG)->EnableWindow(TRUE);
	}
	else //연속자료로 데이터 처리시
	{

		if(strWorkingStatus.IsEmpty()) //적용안함
		{
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(0);
			m_bIsModifyStatus = TRUE;
			GetDlgItem(IDC_chkAPPENDIX_REG_FLAG)->EnableWindow(TRUE);

		}
		else if(_T("SEL111O") == strWorkingStatus) //실시간
		{
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(1);
			m_bIsModifyStatus = TRUE;
			GetDlgItem(IDC_chkAPPENDIX_REG_FLAG)->EnableWindow(TRUE);

		}
		else if(_T("SER111N") == strWorkingStatus) //등록
		{
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(2);
			m_bIsModifyStatus = TRUE;
			GetDlgItem(IDC_chkAPPENDIX_REG_FLAG)->EnableWindow(TRUE);
		}
		else  //기타 자료 상태(실시간 또는 등록 된 자료 -> 단행본으로 등록 하지 못하게 한다)
		{
			pCombo->EnableWindow(FALSE);
			pCombo->SetCurSel(3);
			m_bIsModifyStatus = FALSE;
			GetDlgItem(IDC_chkAPPENDIX_REG_FLAG)->EnableWindow(FALSE);
		}
	}

	
	//폐기여부 
	CString strDISCARD_YN;
	strDISCARD_YN = m_pDM->GetCellData(_T("폐기여부"),m_nCurrentIdx);
	if(_T("Y") == strDISCARD_YN)  m_bDISCARD_YN = TRUE;
	else m_bDISCARD_YN = FALSE;

	UpdateData(FALSE);
	m_pGrid->SetReverse(m_nCurrentIdx);

	return 0;
}

BEGIN_EVENTSINK_MAP(CSeAppendixDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeAppendixDlg)
	ON_EVENT(CSeAppendixDlg, IDC_gridAPPENDIX, -600 /* Click */, OnClickgridAPPENDIX, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSeAppendixDlg::OnClickgridAPPENDIX() 
{
	// TODO: Add your control notification handler code here

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			m_nCurrentIdx = row-1;
			ShowAllData();
		}
	
	}

}

VOID CSeAppendixDlg::OnbtnAPPENDIXINSERT() 
{
	// TODO: Add your control notification handler code here
	CString tmp;
	m_pCM->GetControlMgrData(_T("부록명"),tmp);
	if(tmp.IsEmpty()) {
		AfxMessageBox(_T("부록명이 입력되지 않았습니다!"));
		return;
	}

	//단행본 등록 여부가 표시되지 않을때 기타 자료상태는 적용할 수 없다
	UpdateData(TRUE);
	if(!m_bREG_FLAG) {
		
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS);
		if(pCombo)
		{
			INT nCurSel = -1;
			nCurSel = pCombo->GetCurSel();
			if(3 == nCurSel) 
			{
				AfxMessageBox(_T("기타자료상태는 사용자가 적용할 수 없습니다!"));
				return;
				
			}
		}
	}



	m_pDM->InsertRow(-1);
	INT row = m_pDM->GetRowCount()-1;
	SetAllData(row, _T("I")); //insert
	OnOK();
	
}

VOID CSeAppendixDlg::SetAllData(INT row, CString strUDF)
{
	const INT cnt = 6;
	CString alias[cnt];
	CString data[cnt];

	alias[0] = _T("부록명");
	alias[1] = _T("형식구분");
	alias[2] = _T("매체구분");
	alias[3] = _T("비고");
	alias[4] = _T("폐기일");
	alias[5] = _T("폐기사유");
	

	for(INT i=0;i<cnt;i++)
	{
		m_pCM->GetControlMgrData(alias[i],data[i]);
	}

	m_pDM->SetCellData(alias,6,data,row);
	
	CSeApi api(this);
	
	//UDF처리 
	m_pDM->SetCellData(_T("UDF_형식구분"),data[1],row);
	m_pDM->SetCellData(_T("UDF_매체구분"),data[2],row);


	//단행본등록여부
	//단행본 등록 여부가 표시면 자료상태를 지워주고, 있지 않으면 자료상태를 적용해준다
	UpdateData(TRUE);
	CString strREG_FLAG;
	CString strWorking_Status;
	CString strDB_Working_Status;
	CString strToday = api.GetTodayDate();
	strDB_Working_Status = m_pDM->GetCellData(_T("DB자료상태"),row);
	
	if(m_bREG_FLAG) {
		
		strREG_FLAG = _T("Y");
		strWorking_Status = _T("");
	}
	else
	{
		
		strREG_FLAG = _T("N");

		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS);
		if(pCombo)
		{
			INT nCurSel = -1;
			nCurSel = pCombo->GetCurSel();
			if(0 == nCurSel) 
			{
				strWorking_Status = _T("");

			}
			else if(1 == nCurSel) 
			{
				strWorking_Status = _T("SEL111O");
				//DB자료상태와 다르면 실시간인계일(IMM_LOC_TRANSFER_DATE) 설정
				
				if(strWorking_Status != strDB_Working_Status)
				{
					m_pDM->SetCellData(_T("실시간인계일"),strToday,row);
				}

			}
			else if(2 == nCurSel)
			{
			
				strWorking_Status = _T("SER111N");
				//DB자료상태와 다르면 등록인계일(REG_TRANSFER_DATE) 설정
				if(strWorking_Status != strDB_Working_Status)
				{
					m_pDM->SetCellData(_T("등록인계일"),strToday,row);
				}

			}

		}
	}

	
	m_pDM->SetCellData(_T("자료상태"),strWorking_Status,row);
	m_pDM->SetCellData(_T("단행본등록여부"),strREG_FLAG,row);
	


	//폐기여부
	CString strDISCARD_YN;
	if(m_bDISCARD_YN) strDISCARD_YN = _T("Y");
	else strDISCARD_YN = _T("N");
	
	m_pDM->SetCellData(_T("폐기여부"),strDISCARD_YN,row);
	m_pDM->SetCellData(_T("UDF_상태"),strUDF,row); 

	
	//폐기여부에 따른 등록 구분 설정
	if(_T("N") == strDISCARD_YN) {
		CString strREG_CODE;
		m_pCM->GetControlMgrData(_T("등록구분"),strREG_CODE);
		m_pDM->SetCellData(_T("등록구분"),strREG_CODE,row);
		m_pDM->SetCellData(_T("UDF_등록구분"),strREG_CODE,row);
	}


	m_nCurrentIdx =	row;
	ShowAllData();
}

VOID CSeAppendixDlg::OnbtnAPPENDIXMODIFY() 
{
	// TODO: Add your control notification handler code here
	if( 0 == m_pDM->GetRowCount() || m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}

	CString tmp;
	m_pCM->GetControlMgrData(_T("부록명"),tmp);
	if(tmp.IsEmpty()) {
		AfxMessageBox(_T("부록명이 입력되지 않았습니다!"));
		return;
	}

	//단행본 등록 여부가 표시되지 않을때 기타 자료상태는 적용할 수 없다
	UpdateData(TRUE);
	if(!m_bREG_FLAG) {
		
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS);
		if(pCombo)
		{
			INT nCurSel = -1;
			nCurSel = pCombo->GetCurSel();
			if(3 == nCurSel) 
			{
				AfxMessageBox(_T("기타자료상태는 사용자가 적용할 수 없습니다!"));
				return;
				
			}
		}
	}

	
	CString strUDF;
	strUDF = m_pDM->GetCellData(_T("UDF_상태"),m_nCurrentIdx);

	if(_T("I") == strUDF)
	{
		SetAllData(m_nCurrentIdx, _T("I")); //insert
	}
	else 
	{
		SetAllData(m_nCurrentIdx, _T("U")); //update
	}
	OnOK();

}

VOID CSeAppendixDlg::OnbtnAPPENDIXDELETE() 
{
	// TODO: Add your control notification handler code here
	if( 0 == m_pDM->GetRowCount() || m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}
	
	
	CString strAPPENDIX_NAME;
	strAPPENDIX_NAME = m_pDM->GetCellData(_T("부록명"),m_nCurrentIdx);

	CString msg;
	msg.Format(_T("[ %s ] 부록을 삭제하시겠습니까?"), strAPPENDIX_NAME);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;
	
		

	//자료상태를 검증해 자료상태가 인계상태 이상이면 삭제를 제한한다(아직적용하지 않는다)
	
	
						
	
	//대출상태등을 검증한다
	CString strBOOK_KEY,strVOL_KEY;

	strBOOK_KEY = m_pDM->GetCellData(_T("책KEY"),m_nCurrentIdx);
	strVOL_KEY = m_pDM->GetCellData(_T("권KEY"),m_nCurrentIdx);
	

	CSeApi api(this);
	
	INT ids = -1;

	ids = api.BOOK_DELETE_VERIFY(m_pDM,
							     strBOOK_KEY,
							     strVOL_KEY,
							     _T("A"),
							     strAPPENDIX_NAME);

	if(ids<0) return;





	CString strUDF;
	strUDF = m_pDM->GetCellData(_T("UDF_상태"),m_nCurrentIdx);
	if(_T("I") == strUDF)
	{
		m_pDM->DeleteRow(m_nCurrentIdx);
	
	}
	else 
	{
		CString strAPPENDIX_KEY;
		strAPPENDIX_KEY = m_pDM->GetCellData(_T("부록KEY"),m_nCurrentIdx);
		m_arrayDeleteAppendixKey.Add(strAPPENDIX_KEY);
		
		CString strBOOK_KEY;
		strBOOK_KEY = m_pDM->GetCellData(_T("책KEY"),m_nCurrentIdx);
		m_arrayDeleteBookKey.Add(strBOOK_KEY);

		m_pDM->DeleteRow(m_nCurrentIdx);
	}
	
	INT nRowCnt = m_pDM->GetRowCount();
	

	if(nRowCnt>0)
	{
		m_nCurrentIdx = 0;
		ShowAllData();
	}
	else if(0 == nRowCnt)
	{
		m_nCurrentIdx = -1;
		OnbtnAPPENDIXCLEAR();
		m_pGrid->Display();
	}
	OnOK();
	
}

VOID CSeAppendixDlg::OnbtnAPPENDIXCLEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->AllControlClear();
	m_bREG_FLAG = FALSE;
	m_bDISCARD_YN = FALSE;
	m_pCM->SetControlMgrData(_T("권호명"),strVOL_TITLE);
	UpdateData(FALSE);

}

VOID CSeAppendixDlg::OnOK() 
{
	// TODO: Add extra validation here
	// 일괄 저장 함수 
	INT nSize;
	nSize = m_arrayDeleteAppendixKey.GetSize();
	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();
	if(0 == nSize && 0 == nRowCnt)
	{
		AfxMessageBox(_T("변화된 내용이 없습니다!"));
		return;
	}


	CString strUDF;
	CString strQuery;
	m_pDM->StartFrame();
	CSeApi api(this);
	
	//각 정보의 연계된 foreign KEY를 설정한다
	CString strSPECIES_KEY;
	strSPECIES_KEY = m_pDM_WORK->GetCellData(_T("ISS_종KEY"),m_nCurrentParentIdx);
	CString strVOL_KEY;
	strVOL_KEY = m_pDM_WORK->GetCellData(_T("SV_권KEY"),m_nCurrentParentIdx);
	CString strACQ_VOL_KEY;
	strACQ_VOL_KEY = m_pDM_WORK->GetCellData(_T("SA_수입구분별권KEY"),m_nCurrentParentIdx);
	

	INT nIdx_book;
	CString strREMARK;
	CString strREG_CODE;
	CString strWorking_Status;
	CArray<CString,CString> RemoveAliasList;
	CString strAPPENDIX_KEY;
	CString strBOOK_KEY;
	CString strIMM_LOC_TRANSFER_DATE;
	CString strREG_TRANFER_DATE;
	CString strWorkLogMsg = this->GetWorkLogMsg(WORK_LOG_SEAPPENDIX);
	
	for(INT i=0;i<nRowCnt;i++)
	{
		strUDF = _T("");
		strREMARK = _T("");
		strAPPENDIX_KEY = _T("");
		strBOOK_KEY = _T("");
		strUDF = m_pDM->GetCellData(_T("UDF_상태"),i);

		if(_T("I") == strUDF) //insert
		{
			
			api.MAKE_APPENDIX_BOOK_by_WORK(m_pDM_WORK,
										   m_nCurrentParentIdx,
										   m_nACQ_TYPE,
										   m_pDM,
										   i,
										   m_pDM_BOOK,
										   TRUE);
			
			nIdx_book = m_pDM_BOOK->GetRowCount()-1;
			
			//권KEY , 수입구분별권KEY, 비고 
			m_pDM_BOOK->SetCellData(_T("SB_권KEY"),strVOL_KEY,nIdx_book);
			m_pDM_BOOK->SetCellData(_T("SB_수입구분별권KEY"),strACQ_VOL_KEY,nIdx_book);
			strREMARK = m_pDM->GetCellData(_T("비고"),i);
			strREG_CODE = m_pDM->GetCellData(_T("등록구분"),i);
			strWorking_Status = m_pDM->GetCellData(_T("자료상태"),i);
			strIMM_LOC_TRANSFER_DATE = m_pDM->GetCellData(_T("실시간인계일"),i);
			strREG_TRANFER_DATE = m_pDM->GetCellData(_T("등록인계일"),i);
			m_pDM_BOOK->SetCellData(_T("SB_비고"),strREMARK,nIdx_book);
			m_pDM_BOOK->SetCellData(_T("SB_등록구분_코드"),strREG_CODE,nIdx_book);
			m_pDM_BOOK->SetCellData(_T("SB_자료상태"),strWorking_Status,nIdx_book);
			m_pDM_BOOK->SetCellData(_T("SB_실시간인계일"),strIMM_LOC_TRANSFER_DATE,nIdx_book);
			m_pDM_BOOK->SetCellData(_T("SB_등록인계일"),strREG_TRANFER_DATE,nIdx_book);
	
		
			//부록frame
			RemoveAliasList.RemoveAll();
			RemoveAliasList.Add(_T("UDF_형식구분"));
			RemoveAliasList.Add(_T("UDF_매체구분"));
			RemoveAliasList.Add(_T("UDF_등록구분"));
			RemoveAliasList.Add(_T("UDF_자료상태"));
			RemoveAliasList.Add(_T("등록구분"));
			RemoveAliasList.Add(_T("DB자료상태"));
			RemoveAliasList.Add(_T("실시간인계일"));
			RemoveAliasList.Add(_T("등록인계일"));
			RemoveAliasList.Add(_T("자료상태"));
			RemoveAliasList.Add(_T("비고"));
			m_pDM->InitDBFieldData();
			m_pDM->MakeQueryByJoinedDM(RemoveAliasList, m_pDM, _T("SE_APPENDIX_BOOK_TBL"), i , m_pDM);
			

			//책frame
			RemoveAliasList.RemoveAll();
			m_pDM->InitDBFieldData();
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM_BOOK, _T("SE_BOOK_TBL"), nIdx_book , m_pDM);
	
		}
		else if(_T("U") == strUDF) //update
		{
			
			//부록frame
			RemoveAliasList.RemoveAll();
			RemoveAliasList.Add(_T("UDF_형식구분"));
			RemoveAliasList.Add(_T("UDF_매체구분"));
			RemoveAliasList.Add(_T("UDF_등록구분"));
			RemoveAliasList.Add(_T("UDF_자료상태"));
			RemoveAliasList.Add(_T("등록구분"));
			RemoveAliasList.Add(_T("DB자료상태"));
			RemoveAliasList.Add(_T("실시간인계일"));
			RemoveAliasList.Add(_T("등록인계일"));
			RemoveAliasList.Add(_T("자료상태"));
			RemoveAliasList.Add(_T("비고"));
			RemoveAliasList.Add(_T("부록KEY"));
			strAPPENDIX_KEY = m_pDM->GetCellData(_T("부록KEY"),i);
			strBOOK_KEY = m_pDM->GetCellData(_T("책KEY"),i);
			m_pDM->InitDBFieldData();
			m_pDM->MakeQueryByJoinedDM(RemoveAliasList, m_pDM, _T("SE_APPENDIX_BOOK_TBL"), i , m_pDM, _T("REC_KEY"), _T("NUMERIC"), strAPPENDIX_KEY);
			

			//책(비고,등록구분) frame 
			strREMARK = m_pDM->GetCellData(_T("비고"),i);
			strREG_CODE = m_pDM->GetCellData(_T("등록구분"),i);
			strWorking_Status = m_pDM->GetCellData(_T("자료상태"),i);
			strIMM_LOC_TRANSFER_DATE = m_pDM->GetCellData(_T("실시간인계일"),i);
			strREG_TRANFER_DATE = m_pDM->GetCellData(_T("등록인계일"),i);

			strQuery.Format(_T("UPDATE SE_BOOK_TBL SET REMARK = '%s',REG_CODE = '%s',")
							_T("WORKING_STATUS = '%s',IMM_LOC_TRANSFER_DATE = '%s',")
							_T("REG_TRANSFER_DATE = '%s',LAST_WORK = '%s' WHERE REC_KEY = %s;"),
							strREMARK,strREG_CODE,strWorking_Status,strIMM_LOC_TRANSFER_DATE,
							strREG_TRANFER_DATE,strWorkLogMsg,strBOOK_KEY);
			m_pDM->AddFrame(strQuery);
			
		}

	}
	
	//delete
	for(i=0;i<nSize;i++)
	{
		strAPPENDIX_KEY = m_arrayDeleteAppendixKey.GetAt(i);
		strBOOK_KEY = m_arrayDeleteBookKey.GetAt(i);


		//정리와 동일한 방식으로 삭제
		api.BOOK_CAT_DELETE_APPENDIX(m_pDM,
									 strBOOK_KEY,
									 strSPECIES_KEY,
									 FALSE);
		
	}
	
	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();

	if(ids<0)
	{
		AfxMessageBox(_T("저장에 실패하였습니다!"));
	}
	else
	{
		
		for(i=0;i<nRowCnt;i++)
		{
			//상태값 refresh
			m_pDM->SetCellData(_T("UDF_상태"),_T(""),i);
			m_arrayDeleteAppendixKey.RemoveAll();
			m_arrayDeleteBookKey.RemoveAll();

		}
		
		AfxMessageBox(_T("저장하였습니다!"));


	}

	return;

	CDialog::OnOK();
}

VOID CSeAppendixDlg::OnchkAPPENDIXDISCARDYN() 
{
	// TODO: Add your control notification handler code here
	// 폐기여부 click event : 폐기여부가 선택되면 등록구분을 비활성화 시킨다
	UpdateData(TRUE);

	if(m_bDISCARD_YN) GetDlgItem(IDC_cmbAPPENDIX_REG_CODE)->EnableWindow(FALSE);
	else GetDlgItem(IDC_cmbAPPENDIX_REG_CODE)->EnableWindow(TRUE);

}

VOID CSeAppendixDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	// 처리된 부록수 
	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();
	m_strNewArticleCnt.Format(_T("%d"),nRowCnt);


	CDialog::OnCancel();
}

VOID CSeAppendixDlg::OnchkAPPENDIXREGFLAG() 
{
	// TODO: Add your control notification handler code here
	
	// 단행본 등록 상태가 풀리면 자료상태를 설정할 수 있게 한다
	UpdateData(TRUE);

	if(m_bREG_FLAG) 
	{
		GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS)->EnableWindow(TRUE);
	}
}

HBRUSH CSeAppendixDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
