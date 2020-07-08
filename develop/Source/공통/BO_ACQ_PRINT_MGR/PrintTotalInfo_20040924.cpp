// PrintTotalInfo.cpp: implementation of the CPrintTotalInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrintTotalInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrintTotalInfo::CPrintTotalInfo()
{
	// KOL.UDF.022 시큐어코딩 보완
	InitTotalInfo();
	m_nAcqType = -1;	
}

CPrintTotalInfo::~CPrintTotalInfo()
{

}

DOUBLE CPrintTotalInfo::GetTotalPrice()
{
	return m_fTotalPrice;
}

VOID CPrintTotalInfo::GetTotalPrice(CString &strTotalPrice)
{
	strTotalPrice.Format(_T("%.0f"), m_fTotalPrice);
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strTotalPrice, strTotalPrice);
}

DOUBLE CPrintTotalInfo::GetDiscTotalPrice()
{
	return m_fDiscTotalPrice;
}

VOID CPrintTotalInfo::GetDiscTotalPrice(CString &strDiscTotalPrice)
{
	strDiscTotalPrice.Format(_T("%.0f"), m_fDiscTotalPrice);
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strDiscTotalPrice, strDiscTotalPrice);
}

INT CPrintTotalInfo::GetTotalBookCnt()
{
	return m_nTotalBookCnt;
}

VOID CPrintTotalInfo::GetTotalBookCnt(CString &strTotalBookCnt)
{
	strTotalBookCnt.Format(_T("%d"), m_nTotalBookCnt);
}

INT CPrintTotalInfo::SetTotalInfo(CESL_DataMgr *pDM, INT nFirstIdx/*=-1*/, INT nLastIdx/*=-1*/)
{
	if(pDM == NULL) return -1;
	if(pDM->GetRowCount() == 0) return 0;

	if(!CheckTotalInfoAlias()) return -1;
	InitTotalInfo();

	CString strTmpData;
	if(nFirstIdx < 0) nFirstIdx = 0;
	if(nLastIdx < 0) nLastIdx = pDM->GetRowCount();
	for(INT i=nFirstIdx ; i<nLastIdx ; i++)
	{
		m_nTotalSpeciesCnt++;
		
		if(!m_strBookCntAlias.IsEmpty())
		{
			strTmpData = pDM->GetCellData(m_strBookCntAlias, i);
			m_nTotalBookCnt += _ttoi(strTmpData.GetBuffer(0));
		}
		else 
		{
			m_nTotalBookCnt++;
		}

		if(!m_strPriceAlias.IsEmpty())
		{
			strTmpData = pDM->GetCellData(m_strPriceAlias, i);
			m_fTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
		}
		if(!m_strDiscPriceAlias.IsEmpty())
		{
			strTmpData = pDM->GetCellData(m_strDiscPriceAlias, i);
			m_fDiscTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
		}
	}
	return 0;
}

INT CPrintTotalInfo::SetTotalInfo(CMapStringToString * pArrData, CString strDivAlias, CESL_DataMgr *pDM, INT nFirstIdx/*=-1*/, INT nLastIdx/*=-1*/)
{
	if(pDM == NULL) return -1;
	if(pDM->GetRowCount() == 0) return 0;
	
	if(!CheckTotalInfoAlias()) return -1;
	InitTotalInfo();
	
	CString strTmpData, strTmpValue;
	if(nFirstIdx < 0) nFirstIdx = 0;
	if(nLastIdx < 0) nLastIdx = pDM->GetRowCount();
	for(INT i=nFirstIdx ; i<nLastIdx ; i++)
	{
		strTmpData = pDM->GetCellData(strDivAlias, i);
		if(pArrData && pArrData->GetCount() > 0)
		{
			if(!pArrData->Lookup(strTmpData, strTmpValue))
				continue;
		}
		
		m_nTotalSpeciesCnt++;
		
		if(!m_strBookCntAlias.IsEmpty())
		{
			strTmpData = pDM->GetCellData(m_strBookCntAlias, i);
			m_nTotalBookCnt += _ttoi(strTmpData.GetBuffer(0));
		}
		else 
		{
			m_nTotalBookCnt++;
		}
		
		if(!m_strPriceAlias.IsEmpty())
		{
			strTmpData = pDM->GetCellData(m_strPriceAlias, i);
			m_fTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
		}
		if(!m_strDiscPriceAlias.IsEmpty())
		{
			strTmpData = pDM->GetCellData(m_strDiscPriceAlias, i);
			m_fDiscTotalPrice += (DOUBLE)atof(strTmpData.GetBuffer(0));
		}
	}
	return 0;
}

VOID CPrintTotalInfo::InitTotalInfo()
{
	m_fTotalPrice = 0.0;
	m_fDiscTotalPrice = 0.0;
	m_nTotalBookCnt = 0;
	m_nTotalSpeciesCnt = 0;
}

VOID CPrintTotalInfo::SetAliasInfo(CString strBookCntAlias, CString strPriceAlias, CString strDiscPriceAlias)
{
	m_strBookCntAlias = strBookCntAlias;
	m_strPriceAlias = strPriceAlias;
	m_strDiscPriceAlias = strDiscPriceAlias;
}

BOOL CPrintTotalInfo::CheckTotalInfoAlias()
{
	if(m_strPriceAlias.IsEmpty())
		return FALSE;
	return TRUE;
}
INT CPrintTotalInfo::GetTotalSpeciesCnt()
{
	return m_nTotalSpeciesCnt;
}

VOID CPrintTotalInfo::GetTotalSpeciesCnt(CString &strTotalSpeciesCnt)
{
	strTotalSpeciesCnt.Format(_T("%d"), m_nTotalSpeciesCnt);
}