// KolisNetApi.cpp : implementation file
// 2017/09/21 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// KOLIS-NET OpenAPI

#include "stdafx.h"
#include "KolisNetApi.h"
#include <fstream>
#include <afxinet.h>
#include <malloc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKolisNetSender

CKolisNetSender::CKolisNetSender() {

	// DefaultKolisNetOpenApiInfo.log 파일의 URL 내용이 로그인시 저장된 값보다 우선순위가 높음
	CString fileName, data, url;
	CFileFind filefind;
	fileName.Format(_T("..\\CFG\\EFS\\DefaultKolisNetOpenApiInfo.log"));
	BOOL bIsDefault = FALSE;
	if (TRUE == filefind.FindFile(fileName)) {
		CStdioFile file;
		if (file.Open(fileName, CFile::modeRead|CFile::typeBinary)) {
			TCHAR cUni;
			file.Read(&cUni, sizeof(TCHAR));
			if (0xFEFF != cUni) file.SeekToBegin();
			file.ReadString(data);
			data.TrimLeft(); data.TrimRight();
			AfxExtractSubString(url, data, 0, CHAR(30));
			file.Close();
			if (!url.IsEmpty()) {
				bIsDefault = TRUE;
				SetUrl(url);
			}
		}
	}

	// KOLASIII 로그인시 저장된 KOLIS-NET정보
	// URL+CHAR(30)+LIB_CODE+CHAR(30)+IP+CHAR(30)+USER_ID
	// URL 정보는 DefaultKolisNetOpenApiInfo.log 파일의 내용을 우선으로 적용
	DWORD dwPID = ::GetCurrentProcessId();	
	fileName.Format(_T("..\\CFG\\EFS\\KolisNetOpenApiInfo(%u).log"), dwPID);
	CStdioFile file;
	if (file.Open(fileName, CFile::modeRead|CFile::typeBinary)) {
		TCHAR cUni;
		file.Read(&cUni, sizeof(TCHAR));
		if (0xFEFF != cUni) file.SeekToBegin();
		file.ReadString(data);
		data.TrimLeft(); data.TrimRight();
		if (FALSE == bIsDefault) AfxExtractSubString(url, data, 0, CHAR(30));
		AfxExtractSubString(m_strLibCode, data, 1, CHAR(30));
		AfxExtractSubString(m_strLocalIP, data, 2, CHAR(30));
		AfxExtractSubString(m_strUserID, data, 3, CHAR(30));
		file.Close();
		SetUrl(url);
	} else {
		SetUrl(_T(""));
	}

	// TODO : 테스트용 임시지정
	if (GetUrl().IsEmpty())
	{
		SetUrl(_T("http://192.168.100.119:8080/search/kolisnet/"));
		//SetUrl(_T("http://172.17.100.105:8080/search/kolisnet/"));
	}

	// test mode : _kolisnet_testmode 파일 생성시 테스트 모드로 요청
	fileName.Format(_T("..\\CFG\\_kolisnet_testmode"));
	if (TRUE == filefind.FindFile(fileName)) {
		SetTestMode(TRUE);
	} else {
		SetTestMode(FALSE);
	}

	SetService(_T(""));
	InitParam();
}

CKolisNetSender::~CKolisNetSender() {
	InitParam();
}

void CKolisNetSender::SetUrl(CString url) {
	m_strUrl = url;
}

CString CKolisNetSender::GetUrl() {
	if (!m_strUrl.IsEmpty()) {
		if (0 != m_strUrl.Right(1).Compare(_T("/"))) {
			m_strUrl += _T("/");
		}
	}
	return m_strUrl;
}

void CKolisNetSender::SetService(CString service) {
	m_strService = service;
}

CString CKolisNetSender::GetService() {
	return m_strService;
}

void CKolisNetSender::InitParam() {
	SetParam(_T(""));
	m_saParams.RemoveAll();
	m_saParams.FreeExtra();
}

void CKolisNetSender::AddParam(CString alias, CString value) {
	alias.TrimLeft(); alias.TrimRight();
	value.TrimLeft(); value.TrimRight();
	CStrConvert convert;
	m_saParams.Add(alias + _T("=") + convert.URL_Encode(value));
}

// AddParam을 통한 등록된 parameter 정보를 조합하여 설정한다.
void CKolisNetSender::SetParam() {
	CString param;
	for (INT i = 0; i < m_saParams.GetSize(); i++) {
		if (!param.IsEmpty()) param += _T("&");
		param += m_saParams.GetAt(i);
	}

	SetParam(param);
}

void CKolisNetSender::SetParam(CString param) {
	m_strParam = param;
}

CString CKolisNetSender::GetParam() {
	return m_strParam;
}

void CKolisNetSender::SetTestMode(BOOL mode) {
	m_bTestMode = mode;
}

CString CKolisNetSender::GetTestMode() {
	return m_bTestMode;
}

// KOLIS-NET에 POST방식으로 요청한다. (저장된 URL과 PARAM 활용)
// 성공시 Json형식의 값이 리턴, 실패시 실패사유
BOOL CKolisNetSender::KolisNetRequest(CString &result) {
	// check
	if (GetUrl().IsEmpty()) {
		result = _T("KOLIS-NET OpenAPI URL 정보가 없습니다.");
		writeLog(result, _T("error"));
		return FALSE;
	}
	if (GetService().IsEmpty()) {
		result = _T("KOLIS-NET OpenAPI Service 정보가 없습니다.");
		writeLog(result, _T("error"));
		return FALSE;
	}
	if (GetParam().IsEmpty()) {
		SetParam();
		if (GetParam().IsEmpty()) {
			result = _T("KOLIS-NET OpenAPI PARAMETER 정보가 없습니다.");
			writeLog(result, _T("error"));
			return FALSE;
		}
	}

	// request
	CString url = GetUrl() + GetService();
	writeLog(url, _T("request-url"));
	DWORD dwSearviceType;
	CString strServer, strObject;
	INTERNET_PORT nPort;
	if (!AfxParseURL(url, dwSearviceType, strServer, strObject, nPort)) {
		result = _T("KOLIS-NET OpenAPI URL 정보가 형식에 맞지 않습니다.");
		writeLog(_T("AfxParseURL fail"), _T("error"));
		return FALSE;
	}
	CInternetSession Session;
	CHttpConnection* pServer = NULL;
	CHttpFile *pFile = NULL;
	CStrConvert convert;
	writeLog(GetParam(), _T("request-param"));
	LPSTR sendData = convert.WideCharToUTF8(GetParam());	
	try {
		// timeout : 5 sec
		Session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000 * 5);
		Session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF, 1000);
		Session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);

		if(dwSearviceType==AFX_INET_SERVICE_HTTPS) //HTTPS / HTTP
		{
			//INTERNET_OPEN_TYPE_PRECONFIG|INTERNET_OPEN_TYPE_DIRECT|INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY|INTERNET_OPEN_TYPE_PROXY
			pServer = Session.GetHttpConnection(strServer, INTERNET_FLAG_SECURE, nPort);
			
			pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject, NULL, 0, NULL, NULL, INTERNET_FLAG_RELOAD|INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_SECURE|INTERNET_FLAG_IGNORE_CERT_CN_INVALID);
				
			DWORD dwSecFlags;
			pFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS,dwSecFlags);
			dwSecFlags |=SECURITY_FLAG_IGNORE_UNKNOWN_CA;
			dwSecFlags |=SECURITY_FLAG_IGNORE_CERT_CN_INVALID;
			dwSecFlags |=SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
			dwSecFlags |=SECURITY_FLAG_IGNORE_REVOCATION;
			pFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS,dwSecFlags);
		}
		else 
		{
			pServer = Session.GetHttpConnection(strServer, nPort);
			pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject);
		}
		
		CString strHeader = _T("Content-Type: application/x-www-form-urlencoded\r\n");
		pFile->SendRequest(strHeader, (LPVOID)sendData, strlen(sendData));
		delete []sendData;
		sendData = NULL;
	} catch (CInternetException* e) {
		TCHAR szError[255];

		CString ff;
		ff.Format(_T("%x"),e->m_dwContext);ff.Empty();
		writeLog( ff , _T("error"));
		ff.Format(_T("%d"),e->m_dwError);
		writeLog( ff , _T("error"));
		e->GetErrorMessage(szError, 255);
		e->Delete();
		if (sendData) delete []sendData;
		result = _T("KOLIS-NET OpenAPI 요청에 실패하였습니다.");
		writeLog(CString(_T("CInternetException GetErrorMessage : "))+szError, _T("error"));
		return FALSE;
	}

	// response
	LPSTR lpText = NULL;
	LPSTR lpRead = new CHAR[1024];
	INT nReadLen = 0;
	INT nTotalReadLen = 0;
	do {
		ZeroMemory(lpRead, 1024);
		nReadLen = pFile->Read(lpRead, 1024);
		INT nTextLen = (lpText) ? strlen(lpText) : 0;
		INT nNewTextLen = nTextLen + nReadLen;
		LPSTR lpNewText = new CHAR[nNewTextLen+1];
		if (lpText) memcpy(lpNewText, lpText, nTextLen);
		if (lpRead) memcpy(lpNewText+nTextLen, lpRead, nReadLen);
		nTotalReadLen+=nReadLen;
		lpNewText[nNewTextLen] = '\0';
		if (lpText) delete []lpText;
		lpText = lpNewText;		
	} while (nReadLen > 0);
	LPWSTR szBuffer = convert.UTF8ToWideChar(lpText);
	result.Format(_T("%s"), szBuffer);
	writeLog(result, _T("response"));

	// clear
	delete []szBuffer;
	delete []lpRead;
	delete []lpText;
	pFile->Close();
	pServer->Close();
	Session.Close();
	delete pFile;
	delete pServer;
	
	return TRUE;
}

BOOL CKolisNetSender::KolisNetfileDownload(CString localFileName, CString typeCode, CString baseKey, CString serverFileName, CString &result) {
	// download (1. response string -> 2. url decoding -> 3. base64 decoding -> 4. file write)
	// 1. response string
	CKolisNetSender sender;
	sender.SetUrl(GetUrl());
	sender.SetService(_T("getFileToServer.nl"));
	sender.AddParam(_T("type_code"), typeCode);
	sender.AddParam(_T("base_key"), baseKey);
	sender.AddParam(_T("file_name"), serverFileName);
	if (!sender.KolisNetRequest(result)) {
		return FALSE;
	}
	CJsonReader reader;
	if (!reader.parse(result)) {
		result = _T("KOLIS-NET OpenAPI 파일 다운로드에 실패하였습니다.");
		return FALSE;
	}
	CString resultCode, msg;
	if (0 > reader.get_resultCode_Value(resultCode)) {
		result = _T("KOLIS-NET OpenAPI 파일 다운로드에 실패하였습니다.");
		return FALSE;
	}
	if (_T("1") == resultCode) { // 0:성공, 1:실패
		reader.get_errorMsg_Value(msg);
		result = _T("KOLIS-NET OpenAPI 파일 다운로드에 실패하였습니다.\r\nerror message : ") + msg;
		return FALSE;
	}
	CString data;
	reader.get_fdata_Value(_T("file_data"), 0, data);
	if (data.IsEmpty()) {
		result = _T("file_data is null");
		return FALSE;
	}
	// 2. url decoding 
	//  - get_fdata_Value에서 수행
	// 3. base64 decoding
	CStrConvert convert;
	PBYTE getData = convert.BASE64_Decode(data);
	if (!getData) {
		result = _T("BASE64 Decoding fail");
		return FALSE;
	}
	// 4. file write
	FILE *file = _tfopen(localFileName, _T("wb"));
	if (!file) {
		result = _T("파일 생성에 실패하였습니다.");
		return FALSE;
	}
	size_t writeSize = fwrite(getData, sizeof(BYTE), _msize(getData), file);
	fclose(file);
	if (writeSize != _msize(getData)) {
		result = _T("파일 쓰기에 실패하였습니다.");
		return FALSE;
	}
	delete []getData;

	return TRUE;
}

BOOL CKolisNetSender::KolisNetfileUpload(CString localFileName, CString typeCode, CString baseKey, CString serverFileName, CString &result) {
	// upload (1. file read -> 2. base64 encoding -> 3. url encoding -> 4. request string)
	// 1. file read
	FILE *fp = _tfopen(localFileName, _T("rb"));
	if (!fp) {
		result = _T("파일 열기에 실패하였습니다.");
		return FALSE;
	}
	fseek(fp, 0, SEEK_END);
	LONG fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	PBYTE readData = new BYTE[fileSize];
	size_t readSize = fread(readData, sizeof(BYTE), fileSize, fp);
	fclose(fp);
	if (readSize != fileSize) {
		result = _T("파일 읽기에 실패하였습니다.");
		return FALSE;
	}
	CString strSize;
	strSize.Format(_T("%u"), fileSize);
	// 2. base64 encoding
	CStrConvert convert;
	CString BASE64Enc = convert.BASE64_Encode(readData);
	delete []readData;
	// 3. url encoding
	//  - AddParam에서 수행
	// 4. request string
	CKolisNetSender sender;
	sender.SetUrl(GetUrl());
	sender.SetService(_T("putFileToServer.nl"));
	sender.AddParam(_T("type_code"), typeCode);
	sender.AddParam(_T("base_key"), baseKey);
	sender.AddParam(_T("file_name"), serverFileName);
	sender.AddParam(_T("file_data"), BASE64Enc);
	sender.AddParam(_T("file_size"), strSize);
	sender.AddParam(_T("commit_yn"), _T("Y"));
	if (!sender.KolisNetRequest(result)) {
		return FALSE;
	}
	CJsonReader reader;
	if (!reader.parse(result)) {
		result = _T("KOLIS-NET OpenAPI 파일 업로드에 실패하였습니다.");
		return FALSE;
	}
	CString resultCode, msg;
	if (0 > reader.get_resultCode_Value(resultCode)) {
		result = _T("KOLIS-NET OpenAPI 파일 업로드에 실패하였습니다.");
		return FALSE;
	}
	if (_T("0") != resultCode) { // 0:성공, 1:실패
		reader.get_errorMsg_Value(msg);
		result = _T("KOLIS-NET OpenAPI 파일 업로드에 실패하였습니다.\r\nerror message : ") + msg;
		return FALSE;
	}

	return TRUE;
}

VOID CKolisNetSender::writeLog(CString log, CString mode) {
	if (log.IsEmpty()) return;
	SYSTEMTIME time;
	GetLocalTime(&time);
	CString filename, msg;
	filename.Format(_T("..\\tunning\\KOLIS-NET_%04d%02d%02d.log"), time.wYear, time.wMonth, time.wDay);
	msg.Format(_T("[%02d:%02d:%02d.%03d][%s]\t :: %s"), time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, mode, log);
	CFileFind filefind;
	if (filefind.FindFile(_T("..\\cfg\\_kolisapilog"))) {
		FILE* fp;
		fp = _tfopen(filename, _T("a+b"));
		if (fp == NULL) return;
		fseek(fp, 0, SEEK_END);
		if (0 == ftell(fp)) {
			fputc(0xFF, fp);
			fputc(0xFE, fp);
		}
		msg.TrimLeft(); msg.TrimRight();
		_ftprintf(fp, _T("%s\r\n"), msg);
		fclose(fp);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CJsonReader

CJsonReader::CJsonReader() {
}

CJsonReader::~CJsonReader() {
	clear();
}

BOOL CJsonReader::parse(CString frame) {
	if (1 > frame.GetLength()) return FALSE;
	
	Json::Reader reader;
	BOOL bResult = FALSE;
	m_value.clear();
	CStrConvert convert;
	LPSTR szFrame = convert.WideCharToMultiByte(frame);
	bResult = reader.parse(szFrame, m_value);
	delete []szFrame;
	
	if(m_value.isNull()) return FALSE;
	m_total = m_value[JSON_TOTAL_ALIAS];
	m_totalRow = m_value[JSON_TOTALROW_ALIAS];
	m_columnNames = m_value[JSON_COLUMNNAMES_ALIAS];
	m_fdata = m_value[JSON_FDATA_ALIAS];
	m_resultCode = m_value[JSON_RESULTCODE_ALIAS];
	m_resultValue = m_value[JSON_RESULTVALUE_ALIAS];
	m_errorMsg = m_value[JSON_ERRORMSG_ALIAS];

	return bResult;
}

BOOL CJsonReader::parseFile(CString path) {
	if (1 > path.GetLength()) return FALSE;

	CStrConvert convert;
	LPSTR szFrame = convert.WideCharToMultiByte(path);
	std::ifstream ifs(szFrame);
	delete []szFrame;

	Json::Reader reader;
	BOOL bResult = FALSE;
	m_value.clear();
	bResult = reader.parse(ifs, m_value);

	if(m_value.isNull()) return FALSE;
	m_total = m_value[JSON_TOTAL_ALIAS];
	m_totalRow = m_value[JSON_TOTALROW_ALIAS];
	m_columnNames = m_value[JSON_COLUMNNAMES_ALIAS];
	m_fdata = m_value[JSON_FDATA_ALIAS];
	m_resultCode = m_value[JSON_RESULTCODE_ALIAS];
	m_resultValue = m_value[JSON_RESULTVALUE_ALIAS];
	m_errorMsg = m_value[JSON_ERRORMSG_ALIAS];

	return bResult;
}

INT CJsonReader::get_total_Value() {
	if (m_total.isNull()) return 0;
	if (!m_total.isNumeric()) return 0;

	return m_total.asInt();
}

INT CJsonReader::get_totalRow_Value() {
	if (m_totalRow.isNull()) return 0;
	if (!m_totalRow.isNumeric()) return 0;

	return m_totalRow.asInt();
}

INT CJsonReader::get_columnName_Size() {
	if (m_columnNames.isNull()) return 0;

	return m_columnNames.size();
}

INT CJsonReader::get_columnName_Value(INT index, CString &columnName) {
	if (m_columnNames.isNull()) return -1;
	if (!m_columnNames.isValidIndex(index)) return -2;

	columnName = m_columnNames[index].asCString();

	return 0;
}

CString *CJsonReader::get_columnName_ArrayValue() {
	if (m_columnNames.isNull()) return NULL;
	
	INT nColCnt = get_columnName_Size();
	if (1 > nColCnt) return NULL;

	CString* result = new CString[nColCnt];
	for (INT i = 0; i < nColCnt; i++) {
		result[i] = m_columnNames[i].asCString();
	}

	return result;
}

INT CJsonReader::get_columnName_Index(CString columnName) {
	if (m_columnNames.isNull()) return -1;
	
	INT index = -1;
	for (INT i = 0; i < m_columnNames.size(); i++) {
		if (0 == columnName.Compare(CString(m_columnNames[i].asCString()))) {
			index = i;
			break;
		}
	}
	if (0 > index) return -2;

	return index;
}

INT CJsonReader::get_fdata_Size() {
	if (m_fdata.isNull()) return 0;
	
	return m_fdata.size();
}

INT CJsonReader::get_fdata_Value(CString columnName, INT row, CString &value) {
	if (m_fdata.isNull()) return -1;
	
	INT index = get_columnName_Index(columnName);
	if (0 > index) return -2;

	value = get_fdata_Value(index, row);

	return 0;
}

CString CJsonReader::get_fdata_Value(CString columnName, INT row) {
	CString result;
	if (0 > get_fdata_Value(columnName, row, result)) result = _T("");
	return result;
}

CString CJsonReader::get_fdata_Value(INT col, INT row) {
	CString result;
	if (0 > get_fdata_Value(col, row, result)) result = _T("");
	return result;
}

INT CJsonReader::get_fdata_Value(INT col, INT row, CString &value) {
	if (m_fdata.isNull()) return -1;
	if (0 > row) return -2;
	
	const Json::Value list = m_fdata[row];
	if (list.isNull()) return -3;
	if (!list.isValidIndex(col)) return -4;
	
	CStrConvert convert;
	value = convert.URL_Decode(CString(list[col].asCString()));
	
	return 0;
}

CString **CJsonReader::get_fdata_ArrayValue() {
	if (m_fdata.isNull()) return NULL;
	if (1 > m_fdata.size()) return NULL;
	INT row = get_totalRow_Value();
	if (1 > row) return NULL;
	INT col = get_columnName_Size();
	if (1 > col) return NULL;

	CString **result = new CString *[row];
	for (INT i = 0; i < row; i++) {
		result[i] = new CString[col];
	}

	CStrConvert convert;
	for (i = 0; i < row; i++) { // row
		const Json::Value list = m_fdata[i];
		for (INT j = 0; j < col; j++) { // col
			// arrayValue[row][col]
			result[i][j] = convert.URL_Decode(CString(list[j].asCString()));
		}
	}
	
	return result;
}

INT CJsonReader::get_resultCode_Value(CString &value) {
	if (m_resultCode.isNull()) return -1;
	if (!m_resultCode.isString()) return -2;

	value = m_resultCode.asCString();
	
	return 0;
}

INT CJsonReader::get_resultValue_Value(CString &value) {
	if (m_resultValue.isNull()) return -1;
	if (!m_resultValue.isString()) return -2;

	value = m_resultValue.asCString();

	return 0;
}

INT CJsonReader::get_errorMsg_Value(CString &value) {
	if (m_errorMsg.isNull()) return -1;
	if (!m_errorMsg.isString()) return -2;

	value = m_errorMsg.asCString();

	return 0;
}

void CJsonReader::clear() {
	m_value.clear();
}

/////////////////////////////////////////////////////////////////////////////
// CStrConvert

CStrConvert::CStrConvert() {
}

CStrConvert::~CStrConvert() {
}

LPSTR CStrConvert::WideCharToMultiByte(LPCWSTR str) {
	LPSTR result = NULL;
	INT size = ::WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, NULL, NULL, NULL);
	if (0 < size) {
		result = new CHAR[size+1];
		ZeroMemory(result, size+1);
		size = ::WideCharToMultiByte(CP_ACP, 0, str, -1, result, size, NULL, NULL);
	}
	return result;
}

LPSTR CStrConvert::WideCharToUTF8(LPCWSTR str) {
	LPSTR result = NULL;
	INT size = ::WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, NULL, NULL, NULL);
	if (0 < size) {
		result = new CHAR[size+1];
		ZeroMemory(result, size+1);
		size = ::WideCharToMultiByte(CP_UTF8, 0, str, -1, result, size, NULL, NULL);
	}
	return result;
}

LPWSTR CStrConvert::MultiByteToWideChar(LPCSTR str) {
	LPWSTR result = NULL;
	INT size = ::MultiByteToWideChar(CP_ACP, NULL, str, -1, NULL, NULL);	
	if (0 < size) {
		result = new WCHAR[size+1];
		ZeroMemory(result, size+1);
		size = ::MultiByteToWideChar(CP_ACP, NULL, str, -1, result, size);
	}
	return result;
}

LPSTR CStrConvert::MultiByteToUTF8(LPCSTR str) {
	LPWSTR buff = MultiByteToWideChar(str);
	LPSTR result = WideCharToUTF8(buff);
	delete []buff;
	return result;
}

LPWSTR CStrConvert::UTF8ToWideChar(LPCSTR str) {
	LPWSTR result = NULL;
	INT size = ::MultiByteToWideChar(CP_UTF8, NULL, str, -1, NULL, NULL);	
	if (0 < size) {
		result = new WCHAR[size+1];
		ZeroMemory(result, size+1);
		size = ::MultiByteToWideChar(CP_UTF8, NULL, str, -1, result, size);
	}
	return result;
}

LPSTR CStrConvert::UTF8ToMultiByte(LPCSTR str) {
	LPWSTR buff = UTF8ToWideChar(str);
	LPSTR result = WideCharToMultiByte(buff);
	delete []buff;
	return result;
}

inline BYTE CStrConvert::toHex(const BYTE &x) {
	return x > 9 ? x + 55: x + 48;
}

CString CStrConvert::URL_Encode(CString str) {
	if (str.IsEmpty()) return _T("");

	LPSTR in = WideCharToUTF8(str);
	PBYTE out = URL_Encode(in);
	
	CString result;
	LPWSTR szBuffer = UTF8ToWideChar((LPCSTR)out);
	result = CString(szBuffer);
	
	delete []in;
	delete []out;
	delete []szBuffer;
	
	return result;
}

PBYTE CStrConvert::URL_Encode(LPCSTR src) {
	if (!src || 1 > strlen(src)) return NULL;

	LPBYTE pInBuf =(LPBYTE)src;
	LPBYTE pOutBuf = new BYTE[strlen(src)*3];
	ZeroMemory(pOutBuf, strlen(src)*3);

	UINT idest = 0;
	register LPBYTE pOutTmp = NULL;
	register LPBYTE pInTmp = NULL;

	if(pOutBuf) {
		pInTmp = pInBuf;
		pOutTmp = pOutBuf;
		while (*pInTmp) {
			if (('0' <= *pInTmp && '9' >= *pInTmp) || 
 				('a' <= *pInTmp && 'z' >= *pInTmp) || 
				('A' <= *pInTmp && 'Z' >= *pInTmp) || 
 				('-' == *pInTmp || '_' == *pInTmp || '.' == *pInTmp || '@' == *pInTmp || '*' == *pInTmp)) {
				// do not encode
				*pOutTmp++ = *pInTmp;
				idest++;
			} else if (' ' == *pInTmp) {
				// space
				*pOutTmp++ = '+';
				idest++;
			} else {
				// encode
				*pOutTmp++ = '%';
				*pOutTmp++ = toHex(*pInTmp>>4);
				*pOutTmp++ = toHex(*pInTmp%16);
				idest += 3;
			}
			pInTmp++;
		}
	}

	LPBYTE result = new BYTE[idest+1];
	ZeroMemory(result, idest+1);
	memcpy(result, pOutBuf, idest);

	delete []pOutBuf;

	return result;
}

CString CStrConvert::URL_Decode(CString str) {
	if (str.IsEmpty()) return _T("");

	LPSTR in = WideCharToMultiByte(str);
	LPSTR out = URL_Decode(in);

	CString result;
	LPWSTR szBuffer = UTF8ToWideChar(out);
	result = CString(szBuffer);

	delete []in;
	delete []out;
	delete []szBuffer;
	
	return result;
}

LPSTR CStrConvert::URL_Decode(LPCSTR src) {
	if (!src || 1 > strlen(src)) return NULL;

    UINT isrc = 0;
    UINT idest = 0;
    CHAR charcode[] = "0x000";

	UINT len = strlen(src);
	LPSTR dec = new CHAR[len+1];
	ZeroMemory(dec, len+1);
    
    while (isrc <= len) {
        if('+' == src[isrc]) {
            dec[idest] = ' ';
            isrc++;
            idest++;
        } else if('%' == src[isrc]) {
            isrc++;
            charcode[3] = src[isrc];
            charcode[4] = src[isrc+1];
            dec[idest] = strtol(charcode, NULL, 16);
            isrc += 2;
            idest++;
        } else {
            dec[idest] = src[isrc];
            idest++;
            isrc++;
        }
    }

	LPSTR result = new CHAR[idest];
	ZeroMemory(result, idest);
	memcpy(result, dec, idest-1);

	delete []dec;

	return result;
}

CString CStrConvert::BASE64_Encode(PBYTE data) {
	if (!data || 1 > _msize(data)) return _T("");

	UINT size = _msize(data);
	UINT size_result = size/3*4;
	if (size%3 > 0) size_result += 4;

	LPSTR enc = new CHAR[size_result+1];
	ZeroMemory(enc, size_result+1);
	PBYTE data_pos = data;
	LPSTR result_pos = enc;
	LPCSTR base64key = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	
	for (INT i = 0; i < size/3; ++i) {
		*result_pos++ = base64key[*data_pos>>2];
		*result_pos++ = base64key[((*data_pos&3)<<4)+(*(data_pos+++1)>>4)];
		*result_pos++ = base64key[((*data_pos&15)<<2)+(*(data_pos+++1)>>6)];
		*result_pos++ = base64key[*data_pos&63];
		++data_pos;
	}
	
	switch (size%3) {
	case 1:
		*result_pos++ = base64key[*data_pos>>2];
		*result_pos++ = base64key[(*data_pos&3)<<4];
		*result_pos++ = '=';
		*result_pos++ = '=';
		break;
	case 2:
		*result_pos++ = base64key[*data_pos>>2];
		*result_pos++ = base64key[((*data_pos&3)<<4)+(*(data_pos+++1)>>4)];
		*result_pos++ = base64key[(*data_pos&15)<<2];
		*result_pos++ = '=';
		break;
	default: 
		break;
	}
	
	CString result;
	LPWSTR szBuffer = UTF8ToWideChar(enc);
	result = CString(szBuffer);

	delete []enc;
	delete []szBuffer;

	return result;
}

PBYTE CStrConvert::BASE64_Decode(CString str) {
	if (str.IsEmpty()) return NULL;
	
	LPSTR in = WideCharToMultiByte(str);
	PBYTE out = BASE64_Decode(in);
	
	delete []in;
	
	return out;
}

PBYTE CStrConvert::BASE64_Decode(LPCSTR src) {
	if (!src || 1 > strlen(src)) return NULL;

	INT size = strlen(src);
	PBYTE dec = new BYTE[size+1];
	ZeroMemory(dec, size+1);

	LPCSTR cp;
	INT space_idx = 0, phase;
	INT d, prev_d = 0;
	BYTE c;
    space_idx = 0;
	phase = 0;
	INT DecodeMimeBase64[256] = {
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  // 00-0F
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  // 10-1F
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,  // 20-2F
		52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,  // 30-3F
		-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  // 40-4F
		15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,  // 50-5F
		-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  // 60-6F
		41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,  // 70-7F
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  // 80-8F
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  // 90-9F
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  // A0-AF
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  // B0-BF
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  // C0-CF
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  // D0-DF
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  // E0-EF
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1   // F0-FF
	};
	
    for (cp = src; *cp != '\0'; ++cp) {
		d = DecodeMimeBase64[(INT)*cp];
		if (d != -1) {
			switch (phase) {
			case 0:
				++phase;
				break;
			case 1:
				c = ((prev_d<<2)|((d&0x30)>>4));
				if (space_idx<size) dec[space_idx++] = c;
				++phase;
				break;
			case 2:
				c = (((prev_d&0xf)<<4)|((d&0x3c)>>2));
				if (space_idx<size) dec[space_idx++] = c;
				++phase;
				break;
			case 3:
				c = (((prev_d&0x03)<<6)|d);
				if (space_idx<size) dec[space_idx++] = c;
				phase = 0;
				break;
			}
			prev_d = d;
		}
	}

	PBYTE result = new BYTE[space_idx];
	ZeroMemory(result, space_idx);
	memcpy(result, dec, space_idx-1);

	delete []dec;

    return result;
}
