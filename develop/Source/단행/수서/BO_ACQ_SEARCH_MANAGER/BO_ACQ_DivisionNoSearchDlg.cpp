// BO_ACQ_DivisionNoSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_DivisionNoSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_DivisionNoSearchDlg dialog


CBO_ACQ_DivisionNoSearchDlg::CBO_ACQ_DivisionNoSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBO_SearchDlg(CBO_ACQ_DivisionNoSearchDlg::IDD, pParent)
{
	m_pCM = NULL;
	m_strSelDivNo = _T("");
}
CBO_ACQ_DivisionNoSearchDlg::~CBO_ACQ_DivisionNoSearchDlg()
{
}

VOID CBO_ACQ_DivisionNoSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_DivisionNoSearchDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_DivisionNoSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_DivisionNoSearchDlg)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_cmbDIVSEARCH_ACQCODE, OnSelchangecmbDIVSEARCHACQCODE)
	ON_BN_CLICKED(IDC_btnDIVSEARCH_VIEW, OnbtnDIVSEARCHVIEW)
	ON_EN_KILLFOCUS(IDC_edtDIVSEARCH_ACQYEAR, OnKillfocusedtDIVSEARCHACQYEAR)
	ON_EN_KILLFOCUS(IDC_edtDIVSEARCH_NO, OnKillfocusedtDIVSEARCHNO)
	ON_CBN_SELCHANGE(IDC_cmbSEARCH_DIVNO_WORK_CODE, OnSelchangecmbSEARCHDIVNOWORKCODE)
	ON_BN_CLICKED(IDC_btnDIVSEARCH_CLEAR, OnbtnDIVSEARCHCLEAR)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_BN_CLICKED(IDC_chkSORT_YN, OnchkSORTYN)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_DivisionNoSearchDlg message handlers

VOID CBO_ACQ_DivisionNoSearchDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CBO_ACQ_DivisionNoSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_차수접수번호검색")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM설정
	m_pCM = FindCM(_T("CM_차수접수번호검색"));	

	GetWindowRect(m_rcWindow);
	if(!Init())
	{
		ESLAfxMessageBox(_T("검색화면을 초기화할 수 없습니다."));
		return FALSE;
	}

	//============================================
	//20080603 ADD BY PJW : 수입년을 숫자 4자리 이상 넣지않도록 수정
	//CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtDIVSEARCH_ACQYEAR);
	//pAcqYear->SetLimitText(4);
	//============================================
	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_DivisionNoSearchDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CBO_ACQ_DivisionNoSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	// CM에서 사용자가 선정한 검색조건으로 검색 쿼리를 생성한다.
	CString strAcqYear, strAcqCode, strProvider, strWorkCode, strDate1, strDate2;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("수입구분"), strAcqCode);
	m_pCM->GetControlMgrData(_T("구입처"), strProvider);
	m_pCM->GetControlMgrData(_T("날짜1"), strDate1);
	m_pCM->GetControlMgrData(_T("날짜2"), strDate2);
	m_pCM->GetControlMgrData(_T("그룹번호"), m_strSelDivNo);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);

	TrimAll(strAcqYear);	TrimAll(strAcqCode);
	TrimAll(strProvider);	TrimAll(strWorkCode);
	TrimAll(strDate1);		TrimAll(strDate2);
	TrimAll(m_strSelDivNo);
	
	//입력된 그룹정보 검사
	CString strMsg;
	//m_strSelDivNo = CheckValidateGroupNo(m_pCM, m_strSelDivNo, strAcqYear);
	if(m_strSelDivNo.IsEmpty()) 
	{
		strMsg = _T("차수정보를 설정해 주십시요.");
		ESLAfxMessageBox(strMsg);
		return;
	}
	//그룹컬렉션 구축
	MakeGroupInfo(strAcqYear, m_strSelDivNo, strWorkCode);

	INT ids ;
	// IDX_BO_TBL의 SPECIES_CLASS 0:소장종, 1:작업종, 2:재활용, 3:삭제종
	CString strWhere;
	if(GetOpenMode() == BO_ACQ_WORK_SEARCH || GetOpenMode() == BO_ACQ_HOLD_SEARCH)
		strWhere = _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND I.SPECIES_CLASS IN ('0', '1') AND I.MANAGE_CODE = UDF_MANAGE_CODE");
	else if(GetOpenMode() == BO_PURCHASE_PRINT)
		strWhere = _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND I.SPECIES_CLASS IN ('0', '1') AND I.MANAGE_CODE = UDF_MANAGE_CODE");
	else
		strWhere = _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND I.SPECIES_CLASS IN ('0', '1') AND I.MANAGE_CODE = UDF_MANAGE_CODE");

	CString strWorkingStatus = _T("");
	CString strOption;
	if(GetOpenMode() == BO_ACQ_ORDER) strWorkingStatus = _T("BOA112N");
	else if(GetOpenMode() == BO_ACQ_MISSING) strWorkingStatus = _T("BOA113O");
	else if(GetOpenMode() == BO_ACQ_REGOBJ && strAcqCode == _T("1")) strWorkingStatus = _T("BOA114O");
	else if(GetOpenMode() == BO_PURCHASE_PRINT) strWorkingStatus = _T("");
	else strWorkingStatus = _T("BOA111N");
	
	CString errstr, result;
	
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	TrimAll(strAcqYear);
	ids = pMakeSearchData->IsValidYearData(strAcqYear);
	if( ids < 0)
	{
		errstr.Format(_T("입력하신 수입년도 값(%s)이 올바르지 않습니다."), strAcqYear);
		ESLAfxMessageBox(errstr.GetBuffer(0));
		m_pCM->SetControlMgrData(_T("수입년도"), GetTodayYear());		
		return;
	}
	if(strDate1.IsEmpty() && !strDate2.IsEmpty())
	{
		this->ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
		return;
	}	
//책관련쿼리
	CString strAccuWhereStmt;
	if(strWorkingStatus.IsEmpty()) 
		strOption = _T("");
	else if (strWorkingStatus == _T("BOA111N"))
		strOption.Format(_T(" AND I.REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE WORKING_STATUS='%s' AND ACQ_CODE='1' AND ACQ_YEAR='%s')"), strWorkingStatus, strAcqYear);
	else if (strWorkingStatus == _T("BOA112N"))
		strOption.Format(_T(" AND B.WORKING_STATUS IN ('BOA112N','BOA113O') AND B.ACQ_CODE='1' "));
	else
		strOption.Format(_T(" AND B.WORKING_STATUS='%s' AND B.ACQ_CODE='1' "), strWorkingStatus);
	strAccuWhereStmt += strOption;
	
//수서정보관리 쿼리
	if(!strAcqYear.IsEmpty())
	{
		strOption.Format(_T(" AND P.ORDER_YEAR = '%s'"), strAcqYear);
		strAccuWhereStmt += strOption;
	}
	if(strWorkCode.IsEmpty())
	{
		strOption.Format(_T(" AND I.WORK_CODE IS NULL"));
		strAccuWhereStmt += strOption;
	}
	else
	{
		strOption.Format(_T(" AND I.WORK_CODE = '%s'"), strWorkCode);
		strAccuWhereStmt += strOption;
	}
	if(GetOpenMode() == BO_ACQ_ORDER || GetOpenMode() == BO_ACQ_MISSING)
	{
		if(!strProvider.IsEmpty())
		{
			strOption.Format(_T(" AND P.PROVIDER_CODE = '%s'"), strProvider);
			strAccuWhereStmt += strOption;
		}
		if(!m_strSelDivNo.IsEmpty())
		{
			strOption.Format(_T(" AND P.ORDER_SEQ_NO IN (%s)"), m_strSelDivNo);
			strAccuWhereStmt += strOption;
		}
		strOption = MakeDateCondition_Stmt(_T("P.ORDER_DATE"), strDate1, strDate2);
		if(strOption == _T("-1")) return ;
		strAccuWhereStmt += strOption;
	}
	else if(GetOpenMode() == BO_ACQ_REGOBJ && strAcqCode == _T("1"))
	{
		if(!strProvider.IsEmpty())
		{
			strOption.Format(_T(" AND P.PROVIDER_CODE = '%s'"), strProvider);
			strAccuWhereStmt += strOption;
		}
		if(!m_strSelDivNo.IsEmpty())
		{
			strOption = _T(" AND (") + MakeDivnoOrWorkCodeQuery() + _T(")");
			strAccuWhereStmt += strOption;
		}
		strOption = MakeDateCondition_Stmt(_T("P.CHECKIN_DATE"), strDate1, strDate2);
		if(strOption == _T("-1")) return ;
		strAccuWhereStmt += strOption;
	}
	else if(GetOpenMode() == BO_ACQ_WORK_SEARCH || GetOpenMode() == BO_ACQ_HOLD_SEARCH)
	{
		if(!strProvider.IsEmpty())
		{
			strOption.Format(_T(" AND P.PROVIDER_CODE = '%s'"), strProvider);
			strAccuWhereStmt += strOption;
		}
		if(!m_strSelDivNo.IsEmpty())
		{
			strOption = _T(" AND (") + MakeDivnoOrWorkCodeQuery() + _T(")");
			strAccuWhereStmt += strOption;
		}
		strOption = MakeDateCondition_Stmt(_T("P.INPUT_DATE"), strDate1, strDate2);
		if(strOption == _T("-1")) return ;
		strAccuWhereStmt += strOption;
	}
	else if(GetOpenMode() == BO_PURCHASE_PRINT)
	{
		if(!strProvider.IsEmpty())
		{
			strOption.Format(_T(" AND P.PROVIDER_CODE = '%s'"), strProvider);
			strAccuWhereStmt += strOption;
		}
		if(!m_strSelDivNo.IsEmpty())
		{
			strOption = _T(" AND (") + MakeDivnoOrWorkCodeQuery() + _T(")");
			strAccuWhereStmt += strOption;
		}
		strOption = MakeDateCondition_Stmt(_T("P.INPUT_DATE"), strDate1, strDate2);
		if(strOption == _T("-1")) return ;
		strAccuWhereStmt += strOption;
	}
	
	//쿼리통합
	strWhere += strAccuWhereStmt;
	SetWhereStmt(strWhere);

	// 쿼리 생성후 부모에게 메시지를 보낸다.
	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

BOOL CBO_ACQ_DivisionNoSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		GetDlgItem(IDOK)->SetFocus();
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	else if (pMsg->message == WM_CHAR && pMsg->hwnd == GetDlgItem(IDC_edtDIVSEARCH_NO)->GetSafeHwnd())
    {
		INT nVirtKey = (INT) pMsg->wParam;
		if ((nVirtKey < '0' || nVirtKey > '9') && nVirtKey != ',' && nVirtKey != ' ' && nVirtKey != 8)
		{
			ESLAfxMessageBox(_T("숫자만 입력하실 수 있습니다.")); 
            return TRUE;
		}
    }

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CBO_ACQ_DivisionNoSearchDlg::Init()
{
	//수입구분 리스트 데이터 설정
	CString acqCode[5][2] = 
	{
		_T("1"), _T("구입"),
		_T("2"), _T("기증"),
	};
	CMulitComboBoxManager mgr(this);
	CString strAcqCode;
	
	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("구입"), acqCode, _T("수입구분"), 2);
	strAcqCode = _T("1");
	m_pCM->SetControlMgrData(_T("수입구분"), strAcqCode);
	CESL_ControlMultiComboBox * pElement = (CESL_ControlMultiComboBox*)m_pCM->FindControl(_T("수입구분"));
	pElement->EnableWindow(FALSE);

	ShowCtrlByAcqCode(strAcqCode);
//	ShowCtrlByOpenMode();
	return TRUE;
}

VOID CBO_ACQ_DivisionNoSearchDlg::OnSelchangecmbDIVSEARCHACQCODE() 
{
	// TODO: Add your control notification handler code here
	CString strAcqCode;
	m_pCM->GetControlMgrData(_T("수입구분"), strAcqCode);
	ShowCtrlByAcqCode(strAcqCode);
	SetParentInfo(strAcqCode);
}

VOID CBO_ACQ_DivisionNoSearchDlg::ShowCtrlByAcqCode(CString strAcqCode)
{
	CWnd *pWnd = NULL;
	INT nPurchaseCount = 0;
	INT nDonateCount = 0;
	INT nPurchaseArrCodeResourceID[10];
	INT nDonateArrCodeResourceID[10];

	nPurchaseArrCodeResourceID[nPurchaseCount++] = IDC_stcDIVSEARCH_PROVIDER;
	nPurchaseArrCodeResourceID[nPurchaseCount++] = IDC_cmbDIVSEARCH_PROVIDER;
	nPurchaseArrCodeResourceID[nPurchaseCount++] = IDC_stcDIVSEARCH_ORDER_DATE;
	nPurchaseArrCodeResourceID[nPurchaseCount++] = IDC_edtDIVSEARCH_DATE1;
	nPurchaseArrCodeResourceID[nPurchaseCount++] = IDC_edtDIVSEARCH_DATE2;
	nPurchaseArrCodeResourceID[nPurchaseCount++] = IDC_stcDIVSEARCH_RANGE;

	nDonateArrCodeResourceID[nDonateCount++] = IDC_stcDIVSEARCH_DONATOR;
	nDonateArrCodeResourceID[nDonateCount++] = IDC_edtDIVSEARCH_DONATOR;
	nDonateArrCodeResourceID[nDonateCount++] = IDC_btnDIVSEARCH_DONATOR_SEARCH;
	if(strAcqCode == _T("1"))
	{
		for(INT i=0 ; i<nDonateCount ; i++)
		{
			pWnd = GetDlgItem(nDonateArrCodeResourceID[i]);
			if (pWnd->GetSafeHwnd() != NULL)
				pWnd->ShowWindow(SW_HIDE);
		}
		GetDlgItem(IDC_stcDIVSEARCH_DIVNO)->SetWindowText(_T("차수번호"));
		GetDlgItem(IDC_btnDIVSEARCH_VIEW)->SetWindowText(_T("차수보기"));
		if(GetOpenMode() == BO_ACQ_ORDER || GetOpenMode() == BO_ACQ_MISSING) 
			GetDlgItem(IDC_stcDIVSEARCH_ORDER_DATE)->SetWindowText(_T("주 문 일"));
		else if(GetOpenMode() == BO_ACQ_REGOBJ)
			GetDlgItem(IDC_stcDIVSEARCH_ORDER_DATE)->SetWindowText(_T("검 수 일"));
		else
			GetDlgItem(IDC_stcDIVSEARCH_ORDER_DATE)->SetWindowText(_T("입 력 일"));
	}
	else if(strAcqCode == _T("2"))
	{
		for(INT i=0 ; i<nPurchaseCount ; i++)
		{
			pWnd = GetDlgItem(nPurchaseArrCodeResourceID[i]);
			if (pWnd->GetSafeHwnd() != NULL)
				pWnd->ShowWindow(SW_HIDE);
		}
		GetDlgItem(IDC_stcDIVSEARCH_DIVNO)->SetWindowText(_T("접수번호"));
		GetDlgItem(IDC_btnDIVSEARCH_VIEW)->SetWindowText(_T("접수번호"));
	}	
}

VOID CBO_ACQ_DivisionNoSearchDlg::OnbtnDIVSEARCHVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strWorkCode;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	INT acqCode = GetAcqCode();

	CDivisionNo_List_Dlg dlg(this);
	if(GetOpenMode() == BO_ACQ_ORDER)
		dlg.SetWorkStatus(_T("주문"));
	else if(GetOpenMode() == BO_ACQ_REGOBJ)
		dlg.SetWorkStatus(_T("검수완료"));
	else if(GetOpenMode() == BO_ACQ_MISSING)
		dlg.SetWorkStatus(_T("검수완료"));
	else if(GetOpenMode() == BO_PURCHASE_PRINT)
		dlg.SetWorkStatus(_T("전체자료"));
	else
		dlg.SetWorkStatus(_T("구입대상"));
	dlg.SetSpeciesType(GetSpeciesType());
	dlg.SetAcquisitYear(strAcqYear);
	dlg.SetWorkCode(strWorkCode);
	dlg.SetGroupCollection(m_pGroups);
	if(IDOK == dlg.DoModal())
	{
		m_strSelDivNo = dlg.GetSelDivNo();
		m_pCM->SetControlMgrData(_T("그룹번호"), m_strSelDivNo);
		m_pCM->SetControlMgrData(_T("수입년도"), m_pGroups->GetAcqYear());
		m_pCM->SetControlMgrData(_T("업무구분"), m_pGroups->GetWorkCode());
		OnOK();
	}
}

CString CBO_ACQ_DivisionNoSearchDlg::MakeDivNoNameOrQuery(CString strGroupName)
{
	CString strQuery, strTmpData, strFieldName;
	if(GetAcqCode() == PURCHASE) strFieldName = _T("P.ORDER_SEQ_NO");
	else if(GetAcqCode() == DONATE) strFieldName = _T("P.RECEIPT_NO");
	CStringArray arrGroupName;		arrGroupName.RemoveAll();
	INT ids = ParsingString(_T(","), strGroupName, &arrGroupName);
	INT cnt = arrGroupName.GetSize();

	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData.Format(_T("%s = %s"), strFieldName, arrGroupName.GetAt(i));
		if(strQuery.IsEmpty()) strQuery = strTmpData;
		else strQuery += _T(" OR ") + strTmpData;
	}
	return strQuery;
}

VOID CBO_ACQ_DivisionNoSearchDlg::OnKillfocusedtDIVSEARCHACQYEAR() 
{
	//============================================
	//20080603 ADD BY PJW : 수입년을 숫자 4자리를 넣지않았을 경우 4자리 숫자를 맞춰준다 
	//CString strPubYear;
	//INT nYear;
	//CEdit * pPubYear = (CEdit*)GetDlgItem(IDC_edtDIVSEARCH_ACQYEAR);
	//pPubYear->GetWindowText(strPubYear);
	//if( strPubYear.IsEmpty() )
	//{
	//	return;
	//}
	//else
	//{
	//	nYear = _ttoi(strPubYear);
	//}
	//if( nYear < 1000 || nYear > 9999 )
	//{

	//	ESLAfxMessageBox(_T("1000 ~ 9999 사이의 값을 입력해주세요"));
	//	pPubYear->SetWindowText(_T(""));
	//}
	//============================================
	OnKillfocusedtDIVSEARCHNO();	
}

VOID CBO_ACQ_DivisionNoSearchDlg::OnKillfocusedtDIVSEARCHNO() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strWorkCode;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("그룹번호"), m_strSelDivNo);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	m_strSelDivNo = CheckValidateGroupNo(m_pCM, m_strSelDivNo, strAcqYear, strWorkCode);
	
	m_pCM->SetControlMgrData(_T("그룹번호"), m_strSelDivNo);
}

VOID CBO_ACQ_DivisionNoSearchDlg::ShowCtrlByOpenMode()
{
	if(GetOpenMode() != BO_ACQ_WORK_SEARCH && GetOpenMode() != BO_ACQ_HOLD_SEARCH)
		return ;

	INT i = 0;
	INT nCodeResourcesHeight = 65;
	CWnd *pWnd = NULL;

	CRect rcWindow;
	GetWindowRect(rcWindow);
	
	INT nCount = 0;
	INT nArrCodeResourceID[256];
	nArrCodeResourceID[nCount++] = IDC_stcACQINFO;
	nArrCodeResourceID[nCount++] = IDC_stcDIVSEARCH_PROVIDER;
	nArrCodeResourceID[nCount++] = IDC_stcDIVSEARCH_ORDER_DATE;
	nArrCodeResourceID[nCount++] = IDC_cmbDIVSEARCH_PROVIDER;
	nArrCodeResourceID[nCount++] = IDC_edtDIVSEARCH_DATE1;
	nArrCodeResourceID[nCount++] = IDC_stcDIVSEARCH_RANGE;
	nArrCodeResourceID[nCount++] = IDC_edtDIVSEARCH_DATE2;
	
	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->ShowWindow(SW_HIDE);
		}
	}

	nCount = 0;
//	nArrCodeResourceID[nCount++] = IDC_stcBTNGROUP;
	nArrCodeResourceID[nCount++] = IDOK;
	nArrCodeResourceID[nCount++] = IDCANCEL;
	CRect rcResource;
	for (i = 0; i <nCount ; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->GetWindowRect(rcResource);
			ScreenToClient(rcResource);
			rcResource.top -= nCodeResourcesHeight;
			rcResource.bottom -= nCodeResourcesHeight;
			pWnd->MoveWindow(rcResource);
		}
	}
	
	SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	
	Invalidate();
	UpdateWindow();	
	
/*
	nCount = 0;
	nArrCodeResourceID[nCount++] = IDC_stcSORT_CONDITION_GROUP;
	nArrCodeResourceID[nCount++] = IDC_chkSORT_YN;
	nArrCodeResourceID[nCount++] = IDC_btnSET_SORT_CONDITION;
	for(i=0 ; i<nCount ; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->GetWindowRect(&rcWindow);
			ScreenToClient(rcWindow);
			
			if (!m_bShowMode)
			{
				rcWindow.top -= nCodeResourcesHeight;
				rcWindow.bottom -= nCodeResourcesHeight;
				pWnd->MoveWindow(rcWindow);
			}
		}		
	}
*/
}

VOID CBO_ACQ_DivisionNoSearchDlg::ShowGroupInfo()
{
	CString strAcqYear, strWorkCode;
	strAcqYear = m_pGroups->GetAcqYear();
	if(strAcqYear.IsEmpty()) strAcqYear = GetTodayYear();
	m_pGroups->GetGroupInfo(m_strSelDivNo);
	if(m_strSelDivNo == _T("전체자료")) m_strSelDivNo = _T("");
	m_pCM->SetControlMgrData(_T("그룹번호"), m_strSelDivNo);
	m_pCM->SetControlMgrData(_T("수입년도"), strAcqYear);
	if(m_pGroups->GetGroupCount() == 0)
		m_pInfo->pCodeMgr->GetDefaultCode(_T("업무구분"), strWorkCode);
	else 
		strWorkCode = m_pGroups->GetWorkCode();
	m_pCM->SetControlMgrData(_T("업무구분_코드"), strWorkCode);

	GetDlgItem(IDC_edtDIVSEARCH_NO)->SetFocus();
}

VOID CBO_ACQ_DivisionNoSearchDlg::OnSelchangecmbSEARCHDIVNOWORKCODE() 
{
	// TODO: Add your control notification handler code here
	OnKillfocusedtDIVSEARCHNO();	
}

VOID CBO_ACQ_DivisionNoSearchDlg::OnbtnDIVSEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	ClearScreen(m_pCM);
	m_pGroups->RemoveAll();
	ShowGroupInfo();
}

VOID CBO_ACQ_DivisionNoSearchDlg::OnbtnSETSORTCONDITION() 
{
	// TODO: Add your control notification handler code here
	ShowDataSortDlg();
}

VOID CBO_ACQ_DivisionNoSearchDlg::OnchkSORTYN() 
{
	// TODO: Add your control notification handler code here
	CButton * pBtn = (CButton *)GetDlgItem(IDC_chkSORT_YN);
	if(pBtn && pBtn->GetCheck()) m_bDataSortYN = TRUE;
	else m_bDataSortYN = FALSE;	
}

HBRUSH CBO_ACQ_DivisionNoSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
