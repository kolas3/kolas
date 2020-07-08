// ArrearStatisticsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArrearStatisticsDlg.h"
#include "..\\LOC_CommonAPI\\LocCommonAPI.h"
#include "..\\..\\..\\..\\ESL\\SIReportMgr\\SIReportManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArrearStatisticsDlg dialog


CArrearStatisticsDlg::CArrearStatisticsDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CArrearStatisticsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArrearStatisticsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_bIs3100 = FALSE;
	m_nMode = 0;
	m_strTerm = _T("");
}
CArrearStatisticsDlg::~CArrearStatisticsDlg()
{
}

void CArrearStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArrearStatisticsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArrearStatisticsDlg, CDialog)
	//{{AFX_MSG_MAP(CArrearStatisticsDlg)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnAS_SEARCH, OnbtnASSEARCH)
	ON_BN_CLICKED(IDC_radAS_DAY, OnradASDAY)
	ON_BN_CLICKED(IDC_radAS_MONTH, OnradASMONTH)
	ON_BN_CLICKED(IDC_radAS_YEAR, OnradASYEAR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArrearStatisticsDlg message handlers

BOOL CArrearStatisticsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

 	INT ids;
 	ids = InitESL_Mgr(_T("SM_ARREAR_STATISTICS"));
 	if(0 > ids) return FALSE;
 	
 	CESL_Mgr* pMgr = m_pParent->FindSM(_T("BO_LOC_3100"));
 	if(NULL != pMgr)
 	{
 		m_bIs3100 = TRUE;
 	}
 
 	ids = InitControl();
 	if(0 > ids) return FALSE;

	SetPosition();

	m_nMode = 1;
	SetRadioButton(m_nMode);

// 	SetTimer(1, 100, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CArrearStatisticsDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			m_pParent->PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnAS_SEARCH)->m_hWnd == pMsg->hwnd )
			{
				OnbtnASSEARCH();
			}
			return TRUE;
		}
 	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CArrearStatisticsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CWnd* pGrid = GetDlgItem(IDC_gridAS_MAIN);
	if(pGrid->GetSafeHwnd() == NULL) return;

	CRect rDlg, rGrid;
	GetWindowRect(rDlg);
	pGrid->GetWindowRect(rGrid);
	ScreenToClient(rGrid);

	pGrid->MoveWindow(rGrid.left, rGrid.top, rDlg.Width(), rDlg.Height()-rGrid.top);
}

void CArrearStatisticsDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(1);
		SearchStart();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CArrearStatisticsDlg::OnbtnASSEARCH() 
{
	SetTimer(1, 100, NULL);
}

BEGIN_EVENTSINK_MAP(CArrearStatisticsDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CArrearStatisticsDlg)
	ON_EVENT(CArrearStatisticsDlg, IDC_gridAS_MAIN, -604 /* KeyUp */, OnKeyUpgridASMAIN, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CArrearStatisticsDlg::OnKeyUpgridASMAIN(short FAR* KeyCode, short Shift) 
{
	if(VK_ESCAPE == *KeyCode)
	{
		PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

INT CArrearStatisticsDlg::InitControl()
{
	CString strGetData;
	CTime tGetTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
	CString strCurrent = strGetData.Left(10);
	CTime tCurrent(_ttoi(strCurrent.Mid(0, 4)), _ttoi(strCurrent.Mid(5, 2)), _ttoi(strCurrent.Mid(8, 2)), 0, 0, 0);

	CTime tBefore;
	CString strBefore;
	tBefore = tCurrent;
	strBefore.Format(_T("%04d/%02d/%02d"), tBefore.GetYear(), tBefore.GetMonth(), tBefore.GetDay());

	SetControlData(_T("CM_ARREAR_STATISTICS"), _T("조회기간1"), strBefore);
	SetControlData(_T("CM_ARREAR_STATISTICS"), _T("조회기간2"), strCurrent);

	CEdit *pEdt1 = (CEdit*)GetDlgItem(IDC_edtAS_Year);
	pEdt1->SetLimitText(4);
	CEdit *pEdt2 = (CEdit*)GetDlgItem(IDC_edtAS_Year2);
	pEdt2->SetLimitText(4);
	CEdit *pEdt3 = (CEdit*)GetDlgItem(IDC_edtAS_Month);
	pEdt3->SetLimitText(2);
	CEdit *pEdt4 = (CEdit*)GetDlgItem(IDC_edtAS_Month2);
	pEdt4->SetLimitText(2);
	pEdt1->SetWindowText(strCurrent.Mid(0, 4));
	pEdt2->SetWindowText(strCurrent.Mid(0, 4));
	pEdt3->SetWindowText(strCurrent.Mid(5, 2));
	pEdt4->SetWindowText(strCurrent.Mid(5, 2));

	return 0;
}

INT CArrearStatisticsDlg::GetWhere(CString& strWhere)
{
	CString strFuncName = _T("GetWhere");

	CString strTerm;
	if(0 == m_nMode)
	{
		CString strBefore, strAfter;
		GetControlData(_T("CM_ARREAR_STATISTICS"), _T("조회기간1"), strBefore);
		GetControlData(_T("CM_ARREAR_STATISTICS"), _T("조회기간2"), strAfter);
		if(10 != strBefore.GetLength()) 
		{
			strBefore = _T("");
			SetControlData(_T("CM_ARREAR_STATISTICS"), _T("조회기간1"), strBefore);
			MessageBox(_T("납입일자를 입력하십시오."), _T("통계"), MB_ICONINFORMATION);
			((CEdit*)GetDlgItem(IDC_edtAS_BEFORE))->SetFocus();
			strWhere = _T("");
			return 1;
		}
		if(10 != strAfter.GetLength())
		{
			strAfter = _T("");
			SetControlData(_T("CM_ARREAR_STATISTICS"), _T("조회기간2"), strAfter);
			MessageBox(_T("납입일자를 입력하십시오."), _T("통계"), MB_ICONINFORMATION);
			((CEdit*)GetDlgItem(IDC_edtAS_AFTER))->SetFocus();
			strWhere = _T("");
			return 1;
		}
		strWhere.Format(_T("CL.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strBefore, strAfter);
		strTerm = strBefore + _T(" 부터 ") + strAfter + _T(" 까지");
	}
	else if(1 == m_nMode)
	{
		CString strYear, strYear2, strMonth, strMonth2;
		GetDlgItem(IDC_edtAS_Year)->GetWindowText(strYear);
		GetDlgItem(IDC_edtAS_Year2)->GetWindowText(strYear2);
		GetDlgItem(IDC_edtAS_Month)->GetWindowText(strMonth);
		GetDlgItem(IDC_edtAS_Month2)->GetWindowText(strMonth2);
		// 년도 자릿수
		if(4 != strYear.GetLength()) 
		{
			strYear = _T("");
			GetDlgItem(IDC_edtAS_Year)->SetWindowText(strYear);
			MessageBox(_T("년도를 확인하십시오."), _T("통계"), MB_ICONINFORMATION);
			((CEdit*)GetDlgItem(IDC_edtAS_Year))->SetFocus();
			strWhere = _T("");
			return 1;
		}
		if(4 != strYear2.GetLength()) 
		{
			strYear2 = _T("");
			GetDlgItem(IDC_edtAS_Year2)->SetWindowText(strYear2);
			MessageBox(_T("년도를 확인하십시오."), _T("통계"), MB_ICONINFORMATION);
			GetDlgItem(IDC_edtAS_Year2)->SetFocus();
			strWhere = _T("");
			return 1;
		}
		// 년도 범위확인
		if(1970 > _ttoi(strYear) || 2038 < _ttoi(strYear))
		{
			strYear = _T("");
			GetDlgItem(IDC_edtAS_Year)->SetWindowText(strYear);
			MessageBox(_T("년도 범위는 1970부터 2038까지 입니다."), _T("통계"), MB_ICONINFORMATION);
			((CEdit*)GetDlgItem(IDC_edtAS_Year))->SetFocus();
			strWhere = _T("");
			return 1;
		}
		if(1970 > _ttoi(strYear2) || 2038 < _ttoi(strYear2))
		{
			strYear2 = _T("");
			GetDlgItem(IDC_edtAS_Year2)->SetWindowText(strYear2);
			MessageBox(_T("년도 범위는 1970부터 2038까지 입니다."), _T("통계"), MB_ICONINFORMATION);
			((CEdit*)GetDlgItem(IDC_edtAS_Year2))->SetFocus();
			strWhere = _T("");
			return 1;
		}
		// 월 자릿수 1개 2자리로 치환 (1 -> 01)
		if(1 == strMonth.GetLength())
		{
			strMonth = _T("0") + strMonth;
			GetDlgItem(IDC_edtAS_Month)->SetWindowText(strMonth);
		}
		if(1 == strMonth2.GetLength())
		{
			strMonth2 = _T("0") + strMonth2;
			GetDlgItem(IDC_edtAS_Month2)->SetWindowText(strMonth2);
		}
		// 월 입력확인 (없으면 당월)
		if(strMonth.IsEmpty())
		{
			CString strGetData;
			CTime tGetTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
			strMonth = strGetData.Mid(5, 2);			
			GetDlgItem(IDC_edtAS_Month)->SetWindowText(strMonth);
		}
		if(strMonth2.IsEmpty())
		{
			CString strGetData;
			CTime tGetTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
			strMonth2 = strGetData.Mid(5, 2);			
			GetDlgItem(IDC_edtAS_Month2)->SetWindowText(strMonth2);
		}
		// 월 범위 확인
		if(1 > _ttoi(strMonth) || 12 < _ttoi(strMonth))
		{
			strMonth = _T("");
			GetDlgItem(IDC_edtAS_Month)->SetWindowText(strMonth);
			MessageBox(_T("월 범위는 1부터 12까지 입니다."), _T("통계"), MB_ICONINFORMATION);
			((CEdit*)GetDlgItem(IDC_edtAS_Month))->SetFocus();
			strWhere = _T("");
			return 1;
		}
		if(1 > _ttoi(strMonth2) || 12 < _ttoi(strMonth2))
		{
			strMonth2 = _T("");
			GetDlgItem(IDC_edtAS_Month2)->SetWindowText(strMonth2);
			MessageBox(_T("월 범위는 1부터 12까지 입니다."), _T("통계"), MB_ICONINFORMATION);
			((CEdit*)GetDlgItem(IDC_edtAS_Month2))->SetFocus();
			strWhere = _T("");
			return 1;
		}
		// 달에 해당하는 마지막 일자 구하기
		INT max_day[]= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		INT nYear = _ttoi(strYear);
		INT nMonth = _ttoi(strMonth2);
		if((2 == nMonth) && (0 == nYear % 4) && ((0 != nYear % 100) || (0 == nYear % 400))) 
		{
			max_day[1]= 29;
		}
		CString strLastDay;
		strLastDay.Format(_T("%d"), max_day[nMonth-1]);
		CString strBefore = strYear + _T("/") + strMonth + _T("/01");
		CString strAfter = strYear2 + _T("/") + strMonth2 + _T("/") + strLastDay;
		strWhere.Format(_T("CL.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strBefore, strAfter);
		strTerm = strBefore + _T(" 부터 ") + strAfter + _T(" 까지");
	}
	else if(2 == m_nMode)
	{
		CString strYear, strYear2;
		GetDlgItem(IDC_edtAS_Year)->GetWindowText(strYear);
		GetDlgItem(IDC_edtAS_Year2)->GetWindowText(strYear2);
		// 년도 자릿수
		if(4 != strYear.GetLength()) 
		{
			strYear = _T("");
			GetDlgItem(IDC_edtAS_Year)->SetWindowText(strYear);
			MessageBox(_T("년도를 확인하십시오."), _T("통계"), MB_ICONINFORMATION);
			((CEdit*)GetDlgItem(IDC_edtAS_Year))->SetFocus();
			strWhere = _T("");
			return 1;
		}
		if(4 != strYear2.GetLength()) 
		{
			strYear2 = _T("");
			GetDlgItem(IDC_edtAS_Year2)->SetWindowText(strYear2);
			MessageBox(_T("년도를 확인하십시오."), _T("통계"), MB_ICONINFORMATION);
			GetDlgItem(IDC_edtAS_Year2)->SetFocus();
			strWhere = _T("");
			return 1;
		}
		// 년도 범위확인
		if(1970 > _ttoi(strYear) || 2038 < _ttoi(strYear))
		{
			strYear = _T("");
			GetDlgItem(IDC_edtAS_Year)->SetWindowText(strYear);
			MessageBox(_T("년도 범위는 1970부터 2038까지 입니다."), _T("통계"), MB_ICONINFORMATION);
			((CEdit*)GetDlgItem(IDC_edtAS_Year))->SetFocus();
			strWhere = _T("");
			return 1;
		}
		if(1970 > _ttoi(strYear2) || 2038 < _ttoi(strYear2))
		{
			strYear2 = _T("");
			GetDlgItem(IDC_edtAS_Year2)->SetWindowText(strYear2);
			MessageBox(_T("년도 범위는 1970부터 2038까지 입니다."), _T("통계"), MB_ICONINFORMATION);
			((CEdit*)GetDlgItem(IDC_edtAS_Year2))->SetFocus();
			strWhere = _T("");
			return 1;
		}
		CString strBefore = strYear + _T("/01/01");
		CString strAfter = strYear2 + _T("/12/31");
		strWhere.Format(_T("CL.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strBefore, strAfter);
		strTerm = strBefore + _T(" 부터 ") + strAfter + _T(" 까지");
	}
	else
	{
		strWhere = _T("");
		return 0;
	}

	if(TRUE == m_bIs3100)
	{
		CString strUserKey;
		CESL_DataMgr* pMemberDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		pMemberDM->GetCellData(_T("대출자KEY"), 0, strUserKey);
		if(strUserKey.IsEmpty()) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		if(strWhere.IsEmpty())
		{
			strWhere.Format(_T("CL.USER_KEY=%s"), strUserKey);
		}
		else
		{
			CString strValue;
			strValue.Format(_T(" AND CL.USER_KEY=%s"), strUserKey);
			strWhere += strValue;
		}
		CString strUserName;
		pMemberDM->GetCellData(_T("대출자이름"), 0, strUserName);
		CString strUserNo;
		pMemberDM->GetCellData(_T("대출자번호"), 0, strUserNo);
		strTerm += _T(",  대출자번호 : ") + strUserNo + _T(", 이름 : ") + strUserName;
	}
	m_strTerm = strTerm;

	return 0;
}

INT CArrearStatisticsDlg::SearchStart()
{
	INT ids;
	CString strFuncName = _T("SearchStart");

	CString strWhere = _T("");
	ids = GetWhere(strWhere);
	if(0 != ids) return 0;

	CString strPaymentDate;
	CString strGroupBy;
	if(0 == m_nMode)
	{
		strPaymentDate = _T("TO_CHAR(CL.PAYMENT_DATE,'YYYY/MM/DD')");
		strGroupBy = _T("MU.MANAGE_CODE, TO_CHAR(CL.PAYMENT_DATE,'YYYY/MM/DD')");
	}
	else if(1 == m_nMode)
	{
		strPaymentDate = _T("TO_CHAR(CL.PAYMENT_DATE,'YYYY/MM')");
		strGroupBy = _T("MU.MANAGE_CODE, TO_CHAR(CL.PAYMENT_DATE,'YYYY/MM')");
	}
	else if(2 == m_nMode)
	{
		strPaymentDate = _T("TO_CHAR(CL.PAYMENT_DATE,'YYYY')");
		strGroupBy = _T("MU.MANAGE_CODE, TO_CHAR(CL.PAYMENT_DATE,'YYYY')");
	}
	else
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	}
	CString strQuery;
	strQuery.Format(
		_T("SELECT ECO$F_GET_CODE2DESC('1',MU.MANAGE_CODE) AS MANAGE_CODE, %s AS PAYMENT_DATE, TRIM(TO_CHAR(SUM(PAYMENTS),'999,999,999')) AS SUM, NULL ")
		_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, MN_USER_TBL MU ")
		_T("WHERE CLU.REC_KEY=CL.USER_KEY AND MU.REC_KEY=CL.ACCEPTER_KEY ")
		_T("AND CL.PAYMENT_DATE IS NOT NULL AND TO_DATE(CL.RETURN_DATE,'YYYY/MM/DD')>TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD') ")
		_T("AND CL.STATUS IN ('1','R') AND 0=CL.ARREAR AND 0<CL.PAYMENTS ")
		_T("AND %s ")
		_T("GROUP BY %s ")
		_T("UNION ")
		_T("SELECT ECO$F_GET_CODE2DESC('1',MU.MANAGE_CODE) AS MANAGE_CODE, %s AS PAYMENT_DATE, TRIM(TO_CHAR(SUM(PAYMENTS),'999,999,999')) AS SUM, NULL ")
		_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, MN_USER_TBL MU ")
		_T("WHERE CLU.REC_KEY=CL.USER_KEY AND MU.REC_KEY=CL.ACCEPTER_KEY ")
		_T("AND CL.PAYMENT_DATE IS NOT NULL AND TO_DATE(CL.RETURN_DATE,'YYYY/MM/DD')>TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD') ")
		_T("AND CL.STATUS IN ('1','R') AND 0=CL.ARREAR AND 0<CL.PAYMENTS ")
		_T("AND %s ")
		_T("GROUP BY %s ")
		_T("ORDER BY MANAGE_CODE, PAYMENT_DATE"), strPaymentDate, strWhere, strGroupBy, strPaymentDate, strWhere, strGroupBy);

	CESL_DataMgr *pDM = FindDM(_T("DM_ARREAR_STATISTICS"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	pDM->RestructDataManager(strQuery);
	ControlDisplay(_T("CM_ARREAR_STATISTICS"), _T("통계그리드"));

	if(1 > pDM->GetRowCount())
	{
		MessageBox(_T("통계 결과가 없습니다."), _T("통계"), MB_ICONINFORMATION);
	}
	else
	{
		INT nSum = 0;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			CString strArrear;
			pDM->GetCellData(_T("합계"), i, strArrear);
			strArrear.Replace(_T(","), _T(""));
			nSum += _ttoi(strArrear);
		}
		CString strSum;
		strSum.Format(_T("%d"), nSum);
		CLocCommonAPI::SetMoneyFormat(strSum);
		CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_ARREAR_STATISTICS"), _T("통계그리드"));
		if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		pGrid->SetRows(pDM->GetRowCount()+2);
		pGrid->SetTextMatrix(pDM->GetRowCount()+1, 0, _T("합계"));
		pGrid->SetTextMatrix(pDM->GetRowCount()+1, 3, strSum);
		pDM->InsertRow(-1);
		pDM->SetCellData(_T("번호"), _T("합계"), pDM->GetRowCount()-1);
		pDM->SetCellData(_T("합계"), strSum, pDM->GetRowCount()-1);
	}

	return 0;
}

void CArrearStatisticsDlg::OnradASDAY() 
{
	SetRadioButton(0);
}

void CArrearStatisticsDlg::OnradASMONTH() 
{
	SetRadioButton(1);
}

void CArrearStatisticsDlg::OnradASYEAR() 
{
	SetRadioButton(2);
}

VOID CArrearStatisticsDlg::SetRadioButton(INT nMode)
{
	if(m_nMode != nMode)
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_ARREAR_STATISTICS"));
		if(NULL == pDM) return;
		pDM->FreeData();
		ControlDisplay(_T("CM_ARREAR_STATISTICS"), _T("통계그리드"));
	}
	m_nMode = nMode;

	CWnd* pPos1 = GetDlgItem(IDC_stcAS_Day);
	CWnd* pPos2 = GetDlgItem(IDC_edtAS_Year2);
	CWnd* pPos3 = GetDlgItem(IDC_stcAS_Year2);
	CWnd* pPos4 = GetDlgItem(IDC_btnAS_SEARCH);
	CWnd* pPos5 = GetDlgItem(IDC_radAS_DAY);
	CWnd* pPos6 = GetDlgItem(IDC_radAS_MONTH);
	CWnd* pPos7 = GetDlgItem(IDC_radAS_YEAR);
	CRect rRect;
	pPos1->GetWindowRect(rRect);
	ScreenToClient(rRect);
	if(0 == nMode)
	{
		((CButton*)GetDlgItem(IDC_radAS_DAY))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_radAS_MONTH))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_radAS_YEAR))->SetCheck(FALSE);
		GetDlgItem(IDC_stcAS_DateTitle)->SetWindowText(_T("납입일자"));
		GetDlgItem(IDC_edtAS_BEFORE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtAS_AFTER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtAS_Year)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcAS_Year)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtAS_Month)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcAS_Month)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtAS_Year2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcAS_Year2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtAS_Month2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcAS_Month2)->ShowWindow(SW_HIDE);
		if(rRect!=m_rPosition[0])
		{
			pPos1->MoveWindow(m_rPosition[0], TRUE);
			pPos1->Invalidate();
			pPos2->MoveWindow(m_rPosition[1], TRUE);
			pPos2->Invalidate();
			pPos3->MoveWindow(m_rPosition[2], TRUE);
			pPos3->Invalidate();
			pPos4->MoveWindow(m_rPosition[3], TRUE);
			pPos4->Invalidate();
			pPos5->MoveWindow(m_rPosition[4], TRUE);
			pPos5->Invalidate();
			pPos6->MoveWindow(m_rPosition[5], TRUE);
			pPos6->Invalidate();
			pPos7->MoveWindow(m_rPosition[6], TRUE);
			pPos7->Invalidate();
		}
	}
	else if(1 == nMode)
	{
		((CButton*)GetDlgItem(IDC_radAS_DAY))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_radAS_MONTH))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_radAS_YEAR))->SetCheck(FALSE);
		GetDlgItem(IDC_stcAS_DateTitle)->SetWindowText(_T("납입(월)"));
		GetDlgItem(IDC_edtAS_Year)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcAS_Year)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtAS_Month)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcAS_Month)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtAS_Year2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcAS_Year2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtAS_Month2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcAS_Month2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtAS_BEFORE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtAS_AFTER)->ShowWindow(SW_HIDE);
		if(rRect!=m_rPosition[0])
		{
			pPos1->MoveWindow(m_rPosition[0], TRUE);
			pPos1->Invalidate();
			pPos2->MoveWindow(m_rPosition[1], TRUE);
			pPos2->Invalidate();
			pPos3->MoveWindow(m_rPosition[2], TRUE);
			pPos3->Invalidate();
			pPos4->MoveWindow(m_rPosition[3], TRUE);
			pPos4->Invalidate();
			pPos5->MoveWindow(m_rPosition[4], TRUE);
			pPos5->Invalidate();
			pPos6->MoveWindow(m_rPosition[5], TRUE);
			pPos6->Invalidate();
			pPos7->MoveWindow(m_rPosition[6], TRUE);
			pPos7->Invalidate();
		}
	}
	else if(2 == nMode)
	{
		((CButton*)GetDlgItem(IDC_radAS_DAY))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_radAS_MONTH))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_radAS_YEAR))->SetCheck(TRUE);
		GetDlgItem(IDC_stcAS_DateTitle)->SetWindowText(_T("납입(년)"));
		GetDlgItem(IDC_edtAS_Year)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcAS_Year)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtAS_Month)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcAS_Month)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtAS_Year2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcAS_Year2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtAS_Month2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcAS_Month2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtAS_BEFORE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtAS_AFTER)->ShowWindow(SW_HIDE);
		if(rRect.left==m_rPosition[0].left)
		{
			pPos1->MoveWindow(m_rPosition[0].left-55, m_rPosition[0].top, m_rPosition[0].Width(), m_rPosition[0].Height(), TRUE);
			pPos1->Invalidate();
			pPos2->MoveWindow(m_rPosition[1].left-55, m_rPosition[1].top, m_rPosition[1].Width(), m_rPosition[1].Height(), TRUE);
			pPos2->Invalidate();
			pPos3->MoveWindow(m_rPosition[2].left-55, m_rPosition[2].top, m_rPosition[2].Width(), m_rPosition[2].Height(), TRUE);
			pPos3->Invalidate();
			pPos4->MoveWindow(m_rPosition[3].left-90, m_rPosition[3].top, m_rPosition[3].Width(), m_rPosition[3].Height(), TRUE);
			pPos4->Invalidate();
			pPos5->MoveWindow(m_rPosition[4].left-90, m_rPosition[4].top, m_rPosition[4].Width(), m_rPosition[4].Height(), TRUE);
			pPos5->Invalidate();
			pPos6->MoveWindow(m_rPosition[5].left-90, m_rPosition[5].top, m_rPosition[5].Width(), m_rPosition[5].Height(), TRUE);
			pPos6->Invalidate();
			pPos7->MoveWindow(m_rPosition[6].left-90, m_rPosition[6].top, m_rPosition[6].Width(), m_rPosition[6].Height(), TRUE);
			pPos7->Invalidate();
		}
	}
	else
	{
	}
}

VOID CArrearStatisticsDlg::SetPosition()
{
	CWnd* pPos1 = GetDlgItem(IDC_stcAS_Day);
	CWnd* pPos2 = GetDlgItem(IDC_edtAS_Year2);
	CWnd* pPos3 = GetDlgItem(IDC_stcAS_Year2);
	CWnd* pPos4 = GetDlgItem(IDC_btnAS_SEARCH);
	CWnd* pPos5 = GetDlgItem(IDC_radAS_DAY);
	CWnd* pPos6 = GetDlgItem(IDC_radAS_MONTH);
	CWnd* pPos7 = GetDlgItem(IDC_radAS_YEAR);	
	if(pPos1) pPos1->GetWindowRect(m_rPosition[0]);
	if(pPos2) pPos2->GetWindowRect(m_rPosition[1]);
	if(pPos3) pPos3->GetWindowRect(m_rPosition[2]);
	if(pPos4) pPos4->GetWindowRect(m_rPosition[3]);
	if(pPos5) pPos5->GetWindowRect(m_rPosition[4]);
	if(pPos6) pPos6->GetWindowRect(m_rPosition[5]);
	if(pPos7) pPos7->GetWindowRect(m_rPosition[6]);
	ScreenToClient(m_rPosition[0]);
	ScreenToClient(m_rPosition[1]);
	ScreenToClient(m_rPosition[2]);
	ScreenToClient(m_rPosition[3]);
	ScreenToClient(m_rPosition[4]);
	ScreenToClient(m_rPosition[5]);
	ScreenToClient(m_rPosition[6]);
}

INT CArrearStatisticsDlg::ExcelReport()
{
	INT ids;
	CString strFuncName = _T("ExcelReport");

	CESL_DataMgr *pDM = FindDM(_T("DM_ARREAR_STATISTICS"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	if(1 > pDM->GetRowCount())
	{
		MessageBox(_T("출력할 이력이 없습니다."), _T("출력"), MB_ICONINFORMATION);
		return 1;
	}
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	CString strPrintName;
	if(0 == m_nMode)
	{
		strPrintName = _T("연체료일별통계");
	}
	else if(1 == m_nMode)
	{
		strPrintName = _T("연체료월별통계");
	}
	else if(2 == m_nMode)
	{
		strPrintName = _T("연체료년별통계");
	}
	else
	{
		strPrintName = _T("연체료일별통계");
	}
	ids = pSIReportManager->GetSIReportFileFromDB(strPrintName);
	if(0 > ids) 
	{
		MessageBox(_T("출력 정보를 가져오지 못하였습니다."), _T("출력"), MB_ICONINFORMATION);
		if(pSIReportManager)
			delete pSIReportManager;
		return 1;
	}
	pSIReportManager->SetDataMgr(0, pDM, 0) ;
	
	for(INT i = 0; i < pDM->GetRowCount()-1; i++)
	{
		CString strNum;
		strNum.Format(_T("%d"), i+1);
		pDM->SetCellData(_T("번호"), strNum, i);
	}

	ids = pSIReportManager->SetTempVariable(_T("조건"), m_strTerm);
	ids = pSIReportManager->Print();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	if(pSIReportManager) 
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	return 0;
}