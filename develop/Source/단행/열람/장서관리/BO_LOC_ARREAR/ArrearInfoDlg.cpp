// ArrearInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArrearInfoDlg.h"
#include "..\\LOC_CommonAPI\\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArrearInfoDlg dialog


CArrearInfoDlg::CArrearInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CArrearInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strLoanStopDate = _T("");
	m_strIsIncludeLoanCnt = _T("");
	m_strUserKey = _T("");
}
CArrearInfoDlg::~CArrearInfoDlg()
{
	m_Brush.DeleteObject();
}

void CArrearInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArrearInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArrearInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CArrearInfoDlg)
	ON_BN_CLICKED(IDC_btnAI_CLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnAI_PAYMENTS, OnbtnArrearCheck)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArrearInfoDlg message handlers

BOOL CArrearInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	m_Brush.CreateSolidBrush(RGB(255,0,0));

	if(InitESL_Mgr( _T("SM_BO_LOC_ARREAR") ) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	SetTimer(1, 100, NULL);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CArrearInfoDlg::InitData()
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_ARREAR"));
	if(NULL==pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("InitData"));
	CESL_DataMgr *pCoDM = FindDM(_T("DM_BO_LOC_ARREAR_CO"));
	if(NULL==pCoDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("InitData"));

	if(_T("Y") != m_strIsIncludeLoanCnt)
	{
		INT nIndex;
		CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
		CString strFieldName = _T("DECODE(LOAN_KEY,'1','0',TRIM(TO_CHAR(ARREAR,'999,999,999')))");
		nIndex = pDM->FindColumn(_T("연체료")); 
		if(-1 < nIndex)
		{
			ids = pDM->GetColumnRefInfo(nIndex, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("InitData"));
			ids = pDM->SetDataMgrRef(nIndex, FIELD_ALIAS, strFieldName, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("InitData"));
			ids = pCoDM->GetColumnRefInfo(nIndex, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("InitData"));
			ids = pCoDM->SetDataMgrRef(nIndex, FIELD_ALIAS, strFieldName, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("InitData"));
		}
	}

	CString strWhere;
	strWhere.Format(_T("USER_KEY=%s"), m_strUserKey);
	pDM->RefreshDataManager(strWhere);
	pCoDM->RefreshDataManager(strWhere);
	pDM->AddDM(pCoDM);
	pDM->SORT(_T("반납일"));

	INT nTotal = 0;
	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		CString strValue;
		pDM->GetCellData(_T("연체료"), i, strValue);
		strValue.Replace(_T(","), _T(""));
		nTotal += _ttoi(strValue);
	}
	CString strArrear;
	strArrear.Format(_T("%d"), nTotal);
	CLocCommonAPI::SetMoneyFormat(strArrear);
	
	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_ARREAR") );
	if(NULL==pCM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("InitData"));
	pCM->ControlDisplay(_T("ArrearGrid"));
	pCM->SetControlMgrData(_T("총연체료"), strArrear);
	if(!m_strLoanStopDate.IsEmpty())
	{
		ids = pCM->SetControlMgrData( _T("대출정지일"), m_strLoanStopDate );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("InitData"));
	}

	CESL_DataMgr *pMemberDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("InitData"));
	CString strUserNo;
	pMemberDM->GetCellData(_T("대출자번호"), 0, strUserNo);
	pCM->SetControlMgrData(_T("대출자번호"), strUserNo);
	CString strUserName;
	pMemberDM->GetCellData(_T("대출자이름"), 0, strUserName);
	pCM->SetControlMgrData(_T("대출자이름"), strUserName);

	return 0;

}

void CArrearInfoDlg::OnbtnCLOSE() 
{
	CDialog::OnCancel();	
}

void CArrearInfoDlg::OnbtnArrearCheck() 
{
	CString strQuery;
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_ARREAR"));
	if(NULL == pDM) ERROR_MSG_RETURN_VOID2(-1000, _T("OnbtnArrearCheck"));

	pDM->StartFrame();
	pDM->InitDBFieldData();	

	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		CString strArrear;
		pDM->GetCellData(_T("연체료"), i, strArrear);
		if(strArrear.IsEmpty()) strArrear = _T("ARREAR");
		strArrear.Replace(_T(","), _T(""));
		CString strLoanKey;
		pDM->GetCellData(_T("REC_KEY"), i, strLoanKey);
		CString strTable;
		pDM->GetCellData(_T("TABLE"), i, strTable);
		if(_T("CO_LOAN_TBL") == strTable)
		{
			strQuery.Format(_T("UPDATE CO_LOAN_TBL SET ARREAR=0,PREV_ARREAR=%s,PAYMENT_DATE=SYSDATE,PAYMENTS=%s,ACCEPTER_KEY=%s,LAST_WORK='%s' WHERE REC_KEY=%s;"), strArrear, strArrear, m_pInfo->USER_PK, GetWorkLogMsg(_T("납입처리"),__WFILE__,__LINE__), strLoanKey);
		}
		else
		{
			strQuery.Format(_T("UPDATE LS_WORK_T01 SET ARREAR=0,PREV_ARREAR=%s,PAYMENT_DATE=SYSDATE,PAYMENTS=%s,ACCEPTER_KEY=%s,LAST_WORK='%s' WHERE REC_KEY=%s;"), strArrear, strArrear, m_pInfo->USER_PK, GetWorkLogMsg(_T("납입처리"),__WFILE__,__LINE__), strLoanKey);
		}
		pDM->AddFrame(strQuery);
	}

	pDM->SendFrame(TRUE);	
	pDM->EndFrame();
	
	CESL_DataMgr *pMemberDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pMemberDM) ERROR_MSG_RETURN_VOID2(-1000, _T("OnbtnArrearCheck"));
	pMemberDM->SetCellData(_T("총연체료"), _T("0"), 0);

	CDialog::OnOK();	
}

BOOL CArrearInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnCLOSE();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnAI_PAYMENTS)->m_hWnd == pMsg->hwnd )
			{
				OnbtnArrearCheck();
			}
			else if(GetDlgItem(IDC_btnAI_CLOSE)->m_hWnd == pMsg->hwnd )
			{
				OnbtnCLOSE();
			}
			return TRUE;
		}
 	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CArrearInfoDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(1);
		InitData();
	}
	
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CArrearInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if(IDC_stcAI_MSG == pWnd->GetDlgCtrlID())
	{
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkColor(RGB(255,0,0));
		return (HBRUSH) m_Brush;
	}

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

BEGIN_EVENTSINK_MAP(CArrearInfoDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CArrearInfoDlg)
	ON_EVENT(CArrearInfoDlg, IDC_gridAI_MAIN, -604 /* KeyUp */, OnKeyUpgridAIMAIN, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CArrearInfoDlg::OnKeyUpgridAIMAIN(short FAR* KeyCode, short Shift) 
{
	if(VK_ESCAPE == *KeyCode)
	{
		PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
	}
}
