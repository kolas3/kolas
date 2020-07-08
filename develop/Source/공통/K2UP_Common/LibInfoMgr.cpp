// LibInfoMgr.cpp: implementation of the CLibInfoMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LibInfoMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLibInfoMgr::CLibInfoMgr(CESL_Mgr * pMgr)
{
	m_pMgr = pMgr;
	m_pDM = new CESL_DataMgr;
	m_pDM->SetCONNECTION_INFO(m_pMgr->CONNECTION_INFO);
	m_pDM->InitDMPKFromFile(_T("DM_도서관정보"));
	m_pDM->SetMarcMgr(m_pMgr->m_pInfo->pMarcMgr);
}

CLibInfoMgr::~CLibInfoMgr()
{
	if(m_pDM)
	{
		delete m_pDM;
		m_pDM = NULL;
	}
}

CString CLibInfoMgr::FindDMPK(CString strDMAlias)
{
	CString strFileName;
	strFileName.Format(_T("..\\CFG\\Reference\\DM\\DMPK.CFG"));
	CStdioFile fd;
	if (fd.Open(strFileName, CStdioFile::modeRead | CStdioFile::shareDenyWrite | CFile::typeBinary) == 0) return -1;	// 정보파일을 찾을 수 없음
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	TCHAR szBuf[16000];
	CString strLine;
	INT pos;
	

	INT nStart = 0;
	CString PK, ALIAS;
	CString result = _T("");
	while(TRUE) {
		if (fd.ReadString(szBuf, 16000) == NULL) break;
		if (szBuf[0] == '#') continue;
		strLine.Format(_T("%s"), szBuf);
		strLine.TrimLeft();			strLine.TrimRight();
		
		nStart = 0;

		pos = strLine.Find('|', nStart);
		PK = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		ALIAS = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		PK.TrimLeft();		PK.TrimRight();
		ALIAS.TrimLeft();	ALIAS.TrimRight();

		if (ALIAS == strDMAlias) {
			result = PK;
			break;
		}
	}
	fd.Close();
	return result;
}

INT CLibInfoMgr::GetLibInfo(CString strUserId, CLibInfo *pLibInfo)
{
	//++2008.10.15 UPDATE BY PWR {{++
	// 통합시스템 적용
	CString strQuery;
	strQuery = _T("MANAGE_CODE=UDF_MANAGE_CODE");
//	CString strValue, strQuery, strManageCode;
//	strQuery = _T("SELECT MAX(VALUE_NAME) FROM ESL_MANAGE_TBL WHERE GROUP_2='기타' AND GROUP_3='공통' AND CLASS_ALIAS='관리구분별도서관설정'");
//	m_pDM->GetOneValueQuery(strQuery, strValue);
//	if(strValue == _T("N"))
//	{
//		strManageCode = _T("");
//	}
//	else if(strValue == _T("Y"))
//	{
//		strQuery.Format(_T("SELECT MAX(MANAGE_CODE) FROM MN_USER_TBL WHERE ID = '%s'"), strUserId);
//		m_pDM->GetOneValueQuery(strQuery, strManageCode);
//	}
//
//	if(strManageCode.IsEmpty())
//		strQuery.Format(_T("MANAGE_CODE IS NULL"));
//	else
//		strQuery.Format(_T("MANAGE_CODE = '%s'"), strManageCode);
	//--2008.10.15 UPDATE BY PWR --}}

	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0) return -1;

	if(m_pDM->GetRowCount() == 0) return -1;

	pLibInfo->SetAddress(m_pDM->GetCellData(_T("주소"), 0));
	pLibInfo->SetDirectorName(m_pDM->GetCellData(_T("도서관장"), 0));
	pLibInfo->SetEmail(m_pDM->GetCellData(_T("이메일"), 0));
	pLibInfo->SetFax(m_pDM->GetCellData(_T("팩스"), 0));
	pLibInfo->SetLibCode(m_pDM->GetCellData(_T("도서관코드"), 0));
	pLibInfo->SetLibName(m_pDM->GetCellData(_T("도서관명"), 0));
	pLibInfo->SetTel(m_pDM->GetCellData(_T("전화번호"), 0));
	pLibInfo->SetUrl(m_pDM->GetCellData(_T("홈페이지"), 0));
	pLibInfo->SetZipCode(m_pDM->GetCellData(_T("우편번호"), 0));
	return 0;
}
