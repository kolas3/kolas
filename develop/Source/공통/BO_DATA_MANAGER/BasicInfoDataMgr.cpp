// BasicInfoDataMgr.cpp: implementation of the CBasicInfoDataMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BasicInfoDataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicInfoDataMgr::CBasicInfoDataMgr(CESL_Mgr* pParent /*=NULL*/)
{
	m_pParentMgr = pParent;
	m_pBasicInfoDM = NULL;
	m_bDMCreate = FALSE;

	nAccuSpeciesRelation = 0;
	AccuSpeciesRelation_ACCU[nAccuSpeciesRelation] = _T("BP_수입년도");					nAccuSpeciesRelation++;
	AccuSpeciesRelation_ACCU[nAccuSpeciesRelation] = _T("BP_주제구분_코드");			nAccuSpeciesRelation++;


}

CBasicInfoDataMgr::~CBasicInfoDataMgr()
{
	if (m_pBasicInfoDM && m_bDMCreate) 
	{
		delete m_pBasicInfoDM;
		m_pBasicInfoDM = NULL;
	}
}

INT CBasicInfoDataMgr::SetDataMgr(CESL_DataMgr *pDM)
{
	if(m_pParentMgr == NULL) return -1;
	if(pDM == NULL)
	{
		m_pBasicInfoDM = new CESL_DataMgr;
		if(m_pBasicInfoDM == NULL) return -1;
		m_pBasicInfoDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
		m_pBasicInfoDM->InitDMPKFromFile(_T("DMUP_단행_종_기본정보"));
		m_pBasicInfoDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
		m_bDMCreate = TRUE;
	}
	else
	{
		m_pBasicInfoDM = pDM;
	}
	return 0;
}

INT CBasicInfoDataMgr::SetSpeciesKey(CString strSpeciesKey, INT nRowIdx)
{
	INT ids = -1;
	ids = m_pBasicInfoDM->SetCellData(_T("BP_종KEY"), strSpeciesKey, nRowIdx);
	if(ids < 0) return -1;

	return 0;
}

INT CBasicInfoDataMgr::ACCU_to_SPECIES(CStringArray &strAlias, INT nRowIdx)
{
	INT ids;
	CString strData, strTemp;

	for (INT i = 0; i < nAccuSpeciesRelation; i++) {
		strData = AccuSpeciesRelation_ACCU[i];
		ids = m_pBasicInfoDM->GetCellData(strData, nRowIdx, strTemp);
		if(ids < 0) return -1;
		strAlias.Add((LPCTSTR)strTemp);
	}

	return 0;
}


