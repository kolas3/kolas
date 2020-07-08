// YonChorokSet.cpp: implementation of the CYonChorokSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YonChorokSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYonChorokSet::CYonChorokSet()
{
	m_nCurCnt= 0 ;
	m_nTotalCnt= 0 ;
	ClearChorok() ;
}

CYonChorokSet::~CYonChorokSet()
{
	m_nCurCnt= 0 ;
	m_nTotalCnt= 0 ;
	ClearChorok() ;
}

BOOL CYonChorokSet::Next()
{
	m_nCurCnt++ ;
	if( m_nCurCnt <= m_nTotalCnt )
		return TRUE ;
	else 
		return FALSE ;
}

VOID CYonChorokSet::Free()
{
	m_nCurCnt= 0 ;
	m_nTotalCnt= 0 ;
	ClearChorok() ;
}

CString CYonChorokSet::GetChorokText()
{
	return m_saChorokText.GetAt(m_nCurCnt-1) ;
}

CString CYonChorokSet::GetKisaNo()
{
	return m_saKisaNo.GetAt(m_nCurCnt-1) ;
}

CString CYonChorokSet::GetChorokStatus()
{
	return m_saChorokStatus.GetAt(m_nCurCnt-1) ;
}

CString CYonChorokSet::GetKisaStatus()
{
	return m_saKisaStatus.GetAt(m_nCurCnt-1) ;
}

CString CYonChorokSet::GetKisa()
{
	return m_saKisa.GetAt(m_nCurCnt-1) ;
}

VOID CYonChorokSet::SetChorok(CString sKisaNo, CString sChorokText, CString sChorokStatus, CString sKisaStatus, CString sKisa)
{
	m_saKisaNo.Add(sKisaNo) ;
	m_saChorokText.Add(sChorokText) ;
	m_saChorokStatus.Add(sChorokStatus) ;
	m_saKisaStatus.Add(sKisaStatus) ;
	m_saKisa.Add(sKisa) ;

	m_saReserved.Add(_T(""));

	m_nTotalCnt++ ;
}

INT CYonChorokSet::GetCount()
{
	return m_nTotalCnt ;
}

VOID CYonChorokSet::ClearChorok()
{
	m_saReserved.RemoveAll();
	m_saKisaNo.RemoveAll();
	m_saChorokText.RemoveAll();
	m_saChorokStatus.RemoveAll();
	m_saKisaStatus.RemoveAll();
	m_saKisa.RemoveAll();
}

VOID CYonChorokSet::SetReserved(CString sReserved)
{
	m_saReserved.SetAt(m_nCurCnt-1,sReserved) ;
}

CString CYonChorokSet::GetReserved()
{
	return m_saReserved.GetAt(m_nCurCnt-1) ;
}

VOID CYonChorokSet::GoFirst()
{
	m_nCurCnt= 0 ;
}
