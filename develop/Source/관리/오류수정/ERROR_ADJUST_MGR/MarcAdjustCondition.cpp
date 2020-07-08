// MarcAdjustCondition.cpp: implementation of the CMarcAdjustCondition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcAdjustCondition.h"
#include "CurrencyCodeMarcInfo.h"
#include "PubPlaceMarcInfo.h"
#include "CopyCodeMarcInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcAdjustCondition::CMarcAdjustCondition(CESL_Mgr * pMgr)
{
	m_ArrMarcInfo.RemoveAll();
	m_pParentMgr = pMgr;
}

CMarcAdjustCondition::~CMarcAdjustCondition()
{
	RemoveAll();
}

INT CMarcAdjustCondition::RemoveAll()
{
	CMarcInfo * pInfo = NULL;
	INT nCnt = m_ArrMarcInfo.GetSize();
	if(nCnt == 0) return 0;

	for(INT i=0 ; i<nCnt ; i++)
	{
		pInfo = m_ArrMarcInfo.GetAt(i);
		if(pInfo)
		{
			delete pInfo;
			pInfo = NULL;
		}
	}
	m_ArrMarcInfo.RemoveAll();
	return 0;
}

INT CMarcAdjustCondition::AddMarcInfo(CString strTag, CString strValue, MARC_ADJUST_TYPE nType)
{
	CMarcInfo * pInfo = NULL;
	if(strTag == _T("950$b")) pInfo = new CCurrencyCodeMarcInfo(m_pParentMgr);
	else if(strTag == _T("260$a")) pInfo = new CPubPlaceMarcInfo(m_pParentMgr);
	else if(strTag == _T("049")) pInfo = new CCopyCodeMarcInfo(m_pParentMgr);
	
	if(pInfo == NULL) return -1;
	if(strTag.IsEmpty() || nType == MARC_ADJUST_TYPE_INVALID) return -1;
	
	pInfo->SetMarcTag(strTag);	pInfo->SetMarcInfo(strValue);	pInfo->SetType(nType);

	m_ArrMarcInfo.Add(pInfo);

	return 0;	
}

VOID CMarcAdjustCondition::SetSpeciesKey(CString strData)
{
	CMarcInfo * pInfo = NULL;
	INT nCnt = m_ArrMarcInfo.GetSize();
	if(nCnt == 0) return ;
	for(INT i=0 ; i<nCnt ; i++)
	{
		pInfo = m_ArrMarcInfo.GetAt(i);
		pInfo->SetSpeciesKey(strData);
	}
}

INT CMarcAdjustCondition::Update(CESL_DataMgr *pDM, INT nIdx)
{
	if(pDM == NULL || nIdx < 0 || nIdx > pDM->GetRowCount()-1)
		return -1;
	CMarc marc;
	CString strQuery;
	CString strMarc = pDM->GetCellData(_T("BS_MARC"), nIdx);
	CString strSpeciesKey = pDM->GetCellData(_T("IBS_Á¾KEY"), nIdx);
	if(strSpeciesKey.IsEmpty() || strMarc.IsEmpty()) return 0;

	SetSpeciesKey(strSpeciesKey);
	INT ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	if(ids < 0) return -1;
	
	CMarcInfo * pInfo = NULL;
	INT nCnt = m_ArrMarcInfo.GetSize();
	if(nCnt == 0) return 0;
	for(INT i=0 ; i<nCnt ; i++)
	{
		pInfo = m_ArrMarcInfo.GetAt(i);
		ids = pInfo->Update(pDM, &marc);
		if(ids < 0) return -1;
	}
	
	ids = m_pParentMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if(ids < 0) return -1;

	strMarc.Replace(_T("'"), _T("''"));
	strQuery.Format(_T("UPDATE BO_SPECIES_TBL SET MARC='%s' WHERE REC_KEY=%s;"),
		strMarc, strSpeciesKey);
	pDM->AddFrame(strQuery);
	return 0;
}
