// K2UP_TnsManagerConfig.cpp: implementation of the CK2UP_TnsManagerConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "k2up_tnsmanager.h"
#include "K2UP_TnsManagerConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CK2UP_TnsManagerConfig::CK2UP_TnsManagerConfig()
{
	CFG_FILE_NAME = _T("K2UPTnsManager.cfg");
	nHOME = 0;
	EXECUTE_K2UP_AT_SCREEN_MODE = TRUE;
	nVERSION = 1;
	CHECK_VERSION_URL = _T("http://192.168.1.50/ecopm/UTIL_SMART_UPDATE/versionview.asp?NAME=TnsManager");
}

CK2UP_TnsManagerConfig::~CK2UP_TnsManagerConfig()
{

}

INT CK2UP_TnsManagerConfig::LoadConfig(CString strFileName)
{
	if (strFileName.GetLength() == 0) strFileName = CFG_FILE_NAME;

	CString str;
	CStdioFile fd;
	if (!fd.Open(strFileName, CFile::modeRead | CFile::typeBinary)) {
		str.Format(_T("설정파일을 열수없습니다.[%s]"), strFileName);
		AfxMessageBox(str);
		return -1;
	}
		TCHAR cUni;
		fd.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fd.SeekToBegin();
		}
	CString strHeader, strData;
	INT pos;
	while(TRUE) {
		str = _T("");
		if (fd.ReadString(str) == FALSE) break;
		if (str.GetLength() == 0) continue;
		if (str.GetAt(0) == '#') continue;
		if (str.GetAt(0) == '.') continue;

		strHeader = _T("");			strData = _T("");
		pos = str.Find('=', 0);
		if (pos == -1) continue;

		strHeader = str.Left(pos);
		strData = str.Right(str.GetLength() - pos-1);

		strHeader.TrimLeft();		strHeader.TrimRight();			strHeader.MakeUpper();
		strData.TrimLeft();		strData.TrimRight();			

		if (strHeader == _T("KOLAS II UP.EXE")) K2UP_PATH = strData;
		if (strHeader == _T("EXECUTE_K2UP")) {
			strData.MakeUpper();
			if (strData == _T("TRUE")) {
				EXECUTE_K2UP = TRUE;
			} else {
				EXECUTE_K2UP = FALSE;
			}
		}
		if (strHeader == _T("EXECUTE_K2UP_AT_SCREEN_MODE")) {
			strData.MakeUpper();
			if (strData == _T("TRUE")) {
				EXECUTE_K2UP_AT_SCREEN_MODE = TRUE;
			} else {
				EXECUTE_K2UP_AT_SCREEN_MODE = FALSE;
			}
		}
		if (strHeader == _T("DBMS_HOST")) DBMS_HOST = strData;
		if (strHeader == _T("DBMS_ID")) DBMS_ID = strData;
		if (strHeader == _T("DBMS_PWD")) DBMS_PWD = strData;
		if (strHeader == _T("DBMS_NAME")) DBMS_NAME = strData;
		if (strHeader == _T("ORACLE_HOME")) nHOME = _ttoi(strData);
		if (strHeader == _T("VERSION")) nVERSION = _ttoi(strData);
		if (strHeader == _T("CHECK_VERSION_URL")) CHECK_VERSION_URL = strData;
	}
	fd.Close();

	return 0;
}

INT CK2UP_TnsManagerConfig::SaveConfig(CString strFileName)
{
	CString str;
	if (strFileName.GetLength() == 0) strFileName = CFG_FILE_NAME;
	CStdioFile fd;
	if (!fd.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) {
		str.Format(_T("설정파일을 열수없습니다.[%s]"), strFileName);
		AfxMessageBox(str);
		return -1;
	}
	
	if( 0 == fd.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fd.Write( &cUni, sizeof(TCHAR));
	}
		
	str.Format(_T("KOLAS II UP.EXE = %s\r\n"), K2UP_PATH);
	fd.WriteString(str);
	if (EXECUTE_K2UP == TRUE) {
		str.Format(_T("EXECUTE_K2UP = TRUE\r\n"));
	} else {
		str.Format(_T("EXECUTE_K2UP = FALSE\r\n"));
	}
	fd.WriteString(str);
	if (EXECUTE_K2UP_AT_SCREEN_MODE == TRUE) {
		str.Format(_T("EXECUTE_K2UP_AT_SCREEN_MODE = TRUE\r\n"));
	} else {
		str.Format(_T("EXECUTE_K2UP_AT_SCREEN_MODE = FALS\r\n"));
	}
	fd.WriteString(str);
	str.Format(_T("DBMS_HOST = %s\r\n"), DBMS_HOST);
	fd.WriteString(str);
	str.Format(_T("DBMS_ID = %s\r\n"), DBMS_ID);
	fd.WriteString(str);
	str.Format(_T("DBMS_PWD = %s\r\n"), DBMS_PWD);
	fd.WriteString(str);
	str.Format(_T("DBMS_NAME = %s\r\n"), DBMS_NAME);
	fd.WriteString(str);
	str.Format(_T("ORACLE_HOME = %d\r\n"), nHOME);
	fd.WriteString(str);
	str.Format(_T("VERSION = %d\r\n"), nVERSION);
	fd.WriteString(str);
	str.Format(_T("CHECK_VERSION_URL = %s\r\n"), CHECK_VERSION_URL);
	fd.WriteString(str);

	fd.Close();

	return 0;
}
