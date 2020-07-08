// TnsList.cpp: implementation of the CTnsList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "k2up_tnsmanager.h"
#include "TnsList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTnsList::CTnsList()
{
	LIST.RemoveAll();
}

CTnsList::~CTnsList()
{
	FreeTnsList();
}

INT CTnsList::LoadTnsnames(CString strFileName)
{
	/*
	kolas2 =
	  (DESCRIPTION = (ADDRESS_LIST =  #(ADDRESS = (PROTOCOL = TCP)(HOST = kolas2up)(PORT = 1521)) (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.1.50)(PORT = 1521))
		)
		(CONNECT_DATA =
		  (SERVICE_NAME = kolas2)
		)
	  )
	*/
	FreeTnsList();
	CString strMsg;

	CString strLine;
	CStdioFile file;
	// 오라클
	if (!file.Open(strFileName, CFile::modeRead | CFile::typeBinary))
	{
		strMsg.Format(_T("파일을 열 수 없습니다.[%s]"), strFileName);
		AfxMessageBox(strMsg);
		return -1;
	}
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}

	DWORD dwLen = file.GetLength() * sizeof(TCHAR);
	if (dwLen == 0)
	{
		file.Close();
		return 0;
	}
	TCHAR *szBuffer = new TCHAR[dwLen];
	ZeroMemory(szBuffer, dwLen);

	file.ReadHuge(szBuffer, dwLen);
	file.Close();

	
	CString strFileData;
	strFileData.Format(_T("%s"), szBuffer);

	if (szBuffer) delete [] szBuffer;

	// # 제거
	INT nFind = strFileData.Find(_T("#"));
	while (nFind >= 0)
	{
		strFileData.Delete(nFind, strFileData.Find(_T("\n"), nFind) - nFind + 1);
		nFind = strFileData.Find(_T("#"));
	}

	TCHAR ch;
	INT n = strFileData.Find(_T("("));
	INT nCount = 1;
	INT nStart = 0;

	INT nFileLen = strFileData.GetLength();

	while (TRUE)
	{
		if (nFileLen - 1 < ++n) break;
		ch = strFileData.GetAt(n);
		if (ch == '(')
			nCount++;
		else if (ch == ')')
			nCount--;

		if (nCount == 0)
		{
			CString strOneData = strFileData.Mid(nStart, n - nStart + 1);
			strOneData.TrimLeft	();
			strOneData.TrimRight();
			if (!strOneData.IsEmpty())
			{
				CTns *pTns = new CTns;
				// KEY VALUE
				CString strKeyValue = strOneData.Mid(0, strOneData.Find(_T("=")));
				strKeyValue.TrimLeft();
				strKeyValue.TrimRight();

				// HOST
				GetTnsValue(strOneData, _T("HOST"), pTns->m_strHOST);

				// PORT
				GetTnsValue(strOneData, _T("PORT"), pTns->m_strPORT);

				// SERVICE_NAME
				GetTnsValue(strOneData, _T("SERVICE_NAME"), pTns->m_strSERVICE_NAME);

				strKeyValue.TrimLeft();			strKeyValue.TrimRight();
				strKeyValue.MakeUpper();
				pTns->m_strSID = strKeyValue;
				LIST.AddTail((CObject*)pTns);

			}

			nStart = ++n;
			n = strFileData.Find(_T("("), n);
			nCount++;
			if (n < 0) break;
		}
	}
	
	return 0;
}

INT CTnsList::SaveTnsnames(CString strFileName)
{
	CopyFile(strFileName, strFileName+_T(".bak~K2UP_TnsManager"), FALSE );

	// 오라클
	CStdioFile file;
	if (!file.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		AfxMessageBox(_T("저장 실패 : 파일을 열수 없습니다."));
		return FALSE;
	}

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	CTns *pTns = NULL;

	CString strTemp;

	POSITION pos = LIST.GetHeadPosition();
	while (pos)
	{
		pTns = (CTns*)LIST.GetNext(pos);
		if (pTns == NULL) continue;
		
		strTemp.Format(_T("%s = \r\n")
					   _T("  (DESCRIPTION = \r\n")
					   _T("    (ADDRESS_LIST = \r\n")
					   _T("      (ADDRESS = (PROTOCOL = TCP)(HOST = %s)(PORT = %s)) \r\n")
					   _T("    ) \r\n")
					   _T("    (CONNECT_DATA = \r\n")
					   _T("      (SERVICE_NAME = %s)\r\n")
					   _T("    )\r\n")
					   _T("  )\r\n\r\n")
					   , pTns->m_strSID, pTns->m_strHOST, pTns->m_strPORT, pTns->m_strSERVICE_NAME);

		file.WriteString(strTemp);
	}
	file.Close();
	return 0;
}

INT CTnsList::FreeTnsList(VOID)
{
	INT nData = LIST.GetCount();
	if (nData == 0) return 0;
	CTns * pData;
	for (INT i = 0; i < nData; i++) 
	{
		pData = (CTns*)LIST.RemoveHead();
		if (pData) delete pData;
	}
	LIST.RemoveAll();
	return 0;
}

INT CTnsList::GetTnsValue(CString strTns, CString strKey, CString &strValue)
{
	if (strTns.IsEmpty() || strKey.IsEmpty()) return -1;

	INT nFind = strTns.Find(strKey);
	if (nFind > 0)
	{
		INT a = strTns.Find(_T("="), nFind);
		INT b = strTns.Find(_T(")"), a);
		
		if (a >= 0 && b >= 0)
		{
			strValue = strTns.Mid(a + 1, b - a - 1);
			strValue.TrimLeft();
			strValue.TrimRight();
		}
	}

	return 0;
}

CTns * CTnsList::FindTnsItem(CString strSID)
{
	strSID.TrimLeft();				strSID.TrimRight();
	strSID.MakeUpper();

	INT nData = LIST.GetCount();
	CTns *pData = NULL;
	POSITION pos = LIST.GetHeadPosition();
	for (INT i = 0; i < nData; i++) {
		pData = NULL;
		pData = (CTns*)LIST.GetNext(pos);
		if (pData == NULL) continue;
		if (pData->m_strSID == strSID) return pData;
	}
	return NULL;
}

CTns * CTnsList::AddTnsItem(VOID)
{
	CTns *pTns = new CTns;
	if (pTns == NULL) return NULL;

	LIST.AddTail((CObject*)pTns);
	return pTns;
}