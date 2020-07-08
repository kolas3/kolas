// SearchMono.cpp : implementation file
//

#include "stdafx.h"
//#include "IndexGenerator.h"
#include "SearchMono.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchMono dialog
CSearchMono::CSearchMono(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchMono)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentMgr = pParent;
	m_pCM			= NULL;
//	m_bShowMode = FALSE;
	m_bShowMode = true;
	m_Create = FALSE;
	m_bRecycleData	= FALSE;

//	m_pWorkCodeComboBox = NULL;
//	m_pAcqCodeComboBox = NULL;

	m_pgridIndexGenerator = NULL;
	m_pIndexGeneratorDm = NULL;

	m_pDataProcessingDlg = NULL;

	m_Dlg_brush.CreateSolidBrush(RGB(244,243,238));
}
CSearchMono::~CSearchMono()
{
//	if( NULL != m_pWorkCodeComboBox )
//	{
//		delete m_pWorkCodeComboBox;
//		m_pWorkCodeComboBox = NULL;
//	}
//
//	if( NULL != m_pAcqCodeComboBox )
//	{
//		delete m_pAcqCodeComboBox;
//		m_pAcqCodeComboBox = NULL;
//	}

	if( NULL != m_pIndexTool )
	{
		delete m_pIndexTool;
		m_pIndexTool = NULL;
	}

	m_Dlg_brush.DeleteObject();
}

void CSearchMono::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchMono)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSearchMono, CDialog)
	//{{AFX_MSG_MAP(CSearchMono)
	ON_CBN_SELCHANGE(IDC_cmbSEARCH_CLASS, OnSelchangecmbSEARCHCLASS)
	ON_BN_CLICKED(IDC_btnBASIC_CLEAR, OnbtnBASICCLEAR)
	ON_BN_CLICKED(IDC_btnMonoClose, OnbtnMonoClose)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnCreateMonoIndex, OnbtnCreateMonoIndex)
	ON_BN_CLICKED(IDC_btnMonoIndexInfoView, OnbtnMonoIndexInfoView)	
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchMono message handlers
BOOL CSearchMono::Create(CWnd *pParentWnd)
{
	m_Create = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

//##ModelId=409EC8DD0173
BOOL CSearchMono::Init()
{
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("검색구분"));
	if( pCMB == NULL )	return FALSE;
	pCMB->ResetContent();
	pCMB->AddString(_T("구입차수검색"));
	pCMB->AddString(_T("기증접수번호검색"));
	pCMB->AddString(_T("수서그룹번호검색"));
	//===================================================
	//2010.06.25 ADD BY PJW : 색인 재생성에서 재활용자료는 제외 한다.
//	pCMB->AddString(_T("재활용자료검색"));	// 2005-12-08 추가(구로도서관) 
	//===================================================
	pCMB->AddString(_T("원부번호검색"));
	
	pCMB->SetCurSel(0);

	ShowCtrlBySearchClass();
	
	return TRUE;
}


BOOL CSearchMono::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pControlDm.SetCONNECTION_INFO(m_strConnectInfo);
	
	SetParentInfo();
	LoadCodeInfo();

//	CEdit* pedtREGIST_NO1 = (CEdit*)GetDlgItem(IDC_edtREGIST_NO1);
//	CEdit* pedtREGIST_NO2 = (CEdit*)GetDlgItem(IDC_edtREGIST_NO2);
//
//	pedtREGIST_NO1->SetWindowText(_T("EM0000000001"));
//	pedtREGIST_NO2->SetWindowText(_T("EM0010000100"));

//	CString strddd = m_pInfo->CONNECTION_INFO;
	// SM 설정
	if( InitESL_Mgr(_T("SM_IndexGeneratorSearch_Mono")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
	// DM 설정
	m_pIndexGeneratorDm = FindDM(_T("DM_IndexGenerator_Mono"));
	if( m_pIndexGeneratorDm == NULL )
	{
		ESLAfxMessageBox(_T("[DM_IndexGenerator_Mono]을 설정할 수 없습니다."));
		return FALSE;
	}

	// CM 설정
	m_pCM = FindCM(_T("CM_IndexGeneratorSearch_Mono"));
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_IndexGeneratorSearch_Mono]을 설정할 수 없습니다."));
		return FALSE;
	}

	m_pgridIndexGenerator = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	if( m_pgridIndexGenerator == NULL )
	{
		ESLAfxMessageBox(_T("그리드를 설정할 수 없습니다."));
		return FALSE;
	}

	
	
	// 검색구분 콤보설정
	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("색인재생성 검색 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

//	m_pgridIndexGenerator = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));

//	SetCodeControl(m_pAcqCodeComboBox, IDC_cmbAcqCode, _T("수입구분"));
//
//	SetCodeControl(m_pWorkCodeComboBox, IDC_cmbWorkCode, _T("업무구분"));

	// 윈도우Rect 설정
	GetWindowRect(m_rcWindow);

	ShowControlByShowMode();

	m_pCM->AllControlDisplay();
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//##ModelId=409EC8DD016A
VOID CSearchMono::ShowControlByShowMode()
{
	INT i, nCodeResourcesHeight = 75;
	//CRect rcWindow;
	CArray<INT, INT> ArrResourceID;	ArrResourceID.RemoveAll();
	CWnd* pWnd = NULL;
	

//	m_bShowMode = FALSE;
	m_bShowMode = true;
//	if( m_bShowMode == TRUE )	GetDlgItem(IDC_btnSEARCH_SHOWHIDE)->SetWindowText(_T("간략검색"));
//	else						GetDlgItem(IDC_btnSEARCH_SHOWHIDE)->SetWindowText(_T("상세검색"));
	

	//GetWindowRect(rcWindow);

//	ArrResourceID.Add(IDC_stcGROUP1);
	ArrResourceID.Add(IDC_stcREGIST_NO);
	ArrResourceID.Add(IDC_edtREGIST_NO1);
	ArrResourceID.Add(IDC_stcREGISTRANGE);
	ArrResourceID.Add(IDC_edtREGIST_NO2);
	ArrResourceID.Add(IDC_stcCONTROL_NO);
	ArrResourceID.Add(IDC_edtCONTROL_NO1);
	ArrResourceID.Add(IDC_stcCONTROLRATE);
	ArrResourceID.Add(IDC_edtCONTROL_NO2);
	
	// Show / Hide Code Resource
	for( i=0 ; i<ArrResourceID.GetSize() ; i++ )
	{
		pWnd = GetDlgItem(ArrResourceID.GetAt(i));
		if( pWnd->GetSafeHwnd() != NULL )
		{
			if( m_bShowMode == TRUE )	pWnd->ShowWindow(SW_SHOW);
			else						pWnd->ShowWindow(SW_HIDE);
		}
	}

	if( m_bShowMode == TRUE )
	{
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	}

	if( m_bRecycleData == TRUE )
	{
		GetDlgItem(IDC_edtREGIST_NO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_edtREGIST_NO2)->EnableWindow(FALSE);
	}
	
	Invalidate();
	UpdateWindow();
}

//##ModelId=409EC8DD0295
VOID CSearchMono::OnSelchangecmbSEARCHCLASS() 
{
	UpdateData(TRUE);
	ShowCtrlBySearchClass();
}

//##ModelId=409EC8DD0155
INT CSearchMono::ShowCtrlBySearchClass()
{
	BOOL bWorkCodeShowYN = FALSE;
	BOOL bAcqCodeShowYN = FALSE;
	CString strSearchClass, strYear;


	// 2005-12-08 강봉수
	// 재활용자료 추가 	
	m_bRecycleData = FALSE;

	m_pCM->GetControlMgrData(_T("검색구분"), strSearchClass);
	
	if		( strSearchClass == _T("구입차수검색") )
	{
		strYear = _T("수입년도");
		m_strGroupNo = _T("차수번호");
		bWorkCodeShowYN = TRUE;	//업무구분
		bAcqCodeShowYN = FALSE;
	}	
	else if	( strSearchClass == _T("기증접수번호검색") )
	{
		strYear = _T("수입년도");
		m_strGroupNo = _T("접수번호");
		bWorkCodeShowYN = FALSE;
		bAcqCodeShowYN = FALSE;
	}
	else if	( strSearchClass == _T("수서그룹번호검색") )
	{
		strYear = _T("수입년도");
		m_strGroupNo = _T("그룹번호");
		bWorkCodeShowYN = FALSE;
		bAcqCodeShowYN = TRUE;	//수입구분
	}
	// 2005-12-08 강봉수 재활용자료 추가
	else if	( strSearchClass == _T("재활용자료검색") )
	{
		strYear			= _T("수입년도");
		m_strGroupNo	= _T("그룹번호");
		bWorkCodeShowYN = FALSE;
		bAcqCodeShowYN	= TRUE;	//수입구분
		m_bRecycleData	= TRUE;
	}
	else if	( strSearchClass == _T("원부번호검색") )
	{
		strYear = _T("원부년도");
		m_strGroupNo = _T("원부번호");
		bWorkCodeShowYN = FALSE;
		bAcqCodeShowYN = FALSE;
	}

	GetDlgItem(IDC_STATIC_GROUP_YEAR)->SetWindowText(strYear);
	GetDlgItem(IDC_STATIC_WORK_CODE)->ShowWindow(bWorkCodeShowYN);
	GetDlgItem(IDC_cmbWorkCode)->ShowWindow(bWorkCodeShowYN);
	GetDlgItem(IDC_STATIC_ACQ_CODE)->ShowWindow(bAcqCodeShowYN);
	GetDlgItem(IDC_cmbAcqCode)->ShowWindow(bAcqCodeShowYN);
	GetDlgItem(IDC_STATIC_GROUP_NO)->SetWindowText(m_strGroupNo);
	
	return 0;	
}

//##ModelId=409EC8DD029F
//VOID CSearchMono::OnbtnSEARCHSHOWHIDE() 
//{
////	m_bShowMode = !m_bShowMode;
//	m_bShowMode = true;
//
//	ShowControlByShowMode();	
//}

//##ModelId=409EC8DD02B3
VOID CSearchMono::OnbtnBASICCLEAR() 
{
	ClearScreen(m_pCM);
	ShowCtrlBySearchClass();
}

//##ModelId=409EC8DD017D
VOID CSearchMono::ClearScreen(CESL_ControlMgr *pCM, CStringArray * pArrIgnoreCtrlAlias/*=NULL*/)
{
	if( pCM == NULL )	return ;
	
	CESL_Control_Element* pCE = NULL;
	POSITION pos = pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if( pCE == NULL )	continue;

		if		( pCE->CTRL_TYPE == _T("G") )	continue;
		else if	( pCE->CTRL_TYPE == _T("M") ) 
		{
			CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)pCE->Control;
			if( pCMB != NULL )	pCMB->SetCurSel(pCMB->m_nDefaultRowIdx);
			continue;
		}
		else if	( pCE->CTRL_TYPE == _T("C") )
		{
			CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)pCE->Control;
			if(pCMB) pCMB->SetCurSel(0);
			continue;
		}
		
		if( CheckIgnoreCtrl(pArrIgnoreCtrlAlias, pCE->GetAlias()) )		continue;
		
		pCE->Clear();
	}
}

//##ModelId=409EC8DD0191
BOOL CSearchMono::CheckIgnoreCtrl(CStringArray *pArrIgnoreCtrlAlias, CString strAlias)
{
	if( pArrIgnoreCtrlAlias == NULL )	return FALSE;
	INT cnt = pArrIgnoreCtrlAlias->GetSize();
	
	for( INT i=0 ; i<cnt ; i++ )
	{
		if( pArrIgnoreCtrlAlias->GetAt(i) == strAlias ) 	return TRUE;
	}

	return FALSE;
}

//##ModelId=409EC8DD0141
INT CSearchMono::MakeSearchQueryByAccessionNo()
{
	//원부번호
	INT CheckResult;
	BOOL SubCreate = FALSE;
	CString strAccessionYear, strGroupNo1, strGroupNo2, strRegNo1, strRegNo2, strQuery, strTemp;
	CString strConversRegNo1, strConversRegNo2;

	m_pCM->GetControlMgrData(_T("그룹년도"), strAccessionYear);		 //원부년도
	m_pCM->GetControlMgrData(_T("그룹번호1"), strGroupNo1);			 //원부번호1
	m_pCM->GetControlMgrData(_T("그룹번호2"), strGroupNo2);			 //원부번호2
	m_pCM->GetControlMgrData(_T("등록번호1"), strRegNo1);			 //등록번호1
	m_pCM->GetControlMgrData(_T("등록번호2"), strRegNo2);			 //등록번호2
	strTemp = _T("");
	
	// DM에 연결하지 않은 DB에서 DATA를 가져오는 SUB쿼리부분 추가.
	strQuery.Format(_T(" AND BS_.REC_KEY IN ")
					_T("( SELECT BB_.SPECIES_KEY ")
					_T("FROM BO_BOOK_TBL BB_, CO_ACCESSION_REC_TBL CA_ ")
					_T("WHERE BB_.ACCESSION_REC_KEY = CA_.REC_KEY(+)"));

	//원부년도
	if( strAccessionYear.IsEmpty() == FALSE )
	{
		SubCreate = TRUE;
		strTemp.Format(_T(" AND CA_.ACCESSION_REC_MAKE_YEAR='%s'"), strAccessionYear);
		m_strOption += strTemp;
	}
	
	//원부번호 범위
	CheckResult = CheckRange(strGroupNo1, strGroupNo2);
	switch( CheckResult )
	{
	case 0:
	{
		ESLAfxMessageBox(_T("원부번호의 입력 형식이 올바르지 않습니다."));
		return -1;
	}
		break;
	case 1:
	{
		SubCreate = TRUE;
		strTemp.Format(_T(" AND CA_.ACCESSION_REC_NO=%s"), strGroupNo1);
		m_strOption += strTemp;
	}
		break;
	case 2:
	{
		SubCreate = TRUE;
		strTemp.Format(_T(" AND CA_.ACCESSION_REC_NO BETWEEN %s AND %s"), strGroupNo1, strGroupNo2);
		m_strOption += strTemp;
	}
		break;
	default:         // CheckResult == -1
		break;
	}
	
	//등록번호 범위
	CheckResult = CheckRange(strRegNo1, strRegNo2);
	switch( CheckResult )
	{
	case 0:
	{
		ESLAfxMessageBox(_T("등록번호의 입력 형식이 올바르지 않습니다."));
		return -1;
	}
		break;
	case 1:
	{
		strConversRegNo1 = InputValueCheck(strRegNo1, _T("등록구분"));
		if( _tcsclen(strConversRegNo1) == 0 ) return -1;
		SubCreate = TRUE;
		strTemp.Format(_T(" AND BB_.REG_NO='%s'"), strConversRegNo1);
		m_strOption += strTemp;
	}
		break;
	case 2:
	{
		strConversRegNo1 = InputValueCheck(strRegNo1, _T("등록구분"));
		if( _tcsclen(strConversRegNo1) == 0 ) return -1;
		strConversRegNo2 = InputValueCheck(strRegNo2, _T("등록구분"));
		if( _tcsclen(strConversRegNo2) == 0 ) return -1;
		SubCreate = TRUE;
		strTemp.Format(_T(" AND BB_.REG_NO BETWEEN '%s' AND '%s'"), strConversRegNo1, strConversRegNo2);
		m_strOption += strTemp;
	}	
		break;
	default:	// -1
		break;
	}

	if( SubCreate == FALSE )	return -1;
	
	strTemp.Format(_T(")"));
	m_strOption += strTemp;
	strQuery += m_strOption;
	m_strQuery += strQuery;	
	
	return 0;
}

//##ModelId=409EC8DD014C
INT CSearchMono::MakeSearchQueryByDivNo()
{
	//차수번호
	INT CheckResult;
	BOOL QueryCreate = FALSE;	
	BOOL SubCreate = FALSE;
	CString strAcqYear, strWorkCode, strGroupNo1, strGroupNo2, strRegNo1, strRegNo2, strQuery, strTemp;
	CString strConversRegNo1, strConversRegNo2;
	

	m_pCM->GetControlMgrData(_T("그룹년도"), strAcqYear);			//수입년도
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);			//업무구분
	m_pCM->GetControlMgrData(_T("그룹번호1"), strGroupNo1);			//차수번호1
	m_pCM->GetControlMgrData(_T("그룹번호2"), strGroupNo2);			//차수번호2
	m_pCM->GetControlMgrData(_T("등록번호1"), strRegNo1);			//등록번호1
	m_pCM->GetControlMgrData(_T("등록번호2"), strRegNo2);			//등록번호2
	strTemp = _T("");

	//업무구분
	if( strWorkCode.IsEmpty() == FALSE )
	{
		QueryCreate = TRUE;
		strTemp.Format(_T(" AND BS_.WORK_CODE='%s'"), strWorkCode);
		m_strQuery += strTemp;
	}
	else
	{
		QueryCreate = TRUE;
		strTemp.Format(_T(" AND BS_.WORK_CODE IS NULL"));
		m_strQuery += strTemp;
	}

	// DM에 연결하지 않은 DB에서 DATA를 가져오는 SUB쿼리부분 추가.
	strQuery.Format(_T(" AND BS_.REC_KEY IN ( SELECT BB_.SPECIES_KEY FROM BO_BOOK_TBL BB_ WHERE BB_.ACQ_CODE = '1'"));
	
	//수입년도
	strTemp = _T("");
	if( strAcqYear.IsEmpty() == FALSE )
	{
		SubCreate = TRUE;
		strTemp.Format(_T(" AND BB_.ACQ_YEAR = '%s'"), strAcqYear);
		m_strOption += strTemp;
	}
	
	//차수번호 범위
	CheckResult = CheckRange(strGroupNo1, strGroupNo2);
	switch( CheckResult )
	{
	case 0:
	{
		ESLAfxMessageBox(_T("차수번호의 입력 형식이 올바르지 않습니다."));
		return -1;
	}
		break;
	case 1:
	{
		SubCreate = TRUE;
			strTemp.Format(_T(" AND BB_.SEQ_NO=%s"), strGroupNo1);
		m_strOption += strTemp;
	}
		break;
	case 2:
	{
		SubCreate = TRUE;
			strTemp.Format(_T(" AND BB_.SEQ_NO BETWEEN %s AND %s"), strGroupNo1, strGroupNo2);
		m_strOption += strTemp;
	}
		break;
	default:	//-1
		break;
	}

	//등록번호 범위
	CheckResult = CheckRange(strRegNo1, strRegNo2);
	switch( CheckResult )
	{
	case 0:
	{
		ESLAfxMessageBox(_T("등록번호의 입력 형식이 올바르지 않습니다."));
		return -1;
	}
		break;
	case 1:
	{
		strConversRegNo1 = InputValueCheck(strRegNo1, _T("등록구분"));
		if( _tcsclen(strConversRegNo1) == 0 ) return -1;
		SubCreate = TRUE;
			strTemp.Format(_T(" AND BB_.REG_NO='%s'"), strConversRegNo1);
		m_strOption += strTemp;
	}
		break;
	case 2:
	{
		strConversRegNo1 = InputValueCheck(strRegNo1, _T("등록구분"));
		if( _tcsclen(strConversRegNo1) == 0 ) return -1;
		strConversRegNo2 = InputValueCheck(strRegNo2, _T("등록구분"));
		if( _tcsclen(strConversRegNo2) == 0 ) return -1;
		SubCreate = TRUE;
			strTemp.Format(_T(" AND BB_.REG_NO BETWEEN '%s' AND '%s'"), strConversRegNo1, strConversRegNo2);
		m_strOption += strTemp;
	}
		break;
	default:	//-1
		break;
	}

	if( SubCreate == FALSE && QueryCreate == FALSE )	return -1;

	if( SubCreate == TRUE )		
	{
		strTemp.Format(_T(")"));
	m_strOption += strTemp;
	strQuery += m_strOption;
	m_strQuery += strQuery;	
	}
	
	return 0;
}

//##ModelId=409EC8DD0137
INT CSearchMono::MakeSearchQueryByGroupNo()
{
	//그룹번호
	INT CheckResult;
	BOOL SubCreate = FALSE;
	CString strAcqYear, strAcqCode, strGroupNo1, strGroupNo2, strRegNo1, strRegNo2, strQuery, strTemp, strTbl;
	CString strConversRegNo1, strConversRegNo2;

	m_pCM->GetControlMgrData(_T("그룹년도"), strAcqYear);			//수입년도
	m_pCM->GetControlMgrData(_T("수입구분"), strAcqCode);			//수입구분
	m_pCM->GetControlMgrData(_T("그룹번호1"), strGroupNo1);			//그룹번호1
	m_pCM->GetControlMgrData(_T("그룹번호2"), strGroupNo2);			//그룹번호2
	m_pCM->GetControlMgrData(_T("등록번호1"), strRegNo1);			//등록번호1
	m_pCM->GetControlMgrData(_T("등록번호2"), strRegNo2);			//등록번호2
	strTemp = _T("");
	
	// DM에 연결하지 않은 DB에서 DATA를 가져오는 SUB쿼리부분 추가.
	if( m_bRecycleData == TRUE )	strTbl = _T("BO_DELETE_BOOK_TBL");
	else							strTbl = _T("BO_BOOK_TBL");
	strQuery.Format(_T(" AND BS_.REC_KEY IN ( SELECT BB_.SPECIES_KEY FROM %s BB_ WHERE"), strTbl);

	//수입년도
	if( strAcqYear.IsEmpty() == FALSE )
	{
		SubCreate = TRUE;
		strTemp.Format(_T(" BB_.ACQ_YEAR = '%s'"), strAcqYear);
		m_strOption += strTemp;
	}

	//수입구분
	if( strAcqCode.IsEmpty() == FALSE)
	{
		SubCreate = TRUE;
		if(_tcsclen(strTemp) != 0) m_strOption += _T(" AND");
		strTemp.Format(_T(" BB_.ACQ_CODE='%s'"), strAcqCode);
		m_strOption += strTemp;
	}

	//그룹번호 범위
	CheckResult = CheckRange(strGroupNo1, strGroupNo2);
	switch( CheckResult )
	{
	case 0:
	{
		ESLAfxMessageBox(_T("그룹번호의 입력 형식이 올바르지 않습니다."));
		return -1;
	}
		break;
	case 1:
	{
		SubCreate = TRUE;
		if(_tcsclen(strTemp) != 0) m_strOption += _T(" AND");
		strTemp.Format(_T(" BB_.SEQ_NO='%s'"), strGroupNo1);
		m_strOption += strTemp;
	}
		break;
	case 2:
	{
		SubCreate = TRUE;
		if(_tcsclen(strTemp) != 0) m_strOption += _T(" AND");
		strTemp.Format(_T(" BB_.SEQ_NO BETWEEN '%s' AND '%s'"), strGroupNo1, strGroupNo2);
		m_strOption += strTemp;
	}
		break;
	default:	// -1
		break;
	}

	//등록번호 범위
	if( m_bRecycleData == FALSE )
	{
	CheckResult = CheckRange(strRegNo1, strRegNo2);
		switch( CheckResult )
		{
		case 0:
	{
		ESLAfxMessageBox(_T("등록번호의 입력 형식이 올바르지 않습니다."));
		return -1;
	}
			break;
		case 1:
	{
		strConversRegNo1 = InputValueCheck(strRegNo1, _T("등록구분"));
		if( _tcsclen(strConversRegNo1) == 0 ) return -1;
		SubCreate = TRUE;
		if(_tcsclen(strTemp) != 0) m_strOption += _T(" AND");
		strTemp.Format(_T(" BB_.REG_NO='%s'"), strConversRegNo1);
		m_strOption += strTemp;
	}
			break;
		case 2:
	{
		strConversRegNo1 = InputValueCheck(strRegNo1, _T("등록구분"));
		if( _tcsclen(strConversRegNo1) == 0 ) return -1;
		strConversRegNo2 = InputValueCheck(strRegNo2, _T("등록구분"));
		if( _tcsclen(strConversRegNo2) == 0 ) return -1;
		SubCreate = TRUE;
		if(_tcsclen(strTemp) != 0) m_strOption += _T(" AND");
		strTemp.Format(_T(" BB_.REG_NO BETWEEN '%s' AND '%s'"), strConversRegNo1, strConversRegNo2);
		m_strOption += strTemp;
	}
			break;
		default:	//-1
			break;
		}	
	}

	if( SubCreate == FALSE )	return -1;
	
	
	strTemp.Format(_T(")"));
	m_strOption += strTemp;
	strQuery += m_strOption;
	m_strQuery += strQuery;	

	return 0;
}

//##ModelId=409EC8DD014B
INT CSearchMono::MakeSearchQueryByReceiptNo()
{
	//접수번호
	INT CheckResult;
	BOOL SubCreate = FALSE;
	CString strAcqYear, strGroupNo1, strGroupNo2, strRegNo1, strRegNo2, strQuery, strTemp;
	CString strConversRegNo1, strConversRegNo2;

	m_pCM->GetControlMgrData(_T("그룹년도"), strAcqYear);			//수입년도
	m_pCM->GetControlMgrData(_T("그룹번호1"), strGroupNo1);			//접수번호1
	m_pCM->GetControlMgrData(_T("그룹번호2"), strGroupNo2);			//접수번호2
	m_pCM->GetControlMgrData(_T("등록번호1"), strRegNo1);			//등록번호1
	m_pCM->GetControlMgrData(_T("등록번호2"), strRegNo2);			//등록번호2
	strTemp = _T("");
	
	// DM에 연결하지 않은 DB에서 DATA를 가져오는 SUB쿼리부분 추가.
	// 2005.12.08 강봉수 쿼리문 수정 - 기증자료이므로 수입구분에 기증(2)를 추가한다.
	strQuery.Format(_T(" AND BS_.REC_KEY IN ( SELECT BB_.SPECIES_KEY FROM BO_BOOK_TBL BB_ WHERE BB_.ACQ_CODE = '2'"));

	//수입년도
	if( strAcqYear.IsEmpty() == FALSE )
	{
		SubCreate = TRUE;
		strTemp.Format(_T(" AND BB_.ACQ_YEAR='%s'"), strAcqYear);
		m_strOption += strTemp;
	}

	//접수번호 범위
	CheckResult = CheckRange(strGroupNo1, strGroupNo2);
	switch( CheckResult )
	{
	case 0:
	{
		ESLAfxMessageBox(_T("접수번호의 입력 형식이 올바르지 않습니다."));
		return -1;
	}
		break;
	case 1:
	{
		SubCreate = TRUE;
			strTemp.Format(_T(" AND BB_.SEQ_NO = %s"), strGroupNo1);
		m_strOption += strTemp;
	}
		break;
	case 2:
	{
		SubCreate = TRUE;
			strTemp.Format(_T(" AND BB_.SEQ_NO BETWEEN %s AND %s"), strGroupNo1, strGroupNo2);
		m_strOption += strTemp;
	}
		break;
	default:	//-1
		break;
	}

	//등록번호 범위
	CheckResult = CheckRange(strRegNo1, strRegNo2);
	switch( CheckResult )
	{
	case 0:
	{
		ESLAfxMessageBox(_T("차수번호의 입력 형식이 올바르지 않습니다."));
		return -1;
	}
		break;
	case 1:
	{
		strConversRegNo1 = InputValueCheck(strRegNo1, _T("등록구분"));
		if( _tcsclen(strConversRegNo1) == 0 ) return -1;
		SubCreate = TRUE;
			strTemp.Format(_T(" AND BB_.REG_NO='%s'"), strConversRegNo1);
		m_strOption += strTemp;
	}
		break;
	case 2:
	{
		strConversRegNo1 = InputValueCheck(strRegNo1, _T("등록구분"));
		if( _tcsclen(strConversRegNo1) == 0 ) return -1;
		strConversRegNo2 = InputValueCheck(strRegNo2, _T("등록구분"));
		if( _tcsclen(strConversRegNo2) == 0 ) return -1;
		SubCreate = TRUE;
			strTemp.Format(_T(" AND BB_.REG_NO BETWEEN '%s' AND '%s'"), strConversRegNo1, strConversRegNo2);
		m_strOption += strTemp;
	}
		break;
	default:	//-1
		break;
	}
	
	if( SubCreate == FALSE )	return -1;
	
	strTemp.Format(_T(")"));
	m_strOption += strTemp;
	strQuery += m_strOption;
	m_strQuery += strQuery;	

	return 0;
}

//##ModelId=409EC8DD02BD
VOID CSearchMono::OnOK() 
{
	INT ids, subids, CheckResult;
	CString strControlNo1, strControlNo2, strTemp;
	
	
	m_strQuery = m_strOption = _T("");

	UpdateData(TRUE);

//	m_strQuery.Format(_T(" ( ( IBS.REC_KEY(+) = BS_.REC_KEY)"));
	//관리구분
	CString strManageCode = _T("");
	m_pCM->GetControlMgrData(_T("관리구분"), strManageCode);		//관리구분

	if( !strManageCode.IsEmpty() )
	{
		m_strQuery.Format(_T(" BS_.MANAGE_CODE = '%s' AND ( ( IBS.REC_KEY(+) = BS_.REC_KEY)"), strManageCode);
	}
	else
	{
		m_strQuery.Format(_T(" ( ( IBS.REC_KEY(+) = BS_.REC_KEY)"));
	}

	m_pCM->GetControlMgrData(_T("제어번호1"), strControlNo1);  
	m_pCM->GetControlMgrData(_T("제어번호2"), strControlNo2);  
	
	//제어번호
	CheckResult = CheckRange(strControlNo1, strControlNo2);
	switch( CheckResult )
	{
	case 0:		// 뒷쪽만 입력한 경우.
	{
		ESLAfxMessageBox(_T("제어번호의 입력 형식이 올바르지 않습니다."));
		return;
	}
		break;
	case 1:		// 앞쪽만 입력한 경우.
	{
			if( InputValueCheck(strControlNo1, _T("제어자료구분")) < 0 )
		{
			ESLAfxMessageBox(_T("제어번호의 입력 형식이 올바르지 않습니다."));
			return;
		}
		ids = 0;
		strTemp.Format(_T(" AND BS_.CONTROL_NO='%s'"), strControlNo1);
		m_strQuery += strTemp;
	}
		break;
	case 2:     // 양쪽다 입력한 경우.
	{
	    if(InputValueCheck(strControlNo1, _T("제어자료구분")) < 0)
		{
			ESLAfxMessageBox(_T("제어번호의 입력 형식이 올바르지 않습니다."));
			return;
		}
		if(InputValueCheck(strControlNo2, _T("제어자료구분")) < 0) 
		{
			ESLAfxMessageBox(_T("제어번호의 입력 형식이 올바르지 않습니다."));
			return;
		}
		ids = 0;
		strTemp.Format(_T(" AND BS_.CONTROL_NO BETWEEN '%s' AND '%s'"), strControlNo1, strControlNo2);
		m_strQuery += strTemp;
	}
		break;
	default:	ids = -1;	// -1
		break;
	}
	
	
	if		( m_strGroupNo == _T("차수번호") )		subids = MakeSearchQueryByDivNo();
	else if	( m_strGroupNo == _T("접수번호") )		subids = MakeSearchQueryByReceiptNo();
	else if	( m_strGroupNo == _T("원부번호") )		subids = MakeSearchQueryByAccessionNo();
	else if	( m_strGroupNo == _T("그룹번호") )		subids = MakeSearchQueryByGroupNo();	

	if( (ids < 0) && (subids < 0) )
	{
		ESLAfxMessageBox(_T("검색 질의를 생성할 수 없습니다."));
		return;
	}
	
	strTemp.Format(_T(")"));
	m_strQuery += strTemp;
	SendQueryCreate();
}

VOID CSearchMono::MakeIndexWord(CString &strData)
{
	//해당 데이터의 공백을 지우고, 트림하고, 특수기호를 없앤후,  한자를 한글로 만든다.
	
	strData.Replace(_T(" "),_T(""));
	
	CIndexTool *pIndexTool = NULL;
	pIndexTool = (CIndexTool*)m_pInfo->pIndexTool;
	if(!pIndexTool) return;
	
	CIndex* pIndex = NULL;
	pIndex = pIndexTool->GetIndex();
	if(!pIndex) return;
	
	pIndex->TrimAll(strData);
	pIndex->RemoveSpecialChar(strData);
	pIndex->ConvertHanjaToHangul(strData);
}

//##ModelId=409EC8DD019C
INT CSearchMono::SendQueryCreate()
{
	INT ids, nRow;
	CString strQuery, str;
	

	m_pIndexGeneratorDm->FreeData();
	strQuery.Format(_T("%s"), m_strQuery);
	if( strQuery.IsEmpty() == TRUE )
	{
		ESLAfxMessageBox(_T("검색 질의를 생성할 수 없습니다."));
		return -1;
	}

	m_bRecycleData = IsRecycleData();
	if( m_bRecycleData == TRUE )		m_pIndexGeneratorDm->TBL_NAME = _T("IDX_BO_TBL IBS, BO_DELETE_SPECIES_TBL BS_");
	else								m_pIndexGeneratorDm->TBL_NAME = _T("IDX_BO_TBL IBS, BO_SPECIES_TBL BS_");


	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("검색 결과 자료를 구축중입니다."));
	ids = m_pIndexGeneratorDm->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	DeleteDataProcessingDlg();

	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("검색 질의를 수행할 수 없습니다."));
		return -1;
	}

	nRow = m_pIndexGeneratorDm->GetRowCount();
	if( nRow == 0 )
	{
		m_pgridIndexGenerator->Display();
		str.Format(_T("검색 결과가 없습니다."),nRow);
//		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
		AfxMessageBox(str);
		return 0;
	}
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pIndexGeneratorDm,_T("자료구분"),_T("UDF_자료구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pIndexGeneratorDm,_T("형식구분"),_T("UDF_형식구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pIndexGeneratorDm,_T("보조등록구분"),_T("UDF_보조등록구분"));

	m_pgridIndexGenerator->Display();
	str.Format(_T(" %d 건의 자료가 검색되었습니다."),nRow);
//	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));

	return 0;
	
}



//##ModelId=409EC8DD015F
INT CSearchMono::CheckRange(CString strNumber1, CString strNumber2)
{

	if( strNumber1.IsEmpty() == TRUE )
	{
		//둘 다 비어있는 경우
		if( strNumber2.IsEmpty() == TRUE )		return -1;
		//앞번호는 비어있고 뒷번호는 채워있는 경우
		else									return 0;
	}

	//앞번호는 채워있고 뒷번호는 비어있는 경우
	if( strNumber2.IsEmpty() == TRUE )			return 1;
	
	//앞번호 뒷번호 모두 채워있는 경우
	return 2;    						
}

BOOL CSearchMono::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	

	if (pMsg->message == WM_KEYDOWN)
	{
		if(  VK_RETURN == pMsg->wParam ) 
		{
			if ( GetDlgItem(IDC_cmbSEARCH_CLASS)->m_hWnd == pMsg->hwnd ||
				 GetDlgItem(IDC_edtGROUP_YEAR)->m_hWnd == pMsg->hwnd   ||
				 GetDlgItem(IDC_edtGROUP_NO1)->m_hWnd == pMsg->hwnd    ||
				 GetDlgItem(IDC_edtGROUP_NO2)->m_hWnd == pMsg->hwnd    ||
				 GetDlgItem(IDC_cmbWorkCode)->m_hWnd == pMsg->hwnd    ||
				 GetDlgItem(IDC_edtREGIST_NO1)->m_hWnd == pMsg->hwnd   ||
				 GetDlgItem(IDC_edtREGIST_NO2)->m_hWnd == pMsg->hwnd   ||
				 GetDlgItem(IDC_edtCONTROL_NO1)->m_hWnd == pMsg->hwnd  ||
				 GetDlgItem(IDC_edtCONTROL_NO2)->m_hWnd == pMsg->hwnd   )
			{
				GetDlgItem(IDOK)->SetFocus();
				PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
				return TRUE;	
			}
		}
		else if( VK_ESCAPE == pMsg->wParam   )
		{
			OnbtnMonoClose();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
CString CSearchMono::InputValueCheck(CString strData, CString strCodeClassName)
{
	CString codes[6];
	CString code, strChar, strTemp;
	INT nstrLength, nCodeType;

	nstrLength = strData.GetLength();

	//제어번호의 경우.
	if(strCodeClassName == _T("제어자료구분"))   
	{
		if(nstrLength != 12) return _T("");
		nCodeType =3;	 	
		strChar = strData.Left(nCodeType);

		//구분 코드 이외의 값이 숫자인지를 확인.
		for(INT i=nCodeType; i<nstrLength; i++)
		{
			strTemp = strData.Mid(i,1);
			if(strTemp < 0x0030 || strTemp > 0x0039) return _T("");
		}
	}
	//등록번호의 경우.
	else // if(strCodeClassName == _T("등록구분"))  
	{
		if(nstrLength < 3 ) return _T("");
		nCodeType = 2;		
		strChar = strData.Left(nCodeType);
		
		//구분 코드 이외의 값이 숫자인지를 확인.
		for(INT i=nCodeType; i<nstrLength; i++)
		{
			strTemp = strData.Mid(i,1);
			if(strTemp < 0x0030 || strTemp > 0x0039) return _T("");
		}
		//12자리를 채우지 않고 입력한 경우 빈 자릿수만큼 _T("0")을 추가.
		for(i=0; i<(12 - nstrLength) ; i++)
		{
			strData.Insert(nCodeType, _T("0"));
		}
	}


	//입력값의 코드부분이 정해진 코드값들 중 하나와 일치하는 지 확인.
	INT startrow = 0;
	INT codeCnt = 	m_pInfo->pCodeMgr->GetCodeCount(strCodeClassName);
	for(INT i=0 ; i < codeCnt ; i++)
	{
		startrow = m_pInfo->pCodeMgr->GetCode(strCodeClassName, codes, 6, startrow);
		code = codes[2];  //code[2] : 코드
		
		code.TrimLeft(); code.TrimRight();
		
		if(code == strChar) return strData;
		
		startrow++;
	}

	return _T("");
}


//HBRUSH CSearchMono::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
//{
//	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	return hbr;
//}



VOID CSearchMono::EnableThemeDialogTexture(HWND hwndDlg)
{
	HINSTANCE hDll=LoadLibrary(_T("UxTheme.dll"));							// 'UxTheme.dll'
	if(hDll==NULL)
		return;	// the DLL won't be available on anything except Windows XP
	ULONG (PASCAL *lpfnEnableTheme)(HWND, DWORD);
	(FARPROC&)lpfnEnableTheme=GetProcAddress(hDll,"EnableThemeDialogTexture");	// 'EnableThemeDialogTexture'
	if(lpfnEnableTheme)
		lpfnEnableTheme(hwndDlg, 6);		// ETDT_ENABLETAB = (ETDT_ENABLE | ETDT_USETABTEXTURE) = (2|4) = 6
	else ASSERT(FALSE);
	FreeLibrary(hDll);
}

void CSearchMono::OnbtnMonoClose() 
{
	// TODO: Add your control notification handler code here
//	CDialog::OnCancel();
	::SendMessage(pParentMgr->GetSafeHwnd(), DLG_CLOSE, 0, 0);
}

//INT CSearchMono::SetCodeControl(CESL_ControlMultiComboBox *pMultiComboBox, UINT nID, CString strClassName)
//{
//	// 코드로 부터 관리구분을 읽어온다.
//	if (pMultiComboBox == NULL) 
//	{
//		pMultiComboBox = new CESL_ControlMultiComboBox(NULL);
//	}
//	CString strQuery;
//	CString strClassCode;
//	strQuery.Format(_T("SELECT CLASS_CODE FROM CD_CLASS_CODE_TBL WHERE CLASS_NAME='%s'"), strClassName);
//	m_pControlDm.GetOneValueQuery( strQuery, strClassCode );
//	strClassCode.TrimLeft();			strClassCode.TrimRight();
//	strQuery.Format(_T("SELECT CODE, DESCRIPTION FROM CD_CODE_TBL WHERE CLASS=%s ORDER BY CODE"), strClassCode);
//	m_pControlDm.RestructDataManager(strQuery);
//
//	pMultiComboBox->SubclassDlgItem(nID, this);
//	pMultiComboBox->ResetContent();
//	pMultiComboBox->InitMultiComboBox(2);
//	CString strData[2];
//	strData[0] = _T("");
//	strData[1] = _T("");
//	INT nRows = m_pControlDm.GetRowCount();
//	for (INT i = 0; i < nRows; i++) {
//		strData[0] = m_pControlDm.GetCellData(i, 0);
//		strData[1] = m_pControlDm.GetCellData(i, 1);
//		strData[0].TrimLeft();			strData[0].TrimRight();
//		if (strData[0] == _T("")) strData[0] = _T("   ");
//		pMultiComboBox->SetData(strData, 2);
//	}
//	pMultiComboBox->SetCurSel(0);
//
//	return 0;
//}

void CSearchMono::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
//	GetWindowRect( m_MainRect );
//	CRect ClientRect;
	CStatic *pstcMonoGrid = (CStatic*)GetDlgItem(IDC_stcMonoGrid);
	if( pstcMonoGrid->GetSafeHwnd() == NULL) return;
	pstcMonoGrid->MoveWindow( 5, 135,  cx-5, cy-135 );

	CESL_Grid *pgridMono = (CESL_Grid *)GetDlgItem(IDC_gridMono);
	if( pgridMono->GetSafeHwnd() == NULL) return;
//	pgridMono->MoveWindow( 10, 150,  cx-15, cy-155 );
	pgridMono->MoveWindow( 0, 140,  cx, cy-140 );

//	Invalidate();
	
}


VOID CSearchMono::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) 
	{
		m_pDataProcessingDlg = new CDataProcessingDlg(this);
	}
	
	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(this))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	//프로그래스 컨트롤 초기화
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CSearchMono::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}


INT CSearchMono::LoadCodeInfo()
{
	m_pInfo->pCodeMgr->CONNECTION_INFO = m_pInfo->CONNECTION_INFO;

	return m_pInfo->pCodeMgr->ReadCodeData(m_pInfo->USER_PK, NULL);
}

INT CSearchMono::SetParentInfo()
{
//	m_pInfo = new CESL_Information();
	INT ids = -1;
	m_pInfo->CONNECTION_INFO = m_strConnectInfo;

	CString strQuery;
	//                          0        1         2          3		
//	strQuery.Format(_T("SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE "
//						 _T("FROM MN_USER_TBL ")
//						_T("WHERE ID='%s' AND PASSWD='%s'")), m_strID, m_strPassword);
	strQuery.Format(_T("SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE, ID "
					 _T("FROM MN_USER_TBL ")
					_T("WHERE REC_KEY = ( SELECT MIN( REC_KEY )FROM MN_USER_TBL WHERE PERMITION = 'A' )")));
	ids = m_pControlDm.RestructDataManager(strQuery);
	if (ids < 0) return ids;
	if (m_pControlDm.GetRowCount() < 1) return -1;	// 로그인 실패
	// m_pInfo 세팅한다.
//	m_pInfo->USER_ID		= m_strID;
	m_pInfo->USER_NAME		= m_pControlDm.GetCellData(0, 0); 
	m_pInfo->USER_PRIVILEGE = m_pControlDm.GetCellData(0, 1); 
	m_pInfo->USER_PK		= m_pControlDm.GetCellData(0, 2);
//	m_pInfo->LOCAL_IP		= GetIP();
	m_pInfo->MANAGE_CODE	= m_pControlDm.GetCellData(0, 3);
	m_pInfo->USER_ID		= m_pControlDm.GetCellData(0, 4);
	m_pInfo->MANAGE_CODE.TrimLeft();		
	m_pInfo->MANAGE_CODE.TrimRight();
	m_pControlDm.MANAGE_CODE = m_pInfo->MANAGE_CODE;

	// 2009.11.19 ADD BY PWR : 통합대출권수사용여부가 사용안함이면 통합환경으로 보지 않는다. (PERMITION이 'A'이면 'S'로 설정)
	CString strTemp;
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE VALUE_NAME='통합대출권수사용여부' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_pControlDm.GetOneValueQuery(strQuery, strTemp);
	strTemp.TrimLeft(); strTemp.TrimRight();
	if(_T("Y") != strTemp)
	{
		if(_T("A") == m_pInfo->USER_PRIVILEGE)
		{
			m_pInfo->USER_PRIVILEGE = _T("S");
		}
	}

	//2008.11.11 ADD  BY PJW : 등록구분코드 자리수 설정
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '등록구분코드자리수설정' AND MANAGE_CODE = '%s'"),m_pInfo->MANAGE_CODE);
	m_pControlDm.GetOneValueQuery(strQuery, strTemp);
	m_pInfo->nREG_CODE_LENGTH = _ttoi(strTemp);

	m_pInfo->InitMarcMgr();
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	m_pIndexTool = new CIndexTool;
	m_pInfo->pIndexTool = (CObject*)m_pIndexTool;

	return 0;
}

void CSearchMono::OnbtnCreateMonoIndex() 
{
	// TODO: Add your control notification handler code here
	IndexReproduce();	
}

INT CSearchMono::IndexReproduce()
{
	INT ids, nSelectedRowIdx, nPos = 0;
	BOOL bInsertFrame;
	CString strMarc, strSpeciesKey, strError, strMarcTitle, strMarcInfo;
	CStringArray strAliasArray;
	CIndexDataMgr IndexMgr(this);


	if( IndexMgr.SetDataMgr() == -1 )	return -1;	
	if( m_pIndexGeneratorDm->GetRowCount() == 0 )		return -1;	
	if( m_pgridIndexGenerator == NULL )				return -1;


	ids = m_pgridIndexGenerator->SelectMakeList();
	if( ids < 0 )	return -1;

	if( 1 > m_pgridIndexGenerator->SelectGetCount() )
	{
		ESLAfxMessageBox( _T("자료를 선정해 주십시오") );
		return -1;
	}


	//그리드의 선택된 첫번째 항목의 Position의 Index값을 리턴
	nSelectedRowIdx = m_pgridIndexGenerator->SelectGetHeadPosition();  
	if( nSelectedRowIdx < 0 )	return -1;

	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("색인 재생성 중입니다."));
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, m_pgridIndexGenerator->SelectGetCount());
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);

	do
	{
		// 기본값은 UpdateFrame이다.
		bInsertFrame = FALSE;
		strSpeciesKey.Empty();
		strAliasArray.RemoveAll();
	//InMarc 색인재생성 
		m_pIndexGeneratorDm->GetCellData(_T("BS_MARC"), nSelectedRowIdx, strMarc);
		m_pIndexGeneratorDm->GetCellData(_T("IBS_종KEY"), nSelectedRowIdx, strSpeciesKey);
		// 인덱스 종키가 없다면 인덱스가 존재하지 않는 것이므로 종키를 복사하여 INSERT쿼리를 날린다.
		if( strSpeciesKey.IsEmpty() == TRUE )
		{
			m_pIndexGeneratorDm->GetCellData(_T("BS_종KEY"), nSelectedRowIdx, strSpeciesKey);
			bInsertFrame = TRUE;
		}


	// MARC에서 가져온 총서명과 판사항 값은 바뀌지 않으므로 그대로 유지.
		m_pCM->GetControlMgrData(_T("그리드"), strMarcTitle, nSelectedRowIdx, 4);
		m_pCM->GetControlMgrData(_T("그리드"), strMarcInfo, nSelectedRowIdx, 5);

		// 색인DM의 필드를 초기화 시킨다.
		IndexMgr.m_pIndexDM->InitDBFieldData();
		// Start Frame
		ids = IndexMgr.m_pIndexDM->StartFrame();
		if( ids < 0 )		return ErrorProc(nSelectedRowIdx);
		// 마크정보를 추출하여 배열에 저장한다.
		if( IndexMgr.ExtractIndexInMarc(strMarc, strAliasArray) < 0 )	return ErrorProc(nSelectedRowIdx);
		// 추출된 마크정보를 색인DM에 구축한다.
		if( IndexMgr.RestructIndexInMarc(&strAliasArray) < 0		)	return ErrorProc(nSelectedRowIdx);
		// 종정보를 추가시킨다.
		// 2005-12-12 강봉수 추가
		if( IndexMgr.RestructIndexInSpecies(strSpeciesKey, m_bRecycleData) < 0 )	return ErrorProc(nSelectedRowIdx);		
		// 종키를 이용하여 책테이블의 정보를 색인 DM에 대입한다.
		if( m_bRecycleData == TRUE )
		{
			if( IndexMgr.RestructIndexInDeleteBook(strSpeciesKey) < 0 )	return ErrorProc(nSelectedRowIdx);
		}
		else
		{
			if( IndexMgr.RestructIndexInBook(strSpeciesKey) < 0	)	return ErrorProc(nSelectedRowIdx);
			}
			// 인덱스가 존재하지 않을때에는 Insert 쿼리를 날린다.
			if( bInsertFrame == TRUE )
			{
				// 종구분값은 기본 2로 나타낸다
				if( m_bRecycleData == TRUE )
				{
					IndexMgr.m_pIndexDM->AddDBFieldData(_T("SPECIES_CLASS"), _T("STRING"), _T("2"));
				}
		
				if( IndexMgr.MakeInsert(strSpeciesKey) < 0 )	return ErrorProc(nSelectedRowIdx);
				
			}
			else
			{
				if( IndexMgr.MakeUpdate(strSpeciesKey) < 0 )	return ErrorProc(nSelectedRowIdx);
			}
			// 검색엔진과의 동기화를 맞추기위해 IDX_TMP_BO_TBL에 쿼리를 추가한다.
			//=====================================================
			//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
	// 		if( IndexMgr.SyncIndexWithSearchEngine(strSpeciesKey, IndexMgr.m_pIndexDM) < 0 ) return ErrorProc(nSelectedRowIdx);
			//=====================================================
			

			// 2004.11.11 은평도서관 임시 단행 색인재생성 ECO_SEARCH안씀.
			//if(IndexMgr.SyncIndexWithECO_SEARCH(strSpeciesKey, IndexMgr.m_pIndexDM) < 0 ) return ErrorProc(nSelectedRowIdx);
			
			// Send
			ids = IndexMgr.m_pIndexDM->SendFrame();
			if( ids < 0 )	return ErrorProc(nSelectedRowIdx);
			// End
			ids = IndexMgr.m_pIndexDM->EndFrame();
			if( ids < 0 )	return ErrorProc(nSelectedRowIdx);
			
			// 실패여부를 체크한다.
			if( IndexMgr.UpdateParentDM(strAliasArray, m_pIndexGeneratorDm, nSelectedRowIdx, _T("IBS")) < 0 )
			{
				DeleteDataProcessingDlg();
				strError.Format(_T("%s번째 책에 대한 색인재생성을 DM에 Update하던중 에러발생!"), nSelectedRowIdx);
				ESLAfxMessageBox(strError);
				return -1;
			}
			else
			{
				m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++nPos);		
				m_pgridIndexGenerator->DisplayLine(nSelectedRowIdx);
				m_pCM->SetControlMgrData(_T("그리드"), strMarcTitle, nSelectedRowIdx, 4);
				m_pCM->SetControlMgrData(_T("그리드"), strMarcInfo, nSelectedRowIdx, 5);
		}
	}while( (nSelectedRowIdx = m_pgridIndexGenerator->SelectGetNext()) > -1 );

	DeleteDataProcessingDlg();
	ESLAfxMessageBox(_T("색인재생성 완료!"));
	return 0;
}


INT CSearchMono::ErrorProc(INT nRowIdx)
{
	DeleteDataProcessingDlg();
	ShowErrorMessageBox(nRowIdx);
	return -1;
}

INT CSearchMono::ShowErrorMessageBox(INT nRowIdx)
{
	CString strError;
	strError.Format(_T("%d번째 책에 대한 색인재생성을 수행하던중 에러발생!"), nRowIdx+1);
	ESLAfxMessageBox(strError);
	return -1;
}

void CSearchMono::OnbtnMonoIndexInfoView() 
{
	// TODO: Add your control notification handler code here
	SingleIndexReproduce();
	
}


INT CSearchMono::SingleIndexReproduce()
{
	INT nMode, nSelectCnt, nStartPos, nTmpStartPos, nIndex, i;
	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;

	if( m_pIndexGeneratorDm->GetRowCount() < 1 )
	{
		ESLAfxMessageBox( _T("자료를 검색해 주십시오") );
		return 0;
	}

	// 선정된 첫번째 Row 위치 찾기.
	m_pgridIndexGenerator->SelectMakeList();
	nSelectCnt = m_pgridIndexGenerator->SelectGetCount();
	nStartPos = 0;
	nTmpStartPos = m_pgridIndexGenerator->GetIdx();
	if( nSelectCnt > 0 )
	{
		nMode = 1;
		m_pgridIndexGenerator->SelectGetHeadPosition();
	}
	else
	{
		nMode = 0;
		nSelectCnt = m_pIndexGeneratorDm->GetRowCount();
		nStartPos = m_pgridIndexGenerator->GetIdx();
	}

	// 선정된 Row의 Index을 List에 저장하기.
	for( i=0 ; i<nSelectCnt ; i++ )
	{
		if( nMode == 1 )	nIndex = m_pgridIndexGenerator->SelectGetIdx();
		else				nIndex = i;
	
		// 종KEY 저장.
		// 색인이 없는 경우에도 새로 생성해야 하므로 인덱스테이블이 아닌 종테이블에서 가져온다.
		// 2005-12-08 강봉수 수정
		//m_pIndexGeneratorDm->GetCellData( _T("IBS_종KEY") , nIndex, sSpeciesKey );
		m_pIndexGeneratorDm->GetCellData( _T("BS_종KEY") , nIndex, sSpeciesKey );
		sSpeciesKeyList.AddTail( sSpeciesKey );
		asGridRow.Add( nIndex );

		if( nIndex == nTmpStartPos )	nStartPos = i;
		
		if( nMode == 1 )	m_pgridIndexGenerator->SelectGetNext();
	}

	// Dialog 보이기.
	CIndexDetailViewerDlg IndexViewDlg(this);
	IndexViewDlg.m_nDANorYON = DAN;	
	// 2005-12-08 강봉수 재활용자료 추가
	IndexViewDlg.SetIsRecycleData( m_bRecycleData );

	// Dialog에 보여질 정보들 주기.
	IndexViewDlg.SetDialogInfo(sSpeciesKeyList, m_pgridIndexGenerator, asGridRow, nStartPos);
	IndexViewDlg.DoModal();

	m_pgridIndexGenerator->SetReverse(asGridRow.GetAt(nStartPos));
	
	return 0;
}

INT CSearchMono::CurrentIndexReproduce()
{
	INT ids, nSelectedRowIdx, nPos = 0;
	BOOL bInsertFrame;
	CString strMarc, strSpeciesKey, strError, strMarcTitle, strMarcInfo;
	CStringArray strAliasArray;
	CIndexDataMgr IndexMgr(this);


	if( IndexMgr.SetDataMgr() == -1 )	return -1;	
	if( m_pIndexGeneratorDm->GetRowCount() == 0 )		return -1;	
	if( m_pgridIndexGenerator == NULL )				return -1;

	nSelectedRowIdx = m_pgridIndexGenerator->GetIdx();

	// 기본값은 UpdateFrame이다.
	bInsertFrame = FALSE;
	strSpeciesKey.Empty();
	strAliasArray.RemoveAll();
	//InMarc 색인재생성
	m_pIndexGeneratorDm->GetCellData(_T("BS_MARC"), nSelectedRowIdx, strMarc);
	m_pIndexGeneratorDm->GetCellData(_T("IBS_종KEY"), nSelectedRowIdx, strSpeciesKey);
	// 인덱스 종키가 없다면 인덱스가 존재하지 않는 것이므로 종키를 복사하여 INSERT쿼리를 날린다.
	if( strSpeciesKey.IsEmpty() == TRUE )
	{
		m_pIndexGeneratorDm->GetCellData(_T("BS_종KEY"), nSelectedRowIdx, strSpeciesKey);
		bInsertFrame = TRUE;
	}
	

	// MARC에서 가져온 총서명과 판사항 값은 바뀌지 않으므로 그대로 유지.
	m_pCM->GetControlMgrData(_T("그리드"), strMarcTitle, nSelectedRowIdx, 4);
	m_pCM->GetControlMgrData(_T("그리드"), strMarcInfo, nSelectedRowIdx, 5);

	// 색인DM의 필드를 초기화 시킨다.
	IndexMgr.m_pIndexDM->InitDBFieldData();
	// Start Frame
	ids = IndexMgr.m_pIndexDM->StartFrame();
	if( ids < 0 )		return ErrorProc(nSelectedRowIdx);
	// 마크정보를 추출하여 배열에 저장한다.
	if( IndexMgr.ExtractIndexInMarc(strMarc, strAliasArray) < 0 )	return ErrorProc(nSelectedRowIdx);
	// 추출된 마크정보를 색인DM에 구축한다.
	if( IndexMgr.RestructIndexInMarc(&strAliasArray) < 0		)	return ErrorProc(nSelectedRowIdx);
	// 종키를 이용하여 책테이블의 정보를 색인 DM에 대입한다.
	if( IndexMgr.RestructIndexInSpecies(strSpeciesKey, m_bRecycleData) < 0 )	return ErrorProc(nSelectedRowIdx);		
	if( m_bRecycleData == TRUE )
	{
		if( IndexMgr.RestructIndexInDeleteBook(strSpeciesKey) < 0 )	return ErrorProc(nSelectedRowIdx);
	}
	else
	{
		if( IndexMgr.RestructIndexInBook(strSpeciesKey) < 0	)	return ErrorProc(nSelectedRowIdx);
	}
	// 인덱스가 존재하지 않을때에는 Insert 쿼리를 날린다.
	if( bInsertFrame == TRUE )
	{
		// 종구분값은 기본 2로 나타낸다
		if( m_bRecycleData == TRUE )
		{
			IndexMgr.m_pIndexDM->AddDBFieldData(_T("SPECIES_CLASS"), _T("STRING"), _T("2"));
		}

		if( IndexMgr.MakeInsert(strSpeciesKey) < 0 )	return ErrorProc(nSelectedRowIdx);
	}
	else
	{
		if( IndexMgr.MakeUpdate(strSpeciesKey) < 0 )	return ErrorProc(nSelectedRowIdx);
	}
	// 검색엔진과의 동기화를 맞추기위해 IDX_TMP_BO_TBL에 쿼리를 추가한다.
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	if( IndexMgr.SyncIndexWithSearchEngine(strSpeciesKey, IndexMgr.m_pIndexDM) < 0 ) return ErrorProc(nSelectedRowIdx);
	//=====================================================
	

	// 2004.11.11 은평도서관 임시 단행 색인재생성 ECO_SEARCH안씀.
	//if(IndexMgr.SyncIndexWithECO_SEARCH(strSpeciesKey, IndexMgr.m_pIndexDM) < 0 ) return ErrorProc(nSelectedRowIdx);
	
	// Send
	ids = IndexMgr.m_pIndexDM->SendFrame();
	if( ids < 0 )	return ErrorProc(nSelectedRowIdx);
	// End
	ids = IndexMgr.m_pIndexDM->EndFrame();
	if( ids < 0 )	return ErrorProc(nSelectedRowIdx);
	
	// 실패여부를 체크한다.
	if( IndexMgr.UpdateParentDM(strAliasArray, m_pIndexGeneratorDm, nSelectedRowIdx, _T("IBS")) < 0 )
	{
		strError.Format(_T("%s번째 책에 대한 색인재생성을 DM에 Update하던중 에러발생!"), nSelectedRowIdx);
		ESLAfxMessageBox(strError);
		return -1;
	}
	else
	{
		m_pgridIndexGenerator->DisplayLine(nSelectedRowIdx);
		m_pCM->SetControlMgrData(_T("그리드"), strMarcTitle, nSelectedRowIdx, 4);
		m_pCM->SetControlMgrData(_T("그리드"), strMarcInfo, nSelectedRowIdx, 5);
	}

	return 0;
}

//void CSearchMono::OnbtnMonoSyncIndex() 
//{
//	// TODO: Add your control notification handler code here
//	SyncIndexProc();
//}

//	if( m_pIndexGeneratorDm->GetRowCount() == 0 )		return -1;	
//	if( m_pgridIndexGenerator == NULL )	
//
//INT CSearchMono::SyncIndexProc()
//{
//	CString strDemonVersion;
//	m_strDANorYON.Format(_T("M"));  //단행 : _T("M"), 연속 : _T("S"), 기사 : _T("A")
//
//	// 색인동기화를 수행할건지 한번 더 체크.
//	CString strLine, strTemp, strTotalDBRecCnt;
//	INT nTotalDBRecCnt, Ids;
//	strTemp.Format(_T("SELECT COUNT(*) FROM IDX_BO_TBL"));
//	m_pIndexGeneratorDm->GetOneValueQuery(strTemp, strTotalDBRecCnt);
////	m_pIndexGeneratorDm = NULL;
//	nTotalDBRecCnt = _ttoi(strTotalDBRecCnt);
//
//
//	strTemp = PredictNecessaryTime(nTotalDBRecCnt);
//	strLine.Format(_T("총 %d 건의 데이터에 대해 색인동기화를 실행합니다.\n\n")
//				   _T("예상 소요시간은 %s 입니다.\n\n작업을 진행하시겠습니까?"), nTotalDBRecCnt, strTemp);
//
//	Ids = AfxMessageBox(strLine, MB_YESNO|MB_ICONINFORMATION);
//
//	if(Ids == IDNO) return 0;
//
//	CCtrlProgressThread *pThread;
//    pThread = new CCtrlProgressThread;
//    if (pThread != NULL)
//    {
//		ASSERT_VALID(pThread);
//        // Create Thread in a suspended state so we can set the Priority
//        // before it starts gettings away from us ...
//        if (!pThread->CreateThread(CREATE_SUSPENDED))
//        {
//			delete pThread;
//			return -1;
//        }
//
//		m_ThreadMap.SetAt(pThread->m_nThreadID, pThread);
//
//		DoEvents();
//		pThread->ResumeThread();
//
//		// set priority to normal for now ...
//        VERIFY(pThread->SetThreadPriority(THREAD_PRIORITY_NORMAL));
//
//		pThread->m_DANorYON = m_strDANorYON;
//		pThread->m_pTransferManager = this;
//	
//	}
//
//	return 0;
//}

CString CSearchMono::PredictNecessaryTime(INT nTotalDBRecCnt)
{
	CString strTime = _T("");
	INT hour=0; INT minute=0; INT second=0;
	
	second = (nTotalDBRecCnt / 300) + (nTotalDBRecCnt / 5000) * 5;   //평균 낸 수치 적용.
	if(second == 0) second = 1;

	minute = second / 60;
	second = second % 60;
	hour = minute / 60;
	minute = minute % 60;

	strTime.Format(_T("%d시간 %d분 %d초"), hour, minute, second);

	return strTime;

}


HBRUSH CSearchMono::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	int getdlgid = pWnd->GetDlgCtrlID();
	


	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG) 
	{	
		if( IDC_stcMonoGrid != pWnd->GetDlgCtrlID() )
		{
			pDC->SetBkMode(OPAQUE);		
			pDC->SetBkColor(RGB(244,243,238));	
			return m_Dlg_brush;		
		}
		
	}
	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}