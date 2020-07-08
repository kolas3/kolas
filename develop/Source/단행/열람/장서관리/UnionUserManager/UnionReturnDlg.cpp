// UnionReturnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnionReturnDlg.h"
#include "UnionUserInfoViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnionReturnDlg dialog


CUnionReturnDlg::CUnionReturnDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUnionReturnDlg)
	//}}AFX_DATA_INIT
	m_pmcbReturnLibrary = NULL;

	m_nReturnCode = 0;
	m_bStatus = FALSE;

	EnableThemeDialogTexture(GetSafeHwnd()); 
}

CUnionReturnDlg::~CUnionReturnDlg()
{
	///{{++
	/// 2008.05.02 add by pdw
	/// 대출반납화면을 동기화 시키기 위해서 메시지 호출
	pParentMgr->SendMessage(WM_USER+107);
	///}}--

	if( m_pmcbReturnLibrary )
	{
		delete m_pmcbReturnLibrary;
		m_pmcbReturnLibrary = NULL;
	}

}

void CUnionReturnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnionReturnDlg)
	DDX_Control(pDX, IDC_EDT_UNION_RETURN_REGNO_USERINPUT, m_edtInputRegNo);
	DDX_Control(pDX, IDC_BTN_UNION_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BTN_UNION_SELECT, m_btnSelect);
	DDX_Control(pDX, IDC_EDT_UNION_RETURN_LIBREGNO, m_edtLibRegNo);
	DDX_Control(pDX, IDC_EDT_UNION_RETURN_REGNO, m_edtRegNo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUnionReturnDlg, CDialog)
	//{{AFX_MSG_MAP(CUnionReturnDlg)
	ON_BN_CLICKED(IDC_BTN_UNION_RETURN_CLOSE, OnBtnUnionReturnClose)
	ON_BN_CLICKED(IDC_BTN_UNION_SEARCH, OnBtnUnionSearch)
	ON_BN_CLICKED(IDC_BTN_UNION_RETURN_HISTORY, OnBtnUnionReturnHistory)
	ON_BN_CLICKED(IDC_BTN_UNION_RETURN_INPUT, OnBtnUnionReturnInput)
	ON_BN_CLICKED(IDC_BTN_UNION_RETURN_CLEAR, OnBtnUnionReturnClear)
	ON_BN_CLICKED(IDC_BTN_UNION_SELECT, OnBtnUnionSelect)
	ON_BN_CLICKED(IDC_RADIO_RETURN_CANCEL, OnRadioReturnCancel)
	ON_BN_CLICKED(IDC_RADIO_RETURN, OnRadioReturn)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnionReturnDlg message handlers

BOOL CUnionReturnDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam )
	{		
		HWND MsgH = pMsg->hwnd;
		if( GetDlgItem(IDC_EDT_UNION_RETURN_REGNO)->m_hWnd==MsgH )
		{
			if( ((CButton*)GetDlgItem(IDC_RADIO_RETURN))->GetCheck() == 1)
				SearchUnionLoanHistory();
			else
				SearchUnionReturnHistory();
		}
		else if( GetDlgItem(IDC_EDT_UNION_RETURN_LIBREGNO)->m_hWnd == MsgH )
		{
			SelectUnionLoanHistory();
		}
		return 0;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CUnionReturnDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	if( 0 > InitESL_Mgr(_T("UnionReturnDlg")) )
	{
		AfxMessageBox(_T("InitESL_Mgr ERROR !!"));
		return false;
	}	
	
	// 20071128 ADD BY PWR ++++++++++++++++++++++++++++++++++++++++++
	//등록번호 입력길이 제한
	m_edtRegNo.SetLimitText(12);
	//도서관번호 입력길이 제한
	m_edtLibRegNo.SetLimitText(6);
	//수동입력 확인
	m_bUserInput = FALSE;
	//---------------------------------------------------------------

	///도서관목록 초기화
	if( 0 > InitReturnLibraryList() )
	{
		AfxMessageBox(_T("InitReturnLibraryList ERROR !!"));
		return false;
	}

	///자관정보 설정
	if( 0 > InitRequestLibraryInfo() )
	{
		AfxMessageBox(_T("InitRequestLibraryInfo ERROR !!"));
		return false;
	}
	
	/// 초기화
	if( 0 > Init() )
	{
		AfxMessageBox(_T("Init ERROR !!"));
		return false;
	}
	//자동닫기 (default:NON)
	((CButton*)GetDlgItem(IDC_CHK_HOLD))->SetCheck(0);
	//메시지창초기화
	SetDlgItemText(IDC_STC_MESSAGE, _T(""));

	CenterWindow(this);
	GetWindowRect(m_rect);

	SetControlShow(TRUE);

	return TRUE;
}

void CUnionReturnDlg::OnBtnUnionReturnClose() 
{	
	OnCancel();
}

void CUnionReturnDlg::OnBtnUnionSearch() 
{
EFS_BEGIN

	//20071221 update by pwr
	if( ((CButton*)GetDlgItem(IDC_RADIO_RETURN_CANCEL))->GetCheck() == 0 )
		SearchUnionLoanHistory();
	else
		SearchUnionReturnHistory();

EFS_END	
}

void CUnionReturnDlg::OnBtnUnionSelect() 
{
EFS_BEGIN

	SelectUnionLoanHistory();

EFS_END
}

void CUnionReturnDlg::OnBtnUnionReturnHistory() 
{
EFS_BEGIN

	//20071221 update by pwr
	if( ((CButton*)GetDlgItem(IDC_RADIO_RETURN_CANCEL))->GetCheck() == 0 )
		ReturnUnionLoanHistory();
	else
		ReturnUnionReturnHistory();

EFS_END
}

void CUnionReturnDlg::OnBtnUnionReturnInput() 
{
EFS_BEGIN

	ReturnUnionUserInput();

EFS_END
}

void CUnionReturnDlg::OnBtnUnionReturnClear() 
{
EFS_BEGIN

	//20071224 update by pwr
	if( ((CButton*)GetDlgItem(IDC_RADIO_RETURN_CANCEL))->GetCheck() == 0 )
		Init();	
	else
		OnRadioReturnCancel();
	
EFS_END	
}


///////////////////////////////////////////////////////////////////////
// 도서관정보 멀티콤보박스를 초기화 한다.
// 1. subclassing
// 2. 통합대출공유서버에서 도서관정보 가져오기
// 3. combo에 도서관정보 입력
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::InitReturnLibraryList()
{
EFS_BEGIN
	
	INT		ids;	

	// 1. subclassing
	if( m_pmcbReturnLibrary )
	{
		delete m_pmcbReturnLibrary;
		m_pmcbReturnLibrary = NULL;
	}

	m_pmcbReturnLibrary = new CESL_ControlMultiComboBox(NULL);
	m_pmcbReturnLibrary->SubclassDlgItem(IDC_CMB_UNION_RETURN_LIBLIST, this);

	// 2. 통합대출공유서버에서 도서관정보 가져오기	
	CRS_SQL		  RemoteDBMgr(this);
	CESL_DataMgr* pdmReturnLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));

//	ids = RemoteDBMgr.SetFetchCount(0, 100);
	if( 0 > ids ) return ids;
//	ids = RemoteDBMgr.LOAN_Select(pdmReturnLibrary, _T(""), _T("DM_UNION_RETURN_LIBINFO"), 1, 1);
	if( 0 > ids ) return ids;

	// 3. combo에 도서관정보 입력
	const int nDataCount = 2;
	CString	  strReturnLibraryData[nDataCount];

	m_pmcbReturnLibrary->InitMultiComboBox(nDataCount);

	strReturnLibraryData[0] = _T("      ");
	strReturnLibraryData[1] = _T("적용안함");

	ids = m_pmcbReturnLibrary->SetData(strReturnLibraryData, nDataCount);
	if( 0 > ids ) return ids;

	for( INT i = 0; i < pdmReturnLibrary->GetRowCount(); i++ )
	{		
		ids = pdmReturnLibrary->GetCellData( _T("도서관코드"), i, strReturnLibraryData[0] );
		if( 0 > ids ) return ids;
		ids = pdmReturnLibrary->GetCellData( _T("도서관명"),   i, strReturnLibraryData[1] );
		if( 0 > ids ) return ids;
		
		ids = m_pmcbReturnLibrary->SetData(strReturnLibraryData, nDataCount);
		if( 0 > ids ) return ids;
	}

	m_pmcbReturnLibrary->SetCurSel(0);
	
	return 0;
EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// 자관 도서관 코드정보를 설정한다.
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::InitRequestLibraryInfo()
{
EFS_BEGIN

	INT		  ids;
	const int nFieldCnt = 1;
	CString	  strField[ nFieldCnt ] = { _T("LIB_CODE") };
	CString   strLibCode[ nFieldCnt ];

	ids = CLocCommonAPI::GetLibInfo( strField, strLibCode, nFieldCnt, pParentMgr );
	if( 0 > ids ) return ids;

	m_strLibraryCode = strLibCode[0];

	return 0;

EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// 초기화 함수
// 1. 컨트롤을 초기화 한다.
// 2. 반납버튼을 비활성화 한다.
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::Init()
{
EFS_BEGIN

	INT		ids;

	// 1. 컨트롤을 초기화 한다.
	SetDlgItemText(IDC_EDT_UNION_RETURN_REGNO, _T(""));
	SetDlgItemText(IDC_EDT_UNION_RETURN_REGNO_USERINPUT, _T(""));
	SetDlgItemText(IDC_EDT_UNION_RETURN_LIBREGNO, _T(""));
	//SetDlgItemText(IDC_STC_MESSAGE, _T(""));

	CESL_DataMgr* pdmLoanHistory = FindDM(_T("DM_UNION_LOAN_HISTORY"));
	pdmLoanHistory->FreeData();

	// 20071220 add by pwr
	CESL_DataMgr* pdmTempReturnInfo = FindDM(_T("DM_UNION_TEMP_RETURN_INFO"));
	pdmTempReturnInfo->FreeData();

	ids = AllControlDisplay(_T("CM_UNION_RETURN_DLG"));

	m_pmcbReturnLibrary->SetCurSel(0);
	// 20071220 add by pwr
	((CButton*)GetDlgItem(IDC_RADIO_RETURN ))->SetCheck(1);

	// 2. 반납버튼을 비활성화 한다.
	GetDlgItem(IDC_BTN_UNION_RETURN_HISTORY)->EnableWindow(FALSE);	
	GetDlgItem(IDC_BTN_UNION_RETURN_INPUT)->EnableWindow(FALSE);

	// 활성화
	m_edtRegNo.EnableWindow(TRUE);
	m_btnSearch.EnableWindow(TRUE);
	m_edtLibRegNo.EnableWindow(FALSE);
	m_btnSelect.EnableWindow(FALSE);
	m_edtRegNo.SetFocus();
	
	//맴버번수 초기화
	//수동입력 확인
	m_bUserInput = FALSE;
	//도서관번호
	m_RegNo = "";

	return 0;

EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// 입력된 등록번호를 12자리로 만든다.
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::MakeValidRegNo(CString &strGetRegNo)
{
EFS_BEGIN

	// 1. 입력 유무 확인
	if( strGetRegNo.IsEmpty() )
	{
		//AfxMessageBox(_T("반납 대상 자료의 등록번호를 입력해야 합니다"));
		SelfMessageView(_T("반납 대상 자료의 등록번호를 입력해야 합니다"), FALSE);
		return -1001;
	}

	// 12 자리 체크
	INT		nRegNoLength = strGetRegNo.GetLength();

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	if( 12 < nRegNoLength )
	{
		//AfxMessageBox(_T("등록번호는 12자리입니다"));
		//SelfMessageView(_T("등록번호는 12자리입니다"));
		//CBL_LOC_LOAN_PROC::SelfCloseMessageBox(_T("등록번호는 12자리입니다"));
		strGetRegNo.Empty();
		return -1010;
	}
	else if( 12 > nRegNoLength )
	{
		if(1 == nRegNoLength )	return -1020; //20071221 add by pwr

		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		CString strRegCode = strGetRegNo.Mid(0,2);
// 		CString strRegNum  = strGetRegNo.Mid(2);
// 
// 		for( INT i = 0; i < (12-nRegNoLength); i++ )
// 		{
// 			strRegNum = _T("0") + strRegNum;
// 		}
// 
// 		strGetRegNo = strRegCode + strRegNum;

		CString strRegCode = strGetRegNo.Mid(0,nRegCodeLength);
		CString strRegNum  = strGetRegNo.Mid(nRegCodeLength);
		
		strGetRegNo = m_pInfo->MakeRegNo(strRegCode, strRegNum);
		//=====================================================
		
	}
	
	return 0;

EFS_END
return -1;
}

// 20071128 ADD BY PWR ++++++++++++++++++++++++++++++++++++++++++++++++
///////////////////////////////////////////////////////////////////////
// 대출이력 검색결과 선택
// 1. 도서관번호를 가져온다.
// 2. 검색된 대출이력이 있는지 여부확인
// 2.1. 검색된 대출이력정보에 입력한 도서관번호에대한 도서관코드가 존재여부 확인
// 2.2. 해당 자료 선정
// 3. 검색된 대출이력이 없다 <수동입력>
// 3.1. 콤보상자 해당 도서관으로 변경
// 4. <도서관번호 재입력 >
// 4.1. 처음입력내용과 재입력내용의 비교
// 4.2. 수동입력 여부
// 4.3. 초기화   
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::SelectUnionLoanHistory()
{
EFS_BEGIN

	// 1. 도서관번호를 가져온다.
	CString strLibRegNo;
    GetDlgItemText(IDC_EDT_UNION_RETURN_LIBREGNO, strLibRegNo);

	if( strLibRegNo.IsEmpty() )
	{
		//AfxMessageBox(_T("반납 대상 자료의 도서관번호를 입력해야 합니다"));
		SelfMessageView(_T("반납 대상 자료의 도서관번호를 입력해야 합니다"), FALSE);
		return -1001;
	}
	
	// 4. <도서관번호 재입력 >
	// 4.1. 처음입력내용과 재입력내용의 비교
	if(m_RegNo == strLibRegNo)
	{
		// 4.2. 수동입력 여부
		if(!m_bUserInput)
		{
			//자동입력
			OnBtnUnionReturnHistory();				
		}
		else
		{
			//수동입력
			OnBtnUnionReturnInput();				
		}
		// 4.3. 초기화
		Init();
		return 0;
	}
	else
	{
		//처음입력
		m_RegNo = strLibRegNo;
	}

	CESL_DataMgr* pdmLoanHistory = FindDM(_T("DM_UNION_LOAN_HISTORY"));
	BOOL bResult = FALSE;

	// 2. 검색된 대출이력이 있는지 여부확인.
	if(0 < pdmLoanHistory->GetRowCount())
	{
		// 2.1. 검색된 대출이력정보에 입력한 도서관번호에대한 도서관코드가 존재여부 확인
		for(int i = 0; i < pdmLoanHistory->GetRowCount(); i++)
		{
			CString strLibCode; //대출이력정보의 도서관 코드

			pdmLoanHistory->GetCellData("도서관코드", i, strLibCode);

			if(strLibCode == strLibRegNo)
			{
				// 2.2. 해당 자료 선정
				CESL_Grid* pgrid = (CESL_Grid*)FindControl(_T("CM_UNION_RETURN_DLG"), _T("통합대출이력그리드"));
				pgrid->SetAt(i, pgrid->m_nSelectIdx, pgrid->m_sSelectString);

				SetDlgItemText(IDC_EDT_UNION_RETURN_LIBREGNO, _T(""));
				SetDlgItemText(IDC_STC_MESSAGE, _T(""));
				m_bUserInput = FALSE;
				bResult = TRUE;
				break;
			}
		}
	}

	if(!bResult)
	{
		SelfMessageView(_T("입력한 도서관번호에 해당하는 도서관이 존재하지 않습니다"), FALSE);
		SetDlgItemText(IDC_EDT_UNION_RETURN_LIBREGNO, _T(""));
		return -1002;
	}

	// 3. 검색된 대출이력이 없다 <수동입력>
	CESL_DataMgr* pdmReturnLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));

	CString strCode;
	INT		ids;

	for( INT i = 0; i < pdmReturnLibrary->GetRowCount(); i++ )
	{		
		ids = pdmReturnLibrary->GetCellData( _T("도서관코드"), i, strCode );
		if( 0 > ids ) return ids;
			
		if(strCode == strLibRegNo)
		{
			// 3.1. 콤보상자 해당 도서관으로 변경
			m_pmcbReturnLibrary->SetCurSel(i+1);
			SetDlgItemText(IDC_EDT_UNION_RETURN_LIBREGNO, _T(""));
			m_bUserInput = TRUE;
			break;
		}
	}

	return 0;

EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// 통합대출이력 검색
// 1. 등록번호를 가져온다.
// 2. 통합대출이력 검색 쿼리를 만든다.
// 3. 통합대출이력 검색 
// 4. 검색 결과 화면 출력
// 5. 마무리
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::SearchUnionLoanHistory()
{
EFS_BEGIN

	INT		ids;

	// 1. 등록번호를 가져온다.	
	CString strReturnBookRegNo;
    GetDlgItemText(IDC_EDT_UNION_RETURN_REGNO, strReturnBookRegNo);

	if( strReturnBookRegNo.IsEmpty() )
	{
		//AfxMessageBox(_T("반납 대상 자료의 등록번호를 입력해야 합니다"));
		SelfMessageView(_T("반납 대상 자료의 등록번호를 입력해야 합니다"), FALSE);
		return -1001;
	}

	Init();

	ids = MakeValidRegNo(strReturnBookRegNo);
	if( 0 > ids )	return -1010;

	// 2. 통합대출이력 검색 쿼리를 만든다.
	CString strQuery;
	strQuery.Format(_T("CCL.REG_NO='%s'"), strReturnBookRegNo);

	// 3. 통합대출이력 검색 	
	CRS_SQL		  RemoteDBMgr(this);
	CESL_DataMgr* pdmLoanHistory = FindDM(_T("DM_UNION_LOAN_HISTORY"));

//	ids = RemoteDBMgr.SetFetchCount(0, 100);
	if( 0 > ids ) return ids;
//	ids = RemoteDBMgr.LOAN_Select(pdmLoanHistory, strQuery, _T("DM_UNION_LOAN_HISTORY"), 1, 1);
	if( 0 > ids ) return ids;

	// 4. 검색 결과 화면 출력	
	ids = AllControlDisplay(_T("CM_UNION_RETURN_DLG"));
	if( 0 > ids ) return ids;

	// 5. 마무리
	SetDlgItemText(IDC_EDT_UNION_RETURN_REGNO, _T(""));

	if( 0 < pdmLoanHistory->GetRowCount() )
	{
		if( 1 == pdmLoanHistory->GetRowCount() )
		{
			CString strLibCode;
			CString strCode;
			INT		ids;

			pdmLoanHistory->GetCellData("도서관코드", 0, strLibCode);

			CESL_DataMgr* pdmReturnLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));
			
			for( INT i = 0; i < pdmReturnLibrary->GetRowCount(); i++ )
			{		
				ids = pdmReturnLibrary->GetCellData( _T("도서관코드"), i, strCode );
				if( 0 > ids ) return ids;
				
				if(strCode == strLibCode)
				{
					//콤보상자 해당 도서관으로 변경
					m_pmcbReturnLibrary->SetCurSel(i+1);
					SetDlgItemText(IDC_EDT_UNION_RETURN_LIBREGNO, _T(""));
					m_bUserInput = TRUE;
					break;
				}
			}

			CESL_Grid* pgrid = (CESL_Grid*)FindControl(_T("CM_UNION_RETURN_DLG"), _T("통합대출이력그리드"));
			pgrid->SetAt(0, pgrid->m_nSelectIdx, pgrid->m_sSelectString);

			m_RegNo = strLibCode;
		}
		//AfxMessageBox(_T("대출 이력이 검색 되었습니다"));
		SelfMessageView(_T("대출 이력이 검색 되었습니다"), TRUE);

		GetDlgItem(IDC_BTN_UNION_RETURN_HISTORY)->EnableWindow();		
		GetDlgItem(IDC_BTN_UNION_RETURN_INPUT)->EnableWindow();

		// 20071128 ADD BY PWR ++++++++++++++++++++++++++++++++++++++++++
		// 등록번호로 검색이 되면 등록번호 에디트박스와 검색버튼 비활성화
		// 도서관번호 에디트박스와 선택버튼 활성화하면서
		// 도서관번호 에디트박스에 포커스이동
		m_edtRegNo.EnableWindow(FALSE);
		m_btnSearch.EnableWindow(FALSE);
		m_edtLibRegNo.EnableWindow(TRUE);
		m_btnSelect.EnableWindow(TRUE);
		m_edtLibRegNo.SetFocus();
		// --------------------------------------------------------------
	}
	else
	{
		//AfxMessageBox(_T("검색된 대출 이력이 없습니다"));
		SelfMessageView(_T("검색된 대출 이력이 없습니다"), FALSE);

		GetDlgItem(IDC_BTN_UNION_RETURN_INPUT)->EnableWindow();

		// 20071128 ADD BY PWR ++++++++++++++++++++++++++++++++++++++++++
		// 대출이력이 없으면 등록번호 에디트박스와 검색버튼 비활성화
		// 도서관번호 에디트박스와 선택버튼 활성화하면서
		// 도서관번호 에디트박스에 포커스이동
		m_edtRegNo.EnableWindow(FALSE);
		m_btnSearch.EnableWindow(FALSE);
		m_edtLibRegNo.EnableWindow(TRUE);
		m_btnSelect.EnableWindow(TRUE);
		m_edtLibRegNo.SetFocus();
		// --------------------------------------------------------------
	}

	SetDlgItemText(IDC_EDT_UNION_RETURN_REGNO_USERINPUT, strReturnBookRegNo);

	return 0;

EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// 통합대출이력 타관 반납
// 1. 검색결과가 유무 확인
// 2. 대출이력 그리드에서 선택값 확인
// 3. 도서관 코드를 가져온다.
// 4. 등록번호를 가져온다.
// 5. 통합대출자서버에 쿼리 전송
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::ReturnUnionLoanHistory()
{
EFS_BEGIN

	INT		ids;

	// 1. 검색결과가 유무 확인
	CESL_DataMgr* pdmLoanHistory = FindDM(_T("DM_UNION_LOAN_HISTORY"));
	if( 0 == pdmLoanHistory->GetRowCount() )
	{
		//AfxMessageBox(_T("통합대출이력을 검색해 주십시오"));
		SelfMessageView(_T("통합대출이력을 검색해 주십시오"), FALSE);
		return -1000;
	}

	// 2. 대출이력 그리드에서 선택값 확인
	CESL_Grid* pgrid = (CESL_Grid*)FindControl(_T("CM_UNION_RETURN_DLG"), _T("통합대출이력그리드"));		
	pgrid->SelectMakeList();
	
	INT		nSelectCount = pgrid->SelectGetCount();
	if( 0 == nSelectCount )
	{
		//AfxMessageBox(_T("반납 대상 이력을 선택해 주십시오"));
		SelfMessageView(_T("반납 대상 이력을 선택해 주십시오"), FALSE);
		return -1010;
	}
	else if( 1 < nSelectCount )
	{
		//AfxMessageBox(_T("한개의 반납 대상 이력만 선택해 주십시오"));
		SelfMessageView(_T("한개의 반납 대상 이력만 선택해 주십시오"), FALSE);
		pgrid->UnSelectAll();
		return -1020;
	}

	// 3. 도서관 코드를 가져온다.
	pgrid->SelectGetHeadPosition();
	INT		nSelectIdx = pgrid->SelectGetIdx();

	CString strReturnLibraryCode;
	ids = pdmLoanHistory->GetCellData(_T("도서관코드"), nSelectIdx, strReturnLibraryCode);
	if( 0 > ids ) return -1030;

	// 4. 등록번호를 가져온다.
	CString strReturnBookRegNo;
	ids = pdmLoanHistory->GetCellData(_T("등록번호"), nSelectIdx, strReturnBookRegNo);
	if( 0 > ids )	return -1040;

	// 5. 통합대출자서버에 쿼리 전송
	ids = SendUnionReturnInfo(strReturnLibraryCode, strReturnBookRegNo);
	if( 0 > ids )	return -1050;

	return 0;

EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// 사용자 입력 타관 반납
// 1. 콤보박스에서 도서관 코드를 가져온다.
// 2. 등록번호를 가져온다.
// 3. 타관반납요청 쿼리를 만든다.
// 4. 통합대출자서버에 쿼리 전송
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::ReturnUnionUserInput()
{
EFS_BEGIN

	INT		ids;

	// 1. 콤보박스에서 도서관 코드를 가져온다.
	CString strReturnLibraryCode;
	ids = m_pmcbReturnLibrary->GetData(strReturnLibraryCode);
	
	strReturnLibraryCode.TrimLeft();
	strReturnLibraryCode.TrimRight();

	if( strReturnLibraryCode.IsEmpty() )
	{
		//AfxMessageBox(_T("반납 도서관을 선택해야 합니다"));
		SelfMessageView(_T("반납 도서관을 선택해야 합니다"), FALSE);
		return -1010;
	}

	// 2. 등록번호를 가져온다.
	CString strReturnBookRegNo;
	GetDlgItemText(IDC_EDT_UNION_RETURN_REGNO_USERINPUT, strReturnBookRegNo);

	if( strReturnBookRegNo.IsEmpty() )
	{
		//AfxMessageBox(_T("반납 대상 자료의 등록번호를 입력해야 합니다"));
		SelfMessageView(_T("반납 대상 자료의 등록번호를 입력해야 합니다"), FALSE);
		return -1001;
	}

	ids = MakeValidRegNo(strReturnBookRegNo);
	if( 0 > ids )	return -1020;

	// 3. 타관반납요청 쿼리를 만든다.
	// 4. 통합대출자서버에 쿼리 전송
	ids = SendUnionReturnInfo(strReturnLibraryCode, strReturnBookRegNo);
	if( 0 > ids )	return -1030;

	return 0;

EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// 타관 반납 정보를 통합대출자서버에 전송한다.
// 1. 타관반납요청 쿼리를 만든다.
// 2. 통합대출자서버에 쿼리 전송
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::SendUnionReturnInfo(CString strReturnLibraryCode, CString strReturnBookRegNo)
{
EFS_BEGIN
	
	INT		ids;

	// 1. 타관반납요청 쿼리를 만든다.	
	// 로그를 생성한다.	
	CString strWorkLog;
	strWorkLog = GetWorkLogMsg( _T("타관자료반납신청"), __FILE__, __LINE__ );	

	// MAKE QUERY
	CString strQuery;
	strQuery.Format(_T("INSERT INTO CT_TEMP_RETURN_INFO_TBL(REC_KEY, REQUEST_LIB_CODE, UPDATE_LIB_CODE, REG_NO, REQUEST_DATE, STATUS, FIRST_WORK) ")
					_T("VALUES(ESL_SEQ.NEXTVAL, '%s', '%s', '%s', SYSDATE, '0', '%s');"), m_strLibraryCode, strReturnLibraryCode, strReturnBookRegNo, strWorkLog );
	
	// 2. 통합대출자서버에 쿼리 전송	
	CRS_SQL RemoteDBMgr( this );
//	ids = RemoteDBMgr.LOAN_UnSelect( strQuery );
	if( 0 > ids )
	{
		//AfxMessageBox( _T("통합대출자서버 쿼리 전송 오류!!") );
		SelfMessageView(_T("통합대출자서버 쿼리 전송 오류!! (1)"), FALSE);
		return ids;
	}

	///{{++
	/// 2008.05.02 add by pdw
	/// 타관반납시 이용자 정보의 대출정지 설정을 미리 해준다
	/// 대상 이력을 미리 update 하여 통합대출책수 제한 기능과 연계될수 있도록한다
	/// 1. 이용자 정보를 업데이트 한다	
	/*
UPDATE CT_LIBRARY_LOAN_USER_TBL@K2UPLIS
   SET USER_CLASS = DECODE( USER_CLASS, 0, 
						   DECODE( (SELECT SIGN(TO_DATE(RETURN_PLAN_DATE)-TO_DATE(SYSDATE)) 
									  FROM CT_CO_LOAN_INFO_TBL@K2UPLIS 
									 WHERE STATUS IN ('0','2') 
									   AND LIB_CODE='121007' AND REG_NO='EM0000131513'), -1, 1, 0), USER_CLASS)
 WHERE MASTER_USER_KEY = ( SELECT MASTER_USER_KEY 
                             FROM CT_CO_LOAN_INFO_TBL@K2UPLIS 
							WHERE STATUS IN ('0','2') 
                              AND LIB_CODE='121007' AND REG_NO='EM0000131513' )
   AND LIB_CODE = '121007';   
	*/
	strQuery.Format(_T("UPDATE CT_LIBRARY_LOAN_USER_TBL ")
		              _T(" SET INFERIOR_CLASS = DECODE( INFERIOR_CLASS, 0, ")
					                          _T(" DECODE( (SELECT SIGN(TO_DATE(RETURN_PLAN_DATE)-TO_DATE(SYSDATE)) ")
						        			             _T(" FROM CT_CO_LOAN_INFO_TBL ")
													    _T(" WHERE STATUS IN ('0','2') ")
   													      _T(" AND LIB_CODE='%s' AND REG_NO='%s'), -1, 1, 0), INFERIOR_CLASS) ")
                       _T(" WHERE MASTER_USER_KEY = ( SELECT MASTER_USER_KEY ")
					                            _T(" FROM CT_CO_LOAN_INFO_TBL ")
											   _T(" WHERE STATUS IN ('0','2') ")
											     _T(" AND LIB_CODE='%s' AND REG_NO='%s' ) ")
	                  _T(" AND LIB_CODE = '%s';"), strReturnLibraryCode, strReturnBookRegNo, 
												  strReturnLibraryCode, strReturnBookRegNo, strReturnLibraryCode);
//	ids = RemoteDBMgr.LOAN_UnSelect( strQuery );
	if( 0 > ids )
	{
		//AfxMessageBox( _T("통합대출자서버 쿼리 전송 오류!!") );
		SelfMessageView(_T("통합대출자서버 쿼리 전송 오류!! (2)"), FALSE);
		return ids;
	}
	/// 2. 대출이력 정보를 업데이트 한다
	/*
UPDATE CT_CO_LOAN_INFO_TBL@K2UPLIS 
   SET STATUS = '7', RETURN_DATE = SYSDATE
 WHERE STATUS IN ('0','2') AND LIB_CODE='121007' AND REG_NO='EM0000131513'  
	*/
	strQuery.Format(_T("UPDATE CT_CO_LOAN_INFO_TBL ")
		              _T(" SET STATUS = '7', RETURN_DATE = SYSDATE ")
				    _T(" WHERE STATUS IN ('0','2') AND LIB_CODE='%s' AND REG_NO='%s'; "),
					strReturnLibraryCode, strReturnBookRegNo);
//	ids = RemoteDBMgr.LOAN_UnSelect( strQuery );
	if( 0 > ids )
	{
		//AfxMessageBox( _T("통합대출자서버 쿼리 전송 오류!!") );
		SelfMessageView(_T("통합대출자서버 쿼리 전송 오류!! (3)"), FALSE);
		return ids;
	}
	///}}--

	if( ((CButton*)GetDlgItem(IDC_CHK_HOLD))->GetCheck() == 0 )
		SelfMessageView(_T("반납 신청되었습니다"), TRUE);
	else
	{
		m_nReturnCode = 10000;
		OnOK();
	}

	Init();

	return 0;

EFS_END
return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071220 add by pwr 
// ** 컨트롤 SHOW/HIDE
void CUnionReturnDlg::SetControlShow(BOOL bShow)
{
EFS_BEGIN
	if(!bShow)
	{
		((CButton*)GetDlgItem(IDC_BTN_UNION_RETURN_INPUT))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDT_UNION_RETURN_REGNO_USERINPUT))->ShowWindow(SW_HIDE);
		((CComboBox*)GetDlgItem(IDC_CMB_UNION_RETURN_LIBLIST))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_STC_REGNO))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_STC_LIB))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_STC_HANDOPERATED))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_STC_LIBNO))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_STC_LIBNO2))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDT_UNION_RETURN_LIBREGNO))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_BTN_UNION_SELECT))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_EDT_UNION_RETURN_REGNO))->ShowWindow(SW_SHOW);
		((CMSHFlexGrid*)GetDlgItem(IDC_GRD_LOAN_HISTORY))->ShowWindow(SW_HIDE);
		((CMSHFlexGrid*)GetDlgItem(IDC_GRD_RETURN_HISTORY))->ShowWindow(SW_SHOW);		
		((CButton*)GetDlgItem(IDC_BTN_UNION_SEARCH))->SetWindowText(_T("선 택"));
		((CButton*)GetDlgItem(IDC_BTN_UNION_RETURN_HISTORY))->SetWindowText(_T("반납취소"));
		((CButton*)GetDlgItem(IDC_CHK_HOLD))->SetWindowText(_T("반납취소후 화면닫기"));
		SetDlgItemText(IDC_STC_MESSAGE, _T(""));
		GetDlgItem(IDC_BTN_UNION_RETURN_HISTORY)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_UNION_RETURN_REGNO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_UNION_SEARCH)->EnableWindow(TRUE);
		SetContolPos(bShow);	
	}
	else
	{
		Init();
		((CButton*)GetDlgItem(IDC_BTN_UNION_RETURN_INPUT))->ShowWindow(SW_SHOW);
		((CEdit*)GetDlgItem(IDC_EDT_UNION_RETURN_REGNO_USERINPUT))->ShowWindow(SW_SHOW);
		((CComboBox*)GetDlgItem(IDC_CMB_UNION_RETURN_LIBLIST))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_STC_REGNO))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_STC_LIB))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_STC_HANDOPERATED))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_STC_LIBNO))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_STC_LIBNO2))->ShowWindow(SW_SHOW);
		((CEdit*)GetDlgItem(IDC_EDT_UNION_RETURN_LIBREGNO))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_BTN_UNION_SELECT))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_EDT_UNION_RETURN_REGNO))->ShowWindow(SW_SHOW);
		((CMSHFlexGrid*)GetDlgItem(IDC_GRD_LOAN_HISTORY))->ShowWindow(SW_SHOW);
		((CMSHFlexGrid*)GetDlgItem(IDC_GRD_RETURN_HISTORY))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_BTN_UNION_SEARCH))->SetWindowText(_T("검 색"));
		((CButton*)GetDlgItem(IDC_BTN_UNION_RETURN_HISTORY))->SetWindowText(_T("타관반납"));
		((CButton*)GetDlgItem(IDC_CHK_HOLD))->SetWindowText(_T("타관반납후 화면닫기"));
		SetDlgItemText(IDC_STC_MESSAGE, _T(""));
		SetContolPos(bShow);
	}
EFS_END
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr 
// ** 메시지뷰에 메시지를 보여준다.
void CUnionReturnDlg::SelfMessageView(CString sMsg, BOOL bStatus)
{
EFS_BEGIN

	//색바꾸는거 추가 예정
	if(bStatus)
		m_bStatus = TRUE;
	else
		m_bStatus = FALSE;

//	CStatic* stcMsg = (CStatic*)GetDlgItem();
	SetDlgItemText(IDC_STC_MESSAGE,"  " + sMsg);
//	stcMsg->SetWindowText();

	UpdateWindow();	

	//자동 삭제 예정

EFS_END
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr 
// **등록번호의 입력이 들어와 처음입력값과 두번째 입력값이 같으면 해당 자료 반납취소처리한다.
// 1. 등록번호를 가져온다.	
// 2. 등록번호 유효한지를 검사
// 3. 등록번호를 12자리로 만들어준다.
// 4. 처음입력하는것인지 확인인지를 검사
//		( 처음입력값과 두번째 입력값이 같으면 반납취소처리, 틀리면 새로운선택(처음입력) )
//	- 4.1. 반납취소처리 실행
//	- 4.2. 새로입력(처음입력)
// 5. 입력값에 해당하는 데이타 선정
// 6. 선정값이 한개인지를 검사
// 7. 선정된 등록번호를 전역번수에 저장 (확인할때 비교위해)
INT CUnionReturnDlg::SearchUnionReturnHistory()
{
EFS_BEGIN

	INT		ids;

	// 1. 등록번호를 가져온다.	
	CString strReturnBookRegNo;
    GetDlgItemText(IDC_EDT_UNION_RETURN_REGNO, strReturnBookRegNo);
	SetDlgItemText(IDC_EDT_UNION_RETURN_REGNO, _T(""));

	// 2. 등록번호 유효한지를 검사
	if( strReturnBookRegNo.IsEmpty() )
	{
		SelfMessageView(_T(" 반납 대상 자료의 등록번호를 입력해야 합니다"), FALSE);
		return -1001;
	}

	// 3. 등록번호를 12자리로 만들어준다.
	ids = MakeValidRegNo(strReturnBookRegNo);
	if( 0 > ids )	return -1010;

	// 4. 처음입력하는것인지 확인인지를 검사
	//    ( 처음입력값과 두번째 입력값이 같으면 반납취소처리, 틀리면 새로입력(처음입력) )
	if( m_ReturnRegNo == strReturnBookRegNo)
	{
		// 4.1 반납취소처리 실행
		ReturnUnionReturnHistory();
		return 0;
	}
	else
	{
		// 4.2 새로입력(처음입력)
		ControlDisplay(_T("CM_UNION_RETURN_DLG"), _T("타관반납이력그리드"));
	}

	// 5. 입력값에 해당하는 데이타 선정
	int nCheck = 0;
	for(int i = 0; i < m_pDMReturnHistory->GetRowCount(); i++)
	{
		CString strRegNo;
		m_pDMReturnHistory->GetCellData( _T("등록번호"), i, strRegNo );
		if(strReturnBookRegNo == strRegNo)
		{
			m_pReturnHistoryGrid->SetAt(i, m_pReturnHistoryGrid->m_nSelectIdx, 
											m_pReturnHistoryGrid->m_sSelectString);
			nCheck++;
		}
	}

	// 6. 선정값이 한개인지를 검사
	if( nCheck > 1 )
	{
		SelfMessageView(_T(" 검색된 이력이 두개이상 입니다. 직접 선택해 주세요"), FALSE);
		ControlDisplay(_T("CM_UNION_RETURN_DLG"), _T("타관반납이력그리드"));
		return -1020;
	}

	// 7. 선정된 등록번호를 전역번수에 저장 (확인할때 비교위해)
	m_ReturnRegNo = strReturnBookRegNo;

	return 0;

EFS_END
return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr
// **반납취소처리 실행
// 1. 대상도서관코드를 가져온다.
// 2. 등록번호를 가져온다.
// 3. rec_key를 가져온다.
// 4. 통합대출자서버에 쿼리 전송
INT CUnionReturnDlg::ReturnUnionReturnHistory()
{
EFS_BEGIN

	INT	ids;
	
	m_pReturnHistoryGrid->SelectMakeList();
	m_pReturnHistoryGrid->SelectGetHeadPosition();
	INT	nSelectIdx = m_pReturnHistoryGrid->SelectGetIdx();

	INT	nSelectCount = m_pReturnHistoryGrid->SelectGetCount();

	if( 0 > nSelectCount )
	{
		SelfMessageView(_T(" 반납취소 대상 이력을 선택해 주십시오"), FALSE);
		return -1010;
	}
	else if( 1 < nSelectCount )
	{
		SelfMessageView(_T(" 한개의 반납취소 대상 이력만 선택해 주십시오"), FALSE);
		m_pReturnHistoryGrid->UnSelectAll();
		return -1020;
	}

	// 1. 대상도서관코드를 가져온다.
	CString strReturnLibraryCode;
	ids = m_pDMReturnHistory->GetCellData(_T("대상도서관코드"), nSelectIdx, strReturnLibraryCode);
	if( 0 > ids ) return -1030;

	// 2. 등록번호를 가져온다.
	CString strReturnBookRegNo;
	ids = m_pDMReturnHistory->GetCellData(_T("등록번호"), nSelectIdx, strReturnBookRegNo);
	if( 0 > ids )	return -1040;

	// 3. rec_key를 가져온다.
	CString strRecKey;
	ids = m_pDMReturnHistory->GetCellData(_T("REC_KEY"), nSelectIdx, strRecKey);

	// 4. 통합대출자서버에 쿼리 전송
	ids = SendUnionReturnCancelInfo(strReturnLibraryCode, strReturnBookRegNo, strRecKey);
	if( 0 > ids )	return -1050;

	ControlDisplay(_T("CM_UNION_RETURN_DLG"), _T("타관반납이력그리드"));

	return 0;

EFS_END
return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr
// ** 반납취소처리 라디오버튼 선택
void CUnionReturnDlg::OnRadioReturnCancel() 
{
EFS_BEGIN

	// 1. 컨트롤 처리
	SetControlShow(FALSE);

	// 5. 등록번호 에디트박스로 포커스 이동
	((CEdit*)GetDlgItem(IDC_EDT_UNION_RETURN_REGNO))->SetFocus();

	INT ids;
	// 2. 통합대출이력 검색 	
	CRS_SQL		  RemoteDBMgr(this);
	m_pDMReturnHistory = FindDM(_T("DM_UNION_TEMP_RETURN_INFO"));
	m_pReturnHistoryGrid = (CESL_Grid*)FindControl(_T("CM_UNION_RETURN_DLG"), _T("타관반납이력그리드"));
	
//	ids = RemoteDBMgr.SetFetchCount(0, 100);
	if( 0 > ids ) SelfMessageView(_T(" ERROR!! : RS_SQL - FetchCount"), FALSE);
	
	// 일주일 데이터만을 가져오기위해 조건생성
	CString sQuery;
	GetWeekQuery(sQuery);

//	ids = RemoteDBMgr.LOAN_Select(m_pDMReturnHistory, sQuery, _T("DM_UNION_TEMP_RETURN_INFO"), 1, 1);
	if( 0 > ids ) SelfMessageView(_T(" ERROR!! : RS_SQL - Select"), FALSE);
	
	// 3. 검색 결과 화면 출력
	ids = ControlDisplay("CM_UNION_RETURN_DLG", "타관반납이력그리드");
	if( 0 > ids ) SelfMessageView(_T(" ERROR!! : Grid Display"), FALSE);
	
	// 4. 검색결과 유무 확인
	if( 0 == m_pDMReturnHistory->GetRowCount() )
	{
		SelfMessageView(_T(" 일주일동안 검색된 반납이력이 없습니다."), FALSE);
	}

EFS_END
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr 
// ** 반납처리 라디오버튼 선택
void CUnionReturnDlg::OnRadioReturn() 
{
EFS_BEGIN

	//1. 컨트롤 처리
	SetControlShow(TRUE);
	
	//2. 등록번호 에디트박스로 포커스 이동
	((CEdit*)GetDlgItem(IDC_EDT_UNION_RETURN_REGNO))->SetFocus();

EFS_END
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr 
// ** 반납취소 정보를 통합대출자서버에 전송한다.
// 1. 반납취소요청 쿼리를 만든다.
// 2. 통합대출자서버에 쿼리 전송
INT CUnionReturnDlg::SendUnionReturnCancelInfo(CString strReturnLibraryCode, CString strReturnBookRegNo, CString strRecKey)
{
EFS_BEGIN
	
	INT		ids;

	// 1. 반납취소요청 쿼리를 만든다.	
	// 로그를 생성한다.	
	CString strWorkLog;
	strWorkLog = GetWorkLogMsg( _T("타관자료반납취소신청"), __FILE__, __LINE__ );	

	// MAKE QUERY
	CString strQuery;
	strQuery.Format(_T("INSERT INTO CT_TEMP_RETURN_INFO_TBL(REC_KEY, REQUEST_LIB_CODE, UPDATE_LIB_CODE, REG_NO, REQUEST_DATE, STATUS, CANCEL_DES_REC_KEY, FIRST_WORK) ")
					_T("VALUES(ESL_SEQ.NEXTVAL, '%s', '%s', '%s', SYSDATE, '2', '%s', '%s');"), m_strLibraryCode, strReturnLibraryCode, strReturnBookRegNo, strRecKey, strWorkLog );
	
	// 2. 통합대출자서버에 쿼리 전송	
	CRS_SQL RemoteDBMgr( this );
//	ids = RemoteDBMgr.LOAN_UnSelect( strQuery );
	if( 0 > ids )
	{
		SelfMessageView(_T(" 통합대출자서버 쿼리 전송 오류!! (1)"), FALSE);
		return ids;
	}
	///{{++
	/// 2008.05.02 add by pdw
	/// 타관반납시 이용자 정보의 대출정지 설정을 미리 해준다
	/// 대상 이력을 미리 update 하여 통합대출책수 제한 기능과 연계될수 있도록한다
	/// 1. 이용자정보 업데이트
	///    연체자료를 반납할 경우 정상일 경우만 대출정지로 변경되니
	///    반대일 경우 연체자료를 가지고있다고 판단된다
	///    따라서 이용자정보는 업데이트 할 필요가 없다
	/// 2. 대출이력정보 업데이트
	/*
UPDATE CT_CO_LOAN_INFO_TBL@K2UPLIS 
   SET STATUS = DECODE(NVL(DELAY_CNT,0), 0, '0', '2');
       RETURN_DATE = NULL
 WHERE STATUS='7' AND LIB_CODE='121007' AND REG_NO='EM0000131513'
	*/
	strQuery.Format(_T("UPDATE CT_CO_LOAN_INFO_TBL ")
					  _T(" SET STATUS=DECODE(NVL(DELAY_CNT,0),0,'0','2'), RETURN_DATE=NULL ")
					_T(" WHERE STATUS='7' AND LIB_CODE='%s' AND REG_NO='%s'; "), 
					strReturnLibraryCode, strReturnBookRegNo);
//	ids = RemoteDBMgr.LOAN_UnSelect( strQuery );
	if( 0 > ids )
	{
		SelfMessageView(_T(" 통합대출자서버 쿼리 전송 오류!! (2)"), FALSE);
		return ids;
	}
	///}}--

	if( ((CButton*)GetDlgItem(IDC_CHK_HOLD))->GetCheck() == 0 )
	{
		OnRadioReturnCancel();
		SelfMessageView(_T(" 반납취소 신청 되었습니다."), TRUE);
	}
	else
	{
		m_nReturnCode = 20000;
		OnOK();
	}

	return 0;

EFS_END
return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr
// ** 조건문쿼리 생성 (현재날짜 기준으로 일주일전까지)
void CUnionReturnDlg::GetWeekQuery(CString& sQuery)
{
	CString sDay1;
	CString sDay2;

	// 2010.08.27 UPDATE BY PWR : 로그인시 기록한 현재일자 사용
	CString strGetData;
	CTime tSysdate = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
// 	CTime tSysdate = CLocCommonAPI::GetDBTime(this, NULL);
	CTimeSpan tExpDay( 6 , 0 , 0 , 0 );
	
	sDay1.Format(_T("%s"),tSysdate.Format(_T("%Y/%m/%d")));
	tSysdate -= tExpDay;
	sDay2.Format(_T("%s"),tSysdate.Format(_T("%Y/%m/%d")));

	sQuery.Format(_T("REQUEST_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sDay2, sDay1);
}

void CUnionReturnDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071226 add by pwr 
// ** 컨트롤의 위치와 다이얼로그의 크기를 바꾼다
void CUnionReturnDlg::SetContolPos(BOOL bCheck)
{
	INT nX		= m_rect.left;
	INT nY		= m_rect.top;
	INT nWidth	= m_rect.Width();
	INT nHeight = m_rect.Height()-80;

	CButton* pChkAutoClose = (CButton*)GetDlgItem(IDC_CHK_HOLD);
	CButton* pBtnClear = (CButton*)GetDlgItem(IDC_BTN_UNION_RETURN_CLEAR);
	CButton* pBtnClose = (CButton*)GetDlgItem(IDC_BTN_UNION_RETURN_CLOSE);

	CRect rChk;
	CRect rClear;
	CRect rClose;

	pChkAutoClose->GetWindowRect(rChk);	//채크박스
	pBtnClear->GetWindowRect(rClear);	//초기화버튼
	pBtnClose->GetWindowRect(rClose);	//닫기버튼

	if(!bCheck)
	{
		MoveWindow(nX, nY, nWidth, nHeight);

		pChkAutoClose->MoveWindow(10, nHeight-60, rChk.Width(), rChk.Height());
		pBtnClear->MoveWindow(m_rect.Width()-200, nHeight-70, rClear.Width(), rClear.Height());
		pBtnClose->MoveWindow(m_rect.Width()-110, nHeight-70, rClose.Width(), rClose.Height());
	}
	else
	{
		MoveWindow(nX, nY, nWidth, nHeight + 80);

		pChkAutoClose->MoveWindow(10, nHeight+20, rChk.Width(), rChk.Height());
		pBtnClear->MoveWindow(m_rect.Width()-200, nHeight+10, rClear.Width(), rClear.Height());
		pBtnClose->MoveWindow(m_rect.Width()-110, nHeight+10, rClose.Width(), rClose.Height());
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071228 add by pwr 
// ** 컨트롤의 색설정
HBRUSH CUnionReturnDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// 메시지뷰의 글씨색을 빨간색으로 설정
	if (pWnd->GetDlgCtrlID() == IDC_STC_MESSAGE)
	{
		if(m_bStatus)
			pDC->SetTextColor(RGB(0, 0, 0));
		else
			pDC->SetTextColor(RGB(255, 0, 0));
	}
	
	return hbr; 
}
