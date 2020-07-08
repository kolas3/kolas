// ESL_ReferenceMgr.cpp: implementation of the CESL_ReferenceMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "ESL_ReferenceMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_ReferenceMgr::CESL_ReferenceMgr()
{
	m_pszBlank[0] = '\0';
}

CESL_ReferenceMgr::~CESL_ReferenceMgr()
{

}

VOID CESL_ReferenceMgr::SetCONNECTION_INFO(CString strCONNECTION_INFO)
{
	m_dm.SetCONNECTION_INFO(strCONNECTION_INFO);
	return;
}

// REFERENCE를 비교하여 변경된 SM 만 다운 받는 경우
INT CESL_ReferenceMgr::OPTIMIZEDownLoad(CESL_Information *pInfo, CProgressCtrl *pBar)
{
DWORD S, F, START, FINISH;
	INT ids;
	m_mapOLDSM.RemoveAll();
	m_mapNEWSM.RemoveAll();
	m_mapNEWSMPK.RemoveAll();

S = GetTickCount();

TRACE(_T("(Start) - LoadOLDSMVersion\n"));
START = GetTickCount();
	ids = LoadOLDSMVersion(&m_mapOLDSM);
FINISH = GetTickCount();
TRACE(_T("(Finish) - LoadOLDSMVersion [%d.%03d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);

TRACE(_T("(Start) - DownLoad_SM_Version\n"));
START = GetTickCount();
	m_dm.FreeData();
	// SM 버젼 정보를 내려 받는다.
	DownLoad_SM_Version(&m_mapOLDSM, &m_mapNEWSM, &m_mapNEWSMPK);
FINISH = GetTickCount();
TRACE(_T("(Finish) - DownLoad_SM_Version [%d.%03d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);

TRACE(_T("(Start) - DownLoad_DMPK_Info\n"));
START = GetTickCount();
	ids = DownLoad_DMPK_Info(&m_dm);
FINISH = GetTickCount();
TRACE(_T("(Finish) - DownLoad_DMPK_Info [%d.%03d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);

TRACE(_T("(Start) - SaveSMReferenceToFile\n"));
START = GetTickCount();
	// SM 정보와 SM 에 딸린 DM, CM 정보를 가져온다.
	//                                           0          1              2            3            4
	pInfo->pESLMgr->RestructDataManager(_T("select m.pk, m.esl_mgr_alias, m.engineer, ms.mgr_pk, ms.mgr_type from esl_mgr_tbl m, esl_mgr_sub_tbl ms where m.pk = ms.esl_mgr_pk"));
FINISH = GetTickCount();
TRACE(_T("(Finish) - SaveSMReferenceToFile [%d.%03d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);

	m_mapNEWDM.RemoveAll();
	m_mapNEWCM.RemoveAll();

TRACE(_T("(Start) - CheckNewDMCM\n"));
START = GetTickCount();
	ids = CheckNewDMCM(pInfo->pESLMgr, &m_mapNEWSMPK, &m_mapNEWDM, &m_mapNEWCM);
FINISH = GetTickCount();
TRACE(_T("(Finish) - CheckNewDMCM[%d.%03d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);

TRACE(_T("(Start) - DownLoad_DM\n"));
START = GetTickCount();
	ids = DownLoad_DM(&m_mapNEWDM);
FINISH = GetTickCount();
TRACE(_T("(Finish) - DownLoad_DM[%d.%03d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);

TRACE(_T("(Start) - DownLoad_CM\n"));
START = GetTickCount();
	ids = DownLoad_CM(&m_mapNEWCM);
FINISH = GetTickCount();
TRACE(_T("(Finish) - DownLoad_CM[%d.%03d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);

	// SM 정보와 SM 에 딸린 DM, CM 정보를 저장한다.
	pInfo->SaveSMReferenceToFile(pInfo->pESLMgr);

	// SM_VERSION.TMP 파일을 SM_VERSION.CFG로 복사한다.
	CopyFile(_T("..\\CFG\\REFERENCE\\SM_VERSION.TMP"), _T("..\\CFG\\REFERENCE\\SM_VERSION.CFG"), FALSE);

F = GetTickCount();
TRACE(_T("(Finish) ----------->OPTIMIZEDownLoad Total [%d.%03d sec]\n"), (F-S)/1000, (F-S)%1000);
	return 0;
}

INT CESL_ReferenceMgr::CheckNewDMCM(CESL_DataMgr *pDM, CMapStringToString *pNEWSMPKMAP, CMapStringToString *pNEWDMMAP, CMapStringToString *pNEWCMMAP)
{
	if (pDM == NULL) return -1;
	if (pNEWSMPKMAP == NULL) return -2;
	if (pNEWDMMAP == NULL) return -3;
	if (pNEWCMMAP == NULL) return -4;

	INT nCount = pDM->GetRowCount();
	DWORD *row;
	POSITION pos = pDM->data.GetHeadPosition();

	CString SM_PK = _T(""), SM_ALIAS;
	CString MGR_PK = _T(""), MGR_TYPE;
	for (INT i = 0; i < nCount; i++) {
		row = NULL;
		row = (DWORD*)pDM->data.GetNext(pos);
		if (row == NULL) continue;

		SM_PK = ConvertDWORDtoString(row[0]);
		MGR_PK = ConvertDWORDtoString(row[3]);
		MGR_TYPE = ConvertDWORDtoString(row[4]);
		if (pNEWSMPKMAP->Lookup(SM_PK, SM_ALIAS) == TRUE) {
			if (MGR_TYPE == _T("D")) {
				pNEWDMMAP->SetAt(MGR_PK, MGR_TYPE);
			}
			if (MGR_TYPE == _T("C")) {
				pNEWCMMAP->SetAt(MGR_PK, MGR_TYPE);
			}
		}
	}
	return 0;
}

// REFERENCE를 통채로 다운 받는 경우
INT CESL_ReferenceMgr::FULLDownLoad(CESL_Information *pInfo, CProgressCtrl *pBar)
{
	if (pInfo == NULL) return -1;
	
DWORD START, FINISH, S, F;
S = GetTickCount();
TRACE(_T("(Start) - Get SM Version info\n"));
START = GetTickCount();
	// SM 버젼 정보를 내려 받는다.
	DownLoad_SM_Version(NULL, NULL, NULL);
	m_dm.FreeData();
FINISH = GetTickCount();
TRACE(_T("(Finish) - Get SM Version info [%d.%d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);

TRACE(_T("(Start) - Get SM SUB Info\n"));
START = GetTickCount();
	// SM 정보와 SM 에 딸린 DM, CM 정보를 가져온다.
	pInfo->pESLMgr->RestructDataManager(_T("select m.pk, m.esl_mgr_alias, m.engineer, ms.mgr_pk, ms.mgr_type from esl_mgr_tbl m, esl_mgr_sub_tbl ms where m.pk = ms.esl_mgr_pk"));
	pInfo->SaveSMReferenceToFile(pInfo->pESLMgr);
FINISH = GetTickCount();
TRACE(_T("(Finish) - Get SM SUB Info [%d.%d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);

TRACE(_T("(Start) - Get DM Info\n"));
START = GetTickCount();
	// esl_db_mgr_tbl과 esl_db_mgr_sub_tbl을 Join한 것을 가져온다.
	m_dm.FreeData();
	m_dm.RestructDataManager(_T("select dm.pk, dm.db_mgr_alias, dm.connection_info, dm.tbl_name, dm.condition, dm.extra_condition, dms.field_alias, dms.field_name, dms.field_type, dms.init_value, dms.data_type from esl_db_mgr_tbl dm, esl_db_mgr_sub_tbl dms where dm.pk=dms.db_mgr_pk ORDER BY dm.db_mgr_alias"));
	if (pInfo->SaveDMReferenceToFile(&m_dm) < 0) return -1;
	if (pBar) pBar->StepIt();
FINISH = GetTickCount();
TRACE(_T("(Finish) - Get DM Info [%d.%d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
	
TRACE(_T("(Start) - Get CM Info\n"));
START = GetTickCount();
	// esl_ctrl_mgr_tbl과 esl_ctrl_mgr_sub_tbl을 Join한 것을 가져온다.
	m_dm.FreeData();
	m_dm.RestructDataManager(_T("select cm.pk, cm.ctrl_mgr_alias, cms.ctrl_alias, cms.ctrl_type, cms.ctrl_id, cms.ctrl_style, cms.init_value, cms.list_data, cms.link_data, cms.validation, cms.requirecheck from esl_ctrl_mgr_tbl cm, esl_ctrl_mgr_sub_tbl cms where cm.pk = cms.ctrl_mgr_pk ORDER BY cm.ctrl_mgr_alias"));
	if (pInfo->SaveCMReferenceToFile(&m_dm) < 0) return -1;
	if (pBar) pBar->StepIt();
FINISH = GetTickCount();
TRACE(_T("(Finish) - Get CM Info [%d.%d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);

	// SM_VERSION.TMP 파일을 SM_VERSION.CFG로 복사한다.
	CopyFile(_T("..\\CFG\\REFERENCE\\SM_VERSION.TMP"), _T("..\\CFG\\REFERENCE\\SM_VERSION.CFG"), FALSE);

F = GetTickCount();
TRACE(_T("(Finish) ----------->FULLDownLoad Total [%d.%03d sec]\n"), (F-S)/1000, (F-S)%1000);
		
	return 0;
}

INT CESL_ReferenceMgr::DownLoad_SM_Version(CMapStringToString *pOLDSMMAP, CMapStringToString *pNEWSMMAP, CMapStringToString *pNEWSMPKMAP)
{
	INT nRows;

	m_dm.RestructDataManager(_T("SELECT PK, ESL_MGR_ALIAS, VERSION FROM ESL_MGR_TBL"));
	nRows = m_dm.GetRowCount();
	FILE *fd = _tfopen(_T("..\\CFG\\REFERENCE\\SM_VERSION.TMP"), _T("wb"));
	if (fd == NULL) return -1;

		/*UNCHANGE*/fputc(0xFF, fd);
		/*UNCHANGE*/fputc(0xFE, fd);

	DWORD *row;
	POSITION pos = m_dm.data.GetHeadPosition();
	_ftprintf(fd, _T("# SM_PK | ESL_MGR_ALIAS | VERSION |\r\n"));
	CString SM_PK, SM_ALIAS, SM_VERSION, SM_OLD_VERSION;
	INT nSM_VERSION, nSM_OLD_VERSION;
	for (INT i = 0; i < nRows; i++) {
		row = NULL;
		row = (DWORD*)m_dm.data.GetNext(pos);
		if (row == NULL) continue;
		if (fd == NULL) continue;
		SM_PK = ConvertDWORDtoString(row[0]);
		SM_ALIAS = ConvertDWORDtoString(row[1]);
		SM_VERSION = ConvertDWORDtoString(row[2]);
		_ftprintf(fd, _T("%s|%s|%s|\r\n"), 
										ConvertDWORDtoString(row[0]), 
										SM_ALIAS,
										SM_VERSION 
				);
		if (pOLDSMMAP && pNEWSMMAP) {
			SM_OLD_VERSION = _T("");
			if (pOLDSMMAP->Lookup(SM_ALIAS, SM_OLD_VERSION) == TRUE) {
				nSM_VERSION = _ttoi(SM_VERSION);
				nSM_OLD_VERSION = _ttoi(SM_OLD_VERSION);
				if (nSM_VERSION > nSM_OLD_VERSION) {	// 버젼이 올라간경우 SM
					pNEWSMMAP->SetAt(SM_ALIAS, SM_VERSION);		
					pNEWSMPKMAP->SetAt(SM_PK, SM_ALIAS);
				}
			} else {	// 없는경우 -> 새로 추가된 SM
				pNEWSMMAP->SetAt(SM_ALIAS, SM_VERSION);	
				pNEWSMPKMAP->SetAt(SM_PK, SM_ALIAS);
			}
		}
	}
	if (fd) {
		fclose(fd);
		fd = NULL;
	}
	return 0;
}

VOID CESL_ReferenceMgr::AddToSMMap(CMapStringToString *pMAP, CString SM_ALIAS, CString SM_VERSION)
{
	if (pMAP == NULL) return;
	CString SM_TMP_VERSION;
	pMAP->Lookup(SM_ALIAS, SM_TMP_VERSION);
	return;
}

TCHAR* CESL_ReferenceMgr::ConvertDWORDtoString(DWORD Address)
{
	if (Address == 0) return m_pszBlank;
	return ((TCHAR*)Address);
}

INT CESL_ReferenceMgr::LoadOLDSMVersion(CMapStringToString *pMAP)
{
	if (pMAP == NULL) return -1;
	CString strLine, strSMPK, strSMAlias, strSMVersion;
	CStdioFile fd;
	if (fd.Open(_T("..\\CFG\\REFERENCE\\SM_VERSION.CFG"), CFile::modeRead | CFile::typeBinary) == FALSE) return -1;
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}

	INT nPos1, nPos2, nPos3;
	CString SM_PK, SM_ALIAS, SM_VERSION;
	while(TRUE) {
		if (fd.ReadString(strLine) != TRUE) break;
		if (strLine.GetLength() == 0) continue;
		if (strLine.GetAt(0) == '#') continue;
		if (strLine.GetAt(0) == '.') continue;
	
		nPos1 = strLine.Find('|', 0);
		if (nPos1 == -1) continue;
		nPos2 = strLine.Find('|', nPos1+1);
		if (nPos2 == -1) continue;
		nPos3 = strLine.Find('|', nPos2+1);
		if (nPos3 == -1) continue;

		SM_PK = strLine.Mid(0, nPos1);
		SM_ALIAS = strLine.Mid(nPos1+1, nPos2-nPos1-1);
		SM_ALIAS.TrimLeft();			SM_ALIAS.TrimRight();
		SM_VERSION = strLine.Mid(nPos2+1, nPos3-nPos2-1);
		SM_VERSION.TrimLeft();			SM_VERSION.TrimRight();

		pMAP->SetAt(SM_ALIAS, SM_VERSION);
	}

	fd.Close();
	return 0;
}

INT CESL_ReferenceMgr::DownLoad_DM(CMapStringToString *pNEWDMMAP)
{
	if (pNEWDMMAP == NULL) return -1;
	INT nCount = pNEWDMMAP->GetCount();
	if (nCount == 0) return 0;
	POSITION pos = pNEWDMMAP->GetStartPosition();
	CString DM_PK, DM_TYPE;
	if (pos == NULL) return 0;
	INT ids;
	for (INT i = 0; i < nCount; i++) {
		pNEWDMMAP->GetNextAssoc(pos, DM_PK, DM_TYPE );
		ids = DownLoad_DM(DM_PK);
	}
	TRACE(_T("DownLoad_DM [%d]\n"), nCount);	
	return 0;
}

INT CESL_ReferenceMgr::DownLoad_DM(CString DM_PK)
{
	m_dm.FreeData();
	CString strQuery;
	strQuery.Format(_T("select dm.pk, dm.db_mgr_alias, dm.connection_info, dm.tbl_name, dm.condition, dm.extra_condition, dms.field_alias, dms.field_name, dms.field_type, dms.init_value, dms.data_type from esl_db_mgr_tbl dm, esl_db_mgr_sub_tbl dms where dm.pk=dms.db_mgr_pk AND dm.pk = %s"), DM_PK);
	m_dm.RestructDataManager(strQuery);

	INT nCount = m_dm.GetRowCount();
	CString strLine, strFileName;
	FILE * fd = NULL;
	strFileName.Format(_T("..\\CFG\\Reference\\DM\\%s.DM"), DM_PK);
	fd = _tfopen(strFileName, _T("wb"));
	if (fd == NULL) return -1;

		/*UNCHANGE*/fputc(0xFF, fd);
		/*UNCHANGE*/fputc(0xFE, fd);

	DWORD *row;
	POSITION pos = m_dm.data.GetHeadPosition();
	for (INT i = 0; i < nCount; i++) {
		row = NULL;
		row = (DWORD*)m_dm.data.GetNext(pos);
		if (row == NULL) continue;
		if (i == 0) {
			_ftprintf(fd, _T("#[Data Manager]%s ▶ %s\r\n"), ConvertDWORDtoString(row[0]), ConvertDWORDtoString(row[1]));
			_ftprintf(fd, _T("ALIAS=%s\r\n"), ConvertDWORDtoString(row[1]));
			_ftprintf(fd, _T("CONNECTION_INFO=%s\r\n"), ConvertDWORDtoString(row[2]));
			_ftprintf(fd, _T("TBL_NAME=%s\r\n"), ConvertDWORDtoString(row[3]));
			_ftprintf(fd, _T("CONDITION=%s\r\n"), ConvertDWORDtoString(row[4]));
			_ftprintf(fd, _T("EXTRA_CONDITION=%s\r\n"), ConvertDWORDtoString(row[5]));
			_ftprintf(fd, _T("#Field Alias|Field Name|Field Type|InitValue|Data Type|\r\n"));
		}
		_ftprintf(fd, _T("%s|%s|%s|%s|%s|\r\n"), 
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
}

INT CESL_ReferenceMgr::DownLoad_CM(CMapStringToString *pNEWCMMAP)
{
	if (pNEWCMMAP == NULL) return -1;
	INT nCount = pNEWCMMAP->GetCount();
	if (nCount == 0) return 0;

	POSITION pos = pNEWCMMAP->GetStartPosition();
	CString CM_PK, CM_TYPE;
	if (pos == NULL) return 0;
	INT ids;
	for (INT i = 0; i < nCount; i++) {
		pNEWCMMAP->GetNextAssoc(pos, CM_PK, CM_TYPE );
		ids = DownLoad_CM(CM_PK);
	}
TRACE(_T("DownLoad_CM [%d]\n"), nCount);	
	return 0;
}

INT CESL_ReferenceMgr::DownLoad_CM(CString CM_PK)
{
	m_dm.FreeData();
	CString strQuery;
	strQuery.Format(_T("select cm.pk, cm.ctrl_mgr_alias, cms.ctrl_alias, cms.ctrl_type, cms.ctrl_id, cms.ctrl_style, cms.init_value, cms.list_data, cms.link_data, cms.validation, cms.requirecheck from esl_ctrl_mgr_tbl cm, esl_ctrl_mgr_sub_tbl cms where cm.pk = cms.ctrl_mgr_pk AND cm.pk = %s"), CM_PK);
	m_dm.RestructDataManager(strQuery);
	INT nCount = m_dm.GetRowCount();
	DWORD *row;

	CString strFileName;
	strFileName.Format(_T("..\\CFG\\Reference\\CM\\%s.CM"), CM_PK);
	FILE * fd = _tfopen(strFileName, _T("wb"));
	POSITION pos = m_dm.data.GetHeadPosition();
	if (fd == NULL) return 0;
	
		/*UNCHANGE*/fputc(0xFF, fd);
		/*UNCHANGE*/fputc(0xFE, fd);

	for (INT i = 0; i < nCount; i++) {
		row = NULL;
		row = (DWORD*)m_dm.data.GetNext(pos);
		if (row == NULL) continue;
		if (i == 0) {
			_ftprintf(fd, _T("#[Control Manager] %s ▶ %s\r\n"), ConvertDWORDtoString(row[0]), ConvertDWORDtoString(row[1]));
			_ftprintf(fd, _T("ALIAS=%s\r\n"), ConvertDWORDtoString(row[1]));
			_ftprintf(fd, _T("#CTRL ALIAS|CTRL TYPE|CTRL ID|CTRL STYLE|INIT VALUE|LIST DATA|LINK DATA|VALIDATION|REQUIRE CHECK|\r\n"));
		}
		_ftprintf(fd, _T("%s|%s|%s|%s|%s|%s|%s|%s|%s|\r\n"), 
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
}

INT CESL_ReferenceMgr::DownLoad_DMPK_Info(CESL_DataMgr *pDM)
{
	if (pDM == NULL) return -1;
	FILE *fd_cfg = _tfopen(_T("..\\CFG\\Reference\\DM\\DMPK.CFG"), _T("wb"));
	if (fd_cfg == NULL) return -2;
	
		/*UNCHANGE*/fputc(0xFF, fd_cfg);
		/*UNCHANGE*/fputc(0xFE, fd_cfg);

	pDM->RestructDataManager(_T("SELECT PK, DB_MGR_ALIAS FROM ESL_DB_MGR_TBL"));
	INT nRows = pDM->GetRowCount();

	DWORD *row;
	POSITION pos = pDM->data.GetHeadPosition();
	for (INT i = 0; i < nRows; i++) {
		row = (DWORD*)pDM->data.GetNext(pos);
		if (row == NULL) continue;
		_ftprintf(fd_cfg, _T("%s | %s|\r\n"), ConvertDWORDtoString(row[0]), ConvertDWORDtoString(row[1]));
	}
	fclose(fd_cfg);
	return 0;
}

