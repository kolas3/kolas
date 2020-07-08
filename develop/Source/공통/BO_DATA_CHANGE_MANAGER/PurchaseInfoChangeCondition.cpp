// PurchaseInfoChangeCondition.cpp: implementation of the CPurchaseInfoChangeCondition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PurchaseInfoChangeCondition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPurchaseInfoChangeCondition::CPurchaseInfoChangeCondition()
{
	m_bCurrencyCode = FALSE;
	m_bDiscountRate = FALSE;
	m_bExchangeRate = FALSE;
	m_bProvider = FALSE;
	m_bRemark = FALSE;

	// KOL.UDF.022 시큐어코딩 보완
	m_bBookCount = FALSE;
	m_bApplicant = FALSE;
}

CPurchaseInfoChangeCondition::~CPurchaseInfoChangeCondition()
{

}
