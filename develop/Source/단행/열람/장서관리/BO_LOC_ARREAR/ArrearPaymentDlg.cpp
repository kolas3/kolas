// ArrearPaymentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArrearPaymentDlg.h"
#include "..\\LOC_CommonAPI\\LocCommonAPI.h"
#include "..\\..\\..\\..\\ESL\\SIReportMgr\\SIReportManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArrearPaymentDlg dialog


CArrearPaymentDlg::CArrearPaymentDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CArrearPaymentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArrearPaymentDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_bIs3100 = FALSE;
}
CArrearPaymentDlg::~CArrearPaymentDlg()
{
}

void CArrearPaymentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArrearPaymentDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArrearPaymentDlg, CDialog)
	//{{AFX_MSG_MAP(CArrearPaymentDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnAP_SEARCH, OnbtnAPSEARCH)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArrearPaymentDlg message handlers

BOOL CArrearPaymentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	INT ids;
	ids = InitESL_Mgr(_T("SM_ARREAR_PAYMENT"));
	if(0 > ids) return FALSE;
	
	CESL_Mgr* pMgr = m_pParent->FindSM(_T("BO_LOC_3100"));
	if(NULL != pMgr)
	{
		m_bIs3100 = TRUE;
	}

	if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
	{
		CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_ARREAR_PAYMENT"), _T("납입그리드"));
		if(pGrid)
		{
			pGrid->SetColWidth(pGrid->m_nSelectIdx+1,0,0);
		}
	}

	ids = InitControl();
	if(0 > ids) return FALSE;

// 	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CArrearPaymentDlg::PreTranslateMessage(MSG* pMsg) 
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
			if( GetDlgItem(IDC_btnAP_SEARCH)->m_hWnd == pMsg->hwnd || 
				GetDlgItem(IDC_edtAP_BEFORE)->m_hWnd == pMsg->hwnd ||
				GetDlgItem(IDC_edtAP_AFTER)->m_hWnd == pMsg->hwnd )

			{
				OnbtnAPSEARCH();
			}
			return TRUE;
		}
 	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CArrearPaymentDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void CArrearPaymentDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CWnd* pGrid = GetDlgItem(IDC_gridAP_MAIN);
	if(pGrid->GetSafeHwnd() == NULL) return;

	CRect rDlg, rGrid;
	GetWindowRect(rDlg);
	pGrid->GetWindowRect(rGrid);
	ScreenToClient(rGrid);

	pGrid->MoveWindow(rGrid.left, rGrid.top, rDlg.Width(), rDlg.Height()-rGrid.top);
}

void CArrearPaymentDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(1);
		SearchStart();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CArrearPaymentDlg::OnbtnAPSEARCH() 
{
	SetTimer(1, 100, NULL);
}

BEGIN_EVENTSINK_MAP(CArrearPaymentDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CArrearPaymentDlg)
	ON_EVENT(CArrearPaymentDlg, IDC_gridAP_MAIN, -604 /* KeyUp */, OnKeyUpgridAPMAIN, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CArrearPaymentDlg::OnKeyUpgridAPMAIN(short FAR* KeyCode, short Shift) 
{
	if(VK_ESCAPE == *KeyCode)
	{
		PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

INT CArrearPaymentDlg::InitControl()
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

	SetControlData(_T("CM_ARREAR_PAYMENT"), _T("조회기간1"), strBefore);
	SetControlData(_T("CM_ARREAR_PAYMENT"), _T("조회기간2"), strCurrent);

	return 0;
}

INT CArrearPaymentDlg::SearchStart()
{
	CString strFuncName = _T("SearchStart");

	CString strBefore, strAfter;
	GetControlData(_T("CM_ARREAR_PAYMENT"), _T("조회기간1"), strBefore);
	GetControlData(_T("CM_ARREAR_PAYMENT"), _T("조회기간2"), strAfter);
	if(10 != strBefore.GetLength()) 
	{
		strBefore = _T("");
		SetControlData(_T("CM_ARREAR_PAYMENT"), _T("조회기간1"), strBefore);
	}
	if(10 != strAfter.GetLength())
	{
		strAfter = _T("");
		SetControlData(_T("CM_ARREAR_PAYMENT"), _T("조회기간2"), strAfter);
	}

	CString strWhere;
	if(!strBefore.IsEmpty() && !strAfter.IsEmpty())
	{
		strWhere.Format(_T("CL.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strBefore, strAfter);
	}
	else if(!strBefore.IsEmpty() && strAfter.IsEmpty())
	{
		strWhere.Format(_T("CL.PAYMENT_DATE > TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS')"), strBefore);
	}
	else if(strBefore.IsEmpty() && !strAfter.IsEmpty())
	{
		strWhere.Format(_T("CL.PAYMENT_DATE < TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strAfter);
	}
	else
	{
		strWhere = _T("");
	}

	CString strManageCode;
	GetControlData(_T("CM_ARREAR_PAYMENT"), _T("관리구분"), strManageCode);
	if(!strManageCode.IsEmpty())
	{
		if(!strWhere.IsEmpty()) strWhere += _T(" AND ");
		strWhere += _T("MU.MANAGE_CODE='") + strManageCode + _T("' ");
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

	CESL_DataMgr *pDM = FindDM(_T("DM_ARREAR_PAYMENT"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	CESL_DataMgr *pCoDM = FindDM(_T("DM_ARREAR_PAYMENT_CO"));
	if(NULL == pCoDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	pDM->RefreshDataManager(strWhere);
	pCoDM->RefreshDataManager(strWhere);
	pDM->AddDM(pCoDM);
	pDM->SORT(_T("납입일자"), TRUE);
// 	pDM->SORT(_T("USER_KEY"), TRUE);
	ControlDisplay(_T("CM_ARREAR_PAYMENT"), _T("납입그리드"));

	return 0;
}

INT CArrearPaymentDlg::SetUnPaid()
{
	CString strFuncName = _T("SetUnPaid");

	CESL_DataMgr *pDM = FindDM(_T("DM_ARREAR_PAYMENT"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	if(1 > pDM->GetRowCount())
	{
		return 0;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_ARREAR_PAYMENT"), _T("납입그리드"));
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	pGrid->SelectMakeList();
	if(1 > pGrid->SelectGetCount())
	{
		MessageBox(_T("미납 처리할 자료를 선정해주십시오."), _T("미납처리"), MB_ICONINFORMATION);
		return 0;
	}

	if(IDYES!=MessageBox(_T("선정된 이력을 미납처리 하시겠습니까?\r\n(납입하지 않은 상태로 전환합니다)"), _T("미납처리"), MB_YESNO|MB_ICONINFORMATION))
	{
		return 0;
	}

	INT nIdx;
	INT nTotArrear = 0;
	pDM->StartFrame();
	pDM->InitDBFieldData();	
	pGrid->SelectGetHeadPosition();
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
			MessageBox(strMsg, _T("미납처리"), MB_ICONINFORMATION);
			return 0;
		}
		strArrear.Replace(_T(","), _T(""));
		nTotArrear += _ttoi(strArrear);
		CString strQuery;
		if(!strLoanKey.IsEmpty() && !strTable.IsEmpty())
		{
			if(_T("CO_LOAN_TBL") == strTable)
			{
				strQuery.Format(_T("UPDATE CO_LOAN_TBL SET ARREAR=%s,PREV_ARREAR=NULL,PAYMENT_DATE=NULL,PAYMENTS=NULL,ACCEPTER_KEY=NULL,LAST_WORK='%s' WHERE REC_KEY=%s;"), strArrear, GetWorkLogMsg(_T("미납처리"),__WFILE__,__LINE__), strLoanKey);
			}
			else
			{
				strQuery.Format(_T("UPDATE LS_WORK_T01 SET ARREAR=%s,PREV_ARREAR=NULL,PAYMENT_DATE=NULL,PAYMENTS=NULL,ACCEPTER_KEY=NULL,LAST_WORK='%s' WHERE REC_KEY=%s;"), strArrear, GetWorkLogMsg(_T("미납처리"),__WFILE__,__LINE__), strLoanKey);
			}
		}
		pDM->AddFrame(strQuery);
		pGrid->SelectGetNext();
	}
	pDM->SendFrame(TRUE);
	pDM->EndFrame();

	// 검색된 이용자의 총연체료 증가
	if(TRUE == m_bIs3100)
	{
		CString strTotArrear;
		CESL_DataMgr* pMemberDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		pMemberDM->GetCellData(_T("총연체료"), 0, strTotArrear);
		if(strTotArrear.IsEmpty()) strTotArrear = _T("0");
		strTotArrear.Replace(_T(","), _T(""));
		INT nTot = _ttoi(strTotArrear);
		nTot += nTotArrear;
		strTotArrear.Format(_T("%d"), nTot);
		pMemberDM->SetCellData(_T("총연체료"), strTotArrear, 0);
	}

	return 0;
}

INT CArrearPaymentDlg::ExcelReport()
{
	INT ids;
	CString strFuncName = _T("ExcelReport");

	CESL_DataMgr *pDM = FindDM(_T("DM_ARREAR_PAYMENT"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	if(1 > pDM->GetRowCount())
	{
		MessageBox(_T("출력할 이력이 없습니다."), _T("출력"), MB_ICONINFORMATION);
		return 1;
	}
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("납입연체료현황"));
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
		pDM->GetCellData(_T("납입금액"), i, strArrear);
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