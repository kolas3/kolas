
#include "stdafx.h"
#include "ECOOCI8.h"
#include "except.h"
#include "io.h"
#include "fcntl.h"
BOOL	g_ISLong;

INT OCI8ConnectDB(ORA_ACCESS_TYPE* pAccess, TCHAR* pszUserName, TCHAR* pszPassword)
{
	if (SESSION_USING == pAccess->nStatus) return ERR_SESSION_USING;

	INT ids;

	TCHAR* pAt;
	TCHAR  szServiceName[64];
	TCHAR  szUserName   [64];

	pAt = _tcsstr(pszUserName, _T("@"));
	if (pAt == NULL) return -1;

	_tcsncpy(szUserName, &pszUserName[0], pAt - pszUserName);
	szUserName[pAt - pszUserName] = _T('\0');

	_tcscpy (szServiceName, &pAt[1]);

	if (_tcsicmp(pAccess->szServiceName, szServiceName) != 0 ||
		_tcsicmp(pAccess->szUserName   , szUserName   ) != 0 ||
		_tcsicmp(pAccess->szPassWord   , pszPassword  ) != 0	)
	{
		if (SESSION_OPENED == pAccess->nStatus) OCI8DisconnectDB(pAccess);
	}
	else
	{
		if (SESSION_OPENED == pAccess->nStatus) return 0;
	}

	ids = InitHandles(&pAccess->hpEnv, 
		              &pAccess->hpSvc, 
					  &pAccess->hpErr, 
					  &pAccess->hpSrv, 
					  &pAccess->hpAut, 
					  OCI_DEFAULT);
	if (ids) return -2;

	ids = OCIServerAttach(pAccess->hpSrv,
		                  pAccess->hpErr, 
						  (text*) szServiceName, 
						  (sb4) sizeof(TCHAR) * _tcslen(szServiceName), 
						  (ub4) OCI_DEFAULT);
	if (ids)
	{
		OCIErrorReport(pAccess->hpErr);
		return -3;
	}

	// Set the server handle in the service handle
	ids = OCIAttrSet((dvoid*) pAccess->hpSvc, 
		             (ub4) OCI_HTYPE_SVCCTX , 
					 (dvoid*) pAccess->hpSrv, 
					 (ub4) 0                , 
					 (ub4) OCI_ATTR_SERVER  , 
					 pAccess->hpErr          );
	if (ids)
	{
		OCIErrorReport(pAccess->hpErr);
		return -4;
	}

	// Set attributes in the authentication handle
	ids = OCIAttrSet((dvoid*) pAccess->hpAut                  , 
		             (ub4) OCI_HTYPE_SESSION                  , 
					 (dvoid*) szUserName                      , 
					 (ub4) sizeof(TCHAR) * _tcslen(szUserName), 
					 (ub4) OCI_ATTR_USERNAME                  , 
					 pAccess->hpErr                            );
	if (ids)
	{
		OCIErrorReport(pAccess->hpErr);
		return -5;
	}

	ids = OCIAttrSet((dvoid*) pAccess->hpAut                   , 
		             (ub4) OCI_HTYPE_SESSION                   , 
					 (dvoid*) pszPassword                      , 
					 (ub4) sizeof(TCHAR) * _tcslen(pszPassword), 
					 (ub4) OCI_ATTR_PASSWORD                   , 
					 pAccess->hpErr                             );
	if (ids)
	{
		OCIErrorReport(pAccess->hpErr);
		return -6;
	}

	ids = OCISessionBegin(pAccess->hpSvc, pAccess->hpErr, pAccess->hpAut, OCI_CRED_RDBMS, OCI_DEFAULT);
	if (ids)
	{
		OCIErrorReport(pAccess->hpErr);
		return -7;
	}

	// Set the authentication handle in the Service handle
	ids = OCIAttrSet((dvoid*) pAccess->hpSvc, 
		             (ub4) OCI_HTYPE_SVCCTX , 
					 (dvoid*) pAccess->hpAut, 
					 (ub4) 0                , 
					 (ub4) OCI_ATTR_SESSION , 
					 pAccess->hpErr          );
	if (ids)
	{
		OCIErrorReport(pAccess->hpErr);
		return -8;
	}


	MESSAGE(M$INFORM, MSG_ARGMENT, _T("Oracle Connection As %s."), pszUserName);

	return 0;
}

INT OCI8DisconnectDB(ORA_ACCESS_TYPE* pAccess)
{
	INT ids;

	if (SESSION_USING == pAccess->nStatus) return ERR_SESSION_USING;

	if (SESSION_OPENED == pAccess->nStatus)
	{
		ids = OCISessionEnd(pAccess->hpSvc, pAccess->hpErr, pAccess->hpAut, (ub4) 0);
		if (ids)
		{
			OCIErrorReport(pAccess->hpErr);
			return -1;
		}

		ids = OCIServerDetach(pAccess->hpSrv, pAccess->hpErr, (ub4) OCI_DEFAULT);
		if (ids)
		{
			OCIErrorReport(pAccess->hpErr);
			return -2;
		}

		if (pAccess->hpSrv) (VOID) OCIHandleFree((dvoid*) pAccess->hpSrv, (ub4) OCI_HTYPE_SERVER );
		if (pAccess->hpSvc) (VOID) OCIHandleFree((dvoid*) pAccess->hpSvc, (ub4) OCI_HTYPE_SVCCTX );
		if (pAccess->hpErr) (VOID) OCIHandleFree((dvoid*) pAccess->hpErr, (ub4) OCI_HTYPE_ERROR  );
		if (pAccess->hpAut) (VOID) OCIHandleFree((dvoid*) pAccess->hpAut, (ub4) OCI_HTYPE_SESSION);
		if (pAccess->hpEnv) (VOID) OCIHandleFree((dvoid*) pAccess->hpEnv, (ub4) OCI_HTYPE_ENV    );
	}

	return 0;
}

INT  OCI8SelectProc(
				    ORA_ACCESS_TYPE*      pAccess     ,
				    TCHAR*                pszUserID   ,
                    TCHAR*                pszPasswd   ,
                    TCHAR*                pszSQL      ,
					INT                   nStartRow   ,
					INT                   nMaxFetchCnt,
                    EDBM_DATA_ARRAY_TYPE* pOutData	  , 
					EDBM_BIND_DATA_TYPE *pBind
                   )
{
	INT ids;
	INT nColumnCount;

	ORA_ARRAY_FIELD_TYPE* pField = NULL;

	DB_DATA_ARRAY_TYPE* pData  = NULL;
	OCIStmt*            hpStmt = NULL;

	ids = OraSQLInit(pAccess->hpEnv, pAccess->hpErr, &hpStmt, pszSQL);
	if (ids) return -2;

	if ( pBind )
	{
		ids = OraSQLBindByName(hpStmt, pAccess->hpErr, pBind);
		if (ids)
		{
			OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
			return ids;
		}
	}

	ids = OraSQLExec(pAccess->hpSvc, pAccess->hpErr, hpStmt, TRUE);
	if (ids)
	{
		OCIHandleFree(hpStmt, OCI_HTYPE_STMT);

		return ids;
	}

	ids = OraColArrayInfo(pAccess->hpEnv, pAccess->hpErr, hpStmt, &nColumnCount, &pField, &pData);
	if (ids)
	{
		OCIErrorReport(pAccess->hpErr);
		OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
		return ids;
	}
	
	ids = OraSQLArrayDefine(hpStmt, pAccess->hpErr, nColumnCount, pField );
	if (ids)
	{
		FreeOraArrayFieldType (pField, nColumnCount);
		FreeDBDataArrayType(pData , nColumnCount);
		OCIErrorReport(pAccess->hpErr);
		OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
		return ids;
	}
	
	pOutData->nColumnCount = nColumnCount;
	pOutData->nRowCount = 0;

	ids = OraSQLArrayFetch(pAccess->hpSvc, pAccess->hpErr, hpStmt, nStartRow, nMaxFetchCnt, nColumnCount, pField, &pOutData->nRowCount, pData);
	if (ids)
	{
		FreeOraArrayFieldType (pField, nColumnCount);
		FreeDBDataArrayType(pData , nColumnCount);
		OCIErrorReport(pAccess->hpErr);
		OCIHandleFree(hpStmt, OCI_HTYPE_STMT);

		return ids;
	}

	pOutData->pColumnData = pData;

	FreeOraArrayFieldType(pField, nColumnCount);
	OCIHandleFree(hpStmt, OCI_HTYPE_STMT);

	return 0;
}

INT  OCI8SelectExProc(
	       			  ORA_ACCESS_TYPE*      pAccess  ,
				      TCHAR*                pszUserID,
                      TCHAR*                pszPasswd,
                      TCHAR*                pszSQL   ,
					  ORA_SELECT_STRU*      pSelect  ,
                      EDBM_DATA_ARRAY_TYPE* pOutData ,
					  EDBM_BIND_DATA_TYPE *pBind
                     )
{
	INT ids;

	pSelect->hpStmt = NULL;
	pSelect->pField = NULL;
	pSelect->nColumnCount = 0;
	
	ids = OraSQLInit(pAccess->hpEnv, pAccess->hpErr, &pSelect->hpStmt, pszSQL);
	if (ids)
	{
		return -2;
	}

	if ( pBind )
	{
		ids = OraSQLBindByName(pSelect->hpStmt, pAccess->hpErr, pBind);
		if (ids)
		{
			OCIHandleFree(pSelect->hpStmt, OCI_HTYPE_STMT);
			return ids;
		}
	}

	ids = OraSQLExec(pAccess->hpSvc, pAccess->hpErr, pSelect->hpStmt, TRUE);
	if (ids)
	{
		OCIHandleFree(pSelect->hpStmt, OCI_HTYPE_STMT);
		
		return ids;
	}

	ids = OraColInfo(pAccess->hpEnv, pAccess->hpErr, pSelect->hpStmt, &pSelect->nColumnCount, &pSelect->pField, &pOutData->pColumnData);
	if (ids)
	{
		OCIHandleFree(pSelect->hpStmt, OCI_HTYPE_STMT);
		return -4;
	}

	ids = OraSQLDefine(pSelect->hpStmt, pAccess->hpErr, pSelect->nColumnCount, pSelect->pField);
	if (ids)
	{
		FreeOraFieldType (pSelect->pField , pSelect->nColumnCount);
		FreeDBDataArrayType(pOutData->pColumnData, pSelect->nColumnCount);
		OCIHandleFree(pSelect->hpStmt, OCI_HTYPE_STMT);
		return -5;
	}

	pOutData->nColumnCount = pSelect->nColumnCount;
	pOutData->nRowCount = 0;

	return 0;
}

INT  OCI8SelectFetchProc(
				         ORA_ACCESS_TYPE* pAccess      ,
					     ORA_SELECT_STRU* pSelect      ,
						 INT nStartRow                 ,
					     INT nMaxFetchCnt              ,
                         EDBM_DATA_ARRAY_TYPE* pOutData
                        )
{
	INT ids;

	// Initialize EDBM_DATA_ARRAY_TYPE Variable
	for (INT i = 0; i < pOutData->nColumnCount; i++)
	{
		for (INT j = 0; j <	pOutData->pColumnData[i].nRowCount; j++)
		{
			switch (pOutData->pColumnData[i].nDataType)
			{
			case BINARY:
				free(pOutData->pColumnData[i].pDBDataUnion[j].BinData.pBinaryData);
				pOutData->pColumnData[i].pDBDataUnion[j].BinData.pBinaryData = NULL;
				pOutData->pColumnData[i].pDBDataUnion[j].BinData.nDataSize = 0;
				break;
			case NUMERIC :
			case DATE :
			case STRING :
			default :
				free(pOutData->pColumnData[i].pDBDataUnion[j].pStringData);
				pOutData->pColumnData[i].pDBDataUnion[j].pStringData = NULL;
				break;

			}
		}
		free(pOutData->pColumnData[i].pDBDataUnion);
		pOutData->pColumnData[i].pDBDataUnion = NULL;
	}

	pOutData->nColumnCount = pSelect->nColumnCount;
	pOutData->nRowCount    = 0;

	ids = OraSQLFetch(pAccess->hpSvc       , 
		              pAccess->hpErr       , 
					  pSelect->hpStmt      , 
					  nStartRow            , 
					  nMaxFetchCnt         , 
					  pSelect->nColumnCount, 
					  pSelect->pField      , 
					  &pOutData->nRowCount , 
					  pOutData->pColumnData);
	if (ids)
	{
		FreeOraFieldType (pSelect->pField , pSelect->nColumnCount );
		FreeDBDataArrayType(pOutData->pColumnData, pOutData->nColumnCount);
		OCIHandleFree(pSelect->hpStmt, OCI_HTYPE_STMT);
		if (OCI_NO_DATA == ids) return OCI_NO_DATA;
		return -1;
	}

	if (pOutData->nRowCount == 0)
	{
		FreeOraFieldType(pSelect->pField, pSelect->nColumnCount);
		pSelect->pField = NULL;
		OCIHandleFree(pSelect->hpStmt, OCI_HTYPE_STMT);
	}

	return 0;
}

VOID OCI8StmtHandleFree(OCIStmt* hpStmt)
{
	OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
}

INT OCI8UnSelectProc(
				     ORA_ACCESS_TYPE* pAccess    ,
                     TCHAR*           pszUserID  ,
                     TCHAR*           pszPasswd  ,
                     TCHAR*           pszSQL     ,
					 INT              nFrameIndex, 
					 BOOL			  bBeginFlag
                    )
{
	INT ids;

	TCHAR*   pSQL   = NULL;
	OCIStmt* hpStmt = NULL;	

	pSQL = (TCHAR*) malloc(sizeof(TCHAR) * (StringLength2(pszSQL) + 32));
	if (NULL == pSQL) return -2;
	
	if ( bBeginFlag )
	{
		CString sBuffer = pszSQL;
		sBuffer.Remove(';');
		_stprintf(pSQL, _T("%s"), sBuffer.GetBuffer(0));
	}
	else
		_stprintf(pSQL, _T("BEGIN\n%sEND;"), pszSQL);

	ids = OraSQLInit(pAccess->hpEnv, pAccess->hpErr, &hpStmt, pSQL);
	
	if (ids)
	{
		free(pSQL);
		return -3;
	}

	if (nFrameIndex == -1)
	{
		
		ids =OraSQLBindByPos(hpStmt, pAccess->hpErr, &pAccess->pNonFrameBindData[0]);
		if (ids)
		{
			free(pSQL);
			OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
			return -4;
		}
		

		ids = OraSQLBindByName(hpStmt, pAccess->hpErr, &pAccess->pNonFrameNumberData[0]);
		if (ids)
		{
			free(pSQL);
			OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
			return -5;
		}
	}
	else
	{

		ids = OraSQLBindByName(hpStmt, pAccess->hpErr, &pAccess->pFrameNumberData[nFrameIndex]);
		if (ids)
		{
			free(pSQL);
			OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
			return -7;
		}

		ids =OraSQLBindByPos(hpStmt, pAccess->hpErr, &pAccess->pFrameBindData[nFrameIndex]);
		if (ids)
		{
			free(pSQL);
			OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
			return -6;
		}
	}


	CTime t_start = CTime::GetCurrentTime();
	DWORD START, FINISH;
	START = GetTickCount(); 
	
	ids = OraSQLExec(pAccess->hpSvc, pAccess->hpErr, hpStmt, FALSE);
	
	FINISH = GetTickCount();
	CTime t_end = CTime::GetCurrentTime();
	
	{
		BOOL bFlag;
		
		bFlag = TRUE;
			
		if ( bFlag )
		{
			CFileFind filefind;
			BOOL bFind = filefind.FindFile(_T("..\\cfg\\_log"));

			BOOL bNoLogFind = filefind.FindFile(_T("..\\cfg\\_nolog"));

			if(FALSE == bFind && FALSE == bNoLogFind)
			{
				CTime t = CTime::GetCurrentTime();
				CString filename, msg;
				filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d.tunning"), t.GetYear(), t.GetMonth(), t.GetDay());
				
				CString sLogTime;
				sLogTime.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][%d]++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n"), 
					t_start.GetYear(), t_start.GetMonth(),  t_start.GetDay(), t_start.GetHour(), t_start.GetMinute(), t_start.GetSecond(), 
					nFrameIndex+1);
				CString s_pSQL;
				s_pSQL.Format(_T("%s\r\n"), pSQL);
				s_pSQL.Replace(_T("\n"), _T("\r\n"));
				s_pSQL.Replace(_T("\r\r\n"), _T("\r\n"));
				CString s_Msg;
				s_Msg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][%d][OraSQLExec=%d.%03ds]-------------------------------------------------\r\n"),
					t_end.GetYear(), t_end.GetMonth(),  t_end.GetDay(), t_end.GetHour(), t_end.GetMinute(), t_end.GetSecond(), 
					nFrameIndex+1, (FINISH-START)/1000, (FINISH-START)%1000);

				msg = sLogTime + s_pSQL + s_Msg;

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
						pChar[nCnt] ^= 0xAA; // 이진수 1010 1010
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
						szLine[nCnt] ^= 0xAA; // 이진수 1010 1010
					}
					file.Write(szLine, sizeof(TCHAR)*LEN);
					delete []szLine;
				}
				
				file.Close();
			}
			else if(TRUE == bFind && FALSE == bNoLogFind)
			{
 				CString filename;
 				filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d.txt"), t_start.GetYear(), t_start.GetMonth(), t_start.GetDay());
 				FILE* fp;
 				
 				fp = _tfopen (filename, _T("a+b"));
 				
 				if (NULL!=fp)
 				{
 					CString sLogTime;
 					sLogTime.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][%d]"), 
 						t_start.GetYear(), t_start.GetMonth(),  t_start.GetDay(), t_start.GetHour(), t_start.GetMinute(), t_start.GetSecond(), 
 						nFrameIndex+1);
 					 			
 					fseek(fp, 0, SEEK_END);		
 					if( 0 == ftell(fp) )
 					{
 						/*UNCHANGE*/fputc(0xFF, fp);
 						/*UNCHANGE*/fputc(0xFE, fp);
 					}
 				
					_ftprintf(fp, _T("%s++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n"), sLogTime);

					CString strMsg;
					strMsg.Format(_T("%s"), pSQL);
					strMsg.Replace(_T("\n"), _T("\r\n"));
					strMsg.Replace(_T("\r\r\n"), _T("\r\n"));
					_ftprintf(fp, _T("%s\r\n"), strMsg);
 					_ftprintf(fp, _T("[%04d-%02d-%02d.%02d:%02d:%02d][%d][OraSQLExec=%d.%03ds]-------------------------------------------------\r\n"), 
 						t_end.GetYear(), t_end.GetMonth(),  t_end.GetDay(), t_end.GetHour(), t_end.GetMinute(), t_end.GetSecond(), 
 						nFrameIndex+1, (FINISH-START)/1000, (FINISH-START)%1000);
 					
 					fclose(fp);
 					
 				}
			}
			if (ids)
			{
				free(pSQL);
				OCIHandleFree(hpStmt, OCI_HTYPE_STMT);

				return ids;
			}
		}
	}
	
	OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
	free(pSQL);
	
	return 0;
}

INT OCI8UnSelectProcWithNoBegin
					(
				     ORA_ACCESS_TYPE* pAccess    ,
                     TCHAR*           pszUserID  ,
                     TCHAR*           pszPasswd  ,
                     TCHAR*           pszSQL     ,
					 INT              nFrameIndex
                    )
{
	INT ids;

	TCHAR*   pSQL   = NULL;
	OCIStmt* hpStmt = NULL;

	pSQL = (TCHAR*) malloc(sizeof(TCHAR) * (StringLength2(pszSQL) + 32));
	if (NULL == pSQL) return -2;

	_stprintf(pSQL, _T("%s "), pszSQL);

	ids = OraSQLInit(pAccess->hpEnv, pAccess->hpErr, &hpStmt, pSQL);
	if (ids)
	{
		free(pSQL);
		return -3;
	}

	if (nFrameIndex == -1)
	{
		
		ids =OraSQLBindByPos(hpStmt, pAccess->hpErr, &pAccess->pNonFrameBindData[0]);
		if (ids)
		{
			free(pSQL);
			OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
			return -4;
		}
		

		ids = OraSQLBindByName(hpStmt, pAccess->hpErr, &pAccess->pNonFrameNumberData[0]);
		if (ids)
		{
			free(pSQL);
			OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
			return -5;
		}
	}
	else
	{
		ids = OraSQLBindByName(hpStmt, pAccess->hpErr, &pAccess->pFrameNumberData[nFrameIndex]);
		if (ids)
		{
			free(pSQL);
			OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
			return -7;
		}

		ids =OraSQLBindByPos(hpStmt, pAccess->hpErr, &pAccess->pFrameBindData[nFrameIndex]);
		if (ids)
		{
			free(pSQL);
			OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
			return -6;
		}
	}


CTime t_start = CTime::GetCurrentTime();
DWORD START, FINISH;
START = GetTickCount(); 

	ids = OraSQLExec(pAccess->hpSvc, pAccess->hpErr, hpStmt, FALSE);

	FINISH = GetTickCount();
	CTime t_end = CTime::GetCurrentTime();
	
	{
		BOOL bFlag;
		
		bFlag = TRUE;
		
		if ( _tcsstr(pSQL, _T("workno")) != NULL || _tcsstr(pSQL, _T("WORKNO")) != NULL ) bFlag = FALSE;
		if ( _tcsstr(pSQL, _T("encrypt")) != NULL || _tcsstr(pSQL, _T("ENCRYPT")) != NULL ) bFlag = FALSE;
		if ( _tcsstr(pSQL, _T("decrypt")) != NULL || _tcsstr(pSQL, _T("DECRYPT")) != NULL ) bFlag = FALSE;
		if ( _tcsstr(pSQL, _T("lock table")) != NULL || _tcsstr(pSQL, _T("LOCK TABLE")) != NULL ) bFlag = FALSE;
		
		if ( bFlag )
		{
			CFileFind filefind;
			BOOL bFind = filefind.FindFile(_T("..\\cfg\\_log"));
			BOOL bNoLogFind = filefind.FindFile(_T("..\\cfg\\_nolog"));

			if(FALSE == bFind && FALSE == bNoLogFind)
			{
				CTime t = CTime::GetCurrentTime();
				CString filename, msg;
				filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d.tunning"), t_start.GetYear(), t_start.GetMonth(), t_start.GetDay());
				
				CString sLogTime;
				sLogTime.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][%d]++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n"), 
					t_start.GetYear(), t_start.GetMonth(),  t_start.GetDay(), t_start.GetHour(), t_start.GetMinute(), t_start.GetSecond(), 
					nFrameIndex+1);
				CString s_pSQL;
				s_pSQL.Format(_T("%s\r\n"), pSQL);
				s_pSQL.Replace(_T("\n"), _T("\r\n"));
				s_pSQL.Replace(_T("\r\r\n"), _T("\r\n"));
				CString s_Msg;
				s_Msg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][%d][OraSQLExec=%d.%03ds]-------------------------------------------------\r\n"),
						t_end.GetYear(), t_end.GetMonth(),  t_end.GetDay(), t_end.GetHour(), t_end.GetMinute(), t_end.GetSecond(), 
						nFrameIndex+1, (FINISH-START)/1000, (FINISH-START)%1000);msg = sLogTime + s_pSQL + s_Msg;

				msg = sLogTime + s_pSQL + s_Msg;

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
						pChar[nCnt] ^= 0xAA;
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
 				CString filename;
 				filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d.txt"), t_start.GetYear(), t_start.GetMonth(), t_start.GetDay());
 				FILE* fp;
 				
 				fp = _tfopen (filename, _T("a+b"));
 				 				
 				if (NULL!=fp)
 				{
 					CString sLogTime;
 					sLogTime.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][%d]"), 
 						t_start.GetYear(), t_start.GetMonth(),  t_start.GetDay(), t_start.GetHour(), t_start.GetMinute(), t_start.GetSecond(), 
 						nFrameIndex+1);
 					
 					fseek(fp, 0, SEEK_END);		
 					if( 0 == ftell(fp) )
 					{
 						/*UNCHANGE*/fputc(0xFF, fp);
 						/*UNCHANGE*/fputc(0xFE, fp);
 					}

 					_ftprintf(fp, _T("%s++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n"), sLogTime);

					CString strMsg;
					strMsg.Format(_T("%s"), pSQL);
					strMsg.Replace(_T("\n"), _T("\r\n"));
					strMsg.Replace(_T("\r\r\n"), _T("\r\n"));

 					_ftprintf(fp, _T("[%04d-%02d-%02d.%02d:%02d:%02d][%d][OraSQLExec=%d.%03ds]-------------------------------------------------\r\n"), 
 						t_end.GetYear(), t_end.GetMonth(),  t_end.GetDay(), t_end.GetHour(), t_end.GetMinute(), t_end.GetSecond(), 
 						nFrameIndex+1, (FINISH-START)/1000, (FINISH-START)%1000);
 					
 					fclose(fp);
 					 					
 				}				
			}

			if (ids)
			{
				free(pSQL);
				OCIHandleFree(hpStmt, OCI_HTYPE_STMT);

				return ids;
			}
		}
	}
		
	OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
	free(pSQL);
	
	return 0;
}

INT	 OCI8InsertProc(
				    ORA_ACCESS_TYPE* pAccess         ,
				    TCHAR*           pszUserID       ,
                    TCHAR*           pszPasswd       ,
				    TCHAR*           pszTableName    ,
				    INT              nColumnCount    ,
				    TCHAR**          pszAttributeName,
				    TCHAR**          pszAttributeData,
				    INT*             pInDataType     ,
				    BOOL             bCommit
			       )
{
	INT ids;

	CString sSQLStatement;

	ids = MakeInsertSQL(pszTableName, nColumnCount, pszAttributeName, pszAttributeData, pInDataType, sSQLStatement, &pAccess->pNonFrameBindData[0]);
	if (ids)
	{
		return -1;
	}

	ids = OCI8UnSelectProc(pAccess, pszUserID, pszPasswd, sSQLStatement.GetBuffer(0), -1);
	if (ids)
	{
		MESSAGE(M$WARN, MSG_ARGMENT, _T("<< DB_Access_Proc >> UnSelectProc Error. [%d]"), ids);

		return ids;
	}

	if (bCommit == TRUE)
	{
		ids = OCI8CommitProc(pAccess, pszUserID, pszPasswd);
		if (ids)
		{
			return -3;
		}
	}

	return 0;
}

INT	 OCI8UpdateProc(
				    ORA_ACCESS_TYPE* pAccess           ,
				    TCHAR*           pszUserID         ,
                    TCHAR*           pszPasswd         ,
				    TCHAR*           pszTableName      ,
				    INT              nColumnCount      ,
				    TCHAR**          pszAttributeName  ,
				    TCHAR**          pszAttributeData  ,
				    INT*             pInDataType       ,
				    TCHAR*           pszPKAttributeName,
				    TCHAR*           pszTablePrimaryKey,
				    BOOL             bCommit           ,
					BOOL			 bBeginFlag
			       )
{
	INT ids;

	CString sSQLStatement;

	ids = MakeUpdateSQL(pszTableName, nColumnCount, pszAttributeName, pszAttributeData, pInDataType, pszPKAttributeName, pszTablePrimaryKey, sSQLStatement, &pAccess->pNonFrameBindData[0]);
	if (ids)
	{
		return -1;
	}

	ids = OCI8UnSelectProc(pAccess, pszUserID, pszPasswd, sSQLStatement.GetBuffer(0), -1, bBeginFlag);
	if (ids)
	{
		MESSAGE(M$WARN, MSG_ARGMENT, _T("<< DB_Access_Proc >> UnSelectProc Error. [%d]"), ids);
		
		return ids;
	}
	if (bCommit == TRUE)
	{
		ids = OCI8CommitProc(pAccess, pszUserID, pszPasswd);
		if (ids)
		{
			return -3;
		}
	}

	return 0;
}

INT	 OCI8DeleteProc(
				    ORA_ACCESS_TYPE* pAccess           ,
				    TCHAR*           pszUserID         ,
                    TCHAR*           pszPasswd         ,
				    TCHAR*           pszTableName      ,
				    TCHAR*           pszPKAttributeName,
				    TCHAR*           pszTablePrimaryKey,
				    BOOL             bCommit
			       )
{
	INT ids;

	CString sSQLStatement;

	if (StringLength2(pszTableName)			<= 0 || 
		StringLength2(pszPKAttributeName)	<= 0 || 
		StringLength2(pszTablePrimaryKey)	<= 0	)
	{
		return -1;
	}

	sSQLStatement.Format(_T("DELETE FROM %s WHERE %s = %s;"), pszTableName, pszPKAttributeName, pszTablePrimaryKey);

	ids = OCI8UnSelectProc(pAccess, pszUserID, pszPasswd, sSQLStatement.GetBuffer(0), -1);
	if (ids)
	{
		MESSAGE(M$WARN, MSG_ARGMENT, _T("<< DB_Access_Proc >> UnSelectProc Error. [%d]"), ids);
		
		return ids;
	}

	if (bCommit == TRUE)
	{
		ids = OCI8CommitProc(pAccess, pszUserID, pszPasswd);
		if (ids)
		{
			return -3;
		}
	}

	return 0;
}

INT	 OCI8ExecSQLProc(
				     ORA_ACCESS_TYPE* pAccess  ,
				     TCHAR*           pszUserID,
                     TCHAR*           pszPasswd,
				     TCHAR*           pszSQL   ,
				     BOOL             bCommit
				    )
{
	INT ids;

	if (StringLength2(pszSQL) <= 0)
	{
		return -1;
	}

	ids = OCI8UnSelectProc (pAccess, pszUserID, pszPasswd, pszSQL, -1);	
	if (ids)
	{
		MESSAGE(M$WARN, MSG_ARGMENT, _T("<< DB_Access_Proc >> UnSelectProc Error. [%d]"), ids);
		
		return ids;
	}

	if (bCommit == TRUE)
	{
		ids = OCI8CommitProc(pAccess, pszUserID, pszPasswd);
		if (ids)
		{
			return -3;
		}
	}

	return 0;
}

INT	 OCI8CommitProc(
				    ORA_ACCESS_TYPE* pAccess  ,
				    TCHAR*           pszUserID,
                    TCHAR*           pszPasswd
                   )
{
	INT ids;

	ids = OCITransCommit(pAccess->hpSvc, pAccess->hpErr, (ub4) 0);
	if (ids)
	{
		OCIErrorReport(pAccess->hpErr);
		return -1;
	}

	return 0;
}

INT	 OCI8RollbackProc(
				      ORA_ACCESS_TYPE* pAccess  ,
				      TCHAR*           pszUserID,
                      TCHAR*           pszPasswd
                     )
{
	INT ids;

	ids = OCITransRollback(pAccess->hpSvc, pAccess->hpErr, (ub4) 0);
	if (ids)
	{
		OCIErrorReport(pAccess->hpErr);
		return -1;
	}

	return 0;
}

INT	 OCI8NextNumberProc(
					    ORA_ACCESS_TYPE* pAccess       ,
					    TCHAR*           pszUserID     ,
					    TCHAR*           pszPasswd     ,
				 	    TCHAR*           pszSQL        ,
					    TCHAR*           pszNumberAlias,
					    TCHAR*           pszNumber
				       )
{
	INT ids;

	TCHAR* pSQL = NULL;
	TCHAR  szTmpBuf[DEFAULT_STRING_SIZE];

	OCIStmt* hpStmt = NULL;
	OCIBind* hpBind;

	if (StringLength2(pszSQL) <= 0 || StringLength2(pszNumberAlias) <= 0)
	{
		return -2;
	}

	pSQL = (TCHAR*) malloc(sizeof(TCHAR) * (StringLength2(pszSQL) + 36));
	if (pSQL == NULL)
	{
		return -3;
	}

	_stprintf(pSQL, _T("begin %s end;"), pszSQL);
	
	ids = OraSQLInit(pAccess->hpEnv, pAccess->hpErr, &hpStmt, pSQL);
	if (ids)
	{
		free(pSQL);
		return -4;
	}

	pszNumber[0] = _T('\0');
	szTmpBuf [0] = _T('\0');

	ids = OCIBindByName(
		                hpStmt ,
		                &hpBind ,
						pAccess->hpErr ,
						(const text*) pszNumberAlias ,
                        (sb4) sizeof(TCHAR) * _tcslen(pszNumberAlias),
						(dvoid*) szTmpBuf ,
						sizeof(TCHAR) * DEFAULT_STRING_SIZE,
						SQLT_STR ,
						(dvoid*) 0 ,
                        (ub2*) 0 ,
						(ub2*) 0 ,
						(ub4) 0 ,
						(ub4*) 0 ,
						OCI_DEFAULT
					   );
	if (ids)
	{
		free(pSQL);
		OCIErrorReport(pAccess->hpErr);
		OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
		return -5;
	}

	ids = OraSQLExec(pAccess->hpSvc, pAccess->hpErr, hpStmt, FALSE);
	if (ids)
	{
		free(pSQL);
		OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
		return ids;
	}

	_tcscpy(pszNumber, szTmpBuf);

	free(pSQL);
	OCIHandleFree(hpStmt, OCI_HTYPE_STMT);

	return 0;
}

INT  OCI8GetOneFieldValueProc(
						      ORA_ACCESS_TYPE* pAccess       ,
						      TCHAR*           pszUserID     ,
						      TCHAR*           pszPasswd     ,
						      TCHAR*           pszSQL        ,
						      TCHAR*           pszReturnValue,
							  EDBM_BIND_DATA_TYPE *pBind
                             )
{
	INT ids;

	EDBM_DATA_ARRAY_TYPE OutData;
	
	ids = OCI8SelectProc(pAccess, pszUserID, pszPasswd, pszSQL, 1, 1, &OutData, pBind );

	if (OCI_NO_DATA == ids) ids = 0;

	if (ids)
	{
		return ids;
	}

	if (OutData.nRowCount > 0)
	{
		_tcscpy(pszReturnValue, OutData.pColumnData[0].pDBDataUnion->pStringData);
		ECOStrRTrim(pszReturnValue, pszReturnValue, _T(' '));
		FreeDBDataArrayType(OutData.pColumnData, OutData.nColumnCount);
	}
	else
	{
		pszReturnValue[0] = _T('\0');
	}

	return 0;
}

VOID OCIErrorReport(OCIError* hpErr)
{
	utext msg[512];
	sb4   errcode = 0;

	OCIErrorGet((dvoid*) hpErr       ,
		        (ub4) 1              ,
				(text*) NULL         ,
				&errcode             ,
				(text*) msg          ,
				(ub4) sizeof(msg)    ,
				(ub4) OCI_HTYPE_ERROR);

	MESSAGE(M$WARN, MSG_ARGMENT, _T("\n"));
	MESSAGE(M$WARN, MSG_ARGMENT, _T("◀◀◀ ORACLE ERROR ▶▶▶\n"));
	MESSAGE(M$WARN, MSG_ARGMENT, _T("%s"), msg);
}

INT  InitHandles(OCIEnv** hpEnv, 
				 OCISvcCtx** hpSvc, 
				 OCIError** hpErr, 
				 OCIServer** hpSrv, 
				 OCISession** hpAut, 
				 ub4 InitMode)
{
#ifdef _UNICODE
	if (OCIEnvCreate((OCIEnv**) hpEnv, OCI_UTF16, (dvoid*) 0, (dvoid* (*) (dvoid*, size_t)) 0, (dvoid* (*) (dvoid*, dvoid*, size_t)) 0, (dvoid (*) (dvoid*, dvoid*)) 0, (size_t) 0, (VOID**) 0))
	{
		return -1;
	}
#elif _MBCS
	if (OCIInitialize(InitMode,
					 (dvoid*) 0,
					 (dvoid* (*)(dvoid*, size_t)) 0,
					 (dvoid* (*)(dvoid*, dvoid*, size_t)) 0,
					 (VOID (*)(dvoid*, dvoid*)) 0))
	{
		return -1;
	}

	if (OCIEnvInit((OCIEnv**) hpEnv, (ub4) OCI_DEFAULT, (size_t) 0, (dvoid**) 0))
	{
		return -2;
	}
#endif

	if (OCIHandleAlloc((dvoid*) *hpEnv, (dvoid**) hpSvc, (ub4) OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid**) 0 ))
	{
		return -3;
	}

	if (OCIHandleAlloc((dvoid*) *hpEnv, (dvoid**) hpErr, (ub4) OCI_HTYPE_ERROR, (size_t) 0, (dvoid**) 0 ))
	{
		return -4;
	}

	if (OCIHandleAlloc((dvoid*) *hpEnv, (dvoid**) hpSrv, (ub4) OCI_HTYPE_SERVER, (size_t) 0, (dvoid**) 0 ))
	{
		return -5;
	}

	if (OCIHandleAlloc((dvoid*) *hpEnv, (dvoid**) hpAut, (ub4) OCI_HTYPE_SESSION, (size_t) 0, (dvoid**) 0 ))
	{
		return -6;
	}

	return 0;
}

INT  OraSQLInit(OCIEnv* hpEnv, OCIError* hpErr, OCIStmt** hpStmt, TCHAR* pszSQL)
{
	INT ids;

	if (NULL == *hpStmt)
	{
		ids = OCIHandleAlloc((dvoid*) hpEnv, (dvoid**) hpStmt, OCI_HTYPE_STMT, 0, NULL);
		if (ids) return -1;
	}
	ids = OCIStmtPrepare(*hpStmt, 
						 hpErr, 
						 (const text*) pszSQL, 
						 sizeof(TCHAR) * _tcslen(pszSQL), 
						 OCI_NTV_SYNTAX, 
						 OCI_DEFAULT);
	if (ids)
	{
		OCIErrorReport(hpErr);
		OCIHandleFree(*hpStmt, OCI_HTYPE_STMT);
		return -2;
	}

	return 0;
}

INT OraSQLBindByPos(OCIStmt* hpStmt, OCIError* hpErr, EDBM_BIND_DATA_TYPE* pBindData)
{
	INT i;

	OCIBind* hpBind;

	if (pBindData == NULL) return 0;

	if (pBindData->nCount > 0 && 
		(NULL == pBindData->pValue || NULL == pBindData->nDataType))
	{
		return -1;
	}

	for (i = 0; i < pBindData->nCount; i++)
	{
		hpBind = NULL;
		switch (pBindData->nDataType[i])
		{
		case BINARY:
			if (NULL == pBindData->pValue[i].BinData.pBinaryData) return -2;

			if (OCIBindByPos(hpStmt, 
				             &hpBind,
							 hpErr,
				             i + 1, 
							 (dvoid*) pBindData->pValue[i].BinData.pBinaryData,							
							 (sb4) pBindData->pValue[i].BinData.nDataSize, 
							 SQLT_LBI,
				             (dvoid*) 0, 
							 (ub2*) 0, 
							 (ub2*) 0,
				             (ub4) 0, 
							 (ub4*) 0, 
							 (ub4) OCI_DEFAULT))
			{
				OCIErrorReport(hpErr);
				return -3;
			}
			break;
		case STRING:
		default:
			if (NULL == pBindData->pValue[i].pStringData ) return -4;

			CFileFind filefind;
			BOOL bFind = filefind.FindFile(_T("..\\cfg\\_log"));

			BOOL bNoLogFind = filefind.FindFile(_T("..\\cfg\\_nolog"));

			if(FALSE == bFind && FALSE == bNoLogFind)
			{
				CTime t = CTime::GetCurrentTime();
				CString filename, msg;
				filename.Format(_T("..\\tunning\\ESL_DM__%04d%02d%02d.tunning"), t.GetYear(), t.GetMonth(), t.GetDay());
				
				CString sLogTime;
				sLogTime.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][][=BIND]"), 
						t.GetYear(), t.GetMonth(),  t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
				CString s_Msg;
				s_Msg.Format(_T("%s [Pos=%d] ■■■%s■■■\r\n"), sLogTime, i + 1, pBindData->pValue[i].pStringData);

				msg = s_Msg;

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
						pChar[nCnt] ^= 0xAA;
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
 				CTime t_start = CTime::GetCurrentTime();
 				CString filename;
 				filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d.txt"), t_start.GetYear(), t_start.GetMonth(), t_start.GetDay());
 				FILE* fp;
 				fp = _tfopen (filename, _T("a+b"));
 				if (NULL!=fp)
 				{
 					fseek(fp, 0, SEEK_END);		
 					if( 0 == ftell(fp) )
 					{
 						/*UNCHANGE*/fputc(0xFF, fp);
 						/*UNCHANGE*/fputc(0xFE, fp);
 					}
 
 					CString sLogTime;
 					sLogTime.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][][=BIND]"), 
 									t_start.GetYear(), t_start.GetMonth(),  t_start.GetDay(), t_start.GetHour(), t_start.GetMinute(), t_start.GetSecond());
 					_ftprintf (fp, _T("%s [Pos=%d] ■■■%s■■■\r\n"), sLogTime, i + 1, pBindData->pValue[i].pStringData);
 					fclose(fp);
 				}
			}
			INT nLen = ( sizeof(TCHAR)+1 )*_tcsclen(pBindData->pValue[i].pStringData);
			if (OCIBindByPos(hpStmt, 
				             &hpBind, 
							 hpErr,
				             i + 1, 
							 (dvoid*) pBindData->pValue[i].pStringData,
				             nLen > 0 ? nLen+1 : sizeof(TCHAR) * DEFAULT_STRING_SIZE,								
							 SQLT_STR,
				             (dvoid*) 0, 
							 (ub2*) 0, 
							 (ub2*) 0,
				             (ub4) 0, 
							 (ub4*) 0, 
							 (ub4) OCI_DEFAULT))
			{
				OCIErrorReport(hpErr);
				return -5;
			}
			break;
		}
	}

	return 0;
}

INT  OraSQLBindByName(OCIStmt* hpStmt, OCIError* hpErr, EDBM_BIND_DATA_TYPE* pBindData)
{
	INT i;
	INT ids;
	INT	nLen;
	
	OCIBind* hpBind;
	
	if (pBindData == NULL) return 0;
	
	if (pBindData->nCount > 0 && (NULL == pBindData->pValue || NULL == pBindData->nDataType))
	{
		return -1;
	}
	
	for (i = 0; i < pBindData->nCount; i++)
	{
		hpBind = NULL;
		switch (pBindData->nDataType[i])
		{
		case BINARY:
			if (NULL == pBindData->pValue[i].BinData.pBinaryData) return -2;

			ids = OCIBindByName(hpStmt,
				                &hpBind,
								hpErr,
								(const text*) pBindData->pAlias[i].BinData.pBinaryData,
								-1,
								(dvoid*) pBindData->pValue[i].BinData.pBinaryData,
								sizeof(TCHAR) * pBindData->pValue[i].BinData.nDataSize,
								SQLT_LBI,
								(dvoid*) 0,
								(ub2*) 0,
								(ub2*) 0,
								(ub4) 0,
								(ub4*) 0,
								OCI_DEFAULT);
			
			if (ids)
			{
				OCIErrorReport(hpErr);
				return -2;
			}
			break;
		case STRING:
		default:
			if (NULL == pBindData->pValue[i].pStringData ) return -3;

			CFileFind filefind;
			BOOL bFind = filefind.FindFile(_T("..\\cfg\\_log"));

			BOOL bNoLogFind = filefind.FindFile(_T("..\\cfg\\_nolog"));
			if(FALSE == bFind && FALSE == bNoLogFind)
			{
				CTime t = CTime::GetCurrentTime();
				CString filename, msg;
				filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d.tunning"), t.GetYear(), t.GetMonth(), t.GetDay());

				
				CString sLogTime;
				sLogTime.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][][=BIND]"), 
					t.GetYear(), t.GetMonth(),  t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
				
				CString s_Msg;
				s_Msg.Format(_T("%s [Name=%s] ■■■%s■■■\r\n"), sLogTime, pBindData->pAlias[i].pStringData, pBindData->pValue[i].pStringData);

				msg = s_Msg;

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
						pChar[nCnt] ^= 0xAA;
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
 				CTime t_start = CTime::GetCurrentTime();
 				CString filename;
 				filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d.txt"), t_start.GetYear(), t_start.GetMonth(), t_start.GetDay());
 				FILE* fp;
 				fp = _tfopen (filename, _T("a+b"));
 				if (NULL!=fp)
 				{
 
 					fseek(fp, 0, SEEK_END);		
 					if( 0 == ftell(fp) )
 					{
 						/*UNCHANGE*/fputc(0xFF, fp);
 						/*UNCHANGE*/fputc(0xFE, fp);
 					}
 					
 					CString sLogTime;
 					sLogTime.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][][=BIND]"), 
 						t_start.GetYear(), t_start.GetMonth(),  t_start.GetDay(), t_start.GetHour(), t_start.GetMinute(), t_start.GetSecond());
 					_ftprintf (fp, _T("%s [Name=%s] ■■■%s■■■\r\n"), sLogTime, pBindData->pAlias[i].pStringData, pBindData->pValue[i].pStringData);
 					fclose(fp);
 				}
			}
			nLen = ( sizeof(TCHAR)+1)*_tcsclen(pBindData->pValue[i].pStringData);
			ids = OCIBindByName(hpStmt,
								&hpBind,
								hpErr,
								(const text*) pBindData->pAlias[i].pStringData,
								(sb4) sizeof(TCHAR) * _tcslen(pBindData->pAlias[i].pStringData),
								(dvoid*) pBindData->pValue[i].pStringData,								
								nLen > 0 ? nLen+1 : sizeof(TCHAR) * DEFAULT_STRING_SIZE,								
								SQLT_STR,
								(dvoid*) 0,
								(ub2*) 0,
								(ub2*) 0,
								(ub4) 0,
								(ub4*) 0,
								OCI_DEFAULT);
			
			if (ids)
			{
				OCIErrorReport(hpErr);
				return -4;
			}
			break;
		}
	}

	return 0;
}

INT  OraSQLDefine(OCIStmt* hpStmt, OCIError* hpErr, INT nColumn, ORA_FIELD_TYPE* pField)
{
	INT i;
	INT ids;
	OCIDefine* pDefn = NULL; 
	
	for (i = 0; i < nColumn; i++)
	{
		pDefn = NULL;
		switch (pField[i].nType)
		{
		case SQLT_CLOB:
		case SQLT_BLOB:
			ids = OCIDefineByPos(hpStmt, 
				                 &pDefn, 
								 hpErr, 
								 (ub4) (i + 1), 
								 (dvoid*) (&pField[i].pLobLoc),
				                 (sb4) sizeof(TCHAR) * (pField[i].nSize), 
								 (ub2) (pField[i].nType),
				                 (dvoid*) (&pField[i].nInd), 
								 (ub2*)(&pField[i].nLength),
				                 NULL, 
								 OCI_DEFAULT);
			break;
		default:
			ids = OCIDefineByPos(hpStmt, 
				                 &pDefn, 
								 hpErr, 
								 (ub4) (i + 1), 
								 (dvoid*) (pField[i].pBuffer),
				                 (sb4) sizeof(TCHAR) * (pField[i].nSize), 
								 (ub2) (pField[i].nType),
				                 (dvoid*) (&pField[i].nInd), 
								 (ub2*) (&pField[i].nLength),
				                 NULL, 
								 OCI_DEFAULT);
			break;
		}
		if(0 > ids) 
		{
			OCIErrorReport(hpErr);
			return -1;
		}
	}

	return 0;
}

INT  OraSQLExec(OCISvcCtx* hpSvc, OCIError* hpErr, OCIStmt* hpStmt, BOOL bSelect)
{
	INT ids;

	if (bSelect)
	{
		ids = OCIStmtExecute(hpSvc, hpStmt, hpErr, 0, 0, NULL, NULL, OCI_DEFAULT);
	} 
	else 
	{
		ids = OCIStmtExecute(hpSvc, hpStmt, hpErr, 1, 0, NULL, NULL, OCI_DEFAULT);
	}

	if (OCI_SUCCESS != ids)
	{
		OCIErrorReport(hpErr);
		return -9001;
	}

	return 0;
}

INT  OraColInfo(OCIEnv* hpEnv, OCIError* hpErr, OCIStmt* hpStmt, INT* pColumn, ORA_FIELD_TYPE** pDBField, DB_DATA_ARRAY_TYPE** pDBData)
{
	INT ids;
	INT nColumn = 0;

	OCIParam*           pFieldAttr = NULL;
	DB_DATA_ARRAY_TYPE* pData      = NULL;
	ORA_FIELD_TYPE*     pField     = NULL;
	INT nNameLen = 512;
	TCHAR *pName = NULL;

	while (TRUE)
	{
		ids = OCIParamGet(hpStmt, OCI_HTYPE_STMT, hpErr, (dvoid**) &pFieldAttr, nColumn + 1);
		if (ids)
		{
			break;
		}
		
		nColumn++;
		pData = (DB_DATA_ARRAY_TYPE*) realloc(pData, nColumn * sizeof(DB_DATA_ARRAY_TYPE));
		if ( NULL != pData )
		{
			memset(&pData[nColumn - 1], 0x00, sizeof(DB_DATA_ARRAY_TYPE));
		}
		else
		{
			return -1;
		}

		pField = (ORA_FIELD_TYPE*) realloc(pField, nColumn * sizeof(ORA_FIELD_TYPE));
		if (NULL != pField)
		{
			memset(&pField[nColumn - 1], 0x00, sizeof(ORA_FIELD_TYPE));
		}
		else
		{
			FreeDBDataArrayType(pData, nColumn);
			return -2;
		}
		
		ids = OCIAttrGet((dvoid*) pFieldAttr, 
						 OCI_DTYPE_PARAM, 
						 (dvoid*) &(pField[nColumn - 1].nType), 
						 0, 
						 OCI_ATTR_DATA_TYPE, 
						 hpErr);
		if (ids)
		{
			FreeOraFieldType (pField, nColumn);
			FreeDBDataArrayType(pData , nColumn);
			OCIErrorReport(hpErr);
			return -3;
		}

		ids = OCIAttrGet((dvoid*) pFieldAttr, 
						 OCI_DTYPE_PARAM, 
						 (dvoid*) &(pField[nColumn - 1].nSize), 
						 0, 
						 OCI_ATTR_DATA_SIZE, 
						 hpErr);
		if (ids)
		{
			FreeOraFieldType (pField, nColumn);
			FreeDBDataArrayType(pData , nColumn);
			OCIErrorReport(hpErr);
			return -4;
		}
		pField[nColumn - 1].nSize %= 0xFFFF;
		
		switch (pField[nColumn-1].nType)
		{
		case SQLT_INT:
		case SQLT_FLT:
		case SQLT_UIN:
			pData [nColumn - 1].nDataType = NUMERIC;
			pField[nColumn - 1].nType = SQLT_STR;
			pField[nColumn - 1].nSize += 1;
			break;
		case SQLT_DAT:
			pData [nColumn - 1].nDataType = DATE;
			pField[nColumn - 1].nType = SQLT_STR;
			pField[nColumn - 1].nSize = DATE_TYPE_LEN + 1;
			break;
		case SQLT_LNG:
			pData [nColumn - 1].nDataType = STRING;
			pField[nColumn - 1].nSize = LONG_TYPE_LEN + 1;
			break;
		case SQLT_CLOB:
			pData [nColumn - 1].nDataType = STRING;
			pField[nColumn - 1].nSize = 0;
			OCIDescriptorAlloc(hpEnv, &pField[nColumn - 1].pLobLoc, OCI_DTYPE_LOB, 0, NULL);
			break;
		case SQLT_BLOB:
			pData [nColumn - 1].nDataType = BINARY;
			pField[nColumn - 1].nSize = 0;
			OCIDescriptorAlloc(hpEnv, &pField[nColumn - 1].pLobLoc, OCI_DTYPE_LOB, 0, NULL);
			break;
		case SQLT_FILE:
			FreeOraFieldType (pField, nColumn);
			FreeDBDataArrayType(pData , nColumn);
			return -5;
			break;
		default:
			pData [nColumn - 1].nDataType = STRING;
			pField[nColumn - 1].nType = SQLT_STR;
			pField[nColumn - 1].nSize += 1;
			break;
		}
	
		if (0 < pField[nColumn-1].nSize)
		{
			pField[nColumn-1].pBuffer = (TCHAR*) malloc(sizeof(TCHAR) * pField[nColumn - 1].nSize);
			if (NULL == pField[nColumn-1].pBuffer)
			{
				FreeOraFieldType (pField, nColumn);
				FreeDBDataArrayType(pData , nColumn);
				return -6;
			}
		}
	}

	*pColumn  = nColumn;
	*pDBData  = pData;
	*pDBField = pField;

	return 0;
}

INT  OraSQLFetch(
				 OCISvcCtx*          hpSvc        ,
				 OCIError*           hpErr        ,
				 OCIStmt*            hpStmt       ,
				 INT                 nFetchStartNo,
				 INT                 nFetchCount  ,
				 INT                 nColumn      ,
				 ORA_FIELD_TYPE*     pField       ,
				 INT*                pRow         ,
				 DB_DATA_ARRAY_TYPE* pData
                )
{
	INT  i;
	INT  nRow = 0;
	INT	 ids;
	INT	 nSum;
	
	TCHAR* pLobData=NULL;
	BOOL bAllFetched = FALSE;

	if ( 1 > nFetchStartNo) nFetchStartNo = 1;
	if (-1 > nFetchCount  ) nFetchCount   = -1;

	while (TRUE)
	{
		for (i = 0; i < nColumn; i++)
		{
			wmemset(pField[i].pBuffer, 0x00, pField[i].nSize);
		}

		ids = OCIStmtFetch(hpStmt, hpErr, 1, OCI_FETCH_NEXT, OCI_DEFAULT);
		if (ids == OCI_NO_DATA) 
		{
			bAllFetched = TRUE;
			break;
		}
		if (0 > ids)
		{
			OCIErrorReport(hpErr);
			return -1;
		}
        // Write data to buffer
		nRow++;
		if (nFetchStartNo > nRow) continue;
		for (i = 0; i < nColumn; i++)
		{
			pData[i].pDBDataUnion = (DB_DATA_UNION*) realloc(pData[i].pDBDataUnion, nRow * sizeof(DB_DATA_UNION));
			if(NULL == pData[i].pDBDataUnion) return -2;
			
			memset(&pData[i].pDBDataUnion[nRow - 1], 0x00, sizeof(DB_DATA_UNION));
			pData[i].nRowCount = nRow;
			
			switch (pField[i].nType)
			{
				ub4 amt, offset;

			case SQLT_CLOB:
                // Lob Open nLnd
				if(pField[i].nInd == 0)
				{
					if(0 > OCILobOpen(hpSvc, hpErr, (OCILobLocator*) pField[i].pLobLoc, OCI_LOB_READONLY))
					{
						OCIErrorReport(hpErr);
						return -3;
					}
					// Lob Length
					if(0 > OCILobGetLength(hpSvc, hpErr, (OCILobLocator*) pField[i].pLobLoc, &pData[i].nDataLength))
					{
						OCIErrorReport(hpErr);
						return -4;
					}

					pData[i].nDataLength++;
					// Memory Allocation
					pLobData = (TCHAR*) malloc(sizeof(TCHAR) * pData[i].nDataLength );
					if(NULL == pLobData) return -5;
					// Lob Read
					amt = 0; offset = 1; nSum = 0;
					while(TRUE)
					{
						ub2 clsid = 0;

#ifdef _UNICODE
                        clsid = OCI_UCS2ID;
#endif
						ids = OCILobRead(hpSvc,
							             hpErr,
							             (OCILobLocator*) pField[i].pLobLoc,
							             &amt,
							             offset,
							             &pLobData[0] + nSum ,
							             sizeof(TCHAR) * (pData[i].nDataLength - 1) ,
							             (dvoid*) 0,
							             (sb4 (*)(dvoid*, const dvoid*, ub4, ub1)) 0,
							             (ub2) clsid,
							             (ub1) SQLCS_IMPLICIT);
						if (ids == 0) break;
						else if (ids == 99) nSum += amt;
						else return -6;
					}
					pLobData[pData[i].nDataLength - 1] = _T('\0');
				}
				else
				{
					pLobData = (TCHAR*) malloc(sizeof(TCHAR) * 1);
					pLobData[0] = _T('\0');
				}
				
				pData[i].pDBDataUnion[nRow - 1].pStringData = pLobData;
				OCILobClose(hpSvc, hpErr, (OCILobLocator*) pField[i].pLobLoc);
				break;
			
			case SQLT_BLOB:
				
				if(pField[i].nInd == 0)
				{
					if(0 > OCILobOpen(hpSvc, hpErr, (OCILobLocator*) pField[i].pLobLoc, OCI_LOB_READONLY))
					{
						OCIErrorReport(hpErr);
						return -3;
					}
					// Lob Length
					if(0 > OCILobGetLength(hpSvc, hpErr, (OCILobLocator*) pField[i].pLobLoc, &pData[i].nDataLength))
					{
						OCIErrorReport(hpErr);
						return -4;
					}
					// Memory Allocation
					pLobData = (TCHAR*) malloc(pData[i].nDataLength);
					if(NULL == pLobData) return -5;
					// Lob Read
					amt = 0; offset = 1; nSum = 0;
					while(TRUE)
					{
						ids = OCILobRead(hpSvc,
							             hpErr,
							             (OCILobLocator*) pField[i].pLobLoc,
							             &amt,
							             offset,
							             &pLobData[0] + nSum ,
							             pData[i].nDataLength ,
							             (dvoid*) 0,
							             (sb4 (*)(dvoid*, const dvoid*, ub4, ub1)) 0,
							             (ub2) 0,
							             (ub1) SQLCS_IMPLICIT);
						if (ids == 0) break;
						else if (ids == 99) nSum += amt;
						else return -6;
					}
				}
				else
				{
					pLobData = (TCHAR*) malloc(sizeof(TCHAR) * 1);
					pLobData[0] = _T('\0');
				}
				
				pData[i].pDBDataUnion[nRow - 1].BinData.nDataSize = pData[i].nDataLength;
				pData[i].pDBDataUnion[nRow - 1].BinData.pBinaryData = pLobData;
				OCILobClose(hpSvc, hpErr, (OCILobLocator*) pField[i].pLobLoc);
				
				break;
			
			default:
                // Get Output Buffer Size
				pData[i].nDataLength = _tcslen(pField[i].pBuffer) + 1;
                // Memory Allocation
				pData[i].pDBDataUnion[nRow - 1].pStringData = (TCHAR*) malloc(sizeof(TCHAR) * pData[i].nDataLength);
				if( NULL == pData[i].pDBDataUnion[nRow - 1].pStringData ) return -7;
                // Write Data to Output Buffer
//				wmemcpy(pData[i].pDBDataUnion[nRow - 1].pStringData, pField[i].pBuffer, sizeof(TCHAR) * pData[i].nDataLength);
				wmemcpy(pData[i].pDBDataUnion[nRow - 1].pStringData, pField[i].pBuffer, pData[i].nDataLength);
				ECOStrRTrim(pData[i].pDBDataUnion[nRow - 1].pStringData, pData[i].pDBDataUnion[nRow - 1].pStringData, _T(' '));
				ECOStrRTrim(pData[i].pDBDataUnion[nRow - 1].pStringData, pData[i].pDBDataUnion[nRow - 1].pStringData, _T('\t'));
				break;
				
			} //end of switch(nType)
		} //end of for(i:nColumn)
		if (-1 != nFetchCount && nFetchCount <= nRow) break;
	} //end of while

	*pRow = nRow;

	if (bAllFetched) return OCI_NO_DATA;
	
	return 0;
}

VOID FreeOraFieldType(ORA_FIELD_TYPE *pOraFieldType, INT nColumnCount)
{
	INT i;
	
	if (NULL == pOraFieldType) return;
	
	for (i = 0; i < nColumnCount; i++)
	{
		if (pOraFieldType[i].pBuffer != NULL)
		{
			free(pOraFieldType[i].pBuffer);
			pOraFieldType[i].pBuffer = NULL;
		}
		
		if (NULL != pOraFieldType[i].pLobLoc)
			OCIDescriptorFree(pOraFieldType[i].pLobLoc, OCI_DTYPE_LOB);
	}
	free(pOraFieldType);
	pOraFieldType = NULL;
}

VOID InitOraAccessType(ORA_ACCESS_TYPE pAccess[])
{
	for (INT i=0 ; i<MAX_SESSION_CNT ; i++)
	{
		pAccess[i].hpEnv				= NULL;
		pAccess[i].hpSvc				= NULL;
		pAccess[i].hpAut				= NULL;
		pAccess[i].hpErr				= NULL;
		pAccess[i].szServiceName[0]		= _T('\0');
		pAccess[i].szUserName   [0]		= _T('\0');
		pAccess[i].szPassWord   [0]		= _T('\0');
		pAccess[i].nMaxFetchCount		= DEFAULT_MAX_FETCH_CNT;
		pAccess[i].nOldFrameStreamCount = 0;
		pAccess[i].nFrameStreamCount	= 0;
		pAccess[i].pFrameStream			= NULL;
		pAccess[i].pNonFrameBindData	= NULL;
		pAccess[i].pFrameBindData		= NULL;
		pAccess[i].pNonFrameNumberData	= NULL;
		pAccess[i].pFrameNumberData		= NULL;
		pAccess[i].nRequestID			= -1;
		pAccess[i].nStatus				= SESSION_CLOSED;
		pAccess[i].nArrayFetchSize		= DEFAULT_ARRAY_FETCH_CNT;
		
	}
}

INT	 OCI8UpdateLobProc(
					   ORA_ACCESS_TYPE* pAccess           ,
					   TCHAR*           pszUserID         ,
					   TCHAR*           pszPasswd         ,
					   TCHAR*           pszTableName      ,
					   TCHAR*           pszAttributeName  ,
					   VOID*            pszLobData        ,
					   INT              nLobLen           ,
					   TCHAR*           pszPKAttributeName,
					   TCHAR*           pszTablePrimaryKey,
					   BOOL bCommit
					   )
{
	if ( _tcslen(pszTableName) == 0 ) return -1;
	if ( _tcslen(pszAttributeName) == 0 ) return -1;
	if ( pszLobData == NULL ) return -1;
	if ( nLobLen < 0 ) return -1;
	if ( _tcslen(pszPKAttributeName) == 0 ) return -1;
	if ( _tcslen(pszTablePrimaryKey) == 0 ) return -1;

    INT ids;

	TCHAR pszSQL[1024];
	_stprintf(pszSQL, _T("UPDATE %s SET %s = :ALIAS0 WHERE %s = %s"), 
			  pszTableName, pszAttributeName, pszPKAttributeName, pszTablePrimaryKey);


	OCIStmt* hpStmt = NULL;
	ids = OCIHandleAlloc(pAccess->hpEnv, (dvoid**) &hpStmt, OCI_HTYPE_STMT, 0, NULL);
	if (ids) return -2;

	ids = OCIStmtPrepare(hpStmt, pAccess->hpErr, (const text*) pszSQL, sizeof(TCHAR) * _tcslen(pszSQL), OCI_NTV_SYNTAX, OCI_DEFAULT);
	if (ids)
	{
		OCIErrorReport(pAccess->hpErr);
		OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
		return -3;
	}

	OCIBind* hpBind = NULL;
	if (OCIBindByPos(hpStmt, 
					 &hpBind,
					 pAccess->hpErr,
					 1, 
					 (dvoid*) pszLobData,
					 (sb4) nLobLen, 
					 SQLT_LBI,
					 (dvoid*) 0, 
					 (ub2*) 0, 
					 (ub2*) 0,
					 (ub4) 0, 
					 (ub4*) 0, 
					 (ub4) OCI_DEFAULT)) 
	{
		OCIErrorReport(pAccess->hpErr);
		OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
		return -4;
	}

	ids = OCIStmtExecute(pAccess->hpSvc, hpStmt, pAccess->hpErr, 1, 0, NULL, NULL, OCI_DEFAULT);

	if (OCI_SUCCESS != ids)
	{
		OCIErrorReport(pAccess->hpErr);
		OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
		return -9001;
	}

	// Commit
	if (bCommit == TRUE)
	{
		ids = OCI8CommitProc(pAccess, pszUserID, pszPasswd);
		if (ids)
		{
			return -6;
		}
	}
	
	OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
    return 0;
}

INT	 OCI8UnSelectProcWithoutBeginEnd(
				    ORA_ACCESS_TYPE* pAccess           ,
				    TCHAR*           pszUserID         ,
                    TCHAR*           pszPasswd         ,
					TCHAR*           pszSQL            ,
                    BOOL bCommit
			       )
{
	if ( _tcslen(pszSQL) == 0 ) return -1;

    INT ids;

	ids = OCI8ConnectDB(pAccess, pszUserID, pszPasswd);
	if (ids) return -1;
	
	OCIStmt* hpStmt = NULL;
	ids = OCIHandleAlloc(pAccess->hpEnv, (dvoid**) &hpStmt, OCI_HTYPE_STMT, 0, NULL);
	if (ids) return -2;

	ids = OCIStmtPrepare(hpStmt, pAccess->hpErr, (const text*) pszSQL, sizeof(TCHAR) * _tcslen(pszSQL), OCI_NTV_SYNTAX, OCI_DEFAULT);
	if (ids)
	{
		OCIErrorReport(pAccess->hpErr);
		OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
		return -3;
	}

    // execute
	ids = OCIStmtExecute(pAccess->hpSvc, hpStmt, pAccess->hpErr, 1, 0, NULL, NULL, OCI_DEFAULT);
	
	if (OCI_SUCCESS != ids)
	{
		OCIErrorReport(pAccess->hpErr);
		OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
	
		return -9001;
	}

	// Commit
	if (bCommit == TRUE)
	{
		ids = OCI8CommitProc(pAccess, pszUserID, pszPasswd);
		if (ids)
		{
			return -6;
		}
	}
	
	OCIHandleFree(hpStmt, OCI_HTYPE_STMT);
    return 0;
}

INT  OraSQLFetchEx(
				   OCISvcCtx*          hpSvc        ,
				   OCIError*           hpErr        ,
				   OCIStmt*            hpStmt       ,
				   INT                 nFetchCount  ,
				   INT                 nColumn      ,
				   ORA_FIELD_TYPE*     pField       
				   )
{
	INT  i;
	INT  nRow = 0;
	INT	 ids;
	

    // Fetch one record data
	for (i = 0; i < nColumn; i++)
	{
//		wmemset(pField[i].pBuffer, 0x00, sizeof(TCHAR) * pField[i].nSize);
				wmemset(pField[i].pBuffer, 0x00, pField[i].nSize);
	}

	ids = OCIStmtFetch(hpStmt, hpErr, 1, OCI_FETCH_NEXT, OCI_DEFAULT);
	
	if (OCI_NO_DATA == ids) return OCI_NO_DATA;
	
	if (0 > ids)
	{
		OCIErrorReport(hpErr);
		return -1;
	}
	
	return 0;
}

INT OraColArrayInfo(OCIEnv* hpEnv, OCIError* hpErr, OCIStmt* hpStmt, INT* pColumn, ORA_ARRAY_FIELD_TYPE** pDBField, DB_DATA_ARRAY_TYPE** pDBData)
{
	INT		ids;	
	INT		nColumn = 0;
	INT		nLobLocSize = 0;	
	USHORT	nType;
	
	OCIParam* pFieldAttr = NULL;
	DB_DATA_ARRAY_TYPE* pData = NULL;
	ORA_ARRAY_FIELD_TYPE* pField = NULL;

	g_ISLong = FALSE; 
	while (TRUE)
	{
		// Get pointer to column
		ids = OCIParamGet(hpStmt, OCI_HTYPE_STMT, hpErr, (dvoid**) &pFieldAttr, nColumn + 1);
		if (ids)
		{
			break;
		}
		
		// Increment column count and Allocate an db data array structure
		nColumn++;
		// Get data type & size
		ids = OCIAttrGet((dvoid*) pFieldAttr, OCI_DTYPE_PARAM, (dvoid*) &nType, 0, OCI_ATTR_DATA_TYPE, hpErr);
		if (ids)
		{
			OCIErrorReport(hpErr);
			return -12;
		}

		 switch (nType)
		 {
		 case SQLT_INT:
		 case SQLT_FLT:
		 case SQLT_UIN:
		 case SQLT_DAT:
			 break;
		 case SQLT_LNG:
		 case SQLT_CLOB:
		 case SQLT_BLOB:
			g_ISLong = TRUE;
			break;
		 }
		
		 if ( g_ISLong )
			 break;
	}

	nColumn = 0;
	pFieldAttr = NULL;



	while (TRUE)
	{
		 // Get pointer to column
		 ids = OCIParamGet(hpStmt, OCI_HTYPE_STMT, hpErr, (dvoid**) &pFieldAttr, nColumn + 1);
		 if (ids)
		 {
			 break;
		 }
		 
		 // Increment column count and Allocate an db data array structure
		 nColumn++;
		 pData = (DB_DATA_ARRAY_TYPE*) realloc(pData, nColumn * sizeof(DB_DATA_ARRAY_TYPE));
		 if ( NULL != pData )
		 {
			 memset(&pData[nColumn - 1], 0x00, sizeof(DB_DATA_ARRAY_TYPE));
		 }
		 else
		 {
			 return -1;
		 }

		 pField = (ORA_ARRAY_FIELD_TYPE*) realloc(pField, nColumn * sizeof(ORA_ARRAY_FIELD_TYPE));
		 if (NULL != pField)
		 {
			 memset(&pField[nColumn - 1], 0x00, sizeof(ORA_ARRAY_FIELD_TYPE));
		 }
		 else
		 {
			 FreeDBDataArrayType(pData, nColumn);
			 return -2;
		 }
		 
		 // Get data type & size
		 ids = OCIAttrGet((dvoid*) pFieldAttr, OCI_DTYPE_PARAM, (dvoid*) &(pField[nColumn - 1].nType), 0, OCI_ATTR_DATA_TYPE, hpErr);
		 if (ids)
		 {
			 FreeOraArrayFieldType (pField, nColumn);
			 FreeDBDataArrayType(pData , nColumn);
			 OCIErrorReport(hpErr);
			 return -3;
		 }
		 // for LOBs : 4000, for LONG : 0
		 ids = OCIAttrGet((dvoid*) pFieldAttr, OCI_DTYPE_PARAM, (dvoid*) &(pField[nColumn - 1].nSize), 0, OCI_ATTR_DATA_SIZE, hpErr);
		 if (ids)
		 {
			 FreeOraArrayFieldType (pField, nColumn);
			 FreeDBDataArrayType(pData , nColumn);
			 OCIErrorReport(hpErr);
			 return -4;
		 }
		 pField[nColumn - 1].nSize %= 0xFFFF;
		 
		 // Type conversions to string
		 switch (pField[nColumn-1].nType)
		 {
		 case SQLT_INT:
		 case SQLT_FLT:
		 case SQLT_UIN:
			 pData [nColumn - 1].nDataType = NUMERIC;
			 pField[nColumn - 1].nType = SQLT_STR;
			 pField[nColumn - 1].nSize += 1; // allow for null-terminator
			 break;
		 case SQLT_DAT:
			 pData [nColumn - 1].nDataType = DATE;
			 pField[nColumn - 1].nType = SQLT_STR;
			 pField[nColumn - 1].nSize = DATE_TYPE_LEN + 1;
			 break;

		 case SQLT_LNG:
			 pData [nColumn - 1].nDataType = STRING;
			 pField[nColumn - 1].nType = SQLT_STR;
			 pField[nColumn - 1].nSize = LONG_TYPE_LEN + 1;
			 break;

		 case SQLT_CLOB:
		 case SQLT_BLOB:
			 if ( pField[nColumn-1].nType == SQLT_CLOB )
				 pData [nColumn - 1].nDataType = STRING;
			 else
	 			 pData [nColumn - 1].nDataType = BINARY;
			 pField[nColumn - 1].nSize = 0;

			 OCIDescriptorAlloc(hpEnv, &pField[nColumn - 1].pLobLoc, OCI_DTYPE_LOB, 0, NULL);
			
			 break;
		 case SQLT_FILE:
			 FreeOraArrayFieldType (pField, nColumn);
			 FreeDBDataArrayType(pData , nColumn);
			 return -6;
			 break;
		 case SQLT_RDD: // 2003.1.14(WSO) - ROWID ??
			 pData [nColumn-1].nDataType = STRING;
			 pField[nColumn-1].nType   = SQLT_STR;
			 pField[nColumn-1].nSize   = 19; // ROWID ??? ??? 18 ???
			 break;
		 default:
			 pData [nColumn - 1].nDataType = STRING;
			 pField[nColumn - 1].nType = SQLT_STR;
			 pField[nColumn - 1].nSize += 1; // allow for null-terminator
			 break;
		 }

		 // Set size and malloc data buffer
		 if (0 < pField[nColumn-1].nSize)
		 {
			 pField[nColumn-1].pBuffer = (TCHAR*) calloc((g_ISLong ? 1: g_Arraysize), sizeof(TCHAR) * pField[nColumn - 1].nSize);
			 pField[nColumn-1].nInd    = (SHORT*) calloc((g_ISLong ? 1: g_Arraysize), sizeof(SHORT));
			 pField[nColumn-1].nLength = (USHORT*) calloc((g_ISLong ? 1: g_Arraysize), sizeof(USHORT));
			 if (NULL == pField[nColumn-1].pBuffer)
			 {
				 FreeOraArrayFieldType (pField, nColumn);
				 FreeDBDataArrayType(pData , nColumn);
				 return -7;
			 }
		 }
	} // end of while

	*pColumn = nColumn;
	*pDBData = pData;
	*pDBField = pField;
	
	return 0;
}

// Define storage for receiving input variables
INT OraSQLArrayDefine(OCIStmt* hpStmt, OCIError* hpErr, INT nColumn, ORA_ARRAY_FIELD_TYPE* pField )
{

	 INT i;
	INT ids;
	OCIDefine* pDefn = NULL; // to hold pointer to field definition

	for (i = 0; i < nColumn; i++)
	{
		 // Bind
		 pDefn = NULL;
		 switch (pField[i].nType)
		 {
		 case SQLT_CLOB:
		 case SQLT_BLOB:
			 ids = OCIDefineByPos(	   hpStmt									, 
									   &pDefn									, 
									   hpErr									, 
									   (ub4) (i + 1)							, 
									   (dvoid*) (&pField[i].pLobLoc)			,
									   (sb4) sizeof(TCHAR) * (pField[i].nSize)	, 
									   (ub2) (pField[i].nType) 			     	,
									   (dvoid*) pField[i].nInd 					, 
									   (ub2*)   pField[i].nLength				,
									   NULL										, 
									   OCI_DEFAULT
									   );
			 break;
		 default:
			 ids = OCIDefineByPos(	hpStmt										, 
									&pDefn										, 
									hpErr										, 
									(ub4) (i + 1)								, 
									(dvoid*) (pField[i].pBuffer)				,
									(sb4) sizeof(TCHAR) * (pField[i].nSize)      , 
									(ub2) (pField[i].nType) 			     	,
								    (dvoid*) pField[i].nInd 					, 
								    (ub2*)   pField[i].nLength					,
									NULL										, 
									OCI_DEFAULT);
			 break;
		 } //end of switch(nType)
		 if(0 > ids) { return -1; }
	} //end of for

	 return 0;
}

// Fetch selected data				
INT OraSQLArrayFetch(
				OCISvcCtx* hpSvc ,
				OCIError* hpErr ,
				OCIStmt* hpStmt ,
				INT nFetchStartNo ,
				INT nFetchCount ,
				INT nColumn ,
				ORA_ARRAY_FIELD_TYPE* pField ,
				INT* pRow ,
				DB_DATA_ARRAY_TYPE* pData
				)
{


	INT		nRow, i, j, ids, ids1, ids2, nSum;
	INT		nRowFetched, nCurrFetched ;
	
	CHAR *pCLobData=NULL;
	TCHAR *pBLobData=NULL;
	TCHAR *pValue;
	
	
	if( 1 > nFetchStartNo) nFetchStartNo = 1;
	if(-1 > nFetchCount ) nFetchCount = -1;
	

	nRow		 = 
	nRowFetched  =
	nCurrFetched = 0;

	while (TRUE)
	{
		for (i = 0; i < nColumn; i++)
		{
//			wmemset(pField[i].pBuffer, 0x00, (g_ISLong ? 1: g_Arraysize) * sizeof(TCHAR) * pField[i].nSize);
			wmemset(pField[i].pBuffer, 0x00, (g_ISLong ? 1: g_Arraysize) * pField[i].nSize);
		}

		ids1 = OCIStmtFetch(hpStmt, hpErr, (g_ISLong ? 1: g_Arraysize), OCI_FETCH_NEXT, OCI_DEFAULT);
		if( ids1 == OCI_NO_DATA || ids1 == OCI_ERROR ) 
		{
			ids2 = OCIAttrGet(hpStmt, OCI_HTYPE_STMT, (VOID *) &nRowFetched, 0, OCI_ATTR_ROWS_FETCHED, hpErr);			  
			if ( ids1 == OCI_NO_DATA && nRowFetched == 0 )
				break;
			else if ( nRow > 0 && nRowFetched == 0 )
				break;
			else if ( ids1 == OCI_ERROR && nRowFetched == 0 )
				return -1;
		}	
		else if ( ids1 == OCI_SUCCESS_WITH_INFO || ids1 == OCI_SUCCESS )
		{
			if ( ids1 == OCI_SUCCESS_WITH_INFO )
				OCIErrorReport(hpErr);

			ids2 = OCIAttrGet(hpStmt, OCI_HTYPE_STMT, (VOID *) &nRowFetched, 0, OCI_ATTR_ROWS_FETCHED, hpErr);			  
		}
		else
			return -2;			

		
		for ( j = 0 ; j < nRowFetched ; j++ )
		{
			nRow++;
			for (i = 0; i < nColumn; i++)
			{
				pValue = &pField[i].pBuffer[pField[i].nSize*j];

				pData[i].pDBDataUnion = (DB_DATA_UNION*) realloc(pData[i].pDBDataUnion, nRow * sizeof(DB_DATA_UNION));
				if(NULL == pData[i].pDBDataUnion) { return -3; }
				
				memset(&pData[i].pDBDataUnion[nRow - 1], 0x00, sizeof(DB_DATA_UNION));
				pData[i].nRowCount = nRow;
				
				switch (pField[i].nType)
				{
					ub4 amt, offset;
				case SQLT_CLOB:
					// Lob Open nLnd
					if(pField[i].nInd == 0)
					{
						if(0 > OCILobOpen(hpSvc, hpErr, (OCILobLocator*) pField[i].pLobLoc, OCI_LOB_READONLY))
						{
							OCIErrorReport(hpErr);
							return -4;
						}
						// Lob Length
						if(0 > OCILobGetLength(hpSvc, hpErr, (OCILobLocator*) pField[i].pLobLoc, &pData[i].nDataLength))
						{
							OCIErrorReport(hpErr);
							return -5;
						}
						pData[i].nDataLength++;
						// Memory Allocation
						pCLobData = (CHAR*) malloc(sizeof(CHAR) * pData[i].nDataLength);
						if(NULL == pCLobData) { return -6; }
						// Lob Read
						amt = 0; offset = 1; nSum = 0;
						
						while(TRUE)
						{
							ids = OCILobRead(
								hpSvc ,
								hpErr ,
								(OCILobLocator*) pField[i].pLobLoc ,
								&amt ,
								offset ,
								&pCLobData[0] + nSum ,								
								(pData[i].nDataLength - 1) ,
								(dvoid*) 0 ,
								(sb4 (*)(dvoid*, const dvoid*, ub4, ub1)) 0,
								(ub2) 0 ,
								(ub1) SQLCS_IMPLICIT
								);
							if (ids == 0) break;
							else if(ids == 99) nSum += amt;
							else { return -7; }
						}
						pCLobData[pData[i].nDataLength - 1] = _T('\0');						
					}
					else
					{
						pCLobData = (CHAR*) malloc(sizeof(CHAR) * 1);
						pCLobData[0] = _T('\0');
					}
					
					// Write Data to Output Buffer					
					if(SQLT_CLOB == pField[i].nType)
					{
						// UTF8 --> UNICODE							
						TCHAR* pDestData;
						INT nLength;
						MultiByteToUnicode(pCLobData, &pDestData, &nLength);
						pData[i].pDBDataUnion[nRow - 1].pStringData = pDestData;
						
					}
					else
					{
						// UTF8 --> UNICODE
						TCHAR* pDestData;
						INT nLength;
						MultiByteToUnicode(pCLobData, &pDestData, &nLength);
						pData[i].pDBDataUnion[nRow - 1].BinData.nDataSize = pData[i].nDataLength - 1;
						pData[i].pDBDataUnion[nRow - 1].BinData.pBinaryData = pDestData;
					}
					OCILobClose(hpSvc, hpErr, (OCILobLocator*) pField[i].pLobLoc);
					break;
					
				case SQLT_BLOB:
					if(pField[i].nInd == 0)
					{
						if(0 > OCILobOpen(hpSvc, hpErr, (OCILobLocator*) pField[i].pLobLoc, OCI_LOB_READONLY))
						{
							OCIErrorReport(hpErr);
							return -8;
						}
						// Lob Length
						if(0 > OCILobGetLength(hpSvc, hpErr, (OCILobLocator*) pField[i].pLobLoc, &pData[i].nDataLength))
						{
							OCIErrorReport(hpErr);
							return -9;
						}
						// Memory Allocation
						pBLobData = (TCHAR*) malloc(pData[i].nDataLength);
						if(NULL == pBLobData) return -10;
						// Lob Read
						amt = 0; offset = 1; nSum = 0;
						while(TRUE)
						{
							ids = OCILobRead(hpSvc,
								hpErr,
								(OCILobLocator*) pField[i].pLobLoc,
								&amt,
								offset,
								&pBLobData[0] + nSum ,
								pData[i].nDataLength ,
								(dvoid*) 0,
								(sb4 (*)(dvoid*, const dvoid*, ub4, ub1)) 0,
								(ub2) 0,
								(ub1) SQLCS_IMPLICIT);
							if (ids == 0) break;
							else if (ids == 99) nSum += amt;
							else return -11;
						}
					}
					else
					{
						pBLobData = (TCHAR*) malloc(sizeof(TCHAR) * 1);
						pBLobData[0] = _T('\0');
					}

					pData[i].pDBDataUnion[nRow - 1].BinData.nDataSize = pData[i].nDataLength;
					pData[i].pDBDataUnion[nRow - 1].BinData.pBinaryData = pBLobData;
					OCILobClose(hpSvc, hpErr, (OCILobLocator*) pField[i].pLobLoc);
					break;
					
				default:
					// Get Output Buffer Size
					pData[i].nDataLength = _tcsclen(pValue) + 1;
					// Memory Allocation
					pData[i].pDBDataUnion[nRow - 1].pStringData = (TCHAR*) malloc(sizeof(TCHAR) * pData[i].nDataLength);
					if( NULL == pData[i].pDBDataUnion[nRow - 1].pStringData ) { return -12; }
					// Write Data to Output Buffer
//					wmemcpy(pData[i].pDBDataUnion[nRow - 1].pStringData, pValue, sizeof(TCHAR) * pData[i].nDataLength);
					wmemcpy(pData[i].pDBDataUnion[nRow - 1].pStringData, pValue,  pData[i].nDataLength);
					ECOStrRTrim(pData[i].pDBDataUnion[nRow - 1].pStringData, pData[i].pDBDataUnion[nRow - 1].pStringData, _T(' '));
					ECOStrRTrim(pData[i].pDBDataUnion[nRow - 1].pStringData, pData[i].pDBDataUnion[nRow - 1].pStringData, _T('\t'));
					break;
				} //end of switch(nType)
			} //end of for(i:nColumn)		
		} // end of for (j:fetched count)
	} //end of while

	*pRow = nRow;
	
return 0;
}

VOID FreeOraArrayFieldType(ORA_ARRAY_FIELD_TYPE *pOraFieldType, INT nColumnCount)
{
	INT i;
	
	if (NULL == pOraFieldType) { return; }
	
	for (i = 0; i < nColumnCount; i++)
	{
		if (pOraFieldType[i].pBuffer != NULL)
		{
			free(pOraFieldType[i].pBuffer);
			free(pOraFieldType[i].nInd);
			free(pOraFieldType[i].nLength);
		}
		
		if (NULL != pOraFieldType[i].pLobLoc)
			OCIDescriptorFree(pOraFieldType[i].pLobLoc, OCI_DTYPE_LOB);
	}
	free(pOraFieldType);
	pOraFieldType = NULL;
	
	return;
}

INT	MultiByteToUnicode(CHAR *fi_pSourceData, _TCHAR **fo_pDestinationData, INT *fo_nByteDes )
{

	_TCHAR	*pszDestinationData;
	INT		nWCharNeeded;
	INT		nSouceDataByte;
	

	nSouceDataByte = strlen(fi_pSourceData);
	
	/* Query the number of WChar required to store the Dest string */
	nWCharNeeded = MultiByteToWideChar( CP_UTF8 , 
										0 , 
										fi_pSourceData   , 
										nSouceDataByte   , 
										NULL             , 
										0 );
	if (nWCharNeeded == 0) 
	{
		*fo_nByteDes = 0;
		return -9999;
	}
	

	/* Allocate the required amount of space */	
	/* We need more 2 bytes for '\0' */
	pszDestinationData = (_TCHAR*) malloc (sizeof(_TCHAR)*nWCharNeeded + sizeof(_TCHAR));	
		
	/* Do the conversion */
	nWCharNeeded = MultiByteToWideChar(	CP_UTF8				, 
										0						,
										fi_pSourceData					, 
										nSouceDataByte			   		,
										pszDestinationData          	, 
										nWCharNeeded);
	if (nWCharNeeded == 0) 
	{
		*fo_nByteDes = 0;
		free(pszDestinationData);
		return -9999 ;
	}	

	*(LPWSTR)((LPWSTR)pszDestinationData + nWCharNeeded) = L'\0';
	
	/* MultiByteToWideChar returns # WCHAR, so multiply by 2 */
	*fo_nByteDes = 2*nWCharNeeded ;	

	*fo_pDestinationData = pszDestinationData;


	return 0;
}