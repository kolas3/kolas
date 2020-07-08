


#include "stdafx.h"
#include "resource.h"
#include "Imm.h"
#include "ILL_SEARCH.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CILL_SEARCH::CILL_SEARCH(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	
		
	
	m_nClickBtnIdx = -1;
	m_pParentMgr = pParent;
	m_pCMB = NULL;
}


void CILL_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	
	DDX_Control(pDX, IDC_btnILL_SEARCH_LS_RETURN_DATE, mCtrl_btnLsReturnDate);
	DDX_Control(pDX, IDC_btnILL_SEARCH_LS_LOAN_DATE, mCtrl_btnLsLoanDate);

	DDX_Control(pDX, IDC_btnILL_SEARCH_TAKE_DATE, mCtrl_btnTakeDate);
	DDX_Control(pDX, IDC_btnILL_SEARCH_RETURN_PLAN_DATE, mCtrl_btnReturnPlanDate);
	DDX_Control(pDX, IDC_btnILL_SEARCH_RETURN_DATE, mCtrl_btnReturnDate);
	DDX_Control(pDX, IDC_btnILL_SEARCH_GIVE_DATE, mCtrl_btnGiveDate);
	DDX_Control(pDX, IDC_btnILL_SEARCH_END_DATE, mCtrl_btnEndDate);
	DDX_Control(pDX, IDC_btnILL_SEARCH_APLREQ_DATE, mCtrl_btnAplReqDate);
	DDX_Control(pDX, IDC_btnILL_SEARCH_APL_DATE, mCtrl_btnAplDate);
	
}


BEGIN_MESSAGE_MAP(CILL_SEARCH, CDialog)
	
	//{{AFX_MSG_MAP(CILL_SEARCH)
	ON_BN_CLICKED(IDC_btnCLEAR, OnbtnILLSEARCHRESET)
	ON_EN_SETFOCUS(IDC_edtILL_SEARCH_AUTHOR, OnSetfocusedtILLSEARCHAUTHOR)
	ON_EN_SETFOCUS(IDC_edtILL_SEARCH_PUBLISHER, OnSetfocusedtILLSEARCHPUBLISHER)
	ON_EN_SETFOCUS(IDC_edtILL_SEARCH_REGNO, OnSetfocusedtILLSEARCHREGNO)
	ON_EN_SETFOCUS(IDC_edtILL_SEARCH_TITLE, OnSetfocusedtILLSEARCHTITLE)
	ON_EN_SETFOCUS(IDC_edtILL_SEARCH_USERNO_1, OnSetfocusedtILLSEARCHUSERNO1)
	ON_EN_SETFOCUS(IDC_edtILL_SEARCH_USERNO_2, OnSetfocusedtILLSEARCHUSERNO2)
	ON_EN_SETFOCUS(IDC_edtILL_SEARCH_USERNAME_1, OnSetfocusedtILLSEARCHUSERNAME1)
	ON_EN_SETFOCUS(IDC_edtILL_SEARCH_USERNAME_2, OnSetfocusedtILLSEARCHUSERNAME2)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDM_TODAY, IDM_7DAY, OnClickMenu)
	ON_MESSAGE(WM_CLICK_DATE_BTN, OnClickDateBtn)
	ON_BN_CLICKED(IDC_btnILL_SEARCH_RETURN, OnbtnILLSEARCHRETURN)
	ON_WM_SHOWWINDOW()	
END_MESSAGE_MAP()




BOOL CILL_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			OnOK();
		}
	}
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

CILL_SEARCH::~CILL_SEARCH()
{
	if(NULL != m_pCMB) delete m_pCMB;
}

BOOL CILL_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	InitESL_Mgr(_T("ILL_SEARCH"));
	Init_MultiCombo();
	EnableThemeDialogTexture(GetSafeHwnd());

	
	mCtrl_btnAplDate.SetMenu(IDR_MENU_ILL_SEARCH, m_hWnd);
	mCtrl_btnAplDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 0);

	mCtrl_btnAplReqDate.SetMenu(IDR_MENU_ILL_SEARCH, m_hWnd);
	mCtrl_btnAplReqDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 1);

	mCtrl_btnGiveDate.SetMenu(IDR_MENU_ILL_SEARCH, m_hWnd);
	mCtrl_btnGiveDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 2);

	mCtrl_btnTakeDate.SetMenu(IDR_MENU_ILL_SEARCH, m_hWnd);
	mCtrl_btnTakeDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 3);

	mCtrl_btnReturnPlanDate.SetMenu(IDR_MENU_ILL_SEARCH, m_hWnd);
	mCtrl_btnReturnPlanDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 4);

	mCtrl_btnReturnDate.SetMenu(IDR_MENU_ILL_SEARCH, m_hWnd);
	mCtrl_btnReturnDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 5);

	mCtrl_btnEndDate.SetMenu(IDR_MENU_ILL_SEARCH, m_hWnd);
	mCtrl_btnEndDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 6);

	
	
	mCtrl_btnLsLoanDate.SetMenu(IDR_MENU_ILL_SEARCH, m_hWnd);
	mCtrl_btnLsLoanDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 7);

	
	
	mCtrl_btnLsReturnDate.SetMenu(IDR_MENU_ILL_SEARCH, m_hWnd);
	mCtrl_btnLsReturnDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 8);

	ResetComboBox();
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_ILL_SEARCH"), _T("진행상태"));
	if(NULL == pCMB) ERROR_MSG_RETURN_ERROR_CODE2(0, _T("CILL_SEARCH::OnInitDialog()"));

	pCMB->SetCurSel(0);	


	if(MODE_REQUEST == m_nMode)
	{
		CWnd* pWnd = GetDlgItem(IDC_btnILL_SEARCH_RETURN);
		if(NULL != pWnd && NULL != pWnd->GetSafeHwnd()) pWnd->ShowWindow(SW_SHOW);
	}	

	return TRUE;  
	              
}
int CILL_SEARCH::OnClickDateBtn(WPARAM wParam, LPARAM lParam)
{
	m_nClickBtnIdx = (int)lParam;

	return 0;
}
void CILL_SEARCH::OnClickMenu(WPARAM wParam, LPARAM lParam)
{
	


	COleDateTime Today = Today.GetCurrentTime();

	CString strDay;
	strDay.Format(_T("%04d/%02d/%02d"), Today.GetYear(), Today.GetMonth(), Today.GetDay());

	

	const int DAY_CONTROL_COUNT = 9;

	CString strAlias[DAY_CONTROL_COUNT][2] = 
	{
		{ _T("신청일1"), _T("신청일2")}, { _T("요청일1"), _T("요청일2")}, 
		{ _T("발송일1"), _T("발송일2")}, { _T("입수일1"), _T("입수일2")}, 
		{ _T("복귀예정일1"), _T("복귀예정일2")}, { _T("복귀일1"), _T("복귀일2")},
		{ _T("완료일1"), _T("완료일2")}

		
		
		,{ _T("대출일1"), _T("대출일2")}, { _T("반납일1"), _T("반납일2")},
	};
	
 	CECO_ControlDateEdit* pDate1 = (CECO_ControlDateEdit*)
 		FindControl(_T("CM_ILL_SEARCH"), strAlias[m_nClickBtnIdx][0]);
	CECO_ControlDateEdit* pDate2 = (CECO_ControlDateEdit*)
 		FindControl(_T("CM_ILL_SEARCH"), strAlias[m_nClickBtnIdx][1]);
 	if(NULL == pDate1 || NULL == pDate2) return;
 
 	if(IDM_RESET == wParam)
 	{
 		pDate1->Clear();
		pDate2->Clear();
 	}
 	else if(IDM_TODAY == wParam)
 	{
 		pDate1->SetData(strDay);
		pDate2->SetData(strDay);
 	}	
	else if(IDM_7DAY == wParam)
	{
		pDate2->SetData(strDay);

		COleDateTimeSpan SevenDay;
		SevenDay.SetDateTimeSpan(7, 0, 0, 0);		
		Today.operator -= (SevenDay);
		
		strDay.Format(
			_T("%04d/%02d/%02d"), Today.GetYear(), Today.GetMonth(), Today.GetDay());

		pDate1->SetData(strDay);
	}

}

void CILL_SEARCH::OnbtnILLSEARCHRESET() 
{
	
	CESL_ControlMgr* pCM = FindCM(_T("CM_ILL_SEARCH"));
	if(NULL != pCM)
	{
		pCM->AllControlClear();
	}
}

void CILL_SEARCH::OnSetfocusedtILLSEARCHTITLE() 
{
	
	SetIMEStatus(_T("한글"), IDC_edtILL_SEARCH_TITLE);
}

void CILL_SEARCH::OnSetfocusedtILLSEARCHAUTHOR() 
{
	
	SetIMEStatus(_T("한글"), IDC_edtILL_SEARCH_AUTHOR);
}

void CILL_SEARCH::OnSetfocusedtILLSEARCHPUBLISHER() 
{
	
	SetIMEStatus(_T("한글"), IDC_edtILL_SEARCH_PUBLISHER);
}

void CILL_SEARCH::OnSetfocusedtILLSEARCHUSERNAME1() 
{
	
	SetIMEStatus(_T("한글"), IDC_edtILL_SEARCH_USERNAME_1);
}

void CILL_SEARCH::OnSetfocusedtILLSEARCHUSERNAME2() 
{
	
	SetIMEStatus(_T("한글"), IDC_edtILL_SEARCH_USERNAME_2);
}

void CILL_SEARCH::OnSetfocusedtILLSEARCHREGNO() 
{
	
	SetIMEStatus(_T("영문"), IDC_edtILL_SEARCH_REGNO);
}

void CILL_SEARCH::OnSetfocusedtILLSEARCHUSERNO1() 
{
	
	SetIMEStatus(_T("영문"), IDC_edtILL_SEARCH_USERNO_1);
}

void CILL_SEARCH::OnSetfocusedtILLSEARCHUSERNO2() 
{
	
	SetIMEStatus(_T("영문"), IDC_edtILL_SEARCH_USERNO_2);
}

int CILL_SEARCH::SetIMEStatus(CString strMode, UINT uControlID)
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;
	hIME = ::ImmGetContext(GetDlgItem(uControlID)->m_hWnd);
	if(NULL != hIME)
	{
		ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		if(_T("한글") == strMode)
		{
			dwConversion=IME_CMODE_HANGUL;
		}
		else if(_T("영문") == strMode)
		{
			dwConversion = dwConversion & ~IME_CMODE_LANGUAGE;
			dwConversion |= IME_CMODE_ALPHANUMERIC;
		}
		else return -1;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );		
		ImmReleaseContext(GetDlgItem(uControlID)->m_hWnd,hIME); 
	}
	else return -2;

	return 0;
}

void CILL_SEARCH::OnOK() 
{
	

	CString strSQLWhere;

	Get_SQL_For_UserInfo(strSQLWhere);
	Get_SQL_For_BookInfo(strSQLWhere);
	Get_SQL_For_DayControl(strSQLWhere);
	Get_SQL_For_State(strSQLWhere);
	Get_SQL_For_LibInfo(strSQLWhere);
	m_strSQLWhere = strSQLWhere;
	CDialog::OnOK();

	pParentMgr->SendMessage(WM_SEARCH, (WPARAM)this, NULL);
}

int CILL_SEARCH::Get_SQL_For_DayControl(CString &strQuery)
{
	
	const int DAY_CONTROL_COUNT = 9;

	CString strAlias[DAY_CONTROL_COUNT][2] = 
	{
		{ _T("신청일1"), _T("신청일2")}, { _T("요청일1"), _T("요청일2")}, 
		{ _T("발송일1"), _T("발송일2")}, { _T("입수일1"), _T("입수일2")}, 
		{ _T("복귀예정일1"), _T("복귀예정일2")}, { _T("복귀일1"), _T("복귀일2")},
		{ _T("완료일1"), _T("완료일2")}		
		
		,{ _T("대출일1"), _T("대출일2")}, { _T("반납일1"), _T("반납일2")}
	};
	CString strDBAlias[DAY_CONTROL_COUNT] = 
	{
		_T("APL_DATE"), _T("APLREQ_DATE"), _T("GIVE_DATE"), _T("TAKE_DATE"),
		_T("RETURN_PLAN_DATE"), _T("RETURN_DATE"), _T("END_DATE")
		
		,_T("LS_LOAN_DATE"), _T("LS_RETURN_DATE")
	};

	for(int nCnt = 0; nCnt < DAY_CONTROL_COUNT; nCnt++)
	{
 		CECO_ControlDateEdit* pDate1 = (CECO_ControlDateEdit*)
 			FindControl(_T("CM_ILL_SEARCH"), strAlias[nCnt][0]);
		CECO_ControlDateEdit* pDate2 = (CECO_ControlDateEdit*)
 			FindControl(_T("CM_ILL_SEARCH"), strAlias[nCnt][1]);
 		if(NULL == pDate1 || NULL == pDate2) continue;

		CString strDate1, strDate2;
		pDate1->GetData(strDate1);
		pDate2->GetData(strDate2);

		if(_T("") == strDate1 && _T("") == strDate2)
		{
			continue;
		}
		else if(_T("") == strDate1 && _T("") != strDate2)
		{
			strDate1 = strDate2;
		}
		else if(_T("") != strDate1 && _T("") == strDate2)
		{
			strDate2 = strDate1;
		}

		CString strTmpQuery;
		if(strDate1 != strDate2)
		{
			strTmpQuery.Format(
				_T("T.%s BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND ")
				_T("TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')"),
				strDBAlias[nCnt], strDate1, strDate2);
		}
		else
		{
			strTmpQuery.Format(
				_T("TO_CHAR(T.%s, 'YYYY/MM/DD') = '%s'"),
				strDBAlias[nCnt], strDate1);
		}

		strQuery = strQuery + _T(" AND ") + strTmpQuery;
		
	}
	if(0 == strQuery.Find(_T(" AND "))) strQuery = strQuery.Mid(5);
	
	return 0;
}

int CILL_SEARCH::Get_SQL_For_BookInfo(CString &strQuery)
{
	const int LIKE_COUNT = 3;
	CString strLikeAlias[LIKE_COUNT] = 
	{
		_T("서명"), _T("저자"), _T("발행자")
	};
	CString strDBAlias[LIKE_COUNT] = 
	{
		_T("TITLE_INFO"), _T("AUTHOR_INFO"), _T("PUBLISHER_INFO")
	};	

	for(int nCnt = 0; nCnt < LIKE_COUNT; nCnt++)
	{
 		CESL_ControlEdit* pEdit = (CESL_ControlEdit*)
 			FindControl(_T("CM_ILL_SEARCH"), strLikeAlias[nCnt]);
		if(NULL == pEdit) continue;

		CString strData;
		pEdit->GetData(strData);

		if(_T("") != strData)
		{
			CString strTmpQuery;
			
			strData.Replace(_T("'"), _T("''"));
 			strTmpQuery.Format(_T("T.%s LIKE '%%%s%%'"), strDBAlias[nCnt], strData);
			strQuery = strQuery + _T(" AND ") + strTmpQuery;
		}
	}

	{	
 		CESL_ControlEdit* pEdit = (CESL_ControlEdit*)
 			FindControl(_T("CM_ILL_SEARCH"), _T("등록번호"));
		if(NULL == pEdit) return -1;

		CString strRegNoList;
		pEdit->GetData(strRegNoList);
		strRegNoList.TrimLeft();
		strRegNoList.TrimRight();
		strRegNoList.Replace(_T(" "), _T(""));
		strRegNoList.Replace(_T("	"), _T(""));

		if(_T("") != strRegNoList)
		{
			CString strTmpQuery;
			CString strInCheckValue;
			for(int nCnt = 0; ; nCnt++)
			{
				CString strRegQuery;
				if(FALSE == AfxExtractSubString(strInCheckValue, strRegNoList, nCnt, ',')) 
					break;
				
				if(-1 != strInCheckValue.Find(_T("-")))
				{
					CString strReg1;
					CString strReg2;
					AfxExtractSubString(strReg1, strInCheckValue, 0, '-');
					AfxExtractSubString(strReg2, strInCheckValue, 1, '-');
					
					if(_T("") == strReg1 || _T("") == strReg2)
					{
						AfxMessageBox(_T("등록번호의 범위가 잘못되었습니다."));
						return -2;
					}					
					CLocCommonAPI::MakeRegNoSpaceToZero(this, strReg1, NULL);
					CLocCommonAPI::MakeRegNoSpaceToZero(this, strReg2, NULL);
					
					strRegQuery.Format(_T("(T.REG_NO BETWEEN '%s' AND '%s')"),
						strReg1, strReg2);						
				}
				else 
				{
					CLocCommonAPI::MakeRegNoSpaceToZero(this, strInCheckValue, NULL);
					strRegQuery.Format(_T("(T.REG_NO = '%s')"), strInCheckValue);
				}
				if(TRUE == strTmpQuery.IsEmpty())
				{
					strTmpQuery.Format(_T("(%s"), strRegQuery);
				}
				else
				{
					strTmpQuery = strTmpQuery + _T(" OR ") + strRegQuery;
				}
			}
			strTmpQuery = strTmpQuery + _T(")");
			strQuery = strQuery + _T(" AND ") + strTmpQuery;
		}
	}

	{
		 CESL_ControlEdit* pEdit1 = (CESL_ControlEdit*)
 			FindControl(_T("CM_ILL_SEARCH"), _T("발행년도1"));
		if(NULL == pEdit1) return -2;

		 CESL_ControlEdit* pEdit2 = (CESL_ControlEdit*)
 			FindControl(_T("CM_ILL_SEARCH"), _T("발행년도2"));
		if(NULL == pEdit2) return -3;

		CString strDate1, strDate2;
		pEdit1->GetData(strDate1);
		pEdit2->GetData(strDate2);

		if(_T("") == strDate1 && _T("") != strDate2)
		{
			strDate1 = strDate2;
		}
		else if(_T("") != strDate1 && _T("") == strDate2)
		{
			strDate2 = strDate1;
		}
		
		if(_T("") != strDate1 && _T("") != strDate2)
		{
			CString strTmpQuery;
			if(strDate1 != strDate2)
			{
				strTmpQuery.Format(
					_T("T.PUB_YEAR_INFO BETWEEN '%s' AND '%s'"), strDate1, strDate2);
			}
			else
			{
				strTmpQuery.Format(
					_T("T.PUB_YEAR_INFO = '%s'"), strDate1);
			}
			strQuery = strQuery + _T(" AND ") + strTmpQuery;
		}
	}
	
	if(0 == strQuery.Find(_T(" AND "))) strQuery = strQuery.Mid(5);

	return 0;
}

int CILL_SEARCH::Get_SQL_For_UserInfo(CString &strQuery)
{
	int ids = -1;
	CString strFuncName = _T("CILL_SEARCH::Get_SQL_For_UserInfo");

	const int USER_INFO_COUNT = 5;
	CString strAlias[USER_INFO_COUNT] = 
	{
		_T("이용자번호1"), _T("이용자번호2"), _T("이용자이름1"), _T("이용자이름2"), _T("복귀만료횟수")
	};
	CString strValue[USER_INFO_COUNT] = 
	{
		_T(""), _T(""), _T(""), _T(""), _T("")
	};
	for(int nCnt = 0; nCnt < USER_INFO_COUNT; nCnt++)
	{
		CESL_ControlEdit* pEdit = (CESL_ControlEdit*)
 			FindControl(_T("CM_ILL_SEARCH"), strAlias[nCnt]);
		if(NULL == pEdit) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

		pEdit->GetData(strValue[nCnt]);
	}

	CString strTmpQuery;

	if(_T("") != strValue[0] && _T("") == strValue[1])
	{
		strValue[1] = strValue[0];
	}
	else if(_T("") == strValue[0] && _T("") != strValue[1])
	{
		strValue[0] = strValue[1];
	}
	if(_T("") != strValue[0] && _T("") != strValue[1])
	{
		if(strValue[0] == strValue[1])
		{
			strTmpQuery.Format(
				_T("T.USER_NO = '%s'"), strValue[0]);
		}
		else
		{
			strTmpQuery.Format(
				_T("T.USER_NO BETWEEN '%s' AND '%s'"), strValue[0], strValue[1]);
		}
		strQuery = strQuery + _T(" AND ") + strTmpQuery;
	}
	
	if(_T("") != strValue[2] && _T("") == strValue[3])
	{
		strValue[3] = strValue[2] + _T("힝");
	}
	else if(_T("") == strValue[2] && _T("") != strValue[3])
	{
		strValue[2] = strValue[3];
	}
	if(_T("") != strValue[2] && _T("") != strValue[3])
	{
		if(strValue[2] == strValue[3])
		{
			strTmpQuery.Format(
				_T("IDX_USER_NAME = '%s'"), strValue[2]);
		}
		else
		{
			strTmpQuery.Format(
				_T("IDX_USER_NAME BETWEEN '%s' AND '%s'"), strValue[2], strValue[3]);
		}
		strQuery = strQuery + _T(" AND ") + strTmpQuery;
	}

	if(FALSE == strValue[4].IsEmpty())
	{
		
		
		strQuery = strQuery + _T(" AND ILL_EXPIRE_RETURN_CNT >= ") + strValue[4];
	}

	if(0 == strQuery.Find(_T(" AND "))) strQuery = strQuery.Mid(5);

	return 0;
}

int CILL_SEARCH::Get_SQL_For_LibInfo(CString &strQuery)
{
 	const int LIB_INFO_COUNT = 3;
 	CString strAlias[LIB_INFO_COUNT] = 
 	{
 		_T("제공도서관"), _T("요청도서관"), _T("반납도서관")
 	};
 	CString strDBAlias[LIB_INFO_COUNT] = 
 	{
 		_T("GIVE_LIBCODE"), _T("APL_LIBCODE"), _T("RETURN_LIBCODE")
 	};	
 
 	CString strTmpQuery;
 	CESL_ControlMultiComboBox *pCmb = NULL;
 	for(int nCnt = 0; nCnt < LIB_INFO_COUNT; nCnt++)
 	{
 		pCmb = (CESL_ControlMultiComboBox*)FindControl(_T("CM_ILL_SEARCH"), strAlias[nCnt]);
 		if(NULL == pCmb) continue;
 
 		const int nIdx = pCmb->GetCurSel();
 		if(0 > nIdx) continue;
 
 		CString strLibData;
 		pCmb->GetData(strLibData, nIdx, 0);

 		if(FALSE == strLibData.IsEmpty())

 		{
 			strTmpQuery.Format(
 				_T("%s = (SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s' AND ROWNUM = 1)"),
 				strDBAlias[nCnt], strLibData);
 
 			strQuery = strQuery + _T(" AND ") + strTmpQuery;
 		}
 	}
 
 	if(0 == strQuery.Find(_T(" AND "))) strQuery = strQuery.Mid(5);






































	return 0;
}

int CILL_SEARCH::SetMode(MODE nMode, CString strLibCode)
{
	if(MODE_REQUEST == nMode)
	{
	}
	else if(MODE_OFFER == nMode)
	{
	}
	else return -1;

	m_nMode = nMode;
	m_strLibCode = strLibCode;

	return 0;
}

int CILL_SEARCH::Get_SQL_For_State(CString &strQuery)
{
	int ids = -1;
	CString strFuncName = _T("CILL_SEARCH::Get_SQL_For_State");

	
	CESL_ControlMultiComboBox *pCmb = 
		(CESL_ControlMultiComboBox*)FindControl(_T("CM_ILL_SEARCH"), _T("진행상태"));
	if(NULL == pCmb) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
	const int nIdx = pCmb->GetCurSel();
	if(0 > nIdx) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);	
	CString strStateCode;
	pCmb->GetData(strStateCode, nIdx, 0);
	CString strDesc;
	pCmb->GetData(strDesc, nIdx, 1);

	
	CString strCancelCode;
	INT nIdx2 = m_pCMB->GetCurSel();
	if(0 > nIdx) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
	strCancelCode = m_pCMB->GetItemDataString(nIdx2, 0);

	CString strTmpQuery;
	if(_T("----") == strStateCode)
	{
		if(_T("전체(진행중 자료)") == strDesc)
		{
			strTmpQuery = _T("END_DIV IS NULL");
		}
		else if(_T("전체(완료된 자료)") == strDesc)
		{
			if(_T("----") != strCancelCode)	
			{
				strTmpQuery.Format(_T("END_DIV = '%s'"), strCancelCode);
			}
			else
			{
				strTmpQuery = _T("END_DIV IS NOT NULL");
			}
		}

		if(0 == strTmpQuery.Find(_T(" AND "))) strTmpQuery = strTmpQuery.Mid(5);
	}
	else
	{
		strTmpQuery.Format(_T("T.STATE = '%s'"), strStateCode);		
	}
	if(_T("") != strTmpQuery) strQuery = strQuery + _T(" AND ") + strTmpQuery;

	if(0 == strQuery.Find(_T(" AND "))) strQuery = strQuery.Mid(5);	

	return 0;
}

int CILL_SEARCH::ResetComboBox()
{
	int ids = -1;
	CString strFuncName = _T("CILL_SEARCH::ResetComboBox");

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_ILL_SEARCH"), _T("진행상태"));
	if(NULL == pCMB) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	const int COL_COUNT = 2;
	pCMB->ResetContent();
	pCMB->FreeComboData();
	pCMB->InitMultiComboBox(COL_COUNT);	

	const int STATE_COUNT = 12;
	CString strCmbData[STATE_COUNT][COL_COUNT] = 
	{
		{ _T("----"), _T("전체") },
		{ _T("----"), _T("전체(진행중 자료)") },
		{ _T("----"), _T("전체(완료된 자료)") },
		{ _T("0001"), _T("신청") },
		{ _T("0002"), _T("요청된자료") },
		{ _T("0004"), _T("발송된자료") },
		{ _T("0014"), _T("입수된자료") },
		{ _T("0008"), _T("복귀중자료") },
		{ _T("0009"), _T("완료된자료") },
		{ _T("0016"), _T("신청취소된자료") },
		{ _T("0012"), _T("요청거절된자료") },
		{ _T("0011"), _T("발송거절된자료") }		
	};
	
	for(int nRow = 0; nRow < STATE_COUNT; nRow++)
	{
		for(int nCol = 0; nCol < 2; nCol++)
			pCMB->AddItem(strCmbData[nRow][nCol], nCol, nRow);
	}

	const int LIB_INFO_COUNT = 3;
	CString strAlias[LIB_INFO_COUNT] = { _T("제공도서관"), _T("요청도서관"), _T("반납도서관") };
	CString strSetAlias = (MODE_REQUEST == m_nMode) ? _T("요청도서관") : _T("제공도서관");
	CString strFindAlias, strData;

	int ROW = 0;	
	for(int nCnt = 0; nCnt < LIB_INFO_COUNT; nCnt++)
 	{
		CESL_ControlMultiComboBox *pCmb = (CESL_ControlMultiComboBox*)FindControl(_T("CM_ILL_SEARCH"), strAlias[nCnt]);
		strFindAlias = (strSetAlias == strAlias[nCnt]) ? m_pInfo->MANAGE_CODE : _T("");
		
		ROW = pCmb->GetCount();
		for(int nCnt = 0; nCnt < ROW; nCnt++)
		{
			strData = pCmb->GetItemDataString(nCnt, 0);
			strData.TrimLeft();
			if(strData == strFindAlias)
			{
				pCmb->SetCurSel(nCnt);
				break;
			}
		}
	}

	return 0;
}

void CILL_SEARCH::OnbtnILLSEARCHRETURN() 
{
	
	m_strTblName.Format(
			_T("(SELECT ITL.*, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")
			_T("FROM ILL_TRANS_TBL ITL WHERE ")
			_T("ITL.STATE = '0014' AND ITL.RETURN_PLAN_DATE < SYSDATE AND ")	
			_T("(")
				_T("(ITL.RETURN_LIBCODE = '%s') OR ")
				_T("(ITL.LOAN_KEY IS NULL AND ITL.APL_LIBCODE = '%s')")
			_T(")) A, CO_LOAN_USER_TBL CLU"), m_strLibCode, m_strLibCode);


	m_strSQLWhere.Format(
		_T("T.STATE = '0014' AND T.RETURN_PLAN_DATE < SYSDATE AND ((T.RETURN_LIBCODE = '%s') OR (T.LOAN_KEY IS NULL AND T.APL_LIBCODE = '%s'))"),
		m_strLibCode, m_strLibCode);

	CDialog::OnOK();

	pParentMgr->SendMessage(WM_SEARCH, (WPARAM)this, NULL);
}

int CILL_SEARCH::Init_TableName()
{
	return 0 ;

	
	if(MODE_REQUEST == m_nMode)
	{
		m_strTblName.Format(
			_T("(SELECT ITL.*, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")

			_T("FROM ILL_TRANS_TBL ITL) A, ")
			_T("CO_LOAN_USER_TBL CLU"));

	}
	else if(MODE_OFFER == m_nMode)
	{
		m_strTblName.Format(
			_T("(SELECT ITL.*, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")

			_T("FROM ILL_TRANS_TBL ITL) A, ")
			_T("CO_LOAN_USER_TBL CLU"));

	}
	else return -1;

	return 0;
}

void CILL_SEARCH::OnShowWindow(BOOL bShow, UINT nStatus) 
{





	CDialog::OnShowWindow(bShow, nStatus);
	
	
	
}


int CILL_SEARCH::Init_MultiCombo()
{
 	int ids = 0;
 	CString strQuery = _T("");
 	strQuery.Format(
 		_T("SELECT CODE, DESCRIPTION FROM ILL_CODE_TBL ")
 		_T("WHERE CLASS = 'i04' AND CODE != '0000'"));

	m_DM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
 
 	ids = m_DM.RestructDataManager(strQuery);
 	if(0 > ids) return ids;
		
	if(NULL == m_pCMB)
	{
		m_pCMB = new CESL_ControlMultiComboBox(NULL);	
 		m_pCMB->SubclassDlgItem(IDC_cmb_REFUSE, this);	
 		m_pCMB->InitMultiComboBox(2);
	}

 	CString strData[2] = { _T(""), _T("") };
 	int ROW = m_DM.GetRowCount();
	
	
	strData[0] =_T("----");
	strData[1] =_T("적용안함");
	m_pCMB->SetData(strData, 2);
 	for(int nCnt = 0; nCnt < ROW; nCnt++)
 	{
 		ids = m_DM.GetCellData(nCnt, 0, strData[0]);
 		if(0 > ids) return ids;
 
 		ids = m_DM.GetCellData(nCnt, 1, strData[1]);
 		if(0 > ids) return ids;
 
 		m_pCMB->SetData(strData, 2);
 	}
 	m_pCMB->SetCurSel(0);

	return 0;
}
