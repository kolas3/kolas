// SPF_Work.cpp: implementation of the CSPF_Work class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SPF_Work.h"
#include "efs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSPF_Work::CSPF_Work(CESL_Mgr* pParent /* = NULL */)
{
	m_pParentMgr = pParent;
	m_bError = FALSE;

	m_strLogFilePath = _T("err_log.html");

	m_nPtrCount = 0;
}

CSPF_Work::~CSPF_Work()
{
	FreeLogList();
}

INT CSPF_Work::FreeLogList()
{
	INT nListCount = m_listLog.GetCount();
	if (nListCount == 0) return 0;
	CString *pStr;
	for (INT i = 0; i < nListCount; i++)
	{
		pStr = (CString*)m_listLog.RemoveHead();
		if (pStr) delete pStr;
	}
	m_listLog.RemoveAll();
	return nListCount;
}


//////////////////////////////////////////////////////////////////////
// CSPF_Work Operation
CString CSPF_Work::GetActivePathFromState(CString sState)
{
	return sState;
}

// 추가 함수 데이터 세팅 밎 저장
INT CSPF_Work::InitTempData()
{
	m_MapTempData.RemoveAll();
	return 0;
}

CComVariant CSPF_Work::GetTempData(CString strAlias)
{
	CComVariant Data;
	m_MapTempData.Lookup(strAlias, Data);	
		
	return Data;
}

INT	CSPF_Work::GetTempData(CString strAlias, INT &Data)
{
	Data = GetTempData(strAlias).intVal;
	return 0;
}

INT CSPF_Work::GetTempData(CString strAlias, CString &Data)
{	
	if ( GetTempData(strAlias).vt != VT_EMPTY )
	{
		Data = GetTempData(strAlias).bstrVal;
	}
	else
	{
		Data = _T("");
	}
	
	return 0;
}

INT	CSPF_Work::SetTempData(CString strAlias, CComVariant Data)
{
	m_MapTempData.SetAt(strAlias, Data);
	return 0;
}

INT CSPF_Work::InitTempPtr()
{
	m_nPtrCount = 0;

	return 0;
}

INT CSPF_Work::SetTempPtr(CString strAlias, DWORD* dwPtr)
{
	m_strDataAlias[m_nPtrCount] = strAlias;
	m_dwDataPtr[m_nPtrCount] = dwPtr;
	m_nPtrCount++;

	return 0;
}

INT CSPF_Work::GetTempPtr(CString strAlias, DWORD **dwPtr)
{
	for(INT i=0;i<32;i++) {
		if(strAlias == m_strDataAlias[i]) {
			*dwPtr = m_dwDataPtr[i];
		}
	}
	
	return 0;
}

DWORD* CSPF_Work::GetTempPtr(CString strAlias)
{
    for(INT i=0;i<32;i++) {
		if(strAlias == m_strDataAlias[i]) {
			return m_dwDataPtr[i];
		}
	}
	
	return 0;
}

INT	CSPF_Work::SPF_ErrorFeedback(CString strPath)
{
	CStdioFile file;
	if (!file.Open(m_strLogFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary ))
	{
		m_strLastError = _T("File Open Error!!");
		return -1;
	}

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}	

	CString *pStr = NULL;
	POSITION pos = m_listLog.GetHeadPosition();
	while (pos != NULL)
	{
		pStr = (CString*)m_listLog.GetNext(pos);
		if (pStr == NULL) continue;

		file.WriteString(pStr->GetBuffer(0));
	}

	file.Close();

	// TODO : Send File
	
	return 0;
}

INT	CSPF_Work::SPF_ErrMsg(INT nErrCode)
{
	return 0;
}

INT CSPF_Work::SPFExecutePath(CString strPath)
{
	m_bError = FALSE;
	FreeLogList();
	WriteBasicInformationLog(strPath);

	SetProgramPath(_T("SPF_PATH"), strPath);

	return 0;
}

INT CSPF_Work::SetStep(CString strStep)
{
	// Add Log
	CString *pStr = new CString;
	if (pStr == NULL) return -1;

	pStr->Format(_T("<LI> STEP : %s\n"), strStep);

	m_listLog.AddTail((CObject*)pStr);

	SetProgramPath(_T("SPF_STEP"), strStep);

	return 0;
}

INT CSPF_Work::WriteBasicInformationLog(CString strPath)
{
	CString strTemp;

	// 도서관 이름
	strTemp = _T("<LI>도서관 이름 : 이씨오 도서관\n");

	// ID / IP
	strTemp += _T("<LI>ID / IP : ") + m_pParentMgr->m_pInfo->USER_ID + _T("/") + m_pParentMgr->m_pInfo->LOCAL_IP + _T("\n");

	// PATH
	strTemp += _T("<LI>PATH : ") + strPath + _T("\n");

	CString *pStr = new CString;
	pStr->Format(_T("%s"), strTemp);

	m_listLog.AddTail((CObject*)pStr);
	
	return 0;
}

INT	CSPF_Work::SPF_SendFrame(CESL_DataMgr *pDM)
{
	if (pDM == NULL) return -1;

	INT ids = pDM->SendFrame();
	if (ids < 0)
	{
		CArray<CString, CString&> array;
		pDM->GetFrameLog(&array);
		INT nCount = array.GetSize();

		CString strLog = _T("<TABLE CELLSPACING=1 CELLPADDING=0 BORDER=0 bgcolor = #C8C8C8>\n");
			
		for (INT i = 0; i < nCount; i++)
		{
			strLog += _T("<TR bgcolor = white>\n\t<TD>") + array.GetAt(i) + _T("\n\t</TD>\n</TR>");
		}

		strLog += _T("</TABLE><BR>\n");

		CString *pStr = new CString;
		if (pStr == NULL) return ids;

		pStr->Format(_T("%s"), strLog);

		m_listLog.AddTail((CObject*)pStr);
	}
	return ids;
}

INT	CSPF_Work::SPF_ExecuteQuery(CESL_DataMgr *pDM, CString strQuery, INT nCommitMode /*= 1*/)
{
	if (pDM == NULL) return -1;
	if (strQuery.IsEmpty()) return -2;
	
	INT ids = pDM->ExecuteQuery(strQuery, nCommitMode);
	if (ids < 0)
	{
		CArray<CString, CString&> array;
		pDM->GetFrameLog(&array);
		INT nCount = array.GetSize();

		CString strLog = _T("<TABLE CELLSPACING=1 CELLPADDING=0 BORDER=0 bgcolor = #C8C8C8>\n");
			
		for (INT i = 0; i < nCount; i++)
		{
			strLog += _T("<TR bgcolor = white>\n\t<TD>") + array.GetAt(i) + _T("\n\t</TD>\n</TR>");
		}

		strLog += _T("</TABLE><BR>\n");

		CString *pStr = new CString;
		if (pStr == NULL) return ids;
		pStr->Format(_T("%s"), strLog);

		m_listLog.AddTail((CObject*)pStr);
	}

	return ids;
}

INT	CSPF_Work::SPF_FinishDVM()
{
	if (m_pParentMgr == NULL) return -1;
	
	INT ids = m_pParentMgr->Finish_DVM();
	if (ids < 0)
	{
		CString *pStr = new CString;
		if (pStr == NULL) return ids;

		CString strTemp;
		m_pParentMgr->GetDVMLog(strTemp);
		pStr->Format(_T("%s<BR>\n"), strTemp);

		m_listLog.AddTail((CObject*)pStr);
	}

	return ids;
}
