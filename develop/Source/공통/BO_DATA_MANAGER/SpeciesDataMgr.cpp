// SpeciesDataMgr.cpp: implementation of the CSpeciesDataMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpeciesDataMgr.h"
#include "../INCLUDE/BO_ACQ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("단행종API"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpeciesDataMgr::CSpeciesDataMgr(CESL_Mgr* pParent /*=NULL*/)
{
	m_pParentMgr = pParent;
	m_pSpeciesDM = NULL;
	m_bDMCreate = FALSE;
	m_strSpeciesKey = _T("");

	nIndexSpeciesRelation = 0;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_형식구분_코드");			nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_업무구분");				nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_보조등록구분_코드");		nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_제어자료구분");			nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_분류표구분");				nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_자료구분_코드");			nIndexSpeciesRelation++;
	IndexSpeciesRelation_SPECIES[nIndexSpeciesRelation] = _T("BS_비고");					nIndexSpeciesRelation++;
	
}

CSpeciesDataMgr::~CSpeciesDataMgr()
{
	if (m_pSpeciesDM && m_bDMCreate) 
	{
		delete m_pSpeciesDM;
		m_pSpeciesDM = NULL;
	}
}

//종Key 생성.
INT CSpeciesDataMgr::CreateRecKey(CString &strSpecieskey, CString &strAccukey, INT nSpeciesType)
{
	INT ids;
	ids = m_pSpeciesDM->MakeRecKey(strSpecieskey); 
	if(ids < 0) return -1;
	m_strSpeciesKey = strSpecieskey;

	SPECIES_TYPE speciesType = SPECIES_TYPE_INVALID;
	speciesType = (SPECIES_TYPE)nSpeciesType;
	if(speciesType == BO_BASIC || speciesType == BO_REG_INVALID) strAccukey = _T("-1");
	else 
	{
		ids = m_pSpeciesDM->MakeRecKey(strAccukey); 
		if(ids < 0) return -1;
	}
	return 0;
}

INT CSpeciesDataMgr::GetReckey(INT nRowIdx, CString strReckey)
{
	INT ids = -1;
	ids = m_pSpeciesDM->GetCellData(_T("BS_종KEY"), nRowIdx, strReckey);
	if(ids < 0) return -1;
	
	return 0;
}

// Default 값을 적용 시킨다.
INT CSpeciesDataMgr::SetDefaultValue(INT nRowIdx, INT nOpenMode, INT nSpeciesType)
{
	if(!m_pSpeciesDM) return -1;

	SPECIES_TYPE sType = SPECIES_TYPE_INVALID;
	sType = (SPECIES_TYPE)nSpeciesType;

	CString worker = m_pParentMgr->m_pInfo->USER_ID;
	if(nOpenMode > 0)
	{
		m_pSpeciesDM->SetCellData(_T("BS_입력일"), GetTodayDate(), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_작업상태구분"),_T("1"), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_복본여부"),_T("N"), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_목차공통정보KEY"), _T("-1"), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_작업자"), worker, nRowIdx);
		if(sType == BO_REG_INVALID || sType == BO_REG_PURCHASE || sType == BO_REG_DONATE)
			m_pSpeciesDM->SetCellData(_T("BS_등록인계일자"), GetTodayDate(), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_제어번호"), _T(""), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_복본여부"), _T(""), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_DDC분류기호"), _T(""), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_DDC버전"), _T(""), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_KDC분류기호"), _T(""), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_KDC버전"), _T(""), nRowIdx);
		m_pSpeciesDM->SetCellData(_T("BS_배가위치부호"), _T(""), nRowIdx);
	}	
	m_pSpeciesDM->SetCellData(_T("BS_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nRowIdx);
	
	return 0;
}

CString CSpeciesDataMgr::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CSpeciesDataMgr::INDEX_to_SPECIES(CIndexDataMgr *IndexMgr, INT nIndexRow, INT nSpeciesRow)
{
	INT ids;
	CString strAlias, strData;

	for (INT i = 0; i < nIndexSpeciesRelation; i++) {
		strAlias = IndexMgr->IndexSpeciesRelation_INDEX[i];
		ids = IndexMgr->m_pIndexDM->GetCellData(strAlias, nIndexRow, strData);
		ids = m_pSpeciesDM->SetCellData(IndexSpeciesRelation_SPECIES[i], strData, nSpeciesRow);
	}
	return 0;
}

INT CSpeciesDataMgr::SetDataMgr(CESL_DataMgr *pDM)
{
	if(m_pParentMgr == NULL) return -1;
	if(pDM == NULL)
	{
		m_pSpeciesDM = new CESL_DataMgr;
		if(m_pSpeciesDM == NULL) return -1;
		m_pSpeciesDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
		m_pSpeciesDM->InitDMPKFromFile(_T("DMUP_단행_종_색인정보"));
		m_pSpeciesDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
		m_bDMCreate = TRUE;
	}
	else
	{
		m_pSpeciesDM = pDM;
	}
	return 0;
}

//종에 설정될 이용제한 구분 코드 추출
INT CSpeciesDataMgr::Get_Priority_USELIMIT_CODE(CESL_DataMgr * pDM_BOOK, CESL_DataMgr * pDM_ACCU, CArray <CString, CString> RemoveAliasList,
												INT nRowIdx, INT mode /*=-1*/, CString strPrev_USELIMIT_CODE /*=_T("")*/)
{
	//mode 설명 -1 : 새로 종의 이용제한 구분을 선정하는 경우 1 : 기존 종의 이용제한 구분을 재설정 하는 경우
	CString strUSELIMIT_CODE, strTmpData, strDBFlag;
	strUSELIMIT_CODE.Empty();
	INT rowCnt = pDM_BOOK->GetRowCount();
	INT nPriority = -1;
	INT nMaxPriority = -1;
	if(mode > 0 && strPrev_USELIMIT_CODE.GetLength() > 0)
	{
		if(strPrev_USELIMIT_CODE == _T("GM")) nMaxPriority = 50;
		else if(strPrev_USELIMIT_CODE == _T("IZ")) nMaxPriority = 40;
		else if(strPrev_USELIMIT_CODE == _T("CA")) nMaxPriority = 30;
		else if(strPrev_USELIMIT_CODE == _T("CD")) nMaxPriority = 20;
		else if(strPrev_USELIMIT_CODE == _T("CL")) nMaxPriority = 10;
	}

	for(INT i=0 ; i<rowCnt ; i++)
	{
		pDM_BOOK->GetCellData(_T("BB_DB_FLAG"), i, strDBFlag);
		if(strDBFlag == _T("D")) continue;
		pDM_BOOK->GetCellData(_T("BB_이용제한구분_코드"), i, strTmpData);
		if(strTmpData.IsEmpty()) continue;
		strTmpData.TrimLeft();	strTmpData.TrimRight();		strTmpData.MakeUpper();
		if(strTmpData == _T("GM")) nPriority = 50;
		else if(strTmpData == _T("IZ")) nPriority = 40;
		else if(strTmpData == _T("CA")) nPriority = 30;
		else if(strTmpData == _T("CD")) nPriority = 20;
		else if(strTmpData == _T("CL")) nPriority = 10;
		if(nMaxPriority < nPriority) nMaxPriority = nPriority;
	}
	if(nMaxPriority < 0) return 0;
	else if(nMaxPriority == 50) strUSELIMIT_CODE = _T("GM");
	else if(nMaxPriority == 40) strUSELIMIT_CODE = _T("IZ");
	else if(nMaxPriority == 30) strUSELIMIT_CODE = _T("CA");
	else if(nMaxPriority == 20) strUSELIMIT_CODE = _T("CD");
	else if(nMaxPriority == 10) strUSELIMIT_CODE = _T("CL");

	m_pSpeciesDM->SetCellData(_T("BS_이용제한구분_코드"), strUSELIMIT_CODE, nRowIdx);
	m_pSpeciesDM->SetCellData(_T("BS_종KEY"), m_strSpeciesKey, nRowIdx);
	m_pSpeciesDM->SetCellData(_T("BS_수입년도"), pDM_ACCU->GetCellData(_T("BP_수입년도"), nRowIdx), nRowIdx);
	//주제구분설정
	m_pSpeciesDM->SetCellData(_T("BS_주제구분_코드"), pDM_ACCU->GetCellData(_T("BP_주제구분_코드"), nRowIdx), nRowIdx);
	RemoveAliasList.RemoveAll();
	m_pSpeciesDM->InitDBFieldData();
	m_pSpeciesDM->MakeQueryByDM(RemoveAliasList, m_pSpeciesDM, _T("BO_SPECIES_TBL"), nRowIdx, m_pSpeciesDM);

	return 0;
}