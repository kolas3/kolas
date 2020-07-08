// ILL_RETURN.cpp : implementation file
//

#include "stdafx.h"
#include "ILL_RETURN.h"
#include "Imm.h"
#include "ILL_RETURN_SEARCH_BOOK.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CILL_RETURN dialog


CILL_RETURN::CILL_RETURN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CILL_RETURN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
 	mCtrl_pDlg_OTHER = NULL;
 	mCtrl_pDlg_WE = NULL;
	m_pPreRegNoMgr = NULL;
	m_bIsOtherLoanReturn = FALSE;
}
CILL_RETURN::~CILL_RETURN()
{
	if(NULL != mCtrl_pDlg_OTHER) delete mCtrl_pDlg_OTHER;
	if(NULL != mCtrl_pDlg_WE) delete mCtrl_pDlg_WE;

}

void CILL_RETURN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CILL_RETURN)
	DDX_Control(pDX, IDC_btn_ILL_RETURN, mCtrl_btnReturn);
	DDX_Control(pDX, IDC_tab_ILL_RETURN, mCtrl_TabMain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CILL_RETURN, CDialog)
	//{{AFX_MSG_MAP(CILL_RETURN)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tab_ILL_RETURN, OnSelchangetabILLRETURN)
	ON_BN_CLICKED(IDC_btn_ILL_RETURN, OnbtnILLRETURN)
	ON_EN_SETFOCUS(IDC_edt_ILL_RETURN_REG_NO, OnSetfocusRegNo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+100,OnbtnILLRETURN)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CILL_RETURN message handlers

BOOL CILL_RETURN::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(0 > InitESL_Mgr(_T("SM_ILL_RETURN")))
 	{
 		AfxMessageBox(_T("ERROR : InitESL_Mgr(\"SM_ILL_RETURN\")"));
 	}

	mCtrl_pDlg_OTHER = new CILL_RETURN_OTHER(this);
	mCtrl_pDlg_WE = new CILL_RETURN_WE(this);

	m_strLibCode = m_pInfo->LIB_CODE;
	mCtrl_pDlg_OTHER->m_strLibCode = m_strLibCode;
	mCtrl_pDlg_WE->m_strLibCode = m_strLibCode;
	{
	int ids = 0;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(
			_T("SELECT VALUE_1 ")
			_T("FROM ILL_MANAGE_TBL ")
			_T("WHERE VALUE_NAME = '타관반납기능사용유무' AND MANAGE_CODE = '%s'"),
			m_pInfo->MANAGE_CODE);
	
	CString strValue;
	ids = TmpDM.GetOneValueQuery(strQuery, strValue);
	if(0 > ids) return FALSE;
	
	m_bIsOtherLoanReturn = (_T("Y") == strValue) ? TRUE : FALSE;
	}
	mCtrl_pDlg_OTHER->m_bIsOtherLoanReturn = m_bIsOtherLoanReturn;

	mCtrl_pDlg_OTHER->m_strUserKey = m_strUserKey;
	mCtrl_pDlg_WE->m_strUserKey = m_strUserKey;	

	mCtrl_TabMain.InsertItem(0,	_T("우리도서관 대출자료"));
	mCtrl_TabMain.InsertItem(1,	_T("타관도서관 대출자료"));

	mCtrl_pDlg_WE->Create(this);
	mCtrl_pDlg_OTHER->Create(this);
	
	mCtrl_pDlg_WE->m_pLoan = m_pLoan;
	mCtrl_pDlg_OTHER->m_pLoan = m_pLoan;

	Refresh();
	SetMode(0);
	ResizeControl();

	CString strRegNo;
	GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strRegNo);
	strRegNo.MakeUpper();

	int nMode = 0;	
	int nRowWe = mCtrl_pDlg_WE->FindRegNo(strRegNo);
	int nRowOther = mCtrl_pDlg_OTHER->FindRegNo(strRegNo);

	if(0 <= nRowWe) 
	{
		nMode = 0;
		CheckGrid(mCtrl_pDlg_WE->m_pGrid, nRowWe, TRUE);
	}
	else if(0 <= nRowOther)
	{
		nMode = 1;
		CheckGrid(mCtrl_pDlg_OTHER->m_pGrid, nRowOther, TRUE);
	}	

	SetMode(nMode, FALSE);
	
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 

	GetDlgItem(IDC_edt_ILL_RETURN_REG_NO)->SetFocus();
	
	return FALSE;
	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CILL_RETURN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

void CILL_RETURN::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ResizeControl();
}

int CILL_RETURN::ResizeControl()
{
	CRect rDlg;
	GetClientRect(rDlg);

	// 
	{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDOK);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return -2;

	CRect rBtn;
 	pWnd->GetWindowRect(rBtn);
	ScreenToClient(rBtn);

	rBtn.OffsetRect(-rBtn.left, 0);
	rBtn.OffsetRect(rDlg.right - rBtn.Width() -5, 0);

	pWnd->MoveWindow(rBtn);
	}
	{
	
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_tab_ILL_RETURN);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return -3;
	
	CRect rTab;
 	pWnd->GetWindowRect(rTab);
	ScreenToClient(rTab);

	CRect rSubDlg = rDlg;
	rSubDlg.left = 0;
	rSubDlg.right = rDlg.right;
	rSubDlg.bottom = rDlg.bottom;
	rSubDlg.top = rTab.bottom;

 	mCtrl_pDlg_OTHER->MoveWindow(rSubDlg);
 	mCtrl_pDlg_WE->MoveWindow(rSubDlg);
	}

	return 0;
}

void CILL_RETURN::OnSelchangetabILLRETURN(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIdx = mCtrl_TabMain.GetCurSel();
	SetMode(nIdx);

	*pResult = 0;
}

int CILL_RETURN::SetMode(int nMode, BOOL bRefresh)
{
	if(0 == nMode)
	{
		if(TRUE == bRefresh) mCtrl_pDlg_WE->Refresh();
		mCtrl_pDlg_WE->ShowWindow(SW_SHOW);
		mCtrl_pDlg_OTHER->ShowWindow(SW_HIDE);
		mCtrl_TabMain.SetCurSel(0);
	}
	else if(1 == nMode)
	{
		if(TRUE == bRefresh) mCtrl_pDlg_OTHER->Refresh();
		mCtrl_pDlg_WE->ShowWindow(SW_HIDE);
		mCtrl_pDlg_OTHER->ShowWindow(SW_SHOW);
		mCtrl_TabMain.SetCurSel(1);
	}
	else return -1;

	m_nMode = nMode;
	UpdateWindow();

	return 0;
}

int CILL_RETURN::Return()
{
	int ids = 0;
	if(NULL == m_pLoan) return FALSE;
	ids = m_pLoan->IsSearchedUser();
	if(0 > ids) return ids;
	
	ResetCheckLoanInfoGrid();

	// 등록번호 체크
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_edt_ILL_RETURN_REG_NO);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return -1;
		
	CString strRegNo = _T("");
	pWnd->GetWindowText(strRegNo);
	strRegNo.MakeUpper();

	SetDlgItemText(IDC_edt_ILL_RETURN_REG_NO, _T(""));

	CString strInBarCodeLibCode = _T("");
	CString strInBarCodeLibMngCode = _T("");

	if(_T("") != strRegNo)
	{

		int ids = 0;
		strRegNo.MakeUpper();

		if ( strRegNo.GetLength() == 19 ) {
			CString strTmp = _T("");
			strTmp = strRegNo.Mid(7);			
			strInBarCodeLibCode = strRegNo.Mid(1, 6);
			strRegNo = strTmp;
			
			CESL_DataMgr TmpDM;
			TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
			CString strQuery;
			strQuery.Format(_T("SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE=%s"), strInBarCodeLibCode);
			ids = TmpDM.GetOneValueQuery(strQuery, strInBarCodeLibMngCode);
		}

 		if(0 != GetMode())
 		{
			CString strMsg;
 			BOOL bIsReturn = mCtrl_pDlg_OTHER->IsReturn(strRegNo, strMsg);
 			if(FALSE == bIsReturn) 
			{
 				AfxMessageBox(strMsg); 
 				return 0;
 			}
 		}
		
		CString strLoanKey;
		ids = GetLoanKey(strRegNo, strLoanKey, strInBarCodeLibMngCode);
		if(0 == ids)
		{
			CheckLoanInfoGrid(strLoanKey);
		}
		else return -1;
	}
	else
	{	
		if(1 == GetMode())
		{
			mCtrl_pDlg_OTHER->UnCheckGrid_For_IsNotReturnRegNo();
		}

		CESL_Grid* pGrid = (0 == GetMode()) ? mCtrl_pDlg_WE->m_pGrid : mCtrl_pDlg_OTHER->m_pGrid;
		CESL_DataMgr* pDM = (0 == GetMode()) ? mCtrl_pDlg_WE->m_pDM : mCtrl_pDlg_OTHER->m_pDM;

		pGrid->SelectMakeList();
		if(0 >= pGrid->SelectGetCount())
		{
			AfxMessageBox(_T("먼저 반납할 자료를 선정하여 주십시요."));
			return 0;
		}
		
		INT ids = 0;
		CString strFuncName = _T("Return()");
		CString strMotherKey;
		CString strTemp;
		int nIdx = pGrid->SelectGetHeadPosition();
		do
		{
			ids = pDM->GetCellData(_T("모체자료_키"), nIdx, strMotherKey);
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
			}
			if(_T("") == strMotherKey)
			{
				break;
			}
			
			INT nCnt = 0;
			INT nResult;
			for(INT i=0; i<pDM->GetRowCount(); i++)
			{
				if(_T("V") == pGrid->GetAt(i, 0))
				{
					continue;
				}
				else
				{
					//선정되지 않은것만 비교한다.
					ids = pDM->GetCellData(_T("모체자료_키"), i, strTemp);
					if(0 > ids) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
					}
					if(strTemp == strMotherKey)
					{
						//1번만 물어본다
						if(0 == nCnt)
						{
							nResult = AfxMessageBox(_T("선정되지 않은 부록(모체)자료가 존재합니다.\r\r해당 자료를 일괄처리 하시겠습니까?"), MB_YESNO);
						}
						if(IDYES == nResult)
						{					
							nCnt++;
							pGrid->SetAt(i, 0, _T("V"));
							continue;
						}
						else
						{
							break;
						}
					}
				}
			}
			nIdx = pGrid->SelectGetNext();
		}
		while(0 <= nIdx);
		pGrid->SelectMakeList();

		CopyGridCheckToLoanInfoGrid(pDM, pGrid);
	}

	m_pLoan->SPFExcutePath(_T("책두레반납"));

	if(0 == GetMode()) mCtrl_pDlg_WE->Refresh();		
	else mCtrl_pDlg_OTHER->Refresh();		

	GetDlgItem(IDC_edt_ILL_RETURN_REG_NO)->SetFocus();

	return 0;
}

void CILL_RETURN::OnbtnILLRETURN() 
{
	// TODO: Add your control notification handler code here
 	if(FALSE == IsSearchUser())
 	{
		SearchUser();
		Refresh();
 	}
	else
	{
		Return();
	}	
}


int CILL_RETURN::GetLoanKey(CString strRegNo, CString &strLoanKey, CString strInBarCodeLibMngCode)
{
	int ids = 0;
	CESL_DataMgr* pDM = (0 == GetMode()) ? mCtrl_pDlg_WE->m_pDM : mCtrl_pDlg_OTHER->m_pDM;
	if(NULL == pDM) return -1;
	CString strManageCodeAlias = (0 == GetMode()) ? _T("제공도서관_관리구분") : _T("관리구분_책");

	int nFindCnt = 0;
	CString strTmp, strTmp2, strPreRegNo;
	const int ROW = pDM->GetRowCount();

	for(int nRow = 0; nRow < ROW; nRow++)
	{
		strPreRegNo = strRegNo;
		ids = pDM->GetCellData(_T("등록번호"), nRow, strTmp);
		if(0 > ids) return ids;

		ids = pDM->GetCellData(strManageCodeAlias, nRow, strTmp2);
		if(0 > ids) return ids;

		if ( strInBarCodeLibMngCode.GetLength() == 0 ) {			
			ids = CLocCommonAPI::MakeRegNoSpaceToZero(this, strPreRegNo, strTmp2, m_pPreRegNoMgr);
			if(0 > ids) return ids;

			if(strTmp == strPreRegNo) {			
				ids = pDM->GetCellData(_T("대출반납키"), nRow, strLoanKey);
				if(0 > ids) return ids;			
				
				nFindCnt++;
			}
		}
		else {
			if(strTmp == strPreRegNo && strInBarCodeLibMngCode == strTmp2 ) {			
				ids = pDM->GetCellData(_T("대출반납키"), nRow, strLoanKey);
				if(0 > ids) return ids;			
				
				nFindCnt++;
			}
		}		
	}

	if(nFindCnt > 1) {
		strLoanKey = _T("");
		
		AfxMessageBox(
			_T("같은 등록번호를 가진 자료가 여러개 존재합니다.\r\n")
			_T("그리드에서 반납할 자료를 직접 선택한 후 반납버튼을 통해 반납해 주십시요."));
		return -2;
	}

	return 0;
}

int CILL_RETURN::GetMode()
{
	return m_nMode;
}
 
int CILL_RETURN::CheckLoanInfoGrid(CString strLoanKey)
{
	int ids = 0;
	CString strTmp = _T("");

	CESL_DataMgr* pLoanInfoDM = m_pLoan->m_pParentMgr->FindDM(
		_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pLoanInfoDM) return -1;

	CESL_Grid* pLoanInfoGrid = (CESL_Grid*)m_pLoan->m_pParentMgr->FindControl(
		_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid"));
	if(NULL == pLoanInfoGrid) return -2;


	const int ROW = pLoanInfoDM->GetRowCount();
	for(int nRow = 0; nRow < ROW; nRow++)
	{
		ids = pLoanInfoDM->GetCellData(_T("대출/반납정보KEY"), nRow, strTmp);
		if(0 > ids) return ids;
		if(_T("") == strLoanKey) continue;
		
		if(strTmp == strLoanKey)
		{
			// 선정체크
			pLoanInfoGrid->SetAt(
				nRow,
				pLoanInfoGrid->m_nSelectIdx,
				pLoanInfoGrid->m_sSelectString
				);
		}
	}

	return 0;
}

int CILL_RETURN::CopyGridCheckToLoanInfoGrid(CESL_DataMgr* pDM, CESL_Grid* pGrid)
{
	if(NULL == pGrid) return -1;

	ResetCheckLoanInfoGrid();

	CESL_DataMgr* pLoanInfoDM = m_pLoan->m_pParentMgr->FindDM(
		_T("DM_BO_LOC_3100_LOAN_INFO"));
	
	int ids = 0;
	ids = pGrid->SelectMakeList();
	if(0 > ids) return ids;

	if(0 >= pGrid->SelectGetCount()) return -4;

	{ // 
	CString strTmp = _T("");
	CString strLoanKey = _T("");
	int nIdx = pGrid->SelectGetHeadPosition();
	do
	{
		BOOL bIsReturn = TRUE;

		if(TRUE == bIsReturn)
		{
			ids = pDM->GetCellData(_T("대출반납키"), nIdx, strLoanKey);
			if(0 > ids) return ids;
		
			if(_T("") == strLoanKey) return -5;

			CheckLoanInfoGrid(strLoanKey);
		}
		nIdx = pGrid->SelectGetNext();

	}while(0 <= nIdx);
	}

	return 0;
}

int CILL_RETURN::ResetCheckLoanInfoGrid()
{
	CESL_Grid* pLoanInfoGrid = (CESL_Grid*)m_pLoan->m_pParentMgr->FindControl(
		_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid"));
	if(NULL == pLoanInfoGrid) return -2;
	{
	int ids = pLoanInfoGrid->SelectMakeList();
	if(0 > ids) return ids;
	
	if(0 < pLoanInfoGrid->SelectGetCount())
	{
		int nIdx = pLoanInfoGrid->SelectGetHeadPosition();
		do
		{
			pLoanInfoGrid->SetAt(
				nIdx,
				pLoanInfoGrid->m_nSelectIdx,
				_T("")
				);
			nIdx = pLoanInfoGrid->SelectGetNext();
		}while(0 <= nIdx);
	}
	}

	return 0;
}
void CILL_RETURN::OnSetfocusRegNo() 
{
	// TODO: Add your control notification handler code here
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edt_ILL_RETURN_REG_NO)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion = dwConversion & ~IME_CMODE_LANGUAGE;
		dwConversion |= IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edt_ILL_RETURN_REG_NO)->m_hWnd,hIME); 
    }
}

int CILL_RETURN::Refresh()
{
	
	CWnd* pWndRegNo = (CWnd*)GetDlgItem(IDC_staILL_RETURN_USER);
	if(NULL == pWndRegNo && NULL == pWndRegNo->GetSafeHwnd()) return 0;

	// 반납버튼
	CWnd* pWndBtn = (CWnd*)GetDlgItem(IDC_btn_ILL_RETURN);
	if(NULL == pWndBtn && NULL == pWndBtn->GetSafeHwnd()) return 0;
	
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("Refresh"));
	
	if(FALSE == IsSearchUser())
	{
		pWndBtn->SetWindowText(_T("검색"));
		pWndRegNo->SetWindowText(_T(""));
	}
	else
	{
		int ids = 0;
		CString strName = _T("");
		ids = pDM->GetCellData(_T("대출자이름"), 0, strName);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("Refresh") );
		
		CString strID = _T("");
		ids = pDM->GetCellData(_T("대출자ID"), 0, strID);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("Refresh") );
		
		ids = pDM->GetCellData(_T("대출자KEY"), 0, m_strUserKey);
		if(0 > ids) if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("Refresh") );		

		pWndRegNo->SetWindowText(strName + _T("(") + strID + _T(")"));
		pWndRegNo->ShowWindow(SW_SHOW);

		mCtrl_pDlg_OTHER->m_strUserKey = m_strUserKey;
		mCtrl_pDlg_WE->m_strUserKey = m_strUserKey;	


		if(0 == GetMode())
		{
			mCtrl_pDlg_WE->Refresh();
		}
		else
		{
			mCtrl_pDlg_OTHER->Refresh();
		}
	}
	return 0;
}

BOOL CILL_RETURN::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			OnbtnILLRETURN();

			return TRUE;
		}
	}
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

int CILL_RETURN::SearchUser()
{
 	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_edt_ILL_RETURN_REG_NO);
 	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return -1;
 
 	CString strRegNo = _T("");
 	pWnd->GetWindowText(strRegNo);
	strRegNo.MakeUpper();

	SetDlgItemText(IDC_edt_ILL_RETURN_REG_NO, _T(""));

 	if(_T("") != strRegNo)
 	{
 		int ids = 0;
 		ids = CLocCommonAPI::MakeRegNoSpaceToZero(strRegNo);
 		if(0 > ids) return -2;
 	}
	
	CString strQuery;
	CString strValue;
  	strQuery.Format(
		_T("SELECT USER_KEY, REC_KEY FROM LS_WORK_T01 WHERE STATUS IN ('0', '2', 'L', 'D') AND REG_NO = '%s'"),
		strRegNo);
	
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);	
	TmpDM.RestructDataManager(strQuery);
	
	int nRow = TmpDM.GetRowCount();
	if(0 == nRow) 
	{
		AfxMessageBox(_T("대출중인 책중에 해당 등록번호를 가진 책이 없습니다."));
		return -3;
	}	
	else if(1 == nRow)
	{
		CString strUserKey;
		TmpDM.GetCellData(0, 0, strUserKey);
		strQuery.Format(
			_T("SELECT USER_NO FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s"),
			strUserKey);
		TmpDM.GetOneValueQuery(strQuery, strValue); 
	}
	else 
	{
		CString strRecKeyList;
		
		CString strRecKey;			
		const int ROW_COUNT = TmpDM.GetRowCount();
		for(int nRow = 0; nRow < ROW_COUNT; nRow++)
		{
			TmpDM.GetCellData(nRow, 1, strRecKey);
			strRecKeyList = strRecKeyList + _T(", ") + strRecKey;
		}
		strRecKeyList.TrimLeft(_T(", "));
		
		CILL_RETURN_SEARCH_BOOK Dlg(this);
		Dlg.m_strOption = _T("REC_KEY IN (") + strRecKeyList + _T(")");
		if(IDOK == Dlg.DoModal())
		{
			CString strUserKey = Dlg.m_strUserKey;
			strQuery.Format(
				_T("SELECT USER_NO FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s"),
				strUserKey);
			TmpDM.GetOneValueQuery(strQuery, strValue); 
		}
		else return -4;
	}
		
	SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strValue);	
	m_pLoan->SPFExcutePath(_T("회원_검색_NOASK")); 

	return 0;
}

BOOL CILL_RETURN::IsSearchUser()
{
	CESL_DataMgr * pUserDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
 	return pUserDM->GetRowCount();	
}

int CILL_RETURN::CheckGrid(CESL_Grid *pGrid, int nRow, BOOL bIsCheck)
{
	if(NULL == pGrid) return -1;
	if((int)pGrid->GetRows() <= nRow) return -2;

	CString strCheckString = (TRUE == bIsCheck) ? 
		pGrid->m_sSelectString : _T("");

	pGrid->SetAt(
		nRow,
		pGrid->m_nSelectIdx,
		strCheckString
		);

	return 0;
}
