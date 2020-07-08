// Memo.cpp: implementation of the CMemo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kalendar.h"
#include "Memo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemo::CMemo()
{
	// KOL.UDF.022
	m_stText = _T("");
	m_uYear = 0;
	m_uMonth = 0;
	m_uDay = 0;
}

CMemo::~CMemo()
{

}

void CMemo::GetDate(UINT *uYear, UINT *uMonth, UINT *uDay)
{
	*uYear = m_uYear;
	*uMonth = m_uMonth;
	*uDay = m_uDay;
}

void CMemo::SetDate(UINT uYear, UINT uMonth, UINT uDay)
{
	m_uYear = uYear;
	m_uMonth = uMonth;
	m_uDay = uDay;
}

void CMemo::SetText(CString stText)
{
	m_stText = stText;
}

CString CMemo::GetText()
{
	return m_stText;
}

void CMemo::SetMemo(UINT uYear, UINT uMonth, UINT uDay, CString stText)
{
	SetDate(uYear, uMonth, uDay);
	SetText(stText);
}
