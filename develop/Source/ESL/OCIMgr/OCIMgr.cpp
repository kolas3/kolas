// OCIMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "OCIMgr.h"
#include "OCI_API.h"
#include "except.h"
#include "ECO_FILE_API.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COCIMgrApp

BEGIN_MESSAGE_MAP(COCIMgrApp, CWinApp)
	//{{AFX_MSG_MAP(COCIMgrApp)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COCIMgrApp construction

TCHAR g_szUserID [64];				// DB User ID
TCHAR g_szPasswd [64];				// DB User Password
LONG  g_nRequestID;					// 세션Request ID for DM
ECO_SEMA_HANDLE hSemaphore;			// Semaphore Object for Session management

//###################################################################################################################
class CAutoFree
{
public:
	CAutoFree(INT nRID, INT nSID) 
		: m_nRID(nRID), m_nSID(nSID), m_bError(FALSE)	{												}
	virtual ~CAutoFree()								{ if (m_bError) FreeAllAccessVariable(m_nSID);	}
	VOID SetError(BOOL bError=TRUE)						{ m_bError=bError;								}
protected:
	INT m_nRID;
	INT m_nSID;
	BOOL m_bError;
};
class CAutoFreeFrame : public CAutoFree
{
public:
	CAutoFreeFrame(INT nRID, INT nSID) 
		: CAutoFree(nRID, nSID)							{												}
	virtual ~CAutoFreeFrame()							{ if (m_bError) EndFrameProc(m_nRID);			}
};
#define TEST_TRUE(_true_,_ret_value_)					{ if (!(_true_))  { autoFree.SetError(); return _ret_value_; }}
#define TEST_ZERO(_ids_,_ret_value_)					{ if (0!=(_ids_)) { autoFree.SetError(); return _ret_value_; }}
//###################################################################################################################



COCIMgrApp::COCIMgrApp()
{
	InitOraAccessType(g_MainAccess);

	g_nRequestID = -1;

	TCHAR szBuff[1024];
	TCHAR* pToken;
 
	FILE *fp = NULL;

	CString sBuffer;

	if (fp != NULL)
	{
		sBuffer.Format(_T("%s"), szBuff);
		sBuffer.TrimRight(_T("\r\n"));
		sBuffer.TrimRight(_T(" "));

		_stprintf(szBuff, _T("%s"), sBuffer.GetBuffer(0));

		pToken = _tcstok(szBuff, _T(" "));

		_tcscpy(g_szUserID, pToken);

		pToken = _tcstok(NULL , _T(" "));

		_tcscpy(g_szPasswd, pToken);

		fclose(fp);
	}	

#ifdef __TRACE_DATABASE__
	g_szWorkID[0] = _T('\0');
#endif
}

COCIMgrApp::~COCIMgrApp()
{
	DisconnectDB();
}
/////////////////////////////////////////////////////////////////////////////
// The one and only COCIMgrApp object

COCIMgrApp theApp;


LONG GetSessionRequestID()
{
	return InterlockedIncrement(&g_nRequestID);
}

INT getSession					(
								 INT	nRequestID	,
								 INT	nSQLType	,
								 TCHAR*	pszUserID	,
								 TCHAR*	pszPasswd	,
								 INT*	pSessionID
								 )
{
	ASSERT( SQL_SESSION_NEW			== nSQLType	|| 
			SQL_SESSION_EXIST_OPEN	== nSQLType	|| 
			SQL_SESSION_EXIST		== nSQLType		);

	INT ids;

	BOOL   bAuthChanged = FALSE;
	TCHAR* pAt;
	TCHAR  szServiceName[64];
	TCHAR  szUserName   [64];
	
	pAt = _tcsstr(pszUserID, _T("@"));
	if (pAt == NULL) return -1;
	
	_tcsncpy(szUserName, &pszUserID[0], pAt - pszUserID);
	szUserName[pAt - pszUserID] = _T('\0');
	
	_tcscpy (szServiceName, &pAt[1]);
	
	ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
	if (ids) return ids;	//ERR_SEMAPHORE_CREATE OR ERR_SEMAPHORE_EXISTOPEN OR ERR_SEMAPHORE_NONSIGNALED
	
	INT nSessionID = getSessionIDOfRequest(nRequestID);

	if (ERR_REQUEST_NOT_EXIST != nSessionID)
	{
		
		bAuthChanged = (_tcsicmp(SESSION(nSessionID).szServiceName, szServiceName) != 0 ||
			_tcsicmp(SESSION(nSessionID).szUserName   , szUserName   ) != 0 ||
			_tcsicmp(SESSION(nSessionID).szPassWord   , pszPasswd    ) != 0	);
		
		if (SESSION_CLOSED == STATUS(nSessionID) || bAuthChanged) 
		{
#ifdef _DEBUG_DEEP_LOG_
			{
				CString sTemp;
				sTemp.Format(_T("■ getSession      ■ EXIST but CLOSED or AUTH changed ▷ [RID=%04d][SID=%04d] ▷ Reconnect(OCI8ConnectDB)시 오류\n"), nRequestID, nSessionID);
				LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
				AfxMessageBox(sTemp);
#endif
			}
#endif

			ids = OCI8ConnectDB(&SESSION(nSessionID), pszUserID, pszPasswd);
			if (ids) 
			{
				SemaphoreUnLock(hSemaphore);
				
#ifdef _DEBUG_DEEP_LOG_
				{
					CString sTemp;
					sTemp.Format(_T("■ getSession      ■ EXIST but CLOSED ▷ [RID=%04d][SID=%04d] ▷ Reconnect시 오류\n"), nRequestID, nSessionID);
					LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
					AfxMessageBox(sTemp);
#endif
				}
#endif
				
				return ids;
			}
			STATUS (nSessionID) = SESSION_OPENED;
		}
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp, sTemp2;
			if		(SESSION_OPENED  ==STATUS(nSessionID)) sTemp2 = _T("SESSION_OPENED");
			else if (SESSION_FETCHING==STATUS(nSessionID)) sTemp2 = _T("SESSION_FETCHING");
			else if (SESSION_USING   ==STATUS(nSessionID)) sTemp2 = _T("SESSION_USING");
			sTemp.Format(_T("■ getSession      ■ EXIST       ▷ [RID=%04d][SID=%04d] ▷ [%s]\n"), nRequestID, nSessionID, sTemp2);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
	}
	else
	{
		if (SQL_SESSION_EXIST == nSQLType)
		{
			SemaphoreUnLock(hSemaphore);
			
#ifdef _DEBUG_DEEP_LOG_
			{
				CString sTemp;
				sTemp.Format(_T("■ getSession      ■ ### NOT EXIST   ▷ [RID=%04d] ▷ SQL_SESSION_EXIST인데 RequestID존재하지 않음\n"), nRequestID);
				LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
				AfxMessageBox(sTemp);
#endif
			}
#endif
			
			return ERR_REQUEST_NOT_EXIST;
		}
		
		if (SQL_SESSION_NEW			== nSQLType	|| 
			SQL_SESSION_EXIST_OPEN	== nSQLType		)
		{

			nSessionID = getFirstSessionIDWithStatus(SESSION_OPENED, TRUE);

#ifdef _DEBUG_DEEP_LOG_
			{
				CString sTemp;
				sTemp.Format(_T("■ getSession      ■ NOT EXIST   ▷ [RID=%04d][SID=%04d] ▷ 새로운 Opened세션을 가져옴\n"), nRequestID, nSessionID);
				LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
				AfxMessageBox(sTemp);
#endif
			}
#endif

			if (ERR_STATUS_NOT_EXIST != nSessionID)
			{ 
				if (NOT_SET_REQUEST != REQUEST(nSessionID) ) 
				{
					SemaphoreUnLock(hSemaphore);
					
#ifdef _DEBUG_DEEP_LOG_
					{
						CString sTemp;
						sTemp.Format(_T("■ getSession      ■ NOT EXIST   ▷ [RID=%04d][SID=%04d] ▷ 새로운 Opened세션을 가져왔는데 RequestID가 존재함??????(이런 일은 절대 없다!!!!!)\n"), nRequestID, nSessionID);
						LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
						AfxMessageBox(sTemp);
#endif
					}
#endif
					
					return ERR_SESSION_ALLOCATED;
				}
				
				bAuthChanged = (_tcsicmp(SESSION(nSessionID).szServiceName, szServiceName) != 0 ||
								_tcsicmp(SESSION(nSessionID).szUserName   , szUserName   ) != 0 ||
								_tcsicmp(SESSION(nSessionID).szPassWord   , pszPasswd    ) != 0	);
				if (bAuthChanged)
				{
#ifdef _DEBUG_DEEP_LOG_
					{
						CString sTemp;
						sTemp.Format(_T("■ getSession      ■ NOT EXIST   ▷ [RID=%04d][SID=%04d] ▷ 계정(%s@%s) 변경되어 OCI8ConnectDB 수행 시작\n"), nRequestID, nSessionID, szUserName, szServiceName);
						LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
						AfxMessageBox(sTemp);
#endif
					}
#endif
					ids = OCI8ConnectDB(&SESSION(nSessionID), pszUserID, pszPasswd);
					if (ids) 
					{
						SemaphoreUnLock(hSemaphore);
						return ids;
					}
				}
				
			}
			else
			{
				nSessionID = getFirstSessionIDWithStatus(SESSION_CLOSED, FALSE);

#ifdef _DEBUG_DEEP_LOG_
				{
					CString sTemp;
					sTemp.Format(_T("■ getSession      ■ NOT EXIST   ▷ [RID=%04d][SID=%04d] ▷ Opened세션을 찾을 수 없어 다시 closed세션을 가져옴\n"), nRequestID, nSessionID);
					LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
					AfxMessageBox(sTemp);
#endif
				}
#endif

				if (ERR_STATUS_NOT_EXIST==nSessionID) 
				{
					SemaphoreUnLock(hSemaphore);
					
#ifdef _DEBUG_DEEP_LOG_
					{
						CString sTemp;
						sTemp.Format(_T("■ getSession      ■ NOT EXIST   ▷ [RID=%04d][SID=%04d] ▷ 새로운 Opened세션 없음 & Closed세션도 없음 >>> ERR_SESSION_MAX\n"), nRequestID, nSessionID);
						LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
						AfxMessageBox(sTemp);
#endif
					}
#endif
					
					return ERR_SESSION_MAX;			// -900
				}
				
				#ifdef _DEBUG_DEEP_LOG_
				{
						CString sTemp;
						sTemp.Format(_T("■ getSession      ■ NOT EXIST   ▷ [RID=%04d][SID=%04d] ▷ Closed세션을 가져와 OCI8ConnectDB 수행 시작\n"), nRequestID, nSessionID);
						LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
						AfxMessageBox(sTemp);
#endif
				}
				#endif
				
				ids = OCI8ConnectDB(&SESSION(nSessionID), pszUserID, pszPasswd);
				if (ids) 
				{
					SemaphoreUnLock(hSemaphore);
					
#ifdef _DEBUG_DEEP_LOG_
					{
						CString sTemp;
						sTemp.Format(_T("■ getSession      ■ NOT EXIST   ▷ [RID=%04d][SID=%04d] ▷ 새로운 Opened세션 없음 & Closed세션 가져와 Connect시에 오류 발생\n"), nRequestID, nSessionID);
						LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
						AfxMessageBox(sTemp);
#endif
					}
#endif
					
					return ids;
				}
			}
			STATUS(nSessionID) = SESSION_OPENED ;
			
#ifdef _DEBUG_DEEP_LOG_
			{
				CString sTemp;
				sTemp.Format(_T("■ getSession      ■ NOT EXIST   ▷ [RID=%04d][SID=%04d] ▷ 신규세션 Open함 [SESSION_OPENED]\n"), nRequestID, nSessionID);
				LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
				AfxMessageBox(sTemp);
#endif
			}
#endif
			
		}			
		else
		{
			SemaphoreUnLock(hSemaphore);
			
#ifdef _DEBUG_DEEP_LOG_
			{
				CString sTemp;
				sTemp.Format(_T("■ getSession      ■ SQLType 오류 ▷ [RID=%04d] ▷ SQLType = %d\n"), nRequestID, nSQLType);
				LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
				AfxMessageBox(sTemp);
#endif
			}
#endif
			
			return ERR_SQLTYPE_INVALID;
		}
	}
	
	_tcscpy( SESSION(nSessionID).szServiceName, szServiceName );
	_tcscpy( SESSION(nSessionID).szUserName   , szUserName    );
	_tcscpy( SESSION(nSessionID).szPassWord   , pszPasswd     );
	REQUEST(nSessionID) = nRequestID;
	if (pSessionID) (*pSessionID) = nSessionID;
	
#ifdef _DEBUG_COUNT_SESSION_
	{
		CString sCount;
		sCount.Format(_T("■ getSession      ■ Open세션수  ■ ━[%d]━\n"), countOpenSession(TRUE));
		LoggingProcess(_T("OCIMGR_SESSION"), sCount.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sCount);
#endif
		CString sTemp;
		for (INT i=0 ; i<MAX_SESSION_CNT ; i++)
		{
			sTemp.Format(_T("■ getSession      ■ [SID=%04d] ━SessionStatus = [%d]━\n"), i, STATUS(i));
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
		}
	}
#endif
	
	
	SemaphoreUnLock(hSemaphore);
	
	return 0;
}

INT getFirstSessionIDWithStatus	(INT	nSessionStatus,
								 BOOL	bNotSetRequest)
{
	INT nSessionID = ERR_STATUS_NOT_EXIST;
	for (INT i=0 ; i<MAX_SESSION_CNT ; i++)
	{
		if (bNotSetRequest && NOT_SET_REQUEST!=REQUEST(i)) continue;
		if (nSessionStatus == g_MainAccess[i].nStatus)
		{
			nSessionID = i;
			break;
		}
	}
	return nSessionID;
}

INT getSessionIDOfRequest		(INT	nRequestID)
{
	INT nSessionID = ERR_REQUEST_NOT_EXIST;
	for (INT i=0 ; i<MAX_SESSION_CNT ; i++)
	{
		if (nRequestID == REQUEST(i))
		{
			nSessionID = i;
			break;
		}
	}
	return nSessionID;
}

INT getSessionStatusOfRequest	(INT	nRequestID)	
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	
	return STATUS(nSessionID);
}

INT setSessionStatusForRestFetch	(INT nRequestID)
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;

	INT ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
	if (ids) return ids;
	
	if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_FETCHING;
	else if (SESSION_USING == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
	else ids = ERR_STATUS_INVALID;

	SemaphoreUnLock(hSemaphore);

	return ids;
}

INT countOpenSession			(BOOL bCountUSING)
{
	INT nCnt = 0;
	for (INT i=0 ; i<MAX_SESSION_CNT ; i++)
	{
		if (SESSION_OPENED == STATUS(i)) nCnt++;
		else if (bCountUSING && (SESSION_USING == STATUS(i) || SESSION_FETCHING == STATUS(i))) nCnt++;
	}
	return nCnt;
}

BOOL isClosable					(INT	nSessionID)
{
	ASSERT(MAX_SESSION_CNT>nSessionID);
	ASSERT(-1<nSessionID);
	BOOL bClosable = (SESSION_USING    != STATUS(nSessionID) && 
					  SESSION_FETCHING != STATUS(nSessionID)   );
	return bClosable;
}

INT closeSession				(INT	nSessionID,			
								 BOOL	bForceClose,		
								 BOOL	bUseMaxOpenCnt)		
{
	INT ids;

	ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
	if (ids) return ids;
	
	if (!bForceClose && !isClosable(nSessionID)) 
	{
		SemaphoreUnLock(hSemaphore);

#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp, sTemp2;	
			if		(SESSION_CLOSED==STATUS(nSessionID))	sTemp2 = _T("SESSION_CLOSED");
			else if (SESSION_OPENED==STATUS(nSessionID))	sTemp2 = _T("SESSION_OPENED");
			else if (SESSION_USING==STATUS(nSessionID))		sTemp2 = _T("SESSION_USING");
			else if (SESSION_FETCHING==STATUS(nSessionID))	sTemp2 = _T("SESSION_FETCHING");
			sTemp.Format(_T("□ closeSession    □ 세션닫기불가□ [SID=%04d]           ▷ [%s]\n"), nSessionID, sTemp2);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif

		return ERR_SESSION_USING;
	}

	BOOL bClosable; 
	bClosable = (!bUseMaxOpenCnt) ? TRUE : (MAX_OPEN_SESSION_CNT<countOpenSession(TRUE));
	bClosable = bClosable || bForceClose;
	
	if (bClosable) 
	{
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□ closeSession    □ 세션닫기가능□ [SID=%04d]           ▷ OCI8DisconnectDB 수행 시작\n"), nSessionID);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		ids = OCI8DisconnectDB(&SESSION(nSessionID));
		if (-1==ids) ids = -100;
		if (ids) 
		{
			SemaphoreUnLock(hSemaphore);
			
#ifdef _DEBUG_DEEP_LOG_
			{
				CString sTemp;
				sTemp.Format(_T("□ closeSession    □ [SID=%04d] ▷ OCI8DisconnectDB ▷ %d 리턴\n"), nSessionID, ids);
				LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
				AfxMessageBox(sTemp);
#endif
			}
#endif
			
			return ids;
		}
	}
	
	STATUS(nSessionID)  = (bClosable) ? SESSION_CLOSED : SESSION_OPENED;
	REQUEST(nSessionID) = NOT_SET_REQUEST;
	
#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp, sTemp2;	
		if		(SESSION_CLOSED==STATUS(nSessionID)) sTemp2 = _T("SESSION_CLOSED");
		else if (SESSION_OPENED==STATUS(nSessionID)) sTemp2 = _T("SESSION_OPENED");
		sTemp.Format(_T("□ closeSession    □ 세션을 닫음 ▷ [SID=%04d]           ▷ [%s & NOT_SET_REQUEST]\n"), nSessionID, sTemp2);
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif

#ifdef _DEBUG_COUNT_SESSION_
	{
		CString sCount;
		sCount.Format(_T("□ closeSession    □ Open세션수  □ ━[%d]━\n"), countOpenSession(TRUE));
		LoggingProcess(_T("OCIMGR_SESSION"), sCount.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sCount);
#endif
		CString sTemp;
		for (INT i=0 ; i<MAX_SESSION_CNT ; i++)
		{
			sTemp.Format(_T("□ closeSession    □ [SID=%04d] ━SessionStatus = [%d]━\n"), i, STATUS(i));
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
		}
	}
#endif

	SemaphoreUnLock(hSemaphore);
	
	return 0;
}

INT closeSessionOfReqeust		(INT	nRequestID,		
								 BOOL	bForceClose,	
								 BOOL	bUseMaxOpenCnt)	
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;

#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("□□□□□□□□□ □ closeSessionOfReqeust->closeSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif
	
	INT ids = closeSession(nSessionID, bForceClose, bUseMaxOpenCnt);
	if		(bForceClose && 0>ids) return ids;
	else if (ERR_SESSION_USING != ids && ids) return ids;
	return 0;
	
}

INT closeAllSession	()
{
	INT ids;

	ids = SemaphoreOpen(SEMAPHORE_NAME, 1, &hSemaphore);
	if (ids) return ids;

	for (INT i=0 ; i<MAX_SESSION_CNT ; i++)
	{
		ids = SemaphoreLock(hSemaphore, WAIT_INFINITE);
		if (ids) return ids;

		if (SESSION_CLOSED == STATUS(i))
		{
			SemaphoreUnLock(hSemaphore);
			continue;
		}

		ids = OCI8DisconnectDB(&SESSION(i));
		if (ids) 
		{
			SemaphoreUnLock(hSemaphore);
			return ids;
		}
		STATUS(i)  = SESSION_CLOSED;
		REQUEST(i) = NOT_SET_REQUEST;
		SemaphoreUnLock(hSemaphore);
	}

#ifdef _DEBUG_COUNT_SESSION_
	{
		CString sCount;
		sCount.Format(_T("▩ closeAllSession ▩ Open세션수  ▩ ━[%d]━\n"), countOpenSession(TRUE));
		LoggingProcess(_T("OCIMGR_SESSION"), sCount.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sCount);
#endif
	}
#endif
	
	return 0;
}


INT  ConnectDB					(INT	nRequestID	,
								 TCHAR* pszUserID	,
								 TCHAR* pszPasswd	
								 )		
{
	INT ids;

	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];

	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}
	
#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ ConnectDB->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif

	ids = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, NULL);	
	if (ids) return ids;
	
	return 0;
}

INT  DisconnectDB()
{
	return closeAllSession();
}

INT DisconnectDBOfRequest(INT	nRequestID		,
						  BOOL	bForceClose		,
						  BOOL	bUseMaxOpenCnt
						  )
{
	return closeSessionOfReqeust(nRequestID, bForceClose, bUseMaxOpenCnt);
}

INT  DisconnectDBForOneRecord(INT					nRequestID		, 
							  BOOL					bForceClose		,
							  BOOL					bUseMaxOpenCnt	,
					          ORA_SELECT_STRU*      pSelect			,
					          EDBM_DATA_ARRAY_TYPE* pOutData		
							  )
{
	INT ids;

	FreeOraFieldType   (pSelect->pField , pSelect->nColumnCount );
	FreeDBDataArrayType(pOutData->pColumnData, pOutData->nColumnCount);
	OCI8StmtHandleFree (pSelect->hpStmt);

	pSelect->hpStmt        = NULL;
	pSelect->nColumnCount  = 0;
	pSelect->pField        = NULL;

	pOutData->nColumnCount = 0;
	pOutData->nRowCount    = 0;
	pOutData->pColumnData  = 0;

	ids = closeSessionOfReqeust(nRequestID, bForceClose, bUseMaxOpenCnt);
	if (ids) return ids;

	return 0;
}

INT  SelectProc(
				INT					 nRequestID		,
				TCHAR*               pszUserID		,
                TCHAR*               pszPasswd		,
                TCHAR*               pszSQL			,
                INT*                 pRowCount		,
                INT*                 pColumnCount	,
                DB_DATA_ARRAY_TYPE** pOutData		,
				EDBM_BIND_DATA_TYPE *pBind
               )
{
	INT ids;

	EDBM_DATA_ARRAY_TYPE Data;

	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];

	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}

#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ SelectProc->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif

	INT nSessionID;
	ids = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
	if (ids) return ids;

#ifdef _DEBUG_DEEP_LOG_
	CTime t_start = CTime::GetCurrentTime();
	DWORD START, FINISH;
	START = GetTickCount(); 
#endif

	INT rc = OCI8SelectProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, pszSQL, 1, -1, &Data, pBind);

#ifdef _DEBUG_DEEP_LOG_
	FINISH = GetTickCount();

	{
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
		// ------------------------------------------------------------------------------
			
			_ftprintf(fp, _T("[%04d-%02d-%02d.%02d:%02d:%02d][][OCI8S] ■[OCI8SelectProc=%d.%03ds]▶\r\n"), 
							t_start.GetYear(), t_start.GetMonth(),  t_start.GetDay(), t_start.GetHour(), t_start.GetMinute(), t_start.GetSecond(), 
							(FINISH-START)/1000, (FINISH-START)%1000);
			fclose(fp);
		}
	}
#endif
	
	if (-9001==rc) 
	{
		INT   ret;
		utext msg[512];
		sb4   errcode = 0;
		
		OCIErrorGet((dvoid*)SESSION(nSessionID).hpErr, (ub4) 1, (text*)NULL, &errcode, (text*)msg, (ub4) sizeof(msg), (ub4) OCI_HTYPE_ERROR); 
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SelectProc ▶ [OCIError=%d]\n"), errcode);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		BOOL bReconnectable = FALSE;
			bReconnectable = (3114==errcode) || (1012==errcode) || (28==errcode);
		
		if (!bReconnectable) return -8001;
		
		ret = closeSession(nSessionID, TRUE, TRUE);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SelectProc ▶ closeSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;
		
		ret = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SelectProc ▶ getSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;
		
		rc = OCI8SelectProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, pszSQL, 1, -1, &Data, pBind );
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SelectProc ▶ Reconnected & Retry OCI8SelectProc [Return=%d]\n"), rc);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
	}

	if (OCI_NO_DATA == rc) rc = 0;

	if (-1<rc)
	{
		*pRowCount    = Data.nRowCount;
		*pColumnCount = Data.nColumnCount;
		pOutData[0]   = Data.pColumnData;
	}
	
#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("□□□□□□□□□ □ SelectProc->closeSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif
	
	ids = closeSession(nSessionID, FALSE, TRUE);
//cs--

	if (rc) return rc;
	if (ERR_SESSION_USING != ids && ids) return ids;

	return 0;
}

INT  GetOneFieldValueProc(
						  INT		nRequestID		,
						  TCHAR*	pszUserID		,
						  TCHAR*	pszPasswd		,
						  TCHAR*	pszSQL			,
						  TCHAR*	pszReturnValue	,
						  EDBM_BIND_DATA_TYPE *pBind
						  )
{
	INT ids;
	
	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];
	
	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}
	
#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ GetOneFieldValueProc->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif

	INT nSessionID;
	ids = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
	if (ids) return ids;
	
	ids = OCI8GetOneFieldValueProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, pszSQL, pszReturnValue, pBind);

	if (-9001==ids) 
	{
		INT   ret;
		utext msg[512];
		sb4   errcode = 0;
		
		OCIErrorGet((dvoid*)SESSION(nSessionID).hpErr, (ub4) 1, (text*)NULL, &errcode, (text*)msg, (ub4) sizeof(msg), (ub4) OCI_HTYPE_ERROR); 
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶GetOneFieldValueProc ▶ [OCIError=%d]\n"), errcode);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		BOOL bReconnectable = FALSE;
			bReconnectable = (3114==errcode) || (1012==errcode) || (28==errcode);
		
		if (!bReconnectable) return -8001;
		
		ret = closeSession(nSessionID, TRUE, TRUE);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶GetOneFieldValueProc ▶ closeSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;
		
		ret = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶GetOneFieldValueProc ▶ getSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;
		
		LoggingProcess(_T("ESL_DM"), _T("◀◀◀ ORACLE Reconnected ▶▶▶"));
		
		ids = OCI8GetOneFieldValueProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, pszSQL, pszReturnValue, pBind );
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶GetOneFieldValueProc ▶ Reconnected & OCI8GetOneFieldValueProc [Return=%d]\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
	}


	if (ids)
	{
		FreeAllAccessVariable(nSessionID);
	}
	
#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("□□□□□□□□□ □ GetOneFieldValueProc->closeSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif

	INT rc = closeSession(nSessionID, FALSE, TRUE);

	if (ids) return ids;
	if (ERR_SESSION_USING != rc && rc) return rc;
	
	return 0;
}

INT  SelectProcForOneRecord		(INT					nRequestID	,
								 TCHAR*					pszUserID	,
								 TCHAR*					pszPasswd	,
								 TCHAR*					pszSQL		,
								 ORA_SELECT_STRU*		pSelect		,
								 EDBM_DATA_ARRAY_TYPE*	pOutData	,
								 EDBM_BIND_DATA_TYPE*   pBind
								 )
{
	INT ids;
	INT rc;
	
	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];
	
	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}

#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ SelectProcForOneRecord->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif
	
	INT nSessionID;
	ids = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
	if (ids) return ids;


	ids = OCI8SelectExProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, pszSQL, pSelect, pOutData, pBind);

	if (-9001==ids) 
	{
		INT   ret;
		utext msg[512];
		sb4   errcode = 0;
		
		OCIErrorGet((dvoid*)SESSION(nSessionID).hpErr, (ub4) 1, (text*)NULL, &errcode, (text*)msg, (ub4) sizeof(msg), (ub4) OCI_HTYPE_ERROR); 
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SelectProcForOneRecord ▶ [OCIError=%d]\n"), errcode);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		BOOL bReconnectable = FALSE;
			bReconnectable = (3114==errcode) || (1012==errcode) || (28==errcode);
		
		if (!bReconnectable) return -8001;
		
		ret = closeSession(nSessionID, TRUE, TRUE);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SelectProcForOneRecord ▶ closeSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;
		
		ret = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SelectProcForOneRecord ▶ getSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;

		LoggingProcess(_T("ESL_DM"), _T("◀◀◀ ORACLE Reconnected ▶▶▶"));
		
		ids = OCI8SelectExProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, pszSQL, pSelect, pOutData, pBind);
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SelectProcForOneRecord ▶ Reconnected & Retry OCI8SelectExProc [Return=%d]\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
	}
	
	if (ids)
	{
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ SelectProcForOneRecord[Error=%d]->closeSession ◎。。。。。。。\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif

		rc = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
		if (ERR_SESSION_USING != rc && rc) return rc;
	}
	else
	{
		ids = setSessionStatusForRestFetch(nRequestID);
		if (ids) 
		{

#ifdef _DEBUG_DEEP_LOG_
			{
				CString sTemp;
				sTemp.Format(_T("□□□□□□□□□ □ SelectProcForOneRecord->setSessionStatusForRestFetch[Error=%d]->closeSession ◎。。。。。。。\n"), ids);
				LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
				AfxMessageBox(sTemp);
#endif
			}
#endif

			rc = closeSession(nSessionID, FALSE, TRUE);
			if (ids) return ids;
			if (ERR_SESSION_USING != rc && rc) return rc;
		}
	}
	
	return 0;
}

INT  FetchOneRecord(
					INT						nRequestID	,
					ORA_SELECT_STRU*		pSelect		,
					EDBM_DATA_ARRAY_TYPE*	pOutData	,	
					BOOL					bCloseSession
					)
{
	INT ids;
	BOOL bAllFetched = FALSE;
	
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	
	ids = OCI8SelectFetchProc(&SESSION(nSessionID), pSelect, 1, 1, pOutData);

	if (OCI_NO_DATA != ids && ids) return ids;
	if (bCloseSession || OCI_NO_DATA == ids)
	{
		if (OCI_NO_DATA == ids) bAllFetched = TRUE;

#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ FetchOneRecord->closeSession ◎。。。。。。。\n"));
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif

		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ERR_SESSION_USING != ids && ids) return ids;
	}	

	if (bAllFetched) return OCI_NO_DATA;
	else return 0;
}

INT  SelectProcEx(
				  INT					nRequestID	,
				  TCHAR*				pszUserID   ,
				  TCHAR*				pszPasswd   ,
				  TCHAR*				pszSQL      ,
				  ORA_SELECT_STRU*      pSelect		,
				  EDBM_DATA_ARRAY_TYPE* pOutData	,		
				  EDBM_BIND_DATA_TYPE *pBind
				  )
{
	INT ids;
	INT rc;
	
	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];

	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}

#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ SelectProcEx->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif
	
	INT nSessionID;
	ids = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
	if (ids) return ids;

	ids = OCI8SelectExProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, pszSQL, pSelect, pOutData, pBind);

	if (-9001==ids) 
	{
		INT   ret;
		utext msg[512];
		sb4   errcode = 0;
		
		OCIErrorGet((dvoid*)SESSION(nSessionID).hpErr, (ub4) 1, (text*)NULL, &errcode, (text*)msg, (ub4) sizeof(msg), (ub4) OCI_HTYPE_ERROR); 
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SelectProcEx ▶ [OCIError=%d]\n"), errcode);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		BOOL bReconnectable = FALSE;
			bReconnectable = (3114==errcode) || (1012==errcode) || (28==errcode);
		
		if (!bReconnectable) return -8001;
		
		ret = closeSession(nSessionID, TRUE, TRUE);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SelectProcEx ▶ closeSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;
		
		ret = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SelectProcEx ▶ getSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;

		LoggingProcess(_T("ESL_DM"), _T("◀◀◀ ORACLE Reconnected ▶▶▶"));
		
		ids = OCI8SelectExProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, 
							   pszSQL, pSelect, pOutData, pBind);

#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SelectProcEx ▶ Reconnected & Retry OCI8SelectExProc [Return=%d]\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
	}

	if (ids)
	{

#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ SelectProcEx[Error=%d]->closeSession ◎。。。。。。。\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		rc = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
		if (ERR_SESSION_USING != rc && rc) return rc;
	}
	else
	{
		ids = setSessionStatusForRestFetch(nRequestID);
		if (ids) 
		{

#ifdef _DEBUG_DEEP_LOG_
			{
				CString sTemp;
				sTemp.Format(_T("□□□□□□□□□ □ SelectProcEx->setSessionStatusForRestFetch[Error=%d]->closeSession ◎。。。。。。。\n"), ids);
				LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
				AfxMessageBox(sTemp);
#endif
			}
#endif

			rc = closeSession(nSessionID, FALSE, TRUE);
			if (ids) return ids;
			if (ERR_SESSION_USING != rc && rc) return rc;
		}
	}

	return 0;
}

INT  FetchEx(
			 INT				nRequestID	,
			 ORA_SELECT_STRU*	pSelect		,
			 INT				nRowFetch	,
			 INT				nColumnCount,
			 BOOL				bCloseSession
			 )
{
	INT ids;
	BOOL bAllFetched = FALSE;
	
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	
	ids = OraSQLFetchEx(SESSION(nSessionID).hpSvc, 
						SESSION(nSessionID).hpErr, 
						pSelect->hpStmt, 
						nRowFetch, 
						nColumnCount, 
						pSelect->pField);

	if (OCI_NO_DATA != ids && ids) return ids;
	if (bCloseSession || OCI_NO_DATA == ids)
	{
		if (OCI_NO_DATA == ids) bAllFetched = TRUE;	

#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ FetchEx->closeSession ◎。。。。。。。\n"));
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif

		ids = closeSession(nSessionID, TRUE, TRUE);
		if (ERR_SESSION_USING != ids && ids) return ids;

	}
	

	if (bAllFetched) return OCI_NO_DATA;
	else return 0;
}

INT  UnSelectProc(
				  INT		nRequestID	,
				  TCHAR*	pszUserID   ,
				  TCHAR*	pszPasswd   ,
                  TCHAR*	pszSQL      ,
				  INT		nFrameIndex
				  )
{
	INT ids;
	
	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];
	
	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}
	
#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ UnSelectProc->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif
	
	INT nSessionID;
	ids = getSession(nRequestID, SQL_SESSION_EXIST_OPEN, szTmpUserID, szTmpPasswd, &nSessionID);
	
	if (ids) return ids;

	ids = OCI8UnSelectProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, pszSQL, nFrameIndex);
	
	if (ids)
	{
		FreeAllAccessVariable(nSessionID);

		return ids;
	}

	ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);

	if (ids) return ids;
	
	if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
	else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
	else return ERR_STATUS_INVALID;
	
	SemaphoreUnLock(hSemaphore);

	return 0;
}

INT	 InsertProc(
				INT		nRequestID		,
				TCHAR*  pszUserID       ,
                TCHAR*  pszPasswd       ,
				TCHAR*  pszTableName    ,
				INT     nColumnCount    ,
				TCHAR** pszAttributeName,
				TCHAR** pszAttributeData,
				INT*    pInDataType     ,
				BOOL    bCommit				
				)
{
	INT ids;
	
	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];
	
	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}
	
#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ InsertProc->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif
	
	INT nSessionID;
	ids = getSession(nRequestID, SQL_SESSION_EXIST_OPEN, szTmpUserID, szTmpPasswd, &nSessionID);
	if (ids) return ids;

	if (NULL==SESSION(nSessionID).pNonFrameBindData)
	{
		ids = InitEDBMBindDataType(&SESSION(nSessionID).pNonFrameBindData, 0, 1);
		if (ids) return ids;	//-1인 경우만 있다.
	}

	
	ids = OCI8InsertProc(&SESSION(nSessionID), 
						 szTmpUserID, 
						 szTmpPasswd, 
						 pszTableName, 
						 nColumnCount, 
						 pszAttributeName, 
						 pszAttributeData, 
						 pInDataType, 
						 bCommit);

	if (-9001==ids) 
	{
		INT   ret;
		utext msg[512];
		sb4   errcode = 0;
		
		OCIErrorGet((dvoid*)SESSION(nSessionID).hpErr, (ub4) 1, (text*)NULL, &errcode, (text*)msg, (ub4) sizeof(msg), (ub4) OCI_HTYPE_ERROR); 
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶InsertProc ▶ [OCIError=%d]\n"), errcode);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		BOOL bReconnectable = FALSE;
			bReconnectable = (3114==errcode) || (1012==errcode) || (28==errcode);
		
		if (!bReconnectable) return -8001;
		
		ret = closeSession(nSessionID, TRUE, TRUE);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶InsertProc ▶ closeSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;
		
		ret = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶InsertProc ▶ getSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;

		LoggingProcess(_T("ESL_DM"), _T("◀◀◀ ORACLE Reconnected ▶▶▶"));
		
		ids = OCI8InsertProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, 
							 pszTableName, nColumnCount, pszAttributeName, pszAttributeData, pInDataType, 
							 bCommit);

#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶InsertProc ▶ Reconnected & Retry OCI8InsertProc [Return=%d]\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
	}
	
	if (ids)
	{
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ InsertProc[Error=%d]->closeSession ◎。。。。。。。\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
		FreeAllAccessVariable(nSessionID);
		return ids;
	}
	
	FreeEDBMBindData(&SESSION(nSessionID).pNonFrameBindData  , 1);
	FreeEDBMBindData(&SESSION(nSessionID).pNonFrameNumberData, 1);

	if (bCommit) 
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else return ERR_STATUS_INVALID;	//ids = ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);

#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ InsertProc[Commit]->closeSession ◎。。。。。。。\n"));
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
	}
	else
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else return ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);
	}	

	return 0;
}

INT	 UpdateProc(
				INT		nRequestID			,
				TCHAR*  pszUserID			,
                TCHAR*  pszPasswd			,
				TCHAR*  pszTableName		,
				INT     nColumnCount		,
				TCHAR** pszAttributeName	,
				TCHAR** pszAttributeData	,
				INT*    pInDataType			,
				TCHAR*  pszPKAttributeName	,
				TCHAR*  pszTablePrimaryKey	,
				BOOL    bCommit	            ,
				BOOL	bBeginFlag
				)
{
	INT ids;
	
	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];
	
	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}
	
#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ UpdateProc->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif

	INT nSessionID;
	ids = getSession(nRequestID, SQL_SESSION_EXIST_OPEN, szTmpUserID, szTmpPasswd, &nSessionID);
	if (ids) return ids;

	if (NULL==SESSION(nSessionID).pNonFrameBindData)
	{
		ids = InitEDBMBindDataType(&SESSION(nSessionID).pNonFrameBindData, 0, 1);
		if (ids) return ids;
	}
	
	ids = OCI8UpdateProc(&SESSION(nSessionID), 
						 szTmpUserID, 
						 szTmpPasswd, 
						 pszTableName, 
						 nColumnCount, 
						 pszAttributeName, 
						 pszAttributeData, 
						 pInDataType, 
						 pszPKAttributeName, 
						 pszTablePrimaryKey, 
						 bCommit,
						 bBeginFlag);
	
	if (-9001==ids) 
	{
		INT   ret;
		utext msg[512];
		sb4   errcode = 0;
		
		OCIErrorGet((dvoid*)SESSION(nSessionID).hpErr, (ub4) 1, (text*)NULL, &errcode, (text*)msg, (ub4) sizeof(msg), (ub4) OCI_HTYPE_ERROR); 
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶UpdateProc ▶ [OCIError=%d]\n"), errcode);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		BOOL bReconnectable = FALSE;
			bReconnectable = (3114==errcode) || (1012==errcode) || (28==errcode);
		
		if (!bReconnectable) return -8001;
		
		ret = closeSession(nSessionID, TRUE, TRUE);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶UpdateProc ▶ closeSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;
		
		ret = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶UpdateProc ▶ getSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;

		LoggingProcess(_T("ESL_DM"), _T("◀◀◀ ORACLE Reconnected ▶▶▶"));
		
		ids = OCI8UpdateProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, 
							 pszTableName, nColumnCount, pszAttributeName, pszAttributeData, pInDataType, 
							 pszPKAttributeName, pszTablePrimaryKey, 
							 bCommit);

#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶UpdateProc ▶ Reconnected & Retry OCI8UpdateProc [Return=%d]\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
	}
	
	if (ids)
	{
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ UpdateProc[Error=%d]->closeSession ◎。。。。。。。\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
		FreeAllAccessVariable(nSessionID);
		return ids;
	}
	
	FreeEDBMBindData(&SESSION(nSessionID).pNonFrameBindData  , 1);
	FreeEDBMBindData(&SESSION(nSessionID).pNonFrameNumberData, 1);
	
	if (bCommit) 
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else return ERR_STATUS_INVALID;	//ids = ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);

#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ UpdateProc[Commit]->closeSession ◎。。。。。。。\n"));
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
	}
	else
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else return ERR_STATUS_INVALID;	//ids = ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);
	}

	return 0;
}

INT	 DeleteProc(
				INT		nRequestID			,
				TCHAR*	pszUserID			,
                TCHAR*	pszPasswd			,
				TCHAR*	pszTableName		,
				TCHAR*	pszPKAttributeName	,
				TCHAR*	pszTablePrimaryKey	,
				BOOL	bCommit	
				)
{
	INT ids;
	
	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];
	
	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}

#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ DeleteProc->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif

	INT nSessionID;
	ids = getSession(nRequestID, SQL_SESSION_EXIST_OPEN, szTmpUserID, szTmpPasswd, &nSessionID);
	if (ids) return ids;
	
	ids = OCI8DeleteProc(&SESSION(nSessionID), 
						 szTmpUserID, 
						 szTmpPasswd, 
						 pszTableName, 
						 pszPKAttributeName, 
						 pszTablePrimaryKey, 
						 bCommit);

	if (-9001==ids) 
	{
		INT   ret;
		utext msg[512];
		sb4   errcode = 0;
		
		OCIErrorGet((dvoid*)SESSION(nSessionID).hpErr, (ub4) 1, (text*)NULL, &errcode, (text*)msg, (ub4) sizeof(msg), (ub4) OCI_HTYPE_ERROR); 
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶DeleteProc ▶ [OCIError=%d]\n"), errcode);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		BOOL bReconnectable = FALSE;
			bReconnectable = (3114==errcode) || (1012==errcode) || (28==errcode);
		
		if (!bReconnectable) return -8001;
		
		ret = closeSession(nSessionID, TRUE, TRUE);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶DeleteProc ▶ closeSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;
		
		ret = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶DeleteProc ▶ getSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;

		LoggingProcess(_T("ESL_DM"), _T("◀◀◀ ORACLE Reconnected ▶▶▶"));
		
		ids = OCI8DeleteProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, 
							 pszTableName, pszPKAttributeName, pszTablePrimaryKey, bCommit);

#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶DeleteProc ▶ Reconnected & Retry OCI8DeleteProc [Return=%d]\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
	}
	
	if (ids)
	{
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ DeleteProc[Error=%d]->closeSession ◎。。。。。。。\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
		FreeAllAccessVariable(nSessionID);
		return ids;
	}
	
	FreeEDBMBindData(&SESSION(nSessionID).pNonFrameBindData  , 1);
	FreeEDBMBindData(&SESSION(nSessionID).pNonFrameNumberData, 1);
		
	if (bCommit) 
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else return ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ DeleteProc[Commit]->closeSession ◎。。。。。。。\n"));
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif

		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
	}
	else
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else return ERR_STATUS_INVALID;	//ids = ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);
	}

	return 0;
}

INT	 NextNumberProc(
					INT		nRequestID		,
					TCHAR*	pszUserID		,
					TCHAR*	pszPasswd		,
					TCHAR*	pszSQL			,
					TCHAR*	pszNumberAlias	,
					TCHAR*	pszNumber		,	
					BOOL	bCommit
					)
{
	INT ids;
	
	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];
	
	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}
	
#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ NextNumberProc->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif
	
	INT nSessionID;
	ids = getSession(nRequestID, SQL_SESSION_EXIST_OPEN, szTmpUserID, szTmpPasswd, &nSessionID);
	if (ids) return ids;

	ids = OCI8NextNumberProc(&SESSION(nSessionID), 
							 szTmpUserID, 
							 szTmpPasswd, 
							 pszSQL, 
							 pszNumberAlias, 
							 pszNumber);
	
	if (-9001==ids) 
	{
		INT   ret;
		utext msg[512];
		sb4   errcode = 0;
		
		OCIErrorGet((dvoid*)SESSION(nSessionID).hpErr, (ub4) 1, (text*)NULL, &errcode, (text*)msg, (ub4) sizeof(msg), (ub4) OCI_HTYPE_ERROR); 
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶NextNumberProc ▶ [OCIError=%d]\n"), errcode);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		BOOL bReconnectable = FALSE;
			bReconnectable = (3114==errcode) || (1012==errcode) || (28==errcode);
		
		if (!bReconnectable) return -8001;
		
		ret = closeSession(nSessionID, TRUE, TRUE);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶NextNumberProc ▶ closeSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;
		
		ret = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶NextNumberProc ▶ getSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;

		LoggingProcess(_T("ESL_DM"), _T("◀◀◀ ORACLE Reconnected ▶▶▶"));
		
		ids = OCI8NextNumberProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, 
								 pszSQL, pszNumberAlias, pszNumber);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶NextNumberProc ▶ Reconnected & Retry OCI8NextNumberProc [Return=%d]\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
	}

	if (ids)
	{
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ NextNumberProc[Error=%d]->closeSession ◎。。。。。。。\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
		FreeAllAccessVariable(nSessionID);
		return ids;
	}

	FreeEDBMBindData(&SESSION(nSessionID).pNonFrameNumberData, 1);

	if (bCommit) 
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else return ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ NextNumberProc[Commit]->closeSession ◎。。。。。。。\n"));
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
	}
	else
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else return ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);
	}

	return 0;
}

INT	 ExecSQLProc(
				 INT	nRequestID		,
				 TCHAR*	pszUserID		,
				 TCHAR*	pszPasswd		,
				 TCHAR*	pszSQL			,
				 BOOL	bCommit
				 )
{
	INT ids;
	
	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];
	
	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}

#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ ExecSQLProc->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif
		
	INT nSessionID;
	ids = getSession(nRequestID, SQL_SESSION_EXIST_OPEN, szTmpUserID, szTmpPasswd, &nSessionID);
	if (ids) return ids;

	ids = OCI8ExecSQLProc(&SESSION(nSessionID), 
						  szTmpUserID, 
						  szTmpPasswd, 
						  pszSQL, 
						  bCommit);
	
	if (-9001==ids) 
	{
		INT   ret;
		utext msg[512];
		sb4   errcode = 0;
		
		OCIErrorGet((dvoid*)SESSION(nSessionID).hpErr, (ub4) 1, (text*)NULL, &errcode, (text*)msg, (ub4) sizeof(msg), (ub4) OCI_HTYPE_ERROR); 
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶ExecSQLProc ▶ [OCIError=%d]\n"), errcode);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		BOOL bReconnectable = FALSE;
			bReconnectable = (3114==errcode) || (1012==errcode) || (28==errcode);
		
		if (!bReconnectable) return -8001;
		
		ret = closeSession(nSessionID, TRUE, TRUE);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶ExecSQLProc ▶ closeSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;
		
		ret = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶ExecSQLProc ▶ getSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;

		LoggingProcess(_T("ESL_DM"), _T("◀◀◀ ORACLE Reconnected ▶▶▶"));
		
		ids = OCI8ExecSQLProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, pszSQL, bCommit);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶ExecSQLProc ▶ Reconnected & Retry OCI8ExecSQLProc [Return=%d]\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
	}

	if (ids)
	{
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ ExecSQLProc[Error=%d]->closeSession ◎。。。。。。。\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif

		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
		FreeAllAccessVariable(nSessionID);
		return ids;
	}
	
	FreeEDBMBindData(&SESSION(nSessionID).pNonFrameBindData  , 1);
	FreeEDBMBindData(&SESSION(nSessionID).pNonFrameNumberData, 1);
	
	if (bCommit) 
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else return ERR_STATUS_INVALID;	//ids = ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);

#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ ExecSQLProc[Commit]->closeSession ◎。。。。。。。\n"));
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
	}
	else
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else return ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);
	}
	
	return 0;
}

INT UpdateLobProc(
				  INT		nRequestID			,
				  TCHAR*	pszUserID			,
				  TCHAR*	pszPasswd			,
				  TCHAR*	pszTableName		,
				  TCHAR*	pszAttributeName	,
				  VOID*		pszLobData			,
				  INT		nLobLen				,
				  TCHAR*	pszPKAttributeName	,
				  TCHAR*	pszTablePrimaryKey	,
				  BOOL		bCommit		
				  )
{
	INT ids;
	
	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];
	
	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}

#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ UpdateLobProc->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif


	INT nSessionID;
	ids = getSession(nRequestID, SQL_SESSION_EXIST_OPEN, szTmpUserID, szTmpPasswd, &nSessionID);
	if (ids) return ids;
	
	ids = OCI8UpdateLobProc(&SESSION(nSessionID), 
							szTmpUserID, 
							szTmpPasswd, 
							pszTableName, 
							pszAttributeName, 
							pszLobData, 
							nLobLen, 
							pszPKAttributeName, 
							pszTablePrimaryKey, 
							bCommit);
	
	if (-9001==ids) 
	{
		INT   ret;
		utext msg[512];
		sb4   errcode = 0;
		
		OCIErrorGet((dvoid*)SESSION(nSessionID).hpErr, (ub4) 1, (text*)NULL, &errcode, (text*)msg, (ub4) sizeof(msg), (ub4) OCI_HTYPE_ERROR); 
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶UpdateLobProc ▶ [OCIError=%d]\n"), errcode);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		BOOL bReconnectable = FALSE;
			bReconnectable = (3114==errcode) || (1012==errcode) || (28==errcode);
		
		if (!bReconnectable) return -8001;
		
		ret = closeSession(nSessionID, TRUE, TRUE);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶UpdateLobProc ▶ closeSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;
		
		ret = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶UpdateLobProc ▶ getSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;

		LoggingProcess(_T("ESL_DM"), _T("◀◀◀ ORACLE Reconnected ▶▶▶"));
		
		ids = OCI8UpdateLobProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, 
								pszTableName, pszAttributeName, pszLobData, nLobLen, 
								pszPKAttributeName, pszTablePrimaryKey, 
								bCommit);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶UpdateLobProc ▶ Reconnected & Retry OCI8UpdateLobProc [Return=%d]\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
	}
	
	if (ids)
	{
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ UpdateLobProc[Error=%d]->closeSession ◎。。。。。。。\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
		FreeAllAccessVariable(nSessionID);
		return ids;
	}
	
	if (bCommit) 
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else return ERR_STATUS_INVALID;	//ids = ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);

#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ UpdateLobProc[Commit]->closeSession ◎。。。。。。。\n"));
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
	}
	else
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else return ERR_STATUS_INVALID;	//ids = ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);
	}

	return 0;
}

INT UnSelectProcWithoutBeginEnd(
								INT		nRequestID	,
								TCHAR*	pszUserID	,
								TCHAR*	pszPasswd	,
								TCHAR*	pszSQL		,
								BOOL	bCommit		
								)
{
	INT ids;
	
	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];
	
	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}
	
#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ UnSelectProcWithoutBeginEnd->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif

	INT nSessionID;
	ids = getSession(nRequestID, SQL_SESSION_EXIST_OPEN, szTmpUserID, szTmpPasswd, &nSessionID);
	if (ids) return ids;

	ids = OCI8UnSelectProcWithoutBeginEnd(&SESSION(nSessionID), 
										  szTmpUserID, 
										  szTmpPasswd, 
										  pszSQL, 
										  bCommit);
	
	if (-9001==ids) 
	{
		INT   ret;
		utext msg[512];
		sb4   errcode = 0;
		
		OCIErrorGet((dvoid*)SESSION(nSessionID).hpErr, (ub4) 1, (text*)NULL, &errcode, (text*)msg, (ub4) sizeof(msg), (ub4) OCI_HTYPE_ERROR); 
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶UnSelectProcWithoutBeginEnd ▶ [OCIError=%d]\n"), errcode);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		BOOL bReconnectable = FALSE;
			bReconnectable = (3114==errcode) || (1012==errcode) || (28==errcode);
		
		if (!bReconnectable) return -8001;
		
		ret = closeSession(nSessionID, TRUE, TRUE);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶UnSelectProcWithoutBeginEnd ▶ closeSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;
		
		ret = getSession(nRequestID, SQL_SESSION_NEW, szTmpUserID, szTmpPasswd, &nSessionID);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶UnSelectProcWithoutBeginEnd ▶ getSession for RECONNECTION [Return=%d]\n"), ret);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		if (ret) return ret;

		LoggingProcess(_T("ESL_DM"), _T("◀◀◀ ORACLE Reconnected ▶▶▶"));
		
		ids = OCI8UnSelectProcWithoutBeginEnd(&SESSION(nSessionID), szTmpUserID, szTmpPasswd, 
											  pszSQL, bCommit);
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶UnSelectProcWithoutBeginEnd ▶ Reconnected & Retry OCI8UnSelectProcWithoutBeginEnd [Return=%d]\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
			LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
	}
	
	if (ids)
	{
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ UnSelectProcWithoutBeginEnd[Error=%d]->closeSession ◎。。。。。。。\n"), ids);
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
		FreeAllAccessVariable(nSessionID);
		return ids;
	}
	
	if (bCommit) 
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else return ERR_STATUS_INVALID;	//ids = ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);
		
#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ UnSelectProcWithoutBeginEnd[Commit]->closeSession ◎。。。。。。。\n"));
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif
		
		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ids) return ids;
	}
	else
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else return ERR_STATUS_INVALID;	//ids = ERR_STATUS_INVALID;
		SemaphoreUnLock(hSemaphore);
	}		

	return 0;
}

INT	 CommitProc(
				INT		nRequestID	,
				TCHAR*	pszUserID	,
				TCHAR*	pszPasswd
				)
{
	INT ids;
	
	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];
	
	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}

	INT nSessionID = getSessionIDOfRequest(nRequestID);

	if (ERR_REQUEST_NOT_EXIST==nSessionID)	
	{
		return 0;
	}

	ids = OCI8CommitProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd);
	if (ids)
	{
		FreeAllAccessVariable(nSessionID);		
	}

#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("□□□□□□□□□ □ CommitProc->closeSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif
	
	INT rc = closeSession(nSessionID, FALSE, TRUE);
	
	if (ids) return -1;
	if (ERR_SESSION_USING != rc && rc) return rc;
	
	return 0;
}

INT	 RollbackProc(
				  INT		nRequestID	,	
				  TCHAR*	pszUserID	,
				  TCHAR*	pszPasswd
				  )
{
	INT ids;
	
	TCHAR szTmpUserID[64];
	TCHAR szTmpPasswd[64];
	
	if (g_szUserID[0] != _T('\0') && g_szPasswd[0] != _T('\0'))
	{
		_tcscpy(szTmpUserID, g_szUserID);
		_tcscpy(szTmpPasswd, g_szPasswd);
	}
	else
	{
		_tcscpy(szTmpUserID, pszUserID);
		_tcscpy(szTmpPasswd, pszPasswd);
	}
	
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID)	
	{
		return 0;
	}
	
	ids = OCI8RollbackProc(&SESSION(nSessionID), szTmpUserID, szTmpPasswd);
	if (ids)
	{
		FreeAllAccessVariable(nSessionID);
		return ids;
	}

#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("□□□□□□□□□ □ RollbackProc->closeSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif

	ids = closeSession(nSessionID, FALSE, TRUE);
	if (ERR_SESSION_USING != ids && ids) return ids;

	
	return 0;
}


INT	 StartFrameProc(
					INT		nRequestID	,
					TCHAR*	pszUserID	,
					TCHAR*	pszPasswd
					)
{
	INT ids;

#ifdef _DEBUG_DEEP_LOG_
	{
		CString sTemp;
		sTemp.Format(_T("■■■■■■■■■ ■ StartFrameProc->getSession ◎。。。。。。。\n"));
		LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
		AfxMessageBox(sTemp);
#endif
	}
#endif
	INT nSessionID;
	ids = getSession(nRequestID, SQL_SESSION_EXIST_OPEN, pszUserID, pszPasswd, &nSessionID);
	if (ids) return ids;	
	
	FreeEDBMBindData  (&SESSION(nSessionID).pFrameBindData  , SESSION(nSessionID).nFrameStreamCount);
	FreeEDBMBindData  (&SESSION(nSessionID).pFrameNumberData, SESSION(nSessionID).nFrameStreamCount);
	FreeTwoPtrVariable(&SESSION(nSessionID).pFrameStream    , SESSION(nSessionID).nFrameStreamCount);
	
	SESSION(nSessionID).nOldFrameStreamCount = 0;
	SESSION(nSessionID).nFrameStreamCount    = 0;

	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else return ERR_STATUS_INVALID;	//ids = ERR_STATUS_INVALID;
		
		SemaphoreUnLock(hSemaphore);
	}

	return 0;
}

INT	 SendFrameProc(
				   INT						nRequestID	,
				   TCHAR*					pszUserID	,
				   TCHAR*					pszPasswd	,
				   EDBM_BIND_DATA_TYPE**	pNumberData	
				   )
{
	INT ids;

	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID)	  return ERR_REQUEST_NOT_EXIST;

	if (pNumberData != NULL)
	{
		InitEDBMBindDataType(pNumberData, 0, 1);
	}
	
	
	for (INT i = 0; i < SESSION(nSessionID).nFrameStreamCount; i++)
	{
		if (	NULL==  SESSION(nSessionID).pFrameStream[i] 
			||	0   ==*(SESSION(nSessionID).pFrameStream[i])	) 
		{
			break;
		}
		
		ids = UnSelectProc(nRequestID,
						   pszUserID, 
						   pszPasswd, 
						   SESSION(nSessionID).pFrameStream[i], 
						   i);

		if (-9001==ids) 
		{
			INT   ret;
			utext msg[512];
			sb4   errcode = 0;
			
			OCIErrorGet((dvoid*)SESSION(nSessionID).hpErr, (ub4) 1, (text*)NULL, &errcode, (text*)msg, (ub4) sizeof(msg), (ub4) OCI_HTYPE_ERROR); 
			
#ifdef _DEBUG_DEEP_LOG_
			{
				CString sTemp;
				sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SendFrameProc ▶ [OCIError=%d]\n"), errcode);
				LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
				LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
				AfxMessageBox(sTemp);
#endif
			}
#endif
			
			BOOL bReconnectable = FALSE;
			bReconnectable = (3114==errcode) || (1012==errcode) || (28==errcode);
			
			if (!bReconnectable) return -8001;
			
			ret = closeSession(nSessionID, TRUE, TRUE);
#ifdef _DEBUG_DEEP_LOG_
			{
				CString sTemp;
				sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SendFrameProc ▶ closeSession for RECONNECTION [Return=%d]\n"), ret);
				LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
				LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
				AfxMessageBox(sTemp);
#endif
			}
#endif
			if (ret) return ret;
			
			ret = getSession(nRequestID, SQL_SESSION_NEW, pszUserID, pszPasswd, &nSessionID);
#ifdef _DEBUG_DEEP_LOG_
			{
				CString sTemp;
				sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SendFrameProc ▶ getSession for RECONNECTION [Return=%d]\n"), ret);
				LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
				LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
				AfxMessageBox(sTemp);
#endif
			}
#endif
			if (ret) return ret;
			
			LoggingProcess(_T("ESL_DM"), _T("◀◀◀ ORACLE Reconnected ▶▶▶"));   

			ids = UnSelectProc(nRequestID, pszUserID,  pszPasswd, 
				               SESSION(nSessionID).pFrameStream[i], i);
			
#ifdef _DEBUG_DEEP_LOG_
			{
				CString sTemp;
				sTemp.Format(_T("▶▶▶▶▶▶▶▶▶▶▶SendFrameProc ▶ Reconnected & Retry UnSelectProc [Return=%d]\n"), ids);
				LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
				LoggingProcess(_T("ESL_DM"),		 sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
				AfxMessageBox(sTemp);
#endif
			}
#endif
		
		}

		if (ids)
		{
			if (0 > RollbackProc(nRequestID, pszUserID, pszPasswd)) return -1;
			if (0 > EndFrameProc(nRequestID)) return -2;
			FreeAllAccessVariable(nSessionID);
			return -3;
		}
		
		if (SESSION(nSessionID).pFrameNumberData != NULL)
		{
			
			for (INT j = 0; j < SESSION(nSessionID).pFrameNumberData[i].nCount; j++)
			{
				ids = AddEDBMBindDataByString(pNumberData[0],
											  SESSION(nSessionID).pFrameNumberData[i].nDataType[j],
											  SESSION(nSessionID).pFrameNumberData[i].pAlias[j].pStringData,
											  SESSION(nSessionID).pFrameNumberData[i].pValue[j].pStringData
											  );
				if (ids)
				{
					if (0 > RollbackProc(nRequestID, pszUserID, pszPasswd)) return -4;
					if (0 > EndFrameProc(nRequestID)) return -5;

					FreeAllAccessVariable(nSessionID);
					return -6;
				}
			}
		}
	}
	
	{
		ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		
		if (ids) 
		{
			if (0 > RollbackProc(nRequestID, pszUserID, pszPasswd)) return -7;
			if (0 > EndFrameProc(nRequestID)) return -8;
			FreeAllAccessVariable(nSessionID);
			return ids;
		}
		
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_USING;
		else return ERR_STATUS_INVALID;
		
		SemaphoreUnLock(hSemaphore);

	}

	return 0;
}

INT	 EndFrameProc	(INT	nRequestID)
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID)	  return ERR_REQUEST_NOT_EXIST;
	
	FreeEDBMBindData  (&SESSION(nSessionID).pFrameBindData  , SESSION(nSessionID).nOldFrameStreamCount);
	FreeEDBMBindData  (&SESSION(nSessionID).pFrameNumberData, SESSION(nSessionID).nOldFrameStreamCount);
	FreeTwoPtrVariable(&SESSION(nSessionID).pFrameStream    , SESSION(nSessionID).nFrameStreamCount);
	
	SESSION(nSessionID).nOldFrameStreamCount = 0;
	SESSION(nSessionID).nFrameStreamCount    = 0;

	{
		INT ids = SemaphoreOpenLock(SEMAPHORE_NAME, 1, &hSemaphore, WAIT_INFINITE);
		if (ids) return ids;
		
		if      (SESSION_OPENED == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else if (SESSION_USING  == STATUS(nSessionID))	STATUS(nSessionID) = SESSION_OPENED;
		else return ERR_STATUS_INVALID;	//ids = ERR_STATUS_INVALID;
		
		SemaphoreUnLock(hSemaphore);

#ifdef _DEBUG_DEEP_LOG_
		{
			CString sTemp;
			sTemp.Format(_T("□□□□□□□□□ □ EndFrameProc->closeSession ◎。。。。。。。\n"));
			LoggingProcess(_T("OCIMGR_SESSION"), sTemp.GetBuffer(0));
#ifdef _DEBUG_DEEP_AFXMESSAGE_
			AfxMessageBox(sTemp);
#endif
		}
#endif

		ids = closeSession(nSessionID, FALSE, TRUE);
		if (ERR_SESSION_USING != ids && ids) return ids;
	}
	return 0;
}

INT	 MakeInsertFrameProc(
						 INT		nRequestID			,
						 TCHAR*		pszTableName		,
						 INT		nColumnCount		,
						 TCHAR**	pszAttributeName	,
						 TCHAR**	pszAttributeData	,
						 INT*		pInDataType
						 )
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	
	CAutoFreeFrame autoFree(nRequestID, nSessionID);
	
	{
		INT nAllocedFrameCount = SESSION(nSessionID).nOldFrameStreamCount;
		INT nReallocFrameCount = (NULL==SESSION(nSessionID).pFrameBindData) ? 1 : SESSION(nSessionID).nFrameStreamCount;
		if (nAllocedFrameCount<nReallocFrameCount)
		{
			TEST_ZERO(InitEDBMBindDataType(&SESSION(nSessionID).pFrameBindData, nAllocedFrameCount, nReallocFrameCount), -1);
		}
	}
	{
		INT nAllocedFrameCount = SESSION(nSessionID).nOldFrameStreamCount;
		INT nReallocFrameCount = (NULL==SESSION(nSessionID).pFrameNumberData) ? 1 : SESSION(nSessionID).nFrameStreamCount;
		if (NULL==SESSION(nSessionID).pFrameNumberData) SESSION(nSessionID).nOldFrameStreamCount = 1;
		if (nAllocedFrameCount<nReallocFrameCount)
		{
			TEST_ZERO(InitEDBMBindDataType(&SESSION(nSessionID).pFrameNumberData, nAllocedFrameCount, nReallocFrameCount), -2);
		}
	}

	CString sSQLStatement;
	TEST_ZERO(MakeInsertSQL(pszTableName, 
							nColumnCount, 
							pszAttributeName, 
							pszAttributeData, 
							pInDataType, 
							sSQLStatement, 
							&SESSION(nSessionID).pFrameBindData[SESSION(nSessionID).nOldFrameStreamCount - 1]), -3);

	TEST_ZERO(AddSQLStatementToFrame(nSessionID, sSQLStatement.GetBuffer(0)), -4);
	
	return 0;
}

INT	 MakeUpdateFrameProc(
						 INT		nRequestID			,
						 TCHAR*		pszTableName		,
						 INT		nColumnCount		,
						 TCHAR**	pszAttributeName	,
						 TCHAR**	pszAttributeData	,
						 INT*		pInDataType			,
						 TCHAR*		pszPKAttributeName	,
						 TCHAR*		pszTablePrimaryKey
						 )
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	
	CAutoFreeFrame autoFree(nRequestID, nSessionID);
	
	{
		INT nAllocedFrameCount = SESSION(nSessionID).nOldFrameStreamCount;
		INT nReallocFrameCount = (NULL==SESSION(nSessionID).pFrameBindData) ? 1 : SESSION(nSessionID).nFrameStreamCount;
		
		if (nAllocedFrameCount<nReallocFrameCount)
		{
			TEST_ZERO(InitEDBMBindDataType(&SESSION(nSessionID).pFrameBindData, nAllocedFrameCount, nReallocFrameCount), -1);
		}
	}
	{
		INT nAllocedFrameCount = SESSION(nSessionID).nOldFrameStreamCount;
		INT nReallocFrameCount = (NULL==SESSION(nSessionID).pFrameNumberData) ? 1 : SESSION(nSessionID).nFrameStreamCount;
		if (NULL==SESSION(nSessionID).pFrameNumberData) SESSION(nSessionID).nOldFrameStreamCount = 1;
		if (nAllocedFrameCount<nReallocFrameCount)
		{
			TEST_ZERO(InitEDBMBindDataType(&SESSION(nSessionID).pFrameNumberData, nAllocedFrameCount, nReallocFrameCount), -2);
		}
	}

	CString sSQLStatement;

	TEST_ZERO(MakeUpdateSQL(pszTableName, 
							nColumnCount, 
							pszAttributeName, 
							pszAttributeData, 
							pInDataType, 
							pszPKAttributeName, 
							pszTablePrimaryKey, 
							sSQLStatement, 
							&SESSION(nSessionID).pFrameBindData[SESSION(nSessionID).nOldFrameStreamCount - 1]), -3);
	
	TEST_ZERO(AddSQLStatementToFrame(nSessionID, sSQLStatement.GetBuffer(0)), -4);
	
	return 0;
}

INT	 MakeDeleteFrameProc(
						 INT	nRequestID			,	
						 TCHAR* pszTableName		,
						 TCHAR* pszPKAttributeName	,
						 TCHAR* pszTablePrimaryKey
						 )
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	
	CAutoFreeFrame autoFree(nRequestID, nSessionID);

	CString sSQL;
	
	TEST_TRUE((StringLength2(pszTableName)       > 0 && 
			   StringLength2(pszPKAttributeName) > 0 && 
			   StringLength2(pszTablePrimaryKey) > 0   ), -1);
	
	{
		INT nAllocedFrameCount = SESSION(nSessionID).nOldFrameStreamCount;
		INT nReallocFrameCount = (NULL==SESSION(nSessionID).pFrameBindData) ? 1 : SESSION(nSessionID).nFrameStreamCount;

		if (nAllocedFrameCount<nReallocFrameCount)
		{
			TEST_ZERO(InitEDBMBindDataType(&SESSION(nSessionID).pFrameBindData, nAllocedFrameCount, nReallocFrameCount), -1);
		}
	}
	{
		INT nAllocedFrameCount = SESSION(nSessionID).nOldFrameStreamCount;
		INT nReallocFrameCount = (NULL==SESSION(nSessionID).pFrameNumberData) ? 1 : SESSION(nSessionID).nFrameStreamCount;
		if (NULL==SESSION(nSessionID).pFrameNumberData) SESSION(nSessionID).nOldFrameStreamCount = 1;
		if (nAllocedFrameCount<nReallocFrameCount)
		{
			TEST_ZERO(InitEDBMBindDataType(&SESSION(nSessionID).pFrameNumberData, nAllocedFrameCount, nReallocFrameCount), -2);
		}
	}

	sSQL.Format(_T("DELETE FROM %s WHERE %s = %s;"), pszTableName, pszPKAttributeName, pszTablePrimaryKey);
	
	TEST_ZERO(AddSQLStatementToFrame(nSessionID, sSQL.GetBuffer(0)), -2);
	
	return 0;
}
























//--------------------------------------------------------------------------o
// 함수명 : MakeExecSQLFrameProc
// 기  능 : SQL Frame문을 추가한다
//--------------------------------------------------------------------------
// Return : ERR_REQUEST_NOT_EXIST		- 세션Request ID에 대한 세션을 찾을 수 없다.
//			-1							- pszSQL이 설정되지 않았을 경우
//			AddSQLStatementToFrame(<0)	- 오류시
//			0							- 정상 종료
//--------------------------------------------------------------------------o
INT	 MakeExecSQLFrameProc(
						  INT		nRequestID,		// [I N] 세션Request ID		[#######]	
						  TCHAR*	pszSQL			// [I N] SQL
						  )
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	//if (SESSION_CLOSED == STATUS(nSessionID)) return ERR_SESSION_CLOSED;
	
	CAutoFreeFrame autoFree(nRequestID, nSessionID);

	// Check validation
	TEST_TRUE((StringLength2(pszSQL)>0), -1);
	
	/*2004-0407 Frame 처리 수정-박길주+++++++++++++++++++++++++++++*/
	{
		INT nAllocedFrameCount = SESSION(nSessionID).nOldFrameStreamCount;
		INT nReallocFrameCount = (NULL==SESSION(nSessionID).pFrameBindData) ? 1 : SESSION(nSessionID).nFrameStreamCount;
		//if (NULL==SESSION(nSessionID).pFrameBindData) SESSION(nSessionID).nOldFrameStreamCount = 1;
		if (nAllocedFrameCount<nReallocFrameCount)
		{
			TEST_ZERO(InitEDBMBindDataType(&SESSION(nSessionID).pFrameBindData, nAllocedFrameCount, nReallocFrameCount), -1);
		}
	}
	{
		INT nAllocedFrameCount = SESSION(nSessionID).nOldFrameStreamCount;
		INT nReallocFrameCount = (NULL==SESSION(nSessionID).pFrameNumberData) ? 1 : SESSION(nSessionID).nFrameStreamCount;
		if (NULL==SESSION(nSessionID).pFrameNumberData) SESSION(nSessionID).nOldFrameStreamCount = 1;
		if (nAllocedFrameCount<nReallocFrameCount)
		{
			TEST_ZERO(InitEDBMBindDataType(&SESSION(nSessionID).pFrameNumberData, nAllocedFrameCount, nReallocFrameCount), -2);
		}
	}
	/*2004-0407 Frame 처리 수정-박길주-----------------------------*/

	// Add to frame stream
	TEST_ZERO(AddSQLStatementToFrame(nSessionID, pszSQL), -2);
	
	return 0;
}


//--------------------------------------------------------------------------o
// 함수명 : MakeCommitFrameProc
// 기  능 : Commit Frame문을 추가한다
//--------------------------------------------------------------------------
// Return : ERR_REQUEST_NOT_EXIST		- 세션Request ID에 대한 세션을 찾을 수 없다.
//			-1							- AddSQLStatementToFrame 오류시
//			0							- 정상 종료
//--------------------------------------------------------------------------o
INT	 MakeCommitFrameProc(INT nRequestID)			// [I N] 세션Request ID		[#######]	
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	//if (SESSION_CLOSED == STATUS(nSessionID)) return ERR_SESSION_CLOSED;
	
	CAutoFreeFrame autoFree(nRequestID, nSessionID);

//	/*1031 Frame 처리 수정-박길주+++++++++++++++++++++++++++++*/
//	{
//		if (SESSION(nSessionID).nOldFrameStreamCount == 0)
//		{
//			ids = InitEDBMBindDataType(&SESSION(nSessionID).pFrameBindData, 0, 1);
//			//SESSION(nSessionID).nOldFrameStreamCount = 1;
//		}
//		else
//		{
//			ids = InitEDBMBindDataType(&SESSION(nSessionID).pFrameBindData,
//										SESSION(nSessionID).nOldFrameStreamCount, 
//										SESSION(nSessionID).nFrameStreamCount);
//		}
//		if (ids)
//		{
//			//2003-12-03 [박길주] ========================================
//			//2003-12-03 [박길주] ++++++++++++++++++++++++++++++++++++++++
//			if (0 > EndFrameProc(nRequestID)) return -1;
//			//2003-12-03 [박길주] ----------------------------------------
//			FreeAllAccessVariable(nSessionID);
//			return -2;
//		}
//	}
//	{
//		if (SESSION(nSessionID).nOldFrameStreamCount == 0)
//		{
//			ids = InitEDBMBindDataType(&SESSION(nSessionID).pFrameNumberData, 0, 1);
//			//SESSION(nSessionID).nOldFrameStreamCount = 1;
//		}
//		else
//		{
//			ids = InitEDBMBindDataType(&SESSION(nSessionID).pFrameNumberData, 
//										SESSION(nSessionID).nOldFrameStreamCount, 
//										SESSION(nSessionID).nFrameStreamCount);
//		}
//		if (ids)
//		{
//			//2003-12-03 [박길주] ========================================
//			//2003-12-03 [박길주] ++++++++++++++++++++++++++++++++++++++++
//			if (0 > EndFrameProc(nRequestID)) return -3;
//			//2003-12-03 [박길주] ----------------------------------------
//			FreeAllAccessVariable(nSessionID);
//			return -4;
//		}
//	}
//	/*1031 Frame 처리 수정-박길주-----------------------------*/

	/*2004-0407 Frame 처리 수정-박길주+++++++++++++++++++++++++++++*/
	{
		INT nAllocedFrameCount = SESSION(nSessionID).nOldFrameStreamCount;
		INT nReallocFrameCount = (NULL==SESSION(nSessionID).pFrameBindData) ? 1 : SESSION(nSessionID).nFrameStreamCount;
		//if (NULL==SESSION(nSessionID).pFrameBindData) SESSION(nSessionID).nOldFrameStreamCount = 1;
		if (nAllocedFrameCount<nReallocFrameCount)
		{
			TEST_ZERO(InitEDBMBindDataType(&SESSION(nSessionID).pFrameBindData, nAllocedFrameCount, nReallocFrameCount), -1);
		}
	}
	{
		INT nAllocedFrameCount = SESSION(nSessionID).nOldFrameStreamCount;
		INT nReallocFrameCount = (NULL==SESSION(nSessionID).pFrameNumberData) ? 1 : SESSION(nSessionID).nFrameStreamCount;
		if (NULL==SESSION(nSessionID).pFrameNumberData) SESSION(nSessionID).nOldFrameStreamCount = 1;
		if (nAllocedFrameCount<nReallocFrameCount)
		{
			TEST_ZERO(InitEDBMBindDataType(&SESSION(nSessionID).pFrameNumberData, nAllocedFrameCount, nReallocFrameCount), -2);
		}
	}
	/*2004-0407 Frame 처리 수정-박길주-----------------------------*/


	TEST_ZERO(AddSQLStatementToFrame(nSessionID, _T("COMMIT;")), -1);

	//###########################################
	//STATUS(nSessionID) = SESSION_OPENED;
	//ids = closeSession(nSessionID, TRUE);
	//if (ids) return ids;

	return 0;
}





//--------------------------------------------------------------------------o
// 함수명 : MakeNextNumberFrameProc
// 기  능 : 번호구하기용 Frame문을 추가한다
//--------------------------------------------------------------------------
// Return : ERR_REQUEST_NOT_EXIST	- 세션Request ID에 대한 세션을 찾을 수 없다.
//			-1						- pszSQL이 설정되지 않았을 경우
//			-2						- AddSQLStatementToFrame() 오류시
//			-3						- InitEDBMBindDataType() 오류시
//			-4						- pszAlias[] 매개변수가 설정되지 않았을 경우
//			-5						- AddEDBMBindDataByString() 오류시
//			0						- 정상 종료
//--------------------------------------------------------------------------o
INT	 MakeNextNumberFrameProc(
							 INT		nRequestID	,	// [I N] 세션Request ID		[#######]	
							 TCHAR*		pszSQL		,	// [I N] SQL
							 TCHAR**	pszAlias	,	// [I N] SQL 문 안에 있는 ALIAS들
							 INT		nAliasCount		// [I N] 번호 ALIAS 개수
							 )
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	//if (SESSION_CLOSED == STATUS(nSessionID)) return ERR_SESSION_CLOSED;
	
	CAutoFreeFrame autoFree(nRequestID, nSessionID);

	// Check validation
	TEST_TRUE((StringLength2(pszSQL)>0 && nAliasCount>0), -1);
	
    // Add to frame stream
	TEST_ZERO(AddSQLStatementToFrame(nSessionID, pszSQL), -2);
	
	// Add alias to number structure
	if (nAliasCount > 0)
	{
		/*2004-0407 Frame 처리 수정-박길주+++++++++++++++++++++++++++++*/
		{
			INT nAllocedFrameCount = SESSION(nSessionID).nOldFrameStreamCount;
			INT nReallocFrameCount = (NULL==SESSION(nSessionID).pFrameNumberData) ? 1 : SESSION(nSessionID).nFrameStreamCount;
			if (NULL==SESSION(nSessionID).pFrameNumberData) SESSION(nSessionID).nOldFrameStreamCount = 1;
			if (nAllocedFrameCount<nReallocFrameCount)
			{
				TEST_ZERO(InitEDBMBindDataType(&SESSION(nSessionID).pFrameNumberData, nAllocedFrameCount, nReallocFrameCount), -3);
			}
		}
		/*2004-0407 Frame 처리 수정-박길주-----------------------------*/

		for (INT i = 0; i < nAliasCount; i++)
		{
			TEST_TRUE((StringLength2(pszAlias[i]) > 0), -4);
			TEST_ZERO(AddEDBMBindDataByString(&SESSION(nSessionID).pFrameNumberData[SESSION(nSessionID).nFrameStreamCount - 1], 
											  STRING, 
											  pszAlias[i], 
											  NULL), -5);
		}
	}
	
	return 0;
}







//------------------------------------------------//----------------------------------------o
// Initialization / Deinitialization / Set & Get Properties
//------------------------------------------------//----------------------------------------o







//--------------------------------------------------------------------------o
// 함수명 : FreeOCIHandle
// 기  능 : OCI Statement Handle Free
//--------------------------------------------------------------------------
// Return : 0						- 정상 종료
//--------------------------------------------------------------------------o
INT  FreeOCIHandle(ORA_SELECT_STRU* pSelect)
{
	OCI8StmtHandleFree(pSelect->hpStmt);
	return 0;
}


//--------------------------------------------------------------------------
// 원종 추가
// 함수명 : GetConnectedDB 
// 기  능 : DB가 Connect 되어 있는지 아닌지 Flag를 리턴한다.(TRUE/FALSE)
//--------------------------------------------------------------------------
/*
BOOL  GetConnectedDB(INT nRequestID)
{
	INT nSessionStatus = SESSION_CLOSED;
	for (INT i=0 ; i<MAX_SESSION_CNT ; i++)
	{
		if (g_MainAccess[i].nRequestID == nRequestID)
		{
			nSessionStatus = g_MainAccess[i].nStatus;
			break;
		}
	}
	
	//return (SESSION_OPENED == nSessionStatus || SESSION_ERROR == nSessionStatus) ? TRUE : FALSE;
	return (SESSION_OPENED == nSessionStatus || SESSION_ERROR == nSessionStatus) ? TRUE : FALSE;
}
*/

//--------------------------------------------------------------------------o
// 함수명 : IsConnectedOfRequest
// 기  능 : 세션Request ID에 해당하는 세션이 SESSION_CLOSE 상태면 FALSE, 그렇지 않으면(연결되어 있으면) TRUE
//--------------------------------------------------------------------------
// Return : ERR_REQUEST_NOT_EXIST	- 세션Request ID에 대한 세션을 찾을 수 없다.
//			TRUE	- SESSION_OPENED, SESSION_FETCHING, SESSION_USING 상태(연결된 상태)
//			FALSE	- SESSION_CLOSED 상태
//--------------------------------------------------------------------------o
BOOL  IsConnectedOfRequest	(INT nRequestID)	// [I N] 세션Request ID		[#######]	
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	return (SESSION_CLOSED) ? FALSE : TRUE;
}



//--------------------------------------------------------------------------o
// 함수명 : SetFetchCount
// 기  능 : 세션Request ID에 해당하는 세션에 
//			한번에 SELECT를 수행하여 가져올 수 있는 최대 Record 개수를 설정한다
//--------------------------------------------------------------------------
// Return : ERR_REQUEST_NOT_EXIST	- 세션Request ID에 대한 세션을 찾을 수 없다.
//			0						- 정상 종료
//--------------------------------------------------------------------------o
INT SetMaxFetchCount	(
						 INT nRequestID,		// [I N] 세션Request ID		[#######]	
						 INT nMaxFetchCount		// [I N] 최대 Fetch 가능 레코드 수
						 )
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	SESSION(nSessionID).nMaxFetchCount = nMaxFetchCount;
	return 0;
}

//--------------------------------------------------------------------------o
// 함수명 : GetFetchCount
// 기  능 : 세션Request ID에 해당하는 세션에 
//			한번에 SELECT를 수행하여 가져올 수 있는 최대 Record 개수를 가져온다
//--------------------------------------------------------------------------
// Return : ERR_REQUEST_NOT_EXIST	- 세션Request ID에 대한 세션을 찾을 수 없다.
//			0						- 정상 종료
//--------------------------------------------------------------------------o
INT	 GetMaxFetchCount(INT nRequestID)	// [I N] 세션Request ID		[#######]	
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	return SESSION(nSessionID).nMaxFetchCount;
}

//--------------------------------------------------------------------------o
// 함수명 : SetArrayFetchSize
// 기  능 : 세션Request ID에 해당하는 세션에 
//			한번의 OCIStmtFetch 시에 가져올 레코드 수를 설정한다.
//--------------------------------------------------------------------------
// Return : ERR_REQUEST_NOT_EXIST	- 세션Request ID에 대한 세션을 찾을 수 없다.
//			0						- 정상 종료
//--------------------------------------------------------------------------
// Author : [GJPark@eco.co.kr]
// Created: 2003/8/18 21:00
//--------------------------------------------------------------------------o
INT SetArrayFetchSize			(
								 INT nRequestID,		// [I N] 세션Request ID		[#######]	
								 INT nArrayFetchSize	// [I N] 한번의 OCIStmtFetch 시에 가져올 레코드 수
								 )
{
	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	SESSION(nSessionID).nArrayFetchSize = nArrayFetchSize;
	return 0;
}


//--------------------------------------------------------------------------o
// 함수명 : SetBindingData
// 기  능 : 바인딩 자료를 저장한다
//--------------------------------------------------------------------------
// Return : ERR_REQUEST_NOT_EXIST	- 세션Request ID에 대한 세션을 찾을 수 없다.
//			-1						- Frame이 아닌 경우 AddEDBMBindDataByString 오류시
//			-2						- Frame인      경우 AddEDBMBindDataByString 오류시
//			0						- 정상 종료
//--------------------------------------------------------------------------o
INT	 SetBindingData				(
								 INT nRequestID,		// [I N] 세션Request ID		[#######]	
								 TCHAR* pszAlias,		// [I N] 바인딩 Alias
								 TCHAR* pszData,		// [I N] 바인딩 Data 
								 BOOL bFrame			// [I N] 프레임 여부
								 )
{
	INT ids;

	INT nSessionID = getSessionIDOfRequest(nRequestID);
	if (ERR_REQUEST_NOT_EXIST==nSessionID) return ERR_REQUEST_NOT_EXIST;
	
	if (bFrame == FALSE)
	{
		ids = AddEDBMBindDataByString(&SESSION(nSessionID).pNonFrameBindData[0], STRING, pszAlias, pszData);
		if (ids) return -1;
	}
	else
	{
		ids = AddEDBMBindDataByString(&SESSION(nSessionID).pFrameBindData[SESSION(nSessionID).nFrameStreamCount - 1], STRING, pszAlias, pszData);
		if (ids) return -2;
	}
	
	return 0;
}


//--------------------------------------------------------------------------o
// 함수명 : SetLoginID
// 기  능 : 시스템에서 쓰는 테이블의 FIRST_WORK, LAST_WORK에 로그로 남길
//          로그인 ID를 저장한다
//--------------------------------------------------------------------------o
VOID SetLoginID	(TCHAR* pLoginID)	// [I N] 로그인 ID
{
	_tcscpy(g_szWorkID, pLoginID);
}



