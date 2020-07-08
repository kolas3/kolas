// ESL_Information.cpp: implementation of the CESL_Information class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESL_Information.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"
#include "CommonDefine.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_Information::CESL_Information()
{
	pCodeMgr = NULL;
	pCodeMgr = new CESL_CodeMgr;

	pESLMgr = NULL;
	pESLMgr = new CESL_DataMgr;
    pESLDataVerifyMgr = NULL;
    pESLDataVerifyMgr = new CESL_DataMgr;

	m_pListToolBar = NULL;

	pMarcMgr = NULL;

	MODE = 0;

	m_pszBlank[0] = '\0';
	TITLE = _T("");
	pIndexTool = NULL;

	SMS_USE_MODE			= MNG_INVALID;
	MONETA_USE_MODE			= MNG_INVALID;
	LOAN_USER_SHARE_MODE	= MNG_INVALID;
	LOAN_INFO_SHARE_MODE	= MNG_INVALID;
	SEARCH_INDEX_TYPE		= MNG_INVALID;
	SYSTEM_CLASS			= SYSTEM_INVALID;
	KIIUP_VERSION			= KIIUP_INVALID;

	m_pMain = NULL;
	m_bIsConnectUnityLoanService = TRUE;

	m_bLibSecuUse = FALSE;
	CString strLibSecuHome;
	GetRegistryInfo(_T("Software\\IVS\\Lib-Secu"), _T("HOME"), strLibSecuHome);
	if(!strLibSecuHome.IsEmpty())
	{
		CString strLibSecuPath = strLibSecuHome + _T("exe\\LibSecu.exe");
		CFileFind ff;
		if(TRUE == ff.FindFile(strLibSecuPath))
		{
			m_bLibSecuUse = TRUE;
		}
	}

	m_bKLUse = FALSE;

	m_bCertifyUse = FALSE;
	m_nCertifyAgency = 0;
	m_bCivilNoSearch = FALSE;

	m_bUserPwEncryption = FALSE;
	m_bVisitorPwEncryption = FALSE;
	m_bCardPwEncryption = FALSE;
	m_nUserInfoRenewalDay = 0;
	m_bAgreeInfoRecordYN = FALSE;
	m_bNotifyExpirationYN = FALSE;
	
	m_bFurnishWishLimitYN = FALSE;
	m_nFurnishWishMaxCnt = 0;

	m_bFurnishLimitByTheWeek = FALSE; 

	m_nRegCodeLength = 0;
	m_bUserInfoExportYN = TRUE; 

	m_bRestictionToApply = FALSE;
}

CESL_Information::~CESL_Information()
{
	if (pCodeMgr) {
		delete pCodeMgr;
		pCodeMgr = NULL;
	}
	if (pESLMgr) {
		delete pESLMgr;
		pESLMgr = NULL;
	}
    if (pESLDataVerifyMgr) {
        delete pESLDataVerifyMgr;
		pESLDataVerifyMgr = NULL;
	}

	FreeMarcMgr();
	
	m_CertifyFailCodeDM.FreeData();
}

INT CESL_Information::InitMarcMgr(VOID)
{
EFS_BEGIN

	FreeMarcMgr();

	TCHAR szBuf[512];
	CString strBuf;
	CString strCmd, strData;

	CString strFilename = _T("..\\CFG\\KolasIII_MarcMgr.cfg");
	CStdioFile fd;
	if (fd.Open(strFilename, CStdioFile::modeRead | CStdioFile::shareDenyWrite | CFile::typeBinary) == 0) return 1;		// 마크 사용안하
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}

	BOOL bUseMarc = FALSE;
	CString strDefaultGroupName = _T("MONOGRAPH");
	BOOL bUseRelation = FALSE;

	INT pos;
	for (INT i = 0; TRUE; i++) {
		if (fd.ReadString(szBuf, 511) == NULL) break;
		strBuf.Format(_T("%s"), szBuf);
		strBuf.TrimLeft();			strBuf.TrimRight();
		strBuf.MakeUpper();
		if (strBuf.GetAt(0) == '#') continue;
		if (strBuf.GetAt(0) == '.') continue;

		pos = strBuf.Find('=', 0);
		if (pos == -1) continue;
		strCmd = strBuf.Left(pos);
		strCmd.TrimLeft();		strCmd.TrimRight();
		strData = strBuf.Mid(pos+1);
		strData.TrimLeft();		strData.TrimRight();

		if (strCmd == _T("USEMARC") && strData == _T("TRUE")) bUseMarc = TRUE;
		if (strCmd == _T("USEMARC") && strData == _T("FALSE")) bUseMarc = FALSE;

		if (strCmd == _T("DEFAULTGROUPNAME")) strDefaultGroupName = strData;

		if (strCmd == _T("USERELATION") && strData == _T("TRUE")) bUseRelation = TRUE;
		if (strCmd == _T("USERELATION") && strData == _T("FALSE")) bUseRelation = TRUE;
	}
	fd.Close();

	if (bUseMarc == FALSE) return 0;

	pMarcMgr = new CMarcMgr;
	if (!pMarcMgr) return -1;
	pMarcMgr->InitMarcMgr(strDefaultGroupName, bUseRelation);

	return 0;

EFS_END
return -1;

}

INT CESL_Information::FreeMarcMgr(VOID)
{
EFS_BEGIN

	if (pMarcMgr) {
		delete pMarcMgr;
		pMarcMgr = NULL;
	}
	return 0;

EFS_END
return -1;

}

INT CESL_Information::SaveDMReferenceToFile(CESL_DataMgr *pDM)
{
EFS_BEGIN

	if (!pDM) return -1;
	FILE *fd_cfg = NULL;
	fd_cfg = _tfopen(_T("..\\CFG\\Reference\\DM\\DMPK.CFG"), _T("wb"));
	if (fd_cfg == NULL) return -2;

	fputc(0xFF, fd_cfg);
	fputc(0xFE, fd_cfg);

	INT nCount = pDM->GetRowCount();
	DWORD *row;
	POSITION pos = pDM->data.GetHeadPosition();

	CString strFileName;
	CString strDMAlias = _T("");
	FILE * fd = NULL;
	CString strLine;
	for (INT i = 0; i < nCount; i++) {
		row = NULL;
		row = (DWORD*)pDM->data.GetNext(pos);
		if (strDMAlias.Compare((TCHAR*)row[1]) != 0) {
			strDMAlias.Format(_T("%s"), ConvertDWORDtoString(row[1]));
			if (fd) {
				fclose(fd);
				fd = NULL;
			}
			strFileName.Format(_T("..\\CFG\\Reference\\DM\\%s.DM"), ConvertDWORDtoString(row[0]));
			fd = _tfopen(strFileName, _T("wb"));
					
			fputc(0xFF, fd);
			fputc(0xFE, fd);
			
			if (&fd_cfg != NULL) _ftprintf(fd_cfg, _T("%s | %s|\r\n"), ConvertDWORDtoString(row[0]), ConvertDWORDtoString(row[1]));

			if (&fd != NULL){ 
				_ftprintf(fd, _T("#[Data Manager]%s ▶ %s\r\n"), ConvertDWORDtoString(row[0]), ConvertDWORDtoString(row[1]));
				_ftprintf(fd, _T("ALIAS=%s\r\n"), ConvertDWORDtoString(row[1]));
				_ftprintf(fd, _T("CONNECTION_INFO=%s\r\n"), ConvertDWORDtoString(row[2]));
				_ftprintf(fd, _T("TBL_NAME=%s\r\n"), ConvertDWORDtoString(row[3]));
				_ftprintf(fd, _T("CONDITION=%s\r\n"), ConvertDWORDtoString(row[4]));
				_ftprintf(fd, _T("EXTRA_CONDITION=%s\r\n"), ConvertDWORDtoString(row[5]));
				_ftprintf(fd, _T("#Field Alias|Field Name|Field Type|InitValue|Data Type|\r\n"));
			}
		}
		if (&fd != NULL) _ftprintf(fd, _T("%s|%s|%s|%s|%s|\r\n"), 
										ConvertDWORDtoString(row[6]), 
										ConvertDWORDtoString(row[7]), 
										ConvertDWORDtoString(row[8]), 
										ConvertDWORDtoString(row[9]), 
										ConvertDWORDtoString(row[10])
				);
	}
	if (&fd != NULL) {
		fclose(fd);
		fd = NULL;
	}
	if (fd_cfg) {
		fclose(fd_cfg);
		fd_cfg = NULL;
	}
	return 0;

EFS_END
return -1;

}

INT CESL_Information::SaveCMReferenceToFile(CESL_DataMgr *pDM)
{
EFS_BEGIN

	if (!pDM) return -1;
	INT nCount = pDM->GetRowCount();
	DWORD *row;
	POSITION pos = pDM->data.GetHeadPosition();

	CString strFileName;
	CString strCMAlias = _T("");
	FILE * fd = NULL;
	for (INT i = 0; i < nCount; i++) {
		row = NULL;
		row = (DWORD*)pDM->data.GetNext(pos);
		if (strCMAlias.Compare((TCHAR*)row[1]) != 0) {
			strCMAlias.Format(_T("%s"), ConvertDWORDtoString(row[1]));
			if (fd) {
				fclose(fd);
				fd = NULL;
			}
			strFileName.Format(_T("..\\CFG\\Reference\\CM\\%s.CM"), ConvertDWORDtoString(row[0]));
			fd = _tfopen(strFileName, _T("wb"));
			
			fputc(0xFF, fd);
			fputc(0xFE, fd);

			if (&fd != NULL) {
				_ftprintf(fd, _T("#[Control Manager] %s ▶ %s\r\n"), ConvertDWORDtoString(row[0]), ConvertDWORDtoString(row[1]));
				_ftprintf(fd, _T("ALIAS=%s\r\n"), ConvertDWORDtoString(row[1]));
				_ftprintf(fd, _T("#CTRL ALIAS|CTRL TYPE|CTRL ID|CTRL STYLE|INIT VALUE|LIST DATA|LINK DATA|VALIDATION|REQUIRE CHECK|\r\n"));
			}
		}
		if (fd) _ftprintf(fd, _T("%s|%s|%s|%s|%s|%s|%s|%s|%s|\r\n"), 
													ConvertDWORDtoString(row[2]), 
													ConvertDWORDtoString(row[3]), 
													ConvertDWORDtoString(row[4]), 
													ConvertDWORDtoString(row[5]), 
													ConvertDWORDtoString(row[6]), 
													ConvertDWORDtoString(row[7]), 
													ConvertDWORDtoString(row[8]), 
													ConvertDWORDtoString(row[9]), 
													ConvertDWORDtoString(row[10])
				);
	}
	if (fd) {
		fclose(fd);
		fd = NULL;
	}
	return 0;

EFS_END
return -1;

}

TCHAR* CESL_Information::ConvertDWORDtoString(DWORD Address)
{
	if (Address == 0) return m_pszBlank;
	return ((TCHAR*)Address);
}

INT CESL_Information::SaveSMReferenceToFile(CESL_DataMgr *pDM)
{
EFS_BEGIN

	if (!pDM) return -1;
	INT nCount = pDM->GetRowCount();
	DWORD *row;
	POSITION pos = pDM->data.GetHeadPosition();

	CString strFileName;
	CString strCMAlias = _T("");
	strFileName.Format(_T("..\\CFG\\Reference\\SM.CFG"));
	FILE * fd = _tfopen(strFileName, _T("wb"));
	if (fd == NULL) return -1;
	
	fputc(0xFF, fd);
	fputc(0xFE, fd);

	if (&fd != NULL) _ftprintf(fd, _T("# SM_PK | ESL_MGR_ALIAS | ENGINEER | MGR_PK | MGR_TYPE |\r\n"));
	for (INT i = 0; i < nCount; i++) {
		row = NULL;
		row = (DWORD*)pDM->data.GetNext(pos);
		if (fd) _ftprintf(fd, _T("%s|%s|%s|%s|%s|\r\n"), 
										ConvertDWORDtoString(row[0]), 
										ConvertDWORDtoString(row[1]), 
										ConvertDWORDtoString(row[2]), 
										ConvertDWORDtoString(row[3]),
										ConvertDWORDtoString(row[4]) 
				);
	}
	if (fd) {
		fclose(fd);
		fd = NULL;
	}
	return 0;

EFS_END
return -1;
}

VOID CESL_Information::SetRFIDUseMode(INT mode)				{ RFID_USE_MODE = mode;			}
VOID CESL_Information::SetSMSUseMode(INT mode)				{ SMS_USE_MODE = mode;			}
VOID CESL_Information::SetMONETAUseMode(INT mode)			{ MONETA_USE_MODE = mode;		}
VOID CESL_Information::SetLoanUserShareMode(INT mode)		{ LOAN_USER_SHARE_MODE = mode;	}
VOID CESL_Information::SetLoanInfoShareMode(INT mode)		{ LOAN_INFO_SHARE_MODE = mode;	}
VOID CESL_Information::SetKIIUPClientVersion(TCHAR version)	{ KIIUP_VERSION = version;		}
VOID CESL_Information::SetSearchIndexType(INT mode)			{ SEARCH_INDEX_TYPE = mode;		}

INT		CESL_Information::GetRFIDUseMode()					{ return RFID_USE_MODE;			}
INT		CESL_Information::GetSMSUseMode()					{ return SMS_USE_MODE;			}
INT		CESL_Information::GetMONETAUseMode()				{ return MONETA_USE_MODE;		}
INT		CESL_Information::GetLoanUserShareMode()			{ return LOAN_USER_SHARE_MODE;	}
INT		CESL_Information::GetLoanInfoShareMode()			{ return LOAN_INFO_SHARE_MODE;	}
TCHAR	CESL_Information::GetKIIUPClientVersion()			{ return KIIUP_VERSION;			}
INT		CESL_Information::GetSearchIndexType()				{ return SEARCH_INDEX_TYPE;		}

CString CESL_Information::MakeRegNo( CString strRegCode, CString strRegFigure )
{
	CString strRegNo;
	INT nFigureLength;
	INT nCodeLength;
	CString strTemp;
	INT i;
	strTemp = _T("");

	strRegCode.Replace(_T(" "),_T(""));
	strRegFigure.Replace(_T(" "),_T(""));

	nFigureLength = strRegFigure.GetLength();
	nCodeLength = strRegCode.GetLength();
	for ( i = 0; i < nFigureLength; i++)
	{
		if (!_istdigit(strRegFigure.GetAt(i))) return _T("error");
	}

	for( i = 0 ; i < 12 - nCodeLength - nFigureLength; i++ )
	{
		strTemp += _T("0");
	}


	strRegNo.Format( _T("%s%s%s"), strRegCode, strTemp, strRegFigure );

	return strRegNo;

}

void CESL_Information::SetConnectUnityLoanService(BOOL bIsConnect)
{
	m_bIsConnectUnityLoanService = bIsConnect;

	if(m_pMain)
	{
		m_pMain->PostMessage(WM_USER + 1029, bIsConnect, NULL);
	}
}

BOOL CESL_Information::IsConnectUnityLoanService()
{
	return m_bIsConnectUnityLoanService;
}

INT CESL_Information::GetRegistryInfo(const CString &strPath, const CString &strName, CString &strGetValue, HKEY _hKey)
{
	LONG    ids;
	HKEY    hKey;
	DWORD   dwReserved, dwType, dwSize = 0;
	TCHAR*    pszValue = NULL;
	dwReserved = 0;
	ids = RegOpenKey(_hKey, strPath, &hKey);
 	if(ERROR_SUCCESS == ids)
 	{
 		ids = RegQueryValueEx (hKey, strName, NULL, &dwType, (UCHAR*)pszValue, &dwSize);
 	}
 	if(ERROR_SUCCESS == ids)
 	{
 		pszValue = new TCHAR[dwSize];
 		ids = RegQueryValueEx (hKey, strName, NULL, &dwType, (UCHAR*)pszValue, &dwSize);
 	}
 	RegCloseKey(hKey);
 	if(NULL != pszValue)
 	{
 		strGetValue.Format(_T("%s"), pszValue);
 		delete []pszValue;
 		pszValue = NULL;
 	} 
 	return ids;
}

INT CESL_Information::SetRegistryInfo(const CString &strPath, const CString &strName, CString &strSetValue, HKEY _hKey)
{
	HKEY hKey = NULL; 
	LONG lResult = RegOpenKeyEx(_hKey, strPath, 0, KEY_ALL_ACCESS, &hKey); 
	if(ERROR_SUCCESS != lResult)
	{
		lResult = RegCreateKey(_hKey, strPath, &hKey);
		if(ERROR_SUCCESS != lResult) return -5;
	}
	DWORD dwCount = (sizeof(TCHAR)*(strSetValue.GetLength()+1));
	TCHAR* pszValue = new TCHAR [dwCount];
	ZeroMemory(pszValue, dwCount);
	_tcscpy(pszValue, strSetValue.GetBuffer(0));
	strSetValue.ReleaseBuffer();
	lResult = RegSetValueEx(hKey, strName, NULL, REG_SZ, (LPBYTE)pszValue, dwCount);
	delete []pszValue;
	pszValue = NULL;
	if(ERROR_SUCCESS != lResult) return -6;
	RegCloseKey(hKey);
	return 0;
}

void CESL_Information::GetCertifyFailCodeToDesc(CString strAgency, CString strCode, CString &strDescription, CString &strAction)
{
	CString strGetData;
	for(int i = 0; i < m_CertifyFailCodeDM.GetRowCount(); i++)
	{
		m_CertifyFailCodeDM.GetCellData(i, 0, strGetData);
		if(strGetData == strAgency)
		{
			m_CertifyFailCodeDM.GetCellData(i, 1, strGetData);
			if(strGetData == strCode)
			{
				m_CertifyFailCodeDM.GetCellData(i, 2, strDescription);
				m_CertifyFailCodeDM.GetCellData(i, 3, strAction);
				strAction.Replace(_T("\n"), _T("\r\n"));
				break;
			}
		}
	}
}