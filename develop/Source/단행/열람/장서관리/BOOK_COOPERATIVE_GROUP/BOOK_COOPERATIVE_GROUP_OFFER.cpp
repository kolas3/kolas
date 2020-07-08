#include "stdafx.h"
#include "resource.h"
#include "BOOK_COOPERATIVE_GROUP_OFFER.h"
#include "ILL_VIEW_BOOK_INFO_DLG.h"
#include "ILL_VIEW_LIB_INFO_DLG.h"
#include "ILL_VIEW_STATE_INFO_DLG.h"
#include "ILL_MODIFY.H"
#include "ILL_SEARCH.h"
#include "ILL_COMPARE.h"
#include "..\\BL_LOC_LOAN\\BO_LOC_3170.h"
#include "..\\BL_LOC_LOAN\\BO_LOC_3170_ReserveInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBOOK_COOPERATIVE_GROUP_OFFER::CBOOK_COOPERATIVE_GROUP_OFFER(CESL_Mgr* pParent /*=NULL*/)
	: CBookCooperativeGroup(pParent)
{
	
	m_pGrid = NULL;
	m_pDM = NULL;


	m_pLoanShareManager = NULL;
	m_pLoanMangeValue = NULL;
}


void CBOOK_COOPERATIVE_GROUP_OFFER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOOK_COOPERATIVE_GROUP_OFFER)
		
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOOK_COOPERATIVE_GROUP_OFFER, CDialog)
	//{{AFX_MSG_MAP(CBOOK_COOPERATIVE_GROUP_OFFER)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_cmb_OFFER, OnSelchangecmbOFFER)
	ON_CBN_DROPDOWN(IDC_cmb_OFFER, OnDropdowncmbOFFER)
	ON_BN_CLICKED(IDC_BTN_OFFER_STATE, OnBtnOfferState)		
	ON_BN_CLICKED(IDC_btnSelect, OnbtnSelect)	
	ON_MESSAGE(WM_SEARCH, Search)
	ON_MESSAGE(WM_EXCUTE, OnExcute)
	ON_CONTROL_RANGE(BN_CLICKED, 4500, 4505, OnBtnState)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




void CBOOK_COOPERATIVE_GROUP_OFFER::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	ResizeControl();
}

BOOL CBOOK_COOPERATIVE_GROUP_OFFER::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
 	if(0 > InitESL_Mgr(_T("BOOK_COOPERATIVE_GROUP_OFFER")))
 	{
 		AfxMessageBox(_T("ERROR : InitESL_Mgr(\"BOOK_COOPERATIVE_GROUP_OFFER\")"));
 	}
 	m_pGrid = (CESL_Grid*)FindControl(
		_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("그리드_제공자료"));
		
	
	if( !m_pInfo->m_bUserInfoExportYN )
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}	
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("책두레 제공");
	}

	m_pDM = FindDM(_T("DM_BOOK_COOPERATIVE_GROUP_OFFER"));	

	ResetComboBox();
	{ 
	
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)FindControl(
		_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("멀티콤보_진행상태"));
	if(NULL == pCMB) return FALSE;	
	
	CString strData = _T("");
	int ROW = pCMB->GetCount();
	for(int nCnt = 0; nCnt < ROW; nCnt++)
	{
		strData = pCMB->GetItemDataString(nCnt, 1);
		if(_T("요청된자료") == strData)
		{
			pCMB->SetCurSel(nCnt);
			break;
		}
	} 
	} 
	
	Refresh(_T(""), _T(""));

	m_strState[0][0] = _T("요청된자료");
	m_strState[0][1] = _T("0002");
	m_strState[1][0] = _T("발송된자료");
	m_strState[1][1] = _T("0004");
	m_strState[2][0] = _T("입수된자료");
	m_strState[2][1] = _T("0014");
	m_strState[3][0] = _T("복귀중자료");
	m_strState[3][1] = _T("0008");
	m_strState[4][0] = _T("발송거절된자료");
	m_strState[4][1] = _T("0011");

	INT nCnt = 0;
	INT nBtnStateCnt = 5;
	for(nCnt=0; nCnt < nBtnStateCnt; nCnt++)
	{
		m_pBtnState[nCnt]->Create(
			m_strState[nCnt][0], 
			WS_VISIBLE, 
			CRect(10, 50+(30*nCnt)+(15*nCnt), 160, 50+(30*nCnt)+(15*nCnt)+30),
			this, 
			4500+nCnt);
		m_pBtnState[nCnt]->SetFont(GetFont());
	}	


	return TRUE;  
	              
}

BOOL CBOOK_COOPERATIVE_GROUP_OFFER::Create(CWnd *pParent)
{
	return CDialog::Create(IDD, pParent);
}

BOOL CBOOK_COOPERATIVE_GROUP_OFFER::ResizeControl()
{	
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_grid_OFFER);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return FALSE;
	
	CRect rDlg = CRect(0, 0, 0, 0);
	GetClientRect(rDlg);

	UINT dwFlag = SW_HIDE;
	CString strCaption;
	GetDlgItemText(IDC_BTN_OFFER_STATE, strCaption);
	if(_T("▲") == strCaption)
	{
		rDlg.left += 175;
		dwFlag = SW_SHOW;
	}
	else if(_T("▼") == strCaption)
	{
		dwFlag = SW_HIDE;
	}

	INT nCnt = 0;
	INT nBtnStateCnt = 5;
	for(nCnt=0; nCnt < nBtnStateCnt; nCnt++)
	{
		m_pBtnState[nCnt]->ShowWindow(dwFlag);
	}

	rDlg.top += 40;
	pWnd->MoveWindow(rDlg);

	return TRUE;
}

void CBOOK_COOPERATIVE_GROUP_OFFER::OnSelchangecmbOFFER() 
{
	
 	Refresh(_T(""), _T(""));
}

int CBOOK_COOPERATIVE_GROUP_OFFER::View_BookInfo()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() == 0)
	{
		m_pGrid->SelectRemoveAll();
		INT nRows = m_pDM->GetRowCount();		
		for (int i = 0; i < nRows; i++)
		{
			m_pGrid->m_arraySelect.AddTail(i);
		}
		int nIdx = m_pGrid->GetIdx();
		if(0 > nIdx) nIdx = 0;
		m_pGrid->m_arrayPos = m_pGrid->m_arraySelect.FindIndex(nIdx);
	}
 	int nCnt = m_pGrid->SelectGetCount();
 	if(0 >= nCnt) 
 	{
 		AfxMessageBox(_T("보여줄 정보가 존재하지 않습니다."));
 		return -1;
 	}

	CILL_VIEW_BOOK_INFO_DLG Dlg(this);
	Dlg.m_pInfo = m_pInfo;
	Dlg.m_pParentDM = m_pDM;
	Dlg.m_pParentGrid = m_pGrid;
	Dlg.m_nIdx = m_pGrid->SelectGetIdx();
	Dlg.m_bIsShowChangeRegNo = TRUE;
	Dlg.m_sBookCooperativeMode = m_sBookCooperativeMode;
	Dlg.m_pLoanShareManager = m_pLoanShareManager;	
	Dlg.m_strState = _T("제공");

	Dlg.DoModal();
		
	Refresh();
	RefreshState();

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::ExcutePath(CString strSM)
{
	int ids = -1;

	if( _T("제공_발송") == strSM)
	{
		m_pGrid->SelectMakeList();

		CString strMessage;
		CDataMgr DataMgr;
		ids = PreSend(&DataMgr, strMessage);
		if(0 > ids)
		{
			return ids;
		}
		ids = Send(&DataMgr, &strMessage);
		if(0 > ids)
		{
			return ids;
		}

		Refresh();	
		RefreshState();

		return ids;
	}
	else if( _T("제공_발송거절") == strSM)
	{
		ids = Send_Refuse();
		Refresh();	
		RefreshState();

		return ids;
	}
	else if( _T("제공_발송취소") == strSM)
	{
		ids = Send_Cancel();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("제공_복귀(완료)") == strSM)
	{
		m_pGrid->SelectMakeList();

		ids = Return_Complete();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("제공_취소") == strSM)
	{
		ids = Cancel();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("제공_서지정보") == strSM)
	{
		return View_BookInfo();
	}
	else if( _T("제공_이용자정보") == strSM)
	{
		return View_UserInfo();
	}
	else if( _T("제공_상대기관정보") == strSM)
	{
		return View_LibInfo();
	}
	else if( _T("제공_진행정보") == strSM)
	{
		return View_StateInfo();
	}
	else if(_T("일괄변경") == strSM)
	{
  		CILL_MODIFY Dlg;
  		Dlg.m_pInfo = m_pInfo;
  		Dlg.m_pParentDM = m_pDM;
  		Dlg.m_pParentGrid = m_pGrid;			
  		Dlg.m_strLibCode = m_strLibCode;
		Dlg.m_pLoanShareManager = m_pLoanShareManager;
		Dlg.m_sBookCooperativeMode = m_sBookCooperativeMode;

  		Dlg.DoModal();

		m_pGrid->SetProtectSelectColumn(TRUE);
		Refresh();		
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
	else if(_T("소장여부조사") == strSM)
	{
		ids = Compare();
	}
	else if(_T("RFID보안설정") == strSM)
	{
		ids = RFID_SecurityChange();
	}	
	else if( _T("목록출력") == strSM)
	{			
		if( !m_pInfo->m_bUserInfoExportYN )
		{
			AfxMessageBox(_T("개인정보 출력이 제한되어있습니다."));
			return 0;
		}
		ids = PrintList();
		return ids;
	}
	else if(_T("제공_발송거절취소") == strSM)
	{
		ids = Send_Refuse_Cancel();
		Refresh();	
		RefreshState();
		return ids;
	}	
	else if( _T("제공_도서발송정보출력") == strSM)	
	{						
		if( !m_pInfo->m_bUserInfoExportYN )
		{
			AfxMessageBox(_T("개인정보 출력이 제한되어있습니다."));
			return 0;
		}
		ids = PrintOfferList();
		return ids;
	}

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::View_LibInfo()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() == 0)
	{
		m_pGrid->SelectRemoveAll();
		INT nRows = m_pDM->GetRowCount();		
		for (int i = 0; i < nRows; i++)
		{
			m_pGrid->m_arraySelect.AddTail(i);
		}
		int nIdx = m_pGrid->GetIdx();
		if(0 > nIdx) nIdx = 0;
		m_pGrid->m_arrayPos = m_pGrid->m_arraySelect.FindIndex(nIdx);
	}
 	int nCnt = m_pGrid->SelectGetCount();
 	if(0 >= nCnt)
 	{
 		AfxMessageBox(_T("보여줄 정보가 존재하지 않습니다."));
 		return -1;
 	}	

	CILL_VIEW_LIB_INFO_DLG Dlg;
	Dlg.m_pParentDM = m_pDM;
	Dlg.m_pParentGrid = m_pGrid;
	Dlg.m_strLibAlias = _T("요청도서관_코드");
	Dlg.m_nIdx = m_pGrid->SelectGetIdx();

	Dlg.DoModal();

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::PreSend(CDataMgr* pDataMgr, CString& strMessage)
{
		
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1000;
	}

	CStringArray aCheckArray;
	aCheckArray.Add(_T("상태|!=|0002"));
	aCheckArray.Add(_T("제공도서관_코드|!=|%m_strLibCode%"));

	
	UnCheckGrid(aCheckArray);
	}
		
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 발송 가능한 항목이 없습니다.\r\n")
			_T("발송 가능한 대상은 아래와 같습니다.\r\n\r\n")
			_T("1. 제공도서관이 자관인 경우\r\n")
			_T("2. 진행상태가 요청된자료(0002)인 트랜잭션\r\n")
			);
		return -1010;
	}
	else
	{
		if(NULL == pDataMgr)
		{
			return -1020;
		}

		CString strGet;
		INT nSetIdx = 0;
		INT nIdx = m_pGrid->SelectGetHeadPosition();
		INT nIdx2 = 0;
		do
		{	
			pDataMgr->InsertData(-1);				
			nSetIdx = pDataMgr->GetCount()-1;
			m_pDM->GetCellData(_T("트랜잭션번호"), nIdx, strGet);			
			pDataMgr->SetData(_T("트랜잭션번호"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("책KEY"), nIdx, strGet);
			pDataMgr->SetData(_T("책KEY"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("이용자KEY"), nIdx, strGet);			
			pDataMgr->SetData(_T("이용자KEY"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("발행형태구분"), nIdx, strGet);
			pDataMgr->SetData(_T("발행형태구분"), strGet, nSetIdx);

			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);

	}

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Send(CDataMgr* pDataMgr, CString* pstrMessage)
{
	INT ids = 0;
 	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	if(_T("I") == m_sBookCooperativeMode)
	{
		CString strNO;
				
		MakeTransNoList(pDataMgr, strNO, TRUE);


		if(_T("") == strNO)
		{
			return -1000;
		}
				
		CString strDate, strTemp;
		pDataMgr->GetData(_T("발송일"), 0, strTemp); 
		if(_T("SYSDATE") != strTemp && _T("") != strTemp)
		{			
			strDate.Format(_T("TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS')"), strTemp);
		}
		
		
		else if(_T("SYSDATE") != strTemp && _T("") == strTemp)
		{
			strDate = _T("SYSDATE");
		}
		else
		{
			strDate = strTemp;
		}

		TmpDM.StartFrame();
		CString strQuery;
		strQuery.Format(
			_T("UPDATE ILL_TRANS_TBL ITL SET ")
			_T("ITL.STATE = '0004', ")
			_T("ITL.GIVE_USERID = '%s', ") 
			
			
			_T("ITL.GIVE_DATE = %s, ")

			_T("ITL.WORKNO = ")
				_T("(SELECT ")
				_T("CASE ITL2.PUBLISH_FORM_CODE ")
				_T("WHEN 'MO' THEN (SELECT BB.WORKNO FROM BO_BOOK_TBL BB WHERE BB.REC_KEY = ITL2.BOOK_KEY) ")
				_T("WHEN 'SE' THEN (SELECT BB.WORKNO FROM SE_BOOK_TBL BB WHERE BB.REC_KEY = ITL2.BOOK_KEY) ")
				_T("END ")
				_T("FROM ILL_TRANS_TBL ITL2 ")
				_T("WHERE ITL2.TRANS_NO = ITL.TRANS_NO), ")
			_T("ITL.LAST_WORK = '%s' ")
			_T("WHERE ")
			_T("ITL.TRANS_NO IN (%s) AND ITL.STATE = '0002' AND ITL.GIVE_LIBCODE = '%s';"),
			m_pInfo->USER_ID,
			
			
			strDate,
			GetWorkLogMsg( _T("발송"), __WFILE__, __LINE__ ),
			strNO,
			m_strLibCode
			);
	
		TmpDM.AddFrame(strQuery);
		ids = TmpDM.SendFrame();
		TmpDM.EndFrame();

		if(0 > ids)
		{
			pstrMessage->Format(_T("DB 업데이트에 실패하였습니다.[%d]"), ids);
			return ids;
		}

	}
	else if(_T("D") == m_sBookCooperativeMode)
	{
		
		CString strLoanKey, strGet, strTransNo;


		const INT COUNT = pDataMgr->GetCount();
		for(INT nIdx = 0; nIdx < COUNT; nIdx++)
		{	

			pDataMgr->GetData(_T("트랜잭션번호"), nIdx, strGet);

			TmpDM.StartFrame();			

			StruMakeInsertTbl Stru;
			Stru.pDM = &TmpDM;
			
			TmpDM.MakeRecKey(strLoanKey);
			Stru.strInLoanKey= strLoanKey;


			pDataMgr->GetData(_T("책KEY"), nIdx, strGet);
			Stru.strInBookKey = strGet;


			pDataMgr->GetData(_T("이용자KEY"), nIdx, strGet);
			Stru.strInUserKey = strGet;
			

			pDataMgr->GetData(_T("발행형태구분"), nIdx, strGet);
			Stru.strInPublishFormCode = strGet;

			Make_InsertLoanTblQuery(&Stru);

			CString strQuery;
			strQuery.Format(
				_T("UPDATE ILL_TRANS_TBL ITL SET ")
				_T("ITL.STATE = '0004', ")
				_T("ITL.GIVE_USERID = '%s', ") 
				_T("ITL.GIVE_DATE = SYSDATE, ")
				_T("ITL.LOAN_KEY = %s, ")
				_T("ITL.WORKNO = ")
					_T("(SELECT ")
					_T("CASE ITL2.PUBLISH_FORM_CODE ")
					_T("WHEN 'MO' THEN (SELECT BB.WORKNO FROM BO_BOOK_TBL BB WHERE BB.REC_KEY = ITL2.BOOK_KEY) ")
					_T("WHEN 'SE' THEN (SELECT BB.WORKNO FROM SE_BOOK_TBL BB WHERE BB.REC_KEY = ITL2.BOOK_KEY) ")
					_T("END ")
					_T("FROM ILL_TRANS_TBL ITL2 ")
					_T("WHERE ITL2.TRANS_NO = ITL.TRANS_NO), ")
				_T("ITL.LAST_WORK = '%s' ")
				_T("WHERE ")
				_T("ITL.TRANS_NO = '%s' AND ITL.STATE = '0002' AND ITL.GIVE_LIBCODE = '%s';"),
				m_pInfo->USER_ID,
				strLoanKey,
				GetWorkLogMsg( _T("발송"), __WFILE__, __LINE__ ),
				strTransNo,
				m_strLibCode
				);

			TmpDM.AddFrame(strQuery);
			ids = TmpDM.SendFrame();
			TmpDM.EndFrame();

			if(0 > ids)
			{
				pstrMessage->Format(_T("DB 업데이트에 실패하였습니다.[%d]"), ids);
				return ids;
			}
			
			ShareInfoCenterUpload(strLoanKey, _T("I"), _T("LH_STORAGE_V01"), _T(""));
			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,GIVE_USERID,GIVE_DATE,LOAN_KEY,WORKNO"));

		}
	}
	if(0 > ids)
	{
		return ids;
	}

	CString strNO;
	MakeTransNoList(pDataMgr, strNO);
	

	if(0 != m_pGrid->SelectGetCount())
	{
		return RFID_SecurityChange(strNO, FALSE, TRUE);
	}
	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Send_Cancel()
{
	
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}

	CStringArray aCheckArray;
	aCheckArray.Add(_T("상태|!=|0004"));
	aCheckArray.Add(_T("제공도서관_코드|!=|%m_strLibCode%"));
	
	UnCheckGrid(aCheckArray);
	}	
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 발송취소 가능한 항목이 없습니다.\r\n")
			_T("발송취소 가능한 대상은 아래와 같습니다.\r\n\r\n")
			_T("1. 제공도서관이 자관인 경우\r\n")
			_T("2. 진행상태가 발송된자료(0004)인 트랜잭션\r\n")
			);
		return -1;
	}

	CString strNO = Get_Select_TRANS_NO_List(TRUE);

	if(_T("") == strNO) return -1;

 	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	if(_T("I") == m_sBookCooperativeMode)
	{
		TmpDM.StartFrame();

		CString strQuery;
		strQuery.Format(
			_T("UPDATE ILL_TRANS_TBL SET ")
			_T("LOAN_KEY = NULL, ")
			_T("STATE = '0002', ")
			_T("GIVE_USERID = '%s', ") 
			_T("GIVE_DATE = SYSDATE, ")
			_T("WORKNO = NULL, ")
			_T("LAST_WORK = '%s' ")
			_T("WHERE ")
			_T("TRANS_NO IN (%s) AND STATE = '0004' AND GIVE_LIBCODE = '%s';"),
			m_pInfo->USER_ID,
			GetWorkLogMsg( _T("발송취소"), __WFILE__, __LINE__ ),
			strNO,
			m_strLibCode
			);

		TmpDM.AddFrame(strQuery);
		int ids = TmpDM.SendFrame();
		TmpDM.EndFrame();
		

	}	
	else if(_T("D") == m_sBookCooperativeMode)
	{

		CString strLoanKey, strBookKey, strUserKey, strTransNo;
		int nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{	
			m_pDM->GetCellData(_T("트랜잭션번호"), nIdx, strTransNo);
			m_pDM->GetCellData(_T("대출정보_키"), nIdx, strLoanKey);

			TmpDM.StartFrame();				
			CString strQuery;
			strQuery.Format(
				_T("UPDATE ILL_TRANS_TBL SET ")
				_T("STATE = '0002', ")
				_T("GIVE_USERID = '%s', ") 
				_T("GIVE_DATE = SYSDATE, ")
				_T("LOAN_KEY = NULL, ")
				_T("WORKNO = NULL, ")
				_T("LAST_WORK = '%s' ")
				_T("WHERE ")
				_T("TRANS_NO = '%s' AND STATE = '0004' AND GIVE_LIBCODE = '%s';"),
				m_pInfo->USER_ID,
				GetWorkLogMsg( _T("발송취소"), __WFILE__, __LINE__ ),
				strTransNo,
				m_strLibCode
				);

			TmpDM.AddFrame(strQuery);

			TmpDM.MakeDeleteFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey);

			int ids = TmpDM.SendFrame();
			TmpDM.EndFrame();

			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,GIVE_USERID,GIVE_DATE"));

			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);

	}	
	strNO = Get_Select_TRANS_NO_List();		
	
	return RFID_SecurityChange(strNO, TRUE, TRUE);

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Send_Refuse()
{
	int ids = 0;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_OFFER::Send_Refuse()");
	
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}

	CStringArray aCheckArray;
	aCheckArray.Add(_T("상태|!=|0002"));
	aCheckArray.Add(_T("제공도서관_코드|!=|%m_strLibCode%"));

	
	ids = UnCheckGrid(aCheckArray);
	}
	
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 발송거절 가능한 항목이 없습니다.\r\n")
			_T("발송거절 가능한 대상은 아래와 같습니다.\r\n\r\n")
			_T("1. 제공도서관이 자관인 자료\r\n")
			_T("2. 진행상태가가 요청된자료(0002)인 트랜잭션")			
			);
		return -1;
	}
	
	CREFUSE_DLG Dlg;
	Dlg.m_DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	Dlg.m_DM.SetManageCode(m_pInfo->MANAGE_CODE);
	Dlg.m_strClass = _T("i03");	
	if(IDOK != Dlg.DoModal()) { return -2; }
	
	CString strGIVE_CANCEL = Dlg.m_strCancelCode;
	
	CString strContent = Dlg.m_strContent;

 	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	int nIdx = m_pGrid->SelectGetHeadPosition();
	do
	{	
		TmpDM.StartFrame();
		CString strTransNo = _T("");
		
		{
		ids = m_pDM->GetCellData(_T("트랜잭션번호"), nIdx, strTransNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);		
		
		CString strLastWork = GetWorkLogMsg( _T("발송거절"), __WFILE__, __LINE__ );
		
		CString strQuery;		
		strQuery.Format(		
			_T("UPDATE ILL_TRANS_TBL SET ")		
			_T("STATE = '0011', ")		
			_T("GIVE_USERID = '%s', ") 
			_T("GIVE_DATE = SYSDATE, ")		
			_T("GIVE_CANCEL = '%s', ")	
			_T("GIVE_CONTENT = '%s', ")	
			_T("LAST_WORK = '%s' ")		
			_T("WHERE ")			
			_T("TRANS_NO = '%s';"),
			m_pInfo->USER_ID,	
			strGIVE_CANCEL,	
			strContent,		
			strLastWork,		
			strTransNo	

			);		
		TmpDM.AddFrame(strQuery);
		}
				
		{

		CString strQuery = _T("");
		ids = Get_UpdateBookTblQuery_For_CompleteTransaction(
			nIdx,
			GetWorkLogMsg( _T("발송거절"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);
		}
		TmpDM.AddFrame(strQuery);

		ids = Get_UpdateIdxTblQuery(
			nIdx,
			GetWorkLogMsg( _T("발송거절"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids)
		{
			return ids;
		}
		else if(0 == ids)
		{
			TmpDM.AddFrame(strQuery);
		}

		}		

		ids = TmpDM.SendFrame(TRUE);
		TmpDM.EndFrame();

		if(0 > ids)
		{
			return ids;
		}
		
		if(_T("D") == m_sBookCooperativeMode)
		{
			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,GIVE_USERID,GIVE_DATE,GIVE_CANCEL,GIVE_CONTENT"));	
		}

		Reversion(nIdx);

		nIdx = m_pGrid->SelectGetNext();
	}while(0 <= nIdx);

	return ids;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Return_Complete()
{
	int ids = 0;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_OFFER::Return_Complete()");
	CString strQuery = _T("");
	
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}

	CStringArray aCheckArray;
	aCheckArray.Add(_T("상태|NOT IN|0004,0008,0014"));
	aCheckArray.Add(_T("대출상태|==|대출중"));
	aCheckArray.Add(_T("제공도서관_코드|!=|%m_strLibCode%"));

	
	ids = UnCheckGrid(aCheckArray);
	}
		
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 복귀(완료) 가능한 항목이 없습니다.\r\n")
			_T("복귀(완료) 가능한 대상은 아래와 같습니다.\r\n\r\n")
			_T("1. 자관에서 제공한 자료\r\n")
			_T("2. 진행상태가 발송된자료(0004), 입수된자료(0014), 복귀중자료(0008)인 트랜잭션\r\n")
			_T("3. 현재 대출중이 아닌 자료")
			);
		return -1;
	}

	CString strMessage;
	strMessage.Format(
		_T("선정된 자료들을 복귀(완료) 처리합니다.\r\n")
		_T("진행상태는 완료된자료(0009)로 변경되며 \r\n")
		_T("책의 상태는 책두레대상자료에서 이전상태로 복귀됩니다.\r\n")
		_T("계속하시겠습니까?\r\n\r\n")
		_T("[제공된 도서를 받은 경우에만 '예'를 선택하십시요.]\r\n")
		);
	UINT uResult = AfxMessageBox(strMessage, MB_YESNO);
	if(IDNO == uResult) return -1;

	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	SetCheckAppendixAndMother();
	m_pGrid->SelectMakeList();

	int nIdx = m_pGrid->SelectGetHeadPosition();
	do
	{
		TmpDM.StartFrame();
		CString strTransNo = _T("");
		
		{
		
		ids = m_pDM->GetCellData(_T("트랜잭션번호"), nIdx, strTransNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

		CString strQuery = _T("");
		
		strQuery.Format(
			_T("UPDATE ")
				_T("ILL_TRANS_TBL ")
			_T("SET ")
				_T("STATE = '0009', ")	
				_T("END_USERID = '%s', ") 
				_T("END_DATE = SYSDATE, ")
				_T("END_DIV = '0010', ") 
				_T("LAST_WORK = '%s' ")
			_T("WHERE ")
				_T("TRANS_NO = '%s';"),
				m_pInfo->USER_ID,
				GetWorkLogMsg( _T("복귀(완료)"), __WFILE__, __LINE__ ),
				strTransNo
			);
		TmpDM.AddFrame(strQuery);
		}
				
		{
		
		ids = Get_UpdateBookTblQuery_For_CompleteTransaction(
			nIdx,
			GetWorkLogMsg( _T("복귀(완료)"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);
		TmpDM.AddFrame(strQuery);
		
		ids = Get_UpdateIdxTblQuery(
			nIdx,
			GetWorkLogMsg( _T("복귀(완료)"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids)
		{
			return ids;
		}
		else if(0 == ids)
		{
			TmpDM.AddFrame(strQuery);
		}

		}
	
		Reversion(nIdx);
		
		nIdx = m_pGrid->SelectGetNext();
	}while(0 <= nIdx);


	CString strNO = Get_Select_TRANS_NO_List();
	RFID_SecurityChange(strNO, TRUE, TRUE);

	return 0;
}


VOID CBOOK_COOPERATIVE_GROUP_OFFER::KL_Transaction_end(INT nIdx) 
{
	
	CString strQuery, strGetData, strLoanKey;
	CString strKLUserYN, strBookAppendixFlag;
	
	CESL_DataMgr TmpDM;
	INT ids = 0;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	m_pDM->GetCellData(_T("책이음회원여부"), nIdx, strKLUserYN);
	m_pDM->GetCellData(_T("부록여부"), nIdx, strBookAppendixFlag);
	
	m_pDM->GetCellData(_T("대출정보_키"), nIdx, strLoanKey);

	
	if(_T("Y") == strKLUserYN && strBookAppendixFlag != _T("V") && strLoanKey.IsEmpty() == FALSE)
	{
		ids = m_pDM->GetCellData(_T("이용자_번호"), nIdx, strGetData);

		strQuery.Format(_T("SELECT TO_CHAR(LOAN_STOP_DATE,'YYYY/MM/DD HH24:MI:SS') AS LOAN_STOP_DATE, TO_CHAR(LOAN_STOP_SET_DATE,'YYYY/MM/DD HH24:MI:SS') AS LOAN_STOP_SET_DATE ")
						_T("FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s'"), strGetData);
		TmpDM.RestructDataManager(strQuery);

		CFrameWriter Writer(m_pInfo);
		Writer.setCommand(_T("IL_K11_LOC_B01_SERVICE"));
		Writer.addRecord();
		
		Writer.addElement(_T("통합대출자번호"), strGetData);
		Writer.addElement(_T("대출도서관부호"), m_pInfo->LIB_CODE);
		Writer.addElement(_T("반납도서관부호"), m_pInfo->LIB_CODE);
		ids = m_pDM->GetCellData(_T("반납일_시간"), nIdx, strGetData);
		Writer.addElement(_T("반납일"), strGetData);

		CString strRegNo;
		ids = m_pDM->GetCellData(_T("등록번호"), nIdx, strRegNo);
		Writer.addElement(_T("등록번호"), strRegNo);

		Writer.addElement(_T("상태"), _T("1"));
		ids = m_pDM->GetCellData(_T("대출정보_키"), nIdx, strGetData);
		Writer.addElement(_T("로컬KEY"), strGetData);
		if(TmpDM.GetCellData(0, 0) != _T(""))
		{
			Writer.addElement(_T("회원상태"), _T("1"));
			Writer.addElement(_T("대출정지일"), TmpDM.GetCellData(0, 0));
			Writer.addElement(_T("대출정지부여일"), TmpDM.GetCellData(0, 1));
		}
		else
		{
			Writer.addElement(_T("회원상태"), _T("0"));
		}
		Writer.addElement(_T("지역내상호대차상태"), _T("Y"));

		CFrameSender Sender(m_pInfo);
		Sender.SendTempTbl(Writer.getFrame(), GetWorkLogMsg( _T("책두레복귀(완료)"), __WFILE__, __LINE__ ));
	}
}

BOOL CBOOK_COOPERATIVE_GROUP_OFFER::PreTranslateMessage(MSG* pMsg) 
{
	
	if(WM_KEYDOWN == pMsg->message)
	{
		
		if(VK_ESCAPE == pMsg->wParam)

		{
			return TRUE;
		}
		else if(VK_F10 == pMsg->wParam)
		{	
			PrintList();
			return TRUE;
		}
		
		
		else if( VK_RETURN == pMsg->wParam )
		{
			HWND MsgH = pMsg->hwnd;
			
			if( GetDlgItem(IDC_edtRegNo)->m_hWnd==MsgH )
			{
				OnbtnSelect();
			}
			
			return TRUE;
		}

	}
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Cancel()
{
	int ids = 0;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_OFFER::Cancel()");

	CString strMessage;
	strMessage.Format(
		_T("선정된 트랜잭션을 종료시킵니다.\r\n")
		_T("종료된 트랜잭션은 진행사항의 변동이 불가능하며 \r\n")
		_T("책의 상태는 책두레대상자료에서 이전상태로 복귀됩니다.\r\n")
		_T("계속하시겠습니까?")
		);
	UINT uResult = AfxMessageBox(strMessage, MB_YESNO);
	if(IDNO == uResult) return -1;

	
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}
		
	SetCheckAppendixAndMother();
	m_pGrid->SelectMakeList();

	CStringArray aCheckArray;
	aCheckArray.Add(_T("완료유형|!=|%NULL%"));
	if(_T("I") == m_sBookCooperativeMode)
	{
		aCheckArray.Add(_T("대출정보_키|!=|%NULL%"));
	}
	else if(_T("D") == m_sBookCooperativeMode)
	{
		aCheckArray.Add(_T("대출일|!=|%NULL%"));
	}
	aCheckArray.Add(_T("제공도서관_코드|!=|%m_strLibCode%"));

	
	ids = UnCheckGrid(aCheckArray);
	}
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 취소 가능한 항목이 없습니다.\r\n")
			_T("취소가 불가능한 항목은 다음과 같습니다.\r\n\r\n")
			_T("1. 이미 완료된 항목\r\n")
			_T("  1.1 완료된 자료\r\n")
			_T("  1.2 요청거절된 자료\r\n")
			_T("  1.3 발송거절확인된 자료\r\n")
			_T("  1.4 신청취소된 자료\r\n")
			_T("  1.5 취소된 자료\r\n")
			_T("2. 대출이 일어난 이후의 자료\r\n")
			_T("  2.1 입수된 자료중 대출이 일어난 자료\r\n")
			_T("  2.2 입수된 자료중 대출/반납이 일어난 자료\r\n")
			_T("  2.3 복귀중 자료\r\n")
			_T("3. 제공도서관이 자관이 아닌 자료")
			);
		return -1;
	}
	
	CREFUSE_DLG Dlg;
	Dlg.m_DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	Dlg.m_DM.SetManageCode(m_pInfo->MANAGE_CODE);
	Dlg.m_strClass = _T("i04");		
	Dlg.m_strCaption = _T("취소사유");
	Dlg.m_strCauseCaption = _T("취소사유");

	if(IDOK != Dlg.DoModal()) { return -2; }
	
	CString strEndDiv = Dlg.m_strCancelCode;
	
	CString strEndContent = Dlg.m_strContent;
	
	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	CString strLoanKey;
	int nIdx = m_pGrid->SelectGetHeadPosition();
	do
	{	
		TmpDM.StartFrame();
		CString strTransNo = _T("");
		
		{
		
		ids = m_pDM->GetCellData(_T("트랜잭션번호"), nIdx, strTransNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

		CString strQuery = _T("");
		
		strQuery.Format(
			_T("UPDATE ")
				_T("ILL_TRANS_TBL ")
			_T("SET ")
				_T("LOAN_KEY = NULL, ")
				_T("STATE = '0009', ")	
				_T("END_USERID = '%s', ") 
				_T("END_DATE = SYSDATE, ")
				_T("END_DIV = '%s', ") 
				_T("END_CONTENT= '%s', ")
				_T("LAST_WORK = '%s' ")
			_T("WHERE ")
				_T("TRANS_NO = '%s';"),

				m_pInfo->USER_ID,
				strEndDiv,
				strEndContent,
				GetWorkLogMsg( _T("취소"), __WFILE__, __LINE__ ),
				strTransNo

			);
		TmpDM.AddFrame(strQuery);
		}
		
		
		{

		CString strQuery = _T("");
		ids = Get_UpdateBookTblQuery_For_CompleteTransaction(
			nIdx,
			GetWorkLogMsg( _T("취소"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);

		TmpDM.AddFrame(strQuery);

		ids = Get_UpdateIdxTblQuery(
			nIdx,
			GetWorkLogMsg( _T("취소"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids)
		{
			return ids;
		}
		else if(0 == ids)
		{
			TmpDM.AddFrame(strQuery);
		}

		}
		
		if(_T("D") == m_sBookCooperativeMode)
		{
			m_pDM->GetCellData(_T("대출정보_키"), nIdx, strLoanKey);
			if(FALSE == strLoanKey.IsEmpty())
			{
				TmpDM.MakeDeleteFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey);
			}
		}
		ids = TmpDM.SendFrame(TRUE);
		TmpDM.EndFrame();

		if(0 > ids)
		{
			return ids;
		}

		if(_T("D") == m_sBookCooperativeMode)
		{
			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,END_USERID,END_DATE,END_DIV,END_CONTENT"));
		}		

		Reversion(nIdx);

		nIdx = m_pGrid->SelectGetNext();
	}while(0 <= nIdx);

	return 0;
}
int CBOOK_COOPERATIVE_GROUP_OFFER::Reversion(int nIdx)
{
	EFS_BEGIN

	if(NULL == m_pDM) return -1;
	if(0 > nIdx || m_pDM->GetRowCount() <= nIdx) return -2;

	CString strPublishFormCode;
	m_pDM->GetCellData(_T("발행형태구분"), nIdx, strPublishFormCode);

	CString strManageCode;
	m_pDM->GetCellData(_T("제공도서관_관리구분"), nIdx, strManageCode);

 	CString strRegNo;
 	m_pDM->GetCellData(_T("등록번호"), nIdx, strRegNo);

 	CString strBookKey;
 	m_pDM->GetCellData(_T("책KEY"), nIdx, strBookKey);

	CString strVolInfoColumn;
	CString strBookTbl;
	CString strReverseUserCnt;
	if(_T("MO") == strPublishFormCode)
	{
		strBookTbl = _T("BO_BOOK_TBL BB");
		strVolInfoColumn = _T("VOL_SORT_NO");
		strReverseUserCnt.Format(
			_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2 WHERE CL2.BOOK_KEY = BB.REC_KEY AND CL2.STATUS = '3' )"));
	}
	else if(_T("SE") == strPublishFormCode)
	{
		strBookTbl = _T("SE_BOOK_TBL BB");
		strVolInfoColumn = _T("VOL_KEY");
		strReverseUserCnt.Format(
			_T("(SELECT COUNT(*) FROM LS_WORK_T01 CL2 WHERE CL2.BOOK_KEY = BB.REC_KEY AND CL2.STATUS = '3' )"));
	}
	else if(_T("NB") == strPublishFormCode)
	{
		return 1;
	}
	else return -3;
	
	CString strQuery;
	strQuery.Format(
		_T("SELECT BB.SPECIES_KEY, %s, %s ")
		_T("FROM %s ")
		_T("WHERE BB.REC_KEY = %s AND BB.MANAGE_CODE = '%s' AND ROWNUM = 1"),

		strVolInfoColumn,
		strReverseUserCnt,
		strBookTbl,
		strBookKey,
		strManageCode
		);

	int ids = -1;

	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	ids = TmpDM.RestructDataManager(strQuery);
	
	CString strSpeciesKey;
	TmpDM.GetCellData(0, 0, strSpeciesKey);
	
	CString strVolInfo;
 	TmpDM.GetCellData(0, 1, strVolInfo);
	
	CString strReversionCnt;
	TmpDM.GetCellData(0, 2, strReversionCnt);

		
	if ( m_pLoanMangeValue->m_bVolReserve && FALSE == strVolInfo.IsEmpty())
	{		
		CString sQuery;
		sQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL ") 
			_T(" WHERE CL.BOOK_KEY IN ( SELECT REC_KEY FROM %s WHERE SPECIES_KEY = %s ) ")
			_T("   AND CL.REG_NO IS NULL ")
			_T("   AND CL.STATUS = '3' ")
			_T("   AND CL.VOL_INFO = %s "),
			strBookTbl, strSpeciesKey, strVolInfo);

		TmpDM.GetOneValueQuery(sQuery, strReversionCnt);
	}	
	
	if( 0 == _ttoi(strReversionCnt))  return 0;
	if ( !(m_pLoanMangeValue->m_bVolReserve) )
	{
		CString sSQL;
		sSQL.Format( _T("SELECT REC_KEY FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3' AND RESERVATION_DATE = ")
			_T("(SELECT MIN(RESERVATION_DATE) FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3')") , strBookKey, strBookKey);
		
		CString sLoanKey;
		TmpDM.GetOneValueQuery(sSQL, sLoanKey);
		
		
		if( sLoanKey.IsEmpty() )
			return 0;
	}
	
	INT nYes = MessageBox(_T("이 자료는 예약된 자료입니다. 예약 처리를 하시겠습니까?"), _T("예약 처리"), MB_YESNO | MB_ICONQUESTION);
	if( nYes == IDYES )
	{		
		if(!(m_pLoanMangeValue->m_bVolReserve))
		{
			CBO_LOC_3170 Dlg(this, m_pLoanShareManager , m_pLoanMangeValue , 0);
			Dlg.pMain = pMain;
			Dlg.pParentMgr = (CESL_Mgr*)this;
			Dlg.m_pInfo = m_pInfo;
			Dlg.SetInfo(strSpeciesKey, strVolInfo, strPublishFormCode, strBookKey);
			Dlg.m_nReserveInfoMode	=	1;
			Dlg.DoModal();
		}
		else 
		{
			CBO_LOC_3170_ReserveInfo Dlg(this, m_pLoanShareManager , m_pLoanMangeValue , 0);
			Dlg.pMain = pMain;
			Dlg.pParentMgr = (CESL_Mgr*)this;
			Dlg.m_pInfo = m_pInfo;
			Dlg.SetInfo(strSpeciesKey, strVolInfo, strPublishFormCode, _T(""));
			Dlg.m_nReserveInfoMode	=	1;		
			Dlg.m_strReturnBookKey	=	strBookKey;
			Dlg.m_strReturnBookRegNo	=	strRegNo;			

			Dlg.DoModal();
		}
	}
	
	return 0;

	EFS_END
		return -1;
}


int CBOOK_COOPERATIVE_GROUP_OFFER::Reversion(CDataMgr* pDataMgr, int nIdx)
{
	EFS_BEGIN

	if(NULL == pDataMgr) return -1;
	if(0 > nIdx || pDataMgr->GetCount() <= nIdx) return -2;

	CString strPublishFormCode;
	pDataMgr->GetData(_T("발행형태구분"), nIdx, strPublishFormCode);

	CString strManageCode;
	pDataMgr->GetData(_T("제공도서관_관리구분"), nIdx, strManageCode);

 	CString strRegNo;
 	pDataMgr->GetData(_T("등록번호"), nIdx, strRegNo);

 	CString strBookKey;
 	pDataMgr->GetData(_T("책KEY"), nIdx, strBookKey);

	CString strVolInfoColumn;
	CString strBookTbl;
	CString strReverseUserCnt;
	if(_T("MO") == strPublishFormCode)
	{
		strBookTbl = _T("BO_BOOK_TBL BB");
		strVolInfoColumn = _T("VOL_SORT_NO");
		strReverseUserCnt.Format(
			_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2 WHERE CL2.BOOK_KEY = BB.REC_KEY AND CL2.STATUS = '3' )"));
	}
	else if(_T("SE") == strPublishFormCode)
	{
		strBookTbl = _T("SE_BOOK_TBL BB");
		strVolInfoColumn = _T("VOL_KEY");
		strReverseUserCnt.Format(
			_T("(SELECT COUNT(*) FROM LS_WORK_T01 CL2 WHERE CL2.BOOK_KEY = BB.REC_KEY AND CL2.STATUS = '3' )"));
	}
	else if(_T("NB") == strPublishFormCode)
	{
		return 1;
	}
	else return -3;
	
	CString strQuery;
	strQuery.Format(
		_T("SELECT BB.SPECIES_KEY, %s, %s ")
		_T("FROM %s ")
		_T("WHERE BB.REC_KEY = %s AND BB.MANAGE_CODE = '%s' AND ROWNUM = 1"),
		strVolInfoColumn,
		strReverseUserCnt,
		strBookTbl,
		strBookKey,
		strManageCode
		);

	int ids = -1;

	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	ids = TmpDM.RestructDataManager(strQuery);
	
	CString strSpeciesKey;
	TmpDM.GetCellData(0, 0, strSpeciesKey);

	CString strVolInfo;
 	TmpDM.GetCellData(0, 1, strVolInfo);

	CString strReversionCnt;
	TmpDM.GetCellData(0, 2, strReversionCnt);
	
	if ( m_pLoanMangeValue->m_bVolReserve && FALSE == strVolInfo.IsEmpty())
	{		
		CString sQuery;
		sQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL ") 
			_T(" WHERE CL.BOOK_KEY IN ( SELECT REC_KEY FROM %s WHERE SPECIES_KEY = %s ) ")
			_T("   AND CL.REG_NO IS NULL ")
			_T("   AND CL.STATUS = '3' ")
			_T("   AND CL.VOL_INFO = %s "),
			strBookTbl, strSpeciesKey, strVolInfo);

		TmpDM.GetOneValueQuery(sQuery, strReversionCnt);
	}
		
	if( 0 == _ttoi(strReversionCnt))  return 0;
	if ( !(m_pLoanMangeValue->m_bVolReserve) )
	{
		CString sSQL;
		sSQL.Format( _T("SELECT REC_KEY FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3' AND RESERVATION_DATE = ")
			_T("(SELECT MIN(RESERVATION_DATE) FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3')") , strBookKey, strBookKey);
		
		CString sLoanKey;
		TmpDM.GetOneValueQuery(sSQL, sLoanKey);
		
		
		if( sLoanKey.IsEmpty() )
			return 0;
	}
	
	INT nYes = MessageBox(_T("이 자료는 예약된 자료입니다. 예약 처리를 하시겠습니까?"), _T("예약 처리"), MB_YESNO | MB_ICONQUESTION);
	if( nYes == IDYES )
	{		
		if(!(m_pLoanMangeValue->m_bVolReserve))
		{
			CBO_LOC_3170 Dlg(this, m_pLoanShareManager , m_pLoanMangeValue , 0);
			Dlg.pMain = pMain;
			Dlg.pParentMgr = (CESL_Mgr*)this;
			Dlg.m_pInfo = m_pInfo;
			Dlg.SetInfo(strSpeciesKey, strVolInfo, strPublishFormCode, strBookKey);
			Dlg.m_nReserveInfoMode	=	1;
			Dlg.DoModal();
		}
		else 
		{
			CBO_LOC_3170_ReserveInfo Dlg(this, m_pLoanShareManager , m_pLoanMangeValue , 0);
			Dlg.pMain = pMain;
			Dlg.pParentMgr = (CESL_Mgr*)this;
			Dlg.m_pInfo = m_pInfo;
			Dlg.SetInfo(strSpeciesKey, strVolInfo, strPublishFormCode, _T(""));
			Dlg.m_nReserveInfoMode	=	1;
			Dlg.m_strReturnBookKey	=	strBookKey;
			Dlg.m_strReturnBookRegNo	=	strRegNo;
			

			Dlg.DoModal();
		}
	}
	
	return 0;

	EFS_END
		return -1;
}

void CBOOK_COOPERATIVE_GROUP_OFFER::OnDropdowncmbOFFER() 
{	
	int ids = -1;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_OFFER::OnDropdowncmbREQUEST");

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("멀티콤보_진행상태"));
	if(NULL == pCMB) ERROR_MSG_RETURN_VOID2(-1, strFuncName);

	int nRow = pCMB->GetCurSel();
	if(-1 != nRow)
	{
		CString strData = pCMB->GetItemDataString(nRow, 0);
		if(_T("    ") == strData)
		{
			ResetComboBox();
		}
	}		
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Get_TblName(CString &strTblName, CString strMode, CString strSearchState)
{
	int ids = -1;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_OFFER::Get_TblName");

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("멀티콤보_진행상태"));
	if(NULL == pCMB) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	CString strStateCode, strStateDesc;
	if(strSearchState.IsEmpty())
	{
		const int nIdx = pCMB->GetCurSel();

		strStateCode = pCMB->GetItemDataString(nIdx, 0);
		if(_T("") == strStateCode) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
		
		strStateDesc = pCMB->GetItemDataString(nIdx, 1);
		if(_T("") == strStateCode) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
	}
	else
	{
		strStateCode = strSearchState;
	}
	
	CString strTableName = _T("");	
	if(	_T("0002") == strStateCode		
		|| _T("0004") == strStateCode	
		|| _T("0014") == strStateCode	
		|| _T("0008") == strStateCode	
		|| _T("0011") == strStateCode	
 		|| _T("0012") == strStateCode	
 		|| _T("0009") == strStateCode	
	)
	{		
		CString strStateQuery;
		if(_T("0011") == strStateCode && _T("발송거절된자료") == strStateDesc)
		{
			strStateQuery = _T(" AND END_DIV IS NULL");
		}
		else if( 
			(_T("0011") == strStateCode && _T("최근발송거절확인된자료") == strStateDesc)
			|| (_T("0012") == strStateCode && _T("최근요청거절된자료") == strStateDesc)
			|| _T("0009") == strStateCode
			)
		{
			strStateQuery = _T("AND END_DATE BETWEEN TO_CHAR(ADD_MONTHS(SYSDATE, -3), 'YYYY/MM/DD') AND SYSDATE AND END_DIV IS NOT NULL");

		}	
			
		strTableName.Format(
			_T("(SELECT ITL.*, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")
			_T("FROM ILL_TRANS_TBL ITL WHERE ITL.GIVE_LIBCODE = '%s' AND ITL.STATE = '%s'%s) A"),
			m_strLibCode, strStateCode, strStateQuery);
	}
	else if(_T("----") == strStateCode)
	{
		CString strState = _T("'0002', '0004', '0008', '0011', '0014'");

		strTableName.Format(
			_T("(SELECT ITL.*, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")			
			_T("FROM ILL_TRANS_TBL ITL WHERE ITL.GIVE_LIBCODE = '%s' ")
			_T("AND ITL.STATE IN (%s) AND ITL.END_DIV IS NULL) A")
			, m_strLibCode, strState);
	}
	else if(_T("    ") == strStateCode)
	{
		strTableName = m_strTableName;
	}
	else if(_T("ALL") == strStateCode)
	{
		strTableName = 
			_T("(SELECT ITL.*, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")
			_T("FROM ILL_TRANS_TBL  ITL ) A");
	}
	else
	{
		CString strFormat = _T("");
		strFormat.Format(_T("알 수없는 코드 : %s"), strStateCode);
		AfxMessageBox(strFormat);

		ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);		
	}
	if(_T("MO") == strMode)
	{
		strTableName += _T(", BO_BOOK_TBL BB, IDX_BO_TBL IB, CO_LOAN_USER_TBL CLU");
	}
	else if(_T("SE") == strMode)
	{
		strTableName += _T(", SE_BOOK_TBL BB, IDX_SE_TBL IB, SE_VOL_TBL V, CO_LOAN_USER_TBL CLU");
	}
	else if(_T("NB") == strMode)
	{
		strTableName += _T(", CO_NON_DB_BOOK_TBL BB, CO_LOAN_USER_TBL CLU");
	}
	strTblName = strTableName;

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::ResetComboBox()
{
	int ids = -1;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_OFFER::ResetComboBox");

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("멀티콤보_진행상태"));
	if(NULL == pCMB) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	const int COL_COUNT = 2;
	pCMB->ResetContent();
	pCMB->FreeComboData();
	pCMB->InitMultiComboBox(COL_COUNT);	

	const int STATE_COUNT = 8;
	CString strCmbData[STATE_COUNT][COL_COUNT] = 
	{
		{ _T("----"), _T("전체(진행중 자료)") },		
		{ _T("0002"), _T("요청된자료")},
		{ _T("0004"), _T("발송된자료")},
		{ _T("0008"), _T("복귀중자료")},
		{ _T("0014"), _T("입수된자료")},
		{ _T("0009"), _T("최근완료된자료")},
		{ _T("0011"), _T("발송거절된자료")},
		{ _T("0011"), _T("최근발송거절확인된자료")}
	};
	
	for(int nRow = 0; nRow < STATE_COUNT; nRow++)
	{
		for(int nCol = 0; nCol < 2; nCol++)
			pCMB->AddItem(strCmbData[nRow][nCol], nCol, nRow);
	}

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Compare()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() == 0)
	{
		m_pGrid->SelectRemoveAll();
		INT nRows = m_pDM->GetRowCount();		
		for (int i = 0; i < nRows; i++)
		{
			m_pGrid->m_arraySelect.AddTail(i);
		}
		m_pGrid->m_arrayPos = m_pGrid->m_arraySelect.GetHeadPosition();
	}
 	int nCnt = m_pGrid->SelectGetCount();
 	if(0 >= nCnt) 
 	{
 		AfxMessageBox(_T("검색할 정보가 존재하지 않습니다."));
 		return -1;
 	}

	CILL_COMPARE Dlg(this);
	Dlg.m_pInfo = m_pInfo;
	Dlg.SetMode(CILL_COMPARE::IDX_OFFER);

	if(IDOK == Dlg.DoModal())
	{
		ExcutePath(_T("제공_발송거절"));
	}
	return 0;
}

void CBOOK_COOPERATIVE_GROUP_OFFER::Search(WPARAM wParam, LPARAM lParam)
{
	CILL_SEARCH* pDlg = (CILL_SEARCH*)wParam;

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("멀티콤보_진행상태"));
	if(NULL == pCMB) ERROR_MSG_RETURN_VOID2(-1, _T("Search"));
	
	const int ROW = pCMB->GetRowCount();
	pCMB->AddItem(_T("    "), 0, ROW);
	pCMB->AddItem(_T("검색된자료"), 1, ROW);
	pCMB->SetCurSel(ROW);
	
 	Refresh(_T("ALL"), pDlg->m_strSQLWhere);

}

INT CBOOK_COOPERATIVE_GROUP_OFFER::Make_InsertLoanTblQuery(pStruMakeInsertTbl pStru)
{
	EFS_BEGIN

	if(NULL == pStru->pDM)
	{
		return -1000;
	}

	pStru->pDM->InitDBFieldData();

	
	INT ids = 0;
	const INT nSrcFieldCnt = 15;
	CString StrSrcField[nSrcFieldCnt];
	TCHAR *sDstFieldAlias[nSrcFieldCnt] = 
	{
		_T("BOOK_KEY")	  , _T("SHELF_LOC_CODE"), _T("PUBLISH_FORM_CODE"), _T("CLASS_NO_TYPE"), _T("CLASS_NO")			, 
		_T("MANAGE_CODE"), _T("REG_CODE")		 , _T("SPECIES_KEY")	   , _T("REG_NO")		 , _T("SEPARATE_SHELF_CODE")	, 
		_T("TITLE")		  , _T("AUTHOR")		 , _T("PUBLISHER")		   , _T("CALL_NO")		 , _T("VOL_INFO")		
	};
	TCHAR *sDstFieldTypeAlias[nSrcFieldCnt] = 
	{
		_T("NUMERIC")	  , _T("STRING")         , _T("STRING")	           , _T("STRING")        , _T("STRING"), 
		_T("STRING")	  , _T("STRING")         , _T("NUMERIC")	       , _T("STRING")        , _T("STRING"),
		_T("STRING")	  , _T("STRING")         , _T("STRING")	           , _T("STRING")        , _T("NUMERIC")
	};

	CString strSQL;
	if(_T("MO") == pStru->strInPublishFormCode)
	{		
		strSQL.Format(_T("SELECT ")
			_T(" BB.REC_KEY, BB.SHELF_LOC_CODE, 'MO', BB.CLASS_NO_TYPE, BB.CLASS_NO, ")
			_T(" BB.MANAGE_CODE, BB.REG_CODE, BB.SPECIES_KEY, BB.REG_NO, BB.SEPARATE_SHELF_CODE, ")
			_T(" IB.TITLE_INFO, IB.AUTHOR_INFO, IB.PUB_INFO, ECO$F_MAKE_CALLNO$I(BB.SEPARATE_SHELF_CODE, BB.CLASS_NO, BB.BOOK_CODE, BB.VOL_CODE, BB.COPY_CODE), BB.VOL_SORT_NO ")
			_T("FROM BO_BOOK_TBL BB, IDX_BO_TBL IB WHERE BB.SPECIES_KEY = IB.REC_KEY AND BB.REC_KEY = %s"),
			pStru->strInBookKey);
	}
	else if(_T("SE") == pStru->strInPublishFormCode)
	{		
		strSQL.Format(_T("SELECT ")
			_T(" BB.REC_KEY, BB.SHELF_LOC_CODE, 'SE', BB.CLASS_NO_TYPE, BB.CLASS_NO, ")
			_T(" BB.MANAGE_CODE, BB.REG_CODE, BB.SPECIES_KEY, BB.REG_NO, BB.SEPARATE_SHELF_CODE, ")
			_T(" IB.TITLE_INFO, IB.AUTHOR_INFO, IB.PUB_INFO, ECO$F_MAKE_CALLNO$I(BB.SEPARATE_SHELF_CODE, BB.CLASS_NO, BB.BOOK_CODE, BB.VOL_CODE, BB.COPY_CODE), BB.VOL_KEY ")
			_T("FROM SE_BOOK_TBL BB, IDX_SE_TBL IB WHERE BB.SPECIES_KEY = IB.REC_KEY AND BB.REC_KEY = %s"),
			pStru->strInBookKey);
	}
	pStru->pDM->GetValuesQuery(strSQL, StrSrcField, nSrcFieldCnt);
	
	for( INT i= 0 ; i < nSrcFieldCnt ;i++ )
	{
		if( StrSrcField[i].IsEmpty() && _T("NUMERIC") == sDstFieldTypeAlias[i])
		{
			StrSrcField[i] = _T("NULL");
		}

		ids = pStru->pDM->AddDBFieldData( sDstFieldAlias[i] , sDstFieldTypeAlias[i] , StrSrcField[i], TRUE );
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("Make_InsertLoanTblQuery"));
		}
	}

	
	const INT nSrcUserFieldCnt = 4;
	CString StrUserSrcField[nSrcUserFieldCnt];
	TCHAR *sDstUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("USER_KEY")  , _T("USER_POSITION_CODE"), _T("USER_CLASS_CODE"), _T("MEMBER_CLASS") 
	};
	TCHAR *sDstUserFieldTypeAlias[nSrcUserFieldCnt] = 
	{
		_T("NUMERIC")   , _T("STRING")                  , _T("STRING")               , _T("STRING") 
	};
	strSQL.Format(_T("SELECT ")
		_T(" CLU.REC_KEY, CLU.USER_POSITION_CODE, CLU.USER_CLASS_CODE, CLU.MEMBER_CLASS ")
		_T(" FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY = %s"), pStru->strInUserKey);
	pStru->pDM->GetValuesQuery(strSQL, StrUserSrcField, nSrcUserFieldCnt);

	for( i= 0 ; i < nSrcUserFieldCnt ;i++ )
	{
		ids = pStru->pDM->AddDBFieldData( sDstUserFieldAlias[i] , sDstUserFieldTypeAlias[i] , StrUserSrcField[i], TRUE );
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("InsertLoanRecord"));
		}
	}

	pStru->pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), _T("O"));
	pStru->pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), pStru->strInLoanKey);
	pStru->pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("발송"), __WFILE__, __LINE__));
	pStru->pDM->MakeInsertFrame(_T("LS_WORK_T01"));

	return 0;

	EFS_END
		return -1;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::RefreshState()
{
	CString strCaption;
	GetDlgItemText(IDC_BTN_OFFER_STATE, strCaption);
	if(_T("▲") == strCaption)
	{
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(_T(""));

		CString strQuery;


		strQuery.Format(
			_T("SELECT T.STATE, COUNT(1) ")
			_T("FROM ( ")
				_T("SELECT A.STATE ")
				_T("FROM ILL_TRANS_TBL A, BO_BOOK_TBL BB, IDX_BO_TBL IB, CO_LOAN_USER_TBL CLU ")
				_T("WHERE A.BOOK_KEY = BB.REC_KEY AND BB.SPECIES_KEY = IB.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.END_DIV IS NULL AND GIVE_LIBCODE = '%s' ")
				_T("UNION ALL ")
				_T("SELECT A.STATE ")
				_T("FROM ILL_TRANS_TBL A, SE_BOOK_TBL BB, IDX_SE_TBL IB, SE_VOL_TBL V, CO_LOAN_USER_TBL CLU ")
				_T("WHERE A.BOOK_KEY = BB.REC_KEY AND BB.SPECIES_KEY=IB.REC_KEY AND BB.VOL_KEY=V.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.PUBLISH_FORM_CODE = 'SE' AND GIVE_LIBCODE = '%s' ")
				_T("UNION ALL ")
				_T("SELECT A.STATE ")
				_T("FROM ILL_TRANS_TBL A, CO_NON_DB_BOOK_TBL BB, CO_LOAN_USER_TBL CLU ")
				_T("WHERE A.BOOK_KEY = BB.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.PUBLISH_FORM_CODE = 'NB' AND GIVE_LIBCODE = '%s' ")
			_T(") T ")
			_T("GROUP BY T.STATE"),
			m_pInfo->LIB_CODE, m_pInfo->LIB_CODE, m_pInfo->LIB_CODE);

		TmpDM.RestructDataManager(strQuery);

		CString strState, strCnt, strCaption;
		INT nRow = 0;
		INT ROW_COUNT = TmpDM.GetRowCount();
		INT nCnt = 0;
		for(nCnt = 0; nCnt < 5; nCnt++)
		{
			for(nRow = 0; nRow < ROW_COUNT; nRow++)
			{
				strState = TmpDM.GetCellData(nRow, 0);
				strCnt = TmpDM.GetCellData(nRow, 1);

				if(strState == m_strState[nCnt][1])
				{
					strCaption = m_strState[nCnt][0] + _T("(") + strCnt + _T(")");
					SetDlgItemText(4500+nCnt,strCaption);
					break;
				}
			}
			if(ROW_COUNT <= nRow)
			{
				SetDlgItemText(4500+nCnt,m_strState[nCnt][0]);
			}
		}

	}

	return 0;
}
void CBOOK_COOPERATIVE_GROUP_OFFER::OnBtnOfferState() 
{
	
	CString strCaption;
	GetDlgItemText(IDC_BTN_OFFER_STATE, strCaption);
	if(_T("▲") == strCaption)
	{
		SetDlgItemText(IDC_BTN_OFFER_STATE, _T("▼"));
	}
	else if(_T("▼") == strCaption)
	{
		SetDlgItemText(IDC_BTN_OFFER_STATE, _T("▲"));

		RefreshState();
	}

	ResizeControl();
}
void CBOOK_COOPERATIVE_GROUP_OFFER::OnBtnState(UINT uID)
{
	CString strState = m_strState[uID-4500][1];

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("멀티콤보_진행상태"));
	if(NULL == pCMB) ERROR_MSG_RETURN_VOID2(-1, _T("OnBtnState"));

	CString strData = _T("");		
	int ROW = pCMB->GetCount();
	for(int nCnt = 0; nCnt < ROW; nCnt++)
	{
		strData = pCMB->GetItemDataString(nCnt, 0);
		if(strState == strData)
		{
			strData = pCMB->GetItemDataString(nCnt, 1);
			if(0 > strData.Find(_T("최근")))
			{
				pCMB->SetCurSel(nCnt);
				break;
			}
		}
	} 	

	OnSelchangecmbOFFER();
}

LRESULT CBOOK_COOPERATIVE_GROUP_OFFER::OnExcute(WPARAM wParam, LPARAM lParam)
{
	CString* strMessage = (CString*)lParam;
	CDataMgr* pDataMgr = (CDataMgr*)wParam;

	if(_T("발송") == pDataMgr->GetDataAlias())
	{
		return Send(pDataMgr, strMessage);
	}
	else if(_T("복귀완료") == pDataMgr->GetDataAlias())
	{
		return Return_Complete(pDataMgr, strMessage);
	}

	return 1000;
}


INT CBOOK_COOPERATIVE_GROUP_OFFER::PreReturn_Complete(CDataMgr* pDataMgr, CString* pstrMessage)
{
	int ids = 0;
	CString strFuncName = _T("PreReturn_Complete");
	
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}

	CStringArray aCheckArray;
	aCheckArray.Add(_T("상태|NOT IN|0004,0008,0014"));
	aCheckArray.Add(_T("대출상태|==|대출중"));
	aCheckArray.Add(_T("제공도서관_코드|!=|%m_strLibCode%"));
	
	ids = UnCheckGrid(aCheckArray);
	}
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 복귀(완료) 가능한 항목이 없습니다.\r\n")
			_T("복귀(완료) 가능한 대상은 아래와 같습니다.\r\n\r\n")
			_T("1. 자관에서 제공한 자료\r\n")
			_T("2. 진행상태가 발송된자료(0004), 입수된자료(0014), 복귀중자료(0008)인 트랜잭션\r\n")
			_T("3. 현재 대출중이 아닌 자료")
			);
		return -1;
	}
	else
	{
		if(NULL == pDataMgr)
		{
			return -1020;
		}

		CString strGet;
		INT nSetIdx = 0;
		INT nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{	
			pDataMgr->InsertData(-1);	
			nSetIdx = pDataMgr->GetCount();
			m_pDM->GetCellData(_T("트랜잭션번호"), nIdx, strGet);			
			pDataMgr->SetData(_T("트랜잭션번호"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("책KEY"), nIdx, strGet);
			pDataMgr->SetData(_T("책KEY"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("이용자KEY"), nIdx, strGet);			
			pDataMgr->SetData(_T("이용자KEY"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("발행형태구분"), nIdx, strGet);
			pDataMgr->SetData(_T("발행형태구분"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("제공도서관_관리구분"), nIdx, strGet);
			pDataMgr->SetData(_T("제공도서관_관리구분"), strGet, nSetIdx);

			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);

	}
	
	return 0;
}

INT CBOOK_COOPERATIVE_GROUP_OFFER::Return_Complete(CDataMgr* pDataMgr, CString* pstrMessage)
{
	int ids = 0;
	CString strFuncName = _T("Return_Complete");

	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	INT CNT = pDataMgr->GetCount();

	for(INT nIdx = 0; nIdx < CNT; nIdx++)
	{
		TmpDM.StartFrame();
		CString strTransNo = _T("");
		
		{
		
		pDataMgr->GetData(_T("트랜잭션번호"), nIdx, strTransNo);

		CString strQuery = _T("");
		
		strQuery.Format(
			_T("UPDATE ")
				_T("ILL_TRANS_TBL ")
			_T("SET ")
				_T("STATE = '0009', ")	
				_T("END_USERID = '%s', ") 
				_T("END_DATE = SYSDATE, ")
				_T("END_DIV = '0010', ") 
				_T("LAST_WORK = '%s' ")
			_T("WHERE ")
				_T("TRANS_NO = '%s';"),
				m_pInfo->USER_ID,
				GetWorkLogMsg( _T("복귀(완료)"), __WFILE__, __LINE__ ),
				strTransNo
			);
		TmpDM.AddFrame(strQuery);
		}
		
		
		{

		CString strQuery = _T("");
		ids = Get_UpdateBookTblQuery_For_CompleteTransaction(
			pDataMgr,
			nIdx,
			GetWorkLogMsg( _T("복귀(완료)"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);
		TmpDM.AddFrame(strQuery);
		
		ids = Get_UpdateIdxTblQuery(
			pDataMgr,
			nIdx,
			GetWorkLogMsg( _T("복귀(완료)"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids)
		{
			pstrMessage->Format(_T("DB 업데이트에 실패하였습니다.[%d]"), ids);
			return ids;
		}
		else if(0 == ids)
		{
			TmpDM.AddFrame(strQuery);
		}

		}
		
		ids = TmpDM.SendFrame(TRUE);
		TmpDM.EndFrame();

		if(0 > ids)
		{
			pstrMessage->Format(_T("DB 업데이트에 실패하였습니다.[%d]"), ids);
			return ids;
		}

		if(_T("D") == m_sBookCooperativeMode)
		{
			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,END_USERID,END_DATE,END_DIV"));				
		}		

		Reversion(pDataMgr, nIdx);
	}

	CString strNO;
	MakeTransNoList(pDataMgr, strNO);
	

	if(0 != m_pGrid->SelectGetCount())
	{
		return RFID_SecurityChange(strNO, TRUE, TRUE);
	}

	return 0;
}


INT CBOOK_COOPERATIVE_GROUP_OFFER::MakeValidRegNo(CString &strGetRegNo)
{
EFS_BEGIN

	
	if( strGetRegNo.IsEmpty() )
	{
		AfxMessageBox( _T("반납 대상 자료의 등록번호를 입력해야 합니다") );
		return -1001;
	}

	CString strTmp;
	INT ids = 0;
	CPreRegCodeMgr *preRegCodeMgr = new CPreRegCodeMgr(this);
	ids = preRegCodeMgr->IsInPreRegNo( strGetRegNo , m_pInfo->MANAGE_CODE, strTmp );
	if(1 == ids)
	{
		CLocCommonAPI::MakeRegNoSpaceToZero(this, strGetRegNo, NULL);
	}
	else
	{
		
		INT nLoopCount = m_pManageCodeList->GetRowCount();
		CString strManageCode = _T("");
		for(INT i=0; i<nLoopCount; i++)
		{
			strManageCode = m_pManageCodeList->GetCellData(i, 0);
			ids = preRegCodeMgr->IsInPreRegNo( strGetRegNo , strManageCode, strTmp );
			if(1 == ids)
			{
				CLocCommonAPI::MakeRegNoSpaceToZero(this, strGetRegNo, strManageCode, NULL);
				break;
			}
		}
	}
	if ( preRegCodeMgr != NULL ) 
	{
		delete preRegCodeMgr;
	}

	strTmp = m_pManageCodeList->GetCellData(0, 0);

	
	INT		nRegNoLength = strGetRegNo.GetLength();

	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;

	if( 12 < nRegNoLength || nRegNoLength < nRegCodeLength)
	{
		AfxMessageBox(_T("잘못된 등록번호입니다."));
		strGetRegNo.Empty();
		return -1010;
	}
	else if( 12 > nRegNoLength )
	{		
		CString strRegCode = strGetRegNo.Mid(0,nRegCodeLength);
		CString strRegNum  = strGetRegNo.Mid(nRegCodeLength);

		strGetRegNo = m_pInfo->MakeRegNo(strRegCode, strRegNum);
	}
	
	
	
	return 0;

EFS_END
return -1;
}

INT CBOOK_COOPERATIVE_GROUP_OFFER::SelectSearchHistory()
{
EFS_BEGIN

	INT ids;
	CString strBookRegNo;
	
	CString strInBarcodeLibCode, strInBarcode, strQuery, strDMLibCode;
	GetDlgItemText(IDC_edtRegNo, strInBarcode);
	
	BOOL bSelected = FALSE;

	if( strInBarcode.IsEmpty() )
	{
		AfxMessageBox( _T("제공자료의 등록번호를 입력해야 합니다") );
		return -1001;
	}

	
	if ( strInBarcode.GetLength() == 19 ) {
		strInBarcodeLibCode = strInBarcode.Mid(1, 6);	
		strBookRegNo = strInBarcode.Mid(7);		
	}
	else {
		strBookRegNo = strInBarcode;
		
		ids = MakeValidRegNo( strBookRegNo );
		if( 0 > ids )	return -1010;
		strInBarcodeLibCode = _T("");
	}	

	
	CESL_DataMgr *pDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_OFFER") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SelectSearchHistory()") );
			
	if( pDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("검색된 데이타가 없습니다") );
		return -1001;
	}
	else
	{
		for( int i = 0; i < pDM->GetRowCount(); i++ )
		{
			CString strGridBookRegNo;
			pDM->GetCellData( _T("등록번호"), i, strGridBookRegNo );
			if( strGridBookRegNo == strBookRegNo )
			{
				
				if ( strInBarcodeLibCode.GetLength() > 0 ) {
					pDM->GetCellData( _T("제공도서관_코드"), i, strDMLibCode );
					if ( strInBarcodeLibCode == strDMLibCode ) {
						
						CESL_Grid* pgrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("그리드_제공자료") );
						pgrid->SetAt(i, pgrid->m_nSelectIdx, pgrid->m_sSelectString);
						
						
						
						pDM->SetCellData(_T("선정"), pgrid->m_sSelectString, i);
						bSelected = TRUE;
						break;
					}
				}
				else {
					
					CESL_Grid* pgrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("그리드_제공자료") );
					pgrid->SetAt(i, pgrid->m_nSelectIdx, pgrid->m_sSelectString);
					
					
					
					pDM->SetCellData(_T("선정"), pgrid->m_sSelectString, i);
					bSelected = TRUE;
					break;
				}
			}
		}
		
		
		for( int j = 0; j< pDM->GetRowCount(); j++ )
		{
			CString strCheck;
			CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("그리드_제공자료") );
			pGrid->GetAt(j,0,strCheck);
			if(strCheck == _T(""))
				pDM->SetCellData(_T("선정"),_T(""), j);
		}
	}
	
	
	
	if(bSelected)
	{
		pDM->SORT(_T("선정"), TRUE);
		CESL_Grid* pgrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("그리드_제공자료") );
		pgrid->Display();
	}
	else
	{
		AfxMessageBox( _T("선정된 데이터가 없습니다") );		
	}
	return 0;
EFS_END
return -1;
}

void CBOOK_COOPERATIVE_GROUP_OFFER::OnbtnSelect() 
{
EFS_BEGIN

	SelectSearchHistory();
	
	SetDlgItemText(IDC_edtRegNo, _T(""));
	( CEdit*) GetDlgItem (IDC_edtRegNo)->SetFocus();

EFS_END
}

INT CBOOK_COOPERATIVE_GROUP_OFFER::PrintOfferList()
{
EFS_BEGIN
	
	INT ids;
	
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_OFFER") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
		return 1;
	}
		
	if( PrivacyPrintLogRecord(pSrcDM, _T("책두레도서 발송 정보출력"), _T("E"), 0 ) <0 ) return 1;
		
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_OFFER") , _T("그리드_제공자료") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	INT nMode = 0;
	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();

	if ( nSelectCnt > 6 ) {
		AfxMessageBox( _T("최대 6개의 자료출력이 가능합니다.") );
		return 1;
	}

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_OFFER_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	pPrintDM->FreeData();
	
	const INT nCopyCnt = 7;
	TCHAR *sField[nCopyCnt][2] =
	{		
		{_T("이용자_번호"),_T("이용자_번호")},
		{_T("이용자명"),_T("이용자명")},		
		{_T("서명"),_T("서명")},		
		{_T("저자정보"),_T("저자정보")},
		{_T("요청도서관_이름"),_T("요청도서관_이름")},
		{_T("제공도서관_이름"),_T("제공도서관_이름")},
		{_T("발송일"),_T("발송일")}
	};
		
	CString sData;
	INT nIndex;

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();

		pPrintDM->InsertRow(-1);
		
		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("번호") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		
		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] ,j, sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , j, sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}

		if( nMode == 1)
			pGrid->SelectGetNext();
	}

	
	BOOL bRight = TRUE;
	ids = IsRightPrint(bRight);
	if ( ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("PrintList") );

	if ( bRight == FALSE ) {
		AfxMessageBox( _T("선정된 자료중에서 발송일 또는 대출도서관이 다른 자료가 존재합니다. 확인하여 주십시오.") );
		return 1;
	}
		
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);

	
	ids = pSIReportManager->GetSIReportFileFromDB(_T("책두레_발송정보"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}
	INT iddd = pPrintDM->GetColCount();

	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->PrintOfferList();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return 0;
	
EFS_END
	return -1;
}

INT CBOOK_COOPERATIVE_GROUP_OFFER::IsRightPrint(BOOL& bIsRight)
{
EFS_BEGIN
	INT i = -1, nRowCnt = 0;
	CString strFuncName = _T("IsRightPrint");	
	CString strGiveDate1 = _T(""), strGiveDate2 = _T("");
	CString strAplLib1 = _T(""), strAplLib2 = _T("");

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_OFFER_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , strFuncName );

	bIsRight = TRUE;

	nRowCnt = pPrintDM->GetRowCount();
	for ( i=0; i<nRowCnt; i ++ ) {

		
		if ( i == 0 ) pPrintDM->GetCellData(_T("발송일"), i, strGiveDate1);
		pPrintDM->GetCellData(_T("발송일"), i, strGiveDate2);

		if ( strGiveDate1 != strGiveDate2) {
			bIsRight = FALSE;
			break;
		}

		if ( strGiveDate1.GetLength() == 0 ) {
			bIsRight = FALSE;
			break;
		}

		
		if ( i == 0 ) pPrintDM->GetCellData(_T("요청도서관_이름"), i, strAplLib1);
		pPrintDM->GetCellData(_T("요청도서관_이름"), i, strAplLib2);
		
		if ( strAplLib1 != strAplLib2) {
			bIsRight = FALSE;
			break;
		}

		if ( strAplLib1.GetLength() == 0 ) {
			bIsRight = FALSE;
			break;
		}
	}

	return 0;
EFS_END
	return -1;
}


INT CBOOK_COOPERATIVE_GROUP_OFFER::PrintList()
{
EFS_BEGIN
	
	INT ids;
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_OFFER") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
		return 1;
	}
	
	PrivacyPrintLogRecord(pSrcDM, _T("책두레 제공 목록출력"), _T("E"), 0 );
	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_OFFER") , _T("그리드_제공자료") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	INT nMode = 0;
	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	
	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_OFFER_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	pPrintDM->FreeData();
	
	const INT nCopyCnt = 12;

	TCHAR *sField[nCopyCnt][2] =
	{		
		{_T("이용자_번호"),_T("이용자_번호")},
		{_T("이용자명"),_T("이용자명")},
		{_T("등록번호"),_T("등록번호")},
		{_T("서명"),_T("서명")},
		{_T("부록여부"),_T("부록여부")},
		
		
		{_T("매체구분"),_T("매체구분")},
		{_T("청구기호"),_T("청구기호")},
		{_T("자료실_설명"),_T("자료실_설명")},
		{_T("진행상태"),_T("진행상태")},
		{_T("신청일"),_T("신청일")},
		{_T("요청도서관_이름"),_T("요청도서관_이름")},
		{_T("반납도서관_이름"),_T("반납도서관_이름")}
	};
		
	CString sData;
	INT nIndex;

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();

		pPrintDM->InsertRow(-1);
		
		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("번호") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		
		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , j,  sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] ,j , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}

		if( nMode == 1)
			pGrid->SelectGetNext();
	}	
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pPrintDM , _T("관리구분") , _T("관리구분") );
	
	pPrintDM->SORT(_T("청구기호"));

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);

	
	ids = pSIReportManager->GetSIReportFileFromDB(_T("책두레_제공"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}
	INT iddd = pPrintDM->GetColCount();

	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return 0;
	
EFS_END
	return -1;
}

INT CBOOK_COOPERATIVE_GROUP_OFFER::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	
	return 0;
	
	EFS_END
	return -1;	
}
BEGIN_EVENTSINK_MAP(CBOOK_COOPERATIVE_GROUP_OFFER, CDialog)
//{{AFX_EVENTSINK_MAP(CBOOK_COOPERATIVE_GROUP_OFFER)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

INT CBOOK_COOPERATIVE_GROUP_OFFER::Send_Refuse_Cancel()
{
		
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}

	CStringArray aCheckArray;
	aCheckArray.Add(_T("상태|!=|0011"));
	aCheckArray.Add(_T("제공도서관_코드|!=|%m_strLibCode%"));

	UnCheckGrid(aCheckArray);
	}
		
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 발송취소 가능한 항목이 없습니다.\r\n")
			_T("발송취소 가능한 대상은 아래와 같습니다.\r\n\r\n")
			_T("1. 제공도서관이 자관인 경우\r\n")
			_T("2. 진행상태가 발송거절된 자료(0011)인 트랜잭션\r\n")
			);
		return -1;
	}


	CString strNO = Get_Select_TRANS_NO_List(TRUE);

	if(_T("") == strNO) return -1;

 	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	if(_T("I") == m_sBookCooperativeMode)
	{
		CString strNO = Get_Select_TRANS_NO_List(TRUE);		
		if(_T("") == strNO) return -1;

		int nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{	

			TmpDM.StartFrame();

			CString strQuery;
			strQuery.Format(
				_T("UPDATE ILL_TRANS_TBL SET ")
				_T("LOAN_KEY = NULL, ")
				_T("STATE = '0002', ")
				_T("GIVE_USERID = '%s', ")  
				_T("GIVE_DATE = NULL, ")	
				_T("GIVE_CANCEL = NULL, ")	
				_T("WORKNO = NULL, ")
				_T("LAST_WORK = '%s' ")
				_T("WHERE ")
				_T("TRANS_NO IN (%s) AND STATE = '0011' AND GIVE_LIBCODE = '%s';"),
				m_pInfo->USER_ID,
				GetWorkLogMsg( _T("발송거절취소"), __WFILE__, __LINE__ ),
				strNO,
				m_strLibCode
				);

			TmpDM.AddFrame(strQuery);
			int ids = TmpDM.SendFrame();
			TmpDM.EndFrame();

			
			CString strBookKey = _T("");
			CString strUserKey = _T("");
			
			CString strWorkingStatus = _T("BOL112N");

			m_pDM->GetCellData(_T("책KEY"), nIdx, strBookKey);
			m_pDM->GetCellData(_T("이용자KEY"), nIdx, strUserKey);

			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ")
				_T("WORKING_STATUS = PREV_WORKING_STATUS, ")
				_T("PREV_WORKING_STATUS = '%s' ")
				_T("WHERE ")
				_T("REC_KEY = %s AND MANAGE_CODE = '%s';"),
				strWorkingStatus,strBookKey,m_pInfo->MANAGE_CODE );

			TmpDM.StartFrame();
			TmpDM.AddFrame(strQuery);
			TmpDM.SendFrame();
			TmpDM.EndFrame();
			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);

	}	
	else if(_T("D") == m_sBookCooperativeMode)
	{

		CString strLoanKey, strBookKey, strUserKey, strTransNo;
		int nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{	
			m_pDM->GetCellData(_T("트랜잭션번호"), nIdx, strTransNo);
			m_pDM->GetCellData(_T("대출정보_키"), nIdx, strLoanKey);

			TmpDM.StartFrame();				
			CString strQuery;
			strQuery.Format(
				_T("UPDATE ILL_TRANS_TBL SET ")
				_T("STATE = '0002', ")
				_T("GIVE_USERID = '%s', ") 
				_T("GIVE_DATE = SYSDATE, ")
				_T("LOAN_KEY = NULL, ")
				_T("WORKNO = NULL, ")
				_T("LAST_WORK = '%s' ")
				_T("WHERE ")
				_T("TRANS_NO = '%s' AND STATE = '0011' AND GIVE_LIBCODE = '%s';"),
				m_pInfo->USER_ID,
				GetWorkLogMsg( _T("발송거절취소"), __WFILE__, __LINE__ ),
				strTransNo,
				m_strLibCode
				);

			TmpDM.AddFrame(strQuery);

			TmpDM.MakeDeleteFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey);

			int ids = TmpDM.SendFrame();
			TmpDM.EndFrame();

			
			CString strBookKey = _T("");
			CString strUserKey = _T("");
			
			CString strWorkingStatus = _T("BOL112N");
			
			m_pDM->GetCellData(_T("책KEY"), nIdx, strBookKey);
			m_pDM->GetCellData(_T("이용자KEY"), nIdx, strUserKey);
			
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ")
				_T("WORKING_STATUS = PREV_WORKING_STATUS, ")
				_T("PREV_WORKING_STATUS = '%s' ")
				_T("WHERE ")
				_T("REC_KEY = %s AND MANAGE_CODE = '%s';"),
				strWorkingStatus,strBookKey,m_pInfo->MANAGE_CODE );
			
			TmpDM.StartFrame();
			TmpDM.AddFrame(strQuery);
			TmpDM.SendFrame();
			TmpDM.EndFrame();
			nIdx = m_pGrid->SelectGetNext();

			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,GIVE_USERID,GIVE_DATE"));
			
			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);

	}	
	
	return RFID_SecurityChange(strNO, FALSE, TRUE);

	return 0;
}