// BO_SpeciesData.cpp: implementation of the CBO_SpeciesData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_SpeciesData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_SpeciesData::CBO_SpeciesData(CESL_Mgr * pParentMgr) : CBO_BusinessData(pParentMgr)
{

}

CBO_SpeciesData::~CBO_SpeciesData()
{

}

INT CBO_SpeciesData::ApplyDM(CESL_DataMgr * pDM, INT nIdx)
{
	if(m_pDM == NULL || pDM == NULL) return -1;
	if(nIdx >= pDM->GetRowCount()) return -1;
	
	if(nIdx < 0)
	{
		pDM->InsertRow(-1);
		nIdx = pDM->GetRowCount() - 1;
	}
	
	INT ids ;
	
	INT nColCnt = pDM->GetRefColumnCount();
	INT nPos;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strTmpData;
	for(INT i=0 ; i<nColCnt ; i++)
	{
		pDM->GetColumnRefInfo(i, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
		strTmpData = FIELD_ALIAS.Left(2);
		nPos = FIELD_NAME.Find(_T("."), 0);
		if(nPos >= 0) FIELD_NAME = FIELD_NAME.Mid(nPos+1);
		if(strTmpData == _T("BS")) 
		{
			ids = ApplyDMOneCell(pDM, nIdx, i, FIELD_NAME);
			if(ids < 0) return -1;
		}
	}
	return 0;	
}

INT CBO_SpeciesData::ApplyMarcToData(CMarc * pMarc)
{
	return 0;
}

INT CBO_SpeciesData::ApplyDataToMarc(CMarc * pMarc)
{
	return 0;
}