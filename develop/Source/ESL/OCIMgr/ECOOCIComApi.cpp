
#include "stdafx.h"
#include "ECOOCIComApi.h"
#include "except.h"

#ifdef __LOG_MOD__
#else
#define __LOG_MOD__
#endif

ORA_ACCESS_TYPE g_MainAccess[MAX_SESSION_CNT];
TCHAR g_szWorkID[64] = {'\0',};

VOID CheckClock(TCHAR* pszDesc)
{
	TCHAR szTmp[32];
	static clock_t start = 0;
	clock_t end;
	double duration;
	
	if (start == 0)
	{
		start = clock();
		_stprintf(szTmp, _T("\n%s ==== "), pszDesc);
		WriteLogFile(szTmp);
	}
	else
	{
		end = clock();
		duration = (double)(end - start) / CLOCKS_PER_SEC;
		_stprintf(szTmp, _T("%s %f"), pszDesc, duration);
		WriteLogFile(szTmp);
		start = 0;
	}
}

VOID WriteLogFile(TCHAR* pszString)
{
	FILE* fp;

	fp = _tfopen (OCI_LOG_FILE_NAME, _T("a+b"));
	if (fp == NULL)
	{
		return ;
	}

		fseek(fp, 0, SEEK_END);		
		if( 0 == ftell(fp) )
		{
			/*UNCHANGE*/fputc(0xFF, fp);
			/*UNCHANGE*/fputc(0xFE, fp);
		}

	_ftprintf (fp, _T("\r\n %40s \r\n"), pszString);

	fclose(fp);
}

VOID* ECOMalloc(INT nAllocSize)
{
	TCHAR* pTemp;

	if (nAllocSize <= 0) return NULL;
	
	pTemp = (TCHAR*) malloc(nAllocSize);
	if (pTemp == NULL) return NULL;
	
	wmemset(pTemp, 0x00, nAllocSize);
	
	return pTemp;
}

VOID* ECOCopyMalloc(INT nAllocSize, VOID* pOrgData)
{
	TCHAR* pTemp;

	if (nAllocSize <= 0 ) return NULL;
	if (pOrgData == NULL) return NULL;

	pTemp = (TCHAR*) malloc(nAllocSize);
	if (pTemp == NULL) return NULL;

	wmemcpy(pTemp, (TCHAR*)pOrgData, nAllocSize);

	return pTemp;
}

VOID* ECORealloc(VOID* pData, INT nAllocSize)
{
	TCHAR* pTemp;
	
	if (nAllocSize <= 0)
	{
		ECOFree(pData);
		return NULL;
	}
	
	pTemp = (TCHAR*) realloc(pData, nAllocSize);
	if (pTemp == NULL) return NULL;
	
	return pTemp;
}

VOID ECOFree(VOID* pData)
{
	if (pData != NULL)
	{
		free(pData);
		pData = NULL;
	}
	
	return;
}

INT ECOStrCmpi(TCHAR* pSourceString, TCHAR* pObjectString)
{
	if (pSourceString == NULL && pObjectString == NULL) return 0;
	if (pSourceString == NULL)
	{
		if (StringLength2(pObjectString) == 0) return 0;
		return 1;
	}
	if (pObjectString == NULL)
	{
		if (StringLength2(pSourceString) == 0) return 0;
		return -1;
	}
	
	return _tcsicmp(pSourceString, pObjectString);
}

VOID ECOStrCpy(TCHAR* pSourceString, TCHAR* pObjectString)
{
	if (pSourceString == NULL) return;
	if (pObjectString == NULL)
	{
		pSourceString[0] = _T('\0');
		return;
	}
	
	if (pSourceString == pObjectString) return;
	
	_tcscpy(pSourceString, pObjectString);
	
	return;
}

VOID ECOStrLTrim(TCHAR* pszSourceString, TCHAR* pszObjectString, TCHAR cTrimCharacter)
{
	INT i = 0;
	
	for (i = 0; pszSourceString[i] != _T('\0') && pszSourceString[i] != _T('\n'); i++)
	{
		if(pszSourceString[i] != cTrimCharacter)
		{
			ECOStrCpy(pszObjectString, &pszSourceString[i]);
			return;
		}
	}
	pszObjectString[0] = _T('\0');
}

VOID ECOStrRTrim(TCHAR* pszSourceString, TCHAR* pszObjectString, TCHAR cTrimCharacter)
{
	INT i = 0;
	INT nLen;
	
	ECOStrCpy(pszObjectString, pszSourceString);
	
	nLen = StringLength2(pszSourceString) - 1;
	
	for (i = nLen; i >= 0; i--)
	{
		if(pszSourceString[i] != cTrimCharacter)
		{
			if (nLen == i) return;
			pszObjectString[i + 1] = _T('\0');
			return;
		}
	}
	pszObjectString[0] = _T('\0');
}

VOID ECOStrTrimAll(TCHAR* pszSourceString, TCHAR* pszObjectString, TCHAR cTrimCharacter)
{
	INT i = 0;
	INT j = 0;
	
	for (i = 0; pszSourceString[i] != _T('\0') && pszSourceString[i] != _T('\n'); i++)
	{
		if (pszSourceString[i] == cTrimCharacter);
		else
		{
			pszObjectString[j] = pszSourceString[i];
			j++;
		}
	}
	pszObjectString[j] = _T('\0');
}

VOID FreeDBDataArrayType(DB_DATA_ARRAY_TYPE* pOutData, INT nColumnCount)
{
	if (pOutData == NULL) return;
	
	for (INT i = 0; i < nColumnCount; i++)
	{
		for (INT j = 0; j < pOutData[i].nRowCount; j++)
		{
			switch (pOutData[i].nDataType)
			{
			case BINARY:
				free(pOutData[i].pDBDataUnion[j].BinData.pBinaryData);
				pOutData[i].pDBDataUnion[j].BinData.pBinaryData = NULL;
				pOutData[i].pDBDataUnion[j].BinData.nDataSize = 0;
				break;
			case NUMERIC :
			case DATE :
			case STRING :
			default :
				ECOFree(pOutData[i].pDBDataUnion[j].pStringData);
				break;
				
			}
		}
		ECOFree(pOutData[i].pDBDataUnion);
	}
	ECOFree(pOutData);
}

VOID FreeDBDataUnion(INT nDataType, DB_DATA_UNION* pDBDataUnion)
{
	INT i;
	SET_DATA_TYPE* pSetData;
	
	switch (nDataType)
	{
	case SET_STRING:
		pSetData = (SET_DATA_TYPE*) pDBDataUnion->pSetData;
		if (pSetData != NULL)
		{
			if (pSetData->nRowCount > 0)
			{
				for (i = 0; i < pSetData->nRowCount; i++)
				{
					FreeDBDataUnion(pSetData->nDataType, &pSetData->pDBDataUnion[i]);
				}
				free(pSetData->pDBDataUnion);
				pSetData->pDBDataUnion = NULL;
			}
			free(pDBDataUnion->pSetData);
			pDBDataUnion->pSetData = NULL;
		}
		break;
		
	default:
		if (pDBDataUnion->pStringData != NULL)
		{
			free(pDBDataUnion->pStringData);
			pDBDataUnion->pStringData = NULL;
		}
		
		break;
	}
}

INT Time_LCNV(LONG lgtm, INT* dy, INT * dm, INT* dd, INT* ch, INT* cm, INT* cs)
{
	struct tm *newtime;

	*dy = *dm = *dd = *ch = *cm = *cs = 0;

	newtime = localtime(&lgtm);
	if (newtime == NULL) return(-1);
	*dy = newtime->tm_year;
	*dm = newtime->tm_mon + 1;
	*dd = newtime->tm_mday;
	*ch = newtime->tm_hour;
	*cm = newtime->tm_min;
	*cs = newtime->tm_sec;

	return(1);
}

INT  MESSAGE(
             INT    nSeverity      ,
             TCHAR* szTaskName     ,
             TCHAR* szSourceName   ,
             INT    nSourceLine    ,
             TCHAR* szMessageFormat,
             ...
            )
{
	TCHAR szMessageBuffer[2048];
	va_list va;
	LONG lCurTime;
	INT dy, dm, dd, ch, cm, cs;
	LPTSTR lpszSMailName = _T("\\\\*\\mailslot\\ORACLE_MAIL");
	
	MESSAGE_DATA_TYPE Message;
	
	switch(nSeverity)
	{
	case 4:
		return 0;
		break;
	case 0:
	case 1:
		break;
	case 2:
	case 3:
	default:
		break;
	}
	
	
	time(&lCurTime);
	Time_LCNV(lCurTime, &dy, &dm, &dd, &ch, &cm, &cs);
	if (dy < 1900) dy += 1900;
	
	Message.nSeverity = nSeverity;
	Message.nSourceLine = __LINE__;
	_tcscpy (Message.szSourceName , __WFILE__);
	_stprintf(&Message.szTime[0], _T("[%04d:%02d:%02d:%02d:%02d:%02d]"), dy, dm, dd, ch, cm, cs);
	szMessageBuffer[0] = _T('\0');
	
	va_start (va, szMessageFormat);
	wvsprintf(szMessageBuffer + StringLength2(szMessageBuffer), szMessageFormat, va);
	va_end (va);
	
	szMessageBuffer[255] = _T('\0');
	_tcscpy(Message.szMessage, szMessageBuffer);
	
	if (nSeverity == M$ERROR || nSeverity == M$WARN)
	{
#ifdef __LOG_MOD__
		//LoggingProcess(_T("OCI_MGR"), Message.szMessage);
		LoggingProcess(_T("ESL_DM"), Message.szMessage);
#endif
	}
	
	return 0;
}

INT LoggingProcess(TCHAR* pszFileName, TCHAR* pszLogMessage)
{

#ifdef __LOG_MOD__
	
	CFileFind filefind;
	BOOL bFind = filefind.FindFile(_T("..\\cfg\\_log"));
	BOOL bNoLogFind = filefind.FindFile(_T("..\\cfg\\_nolog"));
	if(FALSE == bFind && FALSE == bNoLogFind)
	{
		CTime t = CTime::GetCurrentTime();
		CString filename, msg;
		filename.Format(_T("..\\tunning\\%s_%04d%02d%02d.tunning"), pszFileName, t.GetYear(), t.GetMonth(), t.GetDay());
		msg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d] %s\n"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), pszLogMessage);
		
		CFileFind filefind;
		UINT nOpenFlags = CFile::modeWrite|CFile::modeCreate|CFile::typeBinary;
		if(TRUE == filefind.FindFile(filename))
		{
			nOpenFlags ^= CFile::modeCreate;
		}
		
		CFile file;
 		BOOL bIsOpen = file.Open(filename, nOpenFlags);
		if(TRUE != bIsOpen) return 0;

		file.SeekToEnd();

		{
		const INT nLen = msg.GetLength();

		TCHAR *pChar = new TCHAR[nLen+1];
		memset(pChar, NULL, nLen);
		pChar[nLen] = '\0';
 		_tcscpy(pChar, (TCHAR*)msg.GetBuffer(0));

		for(INT nCnt=0; nCnt < nLen; nCnt++)
		{
			pChar[nCnt] ^= 0xAA; // ÀÌÁø¼ö 1010 1010
		}

 		file.WriteHuge(pChar, sizeof(TCHAR)*nLen);
		delete []pChar;
		}

		{
			CString strLine = _T("\r\n");	
			const INT LEN = strLine.GetLength();
			TCHAR *szLine = new TCHAR[LEN+1];
			memset(szLine, NULL, LEN);
			szLine[LEN] = '\0';
			_tcscpy(szLine, (TCHAR*)strLine.GetBuffer(0));

			for(INT nCnt=0; nCnt < LEN; nCnt++)
			{
				szLine[nCnt] ^= 0xAA;
			}
			file.Write(szLine, sizeof(TCHAR)*LEN);
			delete []szLine;
		}

 		file.Close();
	}
	else if(TRUE == bFind && FALSE == bNoLogFind)
	{
 		FILE* fp;
 		LONG lCurTime;
 		TCHAR szFileName[514];
 		
 		INT dy, dm, dd, ch, cm, cs;
 		
 		time(&lCurTime);
 		Time_LCNV(lCurTime, &dy, &dm, &dd, &ch, &cm, &cs);
 		if (dy < 1900) dy += 1900;
 		
 		_stprintf(szFileName , _T("..\\tunning\\%s_%04d%02d%02d.txt"), pszFileName, dy , dm, dd);
 	
 		fp = _tfopen(szFileName, _T("a+b"));
 		if (fp == NULL) return 0;
 		
 			fseek(fp, 0, SEEK_END);		
 			if( 0 == ftell(fp) )
 			{
 				/*UNCHANGE*/fputc(0xFF, fp);
 				/*UNCHANGE*/fputc(0xFE, fp);
 			}
 			// ------------------------------------------------------------------------------
 		CString strMsg;
		strMsg.Format(_T("%s"), pszLogMessage);
		strMsg.Replace(_T("\n"), _T("\r\n"));
		strMsg.Replace(_T("\r\r\n"), _T("\r\n"));
		_ftprintf(fp, _T("[%04d-%02d-%02d.%02d:%02d:%02d] %s"), dy, dm, dd, ch, cm, cs, strMsg);
 		fclose(fp);
	}
#endif
	return 0;
}

INT LoggingMailSailot(TCHAR* lpszSMailName, MESSAGE_DATA_TYPE* lpszMessage)
{
	HANDLE hCMail;
	BOOL fBool;
	DWORD dwMailWriteCnt, dwError;
	
	hCMail = CreateFile(lpszSMailName,
		GENERIC_WRITE ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hCMail == INVALID_HANDLE_VALUE)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf, 0, NULL);
		_tprintf((LPTSTR) &lpMsgBuf);
		LocalFree(lpMsgBuf);
		return -1;
	}
	else
	{
		fBool = WriteFile(hCMail,
			(LPVOID)lpszMessage,
			(DWORD)sizeof(MESSAGE_DATA_TYPE),
			&dwMailWriteCnt,
			NULL);
		if (!fBool)
		{
			dwError = GetLastError();
			CloseHandle(hCMail);
			return -2;
		}
	}
	CloseHandle(hCMail);
	
	return 0;
}

INT  MakeInsertSQL(
				   TCHAR*               pszTableName    ,
				   INT                  nColumnCount    ,
				   TCHAR**              pszAttributeName,
				   TCHAR**              pszAttributeData,
				   INT*                 pInDataType     ,
				   CString&             sSQL            ,
				   EDBM_BIND_DATA_TYPE* pBindData
				  )
{
	INT ids;
	INT i;
	
	// Check validation
	if (_tcslen(pszTableName) <= 0 || nColumnCount <= 0) return -1;
	
	// Initialize SQL
	sSQL.Format(_T("INSERT INTO %s("), pszTableName);
	
	// Put AttributeName
	for (i = 0; i < nColumnCount; i++)
	{
		if (i > 0) sSQL += _T(",");
		
		sSQL += pszAttributeName[i];
	}
	
#ifdef __TRACE_DATABASE__
	sSQL += _T(",");
	sSQL += _T("first_work");
#endif
	
	sSQL += _T(") VALUES (");
	
	// Put AttributeData
	for (i = 0; i < nColumnCount; i++)
	{
		if (i > 0) sSQL += _T(",");
		
		if (pszAttributeData[i] == NULL)
		{
			sSQL += _T("NULL");
		}
		else
		{
			ids = MakeSQLValue(sSQL, pszAttributeData[i], pInDataType[i], pBindData);
			if (ids) return -2;
		}
	}
	
#ifdef __TRACE_DATABASE__
	TCHAR szInsertTraceKey[128];
	SYSTEMTIME t;
	GetLocalTime(&t);
	_stprintf(szInsertTraceKey , _T("%s:%04d%02d%02d%02d%02d%02d"), g_szWorkID, t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	sSQL += _T(",");
	sSQL += _T("\'");
	sSQL += szInsertTraceKey;
	sSQL += _T("'");
#endif
	
	sSQL += _T(");");
	
	return 0;
}

INT	 MakeUpdateSQL(
				  TCHAR*               pszTableName      ,
				  INT                  nColumnCount      ,
				  TCHAR**              pszAttributeName  ,
				  TCHAR**              pszAttributeData  ,
				  INT*                 pInDataType       ,
				  TCHAR*               pszPKAttributeName,
				  TCHAR*               pszTablePrimaryKey,
				  CString&             sSQL			     ,
				  EDBM_BIND_DATA_TYPE* pBindData
				 )
{
	INT ids;
	INT i;
	
	// Check validation
	if (_tcslen(pszTableName) <= 0 || nColumnCount <= 0 || _tcslen(pszPKAttributeName) <= 0 || _tcslen(pszTablePrimaryKey) <= 0) return -1;
	
	sSQL.Format(_T("UPDATE %s SET "), pszTableName);
	
	for (i = 0; i < nColumnCount; i++)
	{
		if (i > 0) sSQL += _T(",");
		
		sSQL += pszAttributeName[i];
		sSQL += _T("=");
		
		if (pszAttributeData[i] == NULL)
		{
			sSQL += _T("NULL");
		}
		else
		{
			ids = MakeSQLValue(sSQL, pszAttributeData[i], pInDataType[i], pBindData);
			if (ids) return -2;
		}
	}
	
#ifdef __TRACE_DATABASE__
	
	if ( !(sSQL.Find(_T("LAST_WORK"),0) > 0 || sSQL.Find(_T("last_work"),0) > 0) )
	{
		TCHAR szInsertTraceKey[128];
		SYSTEMTIME t;
		GetLocalTime(&t);
		_stprintf(szInsertTraceKey , _T("%s:%04d%02d%02d%02d%02d%02d"), g_szWorkID, t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
		
		sSQL += _T(",");
		sSQL += _T("last_work");
		sSQL += _T("=");
		sSQL += _T("\'");
		sSQL += szInsertTraceKey;
		sSQL += _T("'");
	}
#endif
	
	sSQL += _T(" WHERE ");
	sSQL += pszPKAttributeName;
	sSQL += _T("=");
	sSQL += pszTablePrimaryKey;
	sSQL += _T(";");
	
	return 0;
}

INT AddSQLStatementToFrame(INT nSessionID, TCHAR* pszSQL)
{
	UINT nFrameStreamLength;
	UINT nSQLStatementLength;
	
	INT nSessionFrameCount = SESSION(nSessionID).nFrameStreamCount;

	if (pszSQL == NULL) return -1;

	//_tcscat(pszSQL, _T("\n"));
	
	if (SESSION(nSessionID).pFrameStream == NULL)
	{
		SESSION(nSessionID).pFrameStream = (TCHAR**) realloc(SESSION(nSessionID).pFrameStream, 
															 sizeof(TCHAR*) * (nSessionFrameCount + 1));
		if (SESSION(nSessionID).pFrameStream == NULL) return -2;
		
		SESSION(nSessionID).pFrameStream[nSessionFrameCount] = NULL;
		
		nSessionFrameCount = SESSION(nSessionID).nFrameStreamCount = 1;	//SESSION(nSessionID).nFrameStreamCount++;
		SESSION(nSessionID).nOldFrameStreamCount = 0;
	}
	
	if (SESSION(nSessionID).pFrameStream[nSessionFrameCount - 1] != NULL) 
	{
		nFrameStreamLength = StringLength2(SESSION(nSessionID).pFrameStream[nSessionFrameCount - 1]);
	} 
	else 
	{
		nFrameStreamLength = 0;
	}
	
#ifdef _UNICODE 
	nSQLStatementLength = wcslen(pszSQL) + sizeof(TCHAR);
	SESSION(nSessionID).pFrameStream[nSessionFrameCount - 1] = (TCHAR *) realloc(SESSION(nSessionID).pFrameStream[nSessionFrameCount - 1], sizeof(TCHAR) * (nFrameStreamLength + nSQLStatementLength + (sizeof(TCHAR)*2)));
#elif _MBCS
	nSQLStatementLength = _tcsclen(pszSQL) + sizeof(TCHAR);
	SESSION(nSessionID).pFrameStream[nSessionFrameCount - 1] = (TCHAR *) realloc(SESSION(nSessionID).pFrameStream[nSessionFrameCount - 1], sizeof(TCHAR) * (nFrameStreamLength + nSQLStatementLength + sizeof(TCHAR)));
#endif

	if (SESSION(nSessionID).pFrameStream[nSessionFrameCount - 1] == NULL) return -4;

#ifdef _UNICODE
	_stprintf(SESSION(nSessionID).pFrameStream[nSessionFrameCount - 1] + nFrameStreamLength, _T("%s%c "), pszSQL, _T('\n'));	
#elif _MBCS
	_stprintf(SESSION(nSessionID).pFrameStream[nSessionFrameCount - 1] + nFrameStreamLength, _T("%s%c"), pszSQL, _T('\n'));
#endif

	SESSION(nSessionID).nOldFrameStreamCount = SESSION(nSessionID).nFrameStreamCount;
		
	if (nFrameStreamLength > 1024 * 32)
	{
		SESSION(nSessionID).pFrameStream = (TCHAR**) realloc(SESSION(nSessionID).pFrameStream, 
															 sizeof(TCHAR*) * (nSessionFrameCount + 1));
		if (SESSION(nSessionID).pFrameStream == NULL) return -3;
		
		SESSION(nSessionID).pFrameStream[nSessionFrameCount] = NULL;
		
		SESSION(nSessionID).nFrameStreamCount++;
		nSessionFrameCount = SESSION(nSessionID).nFrameStreamCount;
	}
	
	return 0;
}

INT	 MakeSQLValue(
				  CString&             sSQL            ,
				  TCHAR*               pszAttributeData,
				  INT                  pInDataType     ,
				  EDBM_BIND_DATA_TYPE* pBindData
				 )
{
	INT ids;
	
	TCHAR *pExpandQuotData = NULL;
	TCHAR szDBDate[11] = { 0, };
	TCHAR szBindingAlias[32];
	
	static UINT nBindingCount = 0;
	
	switch (pInDataType)
	{
	case STRING :
		if (0 == _tcsicmp(pszAttributeData, _T("")))
		{
			sSQL += _T("NULL");
		}
		else
		{
			if ( (sizeof(TCHAR)+1) * StringLength2(pszAttributeData) > 4000)
			{
				_stprintf(szBindingAlias, _T(":ALIAS%d"), nBindingCount++);
				
				ids = AddEDBMBindDataByString(pBindData, STRING, szBindingAlias, pszAttributeData);
				if (ids) return -1;
				
				sSQL += szBindingAlias;
			}
			else
			{
				ids = ExpandQuotation(pszAttributeData, &pExpandQuotData);
				if (ids) return -2;
				
				sSQL += _T("\'");
				sSQL += pExpandQuotData;
				sSQL += _T("\'");
				
				free(pExpandQuotData);
			}
		}
		break;
	case NUMERIC :
		if (0 == _tcsicmp(pszAttributeData, _T("")))
		{
			sSQL += _T("NULL");
		}
		else
		{
			sSQL += pszAttributeData;
		}
		break;
	case DATE :
		if (0 == _tcsicmp(pszAttributeData, _T("")))
		{
			sSQL += _T("NULL");
		}
		else
		{
			sSQL += _T("\'");
			sSQL += pszAttributeData;
			sSQL += _T("\'");
		}
		break;
	case BINARY :
		if (0 == _tcsicmp(pszAttributeData, _T("")))
		{
			sSQL += _T("NULL");
		}
		else
		{
			_stprintf(szBindingAlias, _T(":ALIAS%d"), nBindingCount++);

			ids = AddEDBMBindDataByString(pBindData, BINARY, szBindingAlias, pszAttributeData);
			if (ids) return -1;
			
			sSQL += szBindingAlias;
		}
		break;
	default :
		sSQL += pszAttributeData;
		break;
	}
	
	return 0;
}

INT  ExpandQuotation(
					 TCHAR*  pszInData ,
					 TCHAR** pszOutData
					)
{
	INT i;
	INT nInDataLen;
	INT nOffset = 0;
	INT nExpandCharCnt = 0;
	INT nExpandedDataLen;
	TCHAR* pTmpData;
	
	// Boundary Check
	if (NULL == pszInData || NULL == pszOutData)
	{
		return -1;
	}
	
	// Expand Char Counting
	nInDataLen = _tcslen(pszInData);
	for (i = 0; i < nInDataLen; i++)
	{
		if (pszInData[i] == _T('\''))
		{
			nExpandCharCnt++;
		}
	}
	nExpandedDataLen = nInDataLen + nExpandCharCnt + 1;
	
	// Temp Data Memory Allocation
	pTmpData = (TCHAR*) calloc(nExpandedDataLen, sizeof(TCHAR));
	if (NULL == pTmpData)
	{
		return -2;
	}
	
	// Expand Quotation Mark
	for (i = 0; i < nInDataLen; i++)
	{
		if (_T('\'') == pszInData[i])
		{
			pTmpData[nOffset] = pszInData[i];
			nOffset++;
			pTmpData[nOffset] = pszInData[i];
		}
		else
		{
			pTmpData[nOffset] = pszInData[i];
		}
		nOffset++;
	}
	
	// Set OutData
	*pszOutData = pTmpData;
	
	return 0;
}

INT InitEDBMBindDataType(EDBM_BIND_DATA_TYPE** pBind, INT nCurrentCnt, INT nReallocCnt)
{
	if (nCurrentCnt == nReallocCnt) return 0;

	pBind[0] = (EDBM_BIND_DATA_TYPE*) realloc(pBind[0], sizeof(EDBM_BIND_DATA_TYPE) * nReallocCnt);
	if (pBind[0] == NULL) return -1;

	for (INT i = nCurrentCnt; i < nReallocCnt; i++)
	{
		pBind[0][i].nCount    = 0;
		pBind[0][i].nDataType = NULL;
		pBind[0][i].pAlias    = NULL;
		pBind[0][i].pValue    = NULL;
	}

	return 0;
}

INT AddEDBMBindDataByString(EDBM_BIND_DATA_TYPE* pData, INT nDataType, TCHAR* pAlias, TCHAR* pValue)
{
	if (NULL==pData) return -999;
	
	pData->nDataType = (INT*) realloc(pData->nDataType, sizeof(INT) * (pData->nCount + 1));
	if (pData->nDataType == NULL) return -1;

	pData->nDataType[pData->nCount] = nDataType;

	pData->pAlias = (DB_DATA_UNION*) realloc(pData->pAlias, sizeof(DB_DATA_UNION) * (pData->nCount + 1));
	if (pData->pAlias == NULL) return -2;

	pData->pValue = (DB_DATA_UNION*) realloc(pData->pValue, sizeof(DB_DATA_UNION) * (pData->nCount + 1));
	if (pData->pValue == NULL) return -3;

	pData->pAlias[pData->nCount].pStringData = (TCHAR*) malloc(sizeof(TCHAR) * (_tcslen(pAlias)+sizeof(TCHAR)));
	if (pData->pAlias[pData->nCount].pStringData == NULL) return -4;

	_tcscpy(pData->pAlias[pData->nCount].pStringData, pAlias);

	if (pValue == NULL)
	{
		pData->pValue[pData->nCount].pStringData = (TCHAR*) malloc(sizeof(TCHAR) * DEFAULT_STRING_SIZE);
		if (pData->pValue[pData->nCount].pStringData == NULL) return -5;

		wmemset(pData->pValue[pData->nCount].pStringData, 0x00,  DEFAULT_STRING_SIZE);
	}
	else
	{
		if ( nDataType == BINARY ) 
		{
			INT nSize=0;
			memcpy(&nSize, pValue, 8);
	
			pData->pValue[pData->nCount].BinData.nDataSize = nSize;
			pData->pValue[pData->nCount].BinData.pBinaryData = (TCHAR*) malloc(nSize);
			if (pData->pValue[pData->nCount].BinData.pBinaryData == NULL) return -6;

			wmemcpy(pData->pValue[pData->nCount].BinData.pBinaryData, pValue+sizeof(_TINT), nSize);
		}
		else
		{
			pData->pValue[pData->nCount].pStringData = (TCHAR*) malloc(sizeof(TCHAR) * (_tcslen(pValue) +sizeof(TCHAR)));
			if (pData->pValue[pData->nCount].pStringData == NULL) return -6;

			_tcscpy(pData->pValue[pData->nCount].pStringData, pValue);

		}
	}

	pData->nCount++;

	return 0;
}

VOID FreeEDBMBindData(EDBM_BIND_DATA_TYPE** pData, INT nCount)
{
	INT i, j;

	if (pData[0] != NULL)
	{
		for (i = 0; i < nCount; i++)
		{
			if (pData[0][i].pAlias != NULL)
			{
				for (j = 0; j < pData[0][i].nCount; j++)
				{
					if (pData[0][i].pAlias[j].pStringData != NULL)
					{
						free(pData[0][i].pAlias[j].pStringData);
						pData[0][i].pAlias[j].pStringData = NULL;
					}
				}
				if (NULL!=pData[0]->pAlias) free(pData[0]->pAlias);

				pData[0]->pAlias = NULL;
			}

			if (pData[0][i].pValue != NULL)
			{
				for (j = 0; j < pData[0][i].nCount; j++)
				{
					if ( pData[0][i].nDataType[j] == STRING ) {
						if (pData[0][i].pValue[j].pStringData != NULL)
						{
							free(pData[0][i].pValue[j].pStringData);
							pData[0][i].pValue[j].pStringData = NULL;
						}
					} else if ( pData[0][i].nDataType[j] == BINARY ) {
						if (pData[0][i].pValue[j].BinData.pBinaryData != NULL)
						{
							free(pData[0][i].pValue[j].BinData.pBinaryData);
							pData[0][i].pValue[j].BinData.pBinaryData = NULL;
							pData[0][i].pValue[j].BinData.nDataSize = 0;
						}
					}
				}
				free(pData[0][i].pValue);
				pData[0][i].pValue = NULL;
			}
			if (pData[0][i].nDataType != NULL) {
				free(pData[0][i].nDataType);
				pData[0][i].nDataType = NULL;
			}
		}
		free(pData[0]);
		pData[0] = NULL;
	}
}

VOID FreeTwoPtrVariable(TCHAR*** pTwoPtr, INT nCount)
{
	if (pTwoPtr[0] != NULL)
	{
		for (INT i = 0; i < nCount; i++)
		{
			if (pTwoPtr[0][i] != NULL)
			{
				free(pTwoPtr[0][i]);
				pTwoPtr[0][i] = NULL;
			}
		}

		free(pTwoPtr[0]);
		pTwoPtr[0] = NULL;
	}
}

VOID FreeAllAccessVariable(INT nSessionID)
{
	FreeEDBMBindData	(&SESSION(nSessionID).pFrameBindData ,		SESSION(nSessionID).nFrameStreamCount);
	FreeEDBMBindData	(&SESSION(nSessionID).pFrameNumberData ,	SESSION(nSessionID).nFrameStreamCount);
	FreeEDBMBindData	(&SESSION(nSessionID).pNonFrameBindData ,	1);
	FreeEDBMBindData	(&SESSION(nSessionID).pNonFrameNumberData,	1);
	FreeTwoPtrVariable	(&SESSION(nSessionID).pFrameStream ,		SESSION(nSessionID).nFrameStreamCount);

	SESSION(nSessionID).nFrameStreamCount = 0;
}

INT StringLength(TCHAR * str) 
{
	if (str == NULL) return 0;
#ifdef _UNICODE
	return (wcslen(str));
#elif _MBCS
	return (_tcsclen(str));
#endif
	return (_tcsclen(str));
}

INT StringLength2(TCHAR * str) 
{
	if (str == NULL) return 0;
#ifdef _UNICODE
	return (wcslen(str));
#elif _MBCS
	return (_tcsclen(str));
#endif
	return (_tcsclen(str));
}

INT SemaphoreOpen(
				  TCHAR*			pszSemaphoreName, 
				  INT				nSetValue		, 
				  ECO_SEMA_HANDLE*	pSemaphore 
				  )
{
	*pSemaphore = CreateSemaphore( NULL, nSetValue, nSetValue, pszSemaphoreName );
	if (NULL == *pSemaphore ) 
	{
		if (ERROR_ALREADY_EXISTS == GetLastError()) 
		{
			return SemaphoreExistOpen(pszSemaphoreName, nSetValue, pSemaphore);			
		} 
		else 
		{
			return ERR_SEMAPHORE_CREATE;
		}
	}
	
	return 0;
}

INT SemaphoreExistOpen(
					   TCHAR*			pszSemaphoreName, 
					   INT				nSetValue		, 
					   ECO_SEMA_HANDLE*	pSemaphore 
					   )
{
	*pSemaphore = OpenSemaphore( SEMAPHORE_ALL_ACCESS, 1, pszSemaphoreName );
	if (NULL == *pSemaphore) 
	{ 
		return ERR_SEMAPHORE_EXISTOPEN; 
	}
    return 0;
}

INT SemaphoreLock(
				  ECO_SEMA_HANDLE	hSemaphore, 
				  INT				nTimeOut 
				  )
{
 	DWORD   dwWaitResult;
	if (-1 == nTimeOut) nTimeOut =  INFINITE;
	dwWaitResult = WaitForSingleObject(hSemaphore, nTimeOut);
    if ( WAIT_TIMEOUT == dwWaitResult ) 
		return ERR_SEMAPHORE_NONSIGNALED;
	return (dwWaitResult - WAIT_OBJECT_0);	// = 0
}

INT	SemaphoreOpenLock(
					  TCHAR*			pszSemaphoreName, 
					  INT				nSetValue		, 
					  ECO_SEMA_HANDLE*	pSemaphore		, 
					  INT				nTimeOut
					  )
{
	INT ids;
	ids = SemaphoreOpen(pszSemaphoreName, nSetValue, pSemaphore);
	if (ids) return ids;
	ids = SemaphoreLock(*pSemaphore, nTimeOut);
	if (ids) return ids;
	return 0;
}

INT SemaphoreUnLock(ECO_SEMA_HANDLE hSemaphore)
{
	ReleaseSemaphore( hSemaphore, 1, NULL );
	CloseHandle( hSemaphore ); 
    return 0;
}
