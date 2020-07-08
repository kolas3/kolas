#include "Stdafx.h"
#include "ESL_SearchEngine.h"

#include <afxinet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct _SEARCH_ENGINE_SETTING_
{
	BOOL	USED;	
	CString URL;
	INT		TIMEOUT;	// SECONDS
	INT		SEARCHMODE;	// 0:CTX / 1:형태소
	INT		WRITETYPE;	// 0:JSON / 1:XML / 2:PYTHON / 3:RUBY / 4:PHP / 5:CSV (데이터 통신 용량관련으로 JSON으로만 처리)
	BOOL	INDENT;		// 
	INT		START;
	INT		ROWS;
	CString SORT_KEY;
	BOOL	SORT_DESC;

} SEARCH_ENGINE_SETTING;

typedef struct _SEARCH_ENGINE_STATUS_
{
	BOOL IDX_BO_TBL_STATUS;
	BOOL IDX_BO_TOC_TBL_STATUS;
	BOOL IDX_SE_TBL_STATUS;
	BOOL IDX_SE_TOC_TBL_STATUS;

} SEARCH_ENGINE_STATUS;

SEARCH_ENGINE_SETTING g_setting;
SEARCH_ENGINE_STATUS  g_status;

#define SEARCH_ENGINE_CORE_IDX_BO_TBL		_T("IDX_BO_TBL")
#define SEARCH_ENGINE_CORE_IDX_BO_TOC_TBL	_T("IDX_BO_TOC_TBL")
#define SEARCH_ENGINE_CORE_IDX_SE_TBL		_T("IDX_SE_TBL")
#define SEARCH_ENGINE_CORE_IDX_SE_TOC_TBL	_T("IDX_SE_TOC_TBL")

#define SEARCH_ENGINE_SERVICE_PING			_T("admin/ping")
#define SEARCH_ENGINE_SERVICE_SELECT		_T("select")
#define SEARCH_ENGINE_SERVICE_UPDATE		_T("update")
#define SEARCH_ENGINE_SERVICE_DATAIMPORT	_T("dataimport")

//{{ CONVERT CHAR SRC->DST
#define SEARCH_ENGINE_CHAR_W2UTF8(SRC, DST)	\
	{ \
		DST = NULL; \
		INT CP_SIZE = ::WideCharToMultiByte(CP_UTF8, 0, SRC, -1, NULL, NULL, NULL, NULL); \
		if (CP_SIZE > 0) { \
			DST = new CHAR[CP_SIZE+1]; \
			CP_SIZE = ::WideCharToMultiByte(CP_UTF8, 0, SRC, -1, DST, CP_SIZE, NULL, NULL); \
		} \
	}
#define SEARCH_ENGINE_CHAR_UTF8TOW(SRC, DST)	\
	{ \
		DST = NULL; \
		INT CP_SIZE = ::MultiByteToWideChar(CP_UTF8, NULL, SRC, -1, NULL, NULL); \
		if (CP_SIZE > 0) { \
			DST = new WCHAR[CP_SIZE+1]; \
			CP_SIZE = ::MultiByteToWideChar(CP_UTF8, NULL, SRC, -1, DST, CP_SIZE); \
		} \
	}
#define SEARCH_ENGINE_CHAR_W2ACP(SRC, DST)	\
	{ \
		DST = NULL; \
		INT CP_SIZE = ::WideCharToMultiByte(CP_ACP, 0, SRC, -1, NULL, NULL, NULL, NULL); \
		if (CP_SIZE > 0) { \
			DST = new CHAR[CP_SIZE+1]; \
			CP_SIZE = ::WideCharToMultiByte(CP_ACP, 0, SRC, -1, DST, CP_SIZE, NULL, NULL); \
		} \
	}
//}}

CESL_SearchEngine::CESL_SearchEngine()
{

}

CESL_SearchEngine::~CESL_SearchEngine()
{

}

/////////////////////////////////////////////////////////////////////////
// SETTING FUNCTION
VOID CESL_SearchEngine::SetUsed(BOOL bUsed)
{
	g_setting.USED = bUsed;
}

BOOL CESL_SearchEngine::GetUsed()
{
	return g_setting.USED;
}

VOID CESL_SearchEngine::SetURL(CString strURL)
{
	if (!strURL.IsEmpty()) {
		while (0 == strURL.Right(1).Compare(_T("/"))) 
		{
			strURL = strURL.Mid(0, strURL.GetLength()-1);
		}

		g_setting.URL = strURL;
	}
}

CString CESL_SearchEngine::GetURL()
{
	return g_setting.URL;
}

VOID CESL_SearchEngine::SetTimeOut(INT nTimeOut)
{
	if (nTimeOut > 0)
		g_setting.TIMEOUT = nTimeOut;
	else
		g_setting.TIMEOUT = 5; // 기본값 5초 설정
}

INT CESL_SearchEngine::GetTimeOut()
{
	if (g_setting.TIMEOUT < 0)
		return 5; // 기본값 5초 설정

	return g_setting.TIMEOUT;
}

VOID CESL_SearchEngine::SetSearchMode(INT nSearchMode)
{
	g_setting.SEARCHMODE = nSearchMode;
}

INT CESL_SearchEngine::GetSearchMode()
{
	return g_setting.SEARCHMODE;
}

VOID CESL_SearchEngine::SetWriteType(INT nWriteType)
{
	g_setting.WRITETYPE = nWriteType;
}

INT CESL_SearchEngine::GetWriteType()
{
	// 통신용량 관계로 JSON으로만 처리한다
	//return g_setting.WRITETYPE;
	return 0;
}

VOID CESL_SearchEngine::SetIndent(BOOL bIndent)
{
	g_setting.INDENT = bIndent;
}

BOOL CESL_SearchEngine::GetIndent()
{
	// 통신용량 관계로 OFF로 처리한다
	//return g_setting.INDENT;
	return FALSE; 
}

VOID CESL_SearchEngine::SetStart(INT nIndex)
{
	if (nIndex > 0)
		g_setting.START = nIndex;
	else
		g_setting.START = 0;
}

INT CESL_SearchEngine::GetStart()
{
	if (g_setting.START > 0)
		return g_setting.START;

	return 0;
}

VOID CESL_SearchEngine::SetRows(INT nRows)
{
	if (nRows > 0)
		g_setting.ROWS = nRows;
	else
		g_setting.ROWS = 20000; // IN QUERY LIMIT
}

INT CESL_SearchEngine::GetRows()
{
	if (g_setting.ROWS > 0)
		return g_setting.ROWS;

	return 20000;
}

VOID CESL_SearchEngine::SetSort(CString strSortKey, BOOL bDESC/*=FALSE*/)
{
	g_setting.SORT_KEY = strSortKey;
	g_setting.SORT_DESC = bDESC;
}

VOID CESL_SearchEngine::GetSort(CString &strSortKey, BOOL &bDESC)
{
	strSortKey = g_setting.SORT_KEY;
	bDESC =g_setting.SORT_DESC;
}

VOID CESL_SearchEngine::SetStatus(CString strCore, BOOL bStatus)
{
	if		(strCore.CompareNoCase(SEARCH_ENGINE_CORE_IDX_BO_TBL) == 0)		g_status.IDX_BO_TBL_STATUS = bStatus;
	else if (strCore.CompareNoCase(SEARCH_ENGINE_CORE_IDX_BO_TOC_TBL) == 0) g_status.IDX_BO_TOC_TBL_STATUS = bStatus;
	else if (strCore.CompareNoCase(SEARCH_ENGINE_CORE_IDX_SE_TBL) == 0)		g_status.IDX_SE_TBL_STATUS = bStatus;
	else if (strCore.CompareNoCase(SEARCH_ENGINE_CORE_IDX_SE_TOC_TBL) == 0)	g_status.IDX_SE_TOC_TBL_STATUS = bStatus;
}

BOOL CESL_SearchEngine::GetStatus(CString strCore)
{
	if		(strCore.CompareNoCase(SEARCH_ENGINE_CORE_IDX_BO_TBL) == 0)		return g_status.IDX_BO_TBL_STATUS;
	else if (strCore.CompareNoCase(SEARCH_ENGINE_CORE_IDX_BO_TOC_TBL) == 0) return g_status.IDX_BO_TOC_TBL_STATUS;
	else if (strCore.CompareNoCase(SEARCH_ENGINE_CORE_IDX_SE_TBL) == 0)		return g_status.IDX_SE_TBL_STATUS;
	else if (strCore.CompareNoCase(SEARCH_ENGINE_CORE_IDX_SE_TOC_TBL) == 0)	return g_status.IDX_SE_TOC_TBL_STATUS;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////
// PROTECTED FUNCTION

//{{ LOG TYPE
#define SEARCH_ENGINE_WRITE_LOG_TYPE_NORMAL		0
#define SEARCH_ENGINE_WRITE_LOG_TYPE_REQUEST	1
#define SEARCH_ENGINE_WRITE_LOG_TYPE_QUERY		2
#define SEARCH_ENGINE_WRITE_LOG_TYPE_RESPONSE	3
#define SEARCH_ENGINE_WRITE_LOG_TYPE_ERROR		9
//}}
INT CESL_SearchEngine::_WriteLog(CString strLog, INT nOpt)
{
	if (strLog.IsEmpty()) 
		return 10;

	CFileFind fFind;
	if (!fFind.FindFile(_T("..\\CFG\\_SEARCHENGINELOG")))
		return 20;

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	CString strFileName;
	strFileName.Format(_T("..\\tunning\\SEARCHENGINE_%04d%02d%02d.log"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);

	FILE* fp = _tfopen(strFileName, _T("a+b"));
	if (fp != NULL)
	{
		CString strMsg;
		switch(nOpt)
		{
		case SEARCH_ENGINE_WRITE_LOG_TYPE_NORMAL:
			strMsg.Format(_T("[%02d:%02d:%02d.%03d][INFO] %s"), systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds, strLog);
			break;
		case SEARCH_ENGINE_WRITE_LOG_TYPE_REQUEST:
			strMsg.Format(_T("[%02d:%02d:%02d.%03d][REQUEST] %s"), systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds, strLog);
			break;
		case SEARCH_ENGINE_WRITE_LOG_TYPE_QUERY:
			strMsg.Format(_T("[%02d:%02d:%02d.%03d][QUERY] %s"), systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds, strLog);
			break;
		case SEARCH_ENGINE_WRITE_LOG_TYPE_RESPONSE:
			strMsg.Format(_T("[%02d:%02d:%02d.%03d][RESPONSE] %s"), systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds, strLog);
			break;
		case SEARCH_ENGINE_WRITE_LOG_TYPE_ERROR:
			strMsg.Format(_T("[%02d:%02d:%02d.%03d][ERROR] %s"), systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds, strLog);
			break;
		default:
			strMsg.Format(_T("[%02d:%02d:%02d.%03d][UNDEFINED] %s"), systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds, strLog);
			break;
		}

		fseek(fp, 0, SEEK_END);
		if (0 == ftell(fp))
		{
			// 신규생성이라면 FILE BOM 입력
			fputc(0xFF, fp);
			fputc(0xFE, fp);
		}

		strMsg.TrimLeft();
		strMsg.TrimRight();
		
		_ftprintf(fp, _T("%s\r\n"), strMsg);
		fclose(fp);

		return 0;
	}
	
	return -1;
}

INT CESL_SearchEngine::_Request(INT nQueryType, CString strCore, CString strQuery, CString &strResult, INT nOpt/*=0 NOT USED*/)
{
	CString strURL = CESL_SearchEngine::GetURL();
	if (strURL.IsEmpty())
	{
		return -110;
	}

	if (strCore.IsEmpty())
	{
		return -120;
	}

	if (strQuery.IsEmpty())
	{
		return -130;
	}

	CString strService;
	switch (nQueryType)
	{
	case 0:		strService=SEARCH_ENGINE_SERVICE_SELECT;		break;
	case 1:		strService=SEARCH_ENGINE_SERVICE_PING;			break;
	case 2:		strService=SEARCH_ENGINE_SERVICE_UPDATE;		break;
	case 3:		strService=SEARCH_ENGINE_SERVICE_DATAIMPORT;	break;		
	}

	CString strReqUrl;
	strReqUrl.Format(_T("%s/%s/%s"), strURL, strCore, strService); // http://localhost:8080/solr/IDX_BO_TBL/select
	CESL_SearchEngine::_WriteLog(strReqUrl, SEARCH_ENGINE_WRITE_LOG_TYPE_REQUEST);
	CESL_SearchEngine::_WriteLog(strQuery, SEARCH_ENGINE_WRITE_LOG_TYPE_QUERY);

	// URL 유효성 검사
	DWORD dwSearviceType;
	CString strServer;
	CString strObject;
	INTERNET_PORT nPort;
	if (!AfxParseURL(strReqUrl, dwSearviceType, strServer, strObject, nPort))
	{
		return -210; // 유효하지 않는 URL
	}

	LPSTR lpQuery = NULL;
	// 전송데이터 UTF-8 변환		
	SEARCH_ENGINE_CHAR_W2UTF8(strQuery, lpQuery);
	if (lpQuery == NULL)
	{			
		return -220; // 전송데이터 변환 실패
	}

	INT ids = 0;

	CInternetSession Session;
	CHttpConnection* pServer = NULL;
	CHttpFile* pHttpFile = NULL;
	try
	{		
		Session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000 * CESL_SearchEngine::GetTimeOut()); // TIMEOUT
		Session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF, 1000);
		Session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);

		pServer = Session.GetHttpConnection(strServer, nPort); // THROW CInternetException

		// 요청 파마리터 전달용 FILE핸들 생성
		pHttpFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject); // THROW CInternetException

		CString strHeader = _T("Content-Type: application/x-www-form-urlencoded\r\n");
		pHttpFile->SendRequest(strHeader, (LPVOID)lpQuery, strlen(lpQuery)); // THROW CInternetException

		// RESPONSE
		LPSTR lpText = NULL;
		LPSTR lpRead = new CHAR[1024];
		INT nReadLen = 0;
		INT nTotalReadLen = 0;
		do 
		{
			ZeroMemory(lpRead, 1024);
			nReadLen = pHttpFile->Read(lpRead, 1024);
			INT nTextLen = (lpText) ? strlen(lpText) : 0;
			INT nNewTextLen = nTextLen + nReadLen;
			LPSTR lpNewText = new CHAR[nNewTextLen + 1];
			if (lpText) memcpy(lpNewText, lpText, nTextLen);
			if (lpRead) memcpy(lpNewText + nTextLen, lpRead, nReadLen);
			nTotalReadLen += nReadLen;
			lpNewText[nNewTextLen] = '\0';
			if (lpText) delete [] lpText;
			lpText = lpNewText;		
		} 
		while (nReadLen > 0);

		LPWSTR lpResult = NULL;
		SEARCH_ENGINE_CHAR_UTF8TOW(lpText, lpResult);

		strResult.Format(_T("%s"), lpResult);
		CESL_SearchEngine::_WriteLog(strResult, SEARCH_ENGINE_WRITE_LOG_TYPE_RESPONSE);

		if (lpResult)	delete [] lpResult;
		if (lpRead)		delete [] lpRead;
		if (lpText)		delete [] lpText;
	}
	catch (CInternetException* e)
	{
		TCHAR szError[255];
		e->GetErrorMessage(szError, 255);
		e->Delete();

		CESL_SearchEngine::_WriteLog(CString(_T("CInternetException GetErrorMessage : "))+szError, SEARCH_ENGINE_WRITE_LOG_TYPE_ERROR);
		//return -290;
		ids = -290;
	}

	// CLEAR
	if (lpQuery)
	{
		delete [] lpQuery;
	}

	if (pHttpFile)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}

	if (pServer)
	{
		pServer->Close();
		delete pServer;
	}

	Session.Close();

	return ids;
}

#define SEARCH_ENGINE_QUERY_INDENT				_T("indent")
#define SEARCH_ENGINE_QUERY_WRITETYPE			_T("wt")
#define SEARCH_ENGINE_QUERY_START				_T("start")
#define SEARCH_ENGINE_QUERY_ROWS				_T("rows")
#define SEARCH_ENGINE_QUERY_SORT				_T("sort")
#define SEARCH_ENGINE_QUERY_QUERY				_T("q")
#define SEARCH_ENGINE_QUERY_COMMAND				_T("command")
#define SEARCH_ENGINE_QUERY_COMMIT				_T("commit")

#define SEARCH_ENGINE_QUERY_VALUE_DELTAIMPORT	_T("delta-import")


INT CESL_SearchEngine::_MakeDefaultQuery(INT nQueryType, CString &strDefaultQuery, INT nOpt/*=0 NOT USED*/)
{
	// SOLR의 옵션이 변경될수 있기에 명시적으로 쿼리를 생성한다
	CString strWriteTypeQuery;
	switch (CESL_SearchEngine::GetWriteType())
	{
	case 0:	strWriteTypeQuery.Format(_T("%s=json"), SEARCH_ENGINE_QUERY_WRITETYPE);		break;
	case 1:	strWriteTypeQuery.Format(_T("%s=xml"), SEARCH_ENGINE_QUERY_WRITETYPE);		break;
	case 2:	strWriteTypeQuery.Format(_T("%s=python"), SEARCH_ENGINE_QUERY_WRITETYPE);	break;
	case 3:	strWriteTypeQuery.Format(_T("%s=ruby"), SEARCH_ENGINE_QUERY_WRITETYPE);		break;
	case 4:	strWriteTypeQuery.Format(_T("%s=php"), SEARCH_ENGINE_QUERY_WRITETYPE);		break;
	case 5:	strWriteTypeQuery.Format(_T("%s=csv"), SEARCH_ENGINE_QUERY_WRITETYPE);		break;
	}

	CString strIndentQuery;
	if (CESL_SearchEngine::GetIndent())
		strIndentQuery.Format(_T("%s=on"), SEARCH_ENGINE_QUERY_INDENT);
	else
		strIndentQuery.Format(_T("%s=off"), SEARCH_ENGINE_QUERY_INDENT);

	switch (nQueryType)
	{
	case 0: // 조회
		{
			CString strStartQuery;
			strStartQuery.Format(_T("%s=%d"), SEARCH_ENGINE_QUERY_START, CESL_SearchEngine::GetStart());

			CString strRowsQuery;
			strRowsQuery.Format(_T("%s=%d"), SEARCH_ENGINE_QUERY_ROWS, CESL_SearchEngine::GetRows());

			// 정렬조건은 설정되어 있지 않는 경우가 있다
			CString strSortKey;
			BOOL bSortDesc = FALSE;
			CESL_SearchEngine::GetSort(strSortKey, bSortDesc);

			CString strSortQuery;
			if (!strSortKey.IsEmpty())
			{
				if (bSortDesc)
					strSortQuery.Format(_T("%s=%s desc"), SEARCH_ENGINE_QUERY_SORT, strSortKey);
				else
					strSortQuery.Format(_T("%s=%s asc"), SEARCH_ENGINE_QUERY_SORT, strSortKey);
			}

			if (!strSortQuery.IsEmpty())
				strDefaultQuery.Format(_T("%s&%s&%s&%s&%s"), strWriteTypeQuery, strIndentQuery, strStartQuery, strRowsQuery, strSortQuery);
			else
				strDefaultQuery.Format(_T("%s&%s&%s&%s"), strWriteTypeQuery, strIndentQuery, strStartQuery, strRowsQuery);
		}
		break;
	case 1: // PING
		{
			// 추가 쿼리 없음
			strDefaultQuery.Format(_T("%s&%s"), strWriteTypeQuery, strIndentQuery);
		}
		break;
	case 2: // UPDATE
		{	
			// 자동커밋
			CString strCommit;
			strCommit.Format(_T("%s=true"), SEARCH_ENGINE_QUERY_COMMIT);

			strDefaultQuery.Format(_T("%s&%s&%s"), strWriteTypeQuery, strIndentQuery, strCommit);
		}
		break;
	case 3: // DELTAIMPORT
		{
			// 추가 쿼리 없음
			strDefaultQuery.Format(_T("%s&%s"), strWriteTypeQuery, strIndentQuery);
		}
		break;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////
// QUERY FUNCTION

// 조회 서비스
INT CESL_SearchEngine::RequestSelectQuery(CString strCore, CString strQueryKey, CString strQueryValue, CStringArray &RecKeys, INT nOpt/*=0 NOT USED*/)
{
	INT nQueryType = 0;

	if (strCore.IsEmpty() || strQueryKey.IsEmpty() || strQueryValue.IsEmpty())
		return -11;

	if (!CESL_SearchEngine::GetStatus(strCore))
		return -99; // 검색엔진에 문제가 있음. 최초 로그인시에 확인함

	CString strDefaultQuery;
	CESL_SearchEngine::_MakeDefaultQuery(nQueryType, strDefaultQuery);

	CString strParamQuery;
	strParamQuery.Format(_T("%s=%s:%s"), SEARCH_ENGINE_QUERY_QUERY, strQueryKey, strQueryValue);

	CString strQuery;
	strQuery.Format(_T("%s&%s"), strDefaultQuery, strParamQuery);

	CString strResult;
	INT ids = CESL_SearchEngine::_Request(nQueryType, strCore, strQuery, strResult);
	if (ids < 0) return ids;

	switch (CESL_SearchEngine::GetWriteType())
	{
	case 0: // JSON
		{
			CSearchEngineResponseJSON JSON(nQueryType);
			if (!JSON.Parse(strResult))
				return -21;

			// MAKE KEYLIST
			JSON.GetDocsItemDatas(_T("REC_KEY"), RecKeys);				
		}
		break;
	case 1: // XML
	case 2: // PYTHON
	case 3: // RUBY
	case 4: // PHP
	case 5: // CSV
		// NOT SUPPORT
		break;
	}

	return 0;
}

// 상태확인 서비스
INT CESL_SearchEngine::RequestPingQuery(CString strCore, CString &strPingStatus, INT nOpt/*=0 NOT USED*/)
{
	INT nQueryType = 1;

	if (strCore.IsEmpty())
		return -11;

	CString strDefaultQuery;
	CESL_SearchEngine::_MakeDefaultQuery(nQueryType, strDefaultQuery);

	CString strResult;
	INT ids = CESL_SearchEngine::_Request(nQueryType, strCore, strDefaultQuery, strResult);
	if (ids < 0) return ids;

	switch (CESL_SearchEngine::GetWriteType())
	{
	case 0: // JSON
		{
			CSearchEngineResponseJSON JSON(nQueryType);
			if (!JSON.Parse(strResult))
				return -21;

			JSON.GetPingStatus(strPingStatus);
		}
		break;
	case 1: // XML
	case 2: // PYTHON
	case 3: // RUBY
	case 4: // PHP
	case 5: // CSV
		// NOT SUPPORT
		break;
	}

	return 0;
}

INT CESL_SearchEngine::RequestUpdateQuery(CString strCore, CESL_SearchEngineFieldList &Fields, CString &strUpdateResult, INT nOpt/*=0*/)
{
	return -1; // DELTAIMPORT로 대체
}

// DELTAIMPORT 서비스 (업데이트시 사용)
INT CESL_SearchEngine::RequestDeltaImport(CString strCore, CString &strImportResult, INT nOpt/*=0*/)
{
	INT nQueryType = 3;

	if (strCore.IsEmpty())
		return -11;

	if (!CESL_SearchEngine::GetStatus(strCore))
		return -99; // 검색엔진에 문제가 있음. 최초 로그인시에 확인함

	CString strParamQuery;
	strParamQuery.Format(_T("%s=%s"), SEARCH_ENGINE_QUERY_COMMAND, SEARCH_ENGINE_QUERY_VALUE_DELTAIMPORT);

	CString strDefaultQuery;
	CESL_SearchEngine::_MakeDefaultQuery(nQueryType, strDefaultQuery);

	CString strQuery;
	strQuery.Format(_T("%s&%s"), strDefaultQuery, strParamQuery);

	CString strResult;
	INT ids = CESL_SearchEngine::_Request(nQueryType, strCore, strQuery, strResult);
	if (ids < 0) return ids;
	
	switch (CESL_SearchEngine::GetWriteType())
	{
	case 0: // JSON
		{
			CSearchEngineResponseJSON JSON(nQueryType);
			if (!JSON.Parse(strResult))
				return -21;

			INT nStatus = -1;
			if (JSON.GetStatus(nStatus) < 0)
				return -22;

			if (nStatus == 0)
				strImportResult = _T("OK"); // PINGSTATUS와 통일
			else
				strImportResult.Format(_T("%d"), nStatus);
		}
		break;
	case 1: // XML
	case 2: // PYTHON
	case 3: // RUBY
	case 4: // PHP
	case 5: // CSV
		// NOT SUPPORT
		break;
	}

	return 0;
}

// 삭제 서비스
INT CESL_SearchEngine::RequestDeleteQuery(CString strCore, CStringArray &RecKeys, CString &strDeleteResult, INT nOpt/*=0*/)
{
	INT nQueryType = 2;

	if (strCore.IsEmpty())
		return -11;
	
	INT nCount = RecKeys.GetSize();
	if (nCount == 0)
		return -12;
	
	CString strList;
	for (INT i=0; i<nCount; i++)
	{
		CString strItem;
		strItem.Format(_T("<id>%s</id>"), RecKeys.GetAt(i));
		
		strList += strItem;
	}

	CString strParamQuery;
	strParamQuery.Format(_T("stream.body=<delete>%s</delete>"), strList);

	CString strDefaultQuery;
	CESL_SearchEngine::_MakeDefaultQuery(nQueryType, strDefaultQuery);

	CString strQuery;
	strQuery.Format(_T("%s&%s"), strDefaultQuery, strParamQuery);

	CString strResult;
	INT ids = CESL_SearchEngine::_Request(nQueryType, strCore, strQuery, strResult);
	if (ids < 0) return ids;
	
	switch (CESL_SearchEngine::GetWriteType())
	{
	case 0: // JSON
		{
			CSearchEngineResponseJSON JSON(nQueryType);
			if (!JSON.Parse(strResult))
				return -21;
			
			INT nStatus = -1;
			if (JSON.GetStatus(nStatus) < 0)
				return -22;

			if (nStatus == 0)
				strDeleteResult = _T("OK"); // PINGSTATUS와 통일
			else
				strDeleteResult.Format(_T("%d"), nStatus);
		}
		break;
	case 1: // XML
	case 2: // PYTHON
	case 3: // RUBY
	case 4: // PHP
	case 5: // CSV
		// NOT SUPPORT
		break;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// class CSearchEngineResponseJSON 결과 JSON처리용 클래스 (FOR SOLR ONLY)	
#define SEARCH_ENGINE_JSON_ALIAS_RES_HEADER			"responseHeader"
#define SEARCH_ENGINE_JSON_ALIAS_RES_HEADER_STATUS	"status"
#define SEARCH_ENGINE_JSON_ALIAS_RES_HEADER_QTIME	"QTime"
#define SEARCH_ENGINE_JSON_ALIAS_RES_HEADER_PARAMS	"params"
#define SEARCH_ENGINE_JSON_ALIAS_RES_DATA			"response"
#define SEARCH_ENGINE_JSON_ALIAS_RES_DATA_NUMFOUND	"numFound"
#define SEARCH_ENGINE_JSON_ALIAS_RES_DATA_START		"start"
#define SEARCH_ENGINE_JSON_ALIAS_RES_DATA_DOCS		"docs"
#define SEARCH_ENGINE_JSON_ALIAS_PING_STATUS		"status"

#define SEARCH_ENGINE_JSON_RES_TYPE_SELECT		0
#define SEARCH_ENGINE_JSON_RES_TYPE_PING		1
#define SEARCH_ENGINE_JSON_RES_TYPE_UPDATE		2
#define SEARCH_ENGINE_JSON_RES_TYPE_DATAIMPORT	3

CESL_SearchEngine::CSearchEngineResponseJSON::CSearchEngineResponseJSON(INT nResponseType/*=0*/)
{
	SetResonseType(nResponseType);
}

CESL_SearchEngine::CSearchEngineResponseJSON::~CSearchEngineResponseJSON()
{
	Clear();
}

VOID CESL_SearchEngine::CSearchEngineResponseJSON::Clear()
{
	m_value.clear();
}

BOOL CESL_SearchEngine::CSearchEngineResponseJSON::Parse(CString strJSON)
{
	if (1 > strJSON.GetLength()) 
		return FALSE;

	Clear();
	
	Json::Reader Reader;

	LPSTR lpJSON = NULL;
	SEARCH_ENGINE_CHAR_W2ACP(strJSON, lpJSON);

	Reader.parse(lpJSON, m_value);
	// FREE MEMORY
	delete [] lpJSON;

	if (!m_value.isNull())
	{
		m_valueResHeader = m_value[SEARCH_ENGINE_JSON_ALIAS_RES_HEADER];
			m_valueStatus = m_valueResHeader[SEARCH_ENGINE_JSON_ALIAS_RES_HEADER_STATUS];
			m_valueQTime = m_valueResHeader[SEARCH_ENGINE_JSON_ALIAS_RES_HEADER_QTIME];
			m_valueParams = m_valueResHeader[SEARCH_ENGINE_JSON_ALIAS_RES_HEADER_PARAMS];

		switch (GetResonseType())
		{
		case SEARCH_ENGINE_JSON_RES_TYPE_SELECT:
			{
				m_valueResData = m_value[SEARCH_ENGINE_JSON_ALIAS_RES_DATA];
					m_valueResNumFound = m_valueResData[SEARCH_ENGINE_JSON_ALIAS_RES_DATA_NUMFOUND];
					m_valueResStart = m_valueResData[SEARCH_ENGINE_JSON_ALIAS_RES_DATA_START];
					m_valueResDocs = m_valueResData[SEARCH_ENGINE_JSON_ALIAS_RES_DATA_DOCS];
			}
			break;
		case SEARCH_ENGINE_JSON_RES_TYPE_PING:
			{
				m_valuePingStatus = m_value[SEARCH_ENGINE_JSON_ALIAS_PING_STATUS];				
			}
			break;
		case SEARCH_ENGINE_JSON_RES_TYPE_UPDATE:
			{
				
			}
			break;
		case SEARCH_ENGINE_JSON_RES_TYPE_DATAIMPORT:
			{
				
			}
			break;
		}

		return TRUE;
	}

	return FALSE;	
}

INT CESL_SearchEngine::CSearchEngineResponseJSON::GetStatus(INT &nStatus)
{
	if (m_valueStatus.isNull()) return -1;
	if (!m_valueStatus.isNumeric()) return -2;

	nStatus = m_valueStatus.asInt();

	return 0;
}

INT CESL_SearchEngine::CSearchEngineResponseJSON::GetNumFound(INT &nNumFound)
{
	if (m_valueResNumFound.isNull()) return -1;
	if (!m_valueResNumFound.isNumeric()) return -2;

	nNumFound = m_valueResNumFound.asInt();

	return 0;
}

INT CESL_SearchEngine::CSearchEngineResponseJSON::GetStart(INT &nStart)
{
	if (m_valueResStart.isNull()) return -1;
	if (!m_valueResStart.isNumeric()) return -2;

	nStart = m_valueResStart.asInt();

	return 0;
}

INT CESL_SearchEngine::CSearchEngineResponseJSON::GetDocsSize()
{
	if (m_valueResDocs.isNull()) return 0;

	return m_valueResDocs.size();
}

INT CESL_SearchEngine::CSearchEngineResponseJSON::GetDocsItemData(INT nItemIdx, CString strItemAlias, CString &strItemData)
{
	if (m_valueResDocs.isNull()) return -1;
	if (!m_valueResDocs.isValidIndex(nItemIdx)) return -2;

	Json::Value Item = m_valueResDocs[nItemIdx];
	if (Item.isNull()) return -3;

	LPSTR lpItemAlias = NULL;
	SEARCH_ENGINE_CHAR_W2ACP(strItemAlias, lpItemAlias);

	Json::Value ItemValue = Item[lpItemAlias];
	delete [] lpItemAlias;

	//if (ItemValue.isNull()) return -4;
	if (!ItemValue.isNull())
	{
		strItemData = CString(ItemValue.asCString());
	}

	return 0;
}

INT CESL_SearchEngine::CSearchEngineResponseJSON::GetDocsItemDatas(CString strItemAlias, CStringArray &ItemDatas)
{
	INT nSize = GetDocsSize();
	if (nSize > 0) 
	{
		LPSTR lpItemAlias = NULL;
		SEARCH_ENGINE_CHAR_W2ACP(strItemAlias, lpItemAlias);

		for (INT i=0; i<nSize; i++)
		{
			Json::Value Item = m_valueResDocs[i];
			if (!Item.isNull())
			{
				Json::Value ItemValue = Item[lpItemAlias];
				if (!ItemValue.isNull())
				{
					CString strItemData = CString(ItemValue.asCString());

					ItemDatas.Add(strItemData);
				}
			}
		}

		delete [] lpItemAlias;
	}

	return 0;
}
		
INT CESL_SearchEngine::CSearchEngineResponseJSON::GetPingStatus(CString &strStatus)
{
	if (m_valuePingStatus.isNull()) return -1;

	strStatus = CString(m_valuePingStatus.asCString());

	return 0;
}

