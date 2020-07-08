// TnsSetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS2UP_SETUP.h"
#include "TnsSetupPage.h"
#include "SetupMainSheet.h"
#include "charconvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTnsSetupPage property page

IMPLEMENT_DYNCREATE(CTnsSetupPage, CPropertyPage)

CTnsSetupPage::CTnsSetupPage() : CPropertyPage(CTnsSetupPage::IDD)
{
	//{{AFX_DATA_INIT(CTnsSetupPage)
	m_strOracleFilePath = _T("");
	m_strDeamonFilePath = _T("");
	m_strOraclePort = _T("");
	m_strOracleService = _T("");
	m_strDEAMONTIMEOUT = _T("");
	m_strDEAMONRETRY = _T("");
	m_strDEAMONPORT = _T("");
	//}}AFX_DATA_INIT
	
	m_bError = FALSE;

	m_strDeamonLocalIP = _T("0.0.0.0");
	m_strDeamonLocalPORT = _T("0");
	m_strDeamonLocalTIMEOUT = _T("300");
	m_strDeamonLocalRETRYCNT = _T("3");

	m_strDeamonRemoteIP = _T("0.0.0.0");
	m_strDeamonRemoteTIMEOUT = _T("300");
	m_strDeamonRemoteRETRYCNT = _T("3");
}

CTnsSetupPage::~CTnsSetupPage()
{
	FreeTnsList();
}

INT CTnsSetupPage::FreeTnsList()
{
	CString strKey;
	CTns *pTns = NULL;
	POSITION pos = m_mapTns.GetStartPosition();
	while (pos)
	{
		m_mapTns.GetNextAssoc(pos, strKey, (VOID*&)pTns);
		if (pTns == NULL) continue;

		delete pTns;
		pTns = NULL;
	}

	m_mapTns.RemoveAll();
	
	return 0;
}

VOID CTnsSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTnsSetupPage)
	DDX_Control(pDX, IDC_cDEAMON, m_cDEAMON);
	DDX_Control(pDX, IDC_ipDEAMON, m_IPDeamon);
	DDX_Control(pDX, IDC_ipORACLE, m_IPOracle);
	DDX_Text(pDX, IDC_sORACLE_FILE_PATH, m_strOracleFilePath);
	DDX_Text(pDX, IDC_sDEAMON_FILE_PATH, m_strDeamonFilePath);
	DDX_Text(pDX, IDC_eORACLE_PORT, m_strOraclePort);
	DDX_Text(pDX, IDC_eORACLE_SERVICE, m_strOracleService);
	DDX_Text(pDX, IDC_eDEAMON_TIMEOUT, m_strDEAMONTIMEOUT);
	DDX_Text(pDX, IDC_eDEAMON_RETRY, m_strDEAMONRETRY);
	DDX_Text(pDX, IDC_eDEAMON_PORT, m_strDEAMONPORT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTnsSetupPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTnsSetupPage)
	ON_CBN_SELCHANGE(IDC_cORACLE_SERVICE, OnSelchangecORACLESERVICE)
	ON_BN_CLICKED(IDC_bADD_ORACLE_SERVICE, OnbADDORACLESERVICE)
	ON_BN_CLICKED(IDC_bMODIFY_ORACLE_SERVICE, OnbMODIFYORACLESERVICE)
	ON_BN_CLICKED(IDC_bDELETE_ORACLE_SERVICE, OnbDELETEORACLESERVICE)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_ipORACLE, OnFieldchangedipORACLE)
	ON_EN_CHANGE(IDC_eORACLE_PORT, OnChangeeORACLEPORT)
	ON_EN_CHANGE(IDC_eORACLE_SERVICE, OnChangeeORACLESERVICE)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_ipDEAMON, OnFieldchangedipDEAMON)
	ON_EN_CHANGE(IDC_eDEAMON_PORT, OnChangeeDEAMONPORT)
	ON_CBN_SELCHANGE(IDC_cDEAMON, OnSelchangecDEAMON)
	ON_EN_CHANGE(IDC_eDEAMON_TIMEOUT, OnChangeeDEAMONTIMEOUT)
	ON_EN_CHANGE(IDC_eDEAMON_RETRY, OnChangeeDEAMONRETRY)
	ON_CBN_EDITCHANGE(IDC_cORACLE_SERVICE, OnEditchangecORACLESERVICE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTnsSetupPage Operations
INT CTnsSetupPage::LoadConfig()
{
	// 셋업 전 모드이면 현재 디렉토리에서 파일을 연다.
	if (((CSetupMainSheet*)GetParent())->m_nIDCaption == IDS_PRE_SETUP)
	{
		m_strOracleFilePath = _T("tnsnames.ora");
		m_strDeamonFilePath = _T("interface.cfg");

		// 파일이 존재하면 읽기전용인지 체크한다.

		// 오라클
		CFileFind finder;
		if (finder.FindFile(m_strOracleFilePath))
		{
			finder.FindNextFile();
			if (finder.IsReadOnly())
			{
				if (!SetFileAttributes(m_strOracleFilePath, FILE_ATTRIBUTE_NORMAL))
				{
					m_strLastError = _T("쓸수 없는 매체의 파일입니다.");
					return -1;
				}
			}
		}

		// 데몬
		//2003.12.16 이학중 수정 
		//데몬설정이 ServerInstall로 넘어갔기 때문에 여기서 설정할 필요가 없다. 따라서 이기능은 없앤다.
     /* 
		if (finder.FindFile(m_strDeamonFilePath))
		{
			finder.FindNextFile();
			if (finder.IsReadOnly())
			{
				if (!SetFileAttributes(m_strDeamonFilePath, FILE_ATTRIBUTE_NORMAL))
				{
					m_strLastError = _T("쓸수 없는 매체의 파일입니다.");
					return -1;
				}
			}
		}
	 */
	}
	else
	{
		// 오라클
		TCHAR szBuffer[1024];
		DWORD dwBufLen = sizeof(TCHAR) * 1024;
		ZeroMemory(szBuffer, dwBufLen);

		CString strTemp;

		CString strDefaultHome;
		INT i = -1;
		INT nLastHome = -1;
		INT nHomeCounter = -1;
		BOOL bFindOracleHome = FALSE;
	

		// 레지스트리에 적용된 경로에서 읽어온다.
		/*
		HKEY hKey;
		LPCTSTR lpSubKey = NULL;
		LPCTSTR lpValueName = NULL;
		
		lpSubKey  = _T("SOFTWARE\\ORACLE\\ALL_HOMES");
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
		{
			m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s 키가 존재하지 않습니다."), lpSubKey);
			return -1;
		}

		// DEFAULT_HOME을 가져온다.
		lpValueName = _T("DEFAULT_HOME");
		dwBufLen = sizeof(TCHAR) * 1024;
		if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
		{
			m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), lpSubKey, lpValueName);
			return -1;
		}
		strDefaultHome.Format(_T("%s"), szBuffer);

		// HOME_COUNTER를 가져온다.
		lpValueName = _T("HOME_COUNTER");
		dwBufLen = sizeof(TCHAR) * 1024;
		if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
		{
			m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), lpSubKey, lpValueName);
			return -1;
		}
		nHomeCounter = _ttoi(szBuffer);

		// LAST_HOME을 가져온다.
		lpValueName = _T("LAST_HOME");
		if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
		{
			m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), lpSubKey, lpValueName);
			return -1;
		}

		// LAST_HOME이 가리키는 KEY 확인
		strTemp.Format(_T("SOFTWARE\\ORACLE\\HOME%s"), szBuffer);
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, strTemp, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &hKey) != ERROR_SUCCESS)
		{
			m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s 키가 존재하지 않습니다."), strTemp);
			return -1;
		}
		nLastHome = _ttoi(szBuffer);

		RegCloseKey( hKey );

		// DEFAULT_HOME이 가리키는 KEY 확인
		for (i = 0; i <= nLastHome; i++)
		{
			strTemp.Format(_T("SOFTWARE\\ORACLE\\HOME%d"), i);
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, strTemp, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &hKey) != ERROR_SUCCESS)
			{
				m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s 키가 존재하지 않습니다."), strTemp);
				return -1;
			}

			// ORACLE_HOME_NAME 이 맞는지 확인
			lpValueName = _T("ORACLE_HOME_NAME");
			dwBufLen = sizeof(TCHAR) * 1024;
			if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
			{
				m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), strTemp, lpValueName);
				return -1;
			}

			if (_tcscmp(szBuffer, strDefaultHome) == 0) 
			{
				bFindOracleHome = TRUE;
				break;		
			}
		}

		if (!bFindOracleHome) 
		{
			m_strLastError = _T("DEFAULT HOME을 찾을수 없습니다.");
			return -1;
		}

		// ORACLE_HOME을 확인한다. tnsname.ora로 확인 ㅋㅋ
		lpValueName = _T("ORACLE_HOME");
		dwBufLen = sizeof(TCHAR) * 1024;
		if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
		{
			m_strLastError.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), strTemp, lpValueName);
			return -1;
		}

		m_strOracleFilePath.Format(_T("%s\\network\\admin\\tnsnames.ora"), szBuffer);

		// 데몬
		m_strDeamonFilePath = _T("..\\cfg\\interface.cfg");
		*/
		CStdioFile fd;
		if (fd.Open(_T("..\\CFG\\ENVVAR.CFG"), CStdioFile::modeRead | CFile::typeBinary) == FALSE) return -1;
		TCHAR cUni;
		fd.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fd.SeekToBegin();
		}
		
		CString strLine;
		while(TRUE) {
			if (fd.ReadString(strLine) == FALSE) break;

			strLine.TrimLeft();		strLine.TrimRight();
			if (strLine.GetLength() == 0) continue;
			if (strLine.GetAt(0) == '#') continue;
			if (strLine.GetAt(0) == '.') continue;

			_putenv((LPSTR)(LPCTSTR)strLine.GetBuffer(0));
			strLine.ReleaseBuffer();
		}

		fd.Close();

		m_strOracleFilePath.Format(_T("..\\oracle\\network\\admin\\tnsnames.ora"), szBuffer);
	}

	FetchOracle();

	//2003.12.16 이학중 수정 
	//데몬설정이 ServerInstall로 넘어갔기 때문에 여기서 설정할 필요가 없다. 따라서 이기능은 없앤다.
	//FetchDeamon();

	return 0;
}

INT CTnsSetupPage::FetchOracle()
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
	CString strLine;
	CStdioFile file;
	// 오라클
	if (!file.Open(m_strOracleFilePath, CFile::modeRead))
	{
		if (!file.Open(m_strOracleFilePath, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary))
		{
			m_strLastError = _T("파일을 열수 없습니다.");
			return -1;
		}	
	}	

	DWORD dwLen = file.GetLength()+1;
	if (dwLen == 0)
	{
		file.Close();
		return 0;
	}
	CHAR *szBuffer = new CHAR[dwLen];	
	ZeroMemory(szBuffer, dwLen);
	TCHAR **szUniBuffer= NULL;		
	INT			nConvertLen;

	file.ReadHuge(szBuffer, dwLen);
	file.Close();
	CharConvert cvt;
	

	szUniBuffer = (TCHAR**) malloc (sizeof(TCHAR*));
	cvt.MultiByteToUnicode(szBuffer, szUniBuffer, &nConvertLen);		
	if ( szBuffer )		
	{
		// KOL.UDF.022
		//delete szBuffer;
		delete[] szBuffer;
		szBuffer = NULL;
	}
	
	CString strFileData;
	strFileData.Format(_T("%s"), *szUniBuffer);
	// KOL.UDF.022
	if (&szUniBuffer != NULL) 
	{
		free(szUniBuffer);
		szUniBuffer = NULL;
	}
	
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

				m_mapTns.SetAt(strKeyValue, (VOID*)pTns);

			}

			nStart = ++n;
			n = strFileData.Find(_T("("), n);
			nCount++;
			if (n < 0) break;
		}
	}
	
	return 0;
}

INT CTnsSetupPage::GetTnsValue(CString strTns, CString strKey, CString &strValue)
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

INT CTnsSetupPage::FetchDeamon()
{
	CStdioFile file;
	CString strLine;
	// 데몬
	if (!file.Open(m_strDeamonFilePath, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(m_strDeamonFilePath, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary))
		{
			m_strLastError = _T("파일을 열 수 없습니다.");
			return -1;
		}
		else
		{
			if( 0 == file.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				file.Write( &cUni, sizeof(TCHAR));
			}
		}
	}
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

		if (strLine.IsEmpty() || strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

		if (strLine == _T("[LOCAL_SERVER]"))
		{
			while (file.ReadString(strLine))
			{
				strLine.TrimLeft();
				strLine.TrimRight();

				if (strLine.IsEmpty() || strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

				if (strLine.Left(1) == _T("[")) break;
				
				_stscanf(strLine.GetBuffer(0), _T("%s %s %s %s"), m_strDeamonLocalIP.GetBuffer(64), m_strDeamonLocalPORT.GetBuffer(64), m_strDeamonLocalTIMEOUT.GetBuffer(64), m_strDeamonLocalRETRYCNT.GetBuffer(64));
				m_strDeamonLocalIP.ReleaseBuffer();
				m_strDeamonLocalPORT.ReleaseBuffer();
				m_strDeamonLocalTIMEOUT.ReleaseBuffer();
				m_strDeamonLocalRETRYCNT.ReleaseBuffer();
				break;
			}
		}
		if (strLine == _T("[REMOTE_SERVER]"))
		{
			while (file.ReadString(strLine))
			{
				strLine.TrimLeft();
				strLine.TrimRight();

				if (strLine.IsEmpty() || strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

				if (strLine.Left(1) == _T("[")) break;

				_stscanf(strLine.GetBuffer(0), _T("%s %s %s %s"), m_strDeamonRemoteIP.GetBuffer(64), m_strDeamonRemotePORT.GetBuffer(64), m_strDeamonRemoteTIMEOUT.GetBuffer(64), m_strDeamonRemoteRETRYCNT.GetBuffer(64));
				m_strDeamonRemoteIP.ReleaseBuffer();
				m_strDeamonRemotePORT.ReleaseBuffer();
				m_strDeamonRemoteTIMEOUT.ReleaseBuffer();
				m_strDeamonRemoteRETRYCNT.ReleaseBuffer();
				break;
			}
		}
		if (strLine == _T("[CON_ABS_SERVER]"))
		{
			while (file.ReadString(strLine))
			{
				strLine.TrimLeft();
				strLine.TrimRight();

				if (strLine.IsEmpty() || strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

				if (strLine.Left(1) == _T("[")) break;

				_stscanf(strLine.GetBuffer(0), _T("%s %s %s %s"), m_strDeamonCON_ABS_SERVER_IP.GetBuffer(64), m_strDeamonCON_ABS_SERVER_PORT.GetBuffer(64), m_strDeamonCON_ABS_SERVER_TIMEOUT.GetBuffer(64), m_strDeamonCON_ABS_SERVER_RETRYCNT.GetBuffer(64));
				m_strDeamonCON_ABS_SERVER_IP.ReleaseBuffer();
				m_strDeamonCON_ABS_SERVER_PORT.ReleaseBuffer();
				m_strDeamonCON_ABS_SERVER_TIMEOUT.ReleaseBuffer();
				m_strDeamonCON_ABS_SERVER_RETRYCNT.ReleaseBuffer();
				break;
			}
		}
	}
	file.Close();
	
	Display_DEAMON_CONFIG();

	return 0;
}

INT CTnsSetupPage::Display_DEAMON_CONFIG(VOID)
{
	BYTE nField0;
	BYTE nField1;
	BYTE nField2;
	BYTE nField3;

	if (m_strDEMON_MODE == _T("LOCAL_SERVER")) {
		_stscanf(m_strDeamonLocalIP.GetBuffer(0), _T("%d.%d.%d.%d"), &nField0, &nField1, &nField2, &nField3);
		m_strDeamonLocalIP.ReleaseBuffer();
		m_strDEAMONPORT = m_strDeamonLocalPORT;
		m_strDEAMONRETRY = m_strDeamonLocalRETRYCNT;
		m_strDEAMONTIMEOUT = m_strDeamonLocalTIMEOUT;
	}
	if (m_strDEMON_MODE == _T("REMOTE_SERVER")) {
		_stscanf(m_strDeamonRemoteIP.GetBuffer(0), _T("%d.%d.%d.%d"), &nField0, &nField1, &nField2, &nField3);
		m_strDeamonLocalIP.ReleaseBuffer();
		m_strDEAMONPORT = m_strDeamonRemotePORT;
		m_strDEAMONRETRY = m_strDeamonRemoteRETRYCNT;
		m_strDEAMONTIMEOUT = m_strDeamonRemoteTIMEOUT;
	}
	if (m_strDEMON_MODE == _T("CON_ABS_SERVER")) {
		_stscanf(m_strDeamonCON_ABS_SERVER_IP.GetBuffer(0), _T("%d.%d.%d.%d"), &nField0, &nField1, &nField2, &nField3);
		m_strDeamonLocalIP.ReleaseBuffer();
		m_strDEAMONPORT = m_strDeamonCON_ABS_SERVER_PORT;
		m_strDEAMONRETRY = m_strDeamonCON_ABS_SERVER_RETRYCNT;
		m_strDEAMONTIMEOUT = m_strDeamonCON_ABS_SERVER_TIMEOUT;
	}
	m_IPDeamon.SetAddress(nField0, nField1, nField2, nField3);

	UpdateData(FALSE);
	return 0;
}

INT CTnsSetupPage::Apply_DEAMON_CONFIG(VOID)
{
	UpdateData(TRUE);
	BYTE nField0;
	BYTE nField1;
	BYTE nField2;
	BYTE nField3;

	m_IPDeamon.GetAddress(nField0, nField1, nField2, nField3);

	if (m_strDEMON_MODE == _T("LOCAL_SERVER")) {
		m_strDeamonLocalIP.Format(_T("%d.%d.%d.%d"), nField0, nField1, nField2, nField3);
		m_strDeamonLocalPORT = m_strDEAMONPORT;
		m_strDeamonLocalRETRYCNT = m_strDEAMONRETRY;
		m_strDeamonLocalTIMEOUT = m_strDEAMONTIMEOUT;
	}
	if (m_strDEMON_MODE == _T("REMOTE_SERVER")) {
		m_strDeamonRemoteIP.Format(_T("%d.%d.%d.%d"), nField0, nField1, nField2, nField3);
		m_strDeamonRemotePORT = m_strDEAMONPORT;
		m_strDeamonRemoteRETRYCNT = m_strDEAMONRETRY;
		m_strDeamonRemoteTIMEOUT = m_strDEAMONTIMEOUT;
	}
	if (m_strDEMON_MODE == _T("CON_ABS_SERVER")) {
		m_strDeamonCON_ABS_SERVER_IP.Format(_T("%d.%d.%d.%d"), nField0, nField1, nField2, nField3);
		m_strDeamonCON_ABS_SERVER_PORT = m_strDEAMONPORT;
		m_strDeamonCON_ABS_SERVER_RETRYCNT = m_strDEAMONRETRY;
		m_strDeamonCON_ABS_SERVER_TIMEOUT = m_strDEAMONTIMEOUT;
	}
	return 0;
}

INT CTnsSetupPage::DisplayComboBox()
{
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cORACLE_SERVICE);
	if (pComboBox == NULL) return -1;

	pComboBox->ResetContent();

	CTns *pTns = NULL;
	CString strKey;
	POSITION pos = m_mapTns.GetStartPosition();
	while (pos)
	{
		m_mapTns.GetNextAssoc(pos, strKey, (VOID*&)pTns);
		if (strKey.IsEmpty()) continue;

		pComboBox->AddString(strKey);
	}

	pComboBox->AddString(_T(""));
	pComboBox->SetCurSel(0);

	if (!m_strSelKey.IsEmpty())
	{
		INT nCount = pComboBox->GetCount();
		for (INT idx = 0; idx < nCount; idx++)
		{
			pComboBox->GetLBText(idx, strKey);
			if (strKey == m_strSelKey)
			{ 
				pComboBox->SetCurSel(idx);
				break;
			}
		}
	}

	return 0;
}

INT CTnsSetupPage::Display()
{
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cORACLE_SERVICE);
	if (pComboBox == NULL) return -1;

	CString strKey;
	pComboBox->GetLBText(pComboBox->GetCurSel(), strKey);

	m_strSelKey = strKey;

	if (strKey.IsEmpty()) 
	{
		m_IPOracle.ClearAddress();
		m_strOraclePort.Empty();
		m_strOracleService.Empty();
	}
	else
	{

		CTns *pTns = NULL;
		if (m_mapTns.Lookup(strKey, (VOID*&)pTns))
		{
			BYTE nField0;
			BYTE nField1;
			BYTE nField2;
			BYTE nField3;

			// KOL.UDF.022
			if(&pTns != NULL){
				_stscanf(pTns->m_strHOST, _T("%d.%d.%d.%d"), &nField0, &nField1, &nField2, &nField3);
				m_IPOracle.SetAddress(nField0, nField1, nField2, nField3);

				m_strOraclePort = pTns->m_strPORT;
				m_strOracleService = pTns->m_strSERVICE_NAME;
			}
			
		}
	}

	UpdateData(FALSE);


	return 0;
}

INT CTnsSetupPage::SaveConfig()
{
	UpdateData();
	// 오라클
	CStdioFile file;
	if (!file.Open(m_strOracleFilePath, CFile::modeWrite | CFile::modeCreate ))
	{
		AfxMessageBox(_T("저장 실패 : 파일을 열수 없습니다."));
		return FALSE;
	}
	

	CTns *pTns = NULL;

	CString strKey;
	CString strTemp;	
	
	INT			nConvertLen;
	CHAR**		szpTns = NULL;
	CharConvert cvt;
	
	POSITION pos = m_mapTns.GetStartPosition();	
	while (pos)
	{
		m_mapTns.GetNextAssoc(pos, strKey, (VOID*&)pTns);
		if (pTns == NULL) continue;
		
		strTemp.Format(_T("%s = \n")
					   _T("  (DESCRIPTION = \n")
					   _T("    (ADDRESS_LIST = \r\n")
					   _T("      (ADDRESS = (PROTOCOL = TCP)(HOST = %s)(PORT = %s)) \n")
					   _T("    ) \n")
					   _T("    (CONNECT_DATA = \n")
					   _T("      (SERVICE_NAME = %s)\n")
					   _T("    )\n")
					   _T("  )\n")
					   , strKey, pTns->m_strHOST, pTns->m_strPORT, pTns->m_strSERVICE_NAME);		

		szpTns = (CHAR**) malloc (sizeof(CHAR*));
		
		cvt.UnicodeToMultiByte(strTemp.GetBuffer(0), szpTns, &nConvertLen );
		strTemp.ReleaseBuffer();
		file.Write(*szpTns, nConvertLen);
		free(szpTns);		
	}
	file.Close();

	// 데몬
	// 2003.12.08 - 데몬설정이 ServerInstall로 넘어갔기 때문에 여기서 설정할 필요가 없다. 따라서 이기능은 없앤다.
/*
	if (!file.Open(m_strDeamonFilePath, CFile::modeWrite | CFile::modeCreate))
	{
		AfxMessageBox(_T("저장 실패 : 파일을 열수 없습니다."));
		return FALSE;
	}

	strTemp.Format( _T(".//=======================================//\n")
					_T(".// Server와 통신을 위해 정보를 설정한다. //\n")
					_T(".//=======================================//\n")
					_T(".\n")
					_T("%-20s%-10s%-15s%-10s\n")
					, _T(".IP"), _T("PORT"), _T("TIMEOUT(Sec)"), _T("RETRY CNT"));
	file.WriteString(strTemp);

	file.WriteString(_T("[LOCAL_SERVER]\n"));
	strTemp.Format(_T("%-20s%-10s%-15s%-10s\n"), m_strDeamonLocalIP, m_strDeamonLocalPORT, m_strDeamonLocalTIMEOUT, m_strDeamonLocalRETRYCNT);
	file.WriteString(strTemp);

	file.WriteString(_T("[REMOTE_SERVER]\n"));
	strTemp.Format(_T("%-20s%-10s%-15s%-10s\n"), m_strDeamonRemoteIP, m_strDeamonRemotePORT, m_strDeamonRemoteTIMEOUT, m_strDeamonRemoteRETRYCNT);
	file.WriteString(strTemp);

	file.WriteString(_T("[CON_ABS_SERVER]\n"));
	strTemp.Format(_T("%-20s%-10s%-15s%-10s\n"), m_strDeamonCON_ABS_SERVER_IP, m_strDeamonCON_ABS_SERVER_PORT, m_strDeamonCON_ABS_SERVER_TIMEOUT, m_strDeamonCON_ABS_SERVER_RETRYCNT);
	file.WriteString(strTemp);

	file.Close();
*/
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CTnsSetupPage message handlers

BOOL CTnsSetupPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_cDEAMON.ResetContent();
	m_cDEAMON.AddString(_T("LOCAL_SERVER"));
	m_cDEAMON.AddString(_T("REMOTE_SERVER"));
	m_cDEAMON.AddString(_T("CON_ABS_SERVER"));
	m_cDEAMON.SetCurSel(1);
	UpdateData(FALSE);
	m_strDEMON_MODE = _T("REMOTE_SERVER");

	// 설정값을 읽어온다.
	//CEdit *pEdit = (CEdit*)GetDlgItem(IDC_eTNS);
	if (LoadConfig() < 0)
	{
		m_bError = TRUE;

		MessageBox(_T("설정값을 가져올수 없습니다."));
		
		return FALSE;

	}
	else
	{
		DisplayComboBox();
		Display();
	}

			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTnsSetupPage::OnApply() 
{
	if (m_bError) return FALSE;
   
	OnbMODIFYORACLESERVICE();
	//2003.12.16 이학중 수정 
	//데몬설정이 ServerInstall로 넘어갔기 때문에 여기서 설정할 필요가 없다. 따라서 이기능은 없앤다.
//	Apply_DEAMON_CONFIG();
	SaveConfig();
	
	return CPropertyPage::OnApply();
}

VOID CTnsSetupPage::OnSelchangecORACLESERVICE() 
{
	Display();
}

VOID CTnsSetupPage::OnbADDORACLESERVICE() 
{
	UpdateData();

	BYTE nField0;
	BYTE nField1;
	BYTE nField2;
	BYTE nField3;

	m_IPOracle.GetAddress(nField0, nField1, nField2, nField3);

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cORACLE_SERVICE);
	if (pComboBox == NULL) return;

	CString strKey;

	pComboBox->GetWindowText(strKey);
	if (strKey.IsEmpty()) return;

	m_strSelKey = strKey;

	CTns *pTns = NULL;
	if (m_mapTns.Lookup(strKey, (VOID*&)pTns)) return;

	pTns = new CTns;
	pTns->m_strHOST.Format(_T("%d.%d.%d.%d"), nField0, nField1, nField2, nField3);
	pTns->m_strPORT = m_strOraclePort;
	pTns->m_strSERVICE_NAME = m_strOracleService;

	m_mapTns.SetAt(strKey, (VOID*)pTns);

	DisplayComboBox();
	Display();
	
	SetModified();
}

VOID CTnsSetupPage::OnbMODIFYORACLESERVICE() 
{
	if (m_strSelKey.IsEmpty()) return;

	UpdateData();

	CTns *pTns = NULL;
	if (!m_mapTns.Lookup(m_strSelKey, (VOID*&)pTns)) return;

	if (pTns)
	{
		delete pTns;
		pTns = NULL;
	}

	m_mapTns.RemoveKey(m_strSelKey);
	
	OnbADDORACLESERVICE();
	SetModified();
}

VOID CTnsSetupPage::OnbDELETEORACLESERVICE() 
{
	if (m_strSelKey.IsEmpty()) return;

	CTns *pTns = NULL;
	if (!m_mapTns.Lookup(m_strSelKey, (VOID*&)pTns)) return;

	if (pTns)
	{
		delete pTns;
		pTns = NULL;
	}

	m_mapTns.RemoveKey(m_strSelKey);

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cORACLE_SERVICE);
	if (pComboBox == NULL) return;
	pComboBox->DeleteString(pComboBox->GetCurSel());	

	DisplayComboBox();
	Display();	
	SetModified();
}

VOID CTnsSetupPage::OnFieldchangedipORACLE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetModified();	
	*pResult = 0;
}

VOID CTnsSetupPage::OnChangeeORACLEPORT() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();	
}

VOID CTnsSetupPage::OnChangeeORACLESERVICE() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();	
}

VOID CTnsSetupPage::OnFieldchangedipDEAMON(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetModified();	
	*pResult = 0;
}

VOID CTnsSetupPage::OnChangeeDEAMONPORT() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();	
}

VOID CTnsSetupPage::OnSelchangecDEAMON() 
{
	UpdateData(TRUE);
	Apply_DEAMON_CONFIG();
	CString str;
	m_cDEAMON.GetWindowText(str);
	str.TrimLeft();			str.TrimRight();
	str.MakeUpper();
	m_strDEMON_MODE = str;

	Display_DEAMON_CONFIG();
}

VOID CTnsSetupPage::OnChangeeDEAMONTIMEOUT() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();	
}

VOID CTnsSetupPage::OnChangeeDEAMONRETRY() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();	
}

BOOL CTnsSetupPage::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
	{
// 		if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
// 		{		
// 			return TRUE;
// 		}
		if(IDC_ipORACLE == GetFocus()->GetDlgCtrlID())
		{
			SetModified();
		}
	}
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CTnsSetupPage::OnEditchangecORACLESERVICE() 
{
	// TODO: Add your control notification handler code here
	SetModified();		
}
