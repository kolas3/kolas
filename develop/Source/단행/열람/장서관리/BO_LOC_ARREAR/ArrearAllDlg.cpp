// ArrearAllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArrearAllDlg.h"
#include "..\\LOC_CommonAPI\\LocCommonAPI.h"
#include "..\\..\\..\\..\\ESL\\SIReportMgr\\SIReportManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArrearAllDlg dialog


CArrearAllDlg::CArrearAllDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CArrearAllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArrearAllDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_bIs3100 = FALSE;
}
CArrearAllDlg::~CArrearAllDlg()
{
}

void CArrearAllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArrearAllDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArrearAllDlg, CDialog)
	//{{AFX_MSG_MAP(CArrearAllDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnAA_SEARCH, OnbtnAASEARCH)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArrearAllDlg message handlers

BOOL CArrearAllDlg::PreTranslateMessage(MSG* pMsg) 
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
			if(GetDlgItem(IDC_btnAA_SEARCH)->m_hWnd == pMsg->hwnd )
			{
				OnbtnAASEARCH();
			}
			return TRUE;
		}
 	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CArrearAllDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	INT ids;
	ids = InitESL_Mgr(_T("SM_ARREAR_ALL"));
	if(0 > ids) return FALSE;
	
	CESL_Mgr* pMgr = m_pParent->FindSM(_T("BO_LOC_3100"));
	if(NULL != pMgr)
	{
		m_bIs3100 = TRUE;
	}

	ids = InitControl();
	if(0 > ids) return FALSE;

// 	SetTimer(1, 300, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CArrearAllDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void CArrearAllDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CWnd* pGrid = GetDlgItem(IDC_gridAA_MAIN);
	if(pGrid->GetSafeHwnd() == NULL) return;

	CRect rDlg, rGrid;
	GetWindowRect(rDlg);
	pGrid->GetWindowRect(rGrid);
	ScreenToClient(rGrid);

	pGrid->MoveWindow(rGrid.left, rGrid.top, rDlg.Width(), rDlg.Height()-rGrid.top);
}

void CArrearAllDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(1);
		SearchStart();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CArrearAllDlg::OnbtnAASEARCH() 
{
	SetTimer(1, 100, NULL);
}

INT CArrearAllDlg::InitControl()
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

	SetControlData(_T("CM_ARREAR_ALL"), _T("반납일자1"), strBefore);
	SetControlData(_T("CM_ARREAR_ALL"), _T("반납일자2"), strCurrent);
//  	SetControlData(_T("CM_ARREAR_ALL"), _T("납입일자1"), strBefore);
//  	SetControlData(_T("CM_ARREAR_ALL"), _T("납입일자2"), strCurrent);

	return 0;
}

INT CArrearAllDlg::SearchStart()
{
	CString strFuncName = _T("SearchStart");

	CString strBefore, strAfter;
	GetControlData(_T("CM_ARREAR_ALL"), _T("반납일자1"), strBefore);
	GetControlData(_T("CM_ARREAR_ALL"), _T("반납일자2"), strAfter);
	if(10 != strBefore.GetLength()) 
	{
		strBefore = _T("");
		SetControlData(_T("CM_ARREAR_ALL"), _T("반납일자1"), strBefore);
	}
	if(10 != strAfter.GetLength())
	{
		strAfter = _T("");
		SetControlData(_T("CM_ARREAR_ALL"), _T("반납일자2"), strAfter);
	}
	CString strPaymentWhere, strUnpaidWhere;
	if(!strBefore.IsEmpty() && !strAfter.IsEmpty())
	{
		strPaymentWhere.Format(_T("CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strBefore, strAfter);
		strUnpaidWhere.Format(_T("CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strBefore, strAfter);
	}
	else if(!strBefore.IsEmpty() && strAfter.IsEmpty())
	{
		strPaymentWhere.Format(_T("CL.RETURN_DATE > TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS')"), strBefore);
		strUnpaidWhere.Format(_T("CL.RETURN_DATE > TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS')"), strBefore);
	}
	else if(strBefore.IsEmpty() && !strAfter.IsEmpty())
	{
		strPaymentWhere.Format(_T("CL.RETURN_DATE < TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strAfter);
		strUnpaidWhere.Format(_T("CL.RETURN_DATE < TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strAfter);
	}
	else
	{
		strPaymentWhere = _T("");
		strUnpaidWhere = _T("");
	}

	GetControlData(_T("CM_ARREAR_ALL"), _T("납입일자1"), strBefore);
	GetControlData(_T("CM_ARREAR_ALL"), _T("납입일자2"), strAfter);
	if(10 != strBefore.GetLength()) 
	{
		strBefore = _T("");
		SetControlData(_T("CM_ARREAR_ALL"), _T("납입일자1"), strBefore);
	}
	if(10 != strAfter.GetLength())
	{
		strAfter = _T("");
		SetControlData(_T("CM_ARREAR_ALL"), _T("납입일자2"), strAfter);
	}
	CString strTemp;
	if(!strBefore.IsEmpty() && !strAfter.IsEmpty())
	{
		strTemp.Format(_T("CL.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strBefore, strAfter);
	}
	else if(!strBefore.IsEmpty() && strAfter.IsEmpty())
	{
		strTemp.Format(_T("CL.PAYMENT_DATE > TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS')"), strBefore);
	}
	else if(strBefore.IsEmpty() && !strAfter.IsEmpty())
	{
		strTemp.Format(_T("CL.PAYMENT_DATE < TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strAfter);
	}
	else
	{
		strTemp = _T("");
	}
	if(!strTemp.IsEmpty())
	{
		if(!strPaymentWhere.IsEmpty()) strPaymentWhere += _T(" AND ");
		strPaymentWhere += strTemp;
	}

	CString strManageCode;
	GetControlData(_T("CM_ARREAR_ALL"), _T("관리구분"), strManageCode);
	if(!strManageCode.IsEmpty())
	{
		if(!strPaymentWhere.IsEmpty()) strPaymentWhere += _T(" AND ");
		strPaymentWhere += _T("MU.MANAGE_CODE='") + strManageCode + _T("' ");
		if(!strUnpaidWhere.IsEmpty()) strUnpaidWhere += _T(" AND ");
		strUnpaidWhere += _T("CL.MANAGE_CODE='") + strManageCode + _T("' ");
	}

	if(TRUE == m_bIs3100)
	{
		CString strUserKey;
		CESL_DataMgr* pMemberDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		pMemberDM->GetCellData(_T("대출자KEY"), 0, strUserKey);
		if(strUserKey.IsEmpty()) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		if(strPaymentWhere.IsEmpty())
		{
			strPaymentWhere.Format(_T("CLU.REC_KEY=%s"), strUserKey);
		}
		else
		{
			CString strValue;
			strValue.Format(_T(" AND CLU.REC_KEY=%s"), strUserKey);
			strPaymentWhere += strValue;
		}
		if(strUnpaidWhere.IsEmpty())
		{
			strUnpaidWhere.Format(_T("CLU.REC_KEY=%s"), strUserKey);
		}
		else
		{
			CString strValue;
			strValue.Format(_T(" AND CLU.REC_KEY=%s"), strUserKey);
			strUnpaidWhere += strValue;
		}
	}

	if(!strPaymentWhere.IsEmpty()) strPaymentWhere = _T("AND ") + strPaymentWhere ;
	if(!strUnpaidWhere.IsEmpty()) strUnpaidWhere = _T("AND ") + strUnpaidWhere ;

	CString strQuery;
	strQuery.Format(
		_T("SELECT CL.REC_KEY,TRIM(TO_CHAR(CL.PAYMENTS,'999,999,999')) AS ARREAR,CL.REG_NO,CL.TITLE,CL.AUTHOR,CL.PUBLISHER,CL.CALL_NO,CL.SHELF_LOC_CODE,CL.MANAGE_CODE,TO_CHAR(CL.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS') AS PAYMENT_DATE,TO_CHAR(CL.LOAN_DATE, 'YYYY/MM/DD HH24:MI:SS'),TO_CHAR(CL.RETURN_PLAN_DATE, 'YYYY/MM/DD'),TO_CHAR(CL.RETURN_DATE, 'YYYY/MM/DD HH24:MI:SS') AS RETURN_DATE,ECO$F_GET_CODE2DESC('1', CL.MANAGE_CODE),ECO$F_GET_CODE2DESC('19', CL.SHELF_LOC_CODE),CLU.REC_KEY AS USER_REC_KEY,CLU.USER_NO,CLU.USER_ID,CLU.NAME,CLU.USER_CLASS_CODE,CLU.USER_POSITION_CODE,ECO$F_GET_CODE2DESC('30', CLU.USER_POSITION_CODE),ECO$F_GET_CODE2DESC('31', CLU.USER_CLASS_CODE),MU.REC_KEY,MU.ID,MU.NAME,MU.MANAGE_CODE,ECO$F_GET_CODE2DESC('1', MU.MANAGE_CODE),'LS_WORK_T01','납입' AS SORT,NULL ")
		_T("FROM LS_WORK_T01 CL,CO_LOAN_USER_TBL CLU,MN_USER_TBL MU ")
		_T("WHERE CLU.REC_KEY=CL.USER_KEY AND MU.REC_KEY=CL.ACCEPTER_KEY AND CL.PAYMENT_DATE IS NOT NULL AND 0<CL.PAYMENTS AND TO_DATE(CL.RETURN_DATE,'YYYY/MM/DD')>TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD') AND CL.STATUS IN ('1','R') AND CL.RETURN_DATE IS NOT NULL %s ")
		_T("UNION ")
		_T("SELECT CL.REC_KEY,TRIM(TO_CHAR(CL.PAYMENTS,'999,999,999')) AS ARREAR,CL.REG_NO,CL.TITLE,CL.AUTHOR,CL.PUBLISHER,CL.CALL_NO,CL.SHELF_LOC_CODE,CL.MANAGE_CODE,TO_CHAR(CL.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS') AS PAYMENT_DATE,TO_CHAR(CL.LOAN_DATE, 'YYYY/MM/DD HH24:MI:SS'),TO_CHAR(CL.RETURN_PLAN_DATE, 'YYYY/MM/DD'),TO_CHAR(CL.RETURN_DATE, 'YYYY/MM/DD HH24:MI:SS') AS RETURN_DATE,ECO$F_GET_CODE2DESC('1', CL.MANAGE_CODE),ECO$F_GET_CODE2DESC('19', CL.SHELF_LOC_CODE),CLU.REC_KEY AS USER_REC_KEY,CLU.USER_NO,CLU.USER_ID,CLU.NAME,CLU.USER_CLASS_CODE,CLU.USER_POSITION_CODE,ECO$F_GET_CODE2DESC('30', CLU.USER_POSITION_CODE),ECO$F_GET_CODE2DESC('31', CLU.USER_CLASS_CODE),MU.REC_KEY,MU.ID,MU.NAME,MU.MANAGE_CODE,ECO$F_GET_CODE2DESC('1', MU.MANAGE_CODE),'CO_LOAN_TBL','납입' AS SORT,NULL ")
		_T("FROM CO_LOAN_TBL CL,CO_LOAN_USER_TBL CLU,MN_USER_TBL MU ")
		_T("WHERE CLU.REC_KEY=CL.USER_KEY AND MU.REC_KEY=CL.ACCEPTER_KEY AND CL.PAYMENT_DATE IS NOT NULL AND 0<CL.PAYMENTS AND TO_DATE(CL.RETURN_DATE,'YYYY/MM/DD')>TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD') AND CL.STATUS IN ('1','R') AND CL.RETURN_DATE IS NOT NULL %s ")
		_T("UNION ")
		_T("SELECT CL.REC_KEY,TRIM(TO_CHAR(CL.ARREAR,'999,999,999')) AS ARREAR,CL.REG_NO,CL.TITLE,CL.AUTHOR,CL.PUBLISHER,CL.CALL_NO,CL.SHELF_LOC_CODE,CL.MANAGE_CODE,NULL AS PAYMENT_DATE,TO_CHAR(CL.LOAN_DATE, 'YYYY/MM/DD HH24:MI:SS'),TO_CHAR(CL.RETURN_PLAN_DATE, 'YYYY/MM/DD'),TO_CHAR(CL.RETURN_DATE, 'YYYY/MM/DD HH24:MI:SS') AS RETURN_DATE,ECO$F_GET_CODE2DESC('1', CL.MANAGE_CODE),ECO$F_GET_CODE2DESC('19', CL.SHELF_LOC_CODE),CLU.REC_KEY AS USER_REC_KEY,CLU.USER_NO,CLU.USER_ID,CLU.NAME,CLU.USER_CLASS_CODE,CLU.USER_POSITION_CODE,ECO$F_GET_CODE2DESC('30', CLU.USER_POSITION_CODE),ECO$F_GET_CODE2DESC('31', CLU.USER_CLASS_CODE),NULL,NULL,NULL,NULL,NULL,'LS_WORK_T01','미납' AS SORT,NULL ")
		_T("FROM LS_WORK_T01 CL,CO_LOAN_USER_TBL CLU ")
		_T("WHERE CLU.REC_KEY=CL.USER_KEY AND CL.PAYMENT_DATE IS NULL AND 0<CL.ARREAR AND TO_DATE(CL.RETURN_DATE,'YYYY/MM/DD')>TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD') AND CL.STATUS IN ('1','R') AND CL.RETURN_DATE IS NOT NULL %s ")
		_T("UNION ")
		_T("SELECT CL.REC_KEY,TRIM(TO_CHAR(CL.ARREAR,'999,999,999')) AS ARREAR,CL.REG_NO,CL.TITLE,CL.AUTHOR,CL.PUBLISHER,CL.CALL_NO,CL.SHELF_LOC_CODE,CL.MANAGE_CODE,NULL AS PAYMENT_DATE,TO_CHAR(CL.LOAN_DATE, 'YYYY/MM/DD HH24:MI:SS'),TO_CHAR(CL.RETURN_PLAN_DATE, 'YYYY/MM/DD'),TO_CHAR(CL.RETURN_DATE, 'YYYY/MM/DD HH24:MI:SS') AS RETURN_DATE,ECO$F_GET_CODE2DESC('1', CL.MANAGE_CODE),ECO$F_GET_CODE2DESC('19', CL.SHELF_LOC_CODE),CLU.REC_KEY AS USER_REC_KEY,CLU.USER_NO,CLU.USER_ID,CLU.NAME,CLU.USER_CLASS_CODE,CLU.USER_POSITION_CODE,ECO$F_GET_CODE2DESC('30', CLU.USER_POSITION_CODE),ECO$F_GET_CODE2DESC('31', CLU.USER_CLASS_CODE),NULL,NULL,NULL,NULL,NULL,'CO_LOAN_TBL','미납' AS SORT,NULL ")
		_T("FROM CO_LOAN_TBL CL,CO_LOAN_USER_TBL CLU ")
		_T("WHERE CLU.REC_KEY=CL.USER_KEY AND CL.PAYMENT_DATE IS NULL AND 0<CL.ARREAR AND TO_DATE(CL.RETURN_DATE,'YYYY/MM/DD')>TO_DATE(CL.RETURN_PLAN_DATE,'YYYY/MM/DD') AND CL.STATUS IN ('1','R') AND CL.RETURN_DATE IS NOT NULL %s ")
		_T("ORDER BY SORT, RETURN_DATE DESC, PAYMENT_DATE DESC"), strPaymentWhere, strPaymentWhere, strUnpaidWhere, strUnpaidWhere);
	CESL_DataMgr *pDM = FindDM(_T("DM_ARREAR_ALL"));
	pDM->RestructDataManager(strQuery);
	ControlDisplay(_T("CM_ARREAR_ALL"), _T("전체그리드"));

	return 0;
}

INT CArrearAllDlg::ExcelReport()
{
	INT ids;
	CString strFuncName = _T("ExcelReport");

	CESL_DataMgr *pDM = FindDM(_T("DM_ARREAR_ALL"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	if(1 > pDM->GetRowCount())
	{
		MessageBox(_T("출력할 이력이 없습니다."), _T("출력"), MB_ICONINFORMATION);
		return 1;
	}
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("전체연체료현황"));
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