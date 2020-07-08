// BO_PurchaseInfoData.cpp: implementation of the CBO_PurchaseInfoData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_PurchaseInfoData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_PurchaseInfoData::CBO_PurchaseInfoData(CESL_Mgr * pParentMgr) : CBO_BusinessData(pParentMgr)
{

}

CBO_PurchaseInfoData::~CBO_PurchaseInfoData()
{

}

INT CBO_PurchaseInfoData::ApplyDM(CESL_DataMgr * pDM, INT nIdx)
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
		if(strTmpData == _T("BP")) 
		{
			ids = ApplyDMOneCell(pDM, nIdx, i, FIELD_NAME);
			if(ids < 0) return -1;
		}
	}
	return 0;		
}

INT CBO_PurchaseInfoData::ApplyMarcToData(CMarc * pMarc)
{
	if(pMarc == NULL || m_pParentMgr == NULL || m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;
	
	// 가격 사항
	CString strPrice, strPriceCode, strData;
	INT ids;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$b"), strData);
	if(ids < 0) return -1;
	if (strData.GetLength() > 0) 
	{
		ids = GetDigitData(strData, strPrice);
		if(ids < 0) return -1;
		ids = GetNoDigitData(strData, strPriceCode);
		if(ids < 0) return -1;

		m_pDM->SetCellData(_T("BP_가격"), strPrice, 0);
		m_pDM->SetCellData(_T("BP_화폐구분"), strPriceCode.Left(3), 0);
	}	
	strData.Empty();
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$a"), strData);
	if(ids < 0) return -1;
	if(strData.GetLength() > 0)
	{
		m_pDM->SetCellData(_T("BP_가격성격"), strData, 0);
	}
	strData.Empty();
	ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$c"), strData);
	if(ids < 0) return -1;
	if(strData.GetLength() > 0)
	{
		m_pDM->SetCellData(_T("BP_가격잡정보"), strData, 0);
	}

	return 0;
}

INT CBO_PurchaseInfoData::ApplyDataToMarc(CMarc * pMarc)
{
	if(pMarc == NULL || m_pParentMgr == NULL || m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;
	
	// 가격
	INT ids;
	CString strPrice, strPriceCode, strPriceOther, strPriceCharacter, strData;
	m_pDM->GetCellData(_T("BP_가격"), 0, strPrice);
	m_pDM->GetCellData(_T("BP_화폐구분"), 0, strPriceCode);
	m_pDM->GetCellData(_T("BP_가격잡정보"), 0, strPriceOther);
	m_pDM->GetCellData(_T("BP_가격성격"), 0, strPriceCharacter);
	strData.Format(_T("%s%s"), strPriceCode, strPrice);
	if (strData.GetLength() > 0) 
		ids = m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("950$b"), strData, _T(""), MODIFY_ITEM); 
	else 
		ids = m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("950$b")); 
	if(ids < 0) return -1;

	if (strPriceOther.GetLength() > 0) 
		ids = m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("950$c"), strPriceOther, _T(""), MODIFY_ITEM); 
	else 
		ids = m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("950$c")); 
	if(ids < 0) return -1;

	if (strPriceCharacter.GetLength() > 0) 
		ids = m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("950$a"), strPriceCharacter, _T(""), MODIFY_ITEM);
	else 
		ids = m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("950$a"));
	if(ids < 0) return -1;

	return 0;
}