// ArrearUnpaidDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArrearUnpaidDlg.h"
#include "..\\LOC_CommonAPI\\LocCommonAPI.h"
#include "..\\..\\..\\..\\ESL\\SIReportMgr\\SIReportManager.h"
#include "PaymentRunDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArrearUnpaidDlg dialog


CArrearUnpaidDlg::CArrearUnpaidDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CArrearUnpaidDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArrearUnpaidDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_bIs3100 = FALSE;
}
CArrearUnpaidDlg::~CArrearUnpaidDlg()
{
}

void CArrearUnpaidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArrearUnpaidDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArrearUnpaidDlg, CDialog)
	//{{AFX_MSG_MAP(CArrearUnpaidDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnAU_SEARCH, OnbtnAUSEARCH)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArrearUnpaidDlg message handlers

BOOL CArrearUnpaidDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	INT ids;
	ids = InitESL_Mgr(_T("SM_ARREAR_UNPAID"));
	if(0 > ids) return FALSE;
	
	CESL_Mgr* pMgr = m_pParent->FindSM(_T("BO_LOC_3100"));
	if(NULL != pMgr)
	{
		m_bIs3100 = TRUE;
	}

	ids = InitControl();
	if(0 > ids) return FALSE;

// 	SetTimer(1, 200, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CArrearUnpaidDlg::PreTranslateMessage(MSG* pMsg) 
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
			if(GetDlgItem(IDC_btnAU_SEARCH)->m_hWnd == pMsg->hwnd )
			{
				OnbtnAUSEARCH();
			}
			return TRUE;
		}
 	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CArrearUnpaidDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void CArrearUnpaidDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CWnd* pGrid = GetDlgItem(IDC_gridAU_MAIN);
	if(pGrid->GetSafeHwnd() == NULL) return;

	CRect rDlg, rGrid;
	GetWindowRect(rDlg);
	pGrid->GetWindowRect(rGrid);
	ScreenToClient(rGrid);

	pGrid->MoveWindow(rGrid.left, rGrid.top, rDlg.Width(), rDlg.Height()-rGrid.top);
}

void CArrearUnpaidDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(1);
		SearchStart();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CArrearUnpaidDlg::OnbtnAUSEARCH() 
{
	SetTimer(1, 100, NULL);
}

BEGIN_EVENTSINK_MAP(CArrearUnpaidDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CArrearUnpaidDlg)
	ON_EVENT(CArrearUnpaidDlg, IDC_gridAU_MAIN, -604 /* KeyUp */, OnKeyUpgridAUMAIN, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CArrearUnpaidDlg::OnKeyUpgridAUMAIN(short FAR* KeyCode, short Shift) 
{
	if(VK_ESCAPE == *KeyCode)
	{
		PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

INT CArrearUnpaidDlg::InitControl()
{
	CString strGetData;
	CTime tGetTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
	CString strCurrent = strGetData.Left(10);
	CTime tCurrent(_ttoi(strCurrent.Mid(0, 4)), _ttoi(strCurrent.Mid(5, 2)), _ttoi(strCurrent.Mid(8, 2)), 0, 0, 0);

	CTimeSpan tOneYear(31579200);
	CTime tBefore;
	CString strBefore;
	if(TRUE == m_bIs3100)
	{
		tBefore = tCurrent - tOneYear;
	}
	else
	{
		tBefore = tCurrent;
	}
	strBefore.Format(_T("%04d/%02d/%02d"), tBefore.GetYear(), tBefore.GetMonth(), tBefore.GetDay());

	SetControlData(_T("CM_ARREAR_UNPAID"), _T("조회기간1"), strBefore);
	SetControlData(_T("CM_ARREAR_UNPAID"), _T("조회기간2"), strCurrent);

	return 0;
}

INT CArrearUnpaidDlg::SearchStart()
{
	CString strFuncName = _T("SearchStart");

	CString strBefore, strAfter;
	GetControlData(_T("CM_ARREAR_UNPAID"), _T("조회기간1"), strBefore);
	GetControlData(_T("CM_ARREAR_UNPAID"), _T("조회기간2"), strAfter);
	if(10 != strBefore.GetLength()) 
	{
		strBefore = _T("");
		SetControlData(_T("CM_ARREAR_UNPAID"), _T("조회기간1"), strBefore);
	}
	if(10 != strAfter.GetLength())
	{
		strAfter = _T("");
		SetControlData(_T("CM_ARREAR_UNPAID"), _T("조회기간2"), strAfter);
	}

	CString strWhere;
	if(!strBefore.IsEmpty() && !strAfter.IsEmpty())
	{
		strWhere.Format(_T("CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strBefore, strAfter);
	}
	else if(!strBefore.IsEmpty() && strAfter.IsEmpty())
	{
		strWhere.Format(_T("CL.RETURN_DATE > TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS')"), strBefore);
	}
	else if(strBefore.IsEmpty() && !strAfter.IsEmpty())
	{
		strWhere.Format(_T("CL.RETURN_DATE < TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strAfter);
	}
	else
	{
		strWhere = _T("");
	}

	CString strManageCode;
	GetControlData(_T("CM_ARREAR_UNPAID"), _T("관리구분"), strManageCode);
	if(!strManageCode.IsEmpty())
	{
		if(!strWhere.IsEmpty()) strWhere += _T(" AND ");
		strWhere += _T("CL.MANAGE_CODE='") + strManageCode + _T("' ");
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
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_ARREAR_UNPAID"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	CESL_DataMgr *pCoDM = FindDM(_T("DM_ARREAR_UNPAID_CO"));
	if(NULL == pCoDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	pDM->RefreshDataManager(strWhere);
	pCoDM->RefreshDataManager(strWhere);
	pDM->AddDM(pCoDM);
	pDM->SORT(_T("반납일"), TRUE);
// 	pDM->SORT(_T("USER_KEY"), TRUE);
	ControlDisplay(_T("CM_ARREAR_UNPAID"), _T("미납그리드"));

	return 0;
}

INT CArrearUnpaidDlg::SetPayments()
{
	CString strFuncName = _T("SetUnPaid");

	CESL_DataMgr *pDM = FindDM(_T("DM_ARREAR_UNPAID"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	if(1 > pDM->GetRowCount())
	{
		return 0;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_ARREAR_UNPAID"), _T("미납그리드"));
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	pGrid->SelectMakeList();
	if(1 > pGrid->SelectGetCount())
	{
		MessageBox(_T("납입 처리할 자료를 선정해주십시오."), _T("납입처리"), MB_ICONINFORMATION);
		return 0;
	}

	INT nIdx;
	INT nTotArrear = 0;
	pGrid->SelectGetHeadPosition();
	if(1 < pGrid->SelectGetCount())
	{
		if(IDYES!=MessageBox(_T("선정된 이력을 납입처리 하시겠습니까?"), _T("납입처리"), MB_YESNO|MB_ICONINFORMATION))
		{
			return 0;
		}
		pDM->StartFrame();
		pDM->InitDBFieldData();	
		for(INT i = 0; i < pGrid->SelectGetCount(); i++)
		{
			nIdx = pGrid->SelectGetIdx();
			CString strLoanKey;
			pDM->GetCellData(_T("LOAN_KEY"), nIdx, strLoanKey);
			CString strTable;
			pDM->GetCellData(_T("TABLE"), nIdx, strTable);
			CString strArrear;
			pDM->GetCellData(_T("연체금액"), nIdx, strArrear);
			if(strArrear.IsEmpty())
			{
				CString strMsg;
				strMsg.Format(_T("연체금액이 없는 자료로 인한 중지(선정된 자료중 %d번째)"), nIdx+1);
				MessageBox(strMsg, _T("납부처리"), MB_ICONINFORMATION);
				return 0;
			}
			strArrear.Replace(_T(","), _T(""));
			nTotArrear += _ttoi(strArrear);
			CString strQuery;
			if(!strLoanKey.IsEmpty() && !strTable.IsEmpty())
			{
				if(_T("CO_LOAN_TBL") == strTable)
				{
					strQuery.Format(_T("UPDATE CO_LOAN_TBL SET ARREAR=0,PREV_ARREAR=%s,PAYMENT_DATE=SYSDATE,PAYMENTS=%s,ACCEPTER_KEY=%s,LAST_WORK='%s' WHERE REC_KEY=%s;"), strArrear, strArrear, m_pInfo->USER_PK, GetWorkLogMsg(_T("납입처리"),__WFILE__,__LINE__), strLoanKey);
				}
				else
				{
					strQuery.Format(_T("UPDATE LS_WORK_T01 SET ARREAR=0,PREV_ARREAR=%s,PAYMENT_DATE=SYSDATE,PAYMENTS=%s,ACCEPTER_KEY=%s,LAST_WORK='%s' WHERE REC_KEY=%s;"), strArrear, strArrear, m_pInfo->USER_PK, GetWorkLogMsg(_T("납입처리"),__WFILE__,__LINE__), strLoanKey);
				}
			}
			pDM->AddFrame(strQuery);
			pGrid->SelectGetNext();
		}
		pDM->SendFrame(TRUE);
		pDM->EndFrame();
	}
	else
	{
		nIdx = pGrid->SelectGetIdx();
		CString strArrear;
		pDM->GetCellData(_T("연체금액"), nIdx, strArrear);
		if(strArrear.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("연체금액이 없는 자료로 인한 중지"));
			MessageBox(strMsg, _T("납부처리"), MB_ICONINFORMATION);
			return 0;
		}
		strArrear.Replace(_T(","), _T(""));
		nTotArrear += _ttoi(strArrear);
		CPaymentRunDlg dlg(this);
		dlg.m_nPayment = _ttoi(strArrear);
		if(IDOK == dlg.DoModal())
		{
			pDM->StartFrame();
			pDM->InitDBFieldData();	

			CString strPayment;
			strPayment.Format(_T("%d"), dlg.m_nPayment);
			CString strLoanKey;
			pDM->GetCellData(_T("LOAN_KEY"), nIdx, strLoanKey);
			CString strTable;
			pDM->GetCellData(_T("TABLE"), nIdx, strTable);
			CString strQuery;
			if(!strLoanKey.IsEmpty() && !strTable.IsEmpty())
			{
				if(_T("CO_LOAN_TBL") == strTable)
				{
					strQuery.Format(_T("UPDATE CO_LOAN_TBL SET ARREAR=0,PREV_ARREAR=%s,PAYMENT_DATE=SYSDATE,PAYMENTS=%s,ACCEPTER_KEY=%s,LAST_WORK='%s' WHERE REC_KEY=%s;"), strArrear, strPayment, m_pInfo->USER_PK, GetWorkLogMsg(_T("납입처리"),__WFILE__,__LINE__), strLoanKey);
				}
				else
				{
					strQuery.Format(_T("UPDATE LS_WORK_T01 SET ARREAR=0,PREV_ARREAR=%s,PAYMENT_DATE=SYSDATE,PAYMENTS=%s,ACCEPTER_KEY=%s,LAST_WORK='%s' WHERE REC_KEY=%s;"), strArrear, strPayment, m_pInfo->USER_PK, GetWorkLogMsg(_T("납입처리"),__WFILE__,__LINE__), strLoanKey);
				}
			}
			pDM->AddFrame(strQuery);
			pDM->SendFrame(TRUE);
			pDM->EndFrame();
		}
		else
		{
			return 100;
		}
	}

	// 검색된 이용자의 총연체료 감소
	if(TRUE == m_bIs3100)
	{
		CString strTotArrear;
		CESL_DataMgr* pMemberDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		pMemberDM->GetCellData(_T("총연체료"), 0, strTotArrear);
		if(strTotArrear.IsEmpty()) strTotArrear = _T("0");
		strTotArrear.Replace(_T(","), _T(""));
		if(nTotArrear <= _ttoi(strTotArrear))
		{
			INT nTot = _ttoi(strTotArrear);
			nTot -= nTotArrear;
			strTotArrear.Format(_T("%d"), nTot);
			pMemberDM->SetCellData(_T("총연체료"), strTotArrear, 0);
		}
	}

	return 0;
}

INT CArrearUnpaidDlg::ExcelReport()
{
	INT ids;
	CString strFuncName = _T("ExcelReport");

	CESL_DataMgr *pDM = FindDM(_T("DM_ARREAR_UNPAID"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	if(1 > pDM->GetRowCount())
	{
		MessageBox(_T("출력할 이력이 없습니다."), _T("출력"), MB_ICONINFORMATION);
		return 1;
	}
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("미납연체료현황"));
	if(0 > ids) 
	{
		MessageBox(_T("출력 정보를 가져오지 못하였습니다."), _T("출력"), MB_ICONINFORMATION);
		if(pSIReportManager)
			delete pSIReportManager;
		return 1;
	}
	pSIReportManager->SetDataMgr(0, pDM, 0) ;
	
	INT nSum = 0;
	CString strSum;
	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		CString strArrear;
		pDM->GetCellData(_T("연체금액"), i, strArrear);
		strArrear.Replace(_T(","), _T(""));
		nSum += _ttoi(strArrear);
		CString strNum;
		strNum.Format(_T("%d"), i+1);
		pDM->SetCellData(_T("번호"), strNum, i);
	}
	strSum.Format(_T("%d"), nSum);
	CLocCommonAPI::SetMoneyFormat(strSum);
	CString strValue;
	strValue.Format(_T(" %d건,    %s원"), pDM->GetRowCount(), strSum);
	ids = pSIReportManager->SetTempVariable(_T("합계"), strValue);
	ids = pSIReportManager->Print();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	if(pSIReportManager) 
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	return 0;
}