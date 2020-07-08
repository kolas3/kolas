// RPTUpdaterClientApi.cpp: implementation of the CRPTUpdaterClientApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RPTUpdaterClient.h"
#include "RPTUpdaterClientApi.h"
#include "RPTUpdaterClientCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRPTUpdaterClientApi::CRPTUpdaterClientApi()
{
	m_strServerURL = _T("");
	m_strServerObject = _T("");
	m_strServerLogObject = _T("");
	m_strServerFilePath = _T("");
	m_strLibraryCode = _T("111111"); //test
	m_strCfgFileName = _T("..\\cfg\\rptclient\\rptclient.cfg");
	m_strDefaultCodeFileName = _T("..\\cfg\\rptclient\\DefaultCode.cfg");
	m_strListFileName = _T("..\\cfg\\rptclient\\filedata\\list.tmp");
	m_strLogServerFileName = _T("..\\cfg\\rptclient\\filedata\\logserver.tmp"); 
	m_arrayRptAlias.RemoveAll();

	m_strLibraryName = _T("");
	m_strLogInName = _T("");
}

CRPTUpdaterClientApi::~CRPTUpdaterClientApi()
{

}


INT CRPTUpdaterClientApi::InitApi()
{

	if(ReadFtpCfgFile()<0) {
		AfxMessageBox(_T("설정 파일을 읽지 못하였습니다!"));
		return -1;
	}

	if(GetRptListFromServer()<0) {
		return -1;
	}

	return 0;
}

INT CRPTUpdaterClientApi::ReadFtpCfgFile()
{
	// 설정 파일을 읽는다.
	CStdioFile file;
	if (!file.Open(m_strCfgFileName, CFile::modeRead | CFile::typeBinary)) return -1;

	CString strKey;
	CString strValue;
	CString strLine;
	INT nFind = -1;
	INT ids = -1;
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty()) continue;
		if (strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

		nFind = strLine.Find('=');
		if (nFind < 0) continue;

		ids = _stscanf(strLine.GetBuffer(0), _T("%s = %s"), strKey.GetBuffer(256), strValue.GetBuffer(256));
		strKey.ReleaseBuffer();
		strValue.ReleaseBuffer();

		if (ids == 0 || ids == _TEOF) continue;

		strKey.TrimLeft();
		strKey.TrimRight();

		strValue.TrimLeft();
		strValue.TrimRight();

		if (strKey.IsEmpty() || strValue.IsEmpty()) continue;

		if (strKey == _T("SERVER_URL"))
			m_strServerURL = strValue;
		else if (strKey == _T("SERVER_FILE_PATH"))
			m_strServerFilePath = strValue;
		else if (strKey == _T("SERVER_OBJECT"))
			m_strServerObject = strValue;
		else if (strKey == _T("SERVER_LOG_OBJECT"))
			m_strServerLogObject = strValue;

	}
		
	file.Close();

	return 0;
}

INT CRPTUpdaterClientApi::SetConfig()
{
	//로컬 DB접속을 확인한다.
	if(ConnectTrial()<0)
	{
		AfxMessageBox(_T("로컬 DB 서버 접속에 실패하였습니다!"));
		return -1;
	}
	

	//DM을 설정한다
	CString strConnectionInfo;
	strConnectionInfo = _T("kolas2@k2up/kolas2#[]");
	m_DM.SetCONNECTION_INFO(strConnectionInfo);


	return 0;
}

INT CRPTUpdaterClientApi::ConnectTrial()
{
	
	CString strUserName,strOraclePassword;
	strUserName = _T("kolas2@k2up");	
	strOraclePassword = _T("kolas2#");

	INT nRequestID;
	nRequestID = GetSessionRequestID();
	if (ConnectDB(nRequestID, strUserName.GetBuffer(0), strOraclePassword.GetBuffer(0)) < 0)
	{
		return -1;
	}
	
	return 0;
}

INT CRPTUpdaterClientApi::DownLoadRptFile(CString strAlias)
{
	//해당 알리아스의 파일(*.info , *.data)을 서버로 부터 다운 받는다
	
	HRESULT hr;
	CString strURL;
	
	CString strFileName;
	CString strPath;

	//*.info
	strFileName.Format(_T("%s.info"),strAlias);
	strURL = _T("http://") + m_strServerURL + m_strServerFilePath + strFileName;
	strPath.Format(_T("%s\\FileData\\%s"),CFG_DIRECTORY,strFileName);

	hr = URLDownloadToFile(NULL, strURL, strPath , 0, NULL); 
	
	if(hr != S_OK)
	{
		return -1;
	}
	
	//*.data
	strFileName.Format(_T("%s.data"),strAlias);
	strURL = _T("http://") + m_strServerURL + m_strServerFilePath + strFileName;
	strPath.Format(_T("%s\\FileData\\%s"),CFG_DIRECTORY,strFileName);
	
	hr = URLDownloadToFile(NULL, strURL, strPath , 0, NULL); 
	
	if(hr != S_OK)
	{
		return -1;
	}


	return 0;
}

INT CRPTUpdaterClientApi::SetLibraryCode(CString strID,CString strPass)
{

	CString strQuery;
	CString strResult[3];
	strQuery.Format(_T("SELECT L.LIB_CODE, L.LIB_NAME, U.NAME FROM MN_LIBINFO2_TBL L,MN_USER_TBL U")
					_T(" WHERE L.MANAGE_CODE = U.MANAGE_CODE AND U.ID = '%s' AND U.PASSWD = '%s'"),strID,strPass);


	m_DM.GetValuesQuery(strQuery,strResult,3);
	


	if(strResult[0].IsEmpty())
	{
		
		CString msg;
		msg.Format(_T("해당 아이디로 도서관 부호를 찾지 못하였습니다!\n도서관 부호를 직접 입력하시겠습니까?"));

		if(AfxMessageBox(msg,MB_YESNO) != IDYES) return -1;
		
		CRPTUpdaterClientCode dlg;
		if(dlg.DoModal() != IDOK) return -1;

		m_strLibraryCode = dlg.m_strLibraryCode;
		m_strLibraryName = m_strLibraryCode;
		m_strLogInName = _T("확인안된사용자");
		
	}
	else
	{
		m_strLibraryCode = strResult[0]; //도서관부호값 설정
		m_strLibraryName = strResult[1];
		m_strLogInName = strResult[2];
	}


	return 0;
}

INT CRPTUpdaterClientApi::GetRptListFromServer()
{
	//GET방식

	HRESULT hr;
	CString strURL;
	
	CString strLibraryCode;
	strLibraryCode.Format(_T("?library_code=%s"),m_strLibraryCode);
	strURL = _T("http://") + m_strServerURL + m_strServerObject + strLibraryCode;

	hr = URLDownloadToFile(NULL, strURL, m_strListFileName , 0, NULL); 
	
	if(hr != S_OK)
	{
		AfxMessageBox(_T("서버로부터 패치 리스트를 받는데 실패 하였습니다"));
		return -1;
	}
	

	
	//tmp화일을 읽어 Rpt정보를 구성한다
	CStdioFile listfile;
	CString strListFile;

	strListFile.Format(_T("%s\\FileData\\list.tmp"),CFG_DIRECTORY);
	if (!listfile.Open(strListFile, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("ListFile을 읽을 수 없습니다!"));
		return -1;
	}
	TCHAR cUni;
	listfile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		listfile.SeekToBegin();
	}

	CString strLine;
	CArray<CString,CString> arrayTmp;
	CString msg;

	while(listfile.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();
		
		if(_T("NOCODE") == strLine)
		{
			AfxMessageBox(_T("서버에서 귀하의 도서관 부호를 인증하지 못하였습니다!\n프로그램을 종료합니다.")); 
			return -1;
		}
		else if(_T("NODATA") == strLine)
		{
	
			msg.Format(_T("[%s]의 [%s]님 안녕하세요!\n귀하의 도서관은 현재 패치 받을 출력물이 없습니다.\n")
					   _T("프로그램을 종료합니다."),m_strLibraryName,m_strLogInName);

			AfxMessageBox(msg);
			return -1;
		}

		if(strLine.IsEmpty()) continue;
		
		arrayTmp.Add(strLine);
	}
	
	//RptAlias의 중복을 제거한다
	CString strTmp;
	CString strAlias;
	BOOL bDuplicated = FALSE;

	for(INT i=0;i<arrayTmp.GetSize();i++)
	{
		strTmp = arrayTmp.GetAt(i);
		bDuplicated = FALSE;
		
		for(INT j=0;j<m_arrayRptAlias.GetSize();j++)
		{
			strAlias = m_arrayRptAlias.GetAt(j);
			if(strTmp == strAlias) bDuplicated = TRUE;
		}
			
		if(!bDuplicated) m_arrayRptAlias.Add(strTmp);
	}
	
	return 0;
}


/*
INT CRPTUpdaterClientApi::GetRptListFromServer()
{
    //POST방식
	
	BOOL bResult = TRUE;
	INT nReturn = 0;
	CString strResult;

	try
	{
		CInternetSession session;
		CHttpConnection *pConnection = session.GetHttpConnection(m_strServerURL, (INTERNET_PORT)80, NULL, NULL);
		if (pConnection == NULL) {
			AfxMessageBox(_T("서버와의 연결에 실패 하였습니다!."));
			return -1;
		}

		CHttpFile *pHttpFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, m_strServerObject);
		if (pHttpFile == NULL) {
			AfxMessageBox(_T("서버가 작업요청에 응답하지 않습니다!."));
			return -1;
		}

		CString strSeperate = _T("---------------------------7d13a23b368");
		CString strRequestHeaders;
		strRequestHeaders.Format(_T(
			_T("Accept: text/*\r\n")
			_T("Content-Type: multipart/form-data; boundary=%s\r\n")
			_T("User-Agent: VCApp\r\n")
			), strSeperate);

		pHttpFile->AddRequestHeaders(strRequestHeaders);

		DWORD dwPostDataLen = 0;

		// LIBRARY CODE를 넣는다
		CString strPostData;

		strPostData.Format(_T(
			_T("\r\n--%s\r\n")
			_T("Content-Disposition: form-data; name=\"library_code\"\r\n")
			_T("\r\n%s\r\n")
			_T("\r\n--%s--\r\n")
			), strSeperate, m_strLibraryCode, strSeperate);		

		dwPostDataLen = strPostData.GetLength();
		
		BYTE *pCurPos = NULL;
		BYTE *pPostData = NULL;
		
		pCurPos = pPostData = new BYTE[dwPostDataLen];
		ZeroMemory(pPostData, sizeof(BYTE) * dwPostDataLen);

		wmemcpy(pCurPos, (LPCTSTR)strPostData, strPostData.GetLength());
		pCurPos += strPostData.GetLength();

		// Http Server에 request를 보낸다.
		pHttpFile->SendRequest(NULL, 0, (LPVOID)pPostData, dwPostDataLen);
				
		DWORD dwRet;
		pHttpFile->QueryInfoStatusCode(dwRet);
		
		TCHAR szResp[1024];
		while (pHttpFile->ReadString(szResp, 1024))
		{
			
			
			strResult.Format(_T("%s"),szResp);

			TRACE(_T("%s\n"), szResp);
			
			
			if(_tcsicmp(szResp, _T("-100")) == 0)
			{
				AfxMessageBox(_T("서버로부터 도서관부호를 인증받지 못하였습니다!. 프로그램을 종료합니다"));
				nReturn = -1;
			}
			else if (_tcsicmp(szResp, _T("-101")) == 0)
			{
				AfxMessageBox(_T("패치할 데이터가 없습니다! 프로그램을 종료합니다"));
				nReturn = -1;
			}
			else if (strResult.Left(5) == _T("LIST:"))
			{
				AfxMessageBox(strResult);
			}
			else if (_tcsicmp(szResp, _T("SUCCESS")) == 0)
			{
				nReturn = 0;
			}


		}
		delete (LPVOID)pPostData;

		
		pHttpFile->Close();
		delete pHttpFile;
		delete pConnection;
				
		return 0;
	}
	catch (CInternetException &e)
	{
		TRACE(_T("Caught Exception (%d)\n"), e.m_dwError);
	}
	catch (...)
	{
		return -1;
	}
	

	return nReturn;
}
*/

INT CRPTUpdaterClientApi::LogServer(CString strAlias)
{
	//GET방식

	HRESULT hr;
	CString strURL;
	
	CString strVariables;
	strVariables.Format(_T("?library_code=%s&rpt_alias=%s"),m_strLibraryCode,strAlias);
	strURL = _T("http://") + m_strServerURL + m_strServerLogObject + strVariables;

	hr = URLDownloadToFile(NULL, strURL, m_strLogServerFileName , 0, NULL); 
	
	if(hr != S_OK)
	{
		return -1;
	}
	


	//화일을 읽어 서버로부터의 로그를 확인한다
	CStdioFile logfile;
	CString strListFile;
	CString strLine;

	if (!logfile.Open(m_strLogServerFileName, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("logfile 읽을 수 없습니다!"));
		return -1;
	}
	TCHAR cUni;
	logfile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		logfile.SeekToBegin();
	}

	while(logfile.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();
		
		if(_T("ERROR") == strLine)
		{
			return -1;
		}
		else if(_T("SUCCESS") == strLine)
		{
			return  0;
		}
	}
	

	return -1;
}

INT CRPTUpdaterClientApi::InsertProcedure(CString strAlias)
{
	//해당 alias의 info file을 읽어 
	
	CString strPK;
	INT ids = -1;
	ids = m_DM.MakeRecKey(strPK, SEQ_NAME);

	if (ids<0) {
		return -1; 
	}
	
	
/*	
0-FLAG
1-RPTGROUP
2-ENGINEER
3-DESCRIP
4-RPTSIZE
5-BITMAP_ALIAS
6-FIRST_DATE
7-LAST_DATE
*/

	CString strPath;
	CString msg;
	strPath.Format(_T("%s\\FileData\\%s.data"),CFG_DIRECTORY,strAlias);


	CFile file;
	if (!file.Open(strPath, CFile::modeRead | CFile::typeBinary)) {
		return -1;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	INT rptsize = 0;
	rptsize = file.GetLength();
	BYTE * pBuffer = (BYTE *)malloc(rptsize + 1024);
	file.ReadHuge(pBuffer, rptsize);
	file.Close();
	
	CString strInfo[8];
	ids = GetInfoData(strAlias,strInfo);
	if (ids<0) {
		return -1; 
	}


	CString sCurrentTime;
	CTime t = CTime::GetCurrentTime();
	sCurrentTime.Format(_T("%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	CString sQuery;
	sQuery.Format(_T("INSERT INTO ESL_SIREPORT_TBL (PK, FLAG, RPTALIAS, RPTGROUP, ENGINEER, DESCRIP, RPTSIZE, FIRST_DATE, LAST_DATE) ")
				 _T("VALUES (%s, '%s', '%s', '%s', '%s', '%s', %d, '%s', '%s');"), 
				 strPK, strInfo[0], strAlias, strInfo[1] , strInfo[2], strInfo[3], rptsize, sCurrentTime, sCurrentTime);



	// XML LOB File Update
	ids = m_DM.ExecuteQuery(sQuery, 1);
	if (ids < 0) {
		free (pBuffer);
		return -1;
	}

	ids = m_DM.LOBUpdate(_T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), pBuffer, rptsize);
	if (ids < 0) {
		free (pBuffer);
		return -1;
	}

	free(pBuffer);

	return 0;
}

INT CRPTUpdaterClientApi::UpdateProcedure(CString strAlias,CString strPK)
{

/*	
0-FLAG
1-RPTGROUP
2-ENGINEER
3-DESCRIP
4-RPTSIZE
5-BITMAP_ALIAS
6-FIRST_DATE
7-LAST_DATE
*/
	
	INT ids = -1;

	CString strPath;
	CString msg;
	strPath.Format(_T("%s\\FileData\\%s.data"),CFG_DIRECTORY,strAlias);

	CFile file;
	if (!file.Open(strPath, CFile::modeRead | CFile::typeBinary)) {
		return -1;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	INT rptsize = 0;
	rptsize = file.GetLength();
	BYTE * pBuffer = (BYTE *)malloc(rptsize + 1024);
	file.ReadHuge(pBuffer, rptsize);
	file.Close();
	
	CString strInfo[8];
	ids = GetInfoData(strAlias,strInfo);
	if (ids<0) {
		return -1; 
	}

	CString sCurrentTime;
	CTime t = CTime::GetCurrentTime();
	sCurrentTime.Format(_T("%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	
	CString sQuery;
	sQuery.Format(_T("UPDATE ESL_SIREPORT_TBL SET FLAG='%s', RPTGROUP='%s', "
					 _T("ENGINEER ='%s', DESCRIP='%s', RPTSIZE = %d, BITMAP_ALIAS ='%s', LAST_DATE='%s' WHERE PK=%s;"))
				    ,strInfo[0], strInfo[1], strInfo[2], strInfo[3], rptsize,  strInfo[5], sCurrentTime, strPK);


	// XML LOB File Update
	ids = m_DM.ExecuteQuery(sQuery, 1);
	if (ids < 0) {
		free (pBuffer);
		return -1;
	}

	ids = m_DM.LOBUpdate(_T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), pBuffer, rptsize);
	if (ids < 0) {
		free (pBuffer);
		return -1;
	}

	free(pBuffer);

	return 0;
}


INT CRPTUpdaterClientApi::GetInfoData(CString strAlias, CString strInfo[])
{

/*	
0-FLAG
1-RPTGROUP
2-ENGINEER
3-DESCRIP
4-RPTSIZE
5-BITMAP_ALIAS
6-FIRST_DATE
7-LAST_DATE
*/

	CString strLine;
	CString strPath;
	CStdioFile fileinfo;
	CString msg;
	CString strTitle,strData;
	INT nPos = -1;

	strPath.Format(_T("%s\\FileData\\%s.info"),CFG_DIRECTORY,strAlias);
	if (!fileinfo.Open(strPath, CFile::modeRead | CFile::typeBinary))
	{
		msg.Format(_T("%s File을 읽을 수 없습니다"),strPath);
		return -1;
	}
	TCHAR cUni;
	fileinfo.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fileinfo.SeekToBegin();
	}

	while(fileinfo.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();
		if(strLine.IsEmpty()) continue;

		nPos = strLine.Find(_T(":"));
		if(nPos<0) continue;
	
		strTitle = _T("");
		strData = _T("");

		strTitle = strLine.Left(nPos);
		strData = strLine.Mid(nPos+1);
		
		strTitle.TrimLeft();
		strTitle.TrimRight();
		strData.TrimLeft();
		strData.TrimRight();

		
		if(_T("FLAG") == strTitle)
		{
			strInfo[0] = strData;
		}
		else if(_T("RPTGROUP") == strTitle)
		{
			strInfo[1] = strData;
		}
		else if(_T("ENGINEER") == strTitle)
		{
			strInfo[2] = strData;
		}			
		else if(_T("DESCRIP") == strTitle)
		{
			strInfo[3] = strData;
		}		
		else if(_T("RPTSIZE") == strTitle)
		{
			strInfo[4] = strData;
		}
		else if(_T("BITMAP_ALIAS") == strTitle)
		{
			strInfo[5] = strData;
		}
		else if(_T("FIRST_DATE") == strTitle)
		{
			strInfo[6] = strData;
		}
		else if(_T("LAST_DATE") == strTitle)
		{
			strInfo[7] = strData;
		}
	}
	
	fileinfo.Close();

	return 0;
}


