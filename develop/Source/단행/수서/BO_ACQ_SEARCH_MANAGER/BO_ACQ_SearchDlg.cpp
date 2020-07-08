// BO_ACQ_SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_SearchDlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/RequisitionNo_List_Dlg.h"
#include "../../../공통/BO_SPECIES_API/ISBNChecker.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_SearchDlg dialog
// 생성자
CBO_ACQ_SearchDlg::CBO_ACQ_SearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBO_SearchDlg(CBO_ACQ_SearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_SearchDlg)
	//}}AFX_DATA_INIT
	m_bShowMode = FALSE;
	m_strSelDivNo	= _T("");
	m_pCM = NULL;
	m_pWorkNoList = NULL;

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
// 소멸자
CBO_ACQ_SearchDlg::~CBO_ACQ_SearchDlg()
{
	DeleteWorkNoList();

}

VOID CBO_ACQ_SearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_SearchDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_SearchDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_SearchDlg)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_cmbSEARCH_CLASS_NO_TYPE, OnSelchangecmbSEARCHCLASSNOTYPE)
	ON_EN_KILLFOCUS(IDC_edtSEARCH_DIVISIONNO, OnKillfocusedtSEARCHDIVISIONNO)
	ON_BN_CLICKED(IDC_btnSEARCH_DIVISIONNO_VIEW, OnbtnSEARCHDIVISIONNOVIEW)
	ON_BN_CLICKED(IDC_btnSEARCH_SHOWHIDE, OnbtnSEARCHSHOWHIDE)
	ON_EN_KILLFOCUS(IDC_edtSEARCH_ACQ_YEAR, OnKillfocusedtSEARCHACQYEAR)
	ON_CBN_SELCHANGE(IDC_cmbSEARCH_JOB_CLASS, OnSelchangecmbSEARCHJOBCLASS)
	ON_BN_CLICKED(IDC_btnSEARCH_CLEAR, OnbtnSEARCHCLEAR)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_BN_CLICKED(IDC_chkSORT_YN, OnchkSORTYN)
	ON_EN_SETFOCUS(IDC_edtSEARCH_AUTHOR, OnSetfocusedtSEARCHAUTHOR)
	ON_EN_SETFOCUS(IDC_edtSEARCH_PUBLISHER, OnSetfocusedtSEARCHPUBLISHER)
	ON_EN_SETFOCUS(IDC_edtSEARCH_TITLE, OnSetfocusedtSEARCHTITLE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_SearchDlg message handlers

VOID CBO_ACQ_SearchDlg::OnClose() 
{
	CDialog::OnClose();
}

BOOL CBO_ACQ_SearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// SM설정
	if( InitESL_Mgr(_T("K2UPGRADE_수서자료검색")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		

	// CM/DM 설정
	m_pCM = FindCM(_T("CM_수서자료검색"));		
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox( _T("[CM_수서자료검색]을 초기화 할 수 없습니다.") );
		return FALSE;
	}

	// 입력구분/주제구분 멀티콤보 생성
	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("검색화면을 초기화할 수 없습니다."));
		return FALSE;
	}
	
	GetWindowRect(m_rcWindow);

	// 간략검색/상세검색 버튼 설정에 따라 리소스들의 위치 및 표시를 설정한다.
	ShowControlByShowMode();

	// 입수구분에 따라 리소스들을 설정 및 표시한다.
	ShowControlByAcqCode();

	//============================================
	//20080603 ADD BY PJW : 수입년을 숫자 4자리 이상 넣지않도록 수정
//	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtSEARCH_ACQ_YEAR);
//	pAcqYear->SetLimitText(4);
	//============================================

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 입력구분/주제구분을 멀티콤보로 생성한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
BOOL CBO_ACQ_SearchDlg::Init()
{
	// 입력구분 콤보 리스트 데이터 설정
	CString inputType[5][2] = 
	{
		//===================================
		//2008.07.17 UPDATE BY PJW : 스페이스바 누르면 적용안함으로 가도록 블랭크 길이 수정
// 		_T(" "), _T("적용안함"),
		_T("    "), _T("적용안함"),
		//===================================
		_T("0"), _T("화면입력"),
		_T("1"), _T("재활용반입"),
		_T("2"), _T("희망자료반입"),
		_T("3"), _T("파일반입")
	};
	CMulitComboBoxManager mgr(this);

	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("적용안함"), inputType, _T("입력구분_코드"), 5);
	// 2006-04-11 KBS Modify
	// 주제구분 설정
	//=====================================================
	//2008.07.16 UPDATE BY PJW : 주제구분코드를 디폴트값으로 가지도록 수정
// 	mgr.SetSubjectCodeData(m_pCM->CM_ALIAS, _T("분류표구분_코드"), _T("주제구분_코드"), _T(""));
	CString strSubCode;
	m_pInfo->pCodeMgr->GetDefaultCode(_T("KDC대주제부호관리"), strSubCode);
	if( strSubCode.IsEmpty() )
	{
		strSubCode.Format(_T("  "));
	}
	mgr.SetSubjectCodeData(m_pCM->CM_ALIAS, _T("분류표구분_코드"), _T("주제구분_코드"), strSubCode);
	//=====================================================
	
	//SetSubjectCodeData(m_pCM->CM_ALIAS, _T("분류표구분_코드"), _T("주제구분_코드"));
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 간략검색/상세검색 버튼 설정에 따라 리소스들의 위치 및 표시를 설정한다.
// [ 미니스펙 ]
// 1. 간략검색/상세검색 버튼 설정
// 2. 윈도우 영역 설정
// 3. 아래의 리소스들은 [상세검색]일 경우에만 보이도록 설정한다.
// 4. 간략검색/상세검색 여부에 따라 윈도우 영역을 조절한다.
// 5. 아래의 리소스들은 [간략검색/상세검색]버튼 설정에 따라 윈도우 위치를 조절한다.
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_SearchDlg::ShowControlByShowMode()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT i, nCodeResourcesHeight;
	BOOL bMoveWindow = TRUE;
	CRect rcWindow;	
	CWnd* pWnd = NULL;
	CArray<INT, INT> ArrResourceID;	ArrResourceID.RemoveAll();

	nCodeResourcesHeight = 224;

	//=======================================================================
	// 1. 간략검색/상세검색 버튼 설정
	//=======================================================================
	if( m_bShowMode == TRUE )			GetDlgItem(IDC_btnSEARCH_SHOWHIDE)->SetWindowText(_T("간략검색"));
	else								GetDlgItem(IDC_btnSEARCH_SHOWHIDE)->SetWindowText(_T("상세검색"));

	//=======================================================================
	// 2. 윈도우 영역 설정
	//=======================================================================
	GetWindowRect(rcWindow);

	//=======================================================================
	// 3. 아래의 리소스들은 [상세검색]일 경우에만 보이도록 설정한다.
	//=======================================================================
	ArrResourceID.Add(IDC_stcGROUP1);			ArrResourceID.Add(IDC_stcAUTHOR);
	ArrResourceID.Add(IDC_edtSEARCH_AUTHOR);	ArrResourceID.Add(IDC_stcPUBLISHER);
	ArrResourceID.Add(IDC_edtSEARCH_PUBLISHER);	ArrResourceID.Add(IDC_stcPUBYEAR);
	ArrResourceID.Add(IDC_edtSEARCH_PUBYEAR);	ArrResourceID.Add(IDC_stcISBN);
	ArrResourceID.Add(IDC_edtSEARCH_ISBN);		ArrResourceID.Add(IDC_stcGROUP2);
	ArrResourceID.Add(IDC_stcPROVIDER);			ArrResourceID.Add(IDC_cmbSEARCH_PROVIDER_CODE);
	ArrResourceID.Add(IDC_radSEARCH_MYDATA);	ArrResourceID.Add(IDC_radSEARCH_ALL);
	ArrResourceID.Add(IDC_stcGROUP3);			ArrResourceID.Add(IDC_stcFORMCODE);
	ArrResourceID.Add(IDC_cmbSEARCH_FORM_CODE);	ArrResourceID.Add(IDC_stcUSEOBJCODE);
	ArrResourceID.Add(IDC_cmbSEARCH_USER_OBJ_CODE);	ArrResourceID.Add(IDC_stcINPUTTYPE);
	ArrResourceID.Add(IDC_cmbSEARCH_INPUT_TYPE);	ArrResourceID.Add(IDC_stcSUBJECTCODE);
	ArrResourceID.Add(IDC_cmbSEARCH_SUBJECT_CODE);	ArrResourceID.Add(IDC_stcCLASSNOTYPE);
	ArrResourceID.Add(IDC_cmbSEARCH_CLASS_NO_TYPE);

	// Show / Hide Code Resource
	for( i=0; i<ArrResourceID.GetSize(); i++ )
	{
		pWnd = GetDlgItem(ArrResourceID.GetAt(i));
		if( pWnd->GetSafeHwnd() != NULL )
		{
			if( m_bShowMode == TRUE )	pWnd->ShowWindow(SW_SHOW);
			else						pWnd->ShowWindow(SW_HIDE);
		}
	}

	//=======================================================================
	// 4. 간략검색/상세검색 여부에 따라 윈도우 영역을 조절한다.
	//=======================================================================
	if( m_bShowMode == TRUE )
	{
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	}
	
	//=======================================================================
	// 5. 아래의 리소스들은 [간략검색/상세검색]버튼 설정에 따라 윈도우 위치를 조절한다.
	//=======================================================================
	ArrResourceID.RemoveAll();
	ArrResourceID.Add(IDC_stcSORT_CONDITION_GROUP);		ArrResourceID.Add(IDC_chkSORT_YN);
	ArrResourceID.Add(IDC_btnSET_SORT_CONDITION);
	for( i=0; i<ArrResourceID.GetSize(); i++ )
	{
		pWnd = GetDlgItem(ArrResourceID.GetAt(i));
		if( pWnd->GetSafeHwnd() != NULL )
		{
			pWnd->GetWindowRect(&rcWindow);
			ScreenToClient(rcWindow);
			
			if( m_bShowMode == TRUE )
			{
				rcWindow.top += nCodeResourcesHeight;
				rcWindow.bottom += nCodeResourcesHeight;
				pWnd->MoveWindow(rcWindow);
			}
			else
			{
				rcWindow.top -= nCodeResourcesHeight;
				rcWindow.bottom -= nCodeResourcesHeight;
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

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// -  입수구분에 따라 리소스들을 설정 및 표시한다.
// [ 미니스펙 ]
// 1. 입수구분 값을 가져온다.
// 2. 입수구분(구입/기증)에 따라 리소스들을 설정 및 표시한다.
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_SearchDlg::ShowControlByAcqCode()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT i, acqCode, nPurchaseCount, nDonateCount;
	INT nPurchaseArrCodeResourceID[10], nDonateArrCodeResourceID[10];
	CWnd* pWnd = NULL;

	nPurchaseCount = nDonateCount = 0;

	//=======================================================================
	// 1. 입수구분 값을 가져온다.
	//=======================================================================
	acqCode = GetAcqCode();

	//=======================================================================
	// 2. 수입구분(구입/기증)에 따라 리소스들을 설정 및 표시한다.
	//=======================================================================
	nPurchaseArrCodeResourceID[nPurchaseCount++] = IDC_stcPROVIDER;
	nPurchaseArrCodeResourceID[nPurchaseCount++] = IDC_cmbSEARCH_PROVIDER_CODE;
	nPurchaseArrCodeResourceID[nPurchaseCount++] = IDC_stcSEARCH_WORK_NO;
	nPurchaseArrCodeResourceID[nPurchaseCount++] = IDC_cmbSEARCH_WORK_NO;

	nDonateArrCodeResourceID[nDonateCount++] = IDC_stcSEARCH_DONATOR;
	nDonateArrCodeResourceID[nDonateCount++] = IDC_edtSEARCH_DONATOR;
	nDonateArrCodeResourceID[nDonateCount++] = IDC_btnFIND_DONATOR;

	switch( acqCode )
	{
	case 1:	// 구입
	{
			for( i=0; i<nPurchaseCount; i++ )
		{
				pWnd = GetDlgItem(nPurchaseArrCodeResourceID[i]);
				if( pWnd->GetSafeHwnd() != NULL )	pWnd->ShowWindow(SW_SHOW);
		}

			for( i=0; i<nDonateCount; i++ )
		{
				pWnd = GetDlgItem(nDonateArrCodeResourceID[i]);
				if( pWnd->GetSafeHwnd() != NULL )	pWnd->ShowWindow(SW_HIDE);
		}
			
		GetDlgItem(IDC_stcSEARCH_DIVNO)->SetWindowText(_T("차수번호"));
		GetDlgItem(IDC_btnSEARCH_DIVISIONNO_VIEW)->SetWindowText(_T("차수보기"));
	}
		break;

	case 2:	// 기증
	{
			for( i=0; i<nPurchaseCount; i++ )
		{
			pWnd = GetDlgItem(nPurchaseArrCodeResourceID[i]);
				if( pWnd->GetSafeHwnd() != NULL )	pWnd->ShowWindow(SW_HIDE);
		}

			for( i=0; i<nDonateCount; i++ )
		{
			pWnd = GetDlgItem(nDonateArrCodeResourceID[i]);
				if( pWnd->GetSafeHwnd() != NULL )	pWnd->ShowWindow(SW_SHOW);
		}

		GetDlgItem(IDC_stcSEARCH_DIVNO)->SetWindowText(_T("접수번호"));
		GetDlgItem(IDC_btnSEARCH_DIVISIONNO_VIEW)->SetWindowText(_T("접수번호"));
	}
		break;
	default:
		break;
	}
}

VOID CBO_ACQ_SearchDlg::OnCancel() 
{
	//========================================
	//2008.07.08 ADD BY PJW : 검색창을 닫을경우 작업번호를 적용안함으로 변경
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("작업번호"));
	pCmb->SelectString(0,_T("적용안함"));
	//========================================
	CDialog::OnCancel();
}

VOID CBO_ACQ_SearchDlg::OnOK() 
{
	INT ids ;
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
	CString m_Provider = _T("");
	CString m_SubRegNo = _T("");

	INT iResult;
	CString strAddedISBN, strISBNTo;
	CISBNChecker isbnChecker;

	// IDX_BO_TBL의 SPECIES_CLASS 0:소장종, 1:작업종, 2:재활용, 3:삭제종
	CString strWhere = _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND I.SPECIES_CLASS IN('0', '1')");
	CString strWorkingStatus = _T("");
	if(GetOpenMode() == BO_ACQ_PURCHASE) strWorkingStatus = _T("BOA111N");
	
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
	if(m_strSelDivNo.IsEmpty() && m_pInfo->GetKIIUPClientVersion() != 'L') 
	{
		strMsg = _T("차수정보를 설정해 주십시요.");
		ESLAfxMessageBox(strMsg);
		return;
	}
	
	m_pCM->GetControlMgrData(_T("서명"), m_Title);
	m_pCM->GetControlMgrData(_T("저자"), m_Author);
	m_pCM->GetControlMgrData(_T("발행자"), m_Publisher);
	m_pCM->GetControlMgrData(_T("ISBN"), m_ISBN);
	m_pCM->GetControlMgrData(_T("형식구분_코드"), m_Form_Code);
	m_pCM->GetControlMgrData(_T("이용대상구분_코드"), m_Use_Obj_Code);
	m_pCM->GetControlMgrData(_T("주제구분_코드"), m_Subject_Code);
	m_pCM->GetControlMgrData(_T("입력구분_코드"), m_Input_Type);
	m_pCM->GetControlMgrData(_T("구입처"), m_Provider);
	m_pCM->GetControlMgrData(_T("업무구분_코드"), m_Work_Code);
	m_pCM->GetControlMgrData(_T("분류표구분_코드"), m_Classfy_Code);
	m_pCM->GetControlMgrData(_T("보조등록구분_코드"), m_SubRegNo);
	
	
	TrimAll(m_Title);
	TrimAll(m_Author);
	TrimAll(m_PubYear);
	TrimAll(m_ISBN);
	TrimAll(m_Form_Code);
	TrimAll(m_Use_Obj_Code);
	TrimAll(m_Subject_Code);
	TrimAll(m_Input_Type);
	TrimAll(m_Provider);
	TrimAll(m_Work_Code);
	TrimAll(m_Classfy_Code);
	TrimAll(m_SubRegNo);


	// 그룹컬렉션 구축
	this->MakeGroupInfo(m_Acquisit_Year, m_strSelDivNo, m_Work_Code);

	//본표제, 저작자, 발행자 색인어 생성형식으로 변환
	pMakeSearchData->GetKeyWord(m_Title);		pMakeSearchData->ChangeHanjaToHangul(m_Title);
	pMakeSearchData->GetKeyWord(m_Author);	pMakeSearchData->ChangeHanjaToHangul(m_Author);
	pMakeSearchData->GetKeyWord(m_Publisher);	pMakeSearchData->ChangeHanjaToHangul(m_Publisher);
	pMakeSearchData->GetKeyWord(m_ISBN);

	CString strOption, strAcqCode;

//책관련 쿼리	
	strOption.Format(_T(" AND B.WORKING_STATUS = '%s' AND B.ACQ_CODE = '1'"), strWorkingStatus);
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
	if(!m_Input_Type.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND P.INPUT_TYPE = '%s'"), m_Input_Type);
		strAccuWhereStmt += strOption;
	}
	if(!m_Acquisit_Year.IsEmpty())
	{
		strOption.Format(_T(" AND P.ORDER_YEAR = '%s'"), m_Acquisit_Year);
		strAccuWhereStmt += strOption;
	}
	if(!m_Provider.IsEmpty() && m_bShowMode)
	{
		strOption.Format(_T(" AND P.PROVIDER_CODE = '%s'"), m_Provider);
		strAccuWhereStmt += strOption;
	}
	if(!m_strSelDivNo.IsEmpty())
	{
		//이부분 쿼리가 잘못 되었어여
		//strOption = _T(" AND (") + MakeDivnoOrWorkCodeQuery() + _T(")");
		strOption.Format(_T(" AND P.ORDER_SEQ_NO IN (%s)"), m_strSelDivNo);
		strAccuWhereStmt += strOption;
	}
	else
	{
		strOption.Format(_T(" AND P.ORDER_SEQ_NO IS NULL"), m_strSelDivNo);
		strAccuWhereStmt += strOption;
	}
	CString strWorkNo;
	m_pCM->GetControlMgrData(_T("작업번호"), strWorkNo);
	if(!strWorkNo.IsEmpty() && strWorkNo != _T("적용안함"))
	{
		strOption.Format(_T(" AND P.ORDER_WORK_NO = %s"), strWorkNo);
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
		if( (m_ISBN.IsEmpty()==FALSE) && m_bShowMode)
		{
			switch( m_ISBN.GetLength() )
			{
			case 10:
				{
					strAddedISBN = _T("978") + m_ISBN;
					isbnChecker.ConvertISBN(strAddedISBN.GetBuffer(0), strISBNTo.GetBuffer(MAX_PATH), &iResult );
					strISBNTo.ReleaseBuffer();
					pMakeSearchData->GetKeyWord(strISBNTo);					
					strOption.Format(_T(" AND CATSEARCH(I.ST_CODE, '%s|%s', NULL)>0"), m_ISBN, strISBNTo);
				}
				break;

			case 13:
				{
					if( _T("978") == m_ISBN.Left(3) )
					{
						strAddedISBN = m_ISBN.Mid(3);
						isbnChecker.ConvertISBN(strAddedISBN.GetBuffer(0), strISBNTo.GetBuffer(MAX_PATH), &iResult );
						strISBNTo.ReleaseBuffer();
						pMakeSearchData->GetKeyWord(strISBNTo);
						strOption.Format(_T(" AND CATSEARCH(I.ST_CODE, '%s|%s', NULL)>0"), m_ISBN, strISBNTo);
					}
					else
					{
						strOption.Format(_T(" AND CATSEARCH(I.ST_CODE, '%s', NULL)>0"), m_ISBN);
					}
				}
				break;

			default:
				{
					strOption.Format(_T(" AND CATSEARCH(I.ST_CODE, '%s', NULL)>0"), m_ISBN);
				}
				break;
			}
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
		if( (m_ISBN.IsEmpty()==FALSE) && m_bShowMode)
		{
			switch( m_ISBN.GetLength() )
			{
			case 10:
				{
					strAddedISBN = _T("978") + m_ISBN;
					isbnChecker.ConvertISBN(strAddedISBN.GetBuffer(0), strISBNTo.GetBuffer(MAX_PATH), &iResult );
					strISBNTo.ReleaseBuffer();
					pMakeSearchData->GetKeyWord(strISBNTo);
					strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_ST_CODE, '%s|%s') "), m_ISBN, strISBNTo);
				}
				break;

			case 13:
				{
					if( _T("978") == m_ISBN.Left(3) )
					{
						strAddedISBN = m_ISBN.Mid(3);
						isbnChecker.ConvertISBN(strAddedISBN.GetBuffer(0), strISBNTo.GetBuffer(MAX_PATH), &iResult );
						strISBNTo.ReleaseBuffer();
						pMakeSearchData->GetKeyWord(strISBNTo);						
						strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_ST_CODE, '%s|%s') "), m_ISBN, strISBNTo);
					}
					else
					{
						strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_ST_CODE, '%s') "), m_ISBN);
					}
				}
				break;

			default:
				{
					strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, BO_ST_CODE, '%s') "), m_ISBN);
				}
				break;
			}

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
	if (!(m_Work_Code.IsEmpty()))
	{
		strOption.Format(_T(" AND I.WORK_CODE = '%s'"), m_Work_Code);
		strWhere += strOption;
	}
	else 
	{
		strOption.Format(_T(" AND I.WORK_CODE IS NULL"));
		strWhere += strOption;
	}
	if (!(m_Classfy_Code.IsEmpty()) && m_bShowMode)
	{
		strOption.Format(_T(" AND I.CLASS_NO_TYPE = '%s'"), m_Classfy_Code);
		strWhere += strOption;			
	}
	if (!(m_SubRegNo.IsEmpty()) && m_bShowMode)
	{
		strOption.Format(_T(" AND I.SUB_REG_CODE = '%s'"), m_SubRegNo);
		strWhere += strOption;			
	}

	//UpdateData(TRUE);
	
	// 색인/수서정보/책 정보 쿼리 통합
	strWhere += strAccuWhereStmt;
	SetWhereStmt(strWhere);
	// 쿼리 생성후 부모에게 메시지를 보낸다.
	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
	
	//CDialog::OnOK();
}

BOOL CBO_ACQ_SearchDlg::PreTranslateMessage(MSG* pMsg) 
{
 	if( pMsg->message == WM_KEYDOWN   ) 
	{
		if( pMsg->wParam == VK_RETURN )
	{
		GetDlgItem(IDOK)->SetFocus();
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	}
	else if( pMsg->message == WM_CHAR )
    {
		if( pMsg->hwnd == GetDlgItem(IDC_edtSEARCH_DIVISIONNO)->GetSafeHwnd() )
    {
		INT nVirtKey = (INT) pMsg->wParam;
			if( (nVirtKey < '0' || nVirtKey > '9') && nVirtKey != ',' && nVirtKey != ' ' && nVirtKey != 8 )
		{
			ESLAfxMessageBox(_T("숫자만 입력하실 수 있습니다.")); 
            return TRUE;
		}
    }
    }
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_ACQ_SearchDlg::OnSelchangecmbSEARCHCLASSNOTYPE() 
{
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

VOID CBO_ACQ_SearchDlg::OnKillfocusedtSEARCHDIVISIONNO() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strWorkCode;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("그룹번호"), m_strSelDivNo);
	m_pCM->GetControlMgrData(_T("업무구분_코드"), strWorkCode);
	INT acqCode = GetAcqCode();
	m_strSelDivNo = CheckValidateGroupNo(m_pCM, m_strSelDivNo, strAcqYear, strWorkCode);
	m_pCM->SetControlMgrData(_T("그룹번호"), m_strSelDivNo);
	CStringArray arrDivNo;		arrDivNo.RemoveAll();
	ParsingString(_T(","), m_strSelDivNo, &arrDivNo);
	
	INT cnt = arrDivNo.GetSize();
	INT ids = -1;
	if(cnt == 1 && acqCode == 1)
	{
		ids = SetWorkNoData(strAcqYear, arrDivNo.GetAt(0), strWorkCode);
		if(ids < 0) 
		{
			ESLAfxMessageBox(_T("작업번호 정보를 구축할 수 없습니다."));
			return;
		}
	}	
	else if(cnt > 1 && acqCode == 1)
	{
		if(m_pWorkNoList != NULL)
		{
			delete[] m_pWorkNoList;
			m_pWorkNoList = NULL;
		}
		m_pWorkNoList = new CString[1];
		m_pWorkNoList[0] = _T("적용안함");
		CMulitComboBoxManager mgr(this);
		mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("적용안함"), m_pWorkNoList, _T("작업번호"), 1);
	}
}

VOID CBO_ACQ_SearchDlg::OnbtnSEARCHDIVISIONNOVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strWorkCode;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("업무구분_코드"), strWorkCode);
	INT acqCode = GetAcqCode();

	CDivisionNo_List_Dlg dlg(this);
	dlg.SetWorkStatus(_T("구입대상"));
	dlg.SetAcquisitYear(strAcqYear);
	dlg.SetGroupCollection(m_pGroups);
	dlg.SetWorkCode(strWorkCode);
	dlg.SetSpeciesType(GetSpeciesType());
	if(IDOK == dlg.DoModal())
	{
		m_strSelDivNo = dlg.GetSelDivNo();
		m_pCM->SetControlMgrData(_T("그룹번호"), m_strSelDivNo);
		m_pCM->SetControlMgrData(_T("수입년도"), m_pGroups->GetAcqYear());
		m_pCM->SetControlMgrData(_T("업무구분_코드"), m_pGroups->GetWorkCode());
		OnKillfocusedtSEARCHDIVISIONNO();
		OnOK();
	}
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 인자로 온 수입년도와 차수번호에 따른 작업번호를 설정한다.
// [ 미니스펙 ]
// 1. 그룹매니져 클래스를 초기화/수입년도와 차수번호를 설정한다.
// 2. 그룹매니져에서 [마지막작업번호추출]을 수행한다.
// 3. 작업번호리스트를 지우고 다시 설정한다.
// 4. 설정된 작업번호를 CM에 콤보박스로 표시한다.
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_SearchDlg::SetWorkNoData(CString strAcqYear, CString strDivNo, CString strWorkCode/*=_T("")*/)
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT i, ids;
	CString strTmpData;
	CGroupMgr groupmgr(this);
	CMulitComboBoxManager mgr(this);

	//=======================================================================
	// 1. 그룹매니져 클래스를 초기화/수입년도와 차수번호를 설정한다.
	//=======================================================================
	groupmgr.InitTempData();
	groupmgr.SetTempData(_T("AcqYear"),		strAcqYear.GetBuffer(0)	);
	groupmgr.SetTempData(_T("DivNo"),		strDivNo.GetBuffer(0)	);
	groupmgr.SetTempData(_T("WorkCode"),	strWorkCode.GetBuffer(0));

	//=======================================================================
	// 2. 그룹매니져에서 [마지막작업번호추출]을 수행한다.
	//=======================================================================
	ids = groupmgr.SPFExecutePath(_T("마지막작업번호추출"));
	if( ids < 0 )	return ids;

	//=======================================================================
	// 3. 작업번호리스트를 지우고 다시 설정한다.
	//=======================================================================
	DeleteWorkNoList();
	m_pWorkNoList = new CString[ids+1];
	m_pWorkNoList[0] = _T("적용안함");
	for( i=1; i<=ids; i++ )
	{
		strTmpData.Format(_T("%d"), i);
		m_pWorkNoList[i] = strTmpData;
	}

	//=======================================================================
	// 4. 설정된 작업번호를 CM에 콤보박스로 표시한다.
	//=======================================================================
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("적용안함"), m_pWorkNoList, _T("작업번호"), ids+1);

	return 0;
}

VOID CBO_ACQ_SearchDlg::OnbtnSEARCHSHOWHIDE() 
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
//	ShowControlByAcqCode();
}

VOID CBO_ACQ_SearchDlg::OnKillfocusedtSEARCHACQYEAR() 
{
	// TODO: Add your control notification handler code here
	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtSEARCH_ACQ_YEAR);

	//============================================
	//20080603 ADD BY PJW : 수입년을 숫자 4자리를 넣지않았을 경우 4자리 숫자를 맞춰준다 
//	CString strAcqYear;
//	INT nYear;
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

	if (pAcqYear->GetStyle() & ES_READONLY) return;	
	OnKillfocusedtSEARCHDIVISIONNO();
}


VOID CBO_ACQ_SearchDlg::ShowGroupInfo()
{
	CString strAcqYear, strWorkCode;

	strAcqYear = m_pGroups->GetAcqYear();
	if( strAcqYear.IsEmpty() )	strAcqYear = GetTodayYear();
	m_pGroups->GetGroupInfo(m_strSelDivNo);

	if( m_strSelDivNo == _T("전체자료") )	m_strSelDivNo = _T("");

	m_pCM->SetControlMgrData(_T("그룹번호"), m_strSelDivNo);
	m_pCM->SetControlMgrData(_T("수입년도"), strAcqYear);

	if( m_pGroups->GetGroupCount() == 0 )	m_pInfo->pCodeMgr->GetDefaultCode(_T("업무구분"), strWorkCode);
	else									strWorkCode = m_pGroups->GetWorkCode();

	m_pCM->SetControlMgrData(_T("업무구분_코드"), strWorkCode);
	GetDlgItem(IDC_edtSEARCH_DIVISIONNO)->SetFocus();
}

VOID CBO_ACQ_SearchDlg::OnSelchangecmbSEARCHJOBCLASS() 
{
	// TODO: Add your control notification handler code here
	OnKillfocusedtSEARCHDIVISIONNO();
}

VOID CBO_ACQ_SearchDlg::OnbtnSEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	ClearScreen(m_pCM);
	m_pGroups->RemoveAll();
	ShowGroupInfo();
}

VOID CBO_ACQ_SearchDlg::OnbtnSETSORTCONDITION() 
{
	// TODO: Add your control notification handler code here
	ShowDataSortDlg();
}

VOID CBO_ACQ_SearchDlg::OnchkSORTYN() 
{
	// TODO: Add your control notification handler code here
	CButton * pBtn = (CButton *)GetDlgItem(IDC_chkSORT_YN);
	if(pBtn && pBtn->GetCheck()) m_bDataSortYN = TRUE;
	else m_bDataSortYN = FALSE;	
}

VOID CBO_ACQ_SearchDlg::DeleteWorkNoList()
{
	if( m_pWorkNoList )
	{
		delete[] m_pWorkNoList;
		m_pWorkNoList = NULL;
	}
}

VOID CBO_ACQ_SearchDlg::OnSetfocusedtSEARCHAUTHOR() 
{
	SetIME(IDC_edtSEARCH_AUTHOR, 1);
}

VOID CBO_ACQ_SearchDlg::OnSetfocusedtSEARCHPUBLISHER() 
{
	SetIME(IDC_edtSEARCH_PUBLISHER, 1);
}

VOID CBO_ACQ_SearchDlg::OnSetfocusedtSEARCHTITLE() 
{
	SetIME(IDC_edtSEARCH_TITLE, 1);
}

HBRUSH CBO_ACQ_SearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
