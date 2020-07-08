// BOOK_COOPERATIVE_GROUP_REQUEST.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BOOK_COOPERATIVE_GROUP_REQUEST.h"
#include "ILL_VIEW_BOOK_INFO_DLG.h"
#include "ILL_VIEW_LIB_INFO_DLG.h"
#include "ILL_APL.h"
#include "ILL_APLWEB.h"
#include "ILL_MODIFY.H"
#include "ILL_SEARCH.h"
#include "ILL_COMPARE.h"
#include "..\\BL_LOC_LOAN\\BO_LOC_3170.h"
#include "..\\BL_LOC_LOAN\\BO_LOC_3170_ReserveInfo.h"
#include "..\\BL_LOC_LOAN\\CheckLoanBook.h"
#include "..\..\..\..\SMS/CO_SMS/SMS_MakeMsg.h"
#include "..\..\..\..\SMS/CO_SMS/SMS_Env_Info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]  = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOOK_COOPERATIVE_GROUP_REQUEST dialog


CBOOK_COOPERATIVE_GROUP_REQUEST::CBOOK_COOPERATIVE_GROUP_REQUEST(CESL_Mgr* pParent /*=NULL*/)
	: CBookCooperativeGroup(pParent)
{
	//{{AFX_DATA_INIT(CBOOK_COOPERATIVE_GROUP_REQUEST)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_pGrid = NULL;
	m_pDM = NULL;
	m_bUseSMS = FALSE;

	m_pLoanShareManager = NULL;
	m_pLoanMangeValue = NULL;
	g_strKakaoUsed1.Empty();
}


void CBOOK_COOPERATIVE_GROUP_REQUEST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOOK_COOPERATIVE_GROUP_REQUEST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOOK_COOPERATIVE_GROUP_REQUEST, CDialog)
	//{{AFX_MSG_MAP(CBOOK_COOPERATIVE_GROUP_REQUEST)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_cmb_REQUEST, OnSelchangecmbREQUEST)
	ON_CBN_DROPDOWN(IDC_cmb_REQUEST, OnDropdowncmbREQUEST)
	ON_BN_CLICKED(IDC_BTN_REQUEST_STATE, OnBtnRequestState)
	ON_BN_CLICKED(IDC_btnSelect, OnbtnSelect)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SEARCH, Search)
	ON_CONTROL_RANGE(BN_CLICKED, 4500, 4505, OnBtnState)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBOOK_COOPERATIVE_GROUP_REQUEST message handlers

BOOL CBOOK_COOPERATIVE_GROUP_REQUEST::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
 	if(0 > InitESL_Mgr(_T("BOOK_COOPERATIVE_GROUP_REQUEST")))
 	{
 		AfxMessageBox(_T("ERROR : InitESL_Mgr(\"BOOK_COOPERATIVE_GROUP_REQUEST\")"));
 	}

	m_pGrid = (CESL_Grid*)FindControl(
		_T("CM_BOOK_COOPERATIVE_GROUP_REQUEST"), _T("그리드_요청자료"));

	if( !m_pInfo->m_bUserInfoExportYN )
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("책두레 요청");
	}

	m_pDM = FindDM(_T("DM_BOOK_COOPERATIVE_GROUP_REQUEST"));
	if(NULL == m_pDM) return -2;

	ResetComboBox();
	{
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)FindControl(
		_T("CM_BOOK_COOPERATIVE_GROUP_REQUEST"), _T("멀티콤보_진행상태"));
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
	}

	{
	int ids = 0;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(
		_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE VALUE_NAME = '책두레대출대기기간'"));
	
	CString strValue;
	ids = TmpDM.GetOneValueQuery(strQuery, strValue);
	if(0 > ids) return FALSE;
	
	int nDate = _ttoi(strValue);
	m_strWaitLoanDayCount.Format(_T("%d"), nDate);
	
	CCheckLoanBook LoanBook(this, NULL);
	int nDay = _ttoi(m_strWaitLoanDayCount) -1;
	LoanBook.GetAddTotalHolyDay(nDay);


	COleDateTime OleDelayDay = COleDateTime::GetCurrentTime();
	OleDelayDay.SetDateTime(OleDelayDay.GetYear(), OleDelayDay.GetMonth(), OleDelayDay.GetDay(), 0, 0, 0);
	
	COleDateTimeSpan DelayDay;
	DelayDay.SetDateTimeSpan(nDay, 0, 0, 0);		
	OleDelayDay.operator += (DelayDay);

	m_strWaitLoanDate = OleDelayDay.Format(_T("%Y/%m/%d"));

	DelayDay.SetDateTimeSpan(1, 0, 0, 0);		
	OleDelayDay.operator += (DelayDay);
	m_strReturnPlanDate = OleDelayDay.Format(_T("%Y/%m/%d"));

	strQuery.Format(
		_T("SELECT VALUE_NAME ")
        _T("FROM ESL_MANAGE_TBL ")
        _T("WHERE GROUP_1 = 'L' AND GROUP_2 = '기타' AND GROUP_3 = '공통' AND CLASS_ALIAS = 'SMS버젼정보' ")
		_T("AND MANAGE_CODE = '%s'"),
		m_pInfo->MANAGE_CODE);
	
	ids = TmpDM.GetOneValueQuery(strQuery, strValue);
	if(0 > ids) return FALSE;

	if(_T("N") == strValue) m_bUseSMS = FALSE;
	else m_bUseSMS = TRUE;


	strQuery.Format(
		_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE CLASS_ALIAS = '책두레시스템' AND VALUE_NAME = '신청제한되는복귀(만료)횟수'")
		);

	ids = TmpDM.GetOneValueQuery(strQuery, strValue);
	if(0 > ids) return FALSE;
	m_strExpireAplStopCnt = strValue;

	strQuery.Format(
		_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE CLASS_ALIAS = '책두레시스템' AND VALUE_NAME = '복귀(만료)로신청제한시제한일수'")
		);

	ids = TmpDM.GetOneValueQuery(strQuery, strValue);
	if(0 > ids) return FALSE;
	m_strExpireAplStopDate = strValue;

	strQuery.Format(
		_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='SMS시스템정보' AND MANAGE_CODE='%s'"), 
		m_pInfo->MANAGE_CODE
		);

	ids = TmpDM.GetOneValueQuery(strQuery, m_strSMSVersion);
	if(0 > ids) return FALSE;

	}
	Refresh(_T(""), _T(""));

	m_strState[0][0] = _T("신청");
	m_strState[0][1] = _T("0001");
	m_strState[1][0] = _T("요청된자료");
	m_strState[1][1] = _T("0002");
	m_strState[2][0] = _T("발송된자료");
	m_strState[2][1] = _T("0004");
	m_strState[3][0] = _T("입수된자료");
	m_strState[3][1] = _T("0014");
	m_strState[4][0] = _T("복귀중자료");
	m_strState[4][1] = _T("0008");
	m_strState[5][0] = _T("발송거절된자료");
	m_strState[5][1] = _T("0011");

	INT nCnt = 0;
	INT nBtnStateCnt = 6;
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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBOOK_COOPERATIVE_GROUP_REQUEST::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ResizeControl();
}

BOOL CBOOK_COOPERATIVE_GROUP_REQUEST::Create(CWnd *pParent)
{
	return CDialog::Create(IDD, pParent);
}

BOOL CBOOK_COOPERATIVE_GROUP_REQUEST::ResizeControl()
{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_grid_REQUEST);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return FALSE;
	
	CRect rDlg = CRect(0, 0, 0, 0);
	GetClientRect(rDlg);

	UINT dwFlag = SW_HIDE;
	CString strCaption;
	GetDlgItemText(IDC_BTN_REQUEST_STATE, strCaption);
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
	INT nBtnStateCnt = 6;
	for(nCnt=0; nCnt < nBtnStateCnt; nCnt++)
	{
		m_pBtnState[nCnt]->ShowWindow(dwFlag);
	}

	rDlg.top += 40;
	pWnd->MoveWindow(rDlg);

	return TRUE;

}

void CBOOK_COOPERATIVE_GROUP_REQUEST::OnSelchangecmbREQUEST() 
{
	// TODO: Add your control notification handler code here	
 	Refresh(_T(""), _T(""));
}


int CBOOK_COOPERATIVE_GROUP_REQUEST::View_BookInfo()
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

	CILL_VIEW_BOOK_INFO_DLG Dlg;
	Dlg.m_pInfo = m_pInfo;
	Dlg.m_pParentDM = m_pDM;
	Dlg.m_pParentGrid = m_pGrid;
	Dlg.m_nIdx = m_pGrid->SelectGetIdx();
	Dlg.m_sBookCooperativeMode = m_sBookCooperativeMode;
	Dlg.m_pLoanShareManager = m_pLoanShareManager;

	Dlg.m_bIsShowChangeRegNo = TRUE;
	Dlg.m_strState = _T("요청");
	Dlg.DoModal();
	
	Refresh();
	RefreshState();

	return 0;
}
int CBOOK_COOPERATIVE_GROUP_REQUEST::ExcutePath(CString strSM)
{
	int ids = 0;
	if( _T("요청_요청") == strSM)
	{
		ids = Request();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("요청_요청거절") == strSM)
	{
		ids = Request_Refuse();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("요청_요청취소") == strSM)
	{
		ids = Request_Cancel();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("요청_입수") == strSM)
	{
		ids = Receive();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("요청_입수취소") == strSM)
	{
		ids = Receive_Cancel();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("요청_복귀(만료)") == strSM)
	{
	}
	else if( _T("요청_복귀") == strSM)
	{
		ids = Return();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("요청_발송거절확인") == strSM)
	{
		ids = Send_Refuse_Complete();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("요청_취소") == strSM)
	{
		ids = Cancel();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("요청_서지정보") == strSM)
	{
		return View_BookInfo();
	}
	else if( _T("요청_이용자정보") == strSM)
	{
		return View_UserInfo();
	}
	else if( _T("요청_상대기관정보") == strSM)
	{
		return View_LibInfo();
	}
	else if( _T("요청_진행정보") == strSM)
	{		
		return View_StateInfo();
	}
	else if(_T("요청_신청") == strSM)
	{
		ids = Ask();
		Refresh();
		RefreshState();

		return ids;
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

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_REQUEST::View_LibInfo()
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
	Dlg.m_strLibAlias = _T("제공도서관_코드");
	Dlg.m_nIdx = m_pGrid->SelectGetIdx();

	Dlg.DoModal();

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_REQUEST::Request()
{
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}
		
	CStringArray aCheckArray;
	aCheckArray.Add(_T("상태|!=|0001"));
	aCheckArray.Add(_T("요청도서관_코드|!=|%m_strLibCode%"));	
	
	UnCheckGrid(aCheckArray);
		
	}
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 요청 가능한 항목이 없습니다.\r\n")
			_T("요청가능한 대상은 아래와 같습니다.\r\n\r\n")			
			_T("1. 진행상태가 신청(0001)인 트랜잭션\r\n")
			_T("2. 요청도서관이 자관인 경우\r\n")			
			);
		return -2;
	}

	CString strNO = Get_Select_TRANS_NO_List(TRUE);
	if(_T("") == strNO) return -3;

 	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	CString strLastWork = GetWorkLogMsg( _T("요청"), __WFILE__, __LINE__ );

	CString strQuery;
	strQuery.Format(
		_T("UPDATE ILL_TRANS_TBL SET ")
		_T("STATE = '0002', ")
		_T("APLREQ_USERID = '%s', ")
		_T("APLREQ_DATE = SYSDATE, ")
		_T("LAST_WORK = '%s' ")
		_T("WHERE ")
		_T("TRANS_NO IN (%s) AND STATE = '0001' AND APL_LIBCODE = '%s';"),
		m_pInfo->USER_ID,
		strLastWork,
		strNO,
		m_strLibCode
		);

	TmpDM.StartFrame();
	TmpDM.AddFrame(strQuery);	

	int ids = TmpDM.SendFrame();
	TmpDM.EndFrame();

	if(_T("D") == m_sBookCooperativeMode)
	{
		ShareInfoCenterUpload(strNO, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,APLREQ_USERID,APLREQ_DATE"));
	}

	return ids;
}

int CBOOK_COOPERATIVE_GROUP_REQUEST::Request_Cancel()
{
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}
		
	CStringArray aCheckArray;
	aCheckArray.Add(_T("상태|!=|0002"));
	aCheckArray.Add(_T("요청도서관_코드|!=|%m_strLibCode%"));	
	
	UnCheckGrid(aCheckArray);
		
	}
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 요청취소 가능한 항목이 없습니다.\r\n")
			_T("요청취소 가능한 대상은 아래와 같습니다.\r\n\r\n")			
			_T("1. 진행상태가 요청된자료(0002)인 트랜잭션\r\n")
			_T("2. 요청도서관이 자관인 경우\r\n")			
			);
		return -2;
	}
	
	CString strNO = Get_Select_TRANS_NO_List(TRUE);
	if(_T("") == strNO) return -3;

 	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	CString strLastWork = GetWorkLogMsg( _T("요청취소"), __WFILE__, __LINE__ );

	CString strQuery;
	strQuery.Format(
		_T("UPDATE ILL_TRANS_TBL SET ")
		_T("STATE = '0001', ")
		_T("APLREQ_USERID = '%s', ")
		_T("APLREQ_DATE = SYSDATE, ")
		_T("LAST_WORK = '%s' ")
		_T("WHERE ")
		_T("TRANS_NO IN (%s) AND STATE = '0002' AND APL_LIBCODE = '%s';"),
		m_pInfo->USER_ID,
		strLastWork,
		strNO,
		m_strLibCode
		);

	TmpDM.StartFrame();
	TmpDM.AddFrame(strQuery);

	int ids = TmpDM.SendFrame();
	TmpDM.EndFrame();
	
	if(_T("D") == m_sBookCooperativeMode)
	{
		ShareInfoCenterUpload(strNO, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,APLREQ_USERID,APLREQ_DATE"));
	}	

	return ids;
}

int CBOOK_COOPERATIVE_GROUP_REQUEST::Receive()
{
	int ids = 0;
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}
		
	CStringArray aCheckArray;
	aCheckArray.Add(_T("상태|!=|0004"));
	aCheckArray.Add(_T("요청도서관_코드|!=|%m_strLibCode%"));	
	
 	ids = UnCheckGrid(aCheckArray);
		
	}
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 입수처리 가능한 항목이 없습니다.\r\n")
			_T("입수처리 가능한 대상은 아래와 같습니다.\r\n\r\n")
			_T("1. 진행상태가 발송된자료(0004)인 트랜잭션\r\n")
			_T("2. 요청도서관이 자관인 경우\r\n")
			);
		return -2;
	}	

	CString strNO = Get_Select_TRANS_NO_List(TRUE);
	if(_T("") == strNO) return -3;

 	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	CString strQuery;
	strQuery.Format(
		_T("UPDATE ILL_TRANS_TBL SET ")
		_T("STATE = '0014', ")
		_T("TAKE_USERID = '%s', ")
		_T("RETURN_PLAN_DATE = TO_DATE('%s','YYYY/MM/DD'), ")
		_T("TAKE_DATE = SYSDATE, ")
		_T("LAST_WORK = '%s' ")
		_T("WHERE ")
		_T("TRANS_NO IN (%s) AND STATE = '0004' AND APL_LIBCODE = '%s';"),
		m_pInfo->USER_ID,
		m_strReturnPlanDate,
		GetWorkLogMsg( _T("입수"), __WFILE__, __LINE__ ),
		strNO,
		m_strLibCode
		);

  	TmpDM.StartFrame();
  	TmpDM.AddFrame(strQuery);

	ids = TmpDM.SendFrame();
	TmpDM.EndFrame();

	if(0 != ids)
	{
		return ids;
	}

	if(_T("D") == m_sBookCooperativeMode)
	{
		ShareInfoCenterUpload(strNO, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,TAKE_USERID,RETURN_PLAN_DATE,TAKE_DATE"));
	}			

	if(TRUE == m_bUseSMS)
	{
		int nResult = AfxMessageBox(
			_T("입수된 도서에 대해 이용자에게 문자 메시지를 발송하시겠습니까?"), 
			MB_YESNO);
		if(IDYES == nResult)
		{
			CString strResult;
			ids = SendSMS(_T("입수"), strNO, strResult);

			AfxMessageBox(strResult);

			return ids;
		}
	}

	return 0;
}


int CBOOK_COOPERATIVE_GROUP_REQUEST::Receive_Cancel()
{
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}
		
	CStringArray aCheckArray;
	aCheckArray.Add(_T("상태|!=|0014"));
	if(_T("I") == m_sBookCooperativeMode)
	{
		aCheckArray.Add(_T("대출정보_키|!=|%NULL%"));
	}
	else if(_T("D") == m_sBookCooperativeMode)
	{
		aCheckArray.Add(_T("대출일|!=|%NULL%"));
	}
	
	aCheckArray.Add(_T("요청도서관_코드|!=|%m_strLibCode%"));
	
	UnCheckGrid(aCheckArray);
	
	}
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 입수취소 가능한 항목이 없습니다.\r\n")
			_T("입수취소가 가능한 대상은 아래와 같습니다.\r\n\r\n")
			_T("1. 진행상태가 입수된자료(0014)인 트랜잭션\r\n")
			_T("2. 대출상태가 '대출대기중'인 트랜잭션\r\n")
			_T("3. 요청도서관이 자관인 경우\r\n")
			);
		return -1;
	}	
	
	CString strNO = Get_Select_TRANS_NO_List(TRUE);
	if(_T("") == strNO) return -1;

 	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	CString strLastWork = GetWorkLogMsg( _T("입수취소"), __WFILE__, __LINE__ );

	CString strQuery;
	strQuery.Format(
		_T("UPDATE ILL_TRANS_TBL SET ")
		_T("STATE = '0004', ")
		_T("TAKE_USERID = '%s', ")
		_T("TAKE_DATE = SYSDATE, ")
		_T("RETURN_PLAN_DATE = NULL, ")
		_T("LAST_WORK = '%s' ")
		_T("WHERE ")
		_T("TRANS_NO IN (%s) AND STATE = '0014' AND %s IS NULL AND APL_LIBCODE = '%s';"),
		m_pInfo->USER_ID,
		strLastWork,
		strNO,
		(_T("I") == m_sBookCooperativeMode) ? _T("LOAN_KEY") : _T("LS_LOAN_DATE"),
		m_strLibCode
		);

 	TmpDM.StartFrame();
	TmpDM.AddFrame(strQuery);

	int ids = TmpDM.SendFrame();
	TmpDM.EndFrame();

	if(_T("D") == m_sBookCooperativeMode)
	{
		ShareInfoCenterUpload(strNO, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,TAKE_USERID,TAKE_DATE,RETURN_PLAN_DATE"));
	}			

	return ids;

}

int CBOOK_COOPERATIVE_GROUP_REQUEST::Return()
{
	int ids = 0;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_REQUEST::Return()");

	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}
		
	CStringArray aCheckArray;
	aCheckArray.Add(_T("상태|!=|0014"));
	
	ids = UnCheckGrid(aCheckArray);
		
	}

	{
	m_pGrid->SelectMakeList();
	if(0 < m_pGrid->SelectGetCount())
	{	
		int nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{
			BOOL bIsReturn = FALSE;
			ids = IsReturnPrerequisite(nIdx, bIsReturn);
			if(0 > ids) return ids;

			if(FALSE == bIsReturn)
			{ 
				m_pGrid->SetAt(nIdx, m_pGrid->m_nSelectIdx, _T("")); 
			}
			
			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);
	}
	}
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 복귀 가능한 항목이 없습니다.\r\n")
			_T("복귀가 불가능한 대상은 아래와 같습니다.\r\n\r\n")			
			_T("1. 진행상태가 입수된자료(0014)가 아닌 트랜잭션\r\n")
			_T("2. 복귀예정일이 지나지 않은 자료\r\n")
			_T("3. 요청도서관이 자관이 아닌경우(대출상태가 대출대기중인 경우)\r\n")
			_T("4. 책이 대출되어 반납되지 않은 자료(대출상태가 대출중인 경우)\r\n")
			_T("5. 반납도서관이 자관이 아닌경우(대출상태가 반납완료인 경우)")
			);
		return -1;
	}

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);
	
	SetCheckAppendixAndMother(TRUE);

	CString strQuery;
	strQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE VALUE_NAME = '처리일기준패널티부여' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
	CString strValue;
	ids = TmpDM.GetOneValueQuery(strQuery, strValue);
	if ( ids == -4007 ) strValue = _T("N");   
	else if(0 > ids) return ids;
	
	if(_T("Y") == strValue)
	{	
		CString strInQuery = _T("");
		m_pGrid->SelectMakeList();
		CString strTransNo;
		int nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{
			CString strAppendixYN;
			ids = m_pDM->GetCellData(_T("부록여부"), nIdx, strAppendixYN);
			if(_T("Y")==strAppendixYN) continue;

			ids = m_pDM->GetCellData(_T("트랜잭션번호"), nIdx, strTransNo);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
			CESL_DataMgr TempDM;
			TempDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
			CString strExpireCnt = _T("");
			strQuery.Format(_T("SELECT ILL_EXPIRE_RETURN_CNT FROM CO_LOAN_USER_TBL WHERE REC_KEY = (SELECT USER_KEY FROM ILL_TRANS_TBL WHERE TRANS_NO = '%s')"),strTransNo);			
			TempDM.RestructDataManager(strQuery);
			TempDM.GetCellData(0,0,strExpireCnt);
			if(strExpireCnt.Compare(_T(""))==0)
			{
				TempDM.FreeData();
				strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET ILL_EXPIRE_RETURN_CNT=0 WHERE REC_KEY = (SELECT USER_KEY FROM ILL_TRANS_TBL WHERE TRANS_NO = '%s');"),strTransNo);
				TempDM.ExecuteQuery(strQuery,TRUE);
				
			}
 			CString strLoanKey;
 			strQuery.Format(_T("SELECT LOAN_KEY FROM ILL_TRANS_TBL WHERE TRANS_NO = '%s'"),strTransNo);
 			TmpDM.GetOneValueQuery(strQuery,strLoanKey);
 			if(_T("") == strLoanKey)
 			{
 				if(_T("") == strInQuery) strInQuery += strTransNo;
 				else strInQuery += (_T("','") + strTransNo);
 			}
			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);
	
 		if(_T("") != strInQuery)
 		{
 			TmpDM.StartFrame();
 			CString strUserExpireQuery;
 			strInQuery = _T("'") + strInQuery + _T("'");
 			strUserExpireQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET ILL_EXPIRE_RETURN_CNT = ILL_EXPIRE_RETURN_CNT + 1 WHERE REC_KEY IN (SELECT USER_KEY FROM ILL_TRANS_TBL WHERE TRANS_NO IN (%s));"), strInQuery);
 			TmpDM.AddFrame(strUserExpireQuery);
 			TmpDM.SendFrame();
 			TmpDM.EndFrame();
 		}
	}
	
	TmpDM.StartFrame();

	m_pGrid->SelectMakeList();
	if(0 < m_pGrid->SelectGetCount())
	{	
		CString strUserKey, strLoanKey, strReturnLibCode;
		int nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{
			CString strAppendixYN;
			ids = m_pDM->GetCellData(_T("부록여부"), nIdx, strAppendixYN);
			if(_T("Y")==strAppendixYN) continue;

			CString strTransNo;
			ids = m_pDM->GetCellData(_T("트랜잭션번호"), nIdx, strTransNo);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

 			CString strQuery;
 			strQuery.Format(
 				_T("UPDATE ILL_TRANS_TBL SET ")
 				_T("STATE = '0008', ")
 				_T("RETURN_USERID = '%s', ")
 				_T("RETURN_DATE = SYSDATE, ")
 				_T("LAST_WORK = '%s' ")
 				_T("WHERE ")
 				_T("TRANS_NO = '%s';"),
 				m_pInfo->USER_ID,
 				GetWorkLogMsg( _T("복귀"), __WFILE__, __LINE__ ),
 				strTransNo		
 				);

			if(_T("D") == m_sBookCooperativeMode)
			{
				m_pDM->GetCellData(_T("반납도서관_코드"), nIdx, strReturnLibCode);
				if(_T("") == strReturnLibCode)
				{
 					strQuery.Format(
 						_T("UPDATE ILL_TRANS_TBL SET ")
						_T("LOAN_KEY = NULL, ")
 						_T("STATE = '0008', ")
 						_T("RETURN_USERID = '%s', ")
 						_T("RETURN_DATE = SYSDATE, ")
 						_T("LAST_WORK = '%s' ")
 						_T("WHERE ")
 						_T("TRANS_NO = '%s';"),
 						m_pInfo->USER_ID,
 						GetWorkLogMsg( _T("복귀"), __WFILE__, __LINE__ ),
 						strTransNo		
 						);
					m_pDM->GetCellData(_T("대출정보_키"), nIdx, strLoanKey);
					TmpDM.MakeDeleteFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey);
				}
			}
 			TmpDM.AddFrame(strQuery);

			CString strUserTblQuery;			
		
			ids = Get_UpdateUserTblQuery_For_ExpireReturn(nIdx, strUserTblQuery);

			if(0 > ids) return ids;

			if(_T("") != strUserTblQuery)
			{
				TmpDM.AddFrame(strUserTblQuery);
			}

			ids = TmpDM.SendFrame();
				
			if(_T("D") == m_sBookCooperativeMode)
			{
				ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,RETURN_USERID,RETURN_DATE"));
				if(_T("") != strUserTblQuery)
				{					
					m_pDM->GetCellData(_T("이용자KEY"), nIdx, strUserKey);
					ShareInfoCenterUpload(strUserKey, _T("U"), _T("CO_LOAN_USER_TBL"), _T("ILL_EXPIRE_RETURN_CNT,ILL_APL_STOP_SET_DATE,ILL_APL_STOP_DATE"));
				}
		
				m_pDM->GetCellData(_T("반납도서관_코드"), nIdx, strReturnLibCode);
				if(_T("") == strReturnLibCode)
				{					

				}
			}

			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);
	}	
	
	TmpDM.EndFrame();
	return ids;
}


int CBOOK_COOPERATIVE_GROUP_REQUEST::Request_Refuse()
{
	int ids = 0;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_REQUEST::Request_Refuse()");

	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}
	
	CStringArray aCheckArray;
	aCheckArray.Add(_T("상태|!=|0001"));
	aCheckArray.Add(_T("요청도서관_코드|!=|%m_strLibCode%"));

	ids = UnCheckGrid(aCheckArray);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
	}

	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 요청거절 가능한 항목이 없습니다.\r\n")
			_T("요청거절 가능한 대상은 아래와 같습니다.\r\n\r\n")
			_T("1. 자관에서 신청된 자료\r\n")
			_T("1. 진행상태가 신청된자료(0001)인 트랜잭션")			
			);
		return -1;
	}

	CREFUSE_DLG Dlg;
	Dlg.m_DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	Dlg.m_DM.SetManageCode(m_pInfo->MANAGE_CODE);
	Dlg.m_strClass = _T("i02");
	if(IDOK != Dlg.DoModal()) { return -2; }

	CString strAPLREQ_CANCEL = Dlg.m_strCancelCode;

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

		CString strQuery = _T("");	
		strQuery.Format(
			_T("UPDATE ")
				_T("ILL_TRANS_TBL ")
			_T("SET ")
				_T("STATE = '0012', ")
				_T("APLREQ_USERID = '%s', ") 
				_T("APLREQ_DATE = SYSDATE, ")
				_T("APLREQ_CANCEL = '%s', ")
				_T("APLREQ_CONTENT = '%s', ")
				_T("END_DIV = '0002', ")	
 				_T("END_DATE = SYSDATE, ") 
 				_T("END_USERID = '%s', ")
				_T("LAST_WORK = '%s' ")
			_T("WHERE ")	
			_T("TRANS_NO = '%s' AND STATE = '0001' AND APL_LIBCODE = '%s';"),
			m_pInfo->USER_ID,
			strAPLREQ_CANCEL,
			strContent,
			m_pInfo->USER_ID,
			GetWorkLogMsg( _T("요청거절"), __WFILE__, __LINE__ ),
			strTransNo,
			m_strLibCode
			);
		TmpDM.AddFrame(strQuery);
		}
		if(_T("D") != m_sBookCooperativeMode)
		{
		CString strQuery = _T("");
		ids = Get_UpdateBookTblQuery_For_CompleteTransaction(
			nIdx,
			GetWorkLogMsg( _T("요청거절"), __WFILE__, __LINE__ ),
			strQuery
			);

		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-7, strFuncName);
		TmpDM.AddFrame(strQuery);

		ids = Get_UpdateIdxTblQuery(
			nIdx,
			GetWorkLogMsg( _T("요청거절"), __WFILE__, __LINE__ ),
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
			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,APLREQ_USERID,APLREQ_DATE,APLREQ_CANCEL,APLREQ_CONTENT,END_DIV,END_DATE,END_USERID"));
		}	

		nIdx = m_pGrid->SelectGetNext();
	}while(0 <= nIdx);


	if(TRUE == m_bUseSMS)
	{
		CString strNO = Get_Select_TRANS_NO_List(TRUE);
		CString strQuery;
		int nResult = AfxMessageBox(
			_T("요청거절된 도서에 대해 이용자에게 문자 메시지를 발송하시겠습니까?"), 
			MB_YESNO);
		if(IDYES == nResult)
		{
			CString strResult;
			ids = SendSMS(_T("요청거절"), strNO, strResult);


			AfxMessageBox(strResult);
			return ids;
		}
	}

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_REQUEST::Send_Refuse_Complete()
{
	int ids = 0;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_REQUEST::Send_Refuse_Complete()");

	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}
	
	CStringArray aCheckArray;
	aCheckArray.Add(_T("상태|!=|0011"));
	aCheckArray.Add(_T("요청도서관_코드|!=|%m_strLibCode%"));
	aCheckArray.Add(_T("완료유형|!=|%NULL%"));
	
	ids = UnCheckGrid(aCheckArray);
		
	}
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 발송거절확인이 가능한 항목이 없습니다.\r\n")
			_T("발송거절확인이 가능한 대상은 아래와 같습니다.\r\n\r\n")
			_T("1. 자관에서 신청된 자료\r\n")
			_T("2. 진행상태가 발송거절된자료(0011)인 트랜잭션\r\n")
			_T("3. 발송거절확인이 되지않은 자료")
			);
		return -1;
	}

	int nIdx = m_pGrid->SelectGetHeadPosition();
	do
	{
		{
		CString strCode = _T("");
		ids = m_pDM->GetCellData(_T("발송거절사유_코드"), nIdx, strCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
		
		CString strContent = _T("");
		ids = m_pDM->GetCellData(_T("발송비고"), nIdx, strContent);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
		
		CREFUSE_VIEW_DLG Dlg;
		Dlg.m_DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		Dlg.m_DM.SetManageCode(m_pInfo->MANAGE_CODE);
		Dlg.m_strClass = _T("i03");
		Dlg.m_strCode = strCode;
		Dlg.m_strContent = strContent;
		
		if(IDOK != Dlg.DoModal()) { return -1; }
		}		

		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);
		TmpDM.StartFrame();

		CString strTransNo = _T("");
		{
		ids = m_pDM->GetCellData(_T("트랜잭션번호"), nIdx, strTransNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);

		CString strQuery;
		strQuery.Format(
			_T("UPDATE ILL_TRANS_TBL SET ")		
			_T("END_USERID = '%s', ") 
			_T("END_DATE = SYSDATE, ")		
			_T("END_CONTENT = GIVE_CONTENT, ")	
			_T("END_DIV = '0004', ")	
			_T("LAST_WORK = '%s' ")
			_T("WHERE ")	
			_T("TRANS_NO = '%s' AND STATE = '0011' AND END_DIV IS NULL AND APL_LIBCODE = '%s';"),
			m_pInfo->USER_ID,	
			GetWorkLogMsg( _T("발송거절확인"), __WFILE__, __LINE__ ),
			strTransNo,
			m_strLibCode
			);
		TmpDM.AddFrame(strQuery);
		} 
		ids = TmpDM.SendFrame(TRUE);	
		TmpDM.EndFrame();
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);	 	
		}

		if(_T("D") == m_sBookCooperativeMode)
		{
			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("END_USERID,END_DATE,END_CONTENT,END_DIV"));			
		}

		nIdx = m_pGrid->SelectGetNext();
	}while(0 <= nIdx);

	if(TRUE == m_bUseSMS)
	{
		CString strNO = Get_Select_TRANS_NO_List(TRUE);
		CString strQuery;
		int nResult = AfxMessageBox(
			_T("발송거절된 도서에 대해 이용자에게 문자 메시지를 발송하시겠습니까?"), 
			MB_YESNO);
		if(IDYES == nResult)
		{
			CString strResult;
			ids = SendSMS(_T("발송거절"), strNO, strResult);

			AfxMessageBox(strResult);
			return ids;
		}
	}
	return 0;
}


BOOL CBOOK_COOPERATIVE_GROUP_REQUEST::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_ESCAPE == pMsg->wParam)
		{	
			return TRUE;
		}	
		else if(VK_F2 == pMsg->wParam)
		{
			CFileFind FF;
			if(TRUE == FF.FindFile(_T("..\\cfg\\_debug")))
			{
				CILL_APL Dlg(this);
				Dlg.m_pInfo = m_pInfo;
				Dlg.m_bIsVolReserve = m_pLoanMangeValue->m_bVolReserve;
				Dlg.m_strUnityLoanCntYN = m_pLoanMangeValue->m_sUnityLoanCntYN;
				Dlg.m_sBookCooperativeMode = m_pLoanMangeValue->m_sBookCooperativeMode;
				Dlg.m_pLoanShareManager = m_pLoanShareManager;
				Dlg.m_pLoanMangeValue = m_pLoanMangeValue;
				
				Dlg.DoModal();
			}
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

int CBOOK_COOPERATIVE_GROUP_REQUEST::Cancel()
{
	int ids = 0;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_REQUEST::Cancel()");


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
	aCheckArray.Add(_T("요청도서관_코드|!=|%m_strLibCode%"));

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
			_T("3. 요청도서관이 자관이 아닌 자료")
			);
		return -1;
	}

	CREFUSE_DLG Dlg;
	Dlg.m_DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	Dlg.m_DM.SetManageCode(m_pInfo->MANAGE_CODE);
	Dlg.m_strClass = _T("i04");	// ILL_CODE_TBL의 완료 클래스

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
		CString strCancelStatus = _T("");

		if ( strEndDiv==_T("0099") )
		{
			strCancelStatus = _T("0009");
		}
		else
		{
			strCancelStatus = _T("0008");
		}
		strQuery.Format(
			_T("UPDATE ")
				_T("ILL_TRANS_TBL ")
			_T("SET ")
				_T("LOAN_KEY = NULL, ")	
				_T("STATE = '%s', ")	
				_T("END_USERID = '%s', ") 
				_T("END_DATE = SYSDATE, ")
				_T("END_DIV = '%s', ")
				_T("END_CONTENT= '%s', ")
				_T("LAST_WORK = '%s' ")
				_T("WHERE ")
				_T("TRANS_NO = '%s';"),
				strCancelStatus,
				m_pInfo->USER_ID,
				strEndDiv,
				strEndContent,
				GetWorkLogMsg( _T("취소"), __WFILE__, __LINE__ ),
				strTransNo
			);
		TmpDM.AddFrame(strQuery);
		}	
		if ( strEndDiv==_T("0099") ) 
		{
			if(_T("D") != m_sBookCooperativeMode)
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
			else
			{
				m_pDM->GetCellData(_T("대출정보_키"), nIdx, strLoanKey);
				if(FALSE == strLoanKey.IsEmpty())
				{
					TmpDM.MakeDeleteFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey);
				}
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

		nIdx = m_pGrid->SelectGetNext();
	}while(0 <= nIdx);

	return ids;	
}

int CBOOK_COOPERATIVE_GROUP_REQUEST::Ask()
{
	if(_T("I") == m_pLoanMangeValue->m_sBookCooperativeMode)
	{
		CILL_APL Dlg(this);
		Dlg.m_pInfo = m_pInfo;
		Dlg.m_bIsVolReserve = m_pLoanMangeValue->m_bVolReserve;
		Dlg.m_strUnityLoanCntYN = m_pLoanMangeValue->m_sUnityLoanCntYN;
		Dlg.m_sBookCooperativeMode = m_pLoanMangeValue->m_sBookCooperativeMode;
		Dlg.m_pLoanShareManager = m_pLoanShareManager;
		Dlg.m_pLoanMangeValue = m_pLoanMangeValue;

		Dlg.DoModal();
	}
	else
	{
		CILL_APLWEB Dlg(this);
		Dlg.DoModal();
	}
	return 0;
}

int CBOOK_COOPERATIVE_GROUP_REQUEST::IsReturnPrerequisite(int nRow, BOOL &bIsReturn)
{
	int ids = -1;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_REQUEST::IsReturnPrerequisite");

	if(NULL == m_pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
	if(0 > nRow || m_pDM->GetRowCount() <= nRow) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
	
	
	{
	CString strRetrunLibCode;
	ids = m_pDM->GetCellData(_T("반납도서관_코드"), nRow, strRetrunLibCode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

	CString strLibCodeAlias = (_T("") == strRetrunLibCode) ? _T("요청도서관_코드") : _T("반납도서관_코드");

	CString strLibCode;
	ids = m_pDM->GetCellData(strLibCodeAlias, nRow, strLibCode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

	if(strLibCode != m_strLibCode) 
	{
		bIsReturn = FALSE;

		return 0;
	}

	} 
	{ 

	BOOL bWhere1 = FALSE, bWhere2 = FALSE, bWhere3 = FALSE;
	
	CString strResult = _T("");
	CString strLOAN_KEY, strRetrunLibCode;
	
	if(_T("D") != m_sBookCooperativeMode)
	{
		ids = m_pDM->GetCellData(_T("대출정보_키"), nRow, strLOAN_KEY);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
		if(_T("") == strLOAN_KEY) bWhere1 = TRUE;
	}
	else
	{
		ids = m_pDM->GetCellData(_T("대출일"), nRow, strLOAN_KEY);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
		if(_T("") == strLOAN_KEY) bWhere1 = TRUE;
	}
	if(FALSE == bWhere1)
	{
		ids = m_pDM->GetCellData(_T("반납도서관_코드"), nRow, strRetrunLibCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
		if(_T("") != strRetrunLibCode) bWhere2 = TRUE;
	}	

	CString strReturnPlanDate;
	ids = m_pDM->GetCellData(_T("복귀예정일"), nRow, strReturnPlanDate);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);
	
	int nYear = -1, nMonth = -1, nDay = -1;			
	_stscanf(strReturnPlanDate.GetBuffer(0), _T("%d/%d/%d"), 
		&nYear, &nMonth, &nDay);
	
	COleDateTime dtReturnPlanDate;
	dtReturnPlanDate.SetDate(nYear, nMonth, nDay);
	
	COleDateTime dtSysDate = COleDateTime::GetCurrentTime();
	
	if(dtSysDate > dtReturnPlanDate)
	{
		bWhere3 = TRUE;
	}
	
	if(				
		( (TRUE == bWhere1) || (TRUE == bWhere2) ) && 
		TRUE == bWhere3
		) 
	{ 
		bIsReturn = TRUE;
	}	
	else 
	{ 
		bIsReturn = FALSE; 
		return 0;
	}

	} 
		
	return 0;	
}
	
int CBOOK_COOPERATIVE_GROUP_REQUEST::Get_UpdateUserTblQuery_For_ExpireReturn(int nRow, CString &strQuery)
{
	int ids = -1;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_REQUEST::Get_UpdateUserTblQuery_For_ExpireReturn");

	if(0 == _ttoi(m_strExpireAplStopCnt)) return 0;
	if(0 == _ttoi(m_strExpireAplStopDate)) return 0;

	strQuery = _T("");


	CString strLoanKey;
	ids = m_pDM->GetCellData(_T("대출정보_키"), nRow, strLoanKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
	if(_T("") != strLoanKey) return 0;

	CString strUserKey;
	ids = m_pDM->GetCellData(_T("이용자KEY"), nRow, strUserKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
	if(_T("") == strUserKey) return 0;

	CString strAppendixFlag;
	ids = m_pDM->GetCellData(_T("부록여부"), nRow, strAppendixFlag);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);

	CString strHeadQuery = _T("UPDATE CO_LOAN_USER_TBL CLU");
	CString strMidQuery = _T("SET");
	CString strTailQuery = _T("WHERE CLU.REC_KEY = ") + strUserKey + _T(";");

	CString strAplStopDate;
	CString strExpireReturnCnt;
	{
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		
	CString strExpireQuery;
	strExpireQuery.Format(
		_T("SELECT NVL(ILL_EXPIRE_RETURN_CNT, 0), TO_CHAR(ILL_APL_STOP_DATE, 'YYYY/MM/DD') ")
		_T("FROM CO_LOAN_USER_TBL CLU ")
		_T("WHERE CLU.REC_KEY = %s AND ROWNUM = 1"),
		strUserKey);
	ids = TmpDM.RestructDataManager(strExpireQuery);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
	if(0 >= TmpDM.GetRowCount())
	{
		CString strMsg;
		strMsg.Format(_T("이용자를 찾지 못하였습니다."));
		AfxMessageBox(strMsg);

		return -1;
	}

	ids = TmpDM.GetCellData(0, 0, strExpireReturnCnt);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);

	ids = TmpDM.GetCellData(0, 1, strAplStopDate);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);

	} 

	strQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE VALUE_NAME = '처리일기준패널티부여' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
	CString strValue;
	ids = m_pDM->GetOneValueQuery(strQuery, strValue);
	if(ids == -4007) strValue = _T("N");
	else if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);
	if(_T("V") != strAppendixFlag && _T("Y") != strValue)

	{
		strExpireReturnCnt.Format(_T("%d"), _ttoi(strExpireReturnCnt) +1);
		strMidQuery = strMidQuery + _T(" CLU.ILL_EXPIRE_RETURN_CNT = ") + strExpireReturnCnt + _T(",");
	}
	
	if( (_ttoi(strExpireReturnCnt)  >= _ttoi(m_strExpireAplStopCnt) && _T("V") != strAppendixFlag) )	

	{		
		COleDateTime oleAplStopDate;
		int nYear = 1, nMonth = 1, nDay = 1;
		_stscanf(strAplStopDate.GetBuffer(0), _T("%d/%d/%d"), &nYear, &nMonth, &nDay);
		
		oleAplStopDate.SetDate(nYear, nMonth, nDay);
		
		COleDateTime oleToday = COleDateTime::GetCurrentTime();
		oleToday.SetDate(oleToday.GetYear(), oleToday.GetMonth(), oleToday.GetDay());	
		
		if(oleToday > oleAplStopDate)
		{
			CString strExpireAplStopDate;
			strExpireAplStopDate.Format(_T("%d"), _ttoi(m_strExpireAplStopDate)-1); // 당일포함이기 때문에 -1을 해준다.			
			strMidQuery = strMidQuery + _T(" CLU.ILL_APL_STOP_SET_DATE = SYSDATE,");
			strMidQuery = strMidQuery + _T(" CLU.ILL_APL_STOP_DATE = TO_CHAR(SYSDATE+") + strExpireAplStopDate + _T(", 'YYYY/MM/DD'),");		
		}
		
	}

	strMidQuery.TrimRight(_T(","));
	
	if(_T("SET") == strMidQuery)
	{
		strQuery = _T("");
		return 0;
	}

	strQuery = strHeadQuery + _T(" ") + strMidQuery + _T(" ") + strTailQuery;

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_REQUEST::Get_TblName(CString &strTblName, CString strMode, CString strSearchState)
{
	int ids = -1;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_REQUEST::Get_TblName");

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_REQUEST"), _T("멀티콤보_진행상태"));
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

	CString strReceiveTbl;	
	strReceiveTbl.Format( 
	_T("(")
		_T("SELECT ITL.*, ")
			_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
			_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
			_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")
		_T("FROM ILL_TRANS_TBL ITL WHERE ")
		_T("ITL.STATE = '0014' AND ")
		_T("((ITL.RETURN_LIBCODE IS NULL AND ITL.APL_LIBCODE = '%s') OR (ITL.RETURN_LIBCODE = '%s'))")
		
	_T(")"), m_strLibCode, m_strLibCode);

	CString strReturnTbl;	
	strReturnTbl.Format( 
	_T("(")
		_T("SELECT ITL.*, ")
			_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
			_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
			_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")
		_T("FROM ILL_TRANS_TBL ITL WHERE ")
		_T("ITL.STATE = '0008' AND ")
		_T("((ITL.RETURN_LIBCODE = '%s') OR (ITL.APL_LIBCODE = '%s' AND ITL.RETURN_LIBCODE IS NULL)) ")		
	_T(")"), m_strLibCode, m_strLibCode);

	CString strState = _T("'0001', '0002', '0004', '0011'");

	CString strFullTbl;
	strFullTbl.Format(
		_T("( SELECT * FROM ( %s UNION %s UNION ")
			_T("(SELECT ITL.*, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")
			_T("FROM ILL_TRANS_TBL ITL WHERE ")
			_T("ITL.APL_LIBCODE = '%s' AND ")
			_T("ITL.STATE IN (%s) AND ITL.END_DIV IS NULL)))"),
			strReceiveTbl, strReturnTbl, m_strLibCode, strState);
	
	CString strOption = _T("");	

	CString strTableName = _T("");	
	if(	_T("0001") == strStateCode ||
		_T("0002") == strStateCode ||
		_T("0004") == strStateCode ||
		_T("0011") == strStateCode ||
		_T("0012") == strStateCode
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
			)
		{
			strStateQuery = _T("AND END_DATE BETWEEN TO_CHAR(ADD_MONTHS(SYSDATE, -3), 'YYYY/MM/DD') AND SYSDATE AND END_DIV IS NOT NULL");
		}		

		strTableName.Format(
			_T("(SELECT ITL.*, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")
			_T("FROM ILL_TRANS_TBL  ITL WHERE ITL.APL_LIBCODE = '%s' AND ITL.STATE = '%s'%s) A"),
			m_strLibCode, strStateCode, strStateQuery);
	}	
	else if(_T("0008") == strStateCode)	
	{
		strTableName = strReturnTbl + _T(" A");
	}

	else if(_T("0014") == strStateCode)	
	{
		strTableName = strReceiveTbl + _T(" A");
	}
	else if(_T("----") == strStateCode)
	{
		strTableName = strFullTbl + _T(" A");		
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
void CBOOK_COOPERATIVE_GROUP_REQUEST::OnDropdowncmbREQUEST() 
{
	int ids = -1;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_REQUEST::OnDropdowncmbREQUEST");

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_REQUEST"), _T("멀티콤보_진행상태"));
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

int CBOOK_COOPERATIVE_GROUP_REQUEST::ResetComboBox()
{
	int ids = -1;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_REQUEST::ResetComboBox");

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_REQUEST"), _T("멀티콤보_진행상태"));
	if(NULL == pCMB) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	const int COL_COUNT = 2;
	pCMB->ResetContent();
	pCMB->FreeComboData();
	pCMB->InitMultiComboBox(COL_COUNT);	

	const int STATE_COUNT = 9;
	CString strCmbData[STATE_COUNT][COL_COUNT] = 
	{
		{ _T("----"), _T("전체(진행중 자료)") },
		{ _T("0001"), _T("신청")},
		{ _T("0002"), _T("요청된자료")},
		{ _T("0004"), _T("발송된자료")},
		{ _T("0014"), _T("입수된자료")},
		{ _T("0008"), _T("복귀중자료")},
		{ _T("0012"), _T("최근요청거절된자료")},
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

int CBOOK_COOPERATIVE_GROUP_REQUEST::Compare()
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
	Dlg.SetMode(CILL_COMPARE::IDX_REQUEST);

	if(IDOK == Dlg.DoModal())
	{
		ExcutePath(_T("요청_요청거절"));
	}
	return 0;
}

void CBOOK_COOPERATIVE_GROUP_REQUEST::Search(WPARAM wParam, LPARAM lParam)
{
	CILL_SEARCH* pDlg = (CILL_SEARCH*)wParam;

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_REQUEST"), _T("멀티콤보_진행상태"));
	if(NULL == pCMB) ERROR_MSG_RETURN_VOID2(-1, _T("Search"));
	
	const int ROW = pCMB->GetRowCount();
	pCMB->AddItem(_T("    "), 0, ROW);
	pCMB->AddItem(_T("검색된자료"), 1, ROW);
	pCMB->SetCurSel(ROW);
	
 	Refresh(_T("ALL"), pDlg->m_strSQLWhere);
}

int CBOOK_COOPERATIVE_GROUP_REQUEST::SendSMS(const CString strAlias, const CString strTransNoList, CString& strResult)
{
	int ids = 0;
	CString strType;
	CString strQuery;

	CString strLMSUseYN = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	strQuery.Format(_T("SELECT LMS_USE_YN FROM MN_SMS_TOTAL_ENV_TBL WHERE MANAGE_CODE='%s' AND SMS_TYPE='%s'"), m_pInfo->MANAGE_CODE, m_strSMSVersion);
	TmpDM.GetOneValueQuery(strQuery, strLMSUseYN);
	if ( strLMSUseYN.GetLength() == 0 ) strLMSUseYN = _T("N");

	if (g_strKakaoUsed1.IsEmpty())
	{
		strQuery.Format( 
			_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KAKAO플러스친구시스템정보' AND value_name='KAKAO플러스친구사용유무' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		TmpDM.GetOneValueQuery(strQuery, g_strKakaoUsed1);
		
		if (g_strKakaoUsed1.IsEmpty())
		{
			g_strKakaoUsed1 = _T("N");
		}
		if (g_strKakaoUsed1 == _T("Y"))
		{
			strLMSUseYN = _T("Y");
		}
	}

	if(_T("입수") == strAlias)
	{
		strType = _T("TLL01");
		if ( strLMSUseYN == _T("Y") ) {			
			strQuery.Format(
				_T("SELECT U.REC_KEY, U.NAME, U.HANDPHONE, T.TITLE_INFO, U.SMS_USE_YN, 1, U.USER_CLASS ")
				_T("FROM (SELECT TRANS_NO, USER_KEY, TITLE_INFO FROM ILL_TRANS_TBL WHERE TRANS_NO IN (%s) AND STATE = '0014' AND APL_LIBCODE = '%s') T, CO_LOAN_USER_TBL U ")
				_T("WHERE T.USER_KEY = U.REC_KEY ORDER BY U.REC_KEY"),		
				strTransNoList,	m_strLibCode );				
		}
		else {			
			strQuery.Format(
				_T("SELECT U.REC_KEY, MAX(U.NAME), MAX(U.HANDPHONE), MAX(T.TITLE_INFO), MAX(U.SMS_USE_YN), COUNT(1), MAX(U.USER_CLASS) ")
				_T("FROM (SELECT TRANS_NO, USER_KEY, TITLE_INFO FROM ILL_TRANS_TBL WHERE TRANS_NO IN (%s) AND STATE = '0014' AND APL_LIBCODE = '%s') T, CO_LOAN_USER_TBL U ")
				_T("WHERE T.USER_KEY = U.REC_KEY GROUP BY U.REC_KEY"),					
				strTransNoList,	m_strLibCode );
		}
	}
	else if(_T("요청거절") == strAlias)
	{
		strType = _T("TLL02");
		if ( strLMSUseYN == _T("Y") ) {
			strQuery.Format(
				_T("SELECT U.REC_KEY, U.NAME, U.HANDPHONE, T.TITLE_INFO, U.SMS_USE_YN, 1, U.USER_CLASS ")
				_T("FROM (SELECT TRANS_NO, USER_KEY, TITLE_INFO FROM ILL_TRANS_TBL WHERE TRANS_NO IN (%s) AND STATE = '0012' AND APL_LIBCODE = '%s') T, CO_LOAN_USER_TBL U ")
				_T("WHERE T.USER_KEY = U.REC_KEY ORDER BY U.REC_KEY"),	
				strTransNoList,	m_strLibCode);
		}
		else {
			strQuery.Format(
				_T("SELECT U.REC_KEY, MAX(U.NAME), MAX(U.HANDPHONE), MAX(T.TITLE_INFO), MAX(U.SMS_USE_YN), COUNT(1), MAX(U.USER_CLASS) ")
				_T("FROM (SELECT TRANS_NO, USER_KEY, TITLE_INFO FROM ILL_TRANS_TBL WHERE TRANS_NO IN (%s) AND STATE = '0012' AND APL_LIBCODE = '%s') T, CO_LOAN_USER_TBL U ")
				_T("WHERE T.USER_KEY = U.REC_KEY GROUP BY U.REC_KEY"),	
				strTransNoList,	m_strLibCode);
		}
	}
	else if(_T("발송거절") == strAlias)
	{
		strType = _T("TLL03");
		if ( strLMSUseYN == _T("Y") ) {
			strQuery.Format(
			    _T("SELECT U.REC_KEY, U.NAME, U.HANDPHONE, T.TITLE_INFO, U.SMS_USE_YN, 1, U.USER_CLASS ")
				_T("FROM (SELECT TRANS_NO, USER_KEY, TITLE_INFO FROM ILL_TRANS_TBL WHERE TRANS_NO IN (%s) AND STATE = '0011' AND APL_LIBCODE = '%s') T, CO_LOAN_USER_TBL U ")
				_T("WHERE T.USER_KEY = U.REC_KEY ORDER BY U.REC_KEY"),				
				strTransNoList, m_strLibCode);
		}
		else {
			strQuery.Format(
				_T("SELECT U.REC_KEY, MAX(U.NAME), MAX(U.HANDPHONE), MAX(T.TITLE_INFO), MAX(U.SMS_USE_YN), COUNT(1), MAX(U.USER_CLASS) ")
				_T("FROM (SELECT TRANS_NO, USER_KEY, TITLE_INFO FROM ILL_TRANS_TBL WHERE TRANS_NO IN (%s) AND STATE = '0011' AND APL_LIBCODE = '%s') T, CO_LOAN_USER_TBL U ")
				_T("WHERE T.USER_KEY = U.REC_KEY GROUP BY U.REC_KEY"),
				strTransNoList, m_strLibCode);
		}
	}
	else
	{
		strResult = _T("이용자정보 검색에 실패하였습니다.");
		return -1000;
	}

	TmpDM.RestructDataManager(strQuery);
	
	CString strUserKey;
	CString strSendCallNo = m_strLibTel;
	if(_T("") == strSendCallNo)
	{
		strSendCallNo = m_strLibCode;
	}
	strSendCallNo.Replace(_T("-"), _T(""));
	
	CString strUserName;
	CString strReceiveCallNo;
	CString strTitle;
	CString strMsg;
	CString strSMSUseYN;
	CString strBookCnt;
	CString strUserClass;
	INT nUserClassStopCnt = 0;

	CString strNextUserKey = _T(""), strUserAllTitle = _T("");
	INT nUserALLBookCnt = 0, nTotalLMSSendCnt = 0;

	int nRow = 0;
	int nNoCallNoCnt = 0, nNoSMSCnt = 0;
	const int ROW_COUNT = TmpDM.GetRowCount();
	for(nRow = 0; nRow < ROW_COUNT; nRow++)
	{
		if ( strLMSUseYN == _T("Y")) {
			TmpDM.GetCellData(nRow, 0, strUserKey);
			TmpDM.GetCellData(nRow, 1, strUserName);
			TmpDM.GetCellData(nRow, 2, strReceiveCallNo);
			TmpDM.GetCellData(nRow, 3, strTitle);
			TmpDM.GetCellData(nRow, 4, strSMSUseYN);
			TmpDM.GetCellData(nRow, 5, strBookCnt);
			TmpDM.GetCellData(nRow, 6, strUserClass);
			
			if ( nRow < ROW_COUNT-1 )  { 
				TmpDM.GetCellData(nRow+1, 0, strNextUserKey);
				if ( strUserKey == strNextUserKey ) {
					if ( strUserAllTitle.IsEmpty() == FALSE) strUserAllTitle += _T(",\n");
					strUserAllTitle += strTitle;
					nUserALLBookCnt++;
					continue;
				}
				else {
					if ( strUserAllTitle.IsEmpty() == FALSE) strUserAllTitle += _T(",\n");
					strUserAllTitle += strTitle;
					nUserALLBookCnt++;
				}				
			}
			else {
				if ( strUserAllTitle.IsEmpty() == FALSE) strUserAllTitle += _T(",\n");
				strUserAllTitle += strTitle;
				nUserALLBookCnt++;
			}

			strTitle = strUserAllTitle;
			strUserAllTitle.Empty();
			strBookCnt.Format(_T("%d"), nUserALLBookCnt);
			nUserALLBookCnt = 0;
		} 
		else {
			TmpDM.GetCellData(nRow, 0, strUserKey);
			TmpDM.GetCellData(nRow, 1, strUserName);
			TmpDM.GetCellData(nRow, 2, strReceiveCallNo);
			TmpDM.GetCellData(nRow, 3, strTitle);
			TmpDM.GetCellData(nRow, 4, strSMSUseYN);
			TmpDM.GetCellData(nRow, 5, strBookCnt);
			TmpDM.GetCellData(nRow, 6, strUserClass);
		}
	
		if(strUserClass.Compare(_T("0")) != 0 && strAlias.Compare(_T("입수")) == 0){
			nUserClassStopCnt++;
			continue;
		}

		strReceiveCallNo.Replace(_T("-"), _T(""));
		
		if(_T("") == strReceiveCallNo)
		{
			nNoCallNoCnt++;
			continue;
		}
		if(_T("Y") != strSMSUseYN)
		{
			nNoSMSCnt++;
			continue;
		}

		nTotalLMSSendCnt++;

		CSMS_MakeMsg MakeSMS(this);
		SMS_CONNECTION_INFO SMS_ConnectionInfo;
		SMS_MSG_INFO SMS_MsgInfo;
		
		CSMS_Env_Info SMSEnvInfo(this);
		SMSEnvInfo.m_pInfo = this->m_pInfo;
		SMSEnvInfo.MakeSMSEnv();
		SMSEnvInfo.GetSMSEnv( strType , &SMS_ConnectionInfo , &SMS_MsgInfo );
		
		SMS_MsgInfo.sDefaultMsg = _T("");
		SMS_MsgInfo.sTitle = _T("");
		SMS_MsgInfo.sMsgComponentInfo = _T("");
		
		CString strQuery;
		strQuery.Format(
			_T("SELECT SMS_MAKE_COMPONENT FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE='%s' AND MANAGE_CODE='%s'"),m_strSMSVersion, m_pInfo->MANAGE_CODE);
		TmpDM.GetOneValueQuery(strQuery,SMS_MsgInfo.sMsgComponentInfo);
		strQuery.Format(
			_T("SELECT NAME FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='%s' AND MANAGE_CODE='%s'"), strType, m_pInfo->MANAGE_CODE
			);
		TmpDM.GetOneValueQuery(strQuery,SMS_MsgInfo.sTitle);
		strQuery.Format(
			_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='%s' AND MANAGE_CODE='%s'"), strType, m_pInfo->MANAGE_CODE);
		TmpDM.GetOneValueQuery(strQuery,SMS_MsgInfo.sDefaultMsg);
	
		CString strDay;
		if(_T("TLL01") == strType)
		{
			strDay = m_strWaitLoanDate;
		}

		CString sMsg,sKatokMsg;

		MakeSMS.MakeSMSMsg( strType , &SMS_MsgInfo , strUserName , strDay, strTitle, strBookCnt, sMsg, &sKatokMsg );

		sMsg.Replace(_T("'"), _T("''"));

		sKatokMsg.Replace(_T("'"), _T("''"));
		if (g_strKakaoUsed1 == _T("Y")){

			strQuery.Format( _T("KAKAOPLUS_TOT_SEND_PROC(%s, 0, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');"), 
				strUserKey, 
				strSendCallNo, 
				strReceiveCallNo, 
				sMsg, 
				_T(""), 
				m_pInfo->MANAGE_CODE, 
				strType, 
				m_pInfo->USER_NAME, 
				GetWorkLogMsg(strAlias, __WFILE__, __LINE__), 
				strUserName,
				SMS_MsgInfo.sTitle,
				sKatokMsg );

			ids = TmpDM.ExecuteQuery(strQuery, TRUE);

		}else {
			
			strQuery.Format(
				_T("SMS_TOT_SEND_PROC(%s, 0, '%s', '%s', '%s', 'PUB', '%s', '%s', '%s', '%s', '%s');"), 
				strUserKey,
				strSendCallNo,
				strReceiveCallNo,
				sMsg,
				m_pInfo->MANAGE_CODE,
				strType,
				m_pInfo->USER_ID,
				GetWorkLogMsg(strAlias, __WFILE__, __LINE__),
				strUserName);
			ids = TmpDM.ExecuteQuery(strQuery, TRUE);

		}
		if(0 > ids)
		{
			strResult = _T("발송에 실패하였습니다.");
			return 1010;
		}
	}
		
	if(0 == ROW_COUNT) {
		strResult.Format(_T("발송가능한 목록이 없습니다."));
	}
	else if(0 < ROW_COUNT) {
		if ( strLMSUseYN == _T("Y")) {
			if(0 < nNoCallNoCnt || 0 < nNoSMSCnt || 0 < nUserClassStopCnt)
			{
				strResult.Format(
					_T("전체 대상 목록			: %d\r\n")
					_T("휴대폰번호가 없어 취소된 이용자	: %d\r\n")
					_T("SMS수신여부가 N이라 취소된 이용자	: %d\r\n"),
					ROW_COUNT, nNoCallNoCnt, nNoSMSCnt );	
				CString strTemp;

				if(g_strKakaoUsed1 == _T("Y")){
					strTemp.Format(_T("발송된 카톡메세지	: %d"),nTotalLMSSendCnt);
					strResult += strTemp;
				}else {
					strTemp.Format(_T("발송된 LMS			: %d"),nTotalLMSSendCnt);
					strResult += strTemp;
				}
				
				if( 0 < nUserClassStopCnt ){
					
					strTemp.Format(_T("\r\n입수시 대출정지라 발송안된 이용자 : %d"), nUserClassStopCnt);
					strResult += strTemp;
				}
			}
			else {
				strResult.Format(_T("발송완료 되었습니다."));
			}
		}
		else {
			if(0 < nNoCallNoCnt || 0 < nNoSMSCnt || 0 < nUserClassStopCnt) {
				strResult.Format(
					_T("전체 대상 이용자			: %d\r\n")
					_T("휴대폰번호가 없어 취소된 이용자	: %d\r\n")
					_T("SMS수신여부가 N이라 취소된 이용자	: %d\r\n")
					_T("발송된 건수			: %d"),
					ROW_COUNT, nNoCallNoCnt, nNoSMSCnt, ROW_COUNT - nNoCallNoCnt - nNoSMSCnt);

				if( 0 < nUserClassStopCnt ){
					CString strTemp;
					strTemp.Format(_T("\r\n입수시 대출정지라 발송안된 이용자 : %d"), nUserClassStopCnt);
					strResult += strTemp;
				}
			}
			else {
				strResult.Format(_T("발송완료 되었습니다."));
			}
		}
	}

	return 0;
}


int CBOOK_COOPERATIVE_GROUP_REQUEST::RefreshState()
{
	CString strCaption;
	GetDlgItemText(IDC_BTN_REQUEST_STATE, strCaption);
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
				_T("WHERE A.BOOK_KEY = BB.REC_KEY AND BB.SPECIES_KEY = IB.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.END_DIV IS NULL AND ")
					_T("(")
 						_T("( A.STATE IN ('0001', '0002', '0004', '0011') AND A.APL_LIBCODE = '%s' AND A.END_DIV IS NULL ) OR ")
	 					_T("( A.STATE = '0008' AND ((A.RETURN_LIBCODE = '%s') OR (A.APL_LIBCODE = '%s' AND A.RETURN_LIBCODE IS NULL)) ) OR ")
 						_T("( A.STATE = '0014' AND ((A.RETURN_LIBCODE IS NULL AND A.APL_LIBCODE = '%s') OR (A.RETURN_LIBCODE = '%s')) ) ")
					_T(")")
				_T("UNION ALL ")
				_T("SELECT A.STATE ")
				_T("FROM ILL_TRANS_TBL A, SE_BOOK_TBL BB, IDX_SE_TBL IB, SE_VOL_TBL V, CO_LOAN_USER_TBL CLU ")
				_T("WHERE A.BOOK_KEY = BB.REC_KEY AND BB.SPECIES_KEY=IB.REC_KEY AND BB.VOL_KEY=V.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.PUBLISH_FORM_CODE = 'SE' AND ")
					_T("(")
 						_T("( A.STATE IN ('0001', '0002', '0004', '0011') AND A.APL_LIBCODE = '%s' AND A.END_DIV IS NULL ) OR ")
	 					_T("( A.STATE = '0008' AND ((A.RETURN_LIBCODE = '%s') OR (A.APL_LIBCODE = '%s' AND A.RETURN_LIBCODE IS NULL)) ) OR ")
 						_T("( A.STATE = '0014' AND ((A.RETURN_LIBCODE IS NULL AND A.APL_LIBCODE = '%s') OR (A.RETURN_LIBCODE = '%s')) ) ")
					_T(")")
				_T("UNION ALL ")
				_T("SELECT A.STATE ")
				_T("FROM ILL_TRANS_TBL A, CO_NON_DB_BOOK_TBL BB, CO_LOAN_USER_TBL CLU ")
				_T("WHERE A.BOOK_KEY = BB.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.PUBLISH_FORM_CODE = 'NB' AND ")
					_T("(")
 						_T("( A.STATE IN ('0001', '0002', '0004', '0011') AND A.APL_LIBCODE = '%s' AND A.END_DIV IS NULL ) OR ")
	 					_T("( A.STATE = '0008' AND ((A.RETURN_LIBCODE = '%s') OR (A.APL_LIBCODE = '%s' AND A.RETURN_LIBCODE IS NULL)) ) OR ")
 						_T("( A.STATE = '0014' AND ((A.RETURN_LIBCODE IS NULL AND A.APL_LIBCODE = '%s') OR (A.RETURN_LIBCODE = '%s')) ) ")
					_T(")")
			_T(") T ")
			_T("GROUP BY T.STATE"),
			m_pInfo->LIB_CODE, m_pInfo->LIB_CODE, m_pInfo->LIB_CODE, m_pInfo->LIB_CODE, m_pInfo->LIB_CODE,
			m_pInfo->LIB_CODE, m_pInfo->LIB_CODE, m_pInfo->LIB_CODE, m_pInfo->LIB_CODE, m_pInfo->LIB_CODE,
			m_pInfo->LIB_CODE, m_pInfo->LIB_CODE, m_pInfo->LIB_CODE, m_pInfo->LIB_CODE, m_pInfo->LIB_CODE
			);

		TmpDM.RestructDataManager(strQuery);

		CString strState, strCnt, strCaption;
		INT nRow = 0;
		INT ROW_COUNT = TmpDM.GetRowCount();

		INT nCnt = 0;
		for(nCnt = 0; nCnt < 6; nCnt++)
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
void CBOOK_COOPERATIVE_GROUP_REQUEST::OnBtnRequestState() 
{
	CString strCaption;
	GetDlgItemText(IDC_BTN_REQUEST_STATE, strCaption);
	if(_T("▲") == strCaption)
	{
		SetDlgItemText(IDC_BTN_REQUEST_STATE, _T("▼"));
	}
	else if(_T("▼") == strCaption)
	{
		SetDlgItemText(IDC_BTN_REQUEST_STATE, _T("▲"));
		RefreshState();
	}

	ResizeControl();
}

void CBOOK_COOPERATIVE_GROUP_REQUEST::OnBtnState(UINT uID)
{
	CString strState = m_strState[uID-4500][1];

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_REQUEST"), _T("멀티콤보_진행상태"));
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

	OnSelchangecmbREQUEST();
}

INT CBOOK_COOPERATIVE_GROUP_REQUEST::MakeValidRegNo(CString &strGetRegNo)
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

INT CBOOK_COOPERATIVE_GROUP_REQUEST::SelectSearchHistory()
{
EFS_BEGIN

	INT ids;
	CString strBookRegNo;
	CString strInBarcodeLibCode, strInBarcode, strQuery, strDMLibCode;

	GetDlgItemText(IDC_edtRegNo, strInBarcode);
	
	BOOL bSelected = FALSE;

	if( strInBarcode.IsEmpty() )
	{
		AfxMessageBox( _T("요청자료의 등록번호를 입력해야 합니다") );
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
   
	CESL_DataMgr *pDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_REQUEST") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SelectSearchHistory()") );
			
	if( pDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("검색된 데이터가 없습니다") );
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
						CESL_Grid* pgrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_REQUEST"), _T("그리드_요청자료") );
						pgrid->SetAt(i, pgrid->m_nSelectIdx, pgrid->m_sSelectString);
						pDM->SetCellData(_T("선정"), pgrid->m_sSelectString, i);
						bSelected = TRUE;
						break;
					}
				}
				else {
					CESL_Grid* pgrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_REQUEST"), _T("그리드_요청자료") );
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
			CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_REQUEST"), _T("그리드_요청자료") );
			pGrid->GetAt(j,0,strCheck);
			if(strCheck == _T(""))
				pDM->SetCellData(_T("선정"),_T(""), j);
		}
	}
	if(bSelected)
	{
		pDM->SORT(_T("선정"), TRUE);
		CESL_Grid* pgrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_REQUEST"), _T("그리드_요청자료") );
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

void CBOOK_COOPERATIVE_GROUP_REQUEST::OnbtnSelect() 
{
EFS_BEGIN
	
	SelectSearchHistory();
	SetDlgItemText(IDC_edtRegNo, _T(""));
	( CEdit*) GetDlgItem (IDC_edtRegNo)->SetFocus();

EFS_END
}

INT CBOOK_COOPERATIVE_GROUP_REQUEST::PrintList()
{
EFS_BEGIN
	
	INT ids;

	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_REQUEST") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
		return 1;
	}
	
	if( PrivacyPrintLogRecord(pSrcDM, _T("책두레 제공 목록출력"), _T("E"), 0 ) < 0 ) return 1;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_REQUEST") , _T("그리드_요청자료") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	INT nMode = 0;
	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_REQUEST_PRINT") );
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
		{
			nIndex = i;
		}
		else
		{
			nIndex = pGrid->SelectGetIdx();
		}
		pPrintDM->InsertRow(-1);
		
		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("번호") , sData , i);
		if( ids < 0 ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}
		
		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] ,j , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			}
			
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] ,j , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			}
		}

		if( nMode == 1)
		{
			pGrid->SelectGetNext();
		}
	}
	

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pPrintDM , _T("관리구분") , _T("관리구분") );

	pPrintDM->SORT(_T("청구기호"));

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) 
	{
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);

	ids = pSIReportManager->GetSIReportFileFromDB(_T("책두레_요청"));
	if (ids < 0) 
	{
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}
	
	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->Print();
	if (pSIReportManager) 
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return 0;
	
EFS_END
	return -1;
}

INT CBOOK_COOPERATIVE_GROUP_REQUEST::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	
	return 0;
	
	EFS_END
	return -1;	
}