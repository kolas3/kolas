// Data.cpp: implementation of the CData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kalendar.h"
#include "Data.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CData::CData()
{
	m_uYear = 0;
	m_uMonth = 0;
	m_uDay = 0;
	m_bUseAlarm = FALSE;
	m_stText = _T("");
}

CData::~CData()
{

}

void CData::GetDate(UINT *uYear, UINT *uMonth, UINT *uDay)
{
	if(NULL!=uYear)
		*uYear = m_uYear;
	if(NULL!=uMonth)
		*uMonth = m_uMonth;
	if(NULL!=uDay)
		*uDay = m_uDay;
}

void CData::SetDate(UINT uYear, UINT uMonth, UINT uDay)
{	
	m_uYear = uYear;
	m_uMonth = uMonth;
	m_uDay = uDay;
}

void CData::SetText(CString stText)
{
	m_stText = stText;
}

CString CData::GetText()
{
	return m_stText;
}

void CData::SetData(UINT uYear, UINT uMonth, UINT uDay, CString stText)
{
	SetDate(uYear, uMonth, uDay);
	SetText(stText);
}

void CData::GetData(UINT *uYear, UINT *uMonth, UINT *uDay, CString *strText)
{
	GetDate(uYear, uMonth, uDay);
	if(NULL!=strText)
		*strText = GetText();
}

BOOL CData::GetUseAlarm() const
{
	return m_bUseAlarm;
}

void CData::SetUseAlarm(BOOL bUseAlarm)
{
	m_bUseAlarm = bUseAlarm;
}
