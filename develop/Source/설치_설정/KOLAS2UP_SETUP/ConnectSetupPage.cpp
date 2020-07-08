// ConnectSetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS2UP_SETUP.h"
#include "ConnectSetupPage.h"
#include "OCI_API.h"

#include "SetupMainSheet.h"

// DES 암복호화
#include "FWDES.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectSetupPage property page

IMPLEMENT_DYNCREATE(CConnectSetupPage, CPropertyPage)

CConnectSetupPage::CConnectSetupPage() : CPropertyPage(CConnectSetupPage::IDD)
{
	//{{AFX_DATA_INIT(CConnectSetupPage)
	m_strDBMS = _T("");
	m_strOracleID = _T("");
	m_strOraclePassword = _T("");
	m_strOracleSID = _T("");
	m_strTestResult = _T("");
	m_strK3DBMS = _T("");
	m_strK3OracleID = _T("");
	m_strK3OraclePassword = _T("");
	m_strK3OracleSID = _T("");
	//}}AFX_DATA_INIT

	m_bError = FALSE;
	m_bIsLoad = FALSE;
}

CConnectSetupPage::~CConnectSetupPage()
{
}

VOID CConnectSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectSetupPage)
	DDX_Text(pDX, IDC_DMBS, m_strDBMS);
	DDX_Text(pDX, IDC_eORACLE_ID, m_strOracleID);
	DDX_Text(pDX, IDC_eORACLE_PASSWORD, m_strOraclePassword);
	DDX_Text(pDX, IDC_eORACLE_SID, m_strOracleSID);
	DDX_Text(pDX, IDC_eORACLE_TEST_RESULT, m_strTestResult);
	DDX_Text(pDX, IDC_DMBS2, m_strK3DBMS);
	DDX_Text(pDX, IDC_eORACLE_ID2, m_strK3OracleID);
	DDX_Text(pDX, IDC_eORACLE_PASSWORD2, m_strK3OraclePassword);
	DDX_Text(pDX, IDC_eORACLE_SID2, m_strK3OracleSID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectSetupPage, CPropertyPage)
	//{{AFX_MSG_MAP(CConnectSetupPage)
	ON_EN_CHANGE(IDC_eORACLE_ID, OnChangeeORACLEID)
	ON_EN_CHANGE(IDC_eORACLE_PASSWORD, OnChangeeORACLEPASSWORD)
	ON_EN_CHANGE(IDC_eORACLE_SID, OnChangeeORACLESID)
	ON_BN_CLICKED(IDC_bORACLE_CONNECT_TEST, OnbORACLECONNECTTEST)
	ON_BN_CLICKED(IDC_bORACLE_REG_TEST, OnbORACLEREGTEST)
	ON_EN_CHANGE(IDC_eORACLE_ID2, OnChangeeORACLEID2)
	ON_EN_CHANGE(IDC_eORACLE_PASSWORD2, OnChangeeORACLEPASSWORD2)
	ON_EN_CHANGE(IDC_eORACLE_SID2, OnChangeeORACLESID2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectSetupPage Operations
INT CConnectSetupPage::LoadConfig()
{
 	//++2008.09.30 UPDATE BY CJY {{++
	// UseDMInfoChange.cfg 파일을 복호화하여 읽어들인다.
	// DES클래스의 Simple_Decipher 함수에서 결과값 설정시 힙 오류가 발생하여 
	// 이 곳에 해당 함수내용을 풀어서 작성
	CString strFile = _T("..\\cfg\\UseDMInfoChange.cfg");
	CString strKey = _T("k^Ty");
	CString strDMInfo = _T("");

	/////////////////////////////////////////////////////////////////////////////////
	// Read_DES_File
	// 1. 암호문 파일을 읽는다.
	CFile file;
	CFileException ex;	
	BOOL bIsOpen = file.Open(strFile, CFile::modeRead, &ex);
	if(FALSE == bIsOpen)
	{		
		TCHAR szMsg[1024];
		ex.GetErrorMessage(szMsg, 1024);
		TRACE(szMsg);
		return -1001;
	}
	
	int nLen = file.GetLength()/sizeof(TCHAR);
	TCHAR* pChar = new TCHAR[nLen+1];
	ZeroMemory(pChar, nLen);
	pChar[nLen] = '\0';
	
	file.Read(pChar, nLen*sizeof(TCHAR));
	file.Close();
	
	// 2. 암호문을 평문으로 복호화한다.	
	CString strSrc = _T("");	
	strSrc.Format(_T("%s"), pChar);
	// Read_DES_File
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	// Simple_Decipher

	// 1. CString을 UCHAR로 변환
	UCHAR* szSrc = NULL;
	UINT nSrcLen = 0;

	UCHAR* szDst = NULL;
	UINT nDstLen = 0;

	UCHAR* szKey = NULL;

	// 암호문이 8바이트의 배수가 아니면 잘못된 문장
	if(0 != strSrc.GetLength()% (8/sizeof(TCHAR)) )
	{
		return -35001;
	}
	{
		// 소스 변환
		UINT LEN = strSrc.GetLength();
		TCHAR* sz_Tmp = new TCHAR[LEN+1];
		ZeroMemory(sz_Tmp, LEN);
		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strSrc.GetBuffer(0));
		strSrc.ReleaseBuffer();
		sz_Tmp[LEN] = '\0';

		szSrc = new UCHAR[LEN*sizeof(TCHAR)+1];
		ZeroMemory(szSrc, LEN*sizeof(TCHAR));
		szSrc[LEN*sizeof(TCHAR)] = '\0';

		// TCHAR를 바이트단위로 나누어 UCHAR에 설정한다.
		for(UINT uCnt = 0; uCnt < LEN; uCnt++)
		{
			TCHAR Tmp = sz_Tmp[uCnt];
			szSrc[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szSrc[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}		
		nSrcLen = (LEN) * sizeof(TCHAR);

		delete []sz_Tmp;
	}
	{
		// 결과물 메모리 할당
		UINT LEN = strSrc.GetLength();
		szDst = new UCHAR[LEN*sizeof(TCHAR)+1];
		ZeroMemory(szDst, LEN*sizeof(TCHAR));
		szDst[LEN*sizeof(TCHAR)] = '\0';		
	}	
	{
		// 키 메모리 할당
		const INT KEY_LENGTH = 8/sizeof(TCHAR); // 키는 8바이트
		while(KEY_LENGTH > strKey.GetLength())
		{
			strKey += _T(" ");
		}
		strKey = strKey.Left(KEY_LENGTH);

		TCHAR* sz_Tmp = new TCHAR[KEY_LENGTH+1];
		ZeroMemory(sz_Tmp, KEY_LENGTH);
		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strKey.GetBuffer(0));
		strKey.ReleaseBuffer();
		sz_Tmp[KEY_LENGTH] = '\0';

		szKey = new UCHAR[KEY_LENGTH*sizeof(TCHAR)+1];
		ZeroMemory(szKey, KEY_LENGTH*sizeof(TCHAR));
		szKey[KEY_LENGTH] = '\0';
		for(int uCnt = 0; uCnt < KEY_LENGTH; uCnt++)
		{
			TCHAR Tmp = sz_Tmp[uCnt];
			szKey[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szKey[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}

		delete []sz_Tmp;
	}
	
	// 2. 복호화
	CFWDES DES;
	DES.Decipher(szKey, nSrcLen, szSrc, &nDstLen, szDst);
	
	// 3. CString 변환
	{
//DEL 		const UINT LEN = nDstLen/sizeof(TCHAR);
//DEL 		TCHAR* sz_Tmp = new TCHAR[LEN+1];
//DEL 		ZeroMemory(sz_Tmp, LEN);
//DEL 		sz_Tmp[LEN] = '\0';
//DEL 		
//DEL 		for(UINT nCnt = 0; nCnt < LEN; nCnt++)
//DEL 		{			
//DEL 			sz_Tmp[nCnt] = szDst[nCnt*2];
//DEL 			sz_Tmp[nCnt] = sz_Tmp[nCnt] << 8;
//DEL 			sz_Tmp[nCnt] += szDst[(nCnt*2)+1];
//DEL 		}		
//DEL 		strDMInfo.Format(_T("%s"), sz_Tmp);
		UCHAR* sz_Tmp = new UCHAR[nDstLen+2];
		ZeroMemory(sz_Tmp, nDstLen+1);
		sz_Tmp[nDstLen+1] = '\0';

		
		for(UINT nCnt = 0; nCnt < nDstLen; nCnt++)
		{			
			sz_Tmp[nCnt] = szDst[nCnt];			
		}		
	 	strDMInfo.Format(_T("%s"), (TCHAR*)sz_Tmp);
		delete []sz_Tmp;
	}

	// 4. 메모리 해제
	delete []szKey;
	delete []szSrc;
	delete []szDst;

	// Simple_Decipher
	/////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////
	// 여기서부터 접속정보 설정
	if(
		-1 == strDMInfo.Find(_T("MODE")) ||
		-1 == strDMInfo.Find(_T("BEFORE")) ||
		-1 == strDMInfo.Find(_T("AFTER"))
		)
	{
		strDMInfo = 
			_T("MODE = TRUE\r\n")
			_T("BEFORE = kolas2@k2up/kolas2#[]\r\n")
			_T("AFTER = kolas@kolas3/eco_kolas3#[]\r\n");
	}


	CString strMode = _T("");
	CString strDBInfoBefore = _T("");
	CString strDBInfoAfter = _T("");
	
	CString strAlias = _T("");
	CString strLine = _T("");

	CString info = _T("");
	
	for(int nCnt=0; nCnt < 3; nCnt++)
	{
		AfxExtractSubString(strLine, strDMInfo, nCnt, '\n');
		AfxExtractSubString(strAlias, strLine, 0, '=');
		
		strAlias.TrimLeft();
		strAlias.TrimRight();
		if(_T("MODE") == strAlias)
		{
			AfxExtractSubString(strMode, strLine, 1, '=');
			strMode.TrimLeft();
			strMode.TrimRight();
		}
		else if(_T("BEFORE") == strAlias)
		{
			AfxExtractSubString(strDBInfoBefore, strLine, 1, '=');
			strDBInfoBefore.TrimLeft();
			strDBInfoBefore.TrimRight();
		}
		else if(_T("AFTER") == strAlias)
		{
			AfxExtractSubString(strDBInfoAfter, strLine, 1, '=');
			strDBInfoAfter.TrimLeft();
			strDBInfoAfter.TrimRight();
		}			
	}
	if(_T("TRUE") == strMode)
	{
		info = strDBInfoAfter;
	}
	else if(_T("FALSE") == strMode)
	{
		info = strDBInfoBefore;
	}
	
	_stscanf(info.GetBuffer(0), _T("%[^@]@%[^/]/%[^[]"), m_strOracleID.GetBuffer(256), m_strOracleSID.GetBuffer(256), m_strOraclePassword.GetBuffer(256));
	m_strOracleID.ReleaseBuffer();
	m_strOracleSID.ReleaseBuffer();
	m_strOraclePassword.ReleaseBuffer();

	m_strDBMS = _T("ORACLE");
	m_strSEQ = _T("ESL_SEQ");
	m_strDevID = _T("이씨오");

	m_bIsLoad = TRUE;
	m_strDMInfo = strDMInfo;
	// 여기까지 접속정보 설정
	/////////////////////////////////////////////////////////////////////////////////

	UpdateData(FALSE);
//DEL	이전내용
//DEL 
//DEL 
//DEL 	// 설치 전 모드이면 현재 디렉토리에서 파일을 연다.
//DEL 	LPCTSTR  lpszFileName = NULL;
//DEL 	CStdioFile file;
//DEL 	CFileFind finder;
//DEL 
//DEL #ifdef _UNICODE
//DEL 	if (((CSetupMainSheet*)GetParent())->m_nIDCaption == IDS_PRE_SETUP)
//DEL 		lpszFileName = _T("refeditor_uni.cfg");
//DEL 	else
//DEL 		lpszFileName = _T("..\\cfg\\RefEditor\\refeditor_uni.cfg");
//DEL #else
//DEL 	if (((CSetupMainSheet*)GetParent())->m_nIDCaption == IDS_PRE_SETUP)
//DEL 		lpszFileName = _T("refeditor_mbcs.cfg");
//DEL 	else
//DEL 		lpszFileName = _T("..\\cfg\\RefEditor\\refeditor_mbcs.cfg");
//DEL #endif
//DEL 	
//DEL 
//DEL 	if (!finder.FindFile(lpszFileName))
//DEL 	{
//DEL 		// 파일을 생성한다.
//DEL 		if (!file.Open(lpszFileName, CFile::modeRead | CFile::modeCreate | CFile::typeBinary))
//DEL 		{
//DEL 			MessageBox(_T("파일을 생성할수 없습니다."));
//DEL 			return -1;
//DEL 		}
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		finder.FindNextFile();
//DEL 		// 찾은 파일이 읽기전용이면 풀어준다. 못풀어주면.. 시디롬인줄 알고 에러..
//DEL 		if (finder.IsReadOnly())
//DEL 		{
//DEL 			if (!SetFileAttributes(lpszFileName, FILE_ATTRIBUTE_NORMAL))
//DEL 			{
//DEL 				MessageBox(_T("쓸수 없는 매체의 파일입니다."));
//DEL 				return -1;
//DEL 			}
//DEL 		}
//DEL 
//DEL 		// 파일을 연다.
//DEL 		if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary))
//DEL 		{
//DEL 			MessageBox(_T("파일을 열수 없습니다."));
//DEL 			return -1;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	m_strFileName = lpszFileName;
//DEL 
//DEL 	CString strName;
//DEL 	CString strValue;
//DEL 	
//DEL 	CString strLine;
//DEL 	
//DEL 	TCHAR cUni;
//DEL 	file.Read( &cUni, sizeof(TCHAR) );
//DEL 	if( 0xFEFF != cUni )
//DEL 	{
//DEL 		file.SeekToBegin();
//DEL 	}
//DEL 	while (file.ReadString(strLine))
//DEL 	{
//DEL 		strName.Empty();
//DEL 		strValue.Empty();
//DEL 		strLine.TrimLeft();
//DEL 		strLine.TrimRight();
//DEL 
//DEL 		if (strLine.IsEmpty() || strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;
//DEL 
//DEL 		_stscanf(strLine.GetBuffer(0), _T("%s%s"), strName.GetBuffer(256), strValue.GetBuffer(256));
//DEL 		strName.ReleaseBuffer();
//DEL 		strValue.ReleaseBuffer();
//DEL 
//DEL 		strName.TrimLeft();
//DEL 		strName.TrimRight();
//DEL 
//DEL 		strValue.TrimLeft();
//DEL 		strValue.TrimRight();
//DEL 
//DEL 		if (strName.IsEmpty() || strValue.IsEmpty()) continue;
//DEL 
//DEL 		if (strName == _T("HOST"))
//DEL 		{
//DEL 			//kolas2@kolas2/kolas2#
//DEL 			_stscanf(strValue.GetBuffer(0), _T("%[^@]@%[^/]/%[^[]"), m_strOracleID.GetBuffer(256), m_strOracleSID.GetBuffer(256), m_strOraclePassword.GetBuffer(256));
//DEL 			m_strOracleID.ReleaseBuffer();
//DEL 			m_strOracleSID.ReleaseBuffer();
//DEL 			m_strOraclePassword.ReleaseBuffer();
//DEL 		}
//DEL 		else if (strName == _T("DB"))
//DEL 		{
//DEL 			m_strDBMS = strValue;
//DEL 		}
//DEL 		else if (strName == _T("ID"))
//DEL 		{
//DEL 			m_strDevID = strValue;
//DEL 		}
//DEL 		else if (strName == _T("SEQ"))
//DEL 		{
//DEL 			m_strSEQ = strValue;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	if (m_strSEQ.IsEmpty())
//DEL 		m_strSEQ = _T("esl_seq");
//DEL 	if (m_strDevID.IsEmpty())
//DEL 		m_strDevID = _T("ECO");
//DEL 	if (m_strDBMS.IsEmpty())
//DEL 		m_strDBMS = _T("ORACLE");
//DEL 	
//DEL 	file.Close();

//DEL 	UpdateData(FALSE);
	//--2008.09.30 UPDATE BY CJY --}}

	// 2009.05.12 ADD BY CJY : 이동도서관일때의 k3접속설정 불러오기
	int ids = LoadConfig2();
	return ids;
	
// 	return 0;
}

INT CConnectSetupPage::SaveConfing()
{
	//++2008.09.30 UPDATE BY CJY {{++
	UpdateData(TRUE);
{	
	CString strMode = _T("");

	CString strAlias = _T("");
	CString strLine = _T("");
	CString strModeLine = _T("");
	CString strBeforeLine = _T("");
	CString strAfterLine = _T("");
	
	CString strDMInfo = m_strDMInfo;
	if(FALSE == m_bIsLoad ||
		-1 == m_strDMInfo.Find(_T("MODE")) ||
		-1 == m_strDMInfo.Find(_T("BEFORE")) ||
		-1 == m_strDMInfo.Find(_T("AFTER"))
		)
	{
		strDMInfo = 
			_T("MODE = TRUE\r\n")
			_T("BEFORE = kolas2@k2up/kolas2#[]\r\n")
			_T("AFTER = kolas@kolas3/eco_kolas3#[]\r\n");
	}

	for(int nCnt=0; nCnt < 3; nCnt++)
	{
		AfxExtractSubString(strLine, strDMInfo, nCnt, '\n');
		AfxExtractSubString(strAlias, strLine, 0, '=');
		
		strAlias.TrimLeft();
		strAlias.TrimRight();
		if(_T("MODE") == strAlias)
		{
			AfxExtractSubString(strMode, strLine, 1, '=');
			strMode.TrimLeft();
			strMode.TrimRight();

			strModeLine = strLine;
		}
		else if(_T("BEFORE") == strAlias)
		{
			strBeforeLine = strLine;
		}
		else if(_T("AFTER") == strAlias)
		{
			strAfterLine = strLine;
		}			
	}


	CString info = _T("");
	info.Format(
		_T("%s@%s/%s[]\r\n"), 
		m_strOracleID, 
		m_strOracleSID, 
		m_strOraclePassword
		);

	if(_T("TRUE") == strMode)
	{
		info = _T("AFTER = ") + info;
		strDMInfo.Replace(strAfterLine, info);		
	}
	else if(_T("FALSE") == strMode)
	{
		info = _T("BEFORE = ") + info;
		strDMInfo.Replace(strBeforeLine, info);
	}
		
	strDMInfo.TrimLeft();
	strDMInfo.TrimRight();

	// UseDMInfoChange.cfg 파일을 암호화하여 저장
	// DES클래스의 Simple_Cipher 함수에서 결과값 리턴시 힙 오류가 발생하여
	// 이 곳에 해당 함수내용을 풀어서 작성
	CString strFile = _T("..\\cfg\\UseDMInfoChange.cfg");
	CString strKey = _T("k^Ty");

	CString strSrc = strDMInfo;
	CString strDst = _T("");
	/////////////////////////////////////////////////////////////////////////////////
	// Simple_Cipher
	UCHAR* szSrc = NULL;
	UINT nSrcLen = 0;

	UCHAR* szDst = NULL;
	UINT nDstLen = 0;

	UCHAR* szKey = NULL;

	{
 		// 평문을 8바이트의 배수로 맞춰준다.
 		while(0 != strSrc.GetLength()% (8/sizeof(TCHAR)) )
 		{
 			strSrc += _T(" ");
 		}
	}
	{		
		// 소스 변환
		UINT LEN = strSrc.GetLength();

		TCHAR* sz_Tmp = new TCHAR[LEN+1];
		ZeroMemory(sz_Tmp, LEN);
		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strSrc.GetBuffer(0));
		strSrc.ReleaseBuffer();
		sz_Tmp[LEN] = '\0';

		szSrc = new UCHAR[LEN*sizeof(TCHAR)+1];
		ZeroMemory(szSrc, LEN*sizeof(TCHAR));
		szSrc[LEN*sizeof(TCHAR)] = '\0';

		// TCHAR를 바이트단위로 나누어 UCHAR에 설정한다.
		for(UINT uCnt = 0; uCnt < LEN; uCnt++)
		{
			TCHAR Tmp = sz_Tmp[uCnt];
			szSrc[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szSrc[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}		
		nSrcLen = (LEN) * sizeof(TCHAR);

		delete []sz_Tmp;
	}
	{
		// 결과물 메모리 할당
		UINT LEN = strSrc.GetLength();
		szDst = new UCHAR[LEN*sizeof(TCHAR)+1];
		ZeroMemory(szDst, LEN*sizeof(TCHAR));
		szDst[LEN*sizeof(TCHAR)] = '\0';		
	}	
	{
		// 키 메모리 할당
		const INT KEY_LENGTH = 8/sizeof(TCHAR); // 키는 8바이트
		while(KEY_LENGTH > strKey.GetLength())
		{
			strKey += _T(" ");
		}
		strKey = strKey.Left(KEY_LENGTH);

		TCHAR* sz_Tmp = new TCHAR[KEY_LENGTH+1];
		ZeroMemory(sz_Tmp, KEY_LENGTH);
		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strKey.GetBuffer(0));
		strKey.ReleaseBuffer();
		sz_Tmp[KEY_LENGTH] = '\0';

		szKey = new UCHAR[KEY_LENGTH*sizeof(TCHAR)+1];
		ZeroMemory(szKey, KEY_LENGTH*sizeof(TCHAR));
		szKey[KEY_LENGTH] = '\0';
		for(int uCnt = 0; uCnt < KEY_LENGTH; uCnt++)
		{
			TCHAR Tmp = sz_Tmp[uCnt];
			szKey[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szKey[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}

		delete []sz_Tmp;
	}
	
	// 2. 암호화
	CFWDES DES;
	DES.Cipher(szKey, nSrcLen, szSrc, &nDstLen, szDst);
	
	// 3. CString 변환
	{
		UCHAR* sz_Tmp = new UCHAR[nDstLen+2];
		ZeroMemory(sz_Tmp, nDstLen+1);
		sz_Tmp[nDstLen+1] = '\0';

		
		for(UINT nCnt = 0; nCnt < nDstLen; nCnt++)
		{			
			sz_Tmp[nCnt] = szDst[nCnt];			
		}		
	 	strDst.Format(_T("%s"), (TCHAR*)sz_Tmp);
		delete []sz_Tmp;
	}

	// 4. 메모리 해제
	delete []szKey;
	delete []szSrc;
	delete []szDst;
	// Simple_Cipher
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	// Write_DES_File
//DEL 	CFileFind filefind;	
	UINT nOpenFlags = CFile::modeWrite|CFile::modeCreate|CFile::typeBinary;
//DEL 	if(TRUE == filefind.FindFile(strFile))
//DEL 	{
//DEL 		nOpenFlags ^= CFile::modeCreate;
//DEL 	}
	
	CFile file;
	CFileException ex;
 	BOOL bIsOpen = file.Open(strFile, nOpenFlags, &ex);
	if(FALSE == bIsOpen)
	{
		TCHAR szMsg[1024];
		ex.GetErrorMessage(szMsg, 1024);
		TRACE(szMsg);
		return -1001;
	}

	file.SeekToEnd();
	file.Write((TCHAR*)strDst.GetBuffer(0), (strDst.GetLength())*sizeof(TCHAR));
	strDst.ReleaseBuffer();
	file.Close();
	// Write_DES_File
	/////////////////////////////////////////////////////////////////////////////////	
}
	// 이전소스
//DEL 	// 접속정보를 UseDMInfoChange.cfg에 설정추가
//DEL 	{
//DEL 		BOOL bDBInfoMode = FALSE;		// TRUE 변환Ｏ, FALSE 변환Ｘ
//DEL 		CStdioFile fd;
//DEL 		CString info = _T(""), strDBInfoBefore = _T(""), strDBInfoAfter = _T("");
//DEL 		if (fd.Open(_T("..\\CFG\\UseDMInfoChange.cfg"), CStdioFile::modeRead|CFile::typeBinary) == TRUE) {
//DEL 			TCHAR cUni;
//DEL 			fd.Read( &cUni, sizeof(TCHAR) );
//DEL 			if( 0xFEFF != cUni )
//DEL 			{
//DEL 				fd.SeekToBegin();
//DEL 			}
//DEL 			CString strLine, strTag, strData;
//DEL 			while(TRUE) {
//DEL 				if (fd.ReadString(strLine) == FALSE) break;
//DEL 				strLine.TrimLeft();				strLine.TrimRight();
//DEL 				if (strLine.GetLength() == 0) continue;
//DEL 				
//DEL 				INT nPos = strLine.Find('=', 0);
//DEL 				if (nPos == -1) continue;
//DEL 				
//DEL 				strTag = strLine.Left(nPos);
//DEL 				strTag.MakeUpper();			strTag.TrimLeft();		strTag.TrimRight();
//DEL 				strData = strLine.Mid(nPos+1);
//DEL 				strData.TrimLeft();			strData.TrimRight();
//DEL 				
//DEL 				if (strTag == _T("MODE")) {
//DEL 					strData.MakeUpper();
//DEL 					if (strData == _T("TRUE")) bDBInfoMode = TRUE;
//DEL 					else bDBInfoMode = FALSE;
//DEL 				}
//DEL 				if (strTag == _T("BEFORE")) strDBInfoBefore = strData;
//DEL 				if (strTag == _T("AFTER")) strDBInfoAfter = strData;
//DEL 			}
//DEL 			fd.Close();
//DEL 			if (strDBInfoBefore.GetLength() == 0) bDBInfoMode = FALSE;
//DEL 			if (strDBInfoAfter.GetLength() == 0) bDBInfoMode = FALSE;
//DEL 		}
//DEL 		if (bDBInfoMode == TRUE) 
//DEL 		{
//DEL 			strDBInfoAfter.TrimLeft();		strDBInfoAfter.TrimRight();
//DEL 			strDBInfoAfter.MakeLower();
//DEL 			info = strDBInfoAfter;
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			strDBInfoBefore.TrimLeft();		strDBInfoBefore.TrimRight();
//DEL 			strDBInfoBefore.MakeLower();
//DEL 			info = strDBInfoBefore;
//DEL 		}
//DEL 		
//DEL 		if (fd.Open(_T("..\\CFG\\UseDMInfoChange.cfg"), CStdioFile::modeWrite|CFile::modeCreate|CFile::typeBinary) == TRUE) {
//DEL 			
//DEL 			TCHAR cUni = 0xFEFF;			
//DEL 			if( 0 == fd.GetLength() )
//DEL 			{			
//DEL 				fd.Write( &cUni, sizeof(TCHAR));
//DEL 			}
//DEL 			CString strMode = _T("MODE=");
//DEL 			if(TRUE==bDBInfoMode)
//DEL 			{
//DEL 				strMode = strMode + _T("TRUE\r\n");
//DEL 				strDBInfoBefore.TrimRight(_T("\r\n"));
//DEL 				strDBInfoBefore = _T("BEFORE=") + strDBInfoBefore + _T("\r\n");
//DEL 				
//DEL 				strDBInfoAfter.Format(_T("AFTER=%s@%s/%s[]\r\n"), m_strOracleID, m_strOracleSID, m_strOraclePassword);			
//DEL 				
//DEL 				fd.Write(strMode, strMode.GetLength() * sizeof(TCHAR));
//DEL 				fd.Write(strDBInfoBefore, strDBInfoBefore.GetLength() * sizeof(TCHAR));
//DEL 				fd.Write(strDBInfoAfter, strDBInfoAfter.GetLength() * sizeof(TCHAR));
//DEL 			}
//DEL 			else
//DEL 			{
//DEL 				strMode = strMode + _T("FALSE\r\n");
//DEL 				
//DEL 				strDBInfoBefore.Format(_T("BEFORE=%s@%s/%s[]\r\n"), m_strOracleID, m_strOracleSID, m_strOraclePassword);				
//DEL 				strDBInfoAfter.TrimRight(_T("\r\n"));
//DEL 				strDBInfoAfter = _T("AFTER=") + strDBInfoAfter + _T("\r\n");
//DEL 				
//DEL 				
//DEL 				fd.Write(strMode, strMode.GetLength() * sizeof(TCHAR));
//DEL 				fd.Write(strDBInfoBefore, strDBInfoBefore.GetLength() * sizeof(TCHAR));
//DEL 				fd.Write(strDBInfoAfter, strDBInfoAfter.GetLength() * sizeof(TCHAR));
//DEL 			}
//DEL 			fd.Close();
//DEL 		}
//DEL 	}
	//--2008.09.30 UPDATE BY CJY --}}
		
	// 2009.05.12 ADD BY CJY : 이동도서관용 K3접속설정
	int ids = SaveConfing2();
	return ids;


// 2009.05.12 DEL BY CJY : 사용되지 않는 파일
// 	if (m_strFileName.IsEmpty()) return -1;
// 
// 	CStdioFile file, file2;
// 	m_strFileName2 = _T("..\\CFG\\MOBILE\\MainDB.CFG");
// 	if (!file.Open(m_strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
// 	{
// 		CString strMsg;
// 		strMsg.Format(_T("[%s]파일을 열수 없습니다."), m_strFileName);
// 		return -1;
// 	}
// 	if (!file2.Open(m_strFileName2, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
// 	{
// 		CString strMsg;
// 		strMsg.Format(_T("[%s]파일을 열수 없습니다."), m_strFileName2);
// 		return -1;
// 	}
// 
// 	CString strLine;
// 	TCHAR cUni = 0xFEFF;
// 	
// 	if( 0 == file.GetLength() )
// 	{			
// 		file.Write( &cUni, sizeof(TCHAR));
// 	}
// 	if( 0 == file2.GetLength() )
// 	{			
// 		file2.Write( &cUni, sizeof(TCHAR));
// 	}
// 
// 	// HOST
// 	strLine.Format(_T("HOST %s@%s/%s[]\r\n"), m_strOracleID, m_strOracleSID, m_strOraclePassword);
// 	file.WriteString(strLine);
// 	file2.WriteString(strLine);
// 
// 	// ID
// 	strLine.Format(_T("ID %s\r\n"), m_strDevID);
// 	file.WriteString(strLine);
// 	file2.WriteString(strLine);
// 
// 	// SEQ
// 	strLine.Format(_T("SEQ %s\r\n"), m_strSEQ);
// 	file.WriteString(strLine);
// 	file2.WriteString(strLine);
// 
// 	// DB
// 	strLine.Format(_T("DB %s\r\n"), m_strDBMS);
// 	file.WriteString(strLine);
// 	file2.WriteString(strLine);
// 
// 	file.Close();
// 	file2.Close();

 	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CConnectSetupPage message handlers

BOOL CConnectSetupPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// 설정 파일을 읽어온다.
	if (LoadConfig() < 0)
	{
		m_bError = TRUE;
//DEL 		GetDlgItem(IDC_eORACLE_ID)->EnableWindow(FALSE);
//DEL 		GetDlgItem(IDC_eORACLE_PASSWORD)->EnableWindow(FALSE);
//DEL 		GetDlgItem(IDC_eORACLE_SID)->EnableWindow(FALSE);		
//DEL 		GetDlgItem(IDC_DMBS)->EnableWindow(FALSE);				
//DEL 
//DEL 		GetDlgItem(IDC_sORACLE_TEST)->ShowWindow(SW_HIDE);
//DEL 		GetDlgItem(IDC_bORACLE_CONNECT_TEST)->ShowWindow(SW_HIDE);
//DEL 		GetDlgItem(IDC_bORACLE_REG_TEST)->ShowWindow(SW_HIDE);
//DEL 		GetDlgItem(IDC_eORACLE_TEST_RESULT)->ShowWindow(SW_HIDE);
	}
		

	if (((CSetupMainSheet*)GetParent())->m_nIDCaption == IDS_PRE_SETUP)
	{
		GetDlgItem(IDC_sORACLE_TEST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_bORACLE_CONNECT_TEST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_bORACLE_REG_TEST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_eORACLE_TEST_RESULT)->ShowWindow(SW_HIDE);
	}	

	// 2009.05.12 ADD BY CJY : 이동도서관 K3접속설정 암호화
	CFileFind FF;
	BOOL bIsK3 = FF.FindFile(_T("KOLASIII.exe"));
	BOOL bIsK3Mobile = FF.FindFile(_T("KOLASIII MOBILE.exe"));	

	if(FALSE == bIsK3 && TRUE == bIsK3Mobile)
	{
		const int K3WND_CNT = 9;
		UINT uK3ID[9] = 
		{
			IDC_STATIC_K3,
			IDC_STATIC_K3_ID,
			IDC_STATIC_K3_PASSWORD,
			IDC_STATIC_K3_SERVICE,
			IDC_STATIC_K3_DBMS,
			IDC_eORACLE_ID2,
			IDC_eORACLE_PASSWORD2,
			IDC_eORACLE_SID2,
			IDC_DMBS2
		};

		CWnd* pWnd = NULL;
		int nWndCnt = 0;
		for(nWndCnt = 0; nWndCnt < K3WND_CNT; nWndCnt++)
		{
			pWnd = GetDlgItem(uK3ID[nWndCnt]);
			if(NULL != pWnd)
			{
				pWnd->ShowWindow(SW_SHOW);
			}
		};
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CConnectSetupPage::OnApply() 
{
//DEL 	if (m_bError) return FALSE;
	SaveConfing();
	
	return CPropertyPage::OnApply();
}


VOID CConnectSetupPage::OnChangeeORACLEID() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();
	
}

VOID CConnectSetupPage::OnChangeeORACLEPASSWORD() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();
}

VOID CConnectSetupPage::OnChangeeORACLESID() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();
}

VOID CConnectSetupPage::OnbORACLECONNECTTEST() 
{
	BeginWaitCursor();

	UpdateData();

	CString strUserName;
	strUserName.Format(_T("%s@%s"), m_strOracleID, m_strOracleSID);

	INT nRequestID;
	nRequestID = GetSessionRequestID();
	if (ConnectDB(nRequestID, strUserName.GetBuffer(0), m_strOraclePassword.GetBuffer(0)) < 0)
	{
		m_strTestResult = _T("접속 실패");
	}
	else
	{
		m_strTestResult = _T("접속 성공");
		DisconnectDB();
	}
	strUserName.ReleaseBuffer();
	m_strOraclePassword.ReleaseBuffer();

	UpdateData(FALSE);
	EndWaitCursor();
}

VOID CConnectSetupPage::OnbORACLEREGTEST() 
{
	m_strTestResult.Empty();

	HKEY hKey;
	TCHAR szBuffer[1024];
	DWORD dwBufLen = sizeof(TCHAR) * 1024;

	FILE *fp = NULL;

	LPCTSTR lpSubKey = NULL;
	LPCTSTR lpValueName = NULL;
	LPCTSTR lpData = NULL;
	CString strDefaultHome;
	INT i = -1;
	INT nLastHome = -1;
	INT nHomeCounter = -1;
	CString strTemp;
	CString strMsg;

	BOOL bFindOracleHome = FALSE;

	// HKEY_LOCAL_MACHINE/SOFTWARE/ORACLE 확인
	lpSubKey = _T("SOFTWARE\\ORACLE");
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
	{
		m_strTestResult.Format(_T("HKEY_LOCAL_MACHINE\\%s 키가 존재하지 않습니다."), lpSubKey);
		goto END;
	}

	lpSubKey = _T("SOFTWARE\\ORACLE\\ALL_HOMES");
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
	{
		m_strTestResult.Format(_T("HKEY_LOCAL_MACHINE\\%s 키가 존재하지 않습니다."), lpSubKey);
		goto END;
	}

	// DEFAULT_HOME을 가져온다.
	lpValueName = _T("DEFAULT_HOME");
	dwBufLen = sizeof(TCHAR) * 1024;
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		m_strTestResult.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), lpSubKey, lpValueName);
		goto END;
	}

	strDefaultHome.Format(_T("%s"), szBuffer);

	// HOME_COUNTER를 가져온다.
	lpValueName = _T("HOME_COUNTER");
	dwBufLen = sizeof(TCHAR) * 1024;
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		m_strTestResult.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), lpSubKey, lpValueName);
		goto END;
	}
	nHomeCounter = _ttoi(szBuffer);

	// LAST_HOME을 가져온다.
	lpValueName = _T("LAST_HOME");
	dwBufLen = sizeof(TCHAR) * 1024;
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		m_strTestResult.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), lpSubKey, lpValueName);
		goto END;
	}
	nLastHome = _ttoi(szBuffer);

	RegCloseKey( hKey );

	// DEFAULT_HOME이 가리키는 KEY 확인
	for (i = 0; i <= nLastHome; i++)
	{
		strTemp.Format(_T("SOFTWARE\\ORACLE\\HOME%d"), i);
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, strTemp, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &hKey) != ERROR_SUCCESS)
		{
			m_strTestResult.Format(_T("HKEY_LOCAL_MACHINE\\%s 키가 존재하지 않습니다."), strTemp);
			goto END;
		}

		// ORACLE_HOME_NAME 이 맞는지 확인
		lpValueName = _T("ORACLE_HOME_NAME");
		dwBufLen = sizeof(TCHAR) * 1024;
		if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
		{
			m_strTestResult.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), strTemp, lpValueName);
			goto END;
		}

		if (_tcscmp(szBuffer, strDefaultHome) == 0) 
		{
			bFindOracleHome = TRUE;
			break;		
		}
	}

	if (!bFindOracleHome) 
	{
		m_strTestResult = _T("DEFAULT HOME을 찾을수 없습니다.");
		goto END;
	}

	// ORACLE_HOME을 확인한다. tnsname.ora로 확인 ㅋㅋ
	lpValueName = _T("ORACLE_HOME");
	dwBufLen = sizeof(TCHAR) * 1024;
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		m_strTestResult.Format(_T("HKEY_LOCAL_MACHINE\\%s의 %s 문자열이 존재하지 않습니다."), strTemp, lpValueName);
		goto END;
	}

	strTemp.Format(_T("%s\\network\\admin\\tnsnames.ora"), szBuffer);
	fp = _tfopen(strTemp.GetBuffer(0), _T("rb"));
	strTemp.ReleaseBuffer();
	if (fp == NULL)
	{
		m_strTestResult.Format(_T("ORACLE_HOME과 tnsnames.ora 파일의 위치가 일치하지 않습니다."));
		goto END;
	}
	fclose(fp);

	// NLS_LANG과 NLS_DATE_FORMAT은 될 수 있는한 고쳐주려고 애쓴다... ㅋㅋ

	// NLS_LANG 확인
	lpValueName = _T("NLS_LANG");
	//lpData = _T("KOREAN_KOREA.KO16KSC5601");
	lpData = _T("KOREAN_KOREA.KO16MSWIN949");
	dwBufLen = sizeof(TCHAR) * 1024;
	ZeroMemory(szBuffer, dwBufLen);
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		strMsg.Format(_T("ORACLE_HOME의 %s 문자열이 존재하지 않습니다.->[생성함]\r\n"), lpValueName, lpValueName);
		m_strTestResult += strMsg;
	}

	if (_tcscmp(szBuffer, lpData) != 0)
	{
		if (RegSetValueEx(hKey, lpValueName, 0, REG_SZ, (LPBYTE)lpData, _tcslen(lpData) + 1) != ERROR_SUCCESS)
			goto END;

		strMsg.Format(_T("NLS_LANG 이 잘못되어서 수정했습니다. \r\n [%s->%s]\r\n"), szBuffer, lpData);
		m_strTestResult += strMsg;	
	}


	// NLS_DATE_FORMAT 확인
	lpValueName = _T("NLS_DATE_FORMAT");
	lpData = _T("YYYY/MM/DD");
	dwBufLen = sizeof(TCHAR) * 1024;
	ZeroMemory(szBuffer, dwBufLen);
	if (RegQueryValueEx(hKey, lpValueName, 0, NULL, (LPBYTE)szBuffer, &dwBufLen) != ERROR_SUCCESS)
	{
		strMsg.Format(_T("ORACLE_HOME의 %s 문자열이 존재하지 않습니다.->[생성함]\r\n"), lpValueName, lpValueName);
		m_strTestResult += strMsg;
	}

	if (_tcscmp(szBuffer, _T("YYYY/MM/DD")) != 0)
	{
		if (RegSetValueEx(hKey, lpValueName, 0, REG_SZ, (LPBYTE)lpData, _tcslen(lpData) + 1) != ERROR_SUCCESS)
			goto END;

		strMsg.Format(_T("NLS_DATE_FORMAT 이 잘못되어서 수정했습니다. \r\n [%s->%s]"), szBuffer, lpData);
		m_strTestResult += strMsg;
	}

	m_strTestResult += _T("검사 완료...");

END:
	RegCloseKey( hKey );
	UpdateData(FALSE);
}


/*
@Create
 - 2009.05.12 BY CJY
@Description
 - 이동도서관 환경설정에서의 k3접속설정 불러오기
@Parameter
@Return Value
 - INT 
 - 0, 양수 : 성공
 - 음수 : 실패
@Mini Spec
 1. 
*/
INT CConnectSetupPage::LoadConfig2()
{
 	//++2008.09.30 UPDATE BY CJY {{++
	// UseDMInfoChange.cfg 파일을 복호화하여 읽어들인다.
	// DES클래스의 Simple_Decipher 함수에서 결과값 설정시 힙 오류가 발생하여 
	// 이 곳에 해당 함수내용을 풀어서 작성
	CString strFile = _T("..\\cfg\\Mobile\\MainDB.cfg");
	CString strKey = _T("k^Ty");
	CString strDMInfo = _T("");

	/////////////////////////////////////////////////////////////////////////////////
	// Read_DES_File
	// 1. 암호문 파일을 읽는다.
	CFile file;
	CFileException ex;	
	BOOL bIsOpen = file.Open(strFile, CFile::modeRead, &ex);
	if(FALSE == bIsOpen)
	{		
		TCHAR szMsg[1024];
		ex.GetErrorMessage(szMsg, 1024);
		TRACE(szMsg);
		return -1001;
	}
	
	int nLen = file.GetLength()/sizeof(TCHAR);
	TCHAR* pChar = new TCHAR[nLen+1];
	ZeroMemory(pChar, nLen);
	pChar[nLen] = '\0';
	
	file.Read(pChar, nLen*sizeof(TCHAR));
	file.Close();
	
	// 2. 암호문을 평문으로 복호화한다.	
	CString strSrc = _T("");	
	strSrc.Format(_T("%s"), pChar);
	// Read_DES_File
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	// Simple_Decipher

	// 1. CString을 UCHAR로 변환
	UCHAR* szSrc = NULL;
	UINT nSrcLen = 0;

	UCHAR* szDst = NULL;
	UINT nDstLen = 0;

	UCHAR* szKey = NULL;

	// 암호문이 8바이트의 배수가 아니면 잘못된 문장
	if(0 != strSrc.GetLength()% (8/sizeof(TCHAR)) )
	{
		return -35001;
	}
	{
		// 소스 변환
		UINT LEN = strSrc.GetLength();
		TCHAR* sz_Tmp = new TCHAR[LEN+1];
		ZeroMemory(sz_Tmp, LEN);
		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strSrc.GetBuffer(0));
		strSrc.ReleaseBuffer();
		sz_Tmp[LEN] = '\0';

		szSrc = new UCHAR[LEN*sizeof(TCHAR)+1];
		ZeroMemory(szSrc, LEN*sizeof(TCHAR));
		szSrc[LEN*sizeof(TCHAR)] = '\0';

		// TCHAR를 바이트단위로 나누어 UCHAR에 설정한다.
		for(UINT uCnt = 0; uCnt < LEN; uCnt++)
		{
			TCHAR Tmp = sz_Tmp[uCnt];
			szSrc[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szSrc[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}		
		nSrcLen = (LEN) * sizeof(TCHAR);

		delete []sz_Tmp;
	}
	{
		// 결과물 메모리 할당
		UINT LEN = strSrc.GetLength();
		szDst = new UCHAR[LEN*sizeof(TCHAR)+1];
		ZeroMemory(szDst, LEN*sizeof(TCHAR));
		szDst[LEN*sizeof(TCHAR)] = '\0';		
	}	
	{
		// 키 메모리 할당
		const INT KEY_LENGTH = 8/sizeof(TCHAR); // 키는 8바이트
		while(KEY_LENGTH > strKey.GetLength())
		{
			strKey += _T(" ");
		}
		strKey = strKey.Left(KEY_LENGTH);

		TCHAR* sz_Tmp = new TCHAR[KEY_LENGTH+1];
		ZeroMemory(sz_Tmp, KEY_LENGTH);
		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strKey.GetBuffer(0));
		strKey.ReleaseBuffer();
		sz_Tmp[KEY_LENGTH] = '\0';

		szKey = new UCHAR[KEY_LENGTH*sizeof(TCHAR)+1];
		ZeroMemory(szKey, KEY_LENGTH*sizeof(TCHAR));
		szKey[KEY_LENGTH] = '\0';
		for(int uCnt = 0; uCnt < KEY_LENGTH; uCnt++)
		{
			TCHAR Tmp = sz_Tmp[uCnt];
			szKey[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szKey[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}

		delete []sz_Tmp;
	}
	
	// 2. 복호화
	CFWDES DES;
	DES.Decipher(szKey, nSrcLen, szSrc, &nDstLen, szDst);
	
	// 3. CString 변환
	{
//DEL 		const UINT LEN = nDstLen/sizeof(TCHAR);
//DEL 		TCHAR* sz_Tmp = new TCHAR[LEN+1];
//DEL 		ZeroMemory(sz_Tmp, LEN);
//DEL 		sz_Tmp[LEN] = '\0';
//DEL 		
//DEL 		for(UINT nCnt = 0; nCnt < LEN; nCnt++)
//DEL 		{			
//DEL 			sz_Tmp[nCnt] = szDst[nCnt*2];
//DEL 			sz_Tmp[nCnt] = sz_Tmp[nCnt] << 8;
//DEL 			sz_Tmp[nCnt] += szDst[(nCnt*2)+1];
//DEL 		}		
//DEL 		strDMInfo.Format(_T("%s"), sz_Tmp);
		UCHAR* sz_Tmp = new UCHAR[nDstLen+2];
		ZeroMemory(sz_Tmp, nDstLen+1);
		sz_Tmp[nDstLen+1] = '\0';

		
		for(UINT nCnt = 0; nCnt < nDstLen; nCnt++)
		{			
			sz_Tmp[nCnt] = szDst[nCnt];			
		}		
	 	strDMInfo.Format(_T("%s"), (TCHAR*)sz_Tmp);
		delete []sz_Tmp;
	}

	// 4. 메모리 해제
	delete []szKey;
	delete []szSrc;
	delete []szDst;

	// Simple_Decipher
	/////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////
	// 여기서부터 접속정보 설정

	CString strMode = _T("");
// 	CString strDBInfoBefore = _T("");
// 	CString strDBInfoAfter = _T("");
	
	CString strAlias = _T("");
	CString strValue = _T("");
	CString strLine = _T("");

	CString info = _T("");
	
	for(int nCnt=0; nCnt < 4; nCnt++)
	{
		AfxExtractSubString(strLine, strDMInfo, nCnt, '\n');

		AfxExtractSubString(strAlias, strLine, 0, ' ');
		AfxExtractSubString(strValue, strLine, 1, ' ');		

		strAlias.TrimLeft();
		strAlias.TrimRight();
		strValue.TrimLeft();
		strValue.TrimRight();

		if(_T("HOST") == strAlias)
		{
			info = strValue;
		}
		else if(_T("DB") == strAlias)
		{
			m_strK3DBMS = strValue;
		}
	}
	
	_stscanf(info.GetBuffer(0), _T("%[^@]@%[^/]/%[^[]"), m_strK3OracleID.GetBuffer(256), m_strK3OracleSID.GetBuffer(256), m_strK3OraclePassword.GetBuffer(256));
	m_strK3OracleID.ReleaseBuffer();
	m_strK3OracleSID.ReleaseBuffer();
	m_strK3OraclePassword.ReleaseBuffer();

	if(TRUE == m_strK3DBMS.IsEmpty())
	{
		m_strK3DBMS = _T("ORACLE");
	}
	// 여기까지 접속정보 설정
	/////////////////////////////////////////////////////////////////////////////////
	UpdateData(FALSE);

	return 0;
}

INT CConnectSetupPage::SaveConfing2()
{
	UpdateData(TRUE);
	CString strMode = _T("");

	CString strAlias = _T("");
	CString strLine = _T("");
	CString strModeLine = _T("");
	CString strBeforeLine = _T("");
	CString strAfterLine = _T("");
	
	CString strInfo = _T("");
	strInfo.Format(
		_T("%s@%s/%s[]"), 
		m_strK3OracleID, 
		m_strK3OracleSID, 
		m_strK3OraclePassword
		);

	CString strDMInfo = _T("");
	strDMInfo.Format(
		_T("HOST %s\r\n")
		_T("ID 이씨오\r\n")
		_T("SEQ ESL_SEQ\r\n")
		_T("DB %s"),
		strInfo, m_strK3DBMS);

	// UseDMInfoChange.cfg 파일을 암호화하여 저장
	// DES클래스의 Simple_Cipher 함수에서 결과값 리턴시 힙 오류가 발생하여
	// 이 곳에 해당 함수내용을 풀어서 작성
	CString strFile = _T("..\\cfg\\Mobile\\MainDB.cfg");
	CString strKey = _T("k^Ty");

	CString strSrc = strDMInfo;
	CString strDst = _T("");
	/////////////////////////////////////////////////////////////////////////////////
	// Simple_Cipher
	UCHAR* szSrc = NULL;
	UINT nSrcLen = 0;

	UCHAR* szDst = NULL;
	UINT nDstLen = 0;

	UCHAR* szKey = NULL;

	{
 		// 평문을 8바이트의 배수로 맞춰준다.
 		while(0 != strSrc.GetLength()% (8/sizeof(TCHAR)) )
 		{
 			strSrc += _T(" ");
 		}
	}
	{		
		// 소스 변환
		UINT LEN = strSrc.GetLength();

		TCHAR* sz_Tmp = new TCHAR[LEN+1];
		ZeroMemory(sz_Tmp, LEN);
		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strSrc.GetBuffer(0));
		strSrc.ReleaseBuffer();
		sz_Tmp[LEN] = '\0';

		szSrc = new UCHAR[LEN*sizeof(TCHAR)+1];
		ZeroMemory(szSrc, LEN*sizeof(TCHAR));
		szSrc[LEN*sizeof(TCHAR)] = '\0';

		// TCHAR를 바이트단위로 나누어 UCHAR에 설정한다.
		for(UINT uCnt = 0; uCnt < LEN; uCnt++)
		{
			TCHAR Tmp = sz_Tmp[uCnt];
			szSrc[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szSrc[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}		
		nSrcLen = (LEN) * sizeof(TCHAR);

		delete []sz_Tmp;
	}
	{
		// 결과물 메모리 할당
		UINT LEN = strSrc.GetLength();
		szDst = new UCHAR[LEN*sizeof(TCHAR)+1];
		ZeroMemory(szDst, LEN*sizeof(TCHAR));
		szDst[LEN*sizeof(TCHAR)] = '\0';		
	}	
	{
		// 키 메모리 할당
		const INT KEY_LENGTH = 8/sizeof(TCHAR); // 키는 8바이트
		while(KEY_LENGTH > strKey.GetLength())
		{
			strKey += _T(" ");
		}
		strKey = strKey.Left(KEY_LENGTH);

		TCHAR* sz_Tmp = new TCHAR[KEY_LENGTH+1];
		ZeroMemory(sz_Tmp, KEY_LENGTH);
		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strKey.GetBuffer(0));
		strKey.ReleaseBuffer();
		sz_Tmp[KEY_LENGTH] = '\0';

		szKey = new UCHAR[KEY_LENGTH*sizeof(TCHAR)+1];
		ZeroMemory(szKey, KEY_LENGTH*sizeof(TCHAR));
		szKey[KEY_LENGTH] = '\0';
		for(int uCnt = 0; uCnt < KEY_LENGTH; uCnt++)
		{
			TCHAR Tmp = sz_Tmp[uCnt];
			szKey[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szKey[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}

		delete []sz_Tmp;
	}
	
	// 2. 암호화
	CFWDES DES;
	DES.Cipher(szKey, nSrcLen, szSrc, &nDstLen, szDst);
	
	// 3. CString 변환
	{
		UCHAR* sz_Tmp = new UCHAR[nDstLen+2];
		ZeroMemory(sz_Tmp, nDstLen+1);
		sz_Tmp[nDstLen+1] = '\0';

		
		for(UINT nCnt = 0; nCnt < nDstLen; nCnt++)
		{			
			sz_Tmp[nCnt] = szDst[nCnt];			
		}		
	 	strDst.Format(_T("%s"), (TCHAR*)sz_Tmp);
		delete []sz_Tmp;
	}

	// 4. 메모리 해제
	delete []szKey;
	delete []szSrc;
	delete []szDst;
	// Simple_Cipher
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	// Write_DES_File
//DEL 	CFileFind filefind;	
	UINT nOpenFlags = CFile::modeWrite|CFile::modeCreate|CFile::typeBinary;
//DEL 	if(TRUE == filefind.FindFile(strFile))
//DEL 	{
//DEL 		nOpenFlags ^= CFile::modeCreate;
//DEL 	}
	
	CFile file;
	CFileException ex;
 	BOOL bIsOpen = file.Open(strFile, nOpenFlags, &ex);
	if(FALSE == bIsOpen)
	{
		TCHAR szMsg[1024];
		ex.GetErrorMessage(szMsg, 1024);
		TRACE(szMsg);
		return -1001;
	}

	file.SeekToEnd();
	file.Write((TCHAR*)strDst.GetBuffer(0), (strDst.GetLength())*sizeof(TCHAR));
	strDst.ReleaseBuffer();
	file.Close();
	// Write_DES_File
	/////////////////////////////////////////////////////////////////////////////////	


	return 0;
}

void CConnectSetupPage::OnChangeeORACLEID2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();
}

void CConnectSetupPage::OnChangeeORACLEPASSWORD2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();
}

void CConnectSetupPage::OnChangeeORACLESID2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();	
}
