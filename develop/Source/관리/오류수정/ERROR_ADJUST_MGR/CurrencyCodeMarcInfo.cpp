// CurrencyCodeMarcInfo.cpp: implementation of the CCurrencyCodeMarcInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CurrencyCodeMarcInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCurrencyCodeMarcInfo::CCurrencyCodeMarcInfo(CESL_Mgr * pParentMgr)
	:CMarcInfo(pParentMgr)
{

}

CCurrencyCodeMarcInfo::~CCurrencyCodeMarcInfo()
{

}

INT CCurrencyCodeMarcInfo::Update(CESL_DataMgr *pDM, CMarc *pMarc)
{
	if(m_pParentMgr == NULL || pDM == NULL || pMarc == NULL || m_strSpeciesKey.IsEmpty())
		return -1;

	CString strTmpData, strPrice, strCurrencyCode, strQuery;
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$b"), strTmpData);
	
	INT ids = Parse950_bTag(strTmpData, strPrice, strCurrencyCode);
	strCurrencyCode = m_strMarcInfo;
	strTmpData = strCurrencyCode + strPrice;
	if(!strTmpData.IsEmpty())
	{
		m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("950$b"));
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("950$b"), strTmpData);
	}

	strQuery.Format(
		_T("UPDATE BO_PURCHASEINFO_TBL SET CURRENCY_CODE='%s' WHERE SPECIES_KEY=%s;"),
		m_strMarcInfo, m_strSpeciesKey);
	pDM->AddFrame(strQuery);
	strQuery.Format(
		_T("UPDATE BO_DONINFO_TBL SET CURRENCY_CODE='%s' WHERE SPECIES_KEY=%s;"),
		m_strMarcInfo, m_strSpeciesKey);
	pDM->AddFrame(strQuery);
	return 0;
}

INT CCurrencyCodeMarcInfo::Parse950_bTag(CString strSource, CString &strPrice, CString &strCurrencyCode)
{
	INT nSize = strSource.GetLength();
	TCHAR ch;
	TCHAR szBufDigit[1024];
	INT nBufDigitSize = 0;
	TCHAR szBufNotDigit[1024];
	INT nBufNotDigitSize = 0;
	for (INT i = 0; i < nSize; i++) {
		ch = strSource.GetAt(i);
		if (isdigit(ch)) 
		{
			if ((INT)ch > 0) szBufDigit[nBufDigitSize++] = ch;
		}
		else if(ch == '.')
			szBufDigit[nBufDigitSize++]  = ch;
		else 
		{
			if ((INT)ch > 0) szBufNotDigit[nBufNotDigitSize++] = ch;
		}
		
	}
	szBufDigit[nBufDigitSize] = '\0';
	strPrice.Format(_T("%s"), szBufDigit);
	strPrice.TrimLeft();		strPrice.TrimRight();
	CString strTmpData = strPrice.Left(strPrice.Find(_T(".")));
	if(strTmpData.GetLength() > 8) strTmpData = strPrice.Left(8);
	strPrice = strTmpData + strPrice.Mid(strPrice.Find(_T(".")));
	
	szBufNotDigit[nBufNotDigitSize] = '\0';
	strCurrencyCode.Format(_T("%s"), szBufNotDigit);
	strCurrencyCode.TrimLeft();			strCurrencyCode.TrimRight();
	strCurrencyCode = strCurrencyCode.Left(3);
	return 0;
}