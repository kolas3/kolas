// PubPlaceMarcInfo.cpp: implementation of the CPubPlaceMarcInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PubPlaceMarcInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPubPlaceMarcInfo::CPubPlaceMarcInfo(CESL_Mgr * pParentMgr)
	:CMarcInfo(pParentMgr)
{

}

CPubPlaceMarcInfo::~CPubPlaceMarcInfo()
{

}

INT CPubPlaceMarcInfo::Update(CESL_DataMgr *pDM, CMarc *pMarc)
{
	if(m_pParentMgr == NULL || pDM == NULL || pMarc == NULL)
		return -1;
	if(m_strMarcInfo.IsEmpty()) return 0;

	m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("260$a"));
	m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("260$a"), m_strMarcInfo);
	
	return 0;
}
