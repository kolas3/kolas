// DonateInfoChangeCondition.cpp: implementation of the CDonateInfoChangeCondition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DonateInfoChangeCondition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDonateInfoChangeCondition::CDonateInfoChangeCondition()
{
	m_bCurrencyCode = FALSE;  // 2005-02-22
	m_bDonator = FALSE;
	m_bRegYn = FALSE;
	m_bRemark = FALSE;
	m_nRegYn = -1;
}

CDonateInfoChangeCondition::~CDonateInfoChangeCondition()
{

}
