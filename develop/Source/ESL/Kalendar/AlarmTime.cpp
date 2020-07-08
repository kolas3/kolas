// AlarmTime.cpp: implementation of the CAlarmTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kalendar.h"
#include "AlarmTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlarmTime::CAlarmTime()
{
	// KOL.UDF.022
	m_uYear = 0;
	m_uMonth = 0; 	
	m_uDay = 0;
	m_uHour = 0;
	m_uMinute = 0;
}

CAlarmTime::~CAlarmTime()
{

}

BOOL CAlarmTime::SetTime(UINT uYear, UINT uMonth, UINT uDay, UINT uHour, UINT uMinute)
{
	BOOL Chk = ChkDay(uYear, uMonth,uDay,uHour,uMinute);	
	if(FALSE==Chk)
		return FALSE;

	m_uYear = uYear;
	m_uMonth = uMonth;
	m_uDay = uDay;
	m_uHour = uHour;
	m_uMinute = uMinute;
	return TRUE;
}

CTime CAlarmTime::GetTime() const
{
	CTime ReturnTime(m_uYear, m_uMonth, m_uDay, m_uHour, m_uMinute, 0);
	return ReturnTime;
}

BOOL CAlarmTime::IsSameTime(UINT uYear, UINT uMonth, UINT uDay) const
{
	if(uYear != m_uYear)
		return FALSE;
	if(uMonth != m_uMonth)
		return FALSE;
	if(uDay != m_uDay)
		return FALSE;	

	return TRUE;
}

CString CAlarmTime::GetStringTime()
{
	CString stReturn;	
	stReturn.Format(_T("%d"), m_uYear);	
	stReturn += GetConvertUINTToString(m_uMonth);
	stReturn += GetConvertUINTToString(m_uDay);
	stReturn += GetConvertUINTToString(m_uHour);
	stReturn += GetConvertUINTToString(m_uMinute);	

	return stReturn;
}

// "11"은 11으로 "1"은 01로
CString CAlarmTime::GetConvertUINTToString(UINT uConvertNumber)
{	
	CString stReturn;
	TCHAR* szTemp;
	stReturn.Format(_T("%d"), uConvertNumber);	
	szTemp = (TCHAR*)(LPCTSTR)stReturn;	
	if(10 > _ttoi(szTemp))
	{
		stReturn = _T("0") + stReturn;
	}
	return stReturn;
}

BOOL CAlarmTime::SetTime(CString stTime)
{
	CString stTemp = "";

	UINT uYear = GetConvertStringToUINT(stTime, 4);
	stTime = stTime.Mid(4);
	UINT uMonth = GetConvertStringToUINT(stTime, 2);
	stTime = stTime.Mid(2);
	UINT uDay = GetConvertStringToUINT(stTime, 2);
	stTime = stTime.Mid(2);	
	UINT uHour = GetConvertStringToUINT(stTime, 2);
	stTime = stTime.Mid(2);
	UINT uMinute = GetConvertStringToUINT(stTime, 2);

	return SetTime(uYear, uMonth, uDay, uHour, uMinute);
}

UINT CAlarmTime::GetConvertStringToUINT(CString st, UINT uMultiplication)
{
	CString stTemp = "";
	TCHAR*szTemp;
	UINT uMax = uMultiplication;
	if(uMultiplication>(UINT)st.GetLength())
		uMax = st.GetLength();
	for(UINT i=0;i<uMax;i++)
	{
		stTemp += st.GetAt(i);		
	}
	szTemp = LPTSTR(LPCTSTR(stTemp));
	int uReturn = _ttoi(szTemp);
	return uReturn;
}
