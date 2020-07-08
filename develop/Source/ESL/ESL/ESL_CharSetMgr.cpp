// ESL_CharSetMgr.cpp: implementation of the CESL_CharSetMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESL_CharSetMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_CharSetMgr::CESL_CharSetMgr()
{
EFS_BEGIN
	FromSet = _T("");
	ToSet = _T("");
	pData = NULL;
	FreeData();
	strCharSetVersion = _T("");
EFS_END
}

CESL_CharSetMgr::~CESL_CharSetMgr()
{
EFS_BEGIN
	FreeData();
EFS_END
}

INT CESL_CharSetMgr::FreeData(VOID)
{
EFS_BEGIN
	if (pData) {
		delete[] pData;
	}
	pData = NULL;
	nData = 0;
EFS_END
	return 0;
}

INT CESL_CharSetMgr::Find(CString strFrom, CString &strTo)
{
EFS_BEGIN
	for (INT i = 0; i < nData; i++) {
		if (strFrom.Compare(pData[i].szFrom) == 0) {
			strTo.Format(_T("%s"), pData[i].szTo);
			return 0;
		}
	}
EFS_END
	return -1;
}

INT CESL_CharSetMgr::Find(TCHAR *pszFrom, TCHAR *pszTo)
{
EFS_BEGIN
	for (INT i = 0; i < nData; i++) {
		if (_tcscmp(pszFrom, pData[i].szFrom) == 0) {
			_stprintf(pszTo, _T("%s"), pData[i].szTo);
			return 0;
		}
	}
EFS_END
	return -1;
}

INT CESL_CharSetMgr::FindKoreanWordToChineseWord(CString strFrom, CString strTo[], INT &nStrTo)
{
EFS_BEGIN
	if (FromSet != _T("KOREAN_WORD") || ToSet != _T("CHINESE_WORD")) {
		nStrTo = 0;
		return -1;
	}

	INT nCount = 0;
	INT nStart = 0;
	INT nFinish = nData-1;

	for (INT i = nStart; i <= nFinish; i++) {
		if (strFrom.Compare(pData[i].szFrom) == 0) {
			strTo[nCount].Format(_T("%s"), pData[i].szTo);
			nCount++;
		}
	}
	nStrTo = nCount;

	CString strFileName = GetDataFileName(_T("USER_KOREAN_WORD"), _T("USER_CHINESE_WORD"));

	CString strLine;
	CString strC, strP;

	INT pos;
	CStdioFile fd;
	TCHAR pBuf[2049];
	if (fd.Open(strFileName, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) == TRUE) {
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
		while(TRUE) 
		{
			if (fd.ReadString(pBuf, 2048) == NULL) break;
			if (pBuf[0] == '.' || pBuf[0] == '#') continue;
			strLine.Format(_T("%s"), pBuf);
			pos = strLine.Find(_T("|"));
			if (pos == -1) continue;
			strC = strLine.Left(pos);
			strP = strLine.Mid(pos+1);
			strC.TrimLeft();			strC.TrimRight();
			strP.TrimLeft();			strP.TrimRight();
			
			if (strC == strFrom) {
				strTo[nCount].Format(_T("%s"), strP);;
				nCount++;			
			}
		}
		fd.Close();
		
		nStrTo = nCount;
	}
EFS_END
	return 0;
}

INT CESL_CharSetMgr::InitData(CString strFromSet, CString strToSet)
{
EFS_BEGIN
	FromSet = strFromSet;
	ToSet = strToSet;
	CString strFileName = GetDataFileName(strFromSet, strToSet);
	strFileName.TrimLeft();				strFileName.TrimRight();
	if (strFileName.GetLength() == -1) return -1;

	FreeData();
	CString strLine, strFrom, strTo;
	INT pos;
	CStdioFile fd;
	if (fd.Open(strFileName, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) != TRUE) return-1;
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	nData = 0;
	while(TRUE) {
		fd.ReadString(strLine);
		if (strLine.GetLength() == 0) break;
		if (strLine.GetAt(0) == '.' && strLine.GetLength() > 14) {
			strLine.TrimLeft();				strLine.TrimRight();
			strLine.MakeUpper();
			if (strLine.Left(13) == _T(".FILE_VERSION")) {
				strCharSetVersion.Format(_T("%s"), strLine.Mid(strLine.Find('=', 0)));
				strCharSetVersion.TrimLeft();		strCharSetVersion.TrimRight();
				continue;
			}
		}
		nData++;
	}
	fd.Close();
	
	pData = new CESL_CharSet[nData];

	INT i = 0;
	if (fd.Open(strFileName, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) != TRUE) return-1;

	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	while(TRUE) {
		fd.ReadString(strLine);
		if (strLine.GetLength() == 0) break;

		pos = strLine.Find('|', 0);
		if (pos == -1) continue;
		strFrom = strLine.Left(pos-1);
		strFrom.TrimLeft();				strFrom.TrimRight();
		strTo = strLine.Mid(pos+1);
		strTo.TrimLeft();				strTo.TrimRight();
		_stprintf(pData[i].szFrom, _T("%s\0"), strFrom);
		_stprintf(pData[i].szTo, _T("%s\0"), strTo);

		i++;
	}
	nData = i;
	fd.Close();

EFS_END
	return 0;
}

CString CESL_CharSetMgr::GetDataFileName(CString strFromSet, CString strToSet)
{
EFS_BEGIN
	CStdioFile fd;
if (fd.Open(_T("..\\CFG\\_CO_CHARSET_MANAGEMENT_TBL.TXT"), CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) != TRUE) return _T("");
	CString strLine, strFrom, strTo, strFileName;
	INT pos1, pos2, pos3;
	CString result = _T("");
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	while(TRUE) {
		fd.ReadString(strLine);
		if (strLine.GetLength() == 0) break;
		if (strLine.GetAt(0) == '.' && strLine.GetLength() > 14) {

		}
		if (strLine.GetAt(0) == '.') continue;
		
		strFrom = _T("");		strTo = _T("");		strFileName = _T("");
		pos1 = pos2 = pos3 = -1;
		pos1 = strLine.Find('|', 0);
		if (pos1 == -1) continue;
		pos2 = strLine.Find('|', pos1+1);
		if (pos2 == -1) continue;
		pos3 = strLine.Find('|', pos2+1);
		if (pos3 == -1) continue;

		strFrom = strLine.Mid(0, pos1-1);
		strFrom.TrimLeft();				strFrom.TrimRight();
		strTo = strLine.Mid(pos1+1, pos2-pos1-1);
		strTo.TrimLeft();				strTo.TrimRight();
		strFileName = strLine.Mid(pos2+1, pos3-pos2-1);
		strFileName.TrimLeft();			strFileName.TrimRight();

		if (strFromSet == strFrom && strToSet == strTo) {
			result = strFileName;
			break;
		}
	}
	fd.Close();
	return result;
EFS_END
	return _T("");
}

INT CESL_CharSetMgr::ConvertChineseTokorean(TCHAR *pSource, TCHAR *pTarget, BOOL bFindWordFlag)
{
EFS_BEGIN
	CString strReturn;
	CString strWord;

	INT nCount = _tcsclen(pSource);
	TCHAR ch1, ch2;

	for (INT i = 0; i < nCount; i++)
	{
		ch1 = pSource[i];
		ch2 = pSource[i+1];
		
		if (ch1 & 0x80)
		{
			if( ( ch1 >= 0xCA ) && (ch1  <= 0xFD ) 
				&& ( ch2  >= 0xA1 ) && ( ch2  <= 0xFE) ) 
			{ 
				strWord.Format(_T("%c%c"), ch1, ch2);
				
				Find(strWord, strWord);

				strReturn += strWord;
			}
			else
			{
				strReturn += ch1;
				strReturn += ch2;
			}
			i++; 
		}
		else
		{
			strReturn += ch1;
		}
	}
	
	_stprintf(pTarget, _T("%s\n"), strReturn);

EFS_END
	return 0;
}

INT CESL_CharSetMgr::GetAt(INT nIdx, CString &strFrom, CString &strTo)
{
EFS_BEGIN
	strFrom.Format(_T("%s"), pData[nIdx].szFrom);
	strTo.Format(_T("%s"), pData[nIdx].szTo);
EFS_END
	return 0;
}

