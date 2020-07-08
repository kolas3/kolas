#include "Stdafx.h"
#include "SearchZipCodeDlgOpenAPI.h"
#include "SearchZipCodeDlgAuth.h"
#include "OpenAPI.h"
#include "CfgFileApi.h"

#include "..\\..\\단행\\열람\\장서관리\\LOC_CommonAPI\\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define OPENAPI_TAG_RESULTS			_T("results")
#define OPENAPI_TAG_COMMON			_T("common")
#define OPENAPI_TAG_TOTALCOUNT		_T("totalCount")
#define OPENAPI_TAG_CURRENTPAGE		_T("currentPage")
#define OPENAPI_TAG_COUNTPERPAGE	_T("countPerPage")
#define OPENAPI_TAG_ERRORCODE		_T("errorCode")
#define OPENAPI_TAG_ERRORMESSAGE	_T("errorMessage")
#define OPENAPI_TAG_JUSO			_T("juso")
#define OPENAPI_TAG_ROADADDR		_T("roadAddr")
#define OPENAPI_TAG_ROADADDRPART1	_T("roadAddrPart1")
#define OPENAPI_TAG_ROADADDRPART2	_T("roadAddrPart2")
#define OPENAPI_TAG_JIBUNADDR		_T("jibunAddr")
#define OPENAPI_TAG_ENGADDR			_T("engAddr")
#define OPENAPI_TAG_ZIPNO			_T("zipNo")
#define OPENAPI_TAG_ADMCD			_T("admCd")
#define OPENAPI_TAG_RNMGTSN			_T("rnMgtSn")
#define OPENAPI_TAG_BDMGTSN			_T("bdMgtSn")

#define OPENAPI_RESULT_FILE			_T("..\\cfg\\SearchRoadResult.xml")
#define OPENAPI_CONFIG_FILE			_T("..\\cfg\\LoanReturn\\newAddress_mode.txt")

#define OPENAPI_CONFIG_TAG_REGION		_T("지역구분")
#define OPENAPI_CONFIG_TAG_APPLYTYPE	_T("주소적용")

/////////////////////////////////////////////////////////////////////////////
// CSearchZipCodeDlgOpenAPI Dialog

CSearchZipCodeDlgOpenAPI::CSearchZipCodeDlgOpenAPI(CESL_Mgr* pParent)
	: CESL_Mgr(IDD, pParent)
{
	ASSERT(m_pInfo);

	CoInitialize(NULL);
}

CSearchZipCodeDlgOpenAPI::~CSearchZipCodeDlgOpenAPI()
{
	CoUninitialize();
}

void CSearchZipCodeDlgOpenAPI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchZipCodeDlgOpenAPI)
	DDX_Control(pDX, IDC_CMB_REGION, m_cmbRegion);
	DDX_Control(pDX, IDC_CMB_APPLY_TYPE, m_cmbApplyType);
	DDX_Control(pDX, IDC_EDT_SEARCH, m_edtSearch);	
	DDX_Control(pDX, IDC_EDT_CURRENT_PAGE, m_edtCurrentPage);
	DDX_Control(pDX, IDC_EDT_TOTAL_PAGE, m_edtTotalPage);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSearchZipCodeDlgOpenAPI, CDialog)
	//{{AFX_MSG_MAP(CSearchZipCodeDlgOpenAPI)
	ON_CBN_SELCHANGE(IDC_CMB_REGION, OnSelchangeRegion)
	ON_CBN_SELCHANGE(IDC_CMB_APPLY_TYPE, OnSelchangeApplyType)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnbtnSearch)
	ON_BN_CLICKED(IDC_BTN_PREV_PAGE, OnbtnPrevPage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, OnbtnNextPage)
	ON_BN_CLICKED(IDC_BTN_SETUP, OnbtnSetup)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnbtnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSearchZipCodeDlgOpenAPI, CDialog)
    //{{AFX_EVENTSINK_MAP(CSearchZipCodeDlgOpenAPI)
	ON_EVENT(CSearchZipCodeDlgOpenAPI, IDC_GRID_RESULT, -601 /* DblClick */, OnDblClickGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchZipCodeDlgOpenAPI message handlers


BOOL CSearchZipCodeDlgOpenAPI::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		if (GetDlgItem(IDC_BTN_SEARCH)->IsWindowEnabled())
		{
			OnbtnSearch();
		}
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSearchZipCodeDlgOpenAPI::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd());

	if(InitESL_Mgr(_T("SM_새주소OPENAPI검색")) < 0)
	{
		MessageBox(_T("InitESL_Mgr Error!"), _T("ERROR"));
		return FALSE;
	}
	
	// 인증키 확인
	InitAuthKey();
	// 컨트롤 초기화
	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSearchZipCodeDlgOpenAPI::OnSelchangeRegion()
{
	CCfgFileApi FileApi;
	FileApi.SetFile(OPENAPI_CONFIG_FILE);

	CString strData;
	strData.Format(_T("%d"), m_cmbRegion.GetCurSel());

	FileApi.SetData(OPENAPI_CONFIG_TAG_REGION, strData);
}

VOID CSearchZipCodeDlgOpenAPI::OnSelchangeApplyType()
{
	CCfgFileApi FileApi;
	FileApi.SetFile(OPENAPI_CONFIG_FILE);

	CString strData;
	strData.Format(_T("%d"), m_cmbApplyType.GetCurSel());

	FileApi.SetData(OPENAPI_CONFIG_TAG_APPLYTYPE, strData);
}

VOID CSearchZipCodeDlgOpenAPI::OnbtnSearch()
{
	CString strText;
	m_edtSearch.GetWindowText(strText);
	strText.TrimLeft();strText.TrimRight();

	if (strText.IsEmpty())
	{
		AfxMessageBox(_T("검색어를 입력해주시기 바랍니다"), MB_ICONINFORMATION);
		m_edtSearch.SetFocus();
		return;
	}
	
	// 지역정보를 추가한다
	INT idx = m_cmbRegion.GetCurSel();
	if (idx > 0)
	{
		CString strRegion;
		m_cmbRegion.GetLBText(idx, strRegion);
		strText = (strRegion + _T(" ") + strText);
	}

	SearchBeforeAction();

	SearchOpenAPI(strText, 1);

	SearchAfterAction();
}

VOID CSearchZipCodeDlgOpenAPI::OnbtnPrevPage()
{
	INT nCurrentPage = m_CommonInfo.CURRENTPAGE;

	CString strKeyWord = m_CommonInfo.LASTKEYWORD;

	SearchBeforeAction();

	SearchOpenAPI(strKeyWord, nCurrentPage - 1);

	SearchAfterAction();
}

VOID CSearchZipCodeDlgOpenAPI::OnbtnNextPage()
{
	INT nCurrentPage = m_CommonInfo.CURRENTPAGE;

	CString strKeyWord = m_CommonInfo.LASTKEYWORD;

	SearchBeforeAction();

	SearchOpenAPI(strKeyWord, nCurrentPage + 1);

	SearchAfterAction();
}

VOID CSearchZipCodeDlgOpenAPI::OnbtnSetup()
{
	// 테스트용 스왑
	CString strBeforeKey = m_strAuthKey;

	while (TRUE)
	{
		CSearchZipCodeDlgAuth dlg;
		if (dlg.DoModal() == IDOK)
		{			
			m_strAuthKey = dlg.GetAuthKey();

			// 검색을 시도하여 유효한 인증키인지 확인한다
			SearchOpenAPI(_T("국립중앙도서관"), 1, TRUE);

			if (m_CommonInfo.ERRORCODE != _T("0"))
			{
				if (IDYES == AfxMessageBox(_T("유효하지 않는 인증키입니다. 다시 입력하시겠습니까?"), MB_YESNO|MB_ICONQUESTION))
				{
					continue;
				}
			}
			else
			{
				if (strBeforeKey.IsEmpty())
				{
					strBeforeKey = m_strAuthKey;
				}

				CESL_DataMgr* pApiDM = FindDM(_T("DM_새주소OPENAPI_검색결과"));
				ASSERT(pApiDM);
				pApiDM->StartFrame();

				// DB UPDATE
				if (m_strAuthRecKey.IsEmpty())
				{
					// INSERT
					CString strQuery;
					strQuery.Format(_T("INSERT INTO ESL_MANAGE_TBL(REC_KEY, GROUP_1, GROUP_2, GROUP_3, CLASS_ALIAS, CLASS_DESC, VALUE_NAME, VALUE_1, SYS_SETUP_YN, MANAGE_CODE) ")
									_T("VALUES (ESL_SEQ.NEXTVAL, 'L', '기타', '공통', '도로명주소검색', '안전행정부 도로명주소 검색용 인증키', '인증키', '%s', 'Y', '%s');"),
									m_strAuthKey, m_pInfo->MANAGE_CODE );

					pApiDM->AddFrame(strQuery);
				}
				else
				{
					// UPDATE
					CString strQuery;
					strQuery.Format(_T("UPDATE ESL_MANAGE_TBL SET VALUE_1='%s' WHERE REC_KEY=%s;"), m_strAuthKey, m_strAuthRecKey);

					pApiDM->AddFrame(strQuery);
				}

				if (pApiDM->SendFrame() < 0)
				{
					AfxMessageBox(_T("[도로명주소 안내시스템] 인증키 저장에 실패하였습니다"), MB_ICONSTOP);
				}

				pApiDM->EndFrame();
			}
		}

		break;
	}

	// 이전값으로 복원
	m_strAuthKey = strBeforeKey;

	InitControl();
}

VOID CSearchZipCodeDlgOpenAPI::OnbtnApply()
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_새주소OPENAPI_검색"), _T("MAINGRID"));
	ASSERT(pGrid);

	INT idx = pGrid->GetIdx();
	if (idx < 0)
	{
		AfxMessageBox(_T("선택된 데이터가 없습니다"));
		return;
	}

	CESL_DataMgr* pApiDM = FindDM(_T("DM_새주소OPENAPI_검색결과"));
	ASSERT(pApiDM);

	CString strApplyType;
	m_cmbApplyType.GetLBText(m_cmbApplyType.GetCurSel(), strApplyType);

	if (strApplyType.CompareNoCase(_T("도로명주소")) == 0)
	{
		// 17/09/19 김혜영 : 도로명 주소검색시 세부주소가 조회되지 않는 현상 수정
/*//BEFORE-----------------------------------------------------------------------------
		pApiDM->GetCellData(_T("roadAddrPart1"), idx, m_strAddress);
*///AFTER------------------------------------------------------------------------------
		pApiDM->GetCellData(_T("roadAddr"), idx, m_strAddress);
/*//END-------------------------------------------------------------------------------*/
	}
	else
	{
		pApiDM->GetCellData(_T("jibunAddr"), idx, m_strAddress);
	}

	while (m_strAddress.Replace(_T("  "), _T(" ")) > 0)
	{
		// 문자열 공백 처리
	}

	pApiDM->GetCellData(_T("zipNo"), idx, m_strZipCode);

	CDialog::OnOK();	
}

VOID CSearchZipCodeDlgOpenAPI::OnDblClickGrid()
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_새주소OPENAPI_검색"), _T("MAINGRID"));
	ASSERT(pGrid);

	if (pGrid->GetMouseRow() == 0)
	{
		return;
	}

	CESL_DataMgr* pApiDM = FindDM(_T("DM_새주소OPENAPI_검색결과"));
	ASSERT(pApiDM);

	if (pApiDM->GetRowCount() == 0)
	{
		return;
	}

	OnbtnApply();
}

VOID CSearchZipCodeDlgOpenAPI::InitAuthKey()
{
	//rm.SetCONNECTION_INFO(m_DM.CONNECTION_INFO);
	CESL_DataMgr* pApiDM = FindDM(_T("DM_새주소OPENAPI_검색결과"));
	ASSERT(pApiDM);

	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO(pApiDM->CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(_T("SELECT REC_KEY, VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2='기타' AND GROUP_3='공통' AND CLASS_ALIAS='도로명주소검색' AND VALUE_NAME='인증키' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	
	DM.RestructDataManager(strQuery);
	if (DM.GetRowCount() < 1)	return;

	m_strAuthRecKey = DM.GetCellData(0, 0);
	m_strAuthKey = DM.GetCellData(0, 1); //m_strAuthKey = _T("bnVsbDIwMTQxMTE5MTQyODUx"); // DB에서 관리하도록 일단 테스트개발
}

VOID CSearchZipCodeDlgOpenAPI::InitControl()
{
	// 초기화
	{
		INT nCount = m_cmbRegion.GetCount();
		for (INT i=nCount-1; i>=0; i--)
		{
			m_cmbRegion.DeleteString(i);
		}
	}
	// 지역
	m_cmbRegion.AddString(_T("선택안함"));
	m_cmbRegion.AddString(_T("서울특별시"));
	m_cmbRegion.AddString(_T("부산광역시"));
	m_cmbRegion.AddString(_T("대구광역시"));
	m_cmbRegion.AddString(_T("인천광역시"));
	m_cmbRegion.AddString(_T("광주광역시"));
	m_cmbRegion.AddString(_T("대전광역시"));
	m_cmbRegion.AddString(_T("울산광역시"));
	m_cmbRegion.AddString(_T("세종특별자치시"));
	m_cmbRegion.AddString(_T("경기도"));
	m_cmbRegion.AddString(_T("강원도"));
	m_cmbRegion.AddString(_T("충청북도"));
	m_cmbRegion.AddString(_T("충청남도"));
	m_cmbRegion.AddString(_T("전라북도"));
	m_cmbRegion.AddString(_T("전라남도"));
	m_cmbRegion.AddString(_T("경상북도"));
	m_cmbRegion.AddString(_T("경상남도"));
	m_cmbRegion.AddString(_T("제주특별자치도"));
	m_cmbRegion.SetCurSel(0);

	// 초기화
	{
		INT nCount = m_cmbApplyType.GetCount();
		for (INT i=nCount-1; i>=0; i--)
		{
			m_cmbApplyType.DeleteString(i);
		}
	}
	// 결과적용 콤보 박스
	m_cmbApplyType.AddString(_T("도로명주소"));
	m_cmbApplyType.AddString(_T("지번주소"));
	m_cmbApplyType.SetCurSel(0);
	
	// 이전환경 로드
	CCfgFileApi FileApi;
	FileApi.SetFile(OPENAPI_CONFIG_FILE);

	CString strData;
	FileApi.GetData(OPENAPI_CONFIG_TAG_REGION, strData);
	if (!strData.IsEmpty())
	{
		m_cmbRegion.SetCurSel(_ttoi(strData));
	}

	FileApi.GetData(OPENAPI_CONFIG_TAG_APPLYTYPE, strData);
	if (!strData.IsEmpty())
	{
		m_cmbApplyType.SetCurSel(_ttoi(strData));
	}

	GetDlgItem(IDC_BTN_SEARCH)->EnableWindow(!m_strAuthKey.IsEmpty());
	GetDlgItem(IDC_BTN_SETUP)->EnableWindow(m_strAuthKey.IsEmpty());

	SearchBeforeAction();
}

BOOL CSearchZipCodeDlgOpenAPI::SearchOpenAPI(CString strKeyword, INT nCurPage/*=0*/, BOOL bTest/*=FALSE*/)
{
	if (!bTest) m_CommonInfo.LASTKEYWORD.Empty();

	//검색어 URL 인코딩
	CString strEncodingKeyword = CLocCommonAPI::UrlEncode((LPCTSTR)strKeyword);

	// 요청 URL
	// TODO: 별도로 관리해야하는가??
	CString strRequestURL = _T("http://www.juso.go.kr/addrlink/addrLinkApi.do");
	
	// 요청문자열
	CString strReqStream;
	strReqStream.Format(_T("%s?confmKey=%s&currentPage=%d&countPerPage=10&keyword=%s"), strRequestURL, m_strAuthKey, nCurPage, strEncodingKeyword);

	TRACE(_T("새주소요청문자열------------------------------------------------------\n"));
	TRACE(_T("%s\n"), strReqStream);
	TRACE(_T("새주소요청문자열------------------------------------------------------\n"));

	COpenAPI API;
	API.RequestURL(strReqStream);
	if (!API.GetErrorMessage().IsEmpty())
	{
		CString strMsg;
		strMsg.Format(_T("[도로명주소 안내시스템] 검색요청이 실패하였습니다 : %s"), API.GetErrorMessage());
		if (!bTest) AfxMessageBox(strMsg, MB_ICONSTOP);

		return FALSE;
	}

	// 인코딩 포맷을 강제로 UTF-16으로
	API.m_strSearchResult.Replace(_T("encoding=\"UTF-8\""), _T("encoding=\"UTF-16\""));

	if (!WriteXML(API.m_strSearchResult))
	{
		CString strMsg;
		strMsg.Format(_T("[도로명주소 안내시스템] 검색결과 파일생성에 실패하였습니다!!!"));
		if (!bTest) AfxMessageBox(strMsg, MB_ICONSTOP);

		return FALSE;
	}

	CString strFileName = OPENAPI_RESULT_FILE;
	CComVariant varFileName = (LPCTSTR)strFileName;

	IXMLDOMDocumentPtr XmlDom (_T("Microsoft.XMLDOM"), NULL, CLSCTX_INPROC_SERVER);
	if (!XmlDom->load(varFileName))
	{
		CString strMsg;
		strMsg.Format(_T("[도로명주소 안내시스템] 검색결과 해석에 실패하였습니다!!"));
		if (!bTest) AfxMessageBox(strMsg, MB_ICONSTOP);

		XmlDom.Release();
		return FALSE;
	}

	IXMLDOMNodePtr NodeRoot;
	{
		IXMLDOMNodeListPtr NodeList = XmlDom->childNodes;

		LONG lLength = NodeList->Getlength();
		for (LONG idx=0; idx<lLength; idx++)
		{
			IXMLDOMNodePtr Node = NodeList->item[idx];

			CComBSTR btrName;
			Node->get_baseName(&btrName);

			CString strName(btrName);
			if (strName.CompareNoCase(OPENAPI_TAG_RESULTS) == 0)
			{
				NodeRoot = Node;
			}

			Node.Release();
		}

		NodeList.Release();
	}

	// ROOT 노드 분석
	{
		IXMLDOMNodeListPtr NodeList = NodeRoot->childNodes;

		LONG lLength = NodeList->Getlength();
		for (LONG idx=0; idx<lLength; idx++)
		{
			IXMLDOMNodePtr Node = NodeList->item[idx];

			CComBSTR btrName;
			Node->get_baseName(&btrName);

			CString strName(btrName);
			if		(strName.CompareNoCase(OPENAPI_TAG_COMMON) == 0)
			{
				FetchCommonInfo(Node);		// 가장 먼저 처리되어야 한다
			}
			else if (strName.CompareNoCase(OPENAPI_TAG_JUSO) == 0)
			{
				if (!bTest) FetchJuso(Node);
			}

			Node.Release();
		}


		NodeList.Release();
	}

	NodeRoot.Release();

	XmlDom.Release();

	// 재검색용 키워드 저장
	if (!bTest) m_CommonInfo.LASTKEYWORD = strKeyword;

	// 17/09/18 김혜영 : 주소조회 결과가 없는 경우 메시지 처리
//*/ ADD ---------------------------------------------------------------------------
	if(m_CommonInfo.TOTALCOUNT <= 0)
	{
		MessageBox(_T("검색된 결과가 없습니다."), _T("주소조회"), MB_ICONEXCLAMATION);
	}
//*/ END ---------------------------------------------------------------------------
	
	return TRUE;
}

BOOL CSearchZipCodeDlgOpenAPI::SearchBeforeAction()
{
	m_CommonInfo.TOTALCOUNT   = 0;
	m_CommonInfo.CURRENTPAGE  = 0;
	m_CommonInfo.COUNTPERPAGE = 0;
	m_CommonInfo.ERRORCODE.Empty();
	m_CommonInfo.ERRORMESSAGE.Empty();
	m_CommonInfo.LASTKEYWORD.Empty();

	GetDlgItem(IDC_BTN_PREV_PAGE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NEXT_PAGE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_APPLY)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDT_CURRENT_PAGE)->SetWindowText(_T("0"));
	GetDlgItem(IDC_EDT_TOTAL_PAGE)->SetWindowText(_T("0"));

	CESL_DataMgr* pDM = FindDM(_T("DM_새주소OPENAPI_검색결과"));
	ASSERT(pDM);
	pDM->FreeData();

	return TRUE;
}

BOOL CSearchZipCodeDlgOpenAPI::SearchAfterAction()
{
	if (m_CommonInfo.ERRORCODE != _T("0"))
	{
		CString strMsg;
		if		(m_CommonInfo.ERRORCODE.CompareNoCase(_T("-999")) == 0)
		{
			strMsg.Format(_T("[도로명주소 안내시스템] 장애입니다!!! 조금후에 다시 시도하시기 바랍니다"));
		}
		else if (m_CommonInfo.ERRORCODE.CompareNoCase(_T("E0001")) == 0)
		{
			strMsg.Format(_T("[도로명주소 안내시스템]에서 승인되지 않은 인증키를 사용중입니다!!!"));
			GetDlgItem(IDC_BTN_SEARCH)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SETUP)->EnableWindow(TRUE);
		}
		else if (m_CommonInfo.ERRORCODE.CompareNoCase(_T("E0002")) == 0)
		{
			strMsg.Format(_T("[도로명주소 안내시스템]에서 승인되지 않은 SITE입니다!!!"));
		}
		else if (m_CommonInfo.ERRORCODE.CompareNoCase(_T("E0003")) == 0)
		{
			strMsg.Format(_T("정상적인 경로로 접속하시기 바랍니다."));
		}
		else if (m_CommonInfo.ERRORCODE.CompareNoCase(_T("E0004")) == 0)
		{
			strMsg.Format(_T("최대 검색결과는 1000건으로 제한되어 있습니다."));
		}
		else if (m_CommonInfo.ERRORCODE.CompareNoCase(_T("P0001")) == 0)
		{
			strMsg.Format(_T("검색결과가 너무 많습니다. 도로명주소 또는 지번을 입력하시기 바랍니다."));
		}

		SearchBeforeAction();

		AfxMessageBox(strMsg);
	}
	else
	{
		// 현재페이지가 첫페이지가 아니라면
		if (m_CommonInfo.CURRENTPAGE > 1)
		{
			GetDlgItem(IDC_BTN_PREV_PAGE)->EnableWindow(TRUE);
		}
		// 전체 갯수보다 현재페이지까지의 총합이 작다면
		if ((m_CommonInfo.CURRENTPAGE*m_CommonInfo.COUNTPERPAGE) < m_CommonInfo.TOTALCOUNT)
		{
			GetDlgItem(IDC_BTN_NEXT_PAGE)->EnableWindow(TRUE);
		}
		// 검색결과가 있다면
		if (m_CommonInfo.TOTALCOUNT > 0)
		{
			GetDlgItem(IDC_BTN_APPLY)->EnableWindow(TRUE);

			CString strCurPage;
			strCurPage.Format(_T("%d"), m_CommonInfo.CURRENTPAGE);
			GetDlgItem(IDC_EDT_CURRENT_PAGE)->SetWindowText(strCurPage);

			INT nTotalPage = (m_CommonInfo.TOTALCOUNT / m_CommonInfo.COUNTPERPAGE) + 1;

			CString strTotalPage;
			strTotalPage.Format(_T("%d"), nTotalPage);
			GetDlgItem(IDC_EDT_TOTAL_PAGE)->SetWindowText(strTotalPage);
		}
	}

	ControlDisplay(_T("CM_새주소OPENAPI_검색"), _T("MAINGRID"));

	return TRUE;
}

BOOL CSearchZipCodeDlgOpenAPI::FetchCommonInfo(IXMLDOMNodePtr NodeInfo)
{
	IXMLDOMNodeListPtr NodeList = NodeInfo->childNodes;

	LONG lLength = NodeList->Getlength();
	for (LONG idx=0; idx<lLength; idx++)
	{
		IXMLDOMNodePtr Node = NodeList->item[idx];

		CComBSTR btrName;
		CComBSTR btrText;
		Node->get_baseName(&btrName);
		Node->get_text(&btrText);

		CString strName(btrName);
		CString strText(btrText);
		if		(strName.CompareNoCase(OPENAPI_TAG_TOTALCOUNT) == 0)
		{
			m_CommonInfo.TOTALCOUNT = _ttoi(strText);
		}
		else if (strName.CompareNoCase(OPENAPI_TAG_CURRENTPAGE) == 0)
		{
			m_CommonInfo.CURRENTPAGE = _ttoi(strText);
		}
		else if (strName.CompareNoCase(OPENAPI_TAG_COUNTPERPAGE) == 0)
		{
			m_CommonInfo.COUNTPERPAGE = _ttoi(strText);
		}
		else if (strName.CompareNoCase(OPENAPI_TAG_ERRORCODE) == 0)
		{
			m_CommonInfo.ERRORCODE = strText;
		}
		else if (strName.CompareNoCase(OPENAPI_TAG_ERRORMESSAGE) == 0)
		{
			m_CommonInfo.ERRORMESSAGE = strText;
		}
		else
		{
			// I DON'T KNOW
		}

		Node.Release();
	}

	NodeList.Release();

	// ANALZE INFO

	return TRUE;
}

BOOL CSearchZipCodeDlgOpenAPI::FetchJuso(IXMLDOMNodePtr NodeJuso)
{
	CESL_DataMgr* pDM = FindDM(_T("DM_새주소OPENAPI_검색결과"));
	ASSERT(pDM);

	pDM->InsertRow(-1);
	INT nIdx = pDM->GetRowCount() - 1;

	IXMLDOMNodeListPtr NodeList = NodeJuso->childNodes;
	LONG lLength = NodeList->Getlength();
	for (LONG idx=0; idx<lLength; idx++)
	{
		IXMLDOMNodePtr Node = NodeList->item[idx];

		CComBSTR btrName;
		CComBSTR btrText;
		Node->get_baseName(&btrName);
		Node->get_text(&btrText);

		CString strName(btrName);
		CString strText(btrText);

		pDM->SetCellData(strName, strText, nIdx);

		Node.Release();
	}

	NodeList.Release();

	return TRUE;
}

BOOL CSearchZipCodeDlgOpenAPI::WriteXML(CString strResultXML)
{
	FILE* fp = _tfopen(OPENAPI_RESULT_FILE, _T("wb"));
	if (fp == NULL)
	{
		return FALSE;
	}

	fputc(0xFF, fp);
	fputc(0xFE, fp);

	// WRITE RESULT
	_ftprintf (fp, _T("%s"), strResultXML);
		
	fclose(fp);

	return TRUE;
}