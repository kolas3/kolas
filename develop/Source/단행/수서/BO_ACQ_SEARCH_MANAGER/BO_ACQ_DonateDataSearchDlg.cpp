// BO_ACQ_DonateDataSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_DonateDataSearchDlg.h"
#include "../../../공통/COMMON_DONATOR_MANAGER/DonatorNameGetter_Search.h"
#include "../../../공통/COMMON_DONATOR_MANAGER/DonatorNameGetterDlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/RequisitionNo_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_DonateDataSearchDlg dialog


CBO_ACQ_DonateDataSearchDlg::CBO_ACQ_DonateDataSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBO_SearchDlg(CBO_ACQ_DonateDataSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_DonateDataSearchDlg)
	//}}AFX_DATA_INIT
	m_bShowMode = FALSE;
	m_pCM = NULL;
	m_strSelDivNo = _T("");
	m_strDonatorKey = _T("");

	// openbase flag	
	CFileFind ff;
	if ( ff.FindFile(_T("..\\cfg\\_OPENBASE")) )
	{
		m_bOpenbase = TRUE;
	}
	else
	{
		m_bOpenbase = FALSE;
	}

}
CBO_ACQ_DonateDataSearchDlg::~CBO_ACQ_DonateDataSearchDlg()
{
}

VOID CBO_ACQ_DonateDataSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_DonateDataSearchDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_DonateDataSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_DonateDataSearchDlg)
	ON_BN_CLICKED(IDC_btnFIND_DONATOR, OnbtnFINDDONATOR)
	ON_BN_CLICKED(IDC_btnSEARCH_DIVISIONNO_VIEW, OnbtnSEARCHDIVISIONNOVIEW)
	ON_EN_KILLFOCUS(IDC_edtSEARCH_ACQ_YEAR, OnKillfocusedtSEARCHACQYEAR)
	ON_EN_KILLFOCUS(IDC_edtSEARCH_DIVISIONNO, OnKillfocusedtSEARCHDIVISIONNO)
	ON_CBN_SELCHANGE(IDC_cmbSEARCH_CLASS_NO_TYPE, OnSelchangecmbSEARCHCLASSNOTYPE)
	ON_BN_CLICKED(IDC_btnSEARCH_SHOWHIDE, OnbtnSEARCHSHOWHIDE)
	ON_BN_CLICKED(IDC_btnSEARCH_CLEAR, OnbtnSEARCHCLEAR)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_BN_CLICKED(IDC_chkSORT_YN, OnchkSORTYN)
	ON_EN_SETFOCUS(IDC_edtSEARCH_AUTHOR, OnSetfocusedtSEARCHAUTHOR)
	ON_EN_SETFOCUS(IDC_edtSEARCH_DONATOR, OnSetfocusedtSEARCHDONATOR)
	ON_EN_SETFOCUS(IDC_edtSEARCH_PUBLISHER, OnSetfocusedtSEARCHPUBLISHER)
	ON_EN_SETFOCUS(IDC_edtSEARCH_TITLE, OnSetfocusedtSEARCHTITLE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_DonateDataSearchDlg message handlers

BOOL CBO_ACQ_DonateDataSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_수서기증자료검색")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM설정
	m_pCM = FindCM(_T("CM_수서기증자료검색"));		
	if(!Init())
	{
		ESLAfxMessageBox(_T("검색화면을 초기화할 수 없습니다."));
		return FALSE;
	}
	
	GetWindowRect(m_rcWindow);
	ShowControlByShowMode();
	//============================================
	//20080603 ADD BY PJW : 수입년을 숫자 4자리 이상 넣지않도록 수정
	//CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtSEARCH_ACQ_YEAR);
	//pAcqYear->SetLimitText(4);
	//============================================
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_DonateDataSearchDlg::OnbtnFINDDONATOR() 
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

BOOL CBO_ACQ_DonateDataSearchDlg::Init()
{
	CString inputType[5][2] = 
	{
		_T(" "), _T("적용안함"),
		_T("0"), _T("화면입력"),
		_T("1"), _T("재활용반입"),
		_T("2"), _T("희망자료반입"),
		_T("3"), _T("파일반입")
	};
	CMulitComboBoxManager mgr(this);
	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("적용안함"), inputType, _T("입력구분_코드"), 5);

	//주제구분 설정
	//=====================================================
	//2008.07.16 UPDATE BY PJW : 주제구분코드를 디폴트값으로 가지도록 수정
// 	SetSubjectCodeData(m_pCM->CM_ALIAS, _T("분류표구분_코드"), _T("주제구분_코드"));
	CString strSubCode;
	m_pInfo->pCodeMgr->GetDefaultCode(_T("KDC대주제부호관리"), strSubCode);
	if( strSubCode.IsEmpty() )
	{
		strSubCode.Format(_T("  "));
	}
	mgr.SetSubjectCodeData(m_pCM->CM_ALIAS, _T("분류표구분_코드"), _T("주제구분_코드"), strSubCode);
	//=====================================================
	return TRUE;
}

VOID CBO_ACQ_DonateDataSearchDlg::OnbtnSEARCHDIVISIONNOVIEW() 
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
		OnKillfocusedtSEARCHDIVISIONNO();
		OnOK();
	}	
}

VOID CBO_ACQ_DonateDataSearchDlg::OnKillfocusedtSEARCHACQYEAR() 
{
	// TODO: Add your control notification handler code here
	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtSEARCH_ACQ_YEAR);
	//============================================
	//20080603 ADD BY PJW : 수입년을 숫자 4자리를 넣지않았을 경우 4자리 숫자를 맞춰준다 
//	CString strAcqbYear;
//	INT nYear;
//	pAcqYear->GetWindowText(strAcqbYear);
//	if( strAcqbYear.IsEmpty() )
//	{
//		return;
//	}
//	else
//	{
//		nYear = _ttoi(strAcqbYear);
//	}
//	if( nYear < 1000 || nYear > 9999 )
//	{
//  	ESLAfxMessageBox(_T("1000 ~ 9999 사이의 값을 입력해주세요"));
//		pAcqYear->SetWindowText(_T(""));
//	}
	//============================================
	if (pAcqYear->GetStyle() & ES_READONLY) return;	
	OnKillfocusedtSEARCHDIVISIONNO();	
}

VOID CBO_ACQ_DonateDataSearchDlg::OnKillfocusedtSEARCHDIVISIONNO() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("그룹번호"), m_strSelDivNo);
	m_strSelDivNo = CheckValidateGroupNo(m_pCM, m_strSelDivNo, strAcqYear);
	m_pCM->SetControlMgrData(_T("그룹번호"), m_strSelDivNo);
}

VOID CBO_ACQ_DonateDataSearchDlg::OnSelchangecmbSEARCHCLASSNOTYPE() 
{
	// TODO: Add your control notification handler code here
	
	//================================================================
	//2008.07.16 UPDATE BY PJW : 분류표구분에 따라서 주제구분이 관리에 설정되 디폴트값을 가지도록 수정
// 	SetSubjectCodeData(m_pCM->CM_ALIAS, _T("분류표구분_코드"), _T("주제구분_코드"));
	CString strSubCode;
	CMulitComboBoxManager mgr(this);
	m_pInfo->pCodeMgr->GetDefaultCode(_T("KDC대주제부호관리"), strSubCode);
	strSubCode.TrimLeft();	strSubCode.TrimRight();
	if( strSubCode.IsEmpty() )
	{
		strSubCode.Format(_T("  "));
	}

	mgr.SetSubjectCodeData(m_pCM->CM_ALIAS, _T("분류표구분_코드"), _T("주제구분_코드"), strSubCode);
	//===============================================================
}

BOOL CBO_ACQ_DonateDataSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		GetDlgItem(IDOK)->SetFocus();
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	else if (pMsg->message == WM_CHAR && pMsg->hwnd == GetDlgItem(IDC_edtSEARCH_DIVISIONNO)->GetSafeHwnd())
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

VOID CBO_ACQ_DonateDataSearchDlg::OnbtnSEARCHSHOWHIDE() 
{
	// TODO: Add your control notification handler code here
	if (m_bShowMode)
	{
		m_bShowMode = FALSE;
	}
	else
	{
		m_bShowMode = TRUE;
	}	
	ShowControlByShowMode();	
}


VOID CBO_ACQ_DonateDataSearchDlg::ShowControlByShowMode()
{
	INT i = 0;
	INT nCodeResourcesHeight = 224;
	CWnd *pWnd = NULL;

	if (m_bShowMode)
	{
		GetDlgItem(IDC_btnSEARCH_SHOWHIDE)->SetWindowText(_T("간략검색"));
	}
	else
	{
		GetDlgItem(IDC_btnSEARCH_SHOWHIDE)->SetWindowText(_T("상세검색"));
	}


	CRect rcWindow;
	GetWindowRect(rcWindow);

	CArray<INT, INT> ArrResourceID;	ArrResourceID.RemoveAll();
	ArrResourceID.Add(IDC_stcGROUP1);
	ArrResourceID.Add(IDC_stcAUTHOR);
	ArrResourceID.Add(IDC_edtSEARCH_AUTHOR);
	ArrResourceID.Add(IDC_stcPUBLISHER);
	ArrResourceID.Add(IDC_edtSEARCH_PUBLISHER);
	ArrResourceID.Add(IDC_stcPUBYEAR);
	ArrResourceID.Add(IDC_edtSEARCH_PUBYEAR);
	ArrResourceID.Add(IDC_stcISBN);
	ArrResourceID.Add(IDC_edtSEARCH_ISBN);
	ArrResourceID.Add(IDC_stcGROUP2);
	ArrResourceID.Add(IDC_radSEARCH_MYDATA);
	ArrResourceID.Add(IDC_radSEARCH_ALL);
	ArrResourceID.Add(IDC_stcSEARCH_DONATOR);
	ArrResourceID.Add(IDC_edtSEARCH_DONATOR);
	ArrResourceID.Add(IDC_btnFIND_DONATOR);
	ArrResourceID.Add(IDC_stcGROUP3);
	ArrResourceID.Add(IDC_stcFORMCODE);
	ArrResourceID.Add(IDC_cmbSEARCH_FORM_CODE);
	ArrResourceID.Add(IDC_stcUSEOBJCODE);
	ArrResourceID.Add(IDC_cmbSEARCH_USER_OBJ_CODE);
	ArrResourceID.Add(IDC_stcINPUTTYPE);
	ArrResourceID.Add(IDC_cmbSEARCH_INPUT_TYPE);
	ArrResourceID.Add(IDC_stcSUBJECTCODE);
	ArrResourceID.Add(IDC_cmbSEARCH_SUBJECT_CODE);
	ArrResourceID.Add(IDC_stcWORK_CODE);
	ArrResourceID.Add(IDC_cmbSEARCH_JOB_CLASS);
	ArrResourceID.Add(IDC_stcCLASSNOTYPE);
	ArrResourceID.Add(IDC_cmbSEARCH_CLASS_NO_TYPE);

	// Show / Hide Code Resource
	for (i = 0; i < ArrResourceID.GetSize() ; i++)
	{
		pWnd = GetDlgItem(ArrResourceID.GetAt(i));
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (m_bShowMode)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	if (m_bShowMode)
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);

	ArrResourceID.RemoveAll();
	ArrResourceID.Add(IDC_stcSORT_CONDITION_GROUP);
	ArrResourceID.Add(IDC_chkSORT_YN);
	ArrResourceID.Add(IDC_btnSET_SORT_CONDITION);
	for(i=0 ; i<ArrResourceID.GetSize() ; i++)
	{
		pWnd = GetDlgItem(ArrResourceID.GetAt(i));
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
			else 
			{
				rcWindow.top += nCodeResourcesHeight;
				rcWindow.bottom += nCodeResourcesHeight;
				pWnd->MoveWindow(rcWindow);
			}
		}		
	}
	
	//================================================================
	//2008.07.16 ADD BY PJW : 분류표구분에 따라서 주제구분이 관리에 설정되 디폴트값을 가지도록 수정
	CString strSubCode;
	if( m_bShowMode == TRUE )	
	{
		CMulitComboBoxManager mgr(this);
		m_pInfo->pCodeMgr->GetDefaultCode(_T("KDC대주제부호관리"), strSubCode);
		if( strSubCode.IsEmpty() )
		{
			strSubCode.Format(_T("  "));
		}
		mgr.SetSubjectCodeData(m_pCM->CM_ALIAS, _T("분류표구분_코드"), _T("주제구분_코드"), strSubCode);//_T("  "));
	}
	//================================================================

	Invalidate();
	UpdateWindow();
}

VOID CBO_ACQ_DonateDataSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString m_Use_Obj_Code = _T("");
	CString m_Form_Code = _T("");
	CString m_ISBN = _T("");
	CString m_Publisher = _T("");
	CString m_Author = _T("");
	CString m_Title = _T("");
	CString m_Date2 = _T("");
	CString m_Date1 = _T("");
	CString m_Acquisit_Year = _T("");
	CString m_Classfy_Code = _T("");
	CString m_Work_Code = _T("");
	CString m_PubYear = _T("");
	CString m_Subject_Code = _T("");
	CString m_Input_Type = _T("");
	CString m_Donator = _T("");
	CString m_SubRegNo = _T("");
		
	INT ids ;
	// IDX_BO_TBL의 SPECIES_CLASS 0:소장종, 1:작업종, 2:재활용, 3:삭제종
	CString strWhere = _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND P.DONATOR_KEY = C.REC_KEY(+) AND I.SPECIES_CLASS IN ('0', '1') AND I.MANAGE_CODE = UDF_MANAGE_CODE");
	CString strWorkingStatus = _T("");
	if(GetOpenMode() == BO_ACQ_DONATE) strWorkingStatus = _T("BOA211O");
	else if(GetOpenMode() == BO_ACQ_NONDONATE) strWorkingStatus = _T("BOA212O");

	CString errstr, result;
	
	m_pCM->GetControlMgrData(_T("수입년도"), m_Acquisit_Year);
	TrimAll(m_Acquisit_Year);
	
	ids = pMakeSearchData->IsValidYearData(m_Acquisit_Year);
	if( ids < 0)
	{
		errstr.Format(_T("입력하신 수입년도 값(%s)이 올바르지 않습니다."), m_Acquisit_Year);
		ESLAfxMessageBox(errstr.GetBuffer(0));
		m_pCM->SetControlMgrData(_T("수입년도"), GetTodayYear());		
		return;
	}
	
	m_pCM->GetControlMgrData(_T("입력일1"), m_Date1);
	m_pCM->GetControlMgrData(_T("입력일2"), m_Date2);
	TrimAll(m_Date1);		TrimAll(m_Date2);
	if(m_Date1.IsEmpty() && !m_Date2.IsEmpty())
	{
		this->ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
		return;
	}
	m_pCM->GetControlMgrData(_T("발행년"), m_PubYear);
	TrimAll(m_PubYear);
	ids = pMakeSearchData->IsValidYearData(m_PubYear);
	if( ids < 0)
	{
		errstr.Format(_T("입력하신 발행년도 값(%s)이 올바르지 않습니다."), m_PubYear);
		ESLAfxMessageBox(errstr.GetBuffer(0));
		m_pCM->SetControlMgrData(_T("발행년"), GetTodayYear());
		return;
	}

	m_pCM->GetControlMgrData(_T("그룹번호"), m_strSelDivNo);
	TrimAll(m_strSelDivNo);
	//입력된 그룹정보 검사
	CString strMsg;
	//m_strSelDivNo = CheckValidateGroupNo(m_pCM, m_strSelDivNo, m_Acquisit_Year);
/*	if(m_strSelDivNo.IsEmpty()) 
	{
		strMsg = _T("접수번호정보를 설정해 주십시요.");
		ESLAfxMessageBox(strMsg);
		return;
	}
*/	// 그룹컬렉션 구축
	if(!m_strSelDivNo.IsEmpty())
		this->MakeGroupInfo(m_Acquisit_Year, m_strSelDivNo);

	m_pCM->GetControlMgrData(_T("서명"), m_Title);
	m_pCM->GetControlMgrData(_T("저자"), m_Author);
	m_pCM->GetControlMgrData(_T("발행자"), m_Publisher);
	m_pCM->GetControlMgrData(_T("ISBN"), m_ISBN);
	m_pCM->GetControlMgrData(_T("형식구분_코드"), m_Form_Code);
	m_pCM->GetControlMgrData(_T("이용대상구분_코드"), m_Use_Obj_Code);
	m_pCM->GetControlMgrData(_T("주제구분_코드"), m_Subject_Code);
	m_pCM->GetControlMgrData(_T("입력구분_코드"), m_Input_Type);
	m_pCM->GetControlMgrData(_T("기증자"), m_Donator);
	// 2005-03-10
	m_pCM->GetControlMgrData(_T("분류표구분_코드"), m_Classfy_Code);
	m_pCM->GetControlMgrData(_T("업무구분_코드"), m_Work_Code);
	m_pCM->GetControlMgrData(_T("보조등록구분_코드"), m_SubRegNo);
	
	TrimAll(m_Title);
	TrimAll(m_Author);
	TrimAll(m_PubYear);
	TrimAll(m_ISBN);
	TrimAll(m_Form_Code);
	TrimAll(m_Use_Obj_Code);
	TrimAll(m_Subject_Code);
	TrimAll(m_Input_Type);
	TrimAll(m_Donator);
	// 2005-03-10
	TrimAll(m_Classfy_Code);
	TrimAll(m_Work_Code);
	TrimAll(m_SubRegNo);

	//본표제, 저작자, 발행자 색인어 생성형식으로 변환
	pMakeSearchData->GetKeyWord(m_Title);		pMakeSearchData->ChangeHanjaToHangul(m_Title);
	pMakeSearchData->GetKeyWord(m_Author);	pMakeSearchData->ChangeHanjaToHangul(m_Author);
	pMakeSearchData->GetKeyWord(m_Publisher);	pMakeSearchData->ChangeHanjaToHangul(m_Publisher);
	pMakeSearchData->GetKeyWord(m_ISBN);

	CString strOption, strAcqCode;

//책관련 쿼리	
	strOption.Format(_T(" AND B.WORKING_STATUS = '%s' AND B.ACQ_CODE = '2'"), strWorkingStatus);
	strWhere += strOption;
	
	strOption = MakeDateCondition_Stmt(_T("B.INPUT_DATE"), m_Date1, m_Date2);
	if(strOption == _T("-1")) return ;
	strWhere += strOption;

//수서정보관리 쿼리
	CString strAccuWhereStmt;
	if(!m_Subject_Code.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND P.SUBJECT_CODE = '%s'"), m_Subject_Code);
		strAccuWhereStmt += strOption;
	}
	if(!m_Classfy_Code.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND I.CLASS_NO_TYPE = '%s'"), m_Classfy_Code);
		strAccuWhereStmt += strOption;
	}
	if(!m_Input_Type.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND P.INPUT_TYPE = '%s'"), m_Input_Type);
		strAccuWhereStmt += strOption;
	}
	if(!m_Acquisit_Year.IsEmpty())
	{
		strOption.Format(_T(" AND P.ACQ_YEAR = '%s'"), m_Acquisit_Year);
		strAccuWhereStmt += strOption;
	}

	if(!m_Donator.IsEmpty() && m_bShowMode)
	{
		CESL_DataMgr dm;
		CString strDonatorCnt, sQuery;
		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		sQuery.Format(_T("SELECT COUNT(REC_KEY) FROM CO_DONATOR_TBL WHERE DONATOR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),m_Donator);
		dm.GetOneValueQuery(sQuery,strDonatorCnt);
		INT nCnt = _ttoi(strDonatorCnt);
		
		if(m_strDonatorKey.IsEmpty() && nCnt > 1 )
		{
			AfxMessageBox(_T("중복되었거나 없는 기증자입니다. \n\r 검색을 통해 선정해주세요!"));
			return;
		}

		if(m_strDonatorKey.IsEmpty())
		{
			strOption.Format(_T(" AND P.DONATOR_NAME LIKE '%s%%' "), m_Donator);
			strAccuWhereStmt += strOption;
		}
		else
		{
			strOption.Format(_T(" AND P.DONATOR_NAME LIKE '%s%%' AND P.DONATOR_KEY = '%s' "), m_Donator, m_strDonatorKey);
			strAccuWhereStmt += strOption;
		}
	}	
	if(!m_strSelDivNo.IsEmpty())
	{
		strOption = _T(" AND (") + MakeGroupInfoOrQuery(m_strSelDivNo) + _T(")");
		strAccuWhereStmt += strOption;
	}
	
//색인관련 쿼리
//#ifdef _ORA_CATSEARCH_
	if(m_pInfo->GetSearchIndexType() == ORA_CATSEARCH)
	{
		if(!(m_Title.IsEmpty()))
		{
			strOption.Format(_T(" AND CATSEARCH(I.IDX_TITLE, '%s*', NULL)>0"), m_Title);
			strWhere += strOption;
		}
		if(!(m_Author.IsEmpty()) && m_bShowMode)
		{
			strOption.Format(_T(" AND CATSEARCH(I.IDX_AUTHOR, '%s*', NULL)>0"), m_Author);
			strWhere += strOption;
		}
		if(!(m_Publisher.IsEmpty()) && m_bShowMode)
		{
			strOption.Format(_T(" AND CATSEARCH(I.IDX_PUBLISHER, '%s*', NULL)>0"), m_Publisher);
			strWhere += strOption;
		}
		if(!(m_ISBN.IsEmpty()) && m_bShowMode)
		{
			strOption.Format(_T(" AND CATSEARCH(I.ST_CODE, '%s*', NULL)>0"), m_ISBN);
			strWhere += strOption;
		}
	}
//#else
	else if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		m_arrECOSEARCH.RemoveAll();
		if(!(m_Title.IsEmpty()))
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*') "), m_Title);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}
		if(!(m_Author.IsEmpty()) && m_bShowMode)
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*') "), m_Author);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}
		if(!(m_Publisher.IsEmpty()) && m_bShowMode)
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_IDX_PUBLISHER, '%s*') "), m_Publisher);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}
		if(!(m_ISBN.IsEmpty()) && m_bShowMode)
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_ST_CODE, '%s*') "), m_ISBN);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}

		if ( m_bOpenbase )
		{			
			for ( int i=0 ; i<m_arrECOSEARCH.GetSize() ; i++ )
			{
				strOption = m_arrECOSEARCH.GetAt(i);
				strOption.Replace(_T("AND"), _T("INTERSECT"));
				strWhere += strOption;
			}			
		}
		else
		{		
			ids = GetECOSEARCHQuery(strOption);
			if(ids < 0) 
			{
				ESLAfxMessageBox(_T("검색 쿼리를 생성할 수 없습니다."));
				return;
			}
			if(!strOption.IsEmpty())
			{
				strWhere += _T(" AND I.REC_KEY IN ( ") + strOption + _T(")");
			}
		}
	}
//#endif

	if(!(m_PubYear.IsEmpty()) && m_bShowMode)
	{
		strOption.Format(_T(" AND I.PUB_YEAR = '%s'"), m_PubYear);
		strWhere += strOption;
	}
	if(!m_Form_Code.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND I.FORM_CODE = '%s'"), m_Form_Code);
		strWhere += strOption;
	}
	if(!m_Use_Obj_Code.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND I.USE_OBJ_CODE = '%s'"), m_Use_Obj_Code);
		strWhere += strOption;
	}
	if (!(m_Work_Code.IsEmpty()) && m_bShowMode && GetAcqCode() == DONATE)
	{
		strOption.Format(_T(" AND I.WORK_CODE = '%s'"), m_Work_Code);
		strWhere += strOption;
	}
	if (!(m_SubRegNo.IsEmpty()) && m_bShowMode)
	{
		strOption.Format(_T(" AND I.SUB_REG_CODE = '%s'"), m_SubRegNo);
		strWhere += strOption;			
	}
	

	// 색인/수서정보/책 정보 쿼리 통합
	strWhere += strAccuWhereStmt;
	SetWhereStmt(strWhere);
	// 쿼리 생성후 부모에게 메시지를 보낸다.
	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

VOID CBO_ACQ_DonateDataSearchDlg::ShowGroupInfo()
{
	CString strAcqYear;
	strAcqYear = m_pGroups->GetAcqYear();
	if(strAcqYear.IsEmpty()) strAcqYear = GetTodayYear();
	m_pGroups->GetGroupInfo(m_strSelDivNo);
	if(m_strSelDivNo == _T("전체자료")) m_strSelDivNo = _T("");
	m_pCM->SetControlMgrData(_T("그룹번호"), m_strSelDivNo);
	m_pCM->SetControlMgrData(_T("수입년도"), strAcqYear);	
	GetDlgItem(IDC_edtSEARCH_DIVISIONNO)->SetFocus();
}

VOID CBO_ACQ_DonateDataSearchDlg::OnbtnSEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	ClearScreen(m_pCM);
	m_pGroups->RemoveAll();
	ShowGroupInfo();
}

VOID CBO_ACQ_DonateDataSearchDlg::OnbtnSETSORTCONDITION() 
{
	// TODO: Add your control notification handler code here
	ShowDataSortDlg();
}

VOID CBO_ACQ_DonateDataSearchDlg::OnchkSORTYN() 
{
	// TODO: Add your control notification handler code here
	CButton * pBtn = (CButton *)GetDlgItem(IDC_chkSORT_YN);
	if(pBtn && pBtn->GetCheck()) m_bDataSortYN = TRUE;
	else m_bDataSortYN = FALSE;	
}

VOID CBO_ACQ_DonateDataSearchDlg::OnSetfocusedtSEARCHAUTHOR() 
{
	SetIME(IDC_edtSEARCH_AUTHOR, 1);
}

VOID CBO_ACQ_DonateDataSearchDlg::OnSetfocusedtSEARCHDONATOR() 
{
	SetIME(IDC_edtSEARCH_DONATOR, 1);
}

VOID CBO_ACQ_DonateDataSearchDlg::OnSetfocusedtSEARCHPUBLISHER() 
{
	SetIME(IDC_edtSEARCH_PUBLISHER, 1);
}

VOID CBO_ACQ_DonateDataSearchDlg::OnSetfocusedtSEARCHTITLE() 
{
	SetIME(IDC_edtSEARCH_TITLE, 1);
}

HBRUSH CBO_ACQ_DonateDataSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
