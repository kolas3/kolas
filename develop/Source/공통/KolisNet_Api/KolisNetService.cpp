// 2017/09/21 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// KOLIS-NET OpenAPI

#include "Stdafx.h"
#include "KolisNetService.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//{{ 테스트용 플래그
//#define KOLISNET_TEST_YN
//#define KOLISNET_NO_COMMIT
//}}

#ifdef KOLISNET_NO_COMMIT
#define KOLISNET_COMMIT_FLAG	_T("N")
#else
#define KOLISNET_COMMIT_FLAG	_T("Y")
#endif //KOLISNET_NO_COMMIT

CKolisNetSender g_Sender;

CString	g_strURL;

CKolisNetDataMapper::CKolisNetDataMapper()
{
}

CKolisNetDataMapper::~CKolisNetDataMapper()
{
	ReleaseData();
}

VOID CKolisNetDataMapper::ReleaseData()
{
	INT nCount = m_DataList.GetSize();
	for (INT i=0; i<nCount; i++)
	{
		_DATA_MAP* pItem = m_DataList.GetAt(i);
		ASSERT(pItem);
		delete pItem;
	}
	m_DataList.RemoveAll();
}

INT CKolisNetDataMapper::AddData(CString strTagName, CString strColName)
{
	if (strTagName.IsEmpty() || strColName.IsEmpty())
	{
		return -1;
	}
	
	// 중복예외처리는 하지 않는다
	_DATA_MAP* pItem = new _DATA_MAP;
	ASSERT(pItem);

	pItem->strTagName = strTagName;
	pItem->strColName = strColName;
	
	return m_DataList.Add(pItem);
}

INT CKolisNetDataMapper::GetSize()
{
	return m_DataList.GetSize();
}

INT	CKolisNetDataMapper::GetAt(INT nIndex, CString &strTagName, CString &strColName)
{
	if (nIndex < 0 || nIndex >= m_DataList.GetSize())
	{
		// OUT RANGE
		return -1;
	}

	_DATA_MAP* pItem = m_DataList.GetAt(nIndex);
	ASSERT(pItem);

	strTagName = pItem->strTagName;
	strColName = pItem->strColName;

	return nIndex;
}

CString CKolisNetDataMapper::FindTagName(CString strColName)
{
	INT nCount = m_DataList.GetSize();
	for (INT i=0; i<nCount; i++)
	{
		_DATA_MAP* pItem = m_DataList.GetAt(i);
		ASSERT(pItem);

		if (strColName.Compare(pItem->strColName) == 0)
		{
			return pItem->strTagName;
		}
	}

	// NOT FOUND
	return _T("");
}

CString CKolisNetDataMapper::FindColName(CString strTagName)
{
	INT nCount = m_DataList.GetSize();
	for (INT i=0; i<nCount; i++)
	{
		_DATA_MAP* pItem = m_DataList.GetAt(i);
		ASSERT(pItem);

		if (strTagName.Compare(pItem->strTagName) == 0)
		{
			return pItem->strColName;
		}
	}

	// NOT FOUND
	return _T("");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KOLISNETPARAM_PATTERN_STR(LPSENDER, ATTR)	\
	if (!ATTR.IsEmpty())	\
	{	\
		LPSENDER->AddParam(KOLISNET_PARAM_##ATTR, ATTR);	\
	}
#define KOLISNETPARAM_PATTERN_STR2(LPSENDER, ATTR, VAL)	\
	if (!ATTR.IsEmpty())	\
	{	\
		LPSENDER->AddParam(KOLISNET_PARAM_##ATTR, VAL);	\
	}
#define KOLISNETPARAM_ADD_STR(PARAM, ATTR)	\
	if (!ATTR.IsEmpty())	\
	{	\
		g_Sender.AddParam(PARAM, ATTR);	\
	}


#define KOLISNETREADER_REQUEST_SERVICE(READER, SENDER, LPMGR)	\
	CJsonReader READER;	\
	if (!CKolisNetService::_RequestKolisNet(LPMGR, &SENDER, &READER))	\
	{	\
		return FALSE;	\
	}	\
	CString strRetCode;	\
	if (READER.get_resultCode_Value(strRetCode) < 0)	\
	{	\
		return FALSE;	\
	}	\
	if (strRetCode.CompareNoCase(_T("0")) != 0)	\
	{	\
		return FALSE;	\
	}
	
////////////////////////////////////////////////////////////////////////////////////////////////////////

CKolisNetSearchParam::CKolisNetSearchParam()
{
	ReleaseParam();
}

CKolisNetSearchParam::~CKolisNetSearchParam()
{
}

VOID CKolisNetSearchParam::ReleaseParam()
{
	PUB_FORM_TYPE = PUB_FORM_TYPE::PUB_FORM_TYPE_UB;

	SORT_TITLE = SORT_TYPE::SORT_TYPE_NONE;
	SORT_AUTHOR = SORT_TYPE::SORT_TYPE_NONE;
	SORT_PUBLISHER = SORT_TYPE::SORT_TYPE_NONE;
	SORT_PUB_YEAR = SORT_TYPE::SORT_TYPE_NONE;

	// DEFAULT
	PAGENUM  = 1;
	PAGESIZE = 1000;

	KEYWORD_TYPE_1 = KEYWORD_TYPE::KEYWORD_TYPE_NONE;
	KEYWORD_TYPE_2 = KEYWORD_TYPE::KEYWORD_TYPE_NONE;
	KEYWORD_TYPE_3 = KEYWORD_TYPE::KEYWORD_TYPE_NONE;
	KEYWORD_TYPE_4 = KEYWORD_TYPE::KEYWORD_TYPE_NONE;

	KEYWORD_CONDITION_2 = KEYWORD_CONDITION_TYPE::KEYWORD_CONDITION_TYPE_NONE;
	KEYWORD_CONDITION_3 = KEYWORD_CONDITION_TYPE::KEYWORD_CONDITION_TYPE_NONE;
	KEYWORD_CONDITION_4 = KEYWORD_CONDITION_TYPE::KEYWORD_CONDITION_TYPE_NONE;

	TITLE_TYPE = SEARCH_TYPE::SEARCH_TYPE_NONE;
	AUTHOR_TYPE = SEARCH_TYPE::SEARCH_TYPE_NONE;
	PUBLISHER_TYPE = SEARCH_TYPE::SEARCH_TYPE_NONE;
	
	KOR_VIEW_YN.Empty();
	COLL_TYPE_1.Empty();
	COLL_TYPE_2.Empty();
	COLL_TYPE_NOT.Empty();
	KEYWORD_1.Empty();
	KEYWORD_2.Empty();
	KEYWORD_3.Empty();
	KEYWORD_4.Empty();
	TITLE.Empty();
	AUTHOR.Empty();
	PUBLISHER.Empty();
	CONTROL_NO.Empty();
	ISBN.Empty();
	START_CONTROL_NO.Empty();
	END_CONTROL_NO.Empty();

	SUBJECT.Empty();
	ISSN.Empty();
	STRN.Empty();
	RNSTRN.Empty();
	NBCODE.Empty();
	CODEN.Empty();
	IRSC.Empty();
	UPC.Empty();
	ISMN.Empty();
	EAN.Empty();
	SICI.Empty();
	GPO_CA.Empty();
	CIP_CONTROL_NO.Empty();
	START_CIP_CONTROL_NO.Empty();
	END_CIP_CONTROL_NO.Empty();
	KDC.Empty();
	KDCP.Empty();
	DDC.Empty();
	CEC.Empty();
	CWC.Empty();
	COC.Empty();
	NDC.Empty();
	LC.Empty();
	UDC.Empty();
	UNI_CODE.Empty();
	GOV_CODE.Empty();
	LIB_CODE.Empty();
	NA_CODE.Empty();
	DEGREE_ORG.Empty();
	DEG_CODE.Empty();
	MAT_CODE.Empty();
	MEDIA_CODE.Empty();
	TEXT_LANG.Empty();
	SUM_LANG.Empty();
	PUB_FREQ.Empty();
	IPUB_YEAR.Empty();
	START_IPUB_YEAR.Empty();
	END_IPUB_YEAR.Empty();
	USER_LIMIT_CODE.Empty();
	PUB_FORM_CODE.Empty();
	TOC_YN.Empty();
	ABS_YN.Empty();
	TYPE_CODE.Empty();
	USE_LIMIT_YN.Empty();
	START_UPLOAD_TERM.Empty();
	END_UPLOAD_TERM.Empty();
	START_TOC_CREATE_DATE.Empty();
	END_TOC_CREATE_DATE.Empty();
	START_TOC_UPDATE_DATE.Empty();
	END_TOC_UPDATE_DATE.Empty();
	START_ABS_CREATE_DATE.Empty();
	END_ABS_CREATE_DATE.Empty();
	START_ABS_UPDATE_DATE.Empty();
	END_ABS_UPDATE_DATE.Empty();
}

VOID CKolisNetSearchParam::SettingKolisNetSender(CKolisNetSender* pSender)
{
	if (pSender == NULL)
	{
		return;
	}

	if (SORT_TITLE != SORT_TYPE::SORT_TYPE_NONE)
	{
		pSender->AddParam(KOLISNET_PARAM_SORT_TITLE, GetSortTypeAlias(SORT_TITLE));
	}
	if (SORT_AUTHOR != SORT_TYPE::SORT_TYPE_NONE)
	{
		pSender->AddParam(KOLISNET_PARAM_SORT_AUTHOR, GetSortTypeAlias(SORT_AUTHOR));
	}
	if (SORT_PUBLISHER != SORT_TYPE::SORT_TYPE_NONE)
	{
		pSender->AddParam(KOLISNET_PARAM_SORT_PUBLISHER, GetSortTypeAlias(SORT_PUBLISHER));
	}
	if (SORT_PUB_YEAR != SORT_TYPE::SORT_TYPE_NONE)
	{
		pSender->AddParam(KOLISNET_PARAM_SORT_PUB_YEAR, GetSortTypeAlias(SORT_PUB_YEAR));
	}

	switch (PUB_FORM_TYPE)
	{
	case PUB_FORM_TYPE::PUB_FORM_TYPE_UB:
		pSender->AddParam(KOLISNET_PARAM_PUB_FORM_CODE, _T("UB"));
		break;
	case PUB_FORM_TYPE::PUB_FORM_TYPE_UT:
		pSender->AddParam(KOLISNET_PARAM_PUB_FORM_CODE, _T("UT"));
		break;
	}	

	if (PAGENUM > 0)
	{
		CString strPageNum;
		strPageNum.Format(_T("%d"), PAGENUM);
		pSender->AddParam(KOLISNET_PARAM_PAGENUM, strPageNum);
	}
	if (PAGESIZE > 0)
	{
		CString strPageSize;
		strPageSize.Format(_T("%d"), PAGESIZE);
		pSender->AddParam(KOLISNET_PARAM_PAGESIZE, strPageSize);
	}

	KOLISNETPARAM_PATTERN_STR(pSender, COLL_TYPE_1)
	KOLISNETPARAM_PATTERN_STR(pSender, COLL_TYPE_2)
	KOLISNETPARAM_PATTERN_STR(pSender, COLL_TYPE_NOT)
	
	if (!KEYWORD_1.IsEmpty() && KEYWORD_TYPE_1 != KEYWORD_TYPE::KEYWORD_TYPE_NONE)
	{
		pSender->AddParam(KOLISNET_PARAM_KEYWORD_1, KEYWORD_1);
		pSender->AddParam(KOLISNET_PARAM_KEYWORD_TYPE_1, GetKeywordTypeAlias(KEYWORD_TYPE_1));
	}
	if (!KEYWORD_2.IsEmpty() && KEYWORD_TYPE_2 != KEYWORD_TYPE::KEYWORD_TYPE_NONE && KEYWORD_CONDITION_2 != KEYWORD_CONDITION_TYPE::KEYWORD_CONDITION_TYPE_NONE)
	{
		pSender->AddParam(KOLISNET_PARAM_KEYWORD_2, KEYWORD_2);
		pSender->AddParam(KOLISNET_PARAM_KEYWORD_TYPE_2, GetKeywordTypeAlias(KEYWORD_TYPE_2));
		pSender->AddParam(KOLISNET_PARAM_KEYWORD_CONDITION_2, GetKeywordConditionTypeAlias(KEYWORD_CONDITION_2));
	}
	if (!KEYWORD_3.IsEmpty() && KEYWORD_TYPE_3 != KEYWORD_TYPE::KEYWORD_TYPE_NONE && KEYWORD_CONDITION_3 != KEYWORD_CONDITION_TYPE::KEYWORD_CONDITION_TYPE_NONE)
	{
		pSender->AddParam(KOLISNET_PARAM_KEYWORD_3, KEYWORD_3);
		pSender->AddParam(KOLISNET_PARAM_KEYWORD_TYPE_3, GetKeywordTypeAlias(KEYWORD_TYPE_3));
		pSender->AddParam(KOLISNET_PARAM_KEYWORD_CONDITION_3, GetKeywordConditionTypeAlias(KEYWORD_CONDITION_3));
	}
	if (!KEYWORD_4.IsEmpty() && KEYWORD_TYPE_4 != KEYWORD_TYPE::KEYWORD_TYPE_NONE && KEYWORD_CONDITION_4 != KEYWORD_CONDITION_TYPE::KEYWORD_CONDITION_TYPE_NONE)
	{
		pSender->AddParam(KOLISNET_PARAM_KEYWORD_4, KEYWORD_4);
		pSender->AddParam(KOLISNET_PARAM_KEYWORD_TYPE_4, GetKeywordTypeAlias(KEYWORD_TYPE_4));
		pSender->AddParam(KOLISNET_PARAM_KEYWORD_CONDITION_4, GetKeywordConditionTypeAlias(KEYWORD_CONDITION_4));
	}

	KOLISNETPARAM_PATTERN_STR(pSender, TITLE)
	
	if (TITLE_TYPE > 0)
	{
		pSender->AddParam(KOLISNET_PARAM_TITLE_TYPE, GetSearchTypeAlias(TITLE_TYPE));
	}

	KOLISNETPARAM_PATTERN_STR(pSender, AUTHOR)

	if (AUTHOR_TYPE > 0)
	{
		pSender->AddParam(KOLISNET_PARAM_AUTHOR_TYPE, GetSearchTypeAlias(AUTHOR_TYPE));
	}

	KOLISNETPARAM_PATTERN_STR(pSender, PUBLISHER)

	if (PUBLISHER_TYPE > 0)
	{
		pSender->AddParam(KOLISNET_PARAM_PUBLISHER_TYPE, GetSearchTypeAlias(PUBLISHER_TYPE));
	}

	KOLISNETPARAM_PATTERN_STR(pSender, CONTROL_NO)
	KOLISNETPARAM_PATTERN_STR(pSender, ISBN)
	KOLISNETPARAM_PATTERN_STR(pSender, START_CONTROL_NO)
	KOLISNETPARAM_PATTERN_STR(pSender, END_CONTROL_NO)
	KOLISNETPARAM_PATTERN_STR(pSender, SUBJECT)
	KOLISNETPARAM_PATTERN_STR(pSender, ISSN)
	KOLISNETPARAM_PATTERN_STR(pSender, STRN)
	KOLISNETPARAM_PATTERN_STR(pSender, RNSTRN)
	KOLISNETPARAM_PATTERN_STR(pSender, NBCODE)
	KOLISNETPARAM_PATTERN_STR(pSender, CODEN)
	KOLISNETPARAM_PATTERN_STR(pSender, IRSC)
	KOLISNETPARAM_PATTERN_STR(pSender, UPC)
	KOLISNETPARAM_PATTERN_STR(pSender, ISMN)
	KOLISNETPARAM_PATTERN_STR(pSender, EAN)
	KOLISNETPARAM_PATTERN_STR(pSender, SICI)
	KOLISNETPARAM_PATTERN_STR(pSender, GPO_CA)
	KOLISNETPARAM_PATTERN_STR(pSender, CIP_CONTROL_NO)
	KOLISNETPARAM_PATTERN_STR(pSender, START_CIP_CONTROL_NO)
	KOLISNETPARAM_PATTERN_STR(pSender, END_CIP_CONTROL_NO)
	KOLISNETPARAM_PATTERN_STR(pSender, KDC)
	KOLISNETPARAM_PATTERN_STR(pSender, KDCP)
	KOLISNETPARAM_PATTERN_STR(pSender, DDC)
	KOLISNETPARAM_PATTERN_STR(pSender, CEC)
	KOLISNETPARAM_PATTERN_STR(pSender, CWC)
	KOLISNETPARAM_PATTERN_STR(pSender, COC)
	KOLISNETPARAM_PATTERN_STR(pSender, NDC)
	KOLISNETPARAM_PATTERN_STR(pSender, LC)
	KOLISNETPARAM_PATTERN_STR(pSender, UDC)
	KOLISNETPARAM_PATTERN_STR(pSender, UNI_CODE)
	KOLISNETPARAM_PATTERN_STR(pSender, GOV_CODE)
	KOLISNETPARAM_PATTERN_STR(pSender, LIB_CODE)
	KOLISNETPARAM_PATTERN_STR(pSender, NA_CODE)
	KOLISNETPARAM_PATTERN_STR(pSender, DEGREE_ORG)
	KOLISNETPARAM_PATTERN_STR(pSender, DEG_CODE)
	KOLISNETPARAM_PATTERN_STR(pSender, MAT_CODE)
	KOLISNETPARAM_PATTERN_STR(pSender, MEDIA_CODE)
	KOLISNETPARAM_PATTERN_STR(pSender, TEXT_LANG)
	KOLISNETPARAM_PATTERN_STR(pSender, SUM_LANG)
	KOLISNETPARAM_PATTERN_STR(pSender, PUB_FREQ)
	KOLISNETPARAM_PATTERN_STR(pSender, IPUB_YEAR)
	KOLISNETPARAM_PATTERN_STR(pSender, START_IPUB_YEAR)
	KOLISNETPARAM_PATTERN_STR(pSender, END_IPUB_YEAR)
	KOLISNETPARAM_PATTERN_STR(pSender, USER_LIMIT_CODE)
	KOLISNETPARAM_PATTERN_STR(pSender, PUB_FORM_CODE)
	KOLISNETPARAM_PATTERN_STR(pSender, TOC_YN)
	KOLISNETPARAM_PATTERN_STR(pSender, ABS_YN)
	KOLISNETPARAM_PATTERN_STR(pSender, TYPE_CODE)
	KOLISNETPARAM_PATTERN_STR(pSender, USE_LIMIT_YN)

	KOLISNETPARAM_PATTERN_STR2(pSender, START_UPLOAD_TERM, CKolisNetService::GetDateString(START_UPLOAD_TERM))
	KOLISNETPARAM_PATTERN_STR2(pSender, END_UPLOAD_TERM, CKolisNetService::GetDateString(END_UPLOAD_TERM))
	KOLISNETPARAM_PATTERN_STR2(pSender, START_TOC_CREATE_DATE, CKolisNetService::GetDateString(START_TOC_CREATE_DATE))
	KOLISNETPARAM_PATTERN_STR2(pSender, END_TOC_CREATE_DATE, CKolisNetService::GetDateString(END_TOC_CREATE_DATE))
	KOLISNETPARAM_PATTERN_STR2(pSender, START_TOC_UPDATE_DATE, CKolisNetService::GetDateString(START_TOC_UPDATE_DATE))
	KOLISNETPARAM_PATTERN_STR2(pSender, END_TOC_UPDATE_DATE, CKolisNetService::GetDateString(END_TOC_UPDATE_DATE))
	KOLISNETPARAM_PATTERN_STR2(pSender, START_ABS_CREATE_DATE, CKolisNetService::GetDateString(START_ABS_CREATE_DATE))
	KOLISNETPARAM_PATTERN_STR2(pSender, END_ABS_CREATE_DATE, CKolisNetService::GetDateString(END_ABS_CREATE_DATE))
	KOLISNETPARAM_PATTERN_STR2(pSender, START_ABS_UPDATE_DATE, CKolisNetService::GetDateString(START_ABS_UPDATE_DATE))
	KOLISNETPARAM_PATTERN_STR2(pSender, END_ABS_UPDATE_DATE, CKolisNetService::GetDateString(END_ABS_UPDATE_DATE))
}

CString CKolisNetSearchParam::GetSortTypeAlias(INT nSortType)
{
	CString strAlias;

	switch (nSortType)
	{
	case SORT_TYPE_ASC:		strAlias = _T("asc");		break;
	case SORT_TYPE_DESC:	strAlias = _T("desc");		break;
	}

	return strAlias;
}

CString CKolisNetSearchParam::GetKeywordTypeAlias(INT nKeywordType)
{
	CString strAlias;

	switch (nKeywordType)
	{
	case KEYWORD_TYPE_TITLE:		strAlias = _T("title");		break;
	case KEYWORD_TYPE_AUTHOR:		strAlias = _T("author");	break;
	case KEYWORD_TYPE_PUBLISHER:	strAlias = _T("publisher");	break;
	case KEYWORD_TYPE_KEYWORD:		strAlias = _T("keyword");	break;
	case KEYWORD_TYPE_TOTAL:		strAlias = _T("total");		break;
	}

	return strAlias;
}

CString CKolisNetSearchParam::GetKeywordConditionTypeAlias(INT nKeywordConditionType)
{
	CString strAlias;

	switch (nKeywordConditionType)
	{
	case KEYWORD_CONDITION_TYPE_AND:	strAlias = _T("AND");		break;
	case KEYWORD_CONDITION_TYPE_OR:		strAlias = _T("OR");		break;
	}

	return strAlias;
}

CString CKolisNetSearchParam::GetSearchTypeAlias(INT nSearchType)
{
	CString strAlias;

	switch (nSearchType)
	{
	case SEARCH_TYPE_RIGHT:	strAlias = _T("right");		break;
	case SEARCH_TYPE_EQUAL:	strAlias = _T("equal");		break;
	}

	return strAlias;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
CKolisNetLibInfoParam::CKolisNetLibInfoParam()
{
}
CKolisNetLibInfoParam::~CKolisNetLibInfoParam()
{
}

VOID CKolisNetLibInfoParam::SettingKolisNetSender(CKolisNetSender* pSender)
{
	if (pSender == NULL)
	{
		return;
	}

	KOLISNETPARAM_PATTERN_STR(pSender, LIB_CODE)
	KOLISNETPARAM_PATTERN_STR(pSender, LIB_NAME)
	KOLISNETPARAM_PATTERN_STR(pSender, DIVISION_NAME)
	KOLISNETPARAM_PATTERN_STR(pSender, GRADE)
	KOLISNETPARAM_PATTERN_STR(pSender, LOCAL)
	KOLISNETPARAM_PATTERN_STR(pSender, E_MAIL)
	KOLISNETPARAM_PATTERN_STR(pSender, HOME_PAGE)
	KOLISNETPARAM_PATTERN_STR(pSender, ZIP_CODE)
	KOLISNETPARAM_PATTERN_STR(pSender, ADDRESS)
	KOLISNETPARAM_PATTERN_STR(pSender, TEL)
	KOLISNETPARAM_PATTERN_STR(pSender, FAX)
	KOLISNETPARAM_PATTERN_STR(pSender, ALL_OPEN)
	KOLISNETPARAM_PATTERN_STR(pSender, SUMMER_OPEN_TIME)
	KOLISNETPARAM_PATTERN_STR(pSender, WINTER_OPEN_TIME)
	KOLISNETPARAM_PATTERN_STR(pSender, REGIST_DATE)
	KOLISNETPARAM_PATTERN_STR(pSender, REMARK)
	KOLISNETPARAM_PATTERN_STR(pSender, COPY_PRICE)
	KOLISNETPARAM_PATTERN_STR(pSender, UC_ILL_Y_N)
	KOLISNETPARAM_PATTERN_STR(pSender, UC_ORGIN_Y_N)
	KOLISNETPARAM_PATTERN_STR(pSender, KOLASII_HOME_PAGE)
	KOLISNETPARAM_PATTERN_STR(pSender, UPDATE_DATE)
	KOLISNETPARAM_PATTERN_STR(pSender, FIRST_WORK)
	KOLISNETPARAM_PATTERN_STR(pSender, LAST_WORK)
	KOLISNETPARAM_PATTERN_STR(pSender, DIRECT_UPLOAD_YN)
	KOLISNETPARAM_PATTERN_STR(pSender, PASSWORD)
	KOLISNETPARAM_PATTERN_STR(pSender, LOAN_DATE)
	KOLISNETPARAM_PATTERN_STR(pSender, KOLISNET_USE_YN)		
	KOLISNETPARAM_PATTERN_STR(pSender, USEING_CLIENT_PG)
	KOLISNETPARAM_PATTERN_STR(pSender, KEYWORD)
	KOLISNETPARAM_PATTERN_STR(pSender, PAPER_LIST)	
	KOLISNETPARAM_PATTERN_STR(pSender, APPLY_DATE)
	KOLISNETPARAM_PATTERN_STR(pSender, ALL_CLOSURE)
	KOLISNETPARAM_PATTERN_STR(pSender, WORKER)
	KOLISNETPARAM_PATTERN_STR(pSender, GROUP_CODE)
	KOLISNETPARAM_PATTERN_STR(pSender, INPUT_WORKER)
	KOLISNETPARAM_PATTERN_STR(pSender, INPUT_WORK_DATE)
	KOLISNETPARAM_PATTERN_STR(pSender, UPDATE_WORKER)
	KOLISNETPARAM_PATTERN_STR(pSender, UPDATE_WORK_DATE)
	KOLISNETPARAM_PATTERN_STR(pSender, VIEW_PRIV)
	KOLISNETPARAM_PATTERN_STR(pSender, CLASSIC_USE_YN)
	KOLISNETPARAM_PATTERN_STR(pSender, KOLIS_NEW_YN)
	KOLISNETPARAM_PATTERN_STR(pSender, KOLISNET_NEW_YN)
	KOLISNETPARAM_PATTERN_STR(pSender, FOREIGN_LIB_YN)
	KOLISNETPARAM_PATTERN_STR(pSender, LIB_KEYWORD)
	KOLISNETPARAM_PATTERN_STR(pSender, BLIND_USE_YN)
	KOLISNETPARAM_PATTERN_STR(pSender, MARC_DOWN_YN)
	KOLISNETPARAM_PATTERN_STR(pSender, CLIENT_TYPE)
	KOLISNETPARAM_PATTERN_STR(pSender, INSTALL_TYPE)
	KOLISNETPARAM_PATTERN_STR(pSender, INSTALL_DATE)
	KOLISNETPARAM_PATTERN_STR(pSender, CHARSET_TYPE)
	KOLISNETPARAM_PATTERN_STR(pSender, REFERENCE)
	KOLISNETPARAM_PATTERN_STR(pSender, REVERSE_REFERENCE)
	KOLISNETPARAM_PATTERN_STR(pSender, MANAGE_CLASS)
	KOLISNETPARAM_PATTERN_STR(pSender, KOLISNET_CANCEL_YN)
	KOLISNETPARAM_PATTERN_STR(pSender, ACCOUNT_BANK)
	KOLISNETPARAM_PATTERN_STR(pSender, ACCOUNT_NUM)
	KOLISNETPARAM_PATTERN_STR(pSender, ACCOUNT_NAME)
	KOLISNETPARAM_PATTERN_STR(pSender, USEING_CLIENT_KORCIS_PG)
	KOLISNETPARAM_PATTERN_STR(pSender, LATITUDE)
	KOLISNETPARAM_PATTERN_STR(pSender, LONGITUDE)
	KOLISNETPARAM_PATTERN_STR(pSender, DUP_CHECK_PRIORITY)
	KOLISNETPARAM_PATTERN_STR(pSender, DISUSE_YN)
	KOLISNETPARAM_PATTERN_STR(pSender, LIST_BUILD_AUTHORITY)
	KOLISNETPARAM_PATTERN_STR(pSender, USE_API_YN)
	KOLISNETPARAM_PATTERN_STR(pSender, API_AUTH_KEY)	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

CKolisNetService::CKolisNetService()
{
}

CKolisNetService::~CKolisNetService()
{
}

BOOL CKolisNetService::_RequestKolisNet(CESL_Mgr* pMgr, CKolisNetSender* pSender, CJsonReader* pReader)
{
	if (pMgr == NULL || pSender == NULL || pReader == NULL)
	{
		return FALSE;
	}

	if (pMgr->m_pInfo)
	{
		pSender->AddParam(KOLISNET_PARAM_LOG_SVC_CD, _T("KOLAS III"));
		pSender->AddParam(KOLISNET_PARAM_LOG_LIB_CODE, pMgr->m_pInfo->LIB_CODE);

		// PDJ ADD : Local IP IS NULL 센터에서 -1 리턴함
		if (pMgr->m_pInfo->LOCAL_IP.IsEmpty()) pMgr->m_pInfo->LOCAL_IP=_T("127.0.0.1");

		pSender->AddParam(KOLISNET_PARAM_LOG_CLIENT_IP, pMgr->m_pInfo->LOCAL_IP);
		pSender->AddParam(KOLISNET_PARAM_LOG_USER_ID, pMgr->m_pInfo->USER_ID);
	}
	else
	{
		// 로그 필드를 설정하지 않을 경우 결과값을 받을 수 없기 때문에 부득이하게 기본값 지정
		pSender->AddParam(KOLISNET_PARAM_LOG_SVC_CD, _T("KOLAS III"));
		pSender->AddParam(KOLISNET_PARAM_LOG_LIB_CODE, _T("999999"));
		pSender->AddParam(KOLISNET_PARAM_LOG_CLIENT_IP, _T("1.1.1.1"));
		pSender->AddParam(KOLISNET_PARAM_LOG_USER_ID, _T("kolas"));
	}

#ifdef KOLISNET_TEST_YN
	pSender->AddParam(KOLISNET_PARAM_TESTYN, _T("Y"));
#else
	pSender->AddParam(KOLISNET_PARAM_TESTYN, _T("N"));
#endif

	if (g_strURL.IsEmpty())
	{
		CString strValue;
		pMgr->GetManageValue(_T("기타"), _T("공통"), _T("KOLIS-NET"), _T("OPEN_API_URL"), strValue);

		g_strURL = strValue;
	}
	pSender->SetUrl(g_strURL);

	// 실제 통신 부분
	CString strFrame;
	if (!pSender->KolisNetRequest(strFrame))
	{
		return FALSE;
	}

	if (!pReader->parse(strFrame))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CKolisNetService::_MakeDM(CJsonReader* pSrcReader, CKolisNetDataMapper* pMapper, CESL_DataMgr* pDstDM, BOOL bResetDM/*=TRUE*/)
{
	if (pSrcReader == NULL || pMapper == NULL || pDstDM == NULL)
	{
		return FALSE;
	}

	INT nMapCount = pMapper->GetSize();
	if (nMapCount == 0)
	{
		// WHAT CAN I DO FOR WORK?
		return FALSE;
	}

	// MAKE DM
	if (bResetDM)
	{
		pDstDM->FreeData();
	}

	INT nCount = pSrcReader->get_totalRow_Value();
	for (INT i=0; i<nCount; i++)
	{
		pDstDM->InsertRow(-1);

		for (INT j=0; j<nMapCount; j++)
		{
			CString strTagName;
			CString strColName;

			pMapper->GetAt(j, strTagName, strColName);

			CString strData;
			pSrcReader->get_fdata_Value(strTagName, i, strData);

			pDstDM->SetCellData(strColName, strData, i);
		}		
	}

	return TRUE;
}

CString CKolisNetService::GetSysdate(BOOL bOnlyDay/*=TRUE*/)
{
	CString strDate;
	CTime t = CTime::GetCurrentTime();
	if (bOnlyDay)
	{
		strDate.Format(_T("%4d/%02d/%02d|00:00:00"), t.GetYear(), t.GetMonth(), t.GetDay());
	}
	else
	{
		strDate.Format(_T("%4d/%02d/%02d|%02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	}

	return strDate;
}

CString CKolisNetService::GetDateString(CString strSrcDate, BOOL bZeroHour/*=TRUE*/)
{
	// TODO: strSrcDate형식이 0000/00/00 인지 확인이 필요하다

	CString strDstDate;
	if (bZeroHour)
	{
		strDstDate.Format(_T("%s|00:00:00"), strSrcDate);
	}
	else
	{
		strDstDate.Format(_T("%s|23:59:59"), strSrcDate);
	}

	return strDstDate;
}

CString CKolisNetService::GetFirstWork(CESL_Mgr* pMgr)
{
	CString strFirstWork;

	if (pMgr && pMgr->m_pInfo)
	{
		strFirstWork.Format(_T("%s-%s"), pMgr->m_pInfo->USER_ID, CKolisNetService::GetSysdate(FALSE));
	}
	else
	{
		strFirstWork = CKolisNetService::GetSysdate(FALSE);
	}

	return strFirstWork;
}

BOOL CKolisNetService::GetLibrariesInfo(CESL_Mgr* pMgr, CESL_DataMgr* pLibrariesDM, CKolisNetDataMapper* pMapper, CString strLibCode/*=_T("")*/, CString strLibName/*=_T("")*/)
{
	if (pLibrariesDM == NULL)
	{
		return FALSE;
	}

	if (pMapper == NULL)
	{
		return FALSE;
	}

	// KOLIS-NET M/W LOGIC
	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETLIBINFO);

	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_LIB_CODE, strLibCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_LIB_NAME, strLibName)

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return CKolisNetService::_MakeDM(&Reader, pMapper, pLibrariesDM);
}

BOOL CKolisNetService::GetLibraryInfo(CESL_Mgr* pMgr, CString strLibCode, CString &strLibName, CString &strKolisNetUseFlag, CString &strDirectUploadFlag)
{
	// Initialize
	strLibName.Empty();
	strKolisNetUseFlag.Empty();
	strDirectUploadFlag.Empty();

	if (strLibCode.IsEmpty())
	{
		return FALSE;
	}

	// KOLIS-NET M/W LOGIC
	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETLIBINFO);
	g_Sender.AddParam(KOLISNET_PARAM_LIB_CODE, strLibCode);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	Reader.get_fdata_Value(KOLISNET_PARAM_LIB_NAME, 0, strLibName);
	Reader.get_fdata_Value(KOLISNET_PARAM_KOLISNET_USE_YN, 0, strKolisNetUseFlag);
	Reader.get_fdata_Value(KOLISNET_PARAM_DIRECT_UPLOAD_YN, 0, strDirectUploadFlag);
	
	return TRUE;
}

// 도서관명 확인
BOOL CKolisNetService::GetLibraryName(CESL_Mgr* pMgr, CString strLibCode, CString &strLibName)
{
	CString strKolisNetUseFlag;
	CString strDirectUploadFlag;

	return CKolisNetService::GetLibraryInfo(pMgr, strLibCode, strLibName, strKolisNetUseFlag, strDirectUploadFlag);
}

// KOLIS-NET 사용유무 FLAG확인
BOOL CKolisNetService::GetLibraryKolisNetUseFlag(CESL_Mgr* pMgr, CString strLibCode, CString &strFlag)
{
	CString strLibName;
	CString strDirectUploadFlag;

	return CKolisNetService::GetLibraryInfo(pMgr, strLibCode, strLibName, strFlag, strDirectUploadFlag);
}

// 운영DB UPLOAD FLAG확인 
BOOL CKolisNetService::GetLibraryDirectUploadFlag(CESL_Mgr* pMgr, CString strLibCode, CString &strFlag)
{
	CString strLibName;
	CString strKolisNetUseFlag;

	return CKolisNetService::GetLibraryInfo(pMgr, strLibCode, strLibName, strKolisNetUseFlag, strFlag);
}

// KOLIS-NET REC_KEY
BOOL CKolisNetService::GetSequenceNo(CESL_Mgr* pMgr, CString &strSeqNo)
{
	// Initialize
	strSeqNo.Empty();

	// KOLIS-NET M/W LOGIC
	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETSEQNO);
	
	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	Reader.get_fdata_Value(KOLISNET_PARAM_SEQ_NO, 0, strSeqNo);

	return TRUE;
}

// 제어번호 가져오기
BOOL CKolisNetService::GetControlNo(CESL_Mgr* pMgr, CString strControlCode, CString strYear, CString &strControlNo)
{
	if (strControlCode.IsEmpty() || strYear.IsEmpty())
	{
		return FALSE;
	}

	// Initialize
	strControlNo.Empty();

	// KOLIS-NET M/W LOGIC
	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETCONTROLNO);

	g_Sender.AddParam(KOLISNET_PARAM_CONTROL_NO_CODE, strControlCode);
	g_Sender.AddParam(KOLISNET_PARAM_CREATE_YEAR, strYear);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	Reader.get_fdata_Value(KOLISNET_PARAM_CONTROL_NO, 0, strControlNo);

	return TRUE;
}

// KOLIS-NET 공지사항 가져오기
BOOL CKolisNetService::GetKolisNetNotify(CESL_Mgr* pMgr, CString strRecvLibCode, CESL_DataMgr* pNotifyDM/*ALIAS=DM_UC_NOTIFY OR DM_UC_NOTIFY_DOWN*/, CKolisNetDataMapper* pMapper)
{
	if (strRecvLibCode.IsEmpty())
	{
		return FALSE;
	}

	if (pNotifyDM == NULL)
	{
		return FALSE;
	}

	if (pMapper == NULL)
	{
		return FALSE;
	}

	// KOLIS-NET M/W LOGIC
	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETUCNOTIFYINFO);
	g_Sender.AddParam(KOLISNET_PARAM_CONFIRM, _T("0"));
	g_Sender.AddParam(KOLISNET_PARAM_RECV_LIB_CODE, strRecvLibCode);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	INT nMapCount = pMapper->GetSize();
	if (nMapCount == 0)
	{
		// WHAT CAN I DO FOR WORK?
		return FALSE;
	}

	// MAKE DM
	INT nCount = Reader.get_totalRow_Value();
	for (INT i=0; i<nCount; i++)
	{
		pNotifyDM->InsertRow(-1);

		for (INT j=0; j<nMapCount; j++)
		{
			CString strTagName;
			CString strColName;

			pMapper->GetAt(j, strTagName, strColName);

			CString strData;
			Reader.get_fdata_Value(strTagName, i, strData);

			// 전송날짜 예외처리
			if (strTagName.Compare(KOLISNET_PARAM_TRANS_DATE) == 0 && strData.GetLength() > 10)
			{
				strData = strData.Left(10);
			}

			pNotifyDM->SetCellData(strColName, strData, i);
		}		
	}

	return TRUE;
}

// KOLIS-NET 공지사항 확인설정
BOOL CKolisNetService::SetKolisNetNotifyConfirm(CESL_Mgr* pMgr, CString strUnKey, CString strRecvLibCode, CString strConfirm)
{
	if (strUnKey.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_SETUCNOTIFYCONFIRM);
	g_Sender.AddParam(KOLISNET_PARAM_UN_KEY, strUnKey);
	g_Sender.AddParam(KOLISNET_PARAM_RECV_LIB_CODE, strRecvLibCode);
	g_Sender.AddParam(KOLISNET_PARAM_CONFIRM, strConfirm);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

// KOLIS-NET 공지사항 첨부파일정보 가져오기
BOOL CKolisNetService::GetKolisNetNotifyFileInfo(CESL_Mgr* pMgr, CString strUnKey, CESL_DataMgr* pNotifyFileDM/*ALIAS=DM_UC_NOTIFY_FILE*/, CKolisNetDataMapper* pMapper)
{
	if (strUnKey.IsEmpty())
	{
		return FALSE;
	}

	if (pNotifyFileDM == NULL)
	{
		return FALSE;
	}

	if (pMapper == NULL)
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETUCNOTIFYFILEINFO);
	g_Sender.AddParam(KOLISNET_PARAM_UN_KEY, strUnKey);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return CKolisNetService::_MakeDM(&Reader, pMapper, pNotifyFileDM);
}

// KOLIS-NET 공지사항 등록
BOOL CKolisNetService::MakeKolisNetNotify(CESL_Mgr* pMgr, CString strUnKey, CString strSendLibCode, CString strSendLibName,
										  CString strRecvLibCode, CString strRecvLibName, CString strTitle,
										  CString strContents, CString strConfirm, CString strSrFlag)
{
	if (strUnKey.IsEmpty() || strSrFlag.IsEmpty() || strSendLibCode.IsEmpty() || strSendLibName.IsEmpty() ||
		strRecvLibCode.IsEmpty() || strRecvLibName.IsEmpty() || strTitle.IsEmpty() || strContents.IsEmpty() ||
		strConfirm.IsEmpty() )
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_MAKEUCNOTIFY);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);
	g_Sender.AddParam(KOLISNET_PARAM_UN_KEY, strUnKey);
	g_Sender.AddParam(KOLISNET_PARAM_SEND_LIB_CODE, strSendLibCode);
	g_Sender.AddParam(KOLISNET_PARAM_SEND_LIB_NAME, strSendLibName);
	g_Sender.AddParam(KOLISNET_PARAM_RECV_LIB_CODE, strRecvLibCode);
	g_Sender.AddParam(KOLISNET_PARAM_RECV_LIB_NAME, strRecvLibName);	
	g_Sender.AddParam(KOLISNET_PARAM_PRIORITY, _T("0"));
	g_Sender.AddParam(KOLISNET_PARAM_TITLE, strTitle);
	g_Sender.AddParam(KOLISNET_PARAM_CONTENTS, strContents);
	g_Sender.AddParam(KOLISNET_PARAM_CONFIRM, strConfirm);
	g_Sender.AddParam(KOLISNET_PARAM_SRFLAG, strSrFlag);
	g_Sender.AddParam(KOLISNET_PARAM_TRANS_DATE, CKolisNetService::GetSysdate());
	g_Sender.AddParam(KOLISNET_PARAM_FIRST_WORK, CKolisNetService::GetFirstWork(pMgr));

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

// KOLIS-NET 공지사항 수신도서관 생성
BOOL CKolisNetService::MakeKolisNetNotifyRecvLib(CESL_Mgr* pMgr, CString strUnKey, CString strUnrlKey, CString strLibCode, CString strLibName)
{
	if (strUnKey.IsEmpty() || strUnrlKey.IsEmpty() || strLibCode.IsEmpty() || strLibName.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_MAKEUCNOTIFYRECVLIB);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);
	g_Sender.AddParam(KOLISNET_PARAM_UN_KEY, strUnKey);
	g_Sender.AddParam(KOLISNET_PARAM_UNRL_KEY, strUnrlKey);
	g_Sender.AddParam(KOLISNET_PARAM_LIB_CODE, strLibCode);
	g_Sender.AddParam(KOLISNET_PARAM_LIB_NAME, strLibName);
	g_Sender.AddParam(KOLISNET_PARAM_FIRST_WORK, CKolisNetService::GetFirstWork(pMgr));
	
	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;	
}

// KOLIS-NET 공지사항 첨부파일 삭제
BOOL CKolisNetService::DeleteKolisNetNotifyRecvLib(CESL_Mgr* pMgr, CString strUnKey, CString strRecvLibCode)
{
	if (strUnKey.IsEmpty() || strRecvLibCode.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_DELETEUCNOTIFYRECVLIB);
	g_Sender.AddParam(KOLISNET_PARAM_UN_KEY, strUnKey);
	g_Sender.AddParam(KOLISNET_PARAM_LIB_CODE, strRecvLibCode);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

// KOLIS-NET 공지사항 첨부파일 내려받기
BOOL CKolisNetService::DownloadNotifyFile(CESL_Mgr* pMgr, CString strTypeCode, CString strBaseKey, CString strFileKey, CString strServerFileName, CString strLocalFileName, CString &strResult)
{
	if (strTypeCode.IsEmpty() || strBaseKey.IsEmpty() || strFileKey.IsEmpty() || strServerFileName.IsEmpty())
	{
		return FALSE;
	}

	// KOLIS-NET M/W LOGIC
	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETFILETOSERVER);
	g_Sender.AddParam(KOLISNET_PARAM_TYPE_CODE, strTypeCode);
	g_Sender.AddParam(KOLISNET_PARAM_BASE_KEY, strBaseKey);
	g_Sender.AddParam(KOLISNET_PARAM_FILE_KEY, strFileKey);
	g_Sender.AddParam(KOLISNET_PARAM_FILE_NAME, strServerFileName);

	CJsonReader Reader;
	if (!CKolisNetService::_RequestKolisNet(pMgr, &g_Sender, &Reader))
	{
		strResult = _T("KOLIS-NET OpenAPI 파일 다운로드에 실패하였습니다.");
		return FALSE;
	}

	CString strRetCode;
	if (Reader.get_resultCode_Value(strRetCode) < 0)
	{
		strResult = _T("KOLIS-NET OpenAPI 파일 다운로드에 실패하였습니다.");
		return FALSE;
	}

	// 0: 성공 , 1: 실패
	if (strRetCode.CompareNoCase(_T("0")) != 0)
	{
		CString strMsg;
		Reader.get_errorMsg_Value(strMsg);
		strResult.Format(_T("KOLIS-NET OpenAPI 파일 다운로드에 실패하였습니다.\r\nerror message : %s"), strMsg);
		return FALSE;
	}

	CString strData;
	Reader.get_fdata_Value(KOLISNET_PARAM_FILE_DATA, 0, strData);
	if (strData.IsEmpty())
	{
		strResult = _T("FILE_DATA IS NULL");
		return FALSE;
	}

	CStrConvert Convert;
	PBYTE pStream = Convert.BASE64_Decode(strData);
	if (pStream == NULL)
	{
		strResult = _T("BASE64 DECODING FAIL");
		return FALSE;
	}

	BOOL bRet = FALSE;

	FILE* fp = _tfopen(strLocalFileName, _T("wb"));
	if (fp != NULL)
	{
		size_t szWrite = fwrite(pStream, sizeof(BYTE), _msize(pStream), fp);
		fclose(fp);

		if (szWrite == _msize(pStream))
		{			
			bRet = TRUE; // 이 경우에만 성공
		}
		else
		{
			strResult = _T("파일 쓰기에 실패하였습니다");
		}
	}
	else
	{
		strResult = _T("파일 생성에 실패하였습니다");
	}

	delete [] pStream;

	return bRet;
}

// KOLIS-NET 공지사항 첨부파일 올리기
BOOL CKolisNetService::UploadNotifyFile(CESL_Mgr* pMgr, CString strTypeCode, CString strBaseKey, CString strServerFileName, CString strLocalFileName, CString &strResult)
{
	// ANALYZE FILE
	FILE* fp = _tfopen(strLocalFileName, _T("rb"));
	if (fp == NULL)
	{
		strResult = _T("파일 열기에 실패하였습니다");
		return FALSE;
	}

	fseek(fp, 0, SEEK_END);
	LONG lSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// READ FILE
	PBYTE pStream = new BYTE[lSize];
	size_t szRead = fread(pStream, sizeof(BYTE), lSize, fp);
	fclose(fp);

	if (szRead != lSize)
	{
		delete [] pStream;

		strResult = _T("파일 읽기에 실패하였습니다");
		return FALSE;
	}

	// BASE64 ENCODING
	CStrConvert Convert;
	CString strStream = Convert.BASE64_Encode(pStream);

	delete [] pStream;

	// ETC PARAM
	CString strSize;
	strSize.Format(_T("%u"), lSize);

	// KOLIS-NET M/W LOGIC
	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_PUTFILETOSERVER);
	g_Sender.AddParam(KOLISNET_PARAM_TYPE_CODE, strTypeCode);
	g_Sender.AddParam(KOLISNET_PARAM_BASE_KEY, strBaseKey);
	g_Sender.AddParam(KOLISNET_PARAM_FILE_NAME, strServerFileName);
	g_Sender.AddParam(KOLISNET_PARAM_FILE_SIZE, strSize);
	g_Sender.AddParam(KOLISNET_PARAM_FILE_DATA, strStream);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);

	CJsonReader Reader;
	if (!CKolisNetService::_RequestKolisNet(pMgr, &g_Sender, &Reader))
	{
		strResult = _T("KOLIS-NET OpenAPI 파일 업로드에 실패하였습니다.");
		return FALSE;
	}

	CString strRetCode;
	if (Reader.get_resultCode_Value(strRetCode) < 0)
	{
		strResult = _T("KOLIS-NET OpenAPI 파일 업로드에 실패하였습니다.");
		return FALSE;
	}

	if (strRetCode.CompareNoCase(_T("0")) != 0)
	{
		CString strMsg;
		Reader.get_errorMsg_Value(strMsg);
		strResult.Format(_T("KOLIS-NET OpenAPI 파일 업로드에 실패하였습니다.\r\nerror message : %s"), strMsg);
		return FALSE;
	}

	return TRUE;
}

BOOL CKolisNetService::GetKlNotify(CESL_Mgr* pMgr, CString strRecvLibCode, CESL_DataMgr* pNotifyDM, CKolisNetDataMapper* pMapper)
{
	if (strRecvLibCode.IsEmpty())
	{
		return FALSE;
	}

	if (pNotifyDM == NULL)
	{
		return FALSE;
	}

	if (pMapper == NULL)
	{
		return FALSE;
	}

	// KOLIS-NET M/W LOGIC
	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETKLNOTIFYINFO);
	g_Sender.AddParam(KOLISNET_PARAM_CONFIRM, _T("0"));
	g_Sender.AddParam(KOLISNET_PARAM_SRFLAG, _T("0"));
	g_Sender.AddParam(KOLISNET_PARAM_RECV_LIB_CODE, strRecvLibCode);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	INT nMapCount = pMapper->GetSize();
	if (nMapCount == 0)
	{
		// WHAT CAN I DO FOR WORK?
		return FALSE;
	}

	// MAKE DM
	INT nCount = Reader.get_totalRow_Value();
	for (INT i=0; i<nCount; i++)
	{
		pNotifyDM->InsertRow(-1);

		for (INT j=0; j<nMapCount; j++)
		{
			CString strTagName;
			CString strColName;

			pMapper->GetAt(j, strTagName, strColName);

			CString strData;
			Reader.get_fdata_Value(strTagName, i, strData);

			// 전송날짜 예외처리
			if (strTagName.Compare(KOLISNET_PARAM_TRANS_DATE) == 0 && strData.GetLength() > 10)
			{
				strData = strData.Left(10);
			}

			pNotifyDM->SetCellData(strColName, strData, i);
		}		
	}

	return TRUE;
}

BOOL CKolisNetService::GetKlNotifyFileInfo(CESL_Mgr* pMgr, CString strNotifyKey, CString &strFileName, CString &strFileSize)
{
	if (strNotifyKey.IsEmpty())
	{
		return FALSE;
	}

	// KOLIS-NET M/W LOGIC
	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETKLNOTIFYFILEINFO);
	g_Sender.AddParam(KOLISNET_PARAM_KN_KEY, strNotifyKey);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	// MAKE DM
	INT nCount = Reader.get_totalRow_Value();
	if (nCount > 0)
	{
		Reader.get_fdata_Value(KOLISNET_PARAM_FILE_NAME, 0, strFileName);
		Reader.get_fdata_Value(KOLISNET_PARAM_FILE_SIZE, 0, strFileSize);
	}

	return TRUE;
}

BOOL CKolisNetService::SetKlNotify(CESL_Mgr* pMgr, CString strNotifyKey, CString strConfirm)
{
	if (strNotifyKey.IsEmpty() && strConfirm.IsEmpty())
	{
		return FALSE;
	}

	// KOLIS-NET M/W LOGIC
	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_SETKLNOTIFY);
	g_Sender.AddParam(KOLISNET_PARAM_KN_KEY, strNotifyKey);
	g_Sender.AddParam(KOLISNET_PARAM_CONFIRM, strConfirm);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

// KOLIS-NET 색인 검색
BOOL CKolisNetService::SearchKolisNet(CESL_Mgr* pMgr, CKolisNetSearchParam* pSearchParam, CESL_DataMgr* pResultDM, CKolisNetDataMapper* pMapper)
{
	if (pSearchParam == NULL)
	{
		return FALSE;
	}

	if (pResultDM == NULL)
	{
		return FALSE;
	}

	if (pMapper == NULL)
	{
		return FALSE;
	}

	// 쓰레드 검색을 위해서 지역변수로 샌더를 생성한다
	CKolisNetSender Sender;	
	Sender.SetService(KOLISNET_SERVICE_SEARCHKOLISNETB);
	// PARAM에서 자동 설정
	pSearchParam->SettingKolisNetSender(&Sender);

	KOLISNETREADER_REQUEST_SERVICE(Reader, Sender, pMgr)

	return CKolisNetService::_MakeDM(&Reader, pMapper, pResultDM);
}

BOOL CKolisNetService::SearchKolisNetForKey(CESL_Mgr* pMgr, CString strControlNo, CString &strKey)
{
	if (strControlNo.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_SEARCHKOLISNETB);
	g_Sender.AddParam(KOLISNET_PARAM_START_CONTROL_NO, strControlNo);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	INT nCount = Reader.get_totalRow_Value();
	if (nCount > 0)
	{
		Reader.get_fdata_Value(KOLISNET_PARAM_ID, 0, strKey);
	}

	return TRUE;
}


// KOLIS-NET 색인 검색 (검색결과 REC_KEY수집용)
BOOL CKolisNetService::SearchKolisNetForKeys(CESL_Mgr* pMgr, CKolisNetSearchParam* pSearchParam, CString &strKeyList)
{
	if (pSearchParam == NULL)
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_SEARCHKOLISNETB);
	// PARAM에서 자동 설정
	pSearchParam->SettingKolisNetSender(&g_Sender);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	strKeyList.Empty();

	INT nCount = Reader.get_totalRow_Value();
	for (INT i=0; i<nCount; i++)
	{
		CString strData;
		Reader.get_fdata_Value(KOLISNET_PARAM_ID, i, strData);
		
		if (!strKeyList.IsEmpty())
		{
			strKeyList += _T("|");
		}

		strKeyList += strData;
	}

	return TRUE;
}

// KOLIS-NET 서지정보 검색
BOOL CKolisNetService::GetBibInfo(CESL_Mgr* pMgr, CString strRecKey, CESL_DataMgr* pBibDM, CKolisNetDataMapper* pMapper, CString strHoldLibCode/*=_T("")*/)
{
	if (strRecKey.IsEmpty())
	{
		return FALSE;
	}

	if (pBibDM == NULL)
	{
		return FALSE;
	}

	if (pMapper == NULL)
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETBIBINFO);

	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_BIB_KEY, strRecKey)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_HOLD_LIB_CODE, strHoldLibCode)

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return CKolisNetService::_MakeDM(&Reader, pMapper, pBibDM);
}

BOOL CKolisNetService::GetBibVolInfo(CESL_Mgr* pMgr, CString strBibKey, CESL_DataMgr* pBibVolDM, CKolisNetDataMapper* pMapper)
{
	if (strBibKey.IsEmpty())
	{
		return FALSE;
	}

	if (pBibVolDM == NULL)
	{
		return FALSE;
	}

	if (pMapper == NULL)
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETBIBVOLINFO);
	g_Sender.AddParam(KOLISNET_PARAM_BIB_KEY, strBibKey);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return CKolisNetService::_MakeDM(&Reader, pMapper, pBibVolDM);
}

// KOLIS-NET 소장정보 검색
BOOL CKolisNetService::GetHoldInfo(CESL_Mgr* pMgr, CString strRecKey, CString strHoldControlNo, CString strLocalControlNo, CString strCreateLibCode, CESL_DataMgr* pHoldDM, CKolisNetDataMapper* pMapper)
{
	if (strRecKey.IsEmpty() && strHoldControlNo.IsEmpty() && strLocalControlNo.IsEmpty())
	{
		return FALSE;
	}

	if (pHoldDM == NULL)
	{
		return FALSE;
	}

	if (pMapper == NULL)
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETHOLDINFO);

	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_BIB_KEY, strRecKey)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_HOLD_CONTROL_NO, strHoldControlNo)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_LOCAL_CONTROL_NO, strLocalControlNo)
	// 공지사항등에서는 CREATE_LIB 사용, 배가자료관리 코드 변경시 CREATE_LIB_CODE
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_CREATE_LIB, strCreateLibCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_CREATE_LIB_CODE, strCreateLibCode)

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return CKolisNetService::_MakeDM(&Reader, pMapper, pHoldDM);
}

BOOL CKolisNetService::GetHoldInfoBibKey(CESL_Mgr* pMgr, CString strRecKey, CString strHoldControlNo, CString strLocalControlNo, CString strCreateLibCode, CString &strBibKey)
{
	if (strRecKey.IsEmpty() && strHoldControlNo.IsEmpty() && strLocalControlNo.IsEmpty())
	{
		return FALSE;
	}

	if (!strLocalControlNo.IsEmpty() && strCreateLibCode.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETHOLDINFO);

	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_BIB_KEY, strRecKey)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_HOLD_CONTROL_NO, strHoldControlNo)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_LOCAL_CONTROL_NO, strLocalControlNo)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_CREATE_LIB_CODE, strCreateLibCode)

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	INT nCount = Reader.get_totalRow_Value();
	if (nCount != 0)
	{
		Reader.get_fdata_Value(KOLISNET_PARAM_BIB_KEY, 0, strBibKey);
	}

	return TRUE;
}

// 출판물류유통진흥원 종정보 검색
BOOL CKolisNetService::GetBtpSpeciesInfo(CESL_Mgr* pMgr, CString strRecKey, CESL_DataMgr* pBtpDM, CKolisNetDataMapper* pMapper)
{
	if (strRecKey.IsEmpty())
	{
		return FALSE;
	}

	if (pBtpDM == NULL)
	{
		return FALSE;
	}

	if (pMapper == NULL)
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETBTPSPECIESINFO);
	g_Sender.AddParam(KOLISNET_PARAM_BS_KEY, strRecKey);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return CKolisNetService::_MakeDM(&Reader, pMapper, pBtpDM);

}

// 출판물류유통진흥원 POS데이터 검색
BOOL CKolisNetService::GetBtpSpeciesPosInfo(CESL_Mgr* pMgr, CString strISBN, CESL_DataMgr* pBtpPosDM, CKolisNetDataMapper* pMapper)
{
	if (strISBN.IsEmpty())
	{
		return FALSE;
	}

	if (pBtpPosDM == NULL)
	{
		return FALSE;
	}

	if (pMapper == NULL)
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETBTPSPECIESPOSINFO);
	g_Sender.AddParam(KOLISNET_PARAM_ISBN, strISBN);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return CKolisNetService::_MakeDM(&Reader, pMapper, pBtpPosDM);
}

BOOL CKolisNetService::GetLibCodeAdmnInfo(CESL_Mgr* pMgr, CString strClassCode, CString &strRecKey)
{
	if (strClassCode.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETLIBCODEADMNINFO);
	g_Sender.AddParam(KOLISNET_PARAM_CLASS_CODE, strClassCode);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	INT nCount = Reader.get_totalRow_Value();
	if (nCount == 0)
	{
		return FALSE;
	}

	Reader.get_fdata_Value(KOLISNET_PARAM_LCA_KEY, 0, strRecKey);

	return TRUE;
}

BOOL CKolisNetService::GetLibDropCodeInfo(CESL_Mgr* pMgr, CString strLibCode, CESL_DataMgr* pDropCodeDM, CKolisNetDataMapper* pMapper)
{
	if (strLibCode.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETLIBDROPCODEINFO);
	g_Sender.AddParam(KOLISNET_PARAM_LIB_CODE, strLibCode);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return CKolisNetService::_MakeDM(&Reader, pMapper, pDropCodeDM);
}

BOOL CKolisNetService::GetTConAbsInfo(CESL_Mgr* pMgr, CString strControlNo, CESL_DataMgr* pTConAbsDM, CKolisNetDataMapper* pMapper)
{
	if (strControlNo.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETTCONABSINFO);
	g_Sender.AddParam(KOLISNET_PARAM_UB_CONTROL_NO, strControlNo);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return CKolisNetService::_MakeDM(&Reader, pMapper, pTConAbsDM);
}

BOOL CKolisNetService::GetCountBib(CESL_Mgr* pMgr, CString strRecKey, INT &nCount)
{
	if (strRecKey.IsEmpty())
	{
		return FALSE;
	}

	nCount = 0;

	// KOLIS-NET M/W LOGIC
	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETCOUNTBIB);
	g_Sender.AddParam(KOLISNET_PARAM_BIB_KEY, strRecKey);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	CString strRetValue;
	if (Reader.get_resultValue_Value(strRetValue) < 0)
	{
		return FALSE;
	}

	nCount = _ttoi(strRetValue);

	return TRUE;
}

BOOL CKolisNetService::GetCountHold(CESL_Mgr* pMgr, CString strRecKey, CString strLocalControlNo, CString strCreateLibCode, INT &nCount)
{
	if (strRecKey.IsEmpty() && strLocalControlNo.IsEmpty())
	{
		return FALSE;
	}

	if (!strLocalControlNo.IsEmpty() && strCreateLibCode.IsEmpty())
	{
		return FALSE;
	}

	nCount = 0;

	// KOLIS-NET M/W LOGIC
	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETCOUNTHOLD);
	g_Sender.AddParam(KOLISNET_PARAM_HOLD_KEY, strRecKey);
	g_Sender.AddParam(KOLISNET_PARAM_LOCAL_CONTROL_NO, strLocalControlNo);
	g_Sender.AddParam(KOLISNET_PARAM_CREATE_LIB_CODE, strCreateLibCode);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	CString strRetValue;
	if (Reader.get_resultValue_Value(strRetValue) < 0)
	{
		return FALSE;
	}

	nCount = _ttoi(strRetValue);

	return TRUE;
}

BOOL CKolisNetService::GetBoEditRequestInfo(CESL_Mgr* pMgr, CString strBibKey, CString strStartRequestDate, CString strEndRequestDate, CString strStartLibCode, CString strEndLibCode, CString strProcessStatus, CESL_DataMgr* pEditReqDM, CKolisNetDataMapper* pMapper)
{
	if (strBibKey.IsEmpty() && strStartRequestDate.IsEmpty() && strEndRequestDate.IsEmpty() && strStartLibCode.IsEmpty() && strEndLibCode.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETBOEDITREQUESTINFO);

	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_BIB_KEY, strBibKey)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_START_REQUEST_DATE, strStartRequestDate)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_END_REQUEST_DATE, strEndRequestDate)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_START_LIB_CODE, strStartLibCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_END_LIB_CODE, strEndLibCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_PROCESS_STATUS, strProcessStatus)

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return CKolisNetService::_MakeDM(&Reader, pMapper, pEditReqDM);
}

BOOL CKolisNetService::GetBoMixRequestInfo(CESL_Mgr* pMgr, CString strBibKey, CString strStartRequestDate, CString strEndRequestDate, CString strStartLibCode, CString strEndLibCode, CString strProcessStatus, CESL_DataMgr* pMixReqDM, CKolisNetDataMapper* pMapper)
{
	if (strBibKey.IsEmpty() && strStartRequestDate.IsEmpty() && strEndRequestDate.IsEmpty() && strStartLibCode.IsEmpty() && strEndLibCode.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETBOMIXREQUESTINFO);

	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_BIB_KEY, strBibKey)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_START_REQUEST_DATE, strStartRequestDate)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_END_REQUEST_DATE, strEndRequestDate)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_START_LIB_CODE, strStartLibCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_END_LIB_CODE, strEndLibCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_PROCESS_STATUS, strProcessStatus)

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return CKolisNetService::_MakeDM(&Reader, pMapper, pMixReqDM);
}

BOOL CKolisNetService::SetBibInfo(CESL_Mgr* pMgr, CString strRecKey, CString strUseLimitCode, CString strMarc/*=_T("")*/, CString strControlNo/*=_T("")*/, CString strUpdateDate/*=_T("")*/, CString strMatCode/*=_T("")*/, CString strFormCode/*=_T("")*/, CString strMediaCode/*=_T("")*/, CString strLangCode/*=_T("")*/, CString strNote/*=_T("")*/, CString strKoUniversityCode/*=_T("")*/, CString strGovernInstrumentCode/*=_T("")*/, CString strYearCode/*=_T("")*/, CString strPublishCountryCode/*=_T("")*/, CString strUndoFlag/*=_T("")*/)
{
	if (strRecKey.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_SETBIB);
	g_Sender.AddParam(KOLISNET_PARAM_BIB_KEY, strRecKey);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);

	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_USE_LIMIT_CODE, strUseLimitCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_BIB_MARC, strMarc)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_CONTROL_NO, strControlNo)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_UPDATE_DATE, strUpdateDate)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_MAT_CODE, strMatCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_FORM_CODE, strFormCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_MEDIA_CODE, strMediaCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_LANG_CODE, strLangCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_NOTE, strNote)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_KO_UNIVERSITY_CODE, strKoUniversityCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_GOVERN_INSTRUMENT_CODE, strGovernInstrumentCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_YEAR_CODE, strYearCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_PUBLISH_COUNTRY_CODE, strPublishCountryCode)
	KOLISNETPARAM_ADD_STR(KOLISNET_PARAM_UNDO_FLAG, strUndoFlag)

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

BOOL CKolisNetService::SetHoldInfo(CESL_Mgr* pMgr, CString strRecKey, CString strUseLimitCode, CString strMarc/*=_T("")*/, CString strControlNo/*=_T("")*/, CString strUpdateDate/*=_T("")*/, CString strBibUpdated/*=_T("")*/, CString strLocalControlNo/*=_T("")*/)
{
	if (strRecKey.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_SETHOLD);
	g_Sender.AddParam(KOLISNET_PARAM_HOLD_KEY, strRecKey);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);
	if (!strUseLimitCode.IsEmpty())
	{
		g_Sender.AddParam(KOLISNET_PARAM_USE_LIMIT_CODE, strUseLimitCode);
	}
	if (!strMarc.IsEmpty())
	{
		g_Sender.AddParam(KOLISNET_PARAM_HOLD_MARC, strMarc);
	}
	if (!strControlNo.IsEmpty())
	{
		g_Sender.AddParam(KOLISNET_PARAM_CONTROL_NO, strControlNo);
	}
	if (!strUpdateDate.IsEmpty())
	{
		g_Sender.AddParam(KOLISNET_PARAM_UPDATE_DATE, strUpdateDate);
	}
	if (!strBibUpdated.IsEmpty())
	{
		g_Sender.AddParam(KOLISNET_PARAM_BIB_UPDATED, strBibUpdated);
	}
	if (!strLocalControlNo.IsEmpty())
	{
		g_Sender.AddParam(KOLISNET_PARAM_LOCAL_CONTROL_NO, strLocalControlNo);
	}

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

// 도서관 정보 수정
BOOL CKolisNetService::SetLibInfo(CESL_Mgr* pMgr, CString strRecKey, CKolisNetLibInfoParam* pLibInfoParam)
{
	if (strRecKey.IsEmpty())
	{
		return FALSE;
	}

	if (pLibInfoParam == NULL)
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_SETLIBINFO);
	g_Sender.AddParam(KOLISNET_PARAM_LIB_KEY, strRecKey);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);
	// PARAM에서 자동 설정
	pLibInfoParam->SettingKolisNetSender(&g_Sender);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)
	
	return TRUE;
}

// KOLIS-NET 업로드 통계
BOOL CKolisNetService::GetUploadStatitics(CESL_Mgr* pMgr, CString strStartDate, CString strEndDate, CString strLibCode, CESL_DataMgr* pStatiticsDM, CKolisNetDataMapper* pMapper)
{
	if (strStartDate.IsEmpty() || strEndDate.IsEmpty())
	{
		return FALSE;
	}

	if (pStatiticsDM == NULL)
	{
		return FALSE;
	}

	if (pMapper == NULL)
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_GETUPLOADSTAT);
	g_Sender.AddParam(KOLISNET_PARAM_START_WORK_DATE, strStartDate);
	g_Sender.AddParam(KOLISNET_PARAM_END_WORK_DATE, strEndDate);
	if (!strLibCode.IsEmpty())
	{
		g_Sender.AddParam(KOLISNET_PARAM_LIB_CODE, strLibCode);
	}

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return CKolisNetService::_MakeDM(&Reader, pMapper, pStatiticsDM);
}

// KOLIS-NET UP/DOWN 통계 생성 
BOOL CKolisNetService::MakeUploadStatitics(CESL_Mgr* pMgr, CString strDataKey, CString strLibCode, CString strClientType, CString strScreenType, CString strUpdownFlag, CString strWorkFlag, CString strWorkDate, CString strSpeciesCount)
{
	if (strDataKey.IsEmpty()    || strLibCode.IsEmpty()    || strClientType.IsEmpty() ||
		strUpdownFlag.IsEmpty() || strWorkDate.IsEmpty()   || strSpeciesCount.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_MAKESTATUPLOAD);
	g_Sender.AddParam(KOLISNET_PARAM_DATA_KEY, strDataKey);
	g_Sender.AddParam(KOLISNET_PARAM_LIB_CODE, strLibCode);
	g_Sender.AddParam(KOLISNET_PARAM_CLIENT_TYPE, strClientType);
	g_Sender.AddParam(KOLISNET_PARAM_SCREEN_TYPE, strScreenType);
	g_Sender.AddParam(KOLISNET_PARAM_UPDOWN_FLAG, strUpdownFlag);
	g_Sender.AddParam(KOLISNET_PARAM_WORK_FLAG, strWorkFlag);
	g_Sender.AddParam(KOLISNET_PARAM_WORK_DATE, strWorkDate);
	g_Sender.AddParam(KOLISNET_PARAM_SPECIES_COUNT, strSpeciesCount);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

// 출판유통진흥원 UP/DOWN 통계 생성 
BOOL CKolisNetService::MakeBtpStatitics(CESL_Mgr* pMgr, CString strDataKey, CString strLibCode, CString strClientType, CString strScreenType, CString strUpdownFlag, CString strWorkFlag, CString strWorkDate, CString strSpeciesCount)
{
	if (strDataKey.IsEmpty()    || strLibCode.IsEmpty()    || strClientType.IsEmpty() ||
		strUpdownFlag.IsEmpty() || 
		strWorkDate.IsEmpty()   || strSpeciesCount.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_MAKESTATBTP);
	g_Sender.AddParam(KOLISNET_PARAM_DATA_KEY, strDataKey);
	g_Sender.AddParam(KOLISNET_PARAM_LIB_CODE, strLibCode);
	g_Sender.AddParam(KOLISNET_PARAM_CLIENT_TYPE, strClientType);
	g_Sender.AddParam(KOLISNET_PARAM_SCREEN_TYPE, strScreenType);
	g_Sender.AddParam(KOLISNET_PARAM_UPDOWN_FLAG, strUpdownFlag);
	g_Sender.AddParam(KOLISNET_PARAM_WORK_FLAG, strWorkFlag);
	g_Sender.AddParam(KOLISNET_PARAM_WORK_DATE, strWorkDate);
	g_Sender.AddParam(KOLISNET_PARAM_SPECIES_COUNT, strSpeciesCount);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

BOOL CKolisNetService::MakeBoEditReq(CESL_Mgr* pMgr, CString strRecKey, CString strLibKey, CString strBibKey, CString strMarc, CString strNote)
{
	if (strRecKey.IsEmpty() || strLibKey.IsEmpty() || strBibKey.IsEmpty() || strMarc.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_MAKEBOEDITREQ);
	g_Sender.AddParam(KOLISNET_PARAM_UBMER_KEY, strRecKey);
	g_Sender.AddParam(KOLISNET_PARAM_LIB_KEY, strLibKey);
	g_Sender.AddParam(KOLISNET_PARAM_BIB_KEY, strBibKey);
	g_Sender.AddParam(KOLISNET_PARAM_MARC, strMarc);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);
	g_Sender.AddParam(KOLISNET_PARAM_REQUEST_DATE, CKolisNetService::GetSysdate(FALSE));
	g_Sender.AddParam(KOLISNET_PARAM_FIRST_WORK, CKolisNetService::GetFirstWork(pMgr));
	if (!strNote.IsEmpty())
	{
		g_Sender.AddParam(KOLISNET_PARAM_NOTE, strNote);
	}

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

BOOL CKolisNetService::MakeBoMixReq(CESL_Mgr* pMgr, CString strRecKey, CString strLibKey, CString strBasicBibKey, CString strSelectedBibKey)
{
	if (strRecKey.IsEmpty() || strLibKey.IsEmpty() || strBasicBibKey.IsEmpty() || strSelectedBibKey.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_MAKEBOMIXREQ);
	g_Sender.AddParam(KOLISNET_PARAM_UBMMR_KEY, strRecKey);
	g_Sender.AddParam(KOLISNET_PARAM_LIB_KEY, strLibKey);
	g_Sender.AddParam(KOLISNET_PARAM_BASIC_BIB_KEY, strBasicBibKey);
	g_Sender.AddParam(KOLISNET_PARAM_SELECTED_BIB_KEY_LIST, strSelectedBibKey);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);
	g_Sender.AddParam(KOLISNET_PARAM_REQUEST_DATE, CKolisNetService::GetSysdate(FALSE));
	g_Sender.AddParam(KOLISNET_PARAM_FIRST_WORK, CKolisNetService::GetFirstWork(pMgr));	

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

BOOL CKolisNetService::MakeHoldHisInfo(CESL_Mgr* pMgr, CString strRecKey, CString strPrevVolCode, CString strChangeVolCode, CString strWorker)
{
	if (strRecKey.IsEmpty() || strPrevVolCode.IsEmpty() || strChangeVolCode.IsEmpty() || strWorker.IsEmpty())
	{
		return FALSE;
	}

	// 시퀀스 생성
	CString strSeqNo;
	CKolisNetService::GetSequenceNo(pMgr, strSeqNo);
	
	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_MAKEHOLDINGHIS);
	g_Sender.AddParam(KOLISNET_PARAM_UBHMH_KEY, strSeqNo);
	g_Sender.AddParam(KOLISNET_PARAM_HOLD_KEY, strRecKey);
	g_Sender.AddParam(KOLISNET_PARAM_CHANGE_DATE, CKolisNetService::GetSysdate(FALSE));
	g_Sender.AddParam(KOLISNET_PARAM_PREV_VOL_CODE, strPrevVolCode);
	g_Sender.AddParam(KOLISNET_PARAM_CHANGE_VOL_CODE, strChangeVolCode);
	g_Sender.AddParam(KOLISNET_PARAM_WORKER, strWorker);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

BOOL CKolisNetService::MakeTempMasterUpload(CESL_Mgr* pMgr, CString strRecKey, CString strLibCode, CString strMarc, CString strLocalControlNo, CString strMarcType,CString strRegNoList, CString strUseLimitCodeList, CString strSpeciesLimitCode)
{   
	if (strRecKey.IsEmpty() || strLibCode.IsEmpty() || strMarc.IsEmpty() || strLocalControlNo.IsEmpty() || strMarcType.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_MAKETEMPMASTER);
	g_Sender.AddParam(KOLISNET_PARAM_UTM_KEY, strRecKey);
	g_Sender.AddParam(KOLISNET_PARAM_CREATE_LIB, strLibCode);
	g_Sender.AddParam(KOLISNET_PARAM_MARC, strMarc);
	g_Sender.AddParam(KOLISNET_PARAM_LOCAL_CONTROL_NO, strLocalControlNo);
	g_Sender.AddParam(KOLISNET_PARAM_CREATE_DATE, CKolisNetService::GetSysdate(FALSE));
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);
	g_Sender.AddParam(KOLISNET_PARAM_OLD_YN, _T("N"));
	g_Sender.AddParam(KOLISNET_PARAM_MARC_TYPE, strMarcType);
	g_Sender.AddParam(KOLISNET_PARAM_MATERIAL_TYPE, _T("G"));
	g_Sender.AddParam(KOLISNET_PARAM_CLIENT_TYPE, _T("K3"));
	g_Sender.AddParam(KOLISNET_PARAM_CO_FLAG, _T("N"));
	g_Sender.AddParam(KOLISNET_PARAM_SPECIES_LIMIT_CODE, strSpeciesLimitCode );
	g_Sender.AddParam(KOLISNET_PARAM_REG_NO_LIST, strRegNoList );
	g_Sender.AddParam(KOLISNET_PARAM_USE_LIMIT_CODE_LIST, strUseLimitCodeList );	
	

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

// 임시DB 업로드 통계 생성
BOOL CKolisNetService::MakeStatiticsTempUpload(CESL_Mgr* pMgr, CString strRecKey, CString strLibCode, CString strUpDownFlag, CString strWorkDate, CString strSpeciesCount)
{
	if (strRecKey.IsEmpty() || strLibCode.IsEmpty() || strUpDownFlag.IsEmpty() || strWorkDate.IsEmpty() || strSpeciesCount.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_MAKESTATTEMPLIB);
	g_Sender.AddParam(KOLISNET_PARAM_UTLSTAT_KEY, strRecKey);
	g_Sender.AddParam(KOLISNET_PARAM_LIB_CODE, strLibCode);
	g_Sender.AddParam(KOLISNET_PARAM_UPDOWN_FLAG, strUpDownFlag);
	g_Sender.AddParam(KOLISNET_PARAM_WORK_DATE, strWorkDate);
	g_Sender.AddParam(KOLISNET_PARAM_SPECIES_COUNT, strSpeciesCount);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

// 출판유통 통계 생성
BOOL CKolisNetService::MakeStatiticsBtp(CESL_Mgr* pMgr, CString strRecKey, CString strLibCode, CString strClientType, CString strUpDownFlag, CString strWorkFlag, CString strWorkDate, CString strSpeciesCount)
{
	if (strRecKey.IsEmpty() || strUpDownFlag.IsEmpty() || strLibCode.IsEmpty() || strClientType.IsEmpty() ||
		strWorkFlag.IsEmpty() || strWorkDate.IsEmpty() || strSpeciesCount.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_MAKESTATBTP);
	g_Sender.AddParam(KOLISNET_PARAM_DATA_KEY, strRecKey);
	g_Sender.AddParam(KOLISNET_PARAM_LIB_CODE, strLibCode);
	g_Sender.AddParam(KOLISNET_PARAM_CLIENT_TYPE, strClientType);
	g_Sender.AddParam(KOLISNET_PARAM_UPDOWN_FLAG, strUpDownFlag);
	g_Sender.AddParam(KOLISNET_PARAM_WORK_DATE, strWorkDate);
	g_Sender.AddParam(KOLISNET_PARAM_WORK_FLAG, strWorkFlag);
	g_Sender.AddParam(KOLISNET_PARAM_SPECIES_COUNT, strSpeciesCount);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

BOOL CKolisNetService::DeleteHold(CESL_Mgr* pMgr, CString strHoldKey)
{
	if (strHoldKey.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_DELETEHOLD);
	g_Sender.AddParam(KOLISNET_PARAM_HOLD_KEY, strHoldKey);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

BOOL CKolisNetService::DeleteMarcMixUndoBib(CESL_Mgr* pMgr, CString strBibKey)
{
	if (strBibKey.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_DELETEBOMIXUNDOBIB);
	g_Sender.AddParam(KOLISNET_PARAM_NEW_BIB_KEY, strBibKey);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

BOOL CKolisNetService::DeleteMarcMixUndoHold(CESL_Mgr* pMgr, CString strBibKey)
{
	if (strBibKey.IsEmpty())
	{
		return FALSE;
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_DELETEBOMIXUNDOHOLD);
	g_Sender.AddParam(KOLISNET_PARAM_NEW_BIB_KEY, strBibKey);
	g_Sender.AddParam(KOLISNET_PARAM_COMMIT_YN, KOLISNET_COMMIT_FLAG);

	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)

	return TRUE;
}

BOOL CKolisNetService::setLocalBookRegNoInfo(CESL_Mgr* pMgr, CString strLibcode , CString strControlNo, CString strRegNo, CString strCopyNo, CString strUseLimitCode, CString strSeriveYN , CString strWorkType, CString strShelfCode, CString strVolCode){

	if ( strLibcode.IsEmpty() || strControlNo.IsEmpty() || strRegNo.IsEmpty() || strWorkType.IsEmpty())
	{
		return FALSE;
	}
	if ( strWorkType == _T("U") || strWorkType == _T("I")){

		if(strUseLimitCode.IsEmpty() || strSeriveYN.IsEmpty()){ 
			return FALSE;
		}
	}

	g_Sender.InitParam();
	g_Sender.SetService(KOLISNET_SERVICE_SETLOCALBOOKREGNOINFO);
	g_Sender.AddParam(KOLISNET_PARAM_LIB_CODE, strLibcode);
	g_Sender.AddParam(KOLISNET_PARAM_CONTROL_NO , strControlNo);
	g_Sender.AddParam(KOLISNET_PARAM_REG_NO , strRegNo);
	g_Sender.AddParam(KOLISNET_PARAM_COPY_NO , strCopyNo);
	g_Sender.AddParam(KOLISNET_PARAM_USE_LIMIT_CODE , strUseLimitCode);
	g_Sender.AddParam(KOLISNET_PARAM_SERVICE_YN , strSeriveYN);
	g_Sender.AddParam(KOLISNET_PARAM_WORK_TYPE , strWorkType);
	g_Sender.AddParam(KOLISNET_PARAM_SHELF_CODE , strShelfCode);
	g_Sender.AddParam(KOLISNET_PARAM_VOL_CODE , strVolCode);
	
	KOLISNETREADER_REQUEST_SERVICE(Reader, g_Sender, pMgr)


	return TRUE;
}
