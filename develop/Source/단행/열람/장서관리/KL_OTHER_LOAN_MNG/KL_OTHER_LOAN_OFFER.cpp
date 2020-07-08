// KL_OTHER_LOAN_OFFER.cpp : implementation file
//

#include "stdafx.h"
#include "KL_OTHER_LOAN_OFFER.h"
#include "FrameManager.h"

// 16/02/03 이충무 : 2016년 국립중앙도서관 책이음개선
// MainHeader 추가
/*//NEW------------------------------------------------------------------------*/
#include "KL_OTHER_LOAN_MAIN.h"
#include "..\\..\\..\\..\\공통\\BO_ACQ_PRINT_MGR\\BasicPrintMgr.h"
#include <map>
#include "KL_OTHER_LOAN_EDIT_MEMO.h"
#include "KL_OTHER_LOAN_USER_DETAILVIEW.h"

#include "..\\..\\..\\..\\SMS\\CO_SMS\\SMS_MakeMsg.h"
#include "..\\..\\..\\..\\SMS\\CO_SMS\\SMS_Env_Info.h"
/*//END------------------------------------------------------------------------*/

// 16/02/25 박우람 : 2016년 국립중앙도서관 책이음개선
// 부록 발송
/*//NEW------------------------------------------------------------------------*/
#include "KL_OTHER_APPENDIX_LIST.h"
/*//END------------------------------------------------------------------------*/

// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CString g_strKakaoUsed1;
// KOL.RED.2018.224 ---------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_OFFER dialog


CKL_OTHER_LOAN_OFFER::CKL_OTHER_LOAN_OFFER(CESL_Mgr* pParent /*=NULL*/)
: CKL_OTHER_LOAN_DEFAULT(pParent)
{
	m_strSMAlias = _T("SM_KL_OTHER_LOAN_MNG_OFFER");
	m_strCMAlias = _T("CM_KL_OTHER_LOAN_MNG_OFFER");
	m_strDMAlias = _T("DM_KL_OTHER_LOAN_MNG_OFFER");
	m_pParent = pParent;
	//{{AFX_DATA_INIT(CKL_OTHER_LOAN_OFFER)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}
CKL_OTHER_LOAN_OFFER::~CKL_OTHER_LOAN_OFFER()
{
}

void CKL_OTHER_LOAN_OFFER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_LOAN_OFFER)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_LOAN_OFFER, CKL_OTHER_LOAN_DEFAULT)
//{{AFX_MSG_MAP(CKL_OTHER_LOAN_OFFER)
ON_WM_SIZE()
ON_CBN_SELCHANGE(IDC_cmb, OnSelchangecmb)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_OFFER message handlers

BOOL CKL_OTHER_LOAN_OFFER::Create(CWnd* pParent) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParent);
}

BOOL CKL_OTHER_LOAN_OFFER::OnInitDialog() 
{
	CKL_OTHER_LOAN_DEFAULT::OnInitDialog();
	
	ResetComboBox();
	
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)FindControl(m_strCMAlias, _T("멀티콤보_진행상태"));
	if(NULL == pCMB) return FALSE;	
	
	CString strData = _T("");
	int ROW = pCMB->GetCount();
	for(int nCnt = 0; nCnt < ROW; nCnt++)
	{
		strData = pCMB->GetItemDataString(nCnt, 1);
		if(_T("신청") == strData)
		{
			pCMB->SetCurSel(nCnt);
			break;
		}
	} 

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : 로그 기록시 사유 추가
	//-------------------------------------------------------------------------
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("타관대출관리제공");
	}
	//-------------------------------------------------------------------------

	if (m_pLoanManageValue->m_sLoanUserAccessMode.IsEmpty() || m_pLoanManageValue->m_sLoanUserAccessMode == _T("1") )
	{
		m_pGrid->SetColumnEncryption(_T("이용자명"),TRUE);
		m_pGrid->SetColumnEncryption(_T("생년월일"),TRUE);
	}
	
	Refresh();
	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKL_OTHER_LOAN_OFFER::ResizeControl()
{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_grid);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return FALSE;
	
	CRect rDlg = CRect(0, 0, 0, 0);
	GetClientRect(rDlg);
	
	// 16/02/01 이충무 : 2016년 국립중앙도서관 책이음개선
	// 진행상태에 대한 설명 추가로 인한 위치 이동
	/*//BEFORE-----------------------------------------------------------------------
	rDlg.top += 40;
	*///AFTER------------------------------------------------------------------------
	rDlg.top += 70;
	/*//END------------------------------------------------------------------------*/
	pWnd->MoveWindow(rDlg);
	
	return TRUE;
}



// 콤보박스 초기화
INT CKL_OTHER_LOAN_OFFER::ResetComboBox()
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
	
	// 16/01/29 이충무 : 2016년 국립중앙도서관 책이음개선
	// 대출상태추가
	/*//BEFORE-----------------------------------------------------------------------
	const int STATE_COUNT = 4;
	CString strCmbData[STATE_COUNT][COL_COUNT] = 
	{		
		{ _T("0"), _T("신청")},
		{ _T("1"), _T("발송")},
		{ _T("2"), _T("발송거절")},
		{ _T("3"), _T("입수")},
	};
	*///AFTER------------------------------------------------------------------------
	const int STATE_COUNT = 5;
	CString strCmbData[STATE_COUNT][COL_COUNT] = 
	{		
		{ _T("0"), _T("신청")},
		{ _T("1"), _T("발송")},
		{ _T("2"), _T("발송거절")},
		{ _T("3"), _T("입수")},
		{ _T("4"), _T("타관대출")}
	};
	/*//END------------------------------------------------------------------------*/

	// 16/02/02 이충무 : 2016년 국립중앙도서관 책이음개선
	// 설명 추가
	/*//NEW------------------------------------------------------------------------*/
	m_strDescription[0] = _T("자관 책을 타관에서 대출하기 위해 이용자가 신청한 목록");
	m_strDescription[1] = _T("자관 책을 대출을 위해 대출할 도서관으로 발송한 목록");
	m_strDescription[2] = _T("신청 자료가 어떠한 사유에 의해 발송거절된 목록");
	m_strDescription[3] = _T("자관에서 발송한 책이 대출할 도서관에 입수된 목록");
	m_strDescription[4] = _T("자관 책이 타관에서 대출중인 목록");
	/*//END------------------------------------------------------------------------*/

	
	for(int nRow = 0; nRow < STATE_COUNT; nRow++)
	{
		for(int nCol = 0; nCol < 2; nCol++)
		{
			pCMB->AddItem(strCmbData[nRow][nCol], nCol, nRow);
		}
	}
	
	return 0;
}


void CKL_OTHER_LOAN_OFFER::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	ResizeControl();
	// TODO: Add your message handler code here
	
}
void CKL_OTHER_LOAN_OFFER::OnSelchangecmb() 
{
	// TODO: Add your control notification handler code here
	Refresh();
}

INT CKL_OTHER_LOAN_OFFER::Refresh()
{
	// 16/02/01 이충무 : 2016년 국립중앙도서관 책이음개선
	// 이용자명 표시
	/*//NEW------------------------------------------------------------------------*/
	CString strUserNo = _T("");
	/*//END------------------------------------------------------------------------*/

	CString strState = GetComboBoxState();

	// 16/02/02 이충무 : 2016년 국립중앙도서관 책이음개선
	// 설명 추가
	/*//NEW------------------------------------------------------------------------*/
	int nState = _wtoi(strState);
	if(nState < 0)
	{
		nState = 0;		
	}
	SetDlgItemText(IDC_stext_state_description,m_strDescription[nState]);	
	/*//END------------------------------------------------------------------------*/
	
	BOOL bShow = FALSE;
	if(_T("0") == strState) bShow = TRUE;
	m_pParent->SendMessage(WM_KL_RETURN_CANCEL_CTL, NULL, (LPARAM)bShow);

	// 16/02/03 이충무 : 2016년 국립중앙도서관 책이음개선
	// 상태에 따른 버튼 활성화
	/*//NEW------------------------------------------------------------------------*/
	m_pParent->SendMessage(WM_KL_ACTIVE_BUTTON, NULL, (LPARAM)nState);
	/*//END------------------------------------------------------------------------*/
	
	SetDateControlView();
	CString strDate1,strDate2;

	strDate1 = GetBeforeDate();
	strDate2 = GetAfterDate();
	
	if(m_bIsSearched)
	{
		return 1000;
	}
	
	m_bIsSearched = TRUE;
	
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("TRANSACTION_WORK"), _T("SEARCH"));
	Writer.addElement(_T("SEARCH_OPTION"), _T("OFFER"));
	Writer.addElement(_T("TRANSACTION_CODE"), strState);
	Writer.addElement(_T("LIB_CODE"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("PAGE_NO"), _T("1"));
	Writer.addElement(_T("DISPLAY"), _T("1000"));

	// 15/02/18 이충무 : 2016년 국립중앙도서관 책이음개선
	// 입수시로 변경
	/*//BEFORE-----------------------------------------------------------------------
	if(_T("2") == strState)
	*///AFTER------------------------------------------------------------------------
	//if(_T("3") == strState)
	/*//END------------------------------------------------------------------------*/	
	//2018-11-14 EDIT BY KYJ
	//JOB.2018.0082 신청상태도 일자로 조건 추가 할 수 있도록
	//=======================================================
	if(_T("0") == strState || _T("3") == strState)
	//=======================================================
	{
		Writer.addElement(_T("START_DATE"), strDate1);
		Writer.addElement(_T("END_DATE"), strDate2);
	}
	
	CFrameSender Sender(m_pInfo);	
	CFrameReader Reader; 
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if (0 > ids) 
	{
		while(true) 
		{
			// 2015년 공공도서관 시스템 개선 2015/12/11 : 통합도서서비스 > 책이음서비스
			if(IDYES == MessageBox(_T("책이음서비스 미들웨어와의 통신에 실패하였습니다.\r\n재시도 하시겠습니까?"), _T("조회실패"), MB_YESNO | MB_ICONQUESTION)) 
			{
				ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
				if(0 <= ids) break;
			}
			else
			{
				return 0;
			}
		}
	}
	
	m_pDM->FreeData();
	
	Reader.parseFrame(Reader.getFrame());
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	for(int i = 1; i < Reader.getRecordCount(); i++)
	{
		m_pDM->InsertRow(-1);
		for (int j = 1; j < m_pDM->GetColCount(); j++)
		{
			m_pDM->GetColumnRefInfo(j, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			// 17/11/06 김혜영 : 상호대차 타관대출관리에서의 컬럼명 한글로 교체
/*//BEFORE-----------------------------------------------------------------------------
			m_pDM->SetCellData(FIELD_ALIAS, Reader.getElement(i, FIELD_ALIAS), i-1);
*///AFTER------------------------------------------------------------------------------
			m_pDM->SetCellData(FIELD_ALIAS, Reader.getElement(i, FIELD_NAME), i-1);
/*//END-------------------------------------------------------------------------------*/			
		}

		// 16/02/01 이충무 : 2016년 국립중앙도서관 책이음개선
		// 이용자명 표시
		/*//NEW------------------------------------------------------------------------*/		
		strUserNo = _T("");
		strUserNo = m_pDM->GetCellData(_T("이용자번호"), i-1);
		if(strUserNo.Compare(_T(""))!=0)
		{
			m_pDM->SetCellData(_T("이용자명"), GetUserName(strUserNo), i-1);
			// 17/09/25 김혜영 :  상호대차 타관대출관리에서의 이용자 생년월일 필드 추가 
//*/ ADD ---------------------------------------------------------------------------
			m_pDM->SetCellData(_T("생년월일"), GetBirthDay(strUserNo), i-1);
//*/ END ---------------------------------------------------------------------------
			// 17/11/06 김혜영 : 상호대차 타관대출관리에서의 이용자 직급 필드 추가 
//*/ ADD ---------------------------------------------------------------------------
			m_pDM->SetCellData(_T("이용자직급설명"), GetUserClassDesc(strUserNo), i-1);
//*/ END ---------------------------------------------------------------------------
		}
		
		/*//END------------------------------------------------------------------------*/
	}
	
	
	SetTimer(1, 300, NULL);
	return 0;
}

INT CKL_OTHER_LOAN_OFFER::SendBook()
{

	// 16/03/07 이충무 : 2016년 국립중앙도서관 책이음개선
	// 발송성공메시지 처리 및 발송권수 처리
	/*//NEW------------------------------------------------------------------------*/
	BOOL bSuccess = FALSE;
	m_listUserBookCnt.RemoveAll();
	/*//END------------------------------------------------------------------------*/

	CString strState = GetComboBoxState();
	if(_T("0") != strState)
	{
		AfxMessageBox(_T("요청도서관 관리에서는 신청 상태에서만 발송이 가능합니다."));
		return -1;
	}
	INT ids = 0;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}
	INT nIdx = m_pGrid->SelectGetTailPosition();
	CString strLoanKey;

	// 16/03/07 이충무 : 2016년 국립중앙도서관 책이음개선
	// 발송권수 처리
	/*//NEW------------------------------------------------------------------------*/
	int nBookCnt = 0;
	CString strUserNo = _T("");

	for(int i=0;i<m_pGrid->SelectGetCount();i++)
	{
		m_pDM->GetCellData(_T("이용자번호"), nIdx, strUserNo);

		if(strUserNo.Compare(_T("")) != 0)
		{
			if(m_listUserBookCnt.Lookup(strUserNo, nBookCnt))
			{
				nBookCnt += 1;
			}
			else
			{
				nBookCnt = 1;
			}

			m_listUserBookCnt.SetAt(strUserNo, nBookCnt);
		}				
		nIdx=m_pGrid->SelectGetPrev();
	}

	nIdx = m_pGrid->SelectGetTailPosition();
	
	/*//END------------------------------------------------------------------------*/

	// 16/02/25 박우람 : 2016년 국립중앙도서관 책이음개선
	// 부록신청유무, 부록발송유무, 부록등록번호리스트
	/*//NEW------------------------------------------------------------------------*/
	CString strAppendixRctYN = _T(""), strAppendixSendYN = _T("N"), strAppendixRegNoList = _T("");
	/*//END------------------------------------------------------------------------*/

	CFrameWriter Writer(m_pInfo);
	for(i=0;i<m_pGrid->SelectGetCount();i++)
	{
		// 16/08/25 김혜영 : 부록자료 타관발송시 부록발송유무가 초기화 되지 않아 수정
//*/ ADD -----------------------------------------------------------------------------------------------
		strAppendixSendYN = _T("N");  // 부록발송유무 설정
		strAppendixRegNoList = _T(""); //부록등록번호 리스트 
//*/ END -----------------------------------------------------------------------------------------------

		// 2013.12.19 ADD BY PWR : 2013도서관통합서비스확대구축및개선 (발송처리 성공시 책상태 변경)
		CString strRegNo;
		m_pDM->GetCellData(_T("등록번호"), nIdx, strRegNo);
		if(strRegNo.IsEmpty()) {
			m_pDM->SetCellData(_T("처리결과"),_T("FAIL (REG_NO IS NULL)"),nIdx);
			// 16/06/21 박우람 : 2016년도 공공도서관 기술정보센터 위탁운영
			// index 이동 누락 추가 (부록발송이 다른자료로 처리되는 현상 수정)
			/*//NEW------------------------------------------------------------------------*/
			nIdx=m_pGrid->SelectGetPrev();
			/*//END------------------------------------------------------------------------*/
			continue;
		}

		m_pDM->GetCellData(_T("로컬KEY"), nIdx, strLoanKey);

		// 17/06/29 김혜영 : 책이음신청후 책이 자관대출되었을때 타관에 발송되지 않도록 수정
//*/ ADD ----------------------------------------------------------------------------------
		CString strBookKey = _T("");
		CString strWorkingStatus = _T("");
		CString strQuery = _T("");
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(_T(""));		
		m_pDM->GetCellData(_T("책KEY"), nIdx, strBookKey);		
		strQuery.Format(_T("SELECT WORKING_STATUS FROM BO_BOOK_TBL WHERE REC_KEY='%s'"), strBookKey);
		TmpDM.GetOneValueQuery(strQuery,strWorkingStatus);
		
		if(strWorkingStatus != _T("BOL112N")) //자료상태가 비치자료인지 확인
		{
			m_pDM->SetCellData(_T("처리결과"),_T("FAIL (비치자료 상태가 아님)"),nIdx);			
			nIdx=m_pGrid->SelectGetPrev();			
			continue;
		}
//*/ END ----------------------------------------------------------------------------------

		// 16/02/25 박우람 : 2016년 국립중앙도서관 책이음개선
		// 제공도서관에서 대출도서관으로 발송시 이용자가 부록신청을 한 자료는 부록대출 화면을 표시
		/*//NEW------------------------------------------------------------------------*/
		m_pDM->GetCellData(_T("부록신청유무"), nIdx, strAppendixRctYN);
		if (_T("Y") == strAppendixRctYN) {
			// 부록발송 화면
			CESL_DataMgr *pDM = FindDM(_T("DM_KL_OTHER_APPENDIX_LIST"));
			if (pDM == NULL) {
				m_pDM->SetCellData(_T("처리결과"),_T("FAIL (DM_KL_OTHER_APPENDIX_LIST)"),nIdx);
				// 16/06/21 박우람 : 2016년도 공공도서관 기술정보센터 위탁운영
				// index 이동 누락 추가 (부록발송이 다른자료로 처리되는 현상 수정)
				/*//NEW------------------------------------------------------------------------*/
				nIdx=m_pGrid->SelectGetPrev();
				/*//END------------------------------------------------------------------------*/
				continue;
			}
			CString strWhere;
			strWhere.Format(_T("B.SPECIES_KEY = (SELECT BB.SPECIES_KEY FROM BO_BOOK_TBL BB WHERE BB.REG_NO='%s')"), strRegNo);
			pDM->FreeData();
			INT ids = pDM->RefreshDataManager(strWhere);
			if (0 > ids) {
				m_pDM->SetCellData(_T("처리결과"),_T("FAIL (부록조회 실패)"),nIdx);
				// 16/06/21 박우람 : 2016년도 공공도서관 기술정보센터 위탁운영
				// index 이동 누락 추가 (부록발송이 다른자료로 처리되는 현상 수정)
				/*//NEW------------------------------------------------------------------------*/
				nIdx=m_pGrid->SelectGetPrev();
				/*//END------------------------------------------------------------------------*/
				continue;
			}
			CKL_OTHER_APPENDIX_LIST AppendixList(this);
			ids = AppendixList.DoModal();
			if (IDOK == ids) {
				if (0 < pDM->GetRowCount()) {
					// 부록발송유무 설정
					strAppendixSendYN = _T("Y");			
					// 16/08/05 김혜영 : 부록자료 타관발송시 부록등록번호 리스트가 초기화 되지 않아 수정
//*/ ADD -----------------------------------------------------------------------------------------------
					strAppendixRegNoList = _T("");
//*/ END -----------------------------------------------------------------------------------------------
					// 부록등록번호리스트 설정
					CString strRegNo;
					for (INT i = 0; i < pDM->GetRowCount(); i++) {
						if (FALSE == strAppendixRegNoList.IsEmpty()) strAppendixRegNoList += _T(",");
						pDM->GetCellData(_T("등록번호"), i, strRegNo);
						strAppendixRegNoList += strRegNo;
					}
				}
				// 16/02/26 이충무 : 2016년 국립중앙도서관 책이음개선
				// 발송성공메시지 처리
				/*//NEW------------------------------------------------------------------------*/
				if(!bSuccess)
					bSuccess = TRUE;
				/*//END------------------------------------------------------------------------*/
			} else {
				m_pDM->SetCellData(_T("처리결과"),_T("발송취소"),nIdx);
				// 16/06/21 박우람 : 2016년도 공공도서관 기술정보센터 위탁운영
				// index 이동 누락 추가 (부록발송이 다른자료로 처리되는 현상 수정)
				/*//NEW------------------------------------------------------------------------*/
				nIdx=m_pGrid->SelectGetPrev();
				/*//END------------------------------------------------------------------------*/
				continue;
			}
		}
		/*//END------------------------------------------------------------------------*/

		Writer.clear();
		Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("TRANSACTION_WORK"), _T("UPDATE"));
		Writer.addElement(_T("TRANSACTION_CODE"), _T("1"));
		Writer.addElement(_T("LOAN_KEY"),strLoanKey);
		Writer.addElement(_T("CANCEL"),_T("0"));
		
		// 16/02/25 박우람 : 2016년 국립중앙도서관 책이음개선
		// 발송시 부록발송유무, 부록등록번호리스트 설정
		/*//NEW------------------------------------------------------------------------*/
		Writer.addElement(_T("APPENDIX_SEND_YN"), strAppendixSendYN);
		Writer.addElement(_T("APPENDIX_REGNO_LIST"), strAppendixRegNoList);
		/*//END------------------------------------------------------------------------*/

		CFrameSender Sender(m_pInfo);	
		CFrameReader Reader; 
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if (0 > ids) 
		{
			while(true) 
			{
				// 2015년 공공도서관 시스템 개선 2015/12/11 : 통합도서서비스 > 책이음서비스
				if(IDYES == MessageBox(_T("책이음서비스 미들웨어와의 통신에 실패하였습니다.\r\n재시도 하시겠습니까?"), _T("조회실패"), MB_YESNO | MB_ICONQUESTION)) 
				{
					ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
					if(0 <= ids) break;
				}
				else
				{
					// 16/06/21 박우람 : 2016년도 공공도서관 기술정보센터 위탁운영
					/*//NEW------------------------------------------------------------------------*/
					m_pGrid->Display();
					/*//END------------------------------------------------------------------------*/
					return 0;
				}
			}
		}
		
		Reader.parseFrame(Reader.getFrame());
		if(_T("OK") != Reader.getElement(0,0))
		{
			if(_T("")==Reader.getElement(0,0))
			{
				m_pDM->SetCellData(_T("처리결과"),_T("FAIL"),nIdx);
			}
			else
			{
				// 16/06/21 박우람 : 2016년도 공공도서관 기술정보센터 위탁운영
				// index 오류 발생 예측 수정
				/*//BEFORE-----------------------------------------------------------------------
				m_pDM->SetCellData(_T("RESULT"), Reader.getElement(nIdx, _T("RESULT")), nIdx);
				*///AFTER------------------------------------------------------------------------
				m_pDM->SetCellData(_T("처리결과"), Reader.getElement(0, _T("REASON")), nIdx);
				/*//END------------------------------------------------------------------------*/
			}
			AfxMessageBox(_T("실패 발생"));
			
		}
		else
		{
			// 16/02/26 이충무 : 2016년 국립중앙도서관 책이음개선
			// 발송성공메시지 처리
			/*//NEW------------------------------------------------------------------------*/
			if(!bSuccess)
				bSuccess = TRUE;
			/*//END------------------------------------------------------------------------*/

			// 16/03/02 이충무 : 2016년 국립중앙도서관 책이음개선
			// 발송 SMS
			/*//NEW------------------------------------------------------------------------*/
			SendBook_SendSMS(nIdx);
			m_pDM->DeleteRow(nIdx);
				// 16/06/21 박우람 : 2016년도 공공도서관 기술정보센터 위탁운영
				/*//REM--------------------------------------------------------------------------
				nIdx=m_pGrid->SelectGetPrev();
				*///END--------------------------------------------------------------------------
			/*//END------------------------------------------------------------------------*/
			// 2013.12.19 ADD BY PWR : 2013도서관통합서비스확대구축및개선 (발송처리 성공시 책상태 변경)
			/*
			m_pDM->StartFrame();
			CString strQuery;
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS='BOL611O',PREV_WORKING_STATUS=WORKING_STATUS WHERE REG_NO='%s';"), strRegNo);
			m_pDM->AddFrame(strQuery);
			m_pDM->SendFrame();
			m_pDM->EndFrame();

			m_pDM->DeleteRow(nIdx);
			*/
		}
		// 16/03/03 이충무 : 2016년 국립중앙도서관 책이음개선
		// 다른 코드와 같이 상단으로 이동
		/*//REM--------------------------------------------------------------------------
		//nIdx=m_pGrid->SelectGetPrev();
		*///END--------------------------------------------------------------------------

		// 16/06/21 박우람 : 2016년도 공공도서관 기술정보센터 위탁운영
		// index 이동 누락 추가 (부록발송이 다른자료로 처리되는 현상 수정)
		/*//NEW------------------------------------------------------------------------*/
		nIdx=m_pGrid->SelectGetPrev();
		/*//END------------------------------------------------------------------------*/
	}

	m_pGrid->Display();

	// 16/03/07 이충무 : 2016년 국립중앙도서관 책이음개선
	// 발송성공메시지 처리 빛 발송권수 처리
	/*//BEFORE-----------------------------------------------------------------------
	AfxMessageBox(_T("발송 처리 완료"));
	*///AFTER------------------------------------------------------------------------
	if(bSuccess)
		AfxMessageBox(_T("발송 처리 완료"));

	m_listUserBookCnt.RemoveAll();
	/*//END------------------------------------------------------------------------*/
	
	return ids;
}


INT CKL_OTHER_LOAN_OFFER::OfferCancel()
{
	CString strState = GetComboBoxState();
	// 16/01/29 이충무 : 2016년 국립중앙도서관 책이음개선
	// 대출상태 추가로 취소 불가능 추가
	/*//BEFORE-----------------------------------------------------------------------
	if(_T("0") == strState || _T("3") == strState)
	*///AFTER------------------------------------------------------------------------
	if(_T("0") == strState || _T("3") == strState || _T("4") == strState)
	{
		AfxMessageBox(_T("제공자료관리에서는 발송 및 발송거절 상태에서만 작업취소가 가능합니다."));
		return -1;
	}
	/*//END------------------------------------------------------------------------*/
	INT ids = 0;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;

	// JMJ KOL.RED.003 작업취소시 부록도 같이 적용
	CString strAppendixRegNoList, strAppendixSendYN;

	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}
	INT nIdx = m_pGrid->SelectGetTailPosition();
	CString strLoanKey;
	
	CString strTransactionCode;
	
	if(_T("1")==strState) strTransactionCode = _T("0");
	if(_T("2")==strState) strTransactionCode = _T("0");
	if(_T("3")==strState) strTransactionCode = _T("1");
	
	for(int i=0;i<m_pGrid->SelectGetCount();i++)
	{
		m_pDM->GetCellData(_T("로컬KEY"), nIdx, strLoanKey);
		CFrameWriter Writer(m_pInfo);
		Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("TRANSACTION_WORK"), _T("UPDATE"));
		Writer.addElement(_T("TRANSACTION_CODE"), strTransactionCode);
		Writer.addElement(_T("LOAN_KEY"),strLoanKey);
		Writer.addElement(_T("CANCEL"),_T("1"));

		m_pDM->GetCellData(_T("부록발송유무"), nIdx, strAppendixSendYN);
		m_pDM->GetCellData(_T("부록등록번호리스트"), nIdx, strAppendixRegNoList);
	
		if(strAppendixSendYN.Compare(_T("Y")) == 0){
			Writer.addElement(_T("APPENDIX_SEND_YN"), strAppendixSendYN);
			Writer.addElement(_T("APPENDIX_REGNO_LIST"), strAppendixRegNoList);
		}
		
		CFrameSender Sender(m_pInfo);	
		CFrameReader Reader; 
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if (0 > ids) 
		{
			while(true) 
			{
				// 2015년 공공도서관 시스템 개선 2015/12/11 : 통합도서서비스 > 책이음서비스
				if(IDYES == MessageBox(_T("책이음서비스 미들웨어와의 통신에 실패하였습니다.\r\n재시도 하시겠습니까?"), _T("조회실패"), MB_YESNO | MB_ICONQUESTION)) 
				{
					ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
					if(0 <= ids) break;
				}
				else
				{
					return 0;
				}
			}
		}
		
		if(_T("2")==strState)
		{
			if(_T("OK") != Reader.getElement(0,0))
			{
			CString strRefuseCode, strRefuseMsg;
			strRefuseCode = Reader.getElement(i,_T("REASON"));			
			strRefuseMsg = TransCodeToMsg(strRefuseCode);
			AfxMessageBox(strRefuseMsg);
			return -1;
			}
		}
		Reader.parseFrame(Reader.getFrame());
		if(_T("OK") != Reader.getElement(0,0))
		{
			if(_T("")==Reader.getElement(0,0))
			{
				m_pDM->SetCellData(_T("처리결과"),_T("FAIL"),nIdx);
			}
			else
			{
				// 16/06/21 박우람 : 2016년도 공공도서관 기술정보센터 위탁운영
				// index 오류 발생 예측 수정
				/*//BEFORE-----------------------------------------------------------------------
				m_pDM->SetCellData(_T("RESULT"), Reader.getElement(nIdx, _T("RESULT")), nIdx);
				*///AFTER------------------------------------------------------------------------
				m_pDM->SetCellData(_T("처리결과"), Reader.getElement(0, _T("REASON")), nIdx);
				/*//END------------------------------------------------------------------------*/
			}
			AfxMessageBox(_T("실패 발생"));
		} 
		else 
		{
			m_pDM->DeleteRow(nIdx);
		}

		nIdx=m_pGrid->SelectGetPrev();	
	}

	m_pGrid->Display();
	AfxMessageBox(_T("작업 취소 완료"));

	return ids;
	
}

//INT CKL_OTHER_LOAN_OFFER::SendRefuse(CString strMessage)
INT CKL_OTHER_LOAN_OFFER::SendRefuse()
{	
	INT ids = 0;	
	CString strState = GetComboBoxState();
	if(_T("0") != strState)
	{
		AfxMessageBox(_T("신청 상태에서만 발송거절이 가능합니다."));
		return -1;
	}	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}		
	else if(1 < m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("여러개의 선정을 하실 경우 입력한 발송 거절 메시지가 일괄 적용됩니다."));
	}	
	
	// 17/08/30 김혜영 : 공통>대출반납>타관대출관리에서 발송거절시 메모리누수 현상 발생
/*//BEFORE-----------------------------------------------------------------------------
	CKL_OTHER_LOAN_SEND_REFUSE dlg = new CKL_OTHER_LOAN_SEND_REFUSE(this);
	ids = dlg.DoModal();
	
	CString strMessage = _T("");
	INT nSuccess;
	// 16.10.20 JMJ KOL.RED.008
	BOOL bReasonSendYN = FALSE;
	
	
	nSuccess = dlg.m_nSuccess;
	strMessage = dlg.m_strMessage;
	// 16.10.20 JMJ KOL.RED.008
	bReasonSendYN = dlg.m_bMessageSendYN;
*///AFTER------------------------------------------------------------------------------
	CKL_OTHER_LOAN_SEND_REFUSE *dlg = new CKL_OTHER_LOAN_SEND_REFUSE(this);
	ids = dlg->DoModal();
	
	CString strMessage = _T("");
	INT nSuccess;
	// 16.10.20 JMJ KOL.RED.008
	BOOL bReasonSendYN = FALSE;
	
	nSuccess = dlg->m_nSuccess;
	strMessage = dlg->m_strMessage;
	// 16.10.20 JMJ KOL.RED.008
	bReasonSendYN = dlg->m_bMessageSendYN;
	if (dlg != NULL)
	{
		delete dlg;
		dlg = NULL;
	}
/*//END-------------------------------------------------------------------------------*/
	
	INT nIdx = m_pGrid->SelectGetTailPosition();
	
	// 16/03/07 이충무 : 2016년 국립중앙도서관 책이음개선
	// 발송권수 처리
	/*//NEW------------------------------------------------------------------------*/	
	m_listUserBookCnt.RemoveAll();

	int nBookCnt = 0;
	CString strUserNo = _T("");

	for(int i=0;i<m_pGrid->SelectGetCount();i++)
	{
		m_pDM->GetCellData(_T("이용자번호"), nIdx, strUserNo);

		if(strUserNo.Compare(_T("")) != 0)
		{
			if(m_listUserBookCnt.Lookup(strUserNo, nBookCnt))
			{
				nBookCnt += 1;
			}
			else
			{
				nBookCnt = 1;
			}

			m_listUserBookCnt.SetAt(strUserNo, nBookCnt);
		}				
		nIdx=m_pGrid->SelectGetPrev();
	}

	nIdx = m_pGrid->SelectGetTailPosition();
	/*//END------------------------------------------------------------------------*/

	// 17/08/28 김혜영 : 자관대출된 자료를 발송거절할 때 신청제공여부가 무조건 Y로 변경되는 현상 수정
//*/ ADD ----------------------------------------------------------------------------------
	CString strBookKey = _T("");					
	CString strManageCode = m_pInfo->MANAGE_CODE;
	CString strLILLYN = _T("");
	CString strQuery = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
//*/ END ----------------------------------------------------------------------------------

	CString strLoanKey;
	if(1 == nSuccess)
	{
		for(int i=0;i<m_pGrid->SelectGetCount();i++)
		{
			m_pDM->GetCellData(_T("로컬KEY"), nIdx, strLoanKey);
			CFrameWriter Writer(m_pInfo);
			Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
			Writer.addRecord();
			Writer.addElement(_T("TRANSACTION_WORK"), _T("UPDATE"));
			Writer.addElement(_T("TRANSACTION_CODE"), _T("2"));
			Writer.addElement(_T("LOAN_KEY"),strLoanKey);
			Writer.addElement(_T("SEND_REJECT_REASON"),strMessage);
			Writer.addElement(_T("CANCEL"),_T("0"));
			
			// 17/08/28 김혜영 : 자관대출된 자료를 발송거절할 때 신청제공여부가 무조건 Y로 변경되는 현상 수정
//*/ ADD ----------------------------------------------------------------------------------
			m_pDM->GetCellData(_T("책KEY"), nIdx, strBookKey);	
			if(strBookKey != _T("")) 
			{
				strQuery.Format(_T("SELECT GET_ASK_LILL_LOAN(WORKING_STATUS, REC_KEY, USE_LIMIT_CODE, SEPARATE_SHELF_CODE, REG_CODE, SHELF_LOC_CODE, REG_NO) FROM BO_BOOK_TBL WHERE REC_KEY ='%s' AND MANAGE_CODE ='%s' "), strBookKey, strManageCode);
				TmpDM.GetOneValueQuery(strQuery,strLILLYN);
				if(strLILLYN != _T("O"))
				{
					strLILLYN = _T("N");
				}
				else
				{
					strLILLYN = _T("Y");
				}
				Writer.addElement(_T("SERVICE_YN"),strLILLYN);
			}
//*/ END ----------------------------------------------------------------------------------

			CFrameSender Sender(m_pInfo);	
			CFrameReader Reader; 
			ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if (0 > ids) 
			{
				while(true) 
				{
					// 2015년 공공도서관 시스템 개선 2015/12/11 : 통합도서서비스 > 책이음서비스
					if(IDYES == MessageBox(_T("책이음서비스 미들웨어와의 통신에 실패하였습니다.\r\n재시도 하시겠습니까?"), _T("조회실패"), MB_YESNO | MB_ICONQUESTION)) 
					{
						ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
						if(0 <= ids) break;
					}
					else
					{
						m_pGrid->Display();
						return 0;
					}
				}
			}		
			
			Reader.parseFrame(Reader.getFrame());
			if(_T("OK") != Reader.getElement(0,0))
			{
				if(_T("")==Reader.getElement(0,0))
				{
					m_pDM->SetCellData(_T("처리결과"),_T("FAIL"),nIdx);
				}
				else
				{
					// 16/06/21 박우람 : 2016년도 공공도서관 기술정보센터 위탁운영
					// index 오류 발생 예측 수정
					/*//BEFORE-----------------------------------------------------------------------
					m_pDM->SetCellData(_T("RESULT"), Reader.getElement(nIdx, _T("RESULT")), nIdx);
					*///AFTER------------------------------------------------------------------------
					m_pDM->SetCellData(_T("처리결과"), Reader.getElement(0, _T("REASON")), nIdx);
					/*//END------------------------------------------------------------------------*/
				}
				AfxMessageBox(_T("실패 발생"));
				
			} 
			else 
			{
				// 16/03/02 이충무 : 2016년 국립중앙도서관 책이음개선
				// 수정내용
				/*//BEFORE-----------------------------------------------------------------------
				ids = Refuse_SendSMS(nIdx);
				*///AFTER------------------------------------------------------------------------
				// 16.10.20 JMJ KOL.RED.008
				ids = Refuse_SendSMS(nIdx, strMessage, bReasonSendYN);
				/*//END------------------------------------------------------------------------*/
							
				m_pDM->DeleteRow(nIdx);
			}

			nIdx=m_pGrid->SelectGetPrev();
		}
		
		m_pGrid->Display();
		AfxMessageBox(_T("발송 거절 완료"));		
	}	

	// 16/03/07 이충무 : 2016년 국립중앙도서관 책이음개선
	// 발송권수 처리
	/*//NEW------------------------------------------------------------------------*/	
	m_listUserBookCnt.RemoveAll();
	/*//END------------------------------------------------------------------------*/
	return ids;
}

//발송거절 SMS 문자발송
// 16/03/02 이충무 : 2016년 국립중앙도서관 책이음개선
// 발송거절 이유추가
/*//BEFORE-----------------------------------------------------------------------
INT CKL_OTHER_LOAN_OFFER::Refuse_SendSMS(INT nIdx)
*///AFTER------------------------------------------------------------------------
INT CKL_OTHER_LOAN_OFFER::Refuse_SendSMS(INT nIdx, CString strReason, BOOL bReasonSendYN)
/*//END------------------------------------------------------------------------*/
{
	INT ids=0;
	CString strUserNo;
	CString strMessage;
	CString strQuery;
	m_pDM->GetCellData(_T("이용자번호"), nIdx, strUserNo);

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));

	// 16/03/07 이충무 : 2016년 국립중앙도서관 책이음개선
	// 15/12/10 입수시 수정된 내용 가져다 사용 및 발송권수 처리
	/*//BEFORE-----------------------------------------------------------------------
	strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS13'"));
	*///AFTER------------------------------------------------------------------------
	CString strManageCode = m_pInfo->MANAGE_CODE;
	if (strManageCode.IsEmpty())
		strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS13'"));
	else
		strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS13' AND MANAGE_CODE='%s'"), strManageCode);

	int nBookCnt = 0;
	/*//END------------------------------------------------------------------------*/
	
	TmpDM.RestructDataManager(strQuery);
	TmpDM.GetCellData(0,0,strMessage);
	
	//도서관의 SMS 사용 여부를 체크한다.
	INT nIsSMS = 0;
	nIsSMS = m_pInfo->GetSMSUseMode(); // 0:N, 1:Y
	
	if(1 == nIsSMS)
	{				
		//이용자의 SMS 사용 여부를 체크한다.
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(_T(""));
		
		CString strQuery;
		CString strUserKey;
		strQuery.Format(_T("SELECT CLU.SMS_USE_YN, CLU.HANDPHONE, CLU.NAME, CLU.REC_KEY FROM CO_LOAN_USER_TBL CLU WHERE USER_NO='%s'"),strUserNo);
		dm.RestructDataManager(strQuery);
		CString strSMSYN, strHandPhone, strName;
		dm.GetCellData(0, 0, strSMSYN);
		dm.GetCellData(0, 1, strHandPhone);
		dm.GetCellData(0, 2, strName);
		dm.GetCellData(0, 3, strUserKey);

		// 16/03/02 이충무 : 2016년 국립중앙도서관 책이음개선
		// 발송거절 이유 추가
		/*//BEFORE-----------------------------------------------------------------------
		strMessage = strName+strMessage;
		*///AFTER------------------------------------------------------------------------
		//strMessage = strName+strMessage;		
		/*//END------------------------------------------------------------------------*/
				
		//SMS 사용 시
		if(_T("Y")==strSMSYN)
		{
			// 16/03/07 이충무 : 2016년 국립중앙도서관 책이음개선
			// 발송권수 처리
			/*//NEW------------------------------------------------------------------------*/
			if(!m_listUserBookCnt.Lookup(strUserNo, nBookCnt))
			{
				nBookCnt= 0;
			}

			if(nBookCnt > 0)
			{		
			/*//END------------------------------------------------------------------------*/
				//프로시저 호출!
				CString strType = _T("SMS13");
				
				// 16/03/07 이충무 : 2016년 국립중앙도서관 책이음개선
				// 발송거절 추가 및 발송권수 처리
				/*//NEW------------------------------------------------------------------------*/
				SMS_CONNECTION_INFO		SMS_ConnectionInfo;
				SMS_MSG_INFO	SMS_MsgInfo;

				CSMS_MakeMsg MakeSMS( this );
				CSMS_Env_Info SMSEnvInfo(this);
				SMSEnvInfo.MakeSMSEnv();
				SMSEnvInfo.GetSMSEnv( strType, &SMS_ConnectionInfo, &SMS_MsgInfo );

				CString strTitleInfo;
				m_pDM->GetCellData(_T("서명"), nIdx, strTitleInfo);

				CString strBookCnt;
				strBookCnt.Format(_T("%d"),nBookCnt);
				
				MakeSMS.MakeSMSMsg( strType , &SMS_MsgInfo , strName , _T(""), strTitleInfo , strBookCnt , strMessage );

				/*
				if(strReason.Compare(_T("")) != 0)
				{
					strMessage += _T(",사유:")+strReason;
				}
				*/
				CString strReplace = strMessage;
				strReplace.Replace(_T("'"),_T("''"));								
				CString strResult = _T("");
				// 17/11/01 김혜영 : LMS추가로 인한 처리
/*//BEFORE-----------------------------------------------------------------------------
				strQuery.Format(_T("SELECT SEND_MESSAGE FROM MN_SMS_LOG_TBL WHERE to_char(SEND_TIME, 'yyyymmdd') = to_char(SYSDATE, 'yyyymmdd') AND SEND_MESSAGE = '%s'"),strReplace);
*///AFTER------------------------------------------------------------------------------
				strQuery.Format(_T("SELECT SEND_MESSAGE FROM MN_SMS_LOG_VIEW WHERE to_char(SEND_TIME, 'yyyymmdd') = to_char(SYSDATE, 'yyyymmdd') AND SEND_MESSAGE = '%s'"),strReplace);
/*//END-------------------------------------------------------------------------------*/
				dm.GetOneValueQuery(strQuery, strResult);

				//JOB.2018.0093 당일 보낸 메세지도 확인해서 다시 보낼 수 있도록 처리
				/*//BEFORE==========================================================================================
				if(strResult.Compare(_T("")) == 0)
				{
					ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);					
				}
				// 16.10.20 JMJ KOL.RED.008
				// 사유 별도발송
				if(strReason.Compare(_T("")) != 0 && bReasonSendYN == TRUE)
				{
					strMessage = _T("사유:")+strReason;
					ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);					
				}			
				*///AFTER===========================================================================================
				if(strResult.Compare(_T("")) != 0)
				{
					if(IDYES == MessageBox(_T("금일 발송한 메시지입니다.\r\nSMS를 발송하시겠습니까?"), _T("SMS발송확인"), MB_YESNO | MB_ICONQUESTION)) 
					{
						ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);
						if(strReason.Compare(_T("")) != 0 && bReasonSendYN == TRUE)
						{
							strMessage = _T("사유:")+strReason;
							ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);					
						}
					}
				}
				else 
				{
					ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);	
					if(strReason.Compare(_T("")) != 0 && bReasonSendYN == TRUE)
					{
						strMessage = _T("사유:")+strReason;
						ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);					
					}
				}
				//================================================================================================

				/*//END------------------------------------------------------------------------*/
				if(0 > ids)
				{
					AfxMessageBox(_T("메세지 발송에 실패하였습니다."));
				}
				// 16/03/07 이충무 : 2016년 국립중앙도서관 책이음개선
				// 발송권수 처리
				/*//NEW------------------------------------------------------------------------*/
				else
				{
					m_listUserBookCnt.RemoveKey(strUserNo);
				}
				/*//END------------------------------------------------------------------------*/						
			// 16/03/07 이충무 : 2016년 국립중앙도서관 책이음개선
			// 발송권수 처리
			/*//NEW------------------------------------------------------------------------*/
			}
			/*//END------------------------------------------------------------------------*/						
		}
		else
		{
			AfxMessageBox(_T("해당 이용자는 SMS 서비스를 사용하지 않습니다."));
		}
		
	}
	else
	{
		AfxMessageBox(_T("해당 도서관은 SMS 서비스를 사용하지 않습니다."));
	}
	return ids;
	
}

INT CKL_OTHER_LOAN_OFFER::CallProcedure(CString strUserNo, CString strHandPhone, CString strName, CString strMessage, CString strType)
{	
	INT ids;
	// 1. DM SETTING 
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);
	
	// FIRST_WORK
	CString sFirst_Work;
	sFirst_Work.Format( _T("%s"), GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ) );
	// SEND PHONE
	CString sQuery, sDividCnt, sSendMsg, sSendNum, sRecevNum, sUserKey, sLoanKey, sUserName;
	sRecevNum = strHandPhone;
	CCfgFileApi cfgFileApi;
	CString strCfgFilePath = _T("..\\Cfg\\SMS_Callback.cfg");
	cfgFileApi.SetFile(strCfgFilePath);
	CFileFind ff;
	if(ff.FindFile(strCfgFilePath))
	{
		CString strCallbackTarget;
		cfgFileApi.GetData(_T("회신번호설정적용대상"), strCallbackTarget);
		CString strValue;
		if(_T("1") == strCallbackTarget)
		{
			cfgFileApi.GetData(_T("회신번호"), strValue);
			if(!strValue.IsEmpty()) sSendNum = strValue;
		}
	}
	sRecevNum.Replace(_T("-"), _T(""));
	
	sSendMsg = strMessage;
	sSendMsg.Replace(_T("'"), _T("''"));
	sUserKey = strUserNo;
	sLoanKey = _T("0");
	sUserName = strName;

	// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*	// (1:USER_KEY,2:LOAN_KEY,3:SEND_TEL,4:RECV_TEL,5:MSG,6:MSG_GB,7:MANAGE_CODE,8:SMS_TYPE,9:SEND_WORKER,10:FIRST_WORK,11:USER_NAME)
	sQuery.Format( _T("SMS_TOT_SEND_PROC(%s,%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s');")
		, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg 	// 1,2,3,4,5
		, _T(""), m_pInfo->MANAGE_CODE, strType				// 6,7,8
		, m_pInfo->USER_NAME, sFirst_Work, sUserName );			// 9,10,11

	ids = TmpDM.ExecuteQuery( sQuery, TRUE );
*/	
	if (g_strKakaoUsed1.IsEmpty())
	{
		sQuery.Format( 
			_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KAKAO플러스친구시스템정보' AND value_name='KAKAO플러스친구사용유무' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		TmpDM.GetOneValueQuery(sQuery, g_strKakaoUsed1);
		
		// 설정값이 없다면 N으로 적용
		if (g_strKakaoUsed1.IsEmpty())
		{
			g_strKakaoUsed1 = _T("N");
		}
	}

	if (g_strKakaoUsed1.CompareNoCase(_T("Y")) == 0)
	{
		// 카카오 플러스 친구 발송
		sQuery.Format( _T("KAKAOPLUS_TOT_SEND_PROC(%s, %s, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');")
								, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg 	// 1,2,3,4,5
								, _T(""), m_pInfo->MANAGE_CODE, strType				// 6,7,8
								, m_pInfo->USER_NAME, sFirst_Work, sUserName,				    // 9,10
								m_pInfo->LIB_NAME , sSendMsg );
	}
	else
	{
		// SMS 발송
		sQuery.Format( _T("SMS_TOT_SEND_PROC(%s,%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s');")
				, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg 	// 1,2,3,4,5
				, _T(""), m_pInfo->MANAGE_CODE, strType				// 6,7,8
				, m_pInfo->USER_NAME, sFirst_Work, sUserName );			// 9,10,11
	}
	ids = TmpDM.ExecuteQuery(sQuery, TRUE);
	// KOL.RED.2018.224 ---------------------------------------------------------------------------
	if(0>ids)
	{ 
		AfxMessageBox(_T("메시지 발송에 실패하였습니다."));
	}
	return 0;
	
}

INT CKL_OTHER_LOAN_OFFER::SelectGrid(CString sRegNo)
{
	CString strRegNo;
	// 17/06/28 김혜영 : 타관대출관리에서 바코드로 입력된 등록번호가 그리드에 존재하지 않을때 메시지 처리
//*/ ADD ---------------------------------------------------------------------------
	BOOL bExsistRegNo = FALSE;
//*/ END ---------------------------------------------------------------------------	
	for(int i=0; i<m_pDM->GetRowCount();i++)
	{
		m_pDM->GetCellData(_T("등록번호"), i, strRegNo);
		if(sRegNo == strRegNo)
		{
			// 16/06/21 박우람 : 2016년도 공공도서관 기술정보센터 위탁운영
			// 컬럼 순서 변경시 오류 발생 예측 수정
			/*//BEFORE-----------------------------------------------------------------------
			m_pGrid->SetAt(i, 0, _T("V"));
			m_pGrid->DisplayLine(i);
			*///AFTER------------------------------------------------------------------------
			m_pGrid->SetAt(i, m_pGrid->m_nSelectIdx, m_pGrid->m_sSelectString);
			m_pGrid->m_nReverse = i;
			m_pGrid->SetReverse(i);
			/*//END------------------------------------------------------------------------*/

			// 17/05/19 김혜영 : 타관대출관리/타관반납관리의 GRID 선정컬럼으로 정렬
//*/ ADD ---------------------------------------------------------------------------
			m_pDM->SetCellData(_T("선정"), m_pGrid->m_sSelectString, i);
//*/ END ---------------------------------------------------------------------------

			// 17/06/28 김혜영 : 타관대출관리에서 바코드로 입력된 등록번호가 그리드에 존재하지 않을때 메시지 처리
//*/ ADD ---------------------------------------------------------------------------
			bExsistRegNo = TRUE;
//*/ END ---------------------------------------------------------------------------
		}
		// 17/02/23 김혜영 : 타관대출관리에서 바코드로 자료 선정이 되도록 기능 개선
		// 첫번째 row만 검색하고 종료하도록 되어 있어서 제거함
/*//REM--------------------------------------------------------------------------
		else
		{
			return -1;
		}
*///END--------------------------------------------------------------------------
	}

	// 17/06/28 김혜영 : 타관대출관리에서 바코드로 입력된 등록번호가 그리드에 존재하지 않을때 메시지 처리
//*/ ADD ---------------------------------------------------------------------------
	if(bExsistRegNo != TRUE)
	{
		return 1;
	}
//*/ END --------------------------------------------------------------------------

	// 17/05/19 김혜영 : 타관대출관리/타관반납관리의 GRID 선정컬럼으로 정렬
//*/ ADD ---------------------------------------------------------------------------
	m_pDM->SORT(_T("선정"),TRUE);
	m_pGrid->Display();
//*/ END ---------------------------------------------------------------------------
	return 0;
}

CString CKL_OTHER_LOAN_OFFER::TransCodeToMsg(CString transCode)
{
	CString msg;

	if(_T("100") == transCode)
	{
		msg = _T("통합회원이 아닙니다.");
	}
	else if(_T("200") == transCode)
	{
		msg = _T("요청하는 도서관에 회원가입이 되어 있지 않습니다.");
	}
	else if(_T("300") == transCode)
	{
		// 2015년 공공도서관 시스템 개선 2015/12/11 : 통합도서서비스 > 책이음서비스
		msg = _T("책이음 최대 대출건수를 초과하였습니다.");
	}
	else if(_T("400") == transCode)
	{
		msg = _T("현재 도서가 대출할 수 있는 상태가 아닙니다.");
	}
	else if(_T("500") == transCode)
	{
		msg = _T("해당도서관에 대출정지회원으로 등록되어 있습니다.");
	}
	else if(_T("600") == transCode)
	{
		msg = _T("해당도서관에 제적회원으로 등록되어 있습니다.");
	}
	else if(_T("700") == transCode)
	{
		msg = _T("해당도서관에 탈퇴회원으로 등록되어 있습니다.");
	}
	else if(_T("800") == transCode)
	{
		msg = _T("해당도서관에 연체중인 자료가 있습니다.");
	}
	else if(_T("900") == transCode)
	{
		msg = _T("해당도서관에서 대출최대치를 초과하였습니다.");
	}
	else
	{
		msg = _T("알 수 없는 사유");
	}
	return msg;
}

// 16/02/01 이충무 : 2016년 국립중앙도서관 책이음개선
// 사용자명 가져오기
/*//NEW------------------------------------------------------------------------*/
CString CKL_OTHER_LOAN_OFFER::GetUserName(CString strUserNo)
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
/*//END------------------------------------------------------------------------*/

// 17/09/25 김혜영 :  상호대차 타관대출관리에서의 이용자 생년월일 필드 추가 
//*/ ADD ---------------------------------------------------------------------------
CString CKL_OTHER_LOAN_OFFER::GetBirthDay(CString strUserNo)
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
//*/ END ---------------------------------------------------------------------------

// 17/11/06 김혜영 : 상호대차 타관대출관리에서의 이용자 직급 필드 추가 
//*/ ADD ---------------------------------------------------------------------------
CString CKL_OTHER_LOAN_OFFER::GetUserClassDesc(CString strUserNo)
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
//*/ END ---------------------------------------------------------------------------

// 16/02/04 이충무 : 2016년 국립중앙도서관 책이음개선
// 운송장 출력
/*//NEW------------------------------------------------------------------------*/
BOOL CKL_OTHER_LOAN_OFFER::PrintWaybill()
{
EFS_BEGIN

	CESL_DataMgr* pSrcDM = FindDM(_T("DM_KL_OTHER_LOAN_MNG_OFFER"));
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
	
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
		return 1;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_KL_OTHER_LOAN_MNG_OFFER") , _T("GRID") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	//INT nMode = 0;
	INT nSelectCnt = pGrid->SelectGetCount();

	if(nSelectCnt <= 0)
	{
		AfxMessageBox( _T("선택 사항이 없습니다.") );
		return 1;
	}

	/*
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	*/

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_KL_OTHER_LOAN_MNG_OFFER_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
	pPrintDM->FreeData();

	const INT nCopyCnt = 10;
	// 17/11/06 김혜영 : 상호대차 타관대출관리에서의 컬럼명 한글로 교체
/*//BEFORE-----------------------------------------------------------------------------
	TCHAR *sField[nCopyCnt][2] =
	{
		{_T("USER_NO"),_T("USER_NO")},
		{_T("USER_NAME"),_T("USER_NAME")},
		{_T("REG_NO"),_T("REG_NO")},
		{_T("TITLE_INFO"),_T("TITLE_INFO")},
		{_T("PUB_INFO"),_T("PUB_INFO")},
		{_T("CALL_NO"),_T("CALL_NO")},
		{_T("SHELF_LOC_DESC"),_T("SHELF_LOC_DESC")},
		{_T("APPENDIX_RCT_YN"),_T("APPENDIX_RCT_YN")},
		{_T("APPENDIX_SEND_YN"),_T("APPENDIX_SEND_YN")},
		{_T("MEMO"),_T("MEMO")},		
	};
*///AFTER------------------------------------------------------------------------------
	TCHAR *sField[nCopyCnt][2] =
	{
		{_T("이용자번호"),_T("USER_NO")},
		{_T("이용자명"),_T("USER_NAME")},
		{_T("등록번호"),_T("REG_NO")},
		{_T("서명"),_T("TITLE_INFO")},
		{_T("발행자"),_T("PUB_INFO")},
		{_T("청구기호"),_T("CALL_NO")},
		{_T("자료실명"),_T("SHELF_LOC_DESC")},
		{_T("부록신청유무"),_T("APPENDIX_RCT_YN")},
		{_T("부록발송유무"),_T("APPENDIX_SEND_YN")},
		{_T("메모"),_T("MEMO")},		
	};
/*//END-------------------------------------------------------------------------------*/
	
	CString sData, sData2;
	INT nIndex;
	INT ids;

	pGrid->SelectGetHeadPosition();


	std::map<CString, CString> mapLib;
	std::map<CString, CString>::iterator iter;
	
	CString strDate = pSrcDM->GetDate().Left(10);
	
	for( INT i = 0 ; i < nSelectCnt ; i++)
	{
		nIndex = pGrid->SelectGetIdx();

		ids = pSrcDM->GetCellData( _T("신청도서관부호") , nIndex , sData );

		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );

		ids = pSrcDM->GetCellData( _T("신청도서관") , nIndex , sData2 );

		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );

		mapLib.insert(std::map<CString, CString>::value_type(sData, sData2));
				
		pGrid->SelectGetNext();
	}

	for(iter = mapLib.begin(); iter != mapLib.end(); iter++)
	{
		int nRowNum = 0;

		pPrintDM->FreeData();

		pGrid->SelectGetHeadPosition();

		for( i = 0 ; i < nSelectCnt ; i++ )
		{
			/*
			if( nMode == 0 )
				nIndex = i;
			else
				nIndex = pGrid->SelectGetIdx();
			*/
			nIndex = pGrid->SelectGetIdx();

			ids = pSrcDM->GetCellData( _T("신청도서관부호") , nIndex , sData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );			

			if(sData.Compare(iter->first) != 0)
			{
				pGrid->SelectGetNext();
				continue;
			}

			pPrintDM->InsertRow(-1);
			
			sData.Format( _T("%d") , nRowNum+1 );
			ids = pPrintDM->SetCellData( _T("번호") , sData , nRowNum);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );			
			
			for( INT j = 0 ; j < nCopyCnt ; j++ )
			{
				ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
				if( ids < 0 ) 
				{
					CString sEMsg;
					sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , sData );
					AfxMessageBox( sEMsg );
				}
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
				
				ids = pPrintDM->SetCellData( sField[j][1] , sData , nRowNum );
				if( ids < 0 ) 
				{
					CString sEMsg;
					sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nRowNum , sField[j][0] , sData );
					AfxMessageBox( sEMsg );
				}
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
			}

			//if( nMode == 1)
			pGrid->SelectGetNext();
			nRowNum ++;
		}

		pPrintDM->SORT(_T("USER_NO"));
		pPrintDM->SORT(_T("USER_NAME"));

		for(i=0; i<pPrintDM->GetRowCount();i++)
		{
			sData.Format( _T("%d") , i+1 );
			ids = pPrintDM->SetCellData( _T("번호") , sData , i);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );	
		}

		CString strAcqYear, strGroupInfo;
		CBasicPrintMgr PrintMgr(this);
		PrintMgr.InitPrintData();
		
		PrintMgr.AddPrintData(_T("HOLD_LIB"), m_pInfo->LIB_NAME);
		PrintMgr.AddPrintData(_T("LOAN_LIB"), iter->second);
		PrintMgr.AddPrintData(_T("SEND_DATE"), strDate);
		PrintMgr.SetPrintDM(pPrintDM);
		PrintMgr.SetPrintType(0);
		PrintMgr.SetSIReportFilename(_T("대출운송장"));
		//2019.07.17 JOB.2019.0059 ADD BY KYJ : 로그 기록시 사유 추가
		PrintMgr.SetPrivacyPrintLogParm( TRUE, m_pInfo->USER_PK, m_pInfo->USER_ID, _T("타관대출관리 대출운송장"), m_pInfo->MANAGE_CODE, _T("타관대출관리 대출운송장"),	0 );

		PrintMgr.Print();
	}	

	mapLib.clear();
	
	return TRUE;
EFS_END
		
	return FALSE;
}
/*//END------------------------------------------------------------------------*/

// 16/02/19 이충무 : 2016년 국립중앙도서관 책이음개선
// 메모수정
/*//NEW------------------------------------------------------------------------*/
void CKL_OTHER_LOAN_OFFER::ModifyMemo()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return;
	}

	CKL_OTHER_LOAN_EDIT_MEMO dlgEditMemo(m_pParent);
	dlgEditMemo.SetGrid(m_pGrid);
	dlgEditMemo.SetDM(m_pDM);

	dlgEditMemo.DoModal();

	if(dlgEditMemo.m_BModify)
		Refresh();
}
/*//END------------------------------------------------------------------------*/


// 16/03/02 이충무 : 2016년 국립중앙도서관 책이음개선
// 제공발송
/*//NEW------------------------------------------------------------------------*/
INT CKL_OTHER_LOAN_OFFER::SendBook_SendSMS(INT nIdx)
{
	INT ids=0;
	CString strUserNo;
	CString strMessage;
	CString strQuery;
	int nBookCnt = 0;
	m_pDM->GetCellData(_T("이용자번호"), nIdx, strUserNo);

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));

	CString strManageCode = m_pInfo->MANAGE_CODE;
	if (strManageCode.IsEmpty())
		strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS14'"));
	else
		strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS14' AND MANAGE_CODE='%s'"), strManageCode);
	
	TmpDM.RestructDataManager(strQuery);
	TmpDM.GetCellData(0,0,strMessage);
	
	//도서관의 SMS 사용 여부를 체크한다.
	INT nIsSMS = 0;
	nIsSMS = m_pInfo->GetSMSUseMode(); // 0:N, 1:Y
	
	if(1 == nIsSMS)
	{		
		//이용자의 SMS 사용 여부를 체크한다.
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(_T(""));
		
		CString strQuery;
		CString strUserKey;
		strQuery.Format(_T("SELECT CLU.SMS_USE_YN, CLU.HANDPHONE, CLU.NAME, CLU.REC_KEY FROM CO_LOAN_USER_TBL CLU WHERE USER_NO='%s'"),strUserNo);
		dm.RestructDataManager(strQuery);
		CString strSMSYN, strHandPhone, strName;
		dm.GetCellData(0, 0, strSMSYN);
		dm.GetCellData(0, 1, strHandPhone);
		dm.GetCellData(0, 2, strName);
		dm.GetCellData(0, 3, strUserKey);
						
		//SMS 사용 시
		if(_T("Y")==strSMSYN)
		{
			if(!m_listUserBookCnt.Lookup(strUserNo, nBookCnt))
			{
				nBookCnt= 0;
			}

			if(nBookCnt > 0)
			{
				if(IDYES == MessageBox(_T("발송 처리가 완료되었습니다.\r\nSMS를 발송하시겠습니까?"), _T("SMS발송확인"), MB_YESNO | MB_ICONQUESTION)) 
				{

					//프로시저 호출!
					CString strType = _T("SMS14");
								
					SMS_CONNECTION_INFO		SMS_ConnectionInfo;
					SMS_MSG_INFO	SMS_MsgInfo;

					CSMS_MakeMsg MakeSMS( this );
					CSMS_Env_Info SMSEnvInfo(this);
					SMSEnvInfo.MakeSMSEnv();
					SMSEnvInfo.GetSMSEnv( strType, &SMS_ConnectionInfo, &SMS_MsgInfo );

					CString strTitleInfo;
					m_pDM->GetCellData(_T("서명"), nIdx, strTitleInfo);	
					
					CString strBookCnt;
					strBookCnt.Format(_T("%d"),nBookCnt);

					MakeSMS.MakeSMSMsg( strType , &SMS_MsgInfo , strName , _T(""), strTitleInfo , strBookCnt , strMessage );

					CString strReplace = strMessage;
					strReplace.Replace(_T("'"),_T("''"));								
					CString strResult = _T("");
					// 17/11/01 김혜영 : LMS추가로 인한 처리
/*//BEFORE-----------------------------------------------------------------------------
					strQuery.Format(_T("SELECT SEND_MESSAGE FROM MN_SMS_LOG_TBL WHERE to_char(SEND_TIME, 'yyyymmdd') = to_char(SYSDATE, 'yyyymmdd') AND SEND_MESSAGE = '%s'"),strReplace);
*///AFTER------------------------------------------------------------------------------
					strQuery.Format(_T("SELECT SEND_MESSAGE FROM MN_SMS_LOG_VIEW WHERE to_char(SEND_TIME, 'yyyymmdd') = to_char(SYSDATE, 'yyyymmdd') AND SEND_MESSAGE = '%s'"),strReplace);
/*//END-------------------------------------------------------------------------------*/					
					dm.GetOneValueQuery(strQuery, strResult);

					if(strResult.Compare(_T("")) != 0)
					{
						if(IDYES == MessageBox(_T("금일 발송한 메시지입니다.\r\nSMS를 발송하시겠습니까?"), _T("SMS발송확인"), MB_YESNO | MB_ICONQUESTION)) 
						{
							ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);
						}
					}
					else
					{
						ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);
					}
				}				
			}

			if(0 > ids)
			{
				AfxMessageBox(_T("메세지 발송에 실패하였습니다."));
			}
			else
			{
				m_listUserBookCnt.RemoveKey(strUserNo);
				return 0;
			}
		}
		else
		{
			AfxMessageBox(_T("해당 이용자는 SMS 서비스를 사용하지 않습니다."));
		}
				
	}
	else
	{
		AfxMessageBox(_T("해당 도서관은 SMS 서비스를 사용하지 않습니다."));
	}
	return ids;	
}
/*//END------------------------------------------------------------------------*/


// 16/02/19 이충무 : 2016년 국립중앙도서관 책이음개선
// 이용자상세보기
/*//NEW------------------------------------------------------------------------*/
void CKL_OTHER_LOAN_OFFER::ViewLoanUser()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return;
	}

	CKL_OTHER_LOAN_USER_DETAILVIEW dlgViewLoanUser(m_pParent);
	dlgViewLoanUser.SetGrid(m_pGrid);
	dlgViewLoanUser.SetDM(m_pDM);

	dlgViewLoanUser.DoModal();	
}
/*//END------------------------------------------------------------------------*/