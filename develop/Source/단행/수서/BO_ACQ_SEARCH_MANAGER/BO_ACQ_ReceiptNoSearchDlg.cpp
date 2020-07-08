// BO_ACQ_ReceiptNoSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/RequisitionNo_List_Dlg.h"
#include "BO_ACQ_ReceiptNoSearchDlg.h"
#include "../../../공통/COMMON_DONATOR_MANAGER/DonatorNameGetter_Search.h"
#include "../../../공통/COMMON_DONATOR_MANAGER/DonatorNameGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_ReceiptNoSearchDlg dialog


CBO_ACQ_ReceiptNoSearchDlg::CBO_ACQ_ReceiptNoSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBO_SearchDlg(CBO_ACQ_ReceiptNoSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_ReceiptNoSearchDlg)
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_strDonatorKey = _T("");
}
CBO_ACQ_ReceiptNoSearchDlg::~CBO_ACQ_ReceiptNoSearchDlg()
{
}

VOID CBO_ACQ_ReceiptNoSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_ReceiptNoSearchDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_ReceiptNoSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_ReceiptNoSearchDlg)
	ON_EN_KILLFOCUS(IDC_edtDIVSEARCH_ACQYEAR, OnKillfocusedtDIVSEARCHACQYEAR)
	ON_EN_KILLFOCUS(IDC_edtDIVSEARCH_NO, OnKillfocusedtDIVSEARCHNO)
	ON_BN_CLICKED(IDC_btnDIVSEARCH_VIEW, OnbtnDIVSEARCHVIEW)
	ON_BN_CLICKED(IDC_btnDIVSEARCH_DONATOR_SEARCH, OnbtnDIVSEARCHDONATORSEARCH)
	ON_BN_CLICKED(IDC_btnDIVSEARCH_CLEAR, OnbtnDIVSEARCHCLEAR)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_BN_CLICKED(IDC_chkSORT_YN, OnchkSORTYN)
	ON_EN_SETFOCUS(IDC_edtDIVSEARCH_DONATOR, OnSetfocusedtDIVSEARCHDONATOR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_ReceiptNoSearchDlg message handlers

BOOL CBO_ACQ_ReceiptNoSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_접수번호검색")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM설정
	m_pCM = FindCM(_T("CM_접수번호검색"));	
	

	GetWindowRect(m_rcWindow);
	if(!Init())
	{
		ESLAfxMessageBox(_T("검색화면을 초기화할 수 없습니다."));
		return FALSE;
	}	

	//============================================
	//20080603 ADD BY PJW : 수입년을 숫자 4자리 이상 넣지않도록 수정
//	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtDIVSEARCH_ACQYEAR);
//	pAcqYear->SetLimitText(4);
	//============================================

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_ACQ_ReceiptNoSearchDlg::Init()
{
	CString acqCode[5][2] = 
	{
		_T("1"), _T("구입"),
		_T("2"), _T("기증"),
	};
	CMulitComboBoxManager mgr(this);
	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("기증"), acqCode, _T("수입구분"), 2);
	CString strAcqCode = _T("2");
	m_pCM->SetControlMgrData(_T("수입구분"), strAcqCode);
	CESL_ControlMultiComboBox * pElement = (CESL_ControlMultiComboBox*)m_pCM->FindControl(_T("수입구분"));
	pElement->EnableWindow(FALSE);
	return TRUE;
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnKillfocusedtDIVSEARCHACQYEAR() 
{
	//============================================
	//20080603 ADD BY PJW : 수입년을 숫자 4자리를 넣지않았을 경우 4자리 숫자를 맞춰준다 
//	CString strAcqYear;
//	INT nYear;
//	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtDIVSEARCH_ACQYEAR);
//	pAcqYear->GetWindowText(strAcqYear);
//	if( strAcqYear.IsEmpty() )
//	{
//		return;
//	}
//	else
//	{
//		nYear = _ttoi(strAcqYear);
//	}
//	if( nYear < 1000 || nYear > 9999 )
//	{
//		ESLAfxMessageBox(_T("1000 ~ 9999 사이의 값을 입력해주세요"));
//		pAcqYear->SetWindowText(_T(""));
//	}
	//============================================	

	OnKillfocusedtDIVSEARCHNO();
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnKillfocusedtDIVSEARCHNO() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("그룹번호"), m_strSelDivNo);
	
	m_strSelDivNo = CheckValidateGroupNo(m_pCM, m_strSelDivNo, strAcqYear);
	
	m_pCM->SetControlMgrData(_T("그룹번호"), m_strSelDivNo);
}

BOOL CBO_ACQ_ReceiptNoSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
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

VOID CBO_ACQ_ReceiptNoSearchDlg::OnbtnDIVSEARCHVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear = m_pGroups->GetAcqYear();
	if(strAcqYear.IsEmpty())
		m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);

	CRequisitionNo_List_Dlg dlg(this);
	dlg.SetSpeciesType(GetSpeciesType());
	dlg.SetAcqYear(strAcqYear);
	dlg.SetGroupCollection(m_pGroups);
	if(IDOK == dlg.DoModal())
	{
		m_strSelDivNo = dlg.GetSelInfo();
		m_pCM->SetControlMgrData(_T("그룹번호"), m_strSelDivNo);
		m_pCM->SetControlMgrData(_T("수입년도"), m_pGroups->GetAcqYear());
		OnOK();
	}	
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	// CM에서 사용자가 선정한 검색조건으로 검색 쿼리를 생성한다.
	CString strAcqYear, strAcqCode, strDonator, strDate1, strDate2;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("수입구분"), strAcqCode);
	m_pCM->GetControlMgrData(_T("기증자"), strDonator);
	m_pCM->GetControlMgrData(_T("그룹번호"), m_strSelDivNo);
	m_pCM->GetControlMgrData(_T("등록일1"), strDate1); 
	m_pCM->GetControlMgrData(_T("등록일2"), strDate2); 
	
	TrimAll(strAcqYear);	TrimAll(strAcqCode);
	TrimAll(strDonator);
	TrimAll(m_strSelDivNo);
	
	//입력된 그룹정보 검사
	CString strMsg;
	//그룹컬렉션 구축
	if(!m_strSelDivNo.IsEmpty())
		MakeGroupInfo(strAcqYear, m_strSelDivNo);

	INT ids ;
	// IDX_BO_TBL의 SPECIES_CLASS 0:소장종, 1:작업종, 2:재활용, 3:삭제종
	/*********************************************************************************
	 *********************************************************************************/
	//20080218 UPDATE BY PJW : 접수번호보기에서 나온 책수와 검색책수를 맞춘다.
	//                         P.SPECIES_KEY -> B.SPECIES_KEY 로 변경
	//CString strWhere = _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND P.DONATOR_KEY = C.REC_KEY(+) AND I.SPECIES_CLASS IN('0', '1')");
	CString strWhere = _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = B.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND P.DONATOR_KEY = C.REC_KEY(+) AND I.SPECIES_CLASS IN('0', '1') AND I.MANAGE_CODE = UDF_MANAGE_CODE");
	/*********************************************************************************
	 *********************************************************************************/
	
	CString strWorkingStatus = _T("BOA211O");
	CString strOption;
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
// 등록일(입력일)로부터 쿼리 생성  ========================================= 수정 10.10
	// 2004.11.30 C.INPUT_DATE -> P.INPUT_DATE
	// strOption = MakeDateCondition_Stmt(_T("C.INPUT_DATE"), strDate1, strDate2);
	strOption = MakeDateCondition_Stmt(_T("P.INPUT_DATE"), strDate1, strDate2);
	if(strOption == _T("-1")) return ;
	strWhere += strOption;

	CString strAccuWhereStmt;
	if(GetOpenMode() != BO_DONATE_PRINT)
	{
		strOption.Format(_T(" AND B.WORKING_STATUS = '%s' AND B.ACQ_CODE='2'"), strWorkingStatus);
		strAccuWhereStmt += strOption;
	}
//수서정보관리 쿼리
	if(!strAcqYear.IsEmpty())
	{
		strOption.Format(_T(" AND P.ACQ_YEAR = '%s'"), strAcqYear);
		strAccuWhereStmt += strOption;
	}
	
	if(!strDonator.IsEmpty())
	{
		//strOption.Format(_T(" AND P.DONATOR_NAME LIKE '%s%%'"), strDonator);
		//strAccuWhereStmt += strOption;
		CESL_DataMgr dm;
		CString strDonatorCnt, sQuery;
		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		sQuery.Format(_T("SELECT COUNT(REC_KEY) FROM CO_DONATOR_TBL WHERE DONATOR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),strDonator);
		dm.GetOneValueQuery(sQuery,strDonatorCnt);
		INT nCnt = _ttoi(strDonatorCnt);
		
		if(m_strDonatorKey.IsEmpty() && nCnt > 1 )
		{
			AfxMessageBox(_T("중복되었거나 없는 기증자입니다. \n\r 검색을 통해 선정해주세요!"));
			return;
		}

		if(m_strDonatorKey.IsEmpty())
		{
			strOption.Format(_T(" AND P.DONATOR_NAME LIKE '%s%%' "), strDonator);
			strAccuWhereStmt += strOption;
		}
		else
		{
			strOption.Format(_T(" AND P.DONATOR_NAME LIKE '%s%%' AND P.DONATOR_KEY = '%s' "), strDonator, m_strDonatorKey);
			strAccuWhereStmt += strOption;
		}
	}	
	if(!m_strSelDivNo.IsEmpty())
	{
		strOption = _T(" AND (") + this->MakeGroupInfoOrQuery(m_strSelDivNo) + _T(")");
		strAccuWhereStmt += strOption;
	}
	
	//쿼리통합
	strWhere += strAccuWhereStmt;
	SetWhereStmt(strWhere);

	// 쿼리 생성후 부모에게 메시지를 보낸다.
	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);	
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnbtnDIVSEARCHDONATORSEARCH() 
{
	// TODO: Add your control notification handler code here
	CDonatorNameGetter_Search searchDlg(this);
	searchDlg.SetOpenMode(-1000);
	if(IDOK == searchDlg.DoModal())
	{
		CDonatorNameGetterDlg searchResult(this);
		if(IDOK == searchResult.DoModal())
		{
			m_pCM->SetControlMgrData(_T("기증자"), searchResult.GetDonatorName());
			m_strDonatorKey = searchResult.GetDonatorKey();
		}
	}	
}

VOID CBO_ACQ_ReceiptNoSearchDlg::ShowGroupInfo()
{
	CString strAcqYear;
	strAcqYear = m_pGroups->GetAcqYear();
	if(strAcqYear.IsEmpty()) strAcqYear = GetTodayYear();
	m_pGroups->GetGroupInfo(m_strSelDivNo);
	if(m_strSelDivNo == _T("전체자료")) m_strSelDivNo = _T("");
	m_pCM->SetControlMgrData(_T("그룹번호"), m_strSelDivNo);
	m_pCM->SetControlMgrData(_T("수입년도"), strAcqYear);
	GetDlgItem(IDC_edtDIVSEARCH_NO)->SetFocus();
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnbtnDIVSEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	ClearScreen(m_pCM);
	//20071101 ADD PJW : 화면지우기 번튼누르면 화면 초기화 추가 Init();
	Init();
	m_pGroups->RemoveAll();
	ShowGroupInfo();
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnbtnSETSORTCONDITION() 
{
	// TODO: Add your control notification handler code here
	ShowDataSortDlg();
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnchkSORTYN() 
{
	// TODO: Add your control notification handler code here
	CButton * pBtn = (CButton *)GetDlgItem(IDC_chkSORT_YN);
	if(pBtn && pBtn->GetCheck()) m_bDataSortYN = TRUE;
	else m_bDataSortYN = FALSE;	
}

VOID CBO_ACQ_ReceiptNoSearchDlg::OnSetfocusedtDIVSEARCHDONATOR() 
{
	SetIME(IDC_edtDIVSEARCH_DONATOR, 1);
}

HBRUSH CBO_ACQ_ReceiptNoSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}