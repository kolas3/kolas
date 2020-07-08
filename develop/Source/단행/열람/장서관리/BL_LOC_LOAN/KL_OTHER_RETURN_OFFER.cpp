// KL_OTHER_RETURN_OFFER.cpp : implementation file
//

#include "stdafx.h"
#include "KL_OTHER_RETURN_OFFER.h"
#include "FrameManager.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#include "KL_OTHER_RETURN_EDIT_MEMO.h"
#include "KL_OTHER_RETURN_DETAILVIEW.h"
#include "BO_LOC_3170.h"
#include "BO_LOC_3170_ReserveInfo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_OFFER dialog


CKL_OTHER_RETURN_OFFER::CKL_OTHER_RETURN_OFFER(CESL_Mgr* pParent /*=NULL*/)
	: CKL_OTHER_RETURN_DEFAULT(pParent)
{
	//{{AFX_DATA_INIT(CKL_OTHER_RETURN_OFFER)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strSMAlias = _T("SM_KL_OTHER_RETURN_MNG_OFFER");
	m_strCMAlias = _T("CM_KL_OTHER_RETURN_MNG_OFFER");
	m_strDMAlias = _T("DM_KL_OTHER_RETURN_MNG_OFFER");
	m_pParent = pParent;
}

CKL_OTHER_RETURN_OFFER::~CKL_OTHER_RETURN_OFFER()
{
}

void CKL_OTHER_RETURN_OFFER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_RETURN_OFFER)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_RETURN_OFFER, CKL_OTHER_RETURN_DEFAULT)
	//{{AFX_MSG_MAP(CKL_OTHER_RETURN_OFFER)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_cmb, OnSelchangecmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_OFFER message handlers

BOOL CKL_OTHER_RETURN_OFFER::OnInitDialog() 
{
	CKL_OTHER_RETURN_DEFAULT::OnInitDialog();
	
	// TODO: Add extra initialization here
	ResetComboBox();

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)FindControl(m_strCMAlias, _T("멀티콤보_진행상태"));
	if(NULL == pCMB) return FALSE;	
	
	CString strData = _T("");
	int ROW = pCMB->GetCount();
	for(int nCnt = 0; nCnt < ROW; nCnt++)
	{
		strData = pCMB->GetItemDataString(nCnt, 1);
		if(_T("발송") == strData)
		{
			pCMB->SetCurSel(nCnt);
			break;
		}
	} 

	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("타관반납관리복귀");
	}

	if (m_pLoanManageValue->m_sLoanUserAccessMode.IsEmpty() || m_pLoanManageValue->m_sLoanUserAccessMode == _T("1") )
	{
		m_pGrid->SetColumnEncryption(_T("이용자명"),TRUE);
		m_pGrid->SetColumnEncryption(_T("생년월일"),TRUE);
	}
	
	Refresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKL_OTHER_RETURN_OFFER::Create(CWnd* pWnd)
{
	return CDialog::Create(IDD, pWnd);
}

BOOL CKL_OTHER_RETURN_OFFER::ResizeControl()
{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_grid);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return FALSE;
	
	CRect rDlg = CRect(0, 0, 0, 0);
	GetClientRect(rDlg);

	rDlg.top += 70;

	pWnd->MoveWindow(rDlg);

	return TRUE;
}

INT CKL_OTHER_RETURN_OFFER::ResetComboBox()
{
	int ids = -1;

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(m_strCMAlias, _T("멀티콤보_진행상태"));
	if(!pCMB)
	{
		return -1000;
	}

	const int COL_COUNT = 2;
	pCMB->ResetContent();
	pCMB->FreeComboData();
	pCMB->InitMultiComboBox(COL_COUNT);	

	const int STATE_COUNT = 3;
	CString strCmbData[STATE_COUNT][COL_COUNT] = 
	{		
		{ _T("0"), _T("반납")},
		{ _T("1"), _T("발송")},
		{ _T("2"), _T("복귀")}
	};

	m_strDescription[0] = _T("타관에서 자관의 책이 반납되어 발송 대기중인 목록");
	m_strDescription[1] = _T("타관에서 자관의 책이 반납되어 발송한 목록");
	m_strDescription[2] = _T("타관에서 자관의 책이 반납되어 발송된 자료를 복귀처리한 목록");

	for(int nRow = 0; nRow < STATE_COUNT; nRow++)
	{
		for(int nCol = 0; nCol < 2; nCol++)
		{
			pCMB->AddItem(strCmbData[nRow][nCol], nCol, nRow);
		}
	}

	return 0;
}

void CKL_OTHER_RETURN_OFFER::OnSize(UINT nType, int cx, int cy) 
{
	CKL_OTHER_RETURN_DEFAULT::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ResizeControl();
}

INT CKL_OTHER_RETURN_OFFER::Return_Complete()
{
	int ids = 0;
	CString strFuncName = _T("CKL_OTHER_RETURN_REQUEST::Return_Complete()");

	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}

	CString strLoanLibCode, strHoldLibCode, strTransCode;
	int nIdx = m_pGrid->SelectGetHeadPosition();
	for (INT i = 0; i < m_pGrid->SelectGetCount(); i++) {
		m_pDM->GetCellData(_T("트랜젝션상태"), nIdx, strTransCode);
		if(_T("1") != strTransCode) {
			m_pGrid->SetAt(nIdx, m_pGrid->m_nSelectIdx, _T(""));
			continue;
		}
		m_pDM->GetCellData(_T("제공도서관부호"), nIdx, strHoldLibCode);
		if(!strHoldLibCode.IsEmpty()) {
			if(m_pInfo->LIB_CODE != strHoldLibCode) {
				m_pGrid->SetAt(nIdx, m_pGrid->m_nSelectIdx, _T(""));
				continue;
			}
		} else {
			m_pDM->GetCellData(_T("대출도서관부호"), nIdx, strLoanLibCode);
			if(m_pInfo->LIB_CODE != strLoanLibCode && _T("5") != strTransCode) {
				m_pGrid->SetAt(nIdx, m_pGrid->m_nSelectIdx, _T(""));
				continue;
			}
		}
		nIdx = m_pGrid->SelectGetNext();
	}

	}
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 복귀 가능한 항목이 없습니다.\r\n")
			_T("복귀가 불가능한 대상은 아래와 같습니다.\r\n\r\n")			
			_T("1. 진행상태가 발송(1)이 아닌 자료\r\n")
			);
		return -1;
	}

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);
	
	CFrameWriter Writer(m_pInfo);
	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
	CString strData;
	CString strQuery;

	CStringArray asAppendixRegNoList;
	CString strAppendixSendYN, strAppendixRegNoList;

	m_pGrid->SelectMakeList();
	if(0 < m_pGrid->SelectGetCount())
	{	
		if(1 < m_pGrid->SelectGetCount())
		{
			Writer.setReiteration(TRUE);
		}
		CString strReturnDate, strLoanLibCode, strLoanKey, strData;
		CTime Today = CLocCommonAPI::GetCurrentDateTime(this, strReturnDate);

		INT nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{
			CString strHoldLibCode, strOtherLoanKey;
			m_pDM->GetCellData(_T("제공도서관부호"), nIdx, strHoldLibCode);
				
			Writer.clear();
			Writer.setCommand(_T("IL_K19_LOC_B01_SERVICE"));
			Writer.addRecord();
			Writer.addElement(_T("트랜젝션상태"), _T("2"));
			strLoanLibCode = m_pDM->GetCellData(_T("대출도서관부호"), nIdx);
			Writer.addElement(_T("대출도서관"), strLoanLibCode);
			Writer.addElement(_T("제공도서관부호"), strHoldLibCode);
			strLoanKey = m_pDM->GetCellData(_T("로컬KEY"), nIdx);
			Writer.addElement(_T("로컬KEY"), strLoanKey);
			strOtherLoanKey = m_pDM->GetCellData(_T("타관대출KEY"), nIdx);
			Writer.addElement(_T("타관대출KEY"), strOtherLoanKey);
			strData = m_pDM->GetCellData(_T("타관반납발송일"), nIdx);
			Writer.addElement(_T("타관반납발송일"), strData);
			Writer.addElement(_T("복귀일"), strReturnDate);
			strData = m_pDM->GetCellData(_T("발송자"), nIdx);
			Writer.addElement(_T("발송자"), strData);
			Writer.addElement(_T("입수자"), m_pInfo->USER_ID);
			Writer.addElement(_T("마지막작업"), GetWorkLogMsg(_T("복귀"),__WFILE__ ,__LINE__));

			BOOL bSuccese = FALSE;
			Reader.clear();
			ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if(0 > ids)
			{
				 bSuccese = FALSE;
			}
			else
			{
 				CESL_DataMgr TempDM;
 				Reader.makeDM(&TempDM);
 				CString strTemp = Reader.getCommand();
 				if(_T("ERROR") == strTemp || 1 > TempDM.GetRowCount())
				{
					bSuccese = FALSE;
				}
				else
				{
					TempDM.GetCellData(_T("처리결과"), 0, strData);
					if(_T("OK") == strData)
					{
						TmpDM.StartFrame();

						if(strLoanKey.IsEmpty())
						{
							CString strBookKey;
							m_pDM->GetCellData(_T("책KEY"), nIdx, strBookKey);
							strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS='BOL112N',PREV_WORKING_STATUS='BOL611O' WHERE REC_KEY=%s;"), strBookKey);
						}
						else
						{						
							strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS='BOL112N',PREV_WORKING_STATUS='BOL511O' WHERE REC_KEY=(SELECT BOOK_KEY FROM LS_WORK_T01 WHERE REC_KEY=%s AND PUBLISH_FORM_CODE='MO' UNION SELECT BOOK_KEY FROM CO_LOAN_TBL WHERE REC_KEY=%s AND PUBLISH_FORM_CODE='MO');"), strLoanKey, strLoanKey);
							TmpDM.AddFrame(strQuery);
							strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKING_STATUS='SEL212N',PREV_WORKING_STATUS='SEL511O' WHERE REC_KEY=(SELECT BOOK_KEY FROM LS_WORK_T01 WHERE REC_KEY=%s AND PUBLISH_FORM_CODE='SE' UNION SELECT BOOK_KEY FROM CO_LOAN_TBL WHERE REC_KEY=%s AND PUBLISH_FORM_CODE='SE');"), strLoanKey, strLoanKey);
						}
						TmpDM.AddFrame(strQuery);

						strAppendixSendYN = m_pDM->GetCellData(_T("부록발송유무"), nIdx);
						if (_T("Y") == strAppendixSendYN) {
							strAppendixRegNoList = m_pDM->GetCellData(_T("부록등록번호리스트"), nIdx);
							CLocCommonAPI::MakeArrayList(strAppendixRegNoList, _T(","), asAppendixRegNoList);
							for (INT i = 0; i < asAppendixRegNoList.GetSize(); i++) {
								strQuery.Format(
									_T("UPDATE BO_BOOK_TBL ")
									   _T("SET WORKING_STATUS='BOL112N',")
									       _T("PREV_WORKING_STATUS='BOL511O' ")
									 _T("WHERE REG_NO='%s';"), asAppendixRegNoList.GetAt(i));
								TmpDM.AddFrame(strQuery);
							}
						}

						ids = TmpDM.SendFrame();
						TmpDM.EndFrame();

						if(0 > ids)
						{
							bSuccese = FALSE;
						}
						else
						{
							bSuccese = TRUE;
						}
					}
					else
					{
						bSuccese = FALSE;
					}
				}
			}
			if(TRUE == bSuccese)
			{
				RecordReserveExpireDate(nIdx);
				m_pDM->SetCellData(_T("처리결과"), _T("성공"), nIdx);
			}
			else
			{
				m_pDM->SetCellData(_T("처리결과"), _T("실패"), nIdx);
			}
			nIdx = m_pGrid->SelectGetNext();
		}
		while(0 <= nIdx);
	}	

	if(0 < m_pGrid->SelectGetCount())
	{
		INT nIdx = m_pGrid->SelectGetTailPosition();
		do
		{
			m_pDM->GetCellData(_T("처리결과"), nIdx, strData);
			if(_T("성공") == strData)
			{
				m_pDM->DeleteRow(nIdx);
			}
			nIdx = m_pGrid->SelectGetPrev();
		}
		while(0 <= nIdx);
	}
	m_pGrid->Display();

	return ids;
}

VOID CKL_OTHER_RETURN_OFFER::RecordReserveExpireDate(int nIdx){	
	
	CString strRegNo;
	m_pDM->GetCellData(_T("등록번호"), nIdx, strRegNo);
	if (strRegNo.IsEmpty()) return;

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pParent->m_pInfo->CONNECTION_INFO);
	
	CString strQuery = _T("");
	CString strPublishFormCode = _T("");
	CString strBookKey = _T("");
	int ids = 0;
	if (m_pLoanManageValue->m_bVolReserve)//한줄서기 사용 
	{
		strQuery.Format(
			_T("SELECT MAX(SPECIES_KEY) FROM (")
			_T("SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REG_NO = '%s' ")
			_T("UNION ")
			_T("SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE REG_NO = '%s')")
			,strRegNo,strRegNo
			);
		TmpDM.GetOneValueQuery( strQuery, strBookKey);
		
		if( strBookKey.IsEmpty() ) return;
		
		strQuery.Format(
			_T("SELECT PUBLISH_FORM_CODE FROM LS_WORK_T01 ")
			_T("WHERE SPECIES_KEY = %s ")
			_T("AND STATUS = '3' AND ROWNUM < 2 ")
			,strBookKey
			);

		TmpDM.GetOneValueQuery( strQuery, strPublishFormCode);

		if( strPublishFormCode.IsEmpty() ) return;
		
		CString strTable = _T("");
		if (strPublishFormCode == _T("SE"))
		{
			strTable = _T("SE_BOOK_TBL");
		}else {
			strTable = _T("BO_BOOK_TBL");
		}

		strQuery.Format(
			_T("SELECT CL.BOOK_KEY,CL.SPECIES_KEY,CL.PUBLISH_FORM_CODE,CL.VOL_INFO,CL.TITLE FROM LS_WORK_T01 CL, %s BB ")
			_T("WHERE CL.SPECIES_KEY = BB.SPECIES_KEY ")
			_T("AND CL.STATUS ='3' ")
			_T("AND BB.SPECIES_KEY = %s ")
			_T("AND ( (CL.VOL_INFO = BB.VOL_SORT_NO) OR (CL.VOL_INFO IS NULL AND BB.VOL_SORT_NO IS NULL) ) ")
			_T("AND ROWNUM < 2 ")
			,strTable, strBookKey
			);
	}
	else
	{
		strQuery.Format(
			_T("SELECT MAX(REC_KEY) FROM (")
			_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE REG_NO = '%s' ")
			_T("UNION ")
			_T("SELECT REC_KEY FROM SE_BOOK_TBL WHERE REG_NO = '%s')")
			,strRegNo,strRegNo
			);
		TmpDM.GetOneValueQuery( strQuery, strBookKey);
		
		if( strBookKey.IsEmpty() ) return;
		
		strQuery.Format(
			_T("SELECT PUBLISH_FORM_CODE FROM LS_WORK_T01 ")
			_T("WHERE BOOK_KEY = %s ")
			_T("AND STATUS = '3' ")
			_T("AND ROWNUM < 2 ")
			,strBookKey
			);
		
		TmpDM.GetOneValueQuery( strQuery, strPublishFormCode);
		
		if( strPublishFormCode.IsEmpty() ) return;
		
		CString strTable = _T("");
		if (strPublishFormCode == _T("SE"))
		{
			strTable = _T("SE_BOOK_TBL");
		}else {
			strTable = _T("BO_BOOK_TBL");
		}

		strQuery.Format(
			_T("SELECT CL.BOOK_KEY,CL.SPECIES_KEY,CL.PUBLISH_FORM_CODE,CL.VOL_INFO,CL.REG_NO FROM LS_WORK_T01 CL , %s BB ")
			_T("WHERE CL.BOOK_KEY=BB.REC_KEY ")
			_T("AND CL.STATUS = '3' ")
			_T("AND BB.REC_KEY = %s ")
			_T("AND ROWNUM < 2 ")
			,strTable, strBookKey
			);
	}

	ids = TmpDM.RestructDataManager(strQuery);
	if (ids < 0 || TmpDM.GetRowCount() == 0) return;
	
	CString TmpData[5];
	for (int i =0 ; i < 5 ; i++)
	{
		TmpDM.GetCellData(0,i,TmpData[i]);
	}
	CString strReserveMsg = _T("");
	strReserveMsg.Format(_T("[%s]는 예약된 자료입니다. 예약 처리를 하시겠습니까?"),TmpData[4]);

	if( MessageBox(strReserveMsg,_T("예약처리"),MB_YESNO | MB_ICONQUESTION) == IDNO ) return;

	if (m_pLoanManageValue->m_bVolReserve)
	{
		CBO_LOC_3170_ReserveInfo Dlg( m_pParent ,m_pLoanShareManager, m_pLoanManageValue, 0);
		Dlg.pMain = m_pParent ->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)m_pParent;
		Dlg.m_pInfo = m_pParent ->m_pInfo;
		Dlg.SetInfo(TmpData[1],TmpData[3],TmpData[2],TmpData[0],1);
		Dlg.DoModal();
	}
	else 
	{
		CBO_LOC_3170 Dlg(m_pParent , m_pLoanShareManager ,m_pLoanManageValue , 0);
		Dlg.pMain = m_pParent ->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)m_pParent;
		Dlg.m_pInfo = m_pParent ->m_pInfo;
		Dlg.SetInfo(TmpData[1],TmpData[3],TmpData[2],TmpData[0],1);
		Dlg.DoModal();
	}
	
}

INT CKL_OTHER_RETURN_OFFER::ReturnCancel()
{
	int ids = 0;
	CString strFuncName = _T("ReturnCancel");
	CString strState = GetComboBoxState();
	if(_T("2") != strState)
	{
		AfxMessageBox(_T("복귀자료관리에서는 복귀 상태만 취소 작업이 가능합니다."));
		return -1;
	}

	m_pGrid->SelectMakeList();
	if(1 > m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}

	if(1 < m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("작업취소는 한건씩 처리가능합니다.\r\n취소할 자료 하나만 선정해주십시오."));
		return -1;
	}
	INT nIdx = m_pGrid->SelectGetHeadPosition();
	CString strComeBackDate = m_pDM->GetCellData(_T("복귀일"), nIdx);

	CString strOtherLoanKey, strLoanKey, strCurrentDate;
	strOtherLoanKey = m_pDM->GetCellData(_T("타관대출KEY"), nIdx);
	strLoanKey = m_pDM->GetCellData(_T("로컬KEY"), nIdx);

	CString strBookLibCode = _T(""), strWorkingstatus = _T(""), strPrevWorkingStatus = _T(""), strAppendixList = _T("");
	CStringArray asAppendixRegNoList;
	strBookLibCode = m_pDM->GetCellData(_T("제공도서관부호"), nIdx);
	strAppendixList = m_pDM->GetCellData(_T("부록등록번호리스트"), nIdx);

	if (strComeBackDate.IsEmpty()) {
		AfxMessageBox(_T("복귀일이 없습니다."));
		return -1;
	}
	if (19 != strComeBackDate.GetLength()) {
		AfxMessageBox(_T("복귀일에 오류가 있습니다."));
		return -1;
	}
	CTime t = CLocCommonAPI::GetCurrentDateTime(this, strCurrentDate);
	if (strComeBackDate.Mid(0,10) != strCurrentDate.Mid(0,10)) {
		AfxMessageBox(_T("당일 처리한 자료만 취소 작업이 가능합니다."));
		return -1;
	}	
	if (strLoanKey.IsEmpty()) {
		AfxMessageBox(_T("미대출로 복귀완료된 자료는 취소되지 않습니다."));
		return -1;
	}

	CString strQuery;
	strQuery.Format(_T("SELECT BOOK_KEY,'LS_WORK_T01' FROM LS_WORK_T01 WHERE REC_KEY=%s UNION ALL SELECT BOOK_KEY,'CO_LOAN_TBL' FROM CO_LOAN_TBL WHERE REC_KEY=%s"), strLoanKey, strLoanKey);
	CESL_DataMgr tempDM;
	tempDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	tempDM.RestructDataManager(strQuery);
	if (1 > tempDM.GetRowCount())
	{
		AfxMessageBox(_T("반납 이력 조회에 실패하였습니다."));
		return -1;
	}
	CString strBookKey = tempDM.GetCellData(0,0);
	if(strBookKey.IsEmpty())
	{
		AfxMessageBox(_T("북키가 없습니다."));
		return -1;
	}
	CString strLoanTable = tempDM.GetCellData(0,1);
	if(strLoanTable.IsEmpty()) strLoanTable = _T("LS_WORK_T01");
	strQuery.Format(_T("SELECT (SELECT COUNT(1) FROM %s WHERE BOOK_KEY=%s AND STATUS IN ('0','2','L','D') AND TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS')<LOAN_DATE) A,(SELECT COUNT(1) FROM %s WHERE BOOK_KEY=%s AND STATUS='3' AND TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS')<RESERVATION_DATE) B FROM DUAL"), strLoanTable, strBookKey, strComeBackDate, strLoanTable, strBookKey, strComeBackDate);
	tempDM.RestructDataManager(strQuery);
	if(0 < tempDM.GetRowCount())
	{
		CString strGetLoanCnt = tempDM.GetCellData(0,0);
		if(strGetLoanCnt.IsEmpty()) strGetLoanCnt = _T("0");
		if(0 < _ttoi(strGetLoanCnt))
		{
			AfxMessageBox(_T("복귀처리 이후에 대출된 현황이 존재하므로 취소 작업을 중지합니다."));
			return -1;
		}
		CString strGetReserveCnt = tempDM.GetCellData(0,1);
		if(strGetReserveCnt.IsEmpty()) strGetReserveCnt = _T("0");
		if(0 < _ttoi(strGetReserveCnt))
		{
			AfxMessageBox(_T("복귀처리 이후에 예약된 현황이 존재하므로 취소 작업을 중지합니다."));
			return -1;
		}
	}

	CFrameWriter Writer(m_pInfo);
	CString strReturnLibCode, strLoanLibCode, strLastWork;
	strLastWork = GetWorkLogMsg(_T("타관반납관리취소"),__WFILE__ ,__LINE__);
	Writer.setCommand(_T("IL_K41_LOC_B01_SERVICE"));
	Writer.addRecord();
	CString strHoldLibCode;
	m_pDM->GetCellData(_T("제공도서관부호"), nIdx, strHoldLibCode);
	strLoanLibCode = m_pDM->GetCellData(_T("대출도서관부호"), nIdx);
	Writer.addElement(_T("대출도서관부호"), strLoanLibCode);
	Writer.addElement(_T("제공도서관부호"), strHoldLibCode);
	strReturnLibCode = m_pDM->GetCellData(_T("반납도서관부호"), nIdx);
	Writer.addElement(_T("반납도서관부호"), strReturnLibCode);
	Writer.addElement(_T("로컬KEY"), strLoanKey);
	Writer.addElement(_T("트렌젝션"), strState);
	Writer.addElement(_T("마지막작업"), strLastWork);
	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
	CString strData;

	BOOL bSuccese = FALSE;
	Reader.clear();
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		bSuccese = FALSE;
	}
	else
	{
		CESL_DataMgr TempDM;
		Reader.makeDM(&TempDM);
		CString strTemp = Reader.getCommand();
		if(_T("ERROR") == strTemp || 1 > TempDM.GetRowCount())
		{
			bSuccese = FALSE;
		}
		else
		{
			TempDM.GetCellData(_T("처리결과"), 0, strData);
			if(_T("OK") == strData)
			{
				bSuccese = TRUE;
			}
			else
			{
				bSuccese = FALSE;
			}
		}
	}
	if(TRUE == bSuccese)
	{
		DecideBookStatus(strBookLibCode, strLoanKey, strWorkingstatus, strPrevWorkingStatus);
		if ( strWorkingstatus.GetLength() == 0 || strPrevWorkingStatus.GetLength() == 0 ) return -1;

		tempDM.StartFrame();
		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS='%s', PREV_WORKING_STATUS='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"), strWorkingstatus, strPrevWorkingStatus, strLastWork, strBookKey);
		tempDM.AddFrame(strQuery);

		if ( strAppendixList.GetLength() > 0 ) {
			CLocCommonAPI::MakeArrayList(strAppendixList, _T(","), asAppendixRegNoList);
			for (INT i = 0; i < asAppendixRegNoList.GetSize(); i++) {
				strQuery.Format(
					_T("UPDATE BO_BOOK_TBL ")
					_T("SET WORKING_STATUS='%s',")
					_T("PREV_WORKING_STATUS='%s' ")
					_T("WHERE REG_NO='%s';"), strWorkingstatus, strPrevWorkingStatus, asAppendixRegNoList.GetAt(i));
				tempDM.AddFrame(strQuery);
			}			
		}		
		tempDM.SendFrame();
		tempDM.EndFrame();

		m_pDM->SetCellData(_T("처리결과"), _T("성공"), nIdx);
		Refresh();
		AfxMessageBox(_T("복귀 작업이 취소 되었습니다. (발송 상태로 변경)"));
	}
	else
	{
		m_pDM->SetCellData(_T("처리결과"), _T("실패"), nIdx);
		m_pGrid->Display();
	}
	
	
	return ids;
}

void CKL_OTHER_RETURN_OFFER::DecideBookStatus(CString strBookLibCode, CString strLocalKey, CString& strWorkingstatus, CString& strPrevWorkingStatus)
{
	if ( strBookLibCode.GetLength() == 0 ) {
		strWorkingstatus = _T("BOL511O");
		strPrevWorkingStatus = _T("BOL211O");
	}
	else {
		if ( strLocalKey.GetLength() == 0 ) {
			strWorkingstatus = _T("BOL611O");
			strPrevWorkingStatus = _T("BOL112N");
		}
		else {
			strWorkingstatus = _T("BOL511O");
			strPrevWorkingStatus = _T("BOL611O");
		}
	}
}

void CKL_OTHER_RETURN_OFFER::OnSelchangecmb() 
{
	// TODO: Add your control notification handler code here
	Refresh();
}

INT CKL_OTHER_RETURN_OFFER::Refresh()
{
	CString strState = GetComboBoxState();

	int nState = _wtoi(strState);
	if(nState < 0)
	{
		nState = 0;		
	}
	SetDlgItemText(IDC_stext_state_description,m_strDescription[nState]);
	BOOL bShow = FALSE;
	if(_T("2") == strState) bShow = TRUE;
	m_pParent->SendMessage(WM_KL_RETURN_CANCEL_CTL, NULL, (LPARAM)bShow);
	m_pParent->SendMessage(WM_KL_ACTIVE_BUTTON, NULL, (LPARAM)nState+10);

	SetDateControlView();

	if(m_bIsSearched)
	{
		return 1000;
	}

	m_bIsSearched = TRUE;

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K18_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("트랜젝션상태"), strState);
 	Writer.addElement(_T("대출도서관"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("제공도서관"), m_pInfo->LIB_CODE);
	if(_T("0") == strState)
	{
		Writer.addElement(_T("ORDER_BY"), _T("RETURN_DATE DESC"));
	}
	else if(_T("1") == strState)
	{
		Writer.addElement(_T("ORDER_BY"), _T("TR_SEND_DATE DESC"));
	}
	else if(_T("2") == strState)
	{
		Writer.addElement(_T("복귀일부터"), GetBeforeDate());
		Writer.addElement(_T("복귀일까지"), GetAfterDate());
		Writer.addElement(_T("ORDER_BY"), _T("TR_RETURN_DATE DESC"));
	}
	CString szData = Writer.getFrame();

	CFrameSender Sender(m_pInfo);	
	Sender.CommMiddleWareDaemon(szData, szData);

	m_pDM->FreeData();
	
	CFrameReader Reader;
	Reader.parseFrame(szData);
	Reader.makeDM(m_pDM);
	SetDescDM();

	CString strAlias[2] = {_T("대출도서관"), _T("결제처리KEY")};
	INT nCond[2] = {ESL_DATAMGR_SORT_DESC, ESL_DATAMGR_SORT_DESC};
	m_pDM->SORT(2, strAlias, nCond, ESL_DATAMGR_SORT_DESC);

	CString strUserNo = _T("");
	for(int i=0; i<m_pDM->GetRowCount(); i++)
	{
		strUserNo = m_pDM->GetCellData(_T("이용자번호"), i);
		if(strUserNo.Compare(_T(""))!=0)
		{
			m_pDM->SetCellData(_T("이용자명"), GetUserName(strUserNo), i);
			m_pDM->SetCellData(_T("생년월일"), GetBirthDay(strUserNo), i);
			m_pDM->SetCellData(_T("대출자직급설명"), GetUserClassDesc(strUserNo), i);
		}		
	}

	SetTimer(1, 300, NULL);

	return 0;
}


INT CKL_OTHER_RETURN_OFFER::DoRefresh()
{
	if(m_bIsSearched)
	{
		return 1000;
	}
	EnableWindow(FALSE);

	AfxBeginThread(OtherReturnOfferThread, this);

	return 0;
}

UINT CKL_OTHER_RETURN_OFFER::OtherReturnOfferThread(LPVOID lParam)
{
	CKL_OTHER_RETURN_OFFER* pDlg = (CKL_OTHER_RETURN_OFFER*)lParam;
	pDlg->Refresh();

	return 0;
}

CString CKL_OTHER_RETURN_OFFER::GetUserName(CString strUserNo)
{
	int ids = 0;
	CString strUserName = _T(""), strQuery = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	strQuery.Format(_T("SELECT name FROM co_loan_user_tbl WHERE user_no = '%s'"), strUserNo);
	ids = TmpDM.GetOneValueQuery(strQuery,strUserName);
	if(ids < 0)
	{
		strUserName = _T("");
	}
	return strUserName;
}

CString CKL_OTHER_RETURN_OFFER::GetBirthDay(CString strUserNo)
{
	int ids = 0;
	CString strBirthDay = _T(""), strQuery = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	strQuery.Format(_T("SELECT BIRTHDAY FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s'"), strUserNo);
	ids = TmpDM.GetOneValueQuery(strQuery,strBirthDay);
	if(ids < 0)
	{
		strBirthDay = _T("");
	}
	return strBirthDay;
}

CString CKL_OTHER_RETURN_OFFER::GetUserClassDesc(CString strUserNo)
{
	int ids = 0;
	CString strUserClassDesc = _T(""), strQuery = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	strQuery.Format(_T("SELECT ECO$F_GET_CODE2DESC('31', USER_CLASS_CODE) FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s'"), strUserNo);
	ids = TmpDM.GetOneValueQuery(strQuery,strUserClassDesc);
	if(ids < 0)
	{
		strUserClassDesc = _T("");
	}
	return strUserClassDesc;
}

void CKL_OTHER_RETURN_OFFER::ModifyMemo()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return;
	}

	CKL_OTHER_RETURN_EDIT_MEMO dlgEditMemo(m_pParent);
	dlgEditMemo.SetGrid(m_pGrid);
	dlgEditMemo.SetDM(m_pDM);

	dlgEditMemo.DoModal();

	if(dlgEditMemo.m_BModify)
		Refresh();
}

void CKL_OTHER_RETURN_OFFER::ViewLoanUser()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return;
	}

	CKL_OTHER_RETURN_DETAILVIEW dlgViewLoanUser(m_pParent);
	dlgViewLoanUser.SetGrid(m_pGrid);
	dlgViewLoanUser.SetDM(m_pDM);

	dlgViewLoanUser.DoModal();	
}